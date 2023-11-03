#include "folderreader.h"

#include <string>
#include <iostream>
#include <filesystem>
#include "metastructures.h"

#include <taglib/tag.h>
#include <taglib/fileref.h>

#include <algorithm>
#include <vector>

void fillLibrary(TagLib::Tag * tag) {
    Artist artist = Artist("artist");
    if (std::find(library->artists.begin(), library->artists.end(), artist) != library->artists.end()) {
        std::cout << "XXX" << tag->artist() << "XXX" << std::endl;
    }
    else {

        library->artists.insert(library->artists.end(), artist);
    }
}

void folderLoop(const std::string path) {
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        TagLib::FileRef itemRef(entry.path().c_str());


        if (itemRef.tag()) {
            std::cout << itemRef.tag()->artist() << " - " << itemRef.tag()->title() << " : " << itemRef.tag()->album() << std::endl;
            fillLibrary(itemRef.tag());
        }


        if (entry.is_directory()) {
            folderLoop(entry.path().u8string());
        }
    }
}

void readFolder() {
    std::string path = "/home/mmb/Desktop/UltrafiLibrary";

//    TagLib::FileRef f("/home/mmb/Desktop/UltrafiLibrary/Travis Scott/Travis Scott -  Rodeo [2015]/03. 3500 (feat. Future & 2 Chainz).mp3");

    folderLoop(path);
}




