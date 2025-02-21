/**
 * @file ini_file.cpp
 * @brief Implementation of the IniFile class for handling INI file operations.
 * @details This class provides methods for loading, saving, and retrieving
 *          configuration values from an INI file, supporting multiple data types.
 *
 * This software is distributed under the MIT License. See LICENSE.MIT.md for
 * details.
 *
 * Copyright (C) 2023-2025 Lee C. Bussy (@LBussy). All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 #include "ini_file.hpp"
 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <algorithm>
 #include <cctype>
 #include <stdexcept>

/** @brief Tracks whether there are pending changes to be saved. */
bool _pendingChanges = false;

/**
 * @brief Default constructor.
 */
IniFile::IniFile() {}

/**
 * @brief Constructor with filename.
 * @param filename The name of the INI file to load.
 */
IniFile::IniFile(const std::string &filename) {
    set_filename(filename);
    load();
}

bool IniFile::load() {
    if (_filename.empty()) {
        throw std::runtime_error("Null value filename passed for load.");
    }

    std::ifstream file(_filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open ini file " + _filename + ".");
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

    file.close();
    return true;
}

/**
 * @brief Saves the current INI file to disk.
 * @details Writes the stored key-value pairs back to the file while preserving
 *          comments and formatting. If a key exists in the data structure but
 *          not in the original file, it is written as a new entry.
 * @return True if the file was successfully saved, false otherwise.
 * @throws std::runtime_error If the filename is empty or if the file cannot be opened for writing.
 */
bool IniFile::save() {
    if (_filename.empty()) {
        throw std::runtime_error("Null value filename passed for save.");
    }

    std::ofstream file(_filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write to file " + _filename + ".");
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
    return true;
}

/**
 * @brief Retrieves a value as a string from the INI file.
 * @param section The section name.
 * @param key The key name.
 * @return The corresponding value as a string.
 * @throws std::runtime_error If the section or key is not found.
 */
std::string IniFile::get_value(const std::string& section, const std::string& key) const {
    auto sec = _data.find(section);
    if (sec == _data.end()) {
        throw std::runtime_error("Error retrieving [" + section + "] from '" + _filename + "'.");
    }

    auto val = sec->second.find(key);
    if (val == sec->second.end()) {
        throw std::runtime_error("Error retrieving '" + key + "' from section [" + section + "].");
    }
    return val->second;
}

/**
 * @brief Retrieves a string value from the INI file.
 * @param section The section name.
 * @param key The key name.
 * @return The string representation of the stored value.
 * @throws std::runtime_error If the section or key is not found.
 */
std::string IniFile::get_string_value(const std::string& section, const std::string& key) const {
    return get_value(section, key);
}

/**
 * @brief Retrieves an integer value from the INI file.
 * @param section The section name.
 * @param key The key name.
 * @return The integer representation of the stored value.
 * @throws std::runtime_error If the section or key is not found, or if the value cannot be converted to an integer.
 */
int IniFile::get_int_value(const std::string& section, const std::string& key) const {
    std::string value = get_value(section, key); // Let this throw if needed
    try {
        return std::stoi(value);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Key '" + key + "' in section [" + section + "] is not a valid integer: '" + value + "'");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Key '" + key + "' in section [" + section + "] is out of range for integer: '" + value + "'");
    }
}

/**
 * @brief Converts a string representation of a boolean value to a boolean.
 * @details Performs a case-insensitive comparison to determine if the string
 *          represents a true value. Accepts "true", "1", and variations of
 *          "True".
 * @param value The string to convert.
 * @return True if the string represents a true value, otherwise false.
 */
bool IniFile::string_to_bool(const std::string &value) {
    std::string lower_value = value;
    std::transform(lower_value.begin(), lower_value.end(), lower_value.begin(), ::tolower);
    return (lower_value == "true" || lower_value == "t" || lower_value == "1");
}

/**
 * @brief Retrieves a double value from the INI file.
 * @param section The section name.
 * @param key The key name.
 * @return The double representation of the stored value.
 * @throws std::runtime_error If the section or key is not found, or if the value
 *         cannot be converted to a double.
 */
double IniFile::get_double_value(const std::string& section, const std::string& key) const {
    std::string value = get_value(section, key); // Let this throw if needed
    try {
        return std::stod(value);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Key '" + key + "' in section [" + section + "] is not a valid double: '" + value + "'");
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Key '" + key + "' in section [" + section + "] is out of range for double: '" + value + "'");
    }
}

/**
 * @brief Retrieves a boolean value from the INI file.
 * @param section The section name.
 * @param key The key name.
 * @return The boolean representation of the stored value.
 * @throws std::runtime_error If the section or key is not found.
 */
bool IniFile::get_bool_value(const std::string& section, const std::string& key) const {
    return string_to_bool(get_value(section, key));
}

/**
 * @brief Sets a string value in the INI file.
 * @param section The section name.
 * @param key The key name.
 * @param value The string value to set.
 */
// cppcheck-suppress unusedFunction
void IniFile::set_string_value(const std::string& section, const std::string& key, const std::string& value) {
    _data[section][key] = value;
    _pendingChanges = true;
}

/**
 * @brief Converts a boolean value to a string representation.
 * @param value The boolean value to convert.
 * @return "true" if the value is true, otherwise "false".
 */
std::string IniFile::bool_to_string(bool value) {
    return value ? "true" : "false";
}

/**
 * @brief Sets a boolean value in the INI file.
 * @param section The section name.
 * @param key The key name.
 * @param value The boolean value to set.
 */
void IniFile::set_bool_value(const std::string& section, const std::string& key, bool value) {
    _data[section][key] = bool_to_string(value);
    _pendingChanges = true;
}

/**
 * @brief Sets an integer value in the INI file.
 * @param section The section name.
 * @param key The key name.
 * @param value The integer value to set.
 */
void IniFile::set_int_value(const std::string& section, const std::string& key, int value) {
    _data[section][key] = std::to_string(value);
    _pendingChanges = true;
}

/**
 * @brief Sets a double value in the INI file.
 * @param section The section name.
 * @param key The key name.
 * @param value The double value to set.
 */
void IniFile::set_double_value(const std::string& section, const std::string& key, double value) {
    _data[section][key] = std::to_string(value);
    _pendingChanges = true;
}

/**
 * @brief Trims leading and trailing whitespace characters from a string.
 * @param str The input string to be trimmed.
 * @return A new string with whitespace removed from both ends.
 */
std::string IniFile::trim(const std::string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == std::string::npos) ? "" : str.substr(first, (last - first + 1));
}

/**
 * @brief Determines whether a given line is a comment.
 * @param line The line to check.
 * @return True if the line is a comment, false otherwise.
 */
bool IniFile::is_comment(const std::string &line) {
    return line.empty() || line.front() == ';' || line.front() == '#';
}

/**
 * @brief Commits any pending changes by saving the INI file.
 * @details If there are unsaved changes, this function writes them to the file
 *          and resets the pending changes flag.
 */
// cppcheck-suppress unusedFunction
void IniFile::commit_changes() {
    if (_pendingChanges) {
        save();
        _pendingChanges = false;
    }
}

/**
 * @brief Sets the filename and reloads the INI file.
 * @param filename The filename to set.
 */
void IniFile::set_filename(const std::string &filename) {
    _filename = filename;
    load();
}
