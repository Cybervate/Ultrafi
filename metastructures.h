#ifndef METASTRUCTURES_H
#define METASTRUCTURES_H

#include <vector>
#include <string>

class Song {
public:
    std::string name;
    std::string path;

    unsigned int track = 0;

    Song(std::string songName) {
        name = songName;
    }

//    void setTrack(unsigned int aTrack);
};

class Album {
public:
    std::string name;
    std::vector<Song*> songs;

    Album(std::string albumName) {
        name = albumName;
    }

    Song * addSong(std::string songName);
};

class Artist {
public:
    std::string name;
    std::vector<Album*> albums;

    Artist(std::string toName) {
        name = toName;
        albums = {new Album("Unknown Album")};
    }

    Album * addAlbum(std::string albName);
};

class Library {
public:
    std::vector<Artist*> artists;

    Library() {
        artists = {new Artist("Unknown Artist")};
    }

    Artist * addArtist(std::string aName);
};

extern Library library;

#endif // METASTRUCTURES_H
