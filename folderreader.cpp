#include "folderreader.h"

#include <string>
#include <iostream>
#include <filesystem>
#include "metastructures.h"
#include "scrapecovers.h"

#include <taglib/tag.h>
#include <taglib/fileref.h>

#include <algorithm>
#include <vector>

void fillLibrary(TagLib::Tag * tag, std::string songPath) {
    std::string artName = tag->artist().toCString();
    std::string albName = tag->album().toCString();
    std::string songName = tag->title().toCString();

    Artist * pArtist;
    Album * pAlbum;
    Song * pSong;

    bool artistFound = false;
    if (tag->artist().isEmpty()) {
        artName = "Unknown Artist";
    }
    for(auto &art : library.artists) {
        if(art->name == artName) {
            artistFound = true;
            pArtist = art;
        }
    }
    if (!artistFound) {
        pArtist = library.addArtist(artName);
    }

    bool albumFound = false;
    if (tag->album().isEmpty()) {
        albName = "Unknown Album";
    }
    for (auto &alb : pArtist->albums) {
        if (alb->name == albName) {
            albumFound = true;
            pAlbum = alb;
        }
    }
    if (!albumFound) {
        pAlbum = pArtist->addAlbum(albName);
    }

    if (tag->title().isEmpty()) {
        songName = "Unknown Song";
    }

    pSong = pAlbum->addSong(songName, songPath);

    if(tag->track()) {
        pSong->track = tag->track();
    }

    bool artworkFound = false;
    for (auto &artwork : artLibrary.albumCovers) {
        if (pAlbum->name == artwork->albumName) {
            artworkFound = true;
        }
    }
    if (!artworkFound) {
        artLibrary.addArtwork(songPath, pAlbum->name);
        scrapeCover(songPath, "/home/mmb/Code/Ultrafi/Ultrafi/Covers/", pAlbum->name + ".jpeg");
    }

}

void folderLoop(const std::string path) {
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        TagLib::FileRef itemRef(entry.path().c_str());


        if (itemRef.tag()) {
            std::cout << itemRef.tag()->artist() << " - " << itemRef.tag()->title() << " : " << itemRef.tag()->album() << std::endl;
            fillLibrary(itemRef.tag(), entry.path());
        }
        else if (entry.is_directory()) {
            folderLoop(entry.path().u8string());
        }
    }

}

void sortAlbums() {
    for (auto &a : library.artists) {
        for (auto &alb : a->albums) {
            sort(alb->songs.begin(), alb->songs.end(), [](Song* s1, Song* s2) {
                return s1->track < s2->track;
            });
        }
    }
}

void readFolder() {
    std::string path = "/home/mmb/Desktop/UltrafiLibrary";

    folderLoop(path);
    sortAlbums();

    for (auto a : library.artists) {
        std::cout << "Artist: " << a->name << " ";
        for (auto alb : a->albums) {
            std::cout << "Album: " << alb->name << " " << std::endl;;
            for (auto song : alb->songs) {
                std::cout << song->track << ". " << song->name << std::endl;
            }
        }
        std::cout << std::endl;
    }

    for (auto art : artLibrary.albumCovers) {
        std::cout << "Cover Art: " << art->albumName << std::endl;
    }
//    scrapeCover("/home/mmb/Desktop/UltrafiLibrary/Travis Scott/Travis Scott - ASTROWORLD (2018) [FLAC] [Hunter]/Travis Scott - ASTROWORLD (2018) [FLAC]/01. STARGAZING.flac",
//                "/home/mmb/Code/Ultrafi/Ultrafi/Covers/",
//                "Astroworld.jpeg");
}




