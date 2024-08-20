#include "PlayerManager.h"
#include <nlohmann/json.hpp>



std::vector<Music> PlayerManager::load_filtered_song(FilterType filter) {
    std::vector<Music> filtered_copy;

    std::copy_if(music.begin(), music.end(), std::back_inserter(filtered_copy),
                 [filter](const Music& music_entry) {
                     return music_entry.status_of_liked == filter;
                 });

    return filtered_copy;
}


void PlayerManager::create_table_music() {
    pqxx::work txn{connection};

    txn.exec(
        "CREATE TABLE IF NOT EXISTS MUSIC ("
        "id SERIAL PRIMARY KEY, "
        "status_of_liked INT, "
        "time_to_listen INT, "
        "name VARCHAR(255), "
        "author VARCHAR(255), "
        "path_to_file VARCHAR(255))"
    );

    txn.commit();
}

Music PlayerManager::load_music(std::string name) {
    pqxx::work txn{connection};
    pqxx::result music_res = txn.exec_params(
        "SELECT name, author, status_of_liked, time_to_listen, path_to_file FROM MUSIC WHERE name = $1",
        name
    );

    Music music;
    
    if (!music_res.empty()) {
        music.name = music_res[0]["name"].as<std::string>();
        music.author = music_res[0]["author"].as<std::string>();
        music.status_of_liked = music_res[0]["status_of_liked"].as<int>();
        music.time_to_listen = music_res[0]["time_to_listen"].as<int>();
        music.path_to_file = music_res[0]["path_to_file"].as<std::string>();
    }

    return music;
}



void PlayerManager::create_table_album() {
    pqxx::work txn{connection};

    txn.exec(
        "CREATE TABLE IF NOT EXISTS ALBUMS ("
        "id SERIAL PRIMARY KEY, "
        "status_of_liked INT, "
        "time_to_listen INT, "
        "name VARCHAR(255))"
    );

    txn.commit();
}

void PlayerManager::create_table_album_music() {
    pqxx::work txn{connection};

    txn.exec(
        "CREATE TABLE IF NOT EXISTS ALBUM_MUSIC ("
        "album_id INT REFERENCES ALBUMS(id) ON DELETE CASCADE, "
        "music_id INT REFERENCES MUSIC(id) ON DELETE CASCADE, "
        "PRIMARY KEY (album_id, music_id))"
    );

    txn.commit();
}

void PlayerManager::add_music_to_album(const Album& album, const Music& music) {
    pqxx::work txn{connection};


    pqxx::result album_res = txn.exec_params(
        "SELECT id FROM ALBUM WHERE name = $1",
        album.name
    );

    if (album_res.empty()) {
        throw std::runtime_error("Album not found");
    }

    int album_id = album_res[0]["id"].as<int>();


    pqxx::result music_res = txn.exec_params(
        "SELECT id FROM MUSIC WHERE name = $1 AND author = $2",
        music.name,
        music.author
    );

    if (music_res.empty()) {
        throw std::runtime_error("Music not found");
    }

    int music_id = music_res[0]["id"].as<int>();


    txn.exec_params(
        "INSERT INTO ALBUM_MUSIC (album_id, music_id) VALUES ($1, $2)",
        album_id,
        music_id
    );

    txn.commit();
}

std::vector<std::string> PlayerManager::search_songs(std::string input) {
    std::vector<std::string> result;
    load_songs();
    for (const Music& music_entry : music) {
        if (music_entry.name.find(input) != std::string::npos) { 
            result.push_back(music_entry.name);
        }
    }
    return result;
}

std::vector<std::string> PlayerManager::search_albums(std::string input) {
    std::vector<std::string> result;
    load_songs();
    for (const Album& album : albums) {
        if (album.name.find(input) != std::string::npos) { 
            result.push_back(album.name);
        }
    }
    return result;
}

std::vector<std::string> PlayerManager::search(std::string input) {
    std::vector<std::string> result;
    

    std::vector<std::string> songs = search_songs(input);
    std::vector<std::string> albums = search_albums(input);
    
    result.insert(result.end(), songs.begin(), songs.end());
    result.insert(result.end(), albums.begin(), albums.end());
    
    return result;
}






std::vector<Album> PlayerManager::load_filtered_albums(FilterType filter) {
    std::vector<Album> filtered_copy;

    std::copy_if(albums.begin(), albums.end(), std::back_inserter(filtered_copy),
                 [filter](const Album& album_entry) {
                     return album_entry.status_of_liked == filter;
                 });
    
    return filtered_copy;    
}


void PlayerManager::update_song(const Music& music) {
    pqxx::work txn{connection};

    

    txn.exec_params(
        "UPDATE MUSIC SET "
        "status_of_liked = $1, "
        "time_to_listen = $2, "
        "name = $3, "
        "author = $4, "
        "path_to_file = $5, "
        "WHERE name = $6 AND author = $7",
        music.status_of_liked,
        music.time_to_listen,
        music.name,
        music.author,
        music.path_to_file,
        music.name,
        music.author
    );

    txn.commit();
}


void PlayerManager::delete_album(const Album& album) {
    for (const Music& music_entry : album.album_songs) {
        this->delete_music(music_entry.name);
    }

    remove_album_from_db(album.name);
}


void PlayerManager::remove_album_from_db(const std::string& album_name) {
    pqxx::work txn{connection};

    txn.exec_params(
        "DELETE FROM ALBUMS "
        "WHERE name = $1",
        album_name
    );

    txn.commit();
}


void PlayerManager::remove_music_from_db(const std::string& music_name) {
    pqxx::work txn{connection};

    txn.exec_params(
        "DELETE FROM MUSIC "
        "WHERE name = $1",
        music_name
    );

    txn.commit();
}


