#include "Music.h"


void Music::playMusic() {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(path_to_file)) {
        std::cerr << "Error loading WAV file" << std::endl;
        return;
    }


    sf::Sound sound;

    sound.setBuffer(buffer);
    sound.play();

    while (sound.getStatus() == sf::Sound::Playing) {
        sf::sleep(sf::milliseconds(100));
    }
}

bool Music::check_if_song_already_in_album(const std::string& album_name)  const {
    for(std::string album_entry : album_parents) {
        if(album_entry == album_name) {
            return true;
        }
    }
    return false;
}

void Music::song_liked() {
    status_of_liked = 1;
}

void Music::song_disliked() {
    status_of_liked = -1;
}

void Music::reset_like() {
    status_of_liked = 0;
}

void Music::compute_time_to_listen() {
    std::ifstream file(path_to_file, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file" << std::endl;
        return;
    }

    
    file.seekg(22); 
    uint16_t num_channels;
    file.read(reinterpret_cast<char*>(&num_channels), sizeof(num_channels));

    file.seekg(24); 
    uint32_t sample_rate;
    file.read(reinterpret_cast<char*>(&sample_rate), sizeof(sample_rate));

    file.seekg(34); 
    uint16_t bits_per_sample;
    file.read(reinterpret_cast<char*>(&bits_per_sample), sizeof(bits_per_sample));

    file.seekg(40); 
    uint32_t data_chunk_size;
    file.read(reinterpret_cast<char*>(&data_chunk_size), sizeof(data_chunk_size));

    
    uint32_t byte_rate = sample_rate * num_channels * (bits_per_sample / 8);
    uint32_t num_samples = data_chunk_size / (num_channels * (bits_per_sample / 8));
    double duration_seconds = static_cast<double>(num_samples) / sample_rate;

    time_to_listen = duration_seconds;

    std::cout << "Duration: " << duration_seconds << " seconds" << std::endl;
}