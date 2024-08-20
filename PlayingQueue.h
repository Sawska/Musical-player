#ifndef PLAYING_QUEUE_H
#define PLAYING_QUEUE_H 

#include <queue>
#include <vector>
#include <random>
#include <algorithm>
#include "Music.h"

class PlayingQueue {
public:
    void merge_queues(PlayingQueue& queue);
    void push(const Music& music);
    void remove(const std::string& name);
    void add_to_start(const Music& music);
    void swap(const Music& first_music, const Music& second_music);
    int size() const;
    void shuffle();
    Music pop();
    std::queue<Music> queue;

private:
};

#endif
