#ifndef METASTRUCTURES_H
#define METASTRUCTURES_H

#include <vector>
#include <string>

class Song {
public:
    std::string name;
    std::string path;
};

class Album {
public:
    std::string name;
    std::vector<Song> songs;
};

class Artist {
public:
    std::string name;
    std::vector<Album> albums;

    Artist(std::string toName) {
        name = toName;
    }
};

class Library {
public:
    std::vector<Artist> artists;
};

extern Library* library;

#endif // METASTRUCTURES_H
