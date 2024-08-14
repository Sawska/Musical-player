#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <string>
#include <vector>
#include "Music.h"
#include "Album.cpp"
#include "PlayingQueue.h"
#include <iostream>
#include <pqxx/pqxx>

class PlayerManager {
public:
    enum FilterType {
        LIKED = 1,
        DISLIKED = -1
    };

    void add_music(const Music& music);
    void remove_music_from_db(const std::string& music_name);
    void add_album(const Album& album);
    void remove_album_from_db(const std::string& album_name);
    void delete_album(const Album& album);
    void delete_music(const std::string& music_path);
    void load_songs();
    void load_albums();
    void update_song(const Music& music);
    void update_album(const Album& album);
    void create_table_music();
    void create_table_album();
    void add_real_music_to_album(const Album& album);
    std::vector<Music> load_filtered_song(FilterType filter);
    std::vector<Album> load_filtered_albums(FilterType filter);

    PlayerManager() 
        : connection("postgresql://music_player@localhost") 
    {}

private:
    std::vector<Album> albums;
    std::vector<Music> music;
    PlayingQueue playing_queue;
    pqxx::connection connection;
};

#endif
