#include "ini_file.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept> 

bool _pendingChanges = false;

IniFile::IniFile(LCBLog &logger) : _logger(logger) {
    _logger.logS(INFO, "IniFile instance created without filename.");
}

IniFile::IniFile(const std::string &filename, LCBLog &logger)
    : _logger(logger) {
    set_filename(filename);
    load();
}

bool IniFile::load() {
    if (_filename.empty()) {
        _logger.logE(ERROR, "Error: Filename not set for loading.");
        return false;
    }

    std::ifstream file(_filename);
    if (!file.is_open()) {
        _logger.logE(ERROR, "Error: Cannot open file", _filename);
        return false;
    }

    _data.clear();
    _lines.clear();
    _index.clear();

    std::string line, current_section;
    size_t line_num = 0;

    while (std::getline(file, line)) {
        std::string trimmed = trim(line);
        _lines.push_back(line);

        if (trimmed.empty() || is_comment(trimmed)) {
            line_num++;
            continue;
        }

        if (trimmed.front() == '[' && trimmed.back() == ']') {
            current_section = trimmed.substr(1, trimmed.size() - 2);
        } else {
            size_t pos = trimmed.find('=');
            if (pos != std::string::npos) {
                std::string key = trim(trimmed.substr(0, pos));
                std::string value = trim(trimmed.substr(pos + 1));

                size_t comment_pos = value.find_first_of(";#");
                if (comment_pos != std::string::npos) {
                    value = trim(value.substr(0, comment_pos));
                }

                if (!key.empty()) {
                    _data[current_section][key] = value;
                    _index[current_section][key] = line_num;
                }
            }
        }
        line_num++;
    }

    _logger.logS(INFO, "Successfully loaded INI file:", _filename);
    file.close();
    return true;
}

bool IniFile::save() {
    if (_filename.empty()) {
        _logger.logE(ERROR, "Error: Filename not set for saving.");
        return false;
    }

    std::ofstream file(_filename);
    if (!file.is_open()) {
        _logger.logE(ERROR, "Error: Cannot write to file", _filename);
        return false;
    }

    std::string current_section;
    for (size_t i = 0; i < _lines.size(); ++i) {
        std::string trimmed = trim(_lines[i]);

        if (trimmed.empty() || is_comment(trimmed)) {
            file << _lines[i] << "\n";
            continue;
        }

        if (trimmed.front() == '[' && trimmed.back() == ']') {
            file << _lines[i] << "\n";
            current_section = trimmed.substr(1, trimmed.size() - 2);
            continue;
        }

        size_t pos = trimmed.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(trimmed.substr(0, pos));
            if (!key.empty() && _data.count(current_section) && _data.at(current_section).count(key)) {
                file << key << " = " << _data.at(current_section).at(key) << "\n";
            } else {
                file << _lines[i] << "\n";
            }
        } else {
            file << _lines[i] << "\n";
        }
    }

    file.close();
    _logger.logS(INFO, "Successfully saved INI file:", _filename);
    return true;
}

std::string IniFile::get_value(const std::string& section, const std::string& key) const {
    auto sec = _data.find(section);
    if (sec == _data.end()) {
        _logger.logE(ERROR, "Section not found:", section);
        throw std::runtime_error("Section '" + section + "' not found in INI file.");
    }

    auto val = sec->second.find(key);
    if (val == sec->second.end()) {
        _logger.logE(ERROR, "Key not found in section:", key);
        throw std::runtime_error("Key '" + key + "' not found in section '" + section + "'.");
    }

    return val->second;
}

bool IniFile::string_to_bool(const std::string &value) {
    return (value == "true" || value == "True" || value == "1");
}

int IniFile::get_int_value(const std::string& section, const std::string& key) const {
    try {
        return std::stoi(get_value(section, key));
    } catch (const std::exception& e) {
        _logger.logE(ERROR, "Error parsing integer for", section + "/" + key, e.what());
        return 0;
    }
}

double IniFile::get_double_value(const std::string& section, const std::string& key) const {
    try {
        return std::stod(get_value(section, key));
    } catch (const std::exception& e) {
        _logger.logE(ERROR, "Error parsing double for", section + "/" + key, e.what());
        return 0.0;
    }
}

std::string IniFile::bool_to_string(bool value) {
    return value ? "true" : "false";
}

bool IniFile::get_bool_value(const std::string& section, const std::string& key) const {
    try {
        return string_to_bool(get_value(section, key));
    } catch (const std::runtime_error& e) {
        _logger.logE(ERROR, e.what());
        return false;
    }
}

void IniFile::set_value(const std::string& section, const std::string& key, const std::string& value) {
    _data[section][key] = value;
    _pendingChanges = true;
}

void IniFile::set_bool_value(const std::string& section, const std::string& key, bool value) {
    _data[section][key] = bool_to_string(value);
    _pendingChanges = true;
}

void IniFile::set_int_value(const std::string& section, const std::string& key, int value) {
    _data[section][key] = std::to_string(value);
    _pendingChanges = true;
}

void IniFile::set_double_value(const std::string& section, const std::string& key, double value) {
    _data[section][key] = std::to_string(value);
    _pendingChanges = true;
}

std::string IniFile::trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == std::string::npos) ? "" : str.substr(first, (last - first + 1));
}

bool IniFile::is_comment(const std::string &line) {
    return line.empty() || line.front() == ';' || line.front() == '#';
}

void IniFile::commit_changes() {
    if (_pendingChanges) {
        save();
        _pendingChanges = false;
    }
}

void IniFile::set_filename(const std::string &filename) {
    _filename = filename;
    _logger.logS(INFO, "Filename set to:", _filename);
    load();
}
