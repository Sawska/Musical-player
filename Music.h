#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <vector>

class Music {
public:
    std::string name;
    int time_to_listen;
    std::string path_to_file;
    std::vector<std::string> album_parents;
    int status_of_liked;

    Music(std::string init_name, std::string init_path_to_file, int init_time_to_listen)
        : name(init_name), time_to_listen(init_time_to_listen), path_to_file(init_path_to_file) {}

    void compute_time_to_listen();
    void song_liked();
    void song_disliked();
    void reset_like();
    bool check_if_song_already_in_album();


private:
};

#endif
