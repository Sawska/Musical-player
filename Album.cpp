#include "Album.h"

void Album::compute_listen_time() {
    time_to_listen = 0;
    for(int i =0;i<album_songs.size();i++) {
        time_to_listen += album_songs[i].time_to_listen;
    }
}

#include <algorithm>

void Album::shafl() {

    std::random_device rd;
    std::mt19937 gen(rd());


    std::vector<Music> songs;

    while (order_of_songs.size() > 0) {
        songs.push_back(order_of_songs.pop());
    }


    std::shuffle(songs.begin(), songs.end(), gen);


    for (const auto& song : songs) {
        order_of_songs.push(song);
    }
}


void Album::album_liked() {
    status_of_liked = 1;
}

void Album::album_disliked() {
    status_of_liked = -1;
}

void Album::album_reset() {
    status_of_liked = 0;
}

