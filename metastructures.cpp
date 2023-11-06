#include "metastructures.h"
#include <vector>

Library library;
Artlib artLibrary;

Artist * Library::addArtist(std::string aName) {
    Artist * newArtist = new Artist(aName);
    artists.push_back(newArtist);
    return newArtist;
}

Album * Artist::addAlbum(std::string albName) {
    Album * newAlbum = new Album(albName, this);
    albums.push_back(newAlbum);
    return newAlbum;
}

Song * Album::addSong(std::string songName, std::string songPath, std::string artistName, unsigned int songTrack) {
    Song * newSong = new Song(songName, name, artistName, songPath, songTrack, this);
    songs.push_back(newSong);
    return newSong;
}

void Artlib::artlibInit() {
    QPixmap uArt("/home/mmb/Code/Ultrafi/Ultrafi/Covers/unknowncoverpng");
    albumCovers.push_back(new Artwork(uArt, "Unknown Album"));
}

Artwork * Artlib::addArtwork(QPixmap pixmap, std::string artworkAlbumName) {
    Artwork * newArtwork = new Artwork(pixmap, artworkAlbumName);
    albumCovers.push_back(newArtwork);
    return newArtwork;
}

void Album::addArtwork(Artwork * tAlbumArtwork) {
    albumArtwork = tAlbumArtwork;
}
