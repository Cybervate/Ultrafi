#include "folderreader.h"

#include <string>
#include <iostream>
#include <filesystem>
#include "metastructures.h"

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>
#include <taglib/flacfile.h>
#include <taglib/vorbisfile.h>
#include <taglib/tpropertymap.h>

#include <algorithm>
#include <vector>
#include <string>

TagLib::ID3v2::AttachedPictureFrame * artSearch(TagLib::ID3v2::FrameList fList) {
    for (TagLib::ID3v2::Frame *frame : fList) {
        // "APIC" is code for album cover
        if (frame->frameID() == "APIC") {
            return dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);
        }
    }
    return NULL;
}

void fillLibrary(TagLib::FileRef itemRef, std::string songPath, std::string fileType) {
    TagLib::Tag * tag = itemRef.tag();

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

    pSong = pAlbum->addSong(songName, songPath, tag->track());

    if (pAlbum->albumArtwork == NULL) {

        if (fileType == ".mp3") {

            TagLib::MPEG::File mp3Ref(songPath.c_str());
            TagLib::ID3v2::Tag * mp3Tag = mp3Ref.ID3v2Tag();

            if (mp3Tag) {
                TagLib::ID3v2::AttachedPictureFrame * artworkFrame = artSearch(mp3Tag->frameList());

                if (artworkFrame != NULL && artworkFrame) {
                    TagLib::ByteVector imageData = artworkFrame->picture();

                    QByteArray byteArray(reinterpret_cast<const char*>(imageData.data()), imageData.size());
                    QImage image;
                    image.loadFromData(byteArray);

                    if (!image.isNull()) {
                        QPixmap pixmap = QPixmap::fromImage(image);
                        pAlbum->addArtwork(artLibrary.addArtwork(pixmap, pAlbum->name));
                    }
                }
            }
        }
        else if (fileType == ".flac") {
            TagLib::FLAC::File flacRef(songPath.c_str());

            if (!flacRef.isValid()) {
                std::cout << "FLAC ARTWORK ERROR" << std::endl;
            }
            else {
                if (flacRef.hasXiphComment()) {
                    TagLib::Ogg::XiphComment * vorbisComment = flacRef.xiphComment();

                    if (vorbisComment) {

                        TagLib::List<TagLib::FLAC::Picture*> pictureList = flacRef.pictureList();

                        if (!pictureList.isEmpty()) {

                            TagLib::ByteVector imageData = pictureList.front()->data();

                            for (const TagLib::FLAC::Picture * picture : pictureList) {
                                if (picture->type() == TagLib::FLAC::Picture::FrontCover) {
                                    imageData = picture->data();
                                    break;
                                }
                            }

                            QByteArray byteArray(reinterpret_cast<const char*>(imageData.data()), imageData.size());
                            QImage image;
                            image.loadFromData(byteArray);

                            if (!image.isNull()) {
                                QPixmap pixmap = QPixmap::fromImage(image);
                                pAlbum->addArtwork(artLibrary.addArtwork(pixmap, pAlbum->name));

                            }
                        }
                    }
                }
            }
        }
    }
    else if (pAlbum->name == "Unknown Album") {
        pAlbum->addArtwork(artLibrary.albumCovers.front());
    }

    // Final Check
    if (pAlbum->albumArtwork == NULL) {
        pAlbum->addArtwork(artLibrary.albumCovers.front());
    }
}

void folderLoop(const std::string path) {
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            folderLoop(entry.path().u8string());
            continue;
        }

        std::string fileType = entry.path().extension();
        if (
            fileType != ".mp3" &&
            fileType != ".wav" &&
            fileType != ".flac"
            )
        {
            continue;
        }

        TagLib::FileRef itemRef(entry.path().c_str());

        if (itemRef.tag()) {
//            std::cout << itemRef.tag()->artist() << " - " << itemRef.tag()->title() << " : " << itemRef.tag()->album() << std::endl;
            fillLibrary(itemRef, entry.path(), fileType);
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
}
