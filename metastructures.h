#ifndef METASTRUCTURES_H
#define METASTRUCTURES_H

#include <vector>
#include <string>
#include <QPixmap>

class Artwork;
class Artlib;
class Song;
class Album;
class Artist;
class Library;

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
    Artwork * addArtwork(QPixmap pixmap, std::string artworkAlbumName);
};

class Song {
public:
    std::string name;
    std::string albumName;
    std::string artistName;
    std::string path;

    Album * album;

    unsigned int track = 0;

    Song(std::string songName, std::string tAlbumName, std::string tArtistName, std::string songPath, unsigned int tTrack, Album * tAlbum) {
        name = songName;
        albumName = tAlbumName;
        artistName = tArtistName;
        path = songPath;
        track = tTrack;

        album = tAlbum;
    }

};

class Album {
public:
    std::string name;
    std::vector<Song*> songs;
    Artwork * albumArtwork = NULL;

    Artist * artist;

    Album(std::string albumName, Artist * tArtist) {
        name = albumName;
        artist = tArtist;
    }

    Song * addSong(std::string songName, std::string songPath, std::string artistName, unsigned int songTrack);
    void addArtwork(Artwork * tAlbumArtwork);
};

class Artist {
public:
    std::string name;
    std::vector<Album*> albums;

    Artist(std::string toName) {
        name = toName;
        albums = {new Album("Unknown Album", this)};
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
