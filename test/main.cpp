#include "../libft-detect.hpp"
#include <filesystem>
#include <iostream>

#define MIME_TYPES_PATH "/usr/share/mime/globs"


int main() {
    lft *finder = new lft(MIME_TYPES_PATH);
    for (auto &e : std::filesystem::directory_iterator(".")) {
        lft::filetype *ft = finder->getFiletype(e.path().string());
        std::string t;
        switch (ft->general) {
        case lft::nonexistent: t = "nonexistent"; break;
        case lft::unknown: t = "unknown"; break;
        case lft::directory: t = "directory"; break;
        case lft::symlink: t = "symlink"; break;
        case lft::block: t = "block"; break;
        case lft::character: t = "character"; break;
        case lft::fifo: t = "fifo"; break;
        case lft::socket: t = "socket"; break;
        case lft::image: t = "image"; break;
        case lft::video: t = "video"; break;
        case lft::audio: t = "audio"; break;
        case lft::font: t = "font"; break;
        case lft::model: t = "model"; break;
        case lft::text: t = "text"; break;
        case lft::executable: t = "executable"; break;
        }
        std::cout << e.path().string() << ": " << t;
        if (ft->isHidden())
            std::cout << "  -  hidden";
        std::cout << std::endl;
        delete ft;
    }
    delete finder;
    return 0;
}

// COMPILE:
// cd /path/to/libft-detect
// g++ test/main.cpp bin/libft-detect.so -o test/bin/main -Wall -Wextra -g -std=c++20 -lft-detect -Lbin
//
// EXECUTE:
// LD_LIBRARY_PATH=./bin/ ./test/bin/main
