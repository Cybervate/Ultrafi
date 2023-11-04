#include "scrapecovers.h"

#include <iostream>
#include <stdlib.h>

void scrapeCover(std::string coverPath, std::string targetPath, std::string albumName) {
    std::string command = "ffmpeg -i \"" + coverPath + "\" -an \"" + targetPath + albumName + "\"";
    std::cout << "COMMAND: " << command << std::endl;
    system(command.c_str());
}

void clearCoverFolder() {
    system("rm -rf /home/mmb/Code/Ultrafi/Ultrafi/Covers/*.jpeg");
}
