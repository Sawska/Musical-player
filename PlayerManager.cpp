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
        "path_to_file VARCHAR(255), "
        "album_parents JSON)"
    );

    txn.commit();
}


void PlayerManager::create_table_album() {
    pqxx::work txn{connection};

    txn.exec(
        "CREATE TABLE IF NOT EXISTS ALBUMS ("
        "id SERIAL PRIMARY KEY, "
        "status_of_liked INT, "
        "time_to_listen INT, "
        "name VARCHAR(255), "
        "album_songs JSON)"
    );

    txn.commit();
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

    nlohmann::json album_parents_json = music.album_parents;

    txn.exec_params(
        "UPDATE MUSIC SET "
        "status_of_liked = $1, "
        "time_to_listen = $2, "
        "name = $3, "
        "author = $4, "
        "path_to_file = $5, "
        "album_parents = $6 "
        "WHERE name = $7 AND author = $8",
        music.status_of_liked,
        music.time_to_listen,
        music.name,
        music.author,
        music.path_to_file,
        album_parents_json.dump(),
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

    nlohmann::json album_parents_json = music.album_parents;

    txn.exec_params(
        "INSERT INTO MUSIC (status_of_liked, time_to_listen, name, author, path_to_file, album_parents) "
        "VALUES ($1, $2, $3, $4, $5, $6)",
        music.status_of_liked,
        music.time_to_listen,
        music.name,
        music.author,
        music.path_to_file,
        album_parents_json.dump()
    );

    txn.commit();   
}


void PlayerManager::load_songs() {
    pqxx::work txn{connection};

    for (auto [name, author, status_of_liked, time_to_listen, path_to_file, album_parents_json] : 
         txn.query<std::string, std::string, int, int, std::string, std::string>(
             "SELECT name, author, status_of_liked, time_to_listen, path_to_file, album_parents FROM MUSIC"))
    {
        Music music;
        music.name = name;
        music.author = author;
        music.status_of_liked = status_of_liked;
        music.time_to_listen = time_to_listen;
        music.path_to_file = path_to_file;
        music.album_parents = nlohmann::json::parse(album_parents_json).get<std::vector<std::string>>();

        this->music.push_back(music); 
    }
}

void PlayerManager::delete_music(const std::string& music_path) {
    if (std::filesystem::exists(music_path)) { 
        std::filesystem::remove(music_path);
    }
}

void PlayerManager::add_real_music_to_album(const Album& album) {
    pqxx::work txn{connection};

    for (const std::string& song_name : album.album_songs_for_db) {
        for (auto [name, author, status_of_liked, time_to_listen, path_to_file, album_parents_json] : 
             txn.query<std::string, std::string, int, int, std::string, std::string>(
                 "SELECT name, author, status_of_liked, time_to_listen, path_to_file, album_parents "
                 "FROM MUSIC WHERE name = $1", song_name))
        {
            Music music;
            music.name = name;
            music.author = author;
            music.status_of_liked = status_of_liked;
            music.time_to_listen = time_to_listen;
            music.path_to_file = path_to_file;
            music.album_parents = nlohmann::json::parse(album_parents_json).get<std::vector<std::string>>();

            const_cast<Album&>(album).album_songs.push_back(music); 
        }
    }

    txn.commit();
}

void PlayerManager::add_album(const Album& album) {
    pqxx::work txn{connection};

    nlohmann::json songs = album.album_songs_for_db;

    txn.exec_params(
        "INSERT INTO ALBUMS (name, time_to_listen, album_songs, status_of_liked) "
        "VALUES ($1, $2, $3, $4)",
        album.name,
        album.time_to_listen,
        songs.dump(),
        album.status_of_liked
    );

    txn.commit();
}

void PlayerManager::update_album(const Album& album) {
    pqxx::work txn{connection};   
    nlohmann::json songs = album.album_songs_for_db;

    txn.exec_params(
        "UPDATE ALBUMS SET "
        "time_to_listen = $1, "
        "album_songs = $2, "
        "status_of_liked = $3 "
        "WHERE name = $4",
        album.time_to_listen,
        songs.dump(),
        album.status_of_liked,
        album.name
    );

    txn.commit();
}

void PlayerManager::load_albums() {
    pqxx::work txn{connection};


    auto result = txn.exec(
        "SELECT name, status_of_liked, time_to_listen, album_songs FROM ALBUMS"
    );


    for (const auto& row : result) {

        std::string name = row["name"].c_str();
        int status_of_liked = row["status_of_liked"].as<int>();
        int time_to_listen = row["time_to_listen"].as<int>();
        std::string album_songs_json = row["album_songs"].c_str();


        std::vector<std::string> album_songs_for_db = nlohmann::json::parse(album_songs_json).get<std::vector<std::string>>();


        Album album(name);
        album.status_of_liked = status_of_liked;
        album.time_to_listen = time_to_listen;
        album.album_songs_for_db = album_songs_for_db;
        this->add_real_music_to_album(album);

        this->albums.push_back(album);
    }
}
