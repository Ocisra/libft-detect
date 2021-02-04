#ifndef LIB_FT_DETECT_HPP
#define LIB_FT_DETECT_HPP

#include <filesystem>
#include <string>
#include <unordered_map>

class lft {
    public:
    enum Filetype {
        nonexistent,  // file does not exists
        unknown,      // unrecognized or not decided yet

        // Natives filetypes
        // The OS provides a way to recognized them
        directory,
        symlink,
        block,
        character,
        fifo,
        socket,

        // Mime deduced filetypes
        image,
        video,
        audio,
        font,
        model,  // 3d model
        text,

        // Permissions deduced deduced filetypes
        executable


    };

    Filetype filetype(const std::string &path);
    lft(const std::string &mime_file_path);

    private:
    void parseLine(std::string &line);
    void generateDB(const std::string &path);
    Filetype getNativeFiletype(std::filesystem::path &path);
    Filetype parseMime(std::string &mimetype);
    Filetype matchMime(std::filesystem::path &p);

    std::unordered_map<std::string, std::string> extensions;
    std::unordered_map<std::string, std::string> full;
    std::unordered_map<std::string, std::string> patterns;
};


#endif  // LIB_FT_DETECT_HPP
