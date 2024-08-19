#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <string>
#include <vector>
#include "Music.h"
#include "Album.h"
#include "PlayingQueue.h"
#include <iostream>
#include <pqxx/pqxx>

    enum FilterType {
        LIKED = 1,
        DISLIKED = -1
    };
class PlayerManager {
public:
    void create_table_album_music();
    void add_music_to_album(const Album& album, const Music& music);

    void add_music(const Music& music);
    void remove_music_from_db(const std::string& music_name);
    void add_album(const Album& album);
    void remove_album_from_db(const std::string& album_name);
    void delete_album(const Album& album);
    void delete_music(const std::string& music_path);
    void load_songs();
    void remove_music_from_album(const Album& album,const Music& music);
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
    {
        create_table_album();
        create_table_music();
        create_table_album_music();
    }

    std::vector<Album> albums;
    std::vector<Music> music;
    PlayingQueue playing_queue;
private:
    pqxx::connection connection;
};

#endif
