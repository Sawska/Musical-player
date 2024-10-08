#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <SFML/Audio.hpp>


class Music {
public:
    std::string name;
    std::string author;
    int time_to_listen;
    std::string path_to_file;
    int status_of_liked;
    Music(std::string init_name, std::string init_path_to_file, int init_time_to_listen, std::string author_init)
        : name(init_name), time_to_listen(init_time_to_listen), path_to_file(init_path_to_file), author(author_init) {}

    Music() {}

    void compute_time_to_listen();
    void song_liked();
    void song_disliked();
    void reset_like();
    void playMusic();

private:
};

#endif
