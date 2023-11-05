#ifndef METASTRUCTURES_H
#define METASTRUCTURES_H

#include <vector>
#include <string>
#include <QPixmap>

class Artwork {
public:
    QPixmap albumCover;
    std::string albumName;

    Artwork (QPixmap tAlbumCover, std::string tAlbumName) {
        albumCover = tAlbumCover;
        albumName = tAlbumName;
    }
};

class Artlib {
public:
    std::vector<Artwork*> albumCovers;

    void artlibInit();
    Artwork * addArtwork(std::string artworkPath, std::string artworkAlbumName);
};

class Song {
public:
    std::string name;
    std::string albumName;
    std::string path;

    unsigned int track = 0;

    Song(std::string songName, std::string tAlbumName, std::string songPath) {
        name = songName;
        albumName = tAlbumName;
        path = songPath;
    }

};

class Album {
public:
    std::string name;
    std::vector<Song*> songs;
    Artwork * albumArtwork = NULL;

    Album(std::string albumName) {
        name = albumName;
    }

    Song * addSong(std::string songName, std::string songPath);
    void addArtwork(Artwork * tAlbumArtwork);
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

extern Artlib artLibrary;
extern Library library;

#endif // METASTRUCTURES_H
