#include "libft-detect.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <string.h>
#include <unistd.h>

/**
 * Find if a given string is a pattern
 * Does not support escaping
 * Only supports * and [] rules
 */
static bool isPattern(std::string s) {
    for (auto &c : s) {
        if (c == '[' || c == '*')
            return true;
    }
    return false;
}

/**
 * Pattern matching is NOT yet SUPPORTED
 * Patterns are simply skipped
 *//*
static std::unordered_map<std::string, std::string>::iterator
  matchPattern(std::unordered_map<std::string, std::string> &patterns,
               std::filesystem::path &path) {
    return patterns.end();
}*/


lft::lft(const std::string &mime_file_path) {
    generateDB(mime_file_path);
}

void lft::parseLine(std::string &line) {
    if (line[0] == '#')
        return;
    size_t pos = 0;
    std::string mimetype;
    if ((pos = line.find(':')) != std::string::npos)
        mimetype = line.substr(0, pos);
    else
        return;

    std::string rule;
    rule = line.substr(pos + 1, line.length() - pos);
    if (rule.starts_with("*."))
        extensions.insert({rule.substr(1, rule.length() - 1), mimetype});
    else if (!isPattern(rule))
        full.insert({rule, mimetype});
    else
        patterns.insert({rule, mimetype});
}

void lft::generateDB(const std::string &path) {
    std::ifstream f(path);
    std::string line;
    while (getline(f, line)) {
        parseLine(line);
    }
}

lft::Filetype lft::getNativeFiletype(std::filesystem::path &path) {
    switch (std::filesystem::status(path).type()) {
    case std::filesystem::file_type::directory: return directory;
    case std::filesystem::file_type::symlink: return symlink;
    case std::filesystem::file_type::block: return block;
    case std::filesystem::file_type::character: return character;
    case std::filesystem::file_type::fifo: return fifo;
    case std::filesystem::file_type::socket: return socket;
    default: return unknown;
    }
}

lft::Filetype lft::parseMime(std::string &mimetype) {
    if (mimetype.starts_with("text"))
        return text;
    if (mimetype.starts_with("image"))
        return image;
    if (mimetype.starts_with("video"))
        return video;
    if (mimetype.starts_with("audio"))
        return audio;
    if (mimetype.starts_with("font"))
        return font;
    if (mimetype.starts_with("model"))
        return model;
    return unknown;
}

lft::Filetype lft::matchMime(std::filesystem::path &p) {
    std::unordered_map<std::string, std::string>::iterator entry;
    if ((entry = extensions.find(p.extension().string())) != extensions.end())
        return parseMime(entry->second);
    if ((entry = full.find(p.string())) != full.end())
        return parseMime(entry->second);
    // if ((entry = matchPattern(patterns, p)) != patterns.end())
    // return parseMime(entry->second);

    return unknown;
}

lft::Filetype lft::filetype(const std::string &path) {
    std::filesystem::path p(path);
    if (!std::filesystem::exists(p))
        return nonexistent;

    Filetype ft;

    if ((ft = getNativeFiletype(p)) != unknown)
        return ft;

    if (access(p.c_str(), X_OK) == 0)
        return executable;

    if ((ft = matchMime(p)) != unknown)
        return ft;


    return unknown;
}



// TODO
// custom simplified mime database so i come without deps + don't need that much different
// types
