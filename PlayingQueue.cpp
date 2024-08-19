#include "PlayingQueue.h"

void PlayingQueue::push(const Music& music) {
    queue.push(music);
}

int PlayingQueue::size() const {
    return queue.size();
}

void PlayingQueue::shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Music> songs;


    while (!queue.empty()) {
        songs.push_back(queue.front());
        queue.pop();
    }


    std::shuffle(songs.begin(), songs.end(), gen);


    for (const auto& song : songs) {
        queue.push(song);
    }
}

Music PlayingQueue::pop() {
    if (!queue.empty()) {
        Music front_music = queue.front();
        queue.pop();
        return front_music;
    }

    throw std::out_of_range("Queue is empty");
}

void PlayingQueue::merge_queues(PlayingQueue& queue_copy) {
    while(queue_copy.size() > 0) {
        push(queue_copy.pop());
    }
}

void PlayingQueue::remove(const std::string& name) {
    std::queue<Music> copy;

    while(!queue.empty()) {
        Music music = queue.front();
        queue.pop();

        if(music.name != name) {
            copy.push(music);
        }
    }

    queue = copy;
}

void PlayingQueue::add_to_start(const Music& music) {
    std::queue<Music> temp_queue;
    temp_queue.push(music);

    while (!queue.empty()) {
        temp_queue.push(queue.front());
        queue.pop();
    }

    queue = temp_queue;
}

void PlayingQueue::swap(const Music& first_music, const Music& second_music) {
    std::queue<Music> temp_queue;

    while(!queue.empty()) {
        Music music = queue.front();
        queue.pop();

        if (music.name == first_music.name) {
            temp_queue.push(second_music);
        } else if (music.name == second_music.name) {
            temp_queue.push(first_music);
        } else {
            temp_queue.push(music);
        }
    }

    queue = temp_queue;
}
