#ifndef Album_H
#define Album_H

#include <string>
#include <vector>
#include "Music.h"
#include <queue>
#include "PlayingQueue.h"
#include <random>
#include <set>

class Album {
    public:
    std::string name;
    int time_to_listen;
    std::vector<Music> album_songs;
    int status_of_liked;
    PlayingQueue order_of_songs;


    Album(std::string init_name) : name(init_name) {}
    Album() {}
    void compute_listen_time();
    void shafl();
    void album_liked();
    void album_disliked();
    void album_reset();
};


#endif