void PlayerManager::add_music(const Music& music) {
    pqxx::work txn{connection};


    txn.exec_params(
        "INSERT INTO MUSIC (status_of_liked, time_to_listen, name, author, path_to_file) "
        "VALUES ($1, $2, $3, $4, $5, $6)",
        music.status_of_liked,
        music.time_to_listen,
        music.name,
        music.author,
        music.path_to_file
    );

    txn.commit();   
}


void PlayerManager::load_songs() {
    pqxx::work txn{connection};

    pqxx::result result = txn.exec(
        "SELECT name, author, status_of_liked, time_to_listen, path_to_file FROM MUSIC"
    );

    for (const auto& row : result) {
        Music music;
        music.name = row["name"].as<std::string>();
        music.author = row["author"].as<std::string>();
        music.status_of_liked = row["status_of_liked"].as<int>();
        music.time_to_listen = row["time_to_listen"].as<int>();
        music.path_to_file = row["path_to_file"].as<std::string>();

        this->music.push_back(music);
    }

    txn.commit();
}

Album PlayerManager::load_album(std::string name) {
    pqxx::work txn{connection};

    pqxx::result result = txn.exec_params(
        "SELECT name, status_of_liked, time_to_listen FROM ALBUMS WHERE name = $1",
        name
    );

    Album album(name);
    
    if (!result.empty()) {
        album.name = result[0]["name"].as<std::string>();
        album.status_of_liked = result[0]["status_of_liked"].as<int>();
        album.time_to_listen = result[0]["time_to_listen"].as<int>();


        this->add_real_music_to_album(album);
    }

    return album;
}


void PlayerManager::remove_music_from_album(const Album& album, const Music& music) {
    pqxx::work txn{connection};

    
    int album_id;
    pqxx::result album_id_result = txn.exec_params(
        "SELECT id FROM ALBUMS WHERE name = $1", album.name);
    
    if (album_id_result.size() > 0) {
        album_id = album_id_result[0]["id"].as<int>();
    } else {
        std::cerr << "Album not found in the database." << std::endl;
        return;
    }

    
    int music_id;
    pqxx::result music_id_result = txn.exec_params(
        "SELECT id FROM MUSIC WHERE name = $1 AND author = $2", music.name, music.author);
    
    if (music_id_result.size() > 0) {
        music_id = music_id_result[0]["id"].as<int>();
    } else {
        std::cerr << "Music not found in the database." << std::endl;
        return;
    }


    txn.exec_params(
        "DELETE FROM ALBUM_MUSIC WHERE album_id = $1 AND music_id = $2",
        album_id,
        music_id
    );

    txn.commit();
}


void PlayerManager::delete_music(const std::string& music_path) {
    if (std::filesystem::exists(music_path)) { 
        std::filesystem::remove(music_path);
    }
}

void PlayerManager::add_real_music_to_album(const Album& album) {
    pqxx::work txn{connection};


    int album_id;
    pqxx::result album_id_result = txn.exec_params(
        "SELECT id FROM ALBUMS WHERE name = $1", album.name);
    
    if (album_id_result.size() > 0) {
        album_id = album_id_result[0]["id"].as<int>();
    } else {
        std::cerr << "Album not found in the database." << std::endl;
        return;
    }


    pqxx::result music_ids_result = txn.exec_params(
        "SELECT music_id FROM ALBUM_MUSIC WHERE album_id = $1", album_id);
    
    for (const auto& row : music_ids_result) {
        int music_id = row["music_id"].as<int>();

        
        pqxx::result music_result = txn.exec_params(
            "SELECT name, author, status_of_liked, time_to_listen, path_to_file "
            "FROM MUSIC WHERE id = $1", music_id);
        
        if (music_result.size() > 0) {
            const auto& music_row = music_result[0];

            Music music;
            music.name = music_row["name"].as<std::string>();
            music.author = music_row["author"].as<std::string>();
            music.status_of_liked = music_row["status_of_liked"].as<int>();
            music.time_to_listen = music_row["time_to_listen"].as<int>();
            music.path_to_file = music_row["path_to_file"].as<std::string>();

        
            const_cast<Album&>(album).album_songs.push_back(music);
        }
    }

    txn.commit();
}


void PlayerManager::add_album(const Album& album) {
    pqxx::work txn{connection};

    

    txn.exec_params(
        "INSERT INTO ALBUMS (name, time_to_listen,status_of_liked) "
        "VALUES ($1, $2, $3, $4)",
        album.name,
        album.time_to_listen,
        album.status_of_liked
    );

    txn.commit();
}

void PlayerManager::update_album(const Album& album) {
    pqxx::work txn{connection};   
    

    txn.exec_params(
        "UPDATE ALBUMS SET "
        "time_to_listen = $1, "
        "status_of_liked = $2 "
        "WHERE name = $3",
        album.time_to_listen,
        album.status_of_liked,
        album.name
    );

    txn.commit();
}

void PlayerManager::load_albums() {
    pqxx::work txn{connection};


    auto result = txn.exec(
        "SELECT name, status_of_liked, time_to_listen FROM ALBUMS"
    );


    for (const auto& row : result) {

        std::string name = row["name"].c_str();
        int status_of_liked = row["status_of_liked"].as<int>();
        int time_to_listen = row["time_to_listen"].as<int>();
        Album album(name);
        album.status_of_liked = status_of_liked;
        album.time_to_listen = time_to_listen;

        this->add_real_music_to_album(album);

        this->albums.push_back(album);
    }
}


