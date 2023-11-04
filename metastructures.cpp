#include "metastructures.h"
#include <vector>

Library library;

Artist * Library::addArtist(std::string aName) {
    Artist * newArtist = new Artist(aName);
    artists.push_back(newArtist);
    return newArtist;
}

Album * Artist::addAlbum(std::string albName) {
    Album * newAlbum = new Album(albName);
    albums.push_back(newAlbum);
    return newAlbum;
}

Song * Album::addSong(std::string songName) {
    Song * newSong = new Song(songName);
    songs.push_back(newSong);
    return newSong;
}

//void Song::setTrack(unsigned int aTrack) {
//    track = aTrack;
//}
