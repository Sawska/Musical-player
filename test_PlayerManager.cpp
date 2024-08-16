#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <filesystem>
#include "Album.h"           
#include "PlayingQueue.h"    
#include "Music.h"           
#include "PlayerManager.h"
#include <iostream>

TEST_CASE("load music function test", "[music]") {
    PlayerManager pm;
    pm.load_songs();
    REQUIRE(pm.music.size() == 0);
}

TEST_CASE("add music function test", "[music]") {
    PlayerManager pm;
    Music music("test1", "example_path", 28, "example_author");
    pm.add_music(music);
    pm.load_songs();
    REQUIRE(pm.music.size() == 1);
    pm.remove_music_from_db("test1");  
}

TEST_CASE("remove music function test", "[music]") {
    PlayerManager pm;
    Music music("test1", "example_path", 28, "example_author");
    pm.add_music(music); 
    pm.remove_music_from_db("test1");
    pm.load_songs();
    REQUIRE(pm.music.size() == 0);
}

TEST_CASE("update music function test", "[music]") {
    PlayerManager pm;
    Music music("test1", "example_path", 28, "example_author");
    pm.add_music(music);
    music.name = "test2";
    pm.update_song(music);
    pm.load_songs();
    REQUIRE(pm.music[0].name == "test2");
    pm.remove_music_from_db("test2");  
}

TEST_CASE("filter liked song function test", "[music]") {
    PlayerManager pm;
    Music music("test1", "example_path", 28, "example_author");
    music.song_liked();
    pm.add_music(music);
    Music music1("test2", "example_path", 28, "example_author");
    pm.add_music(music1);
    pm.load_songs();
    std::vector<Music> music_liked = pm.load_filtered_song(FilterType::LIKED);
    REQUIRE(music_liked.size() == 1);
    pm.remove_music_from_db("test1");
    pm.remove_music_from_db("test2");
}

TEST_CASE("filter disliked song function test", "[music]") {
    PlayerManager pm;
    Music music("test1", "example_path", 28, "example_author");
    music.song_disliked();
    pm.add_music(music);
    Music music1("test2", "example_path", 28, "example_author");
    pm.add_music(music1);  
    pm.load_songs();
    std::vector<Music> music_disliked = pm.load_filtered_song(FilterType::DISLIKED);
    REQUIRE(music_disliked.size() == 1);
    pm.remove_music_from_db("test1");
    pm.remove_music_from_db("test2");
}

TEST_CASE("add album", "[album]") {
    PlayerManager pm;
    Album album("example_album");
    pm.add_album(album);
    pm.load_albums();
    REQUIRE(pm.albums.size() == 1);
    pm.remove_album_from_db("example_album");  
}

TEST_CASE("remove album", "[album]") {
    PlayerManager pm;
    Album album("example_album");
    pm.add_album(album);
    pm.remove_album_from_db("example_album");
    pm.load_albums();
    REQUIRE(pm.albums.size() == 0);
}

TEST_CASE("update album", "[album]") {
    PlayerManager pm;
    Album album("example_album");
    pm.add_album(album);
    album.name = "test1";
    pm.update_album(album); 
    pm.load_albums();
    REQUIRE(pm.albums[0].name == "test1");
    pm.remove_album_from_db("test1");
}

TEST_CASE("filter liked albums function test", "[album]") {
    PlayerManager pm;
    Album album("example_album");
    album.album_liked();
    Album album1("example_album2");
    pm.add_album(album);
    pm.add_album(album1);
    std::vector<Album> albums = pm.load_filtered_albums(FilterType::LIKED);
    REQUIRE(albums.size() == 1);
    pm.remove_album_from_db("example_album");
    pm.remove_album_from_db("example_album2");
}



TEST_CASE("delete music file", "[music]") {
    PlayerManager pm;
    std::string file_path = "/Users/alexanderkorzh/Desktop/coding/musical_player/example.txt";
    Music music("example", file_path, 123, "example_author");

    
    pm.add_music(music);

    
    REQUIRE(std::filesystem::exists(file_path) == true);

    
    pm.delete_music(music.path_to_file);

    
    REQUIRE(std::filesystem::exists(file_path) == false);

    
    pm.remove_music_from_db("example");
}

TEST_CASE("connect music to album","[album]") {
    PlayerManager pm;
    Music music("example","sdsd",213,"example");
    Album album("example");

    pm.add_music_to_album(album,music);
    pm.add_real_music_to_album(album);

    REQUIRE(album.album_songs.size() == 1);

    pm.remove_album_from_db("example");
    pm.remove_music_from_db("example");
}

TEST_CASE("count time to listen","[album]") {
    PlayerManager pm;
    Music music("example1","sdsd",5,"example");
    Music music1("example2","sdsd",5,"example");
    Album album("example");

    pm.add_music_to_album(album,music);
    pm.add_music_to_album(album,music1);
    pm.add_real_music_to_album(album);
    album.compute_listen_time();
    REQUIRE(album.time_to_listen == 10);
    pm.delete_album(album);
}



TEST_CASE("remove album") {
    PlayerManager pm;
    Music music("example1","sdsd",5,"example");
    Music music1("example2","sdsd",5,"example");
    Album album("example");

    pm.add_music_to_album(album,music);
    pm.add_music_to_album(album,music1);
    pm.add_real_music_to_album(album);

    pm.remove_album_from_db(album.name);
}
int main() {
    return 0;
}
