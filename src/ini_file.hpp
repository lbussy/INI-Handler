/**
 * @file ini_file.hpp
 * @brief INI file handling class definition.
 * @details This class provides functionality to load, save, and retrieve
 *          configuration values from an INI file format. Supports string,
 *          boolean, integer, and double values with optional default values.
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

#ifndef INI_FILE_HPP
#define INI_FILE_HPP

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

/**
 * @class IniFile
 * @brief Handles reading and writing INI-style configuration files.
 * @details This class loads an INI file into an internal data structure,
 *          allowing retrieval and modification of values with optional
 *          default fallbacks.
 */
class IniFile
{
public:
    /**
     * @brief Constructor with no file ilename reference.
     */
    explicit IniFile();

    /**
     * @brief Constructor with filename reference.
     * @param filename Path to the INI file.
     */
    IniFile(const std::string &filename);

    /**
     * @brief Sets the filename for loading and saving operations.
     * @param filename Path to the INI file.
     */
    void set_filename(const std::string &filename);

    /**
     * @brief Loads the INI file into memory.
     * @return True if the file was successfully loaded, false otherwise.
     */
    bool load();

    /**
     * @brief Saves the current data to the INI file.
     * @return True if the file was successfully saved, false otherwise.
     */
    bool save();

    /**
     * @brief Retrieves a string value from the INI file.
     * @param section The section name.
     * @param key The key name.
     * @return The corresponding value as a string.
     * @throws std::runtime_error if the section or key is not found.
     */
    std::string get_value(const std::string &section, const std::string &key) const;

    /**
     * @brief Retrieves a string value with an optional default.
     */
    std::string get_string_value(const std::string &section, const std::string &key) const;
    // std::string get_string_value(const std::string &section, const std::string &key, const std::string &default_value) const;

    /**
     * @brief Retrieves a boolean value with an optional default.
     */
    bool get_bool_value(const std::string &section, const std::string &key) const;
    // bool get_bool_value(const std::string &section, const std::string &key, bool default_value) const;

    /**
     * @brief Retrieves an integer value with an optional default.
     */
    int get_int_value(const std::string &section, const std::string &key) const;
    // int get_int_value(const std::string &section, const std::string &key, int default_value) const;

    /**
     * @brief Retrieves a double value with an optional default.
     */
    double get_double_value(const std::string &section, const std::string &key) const;
    // double get_double_value(const std::string &section, const std::string &key, double default_value) const;

    /**
     * @brief Sets a string value in the INI file.
     */
    void set_string_value(const std::string &section, const std::string &key, const std::string &value);
    void set_bool_value(const std::string &section, const std::string &key, bool value);
    void set_int_value(const std::string &section, const std::string &key, int value);
    void set_double_value(const std::string &section, const std::string &key, double value);

    /**
     * @brief Commits any pending changes to the INI file.
     */
    void commit_changes();

private:
    std::string _filename;                                                     ///< Path to the INI file.
    std::map<std::string, std::unordered_map<std::string, std::string>> _data; ///< Internal data storage.
    std::vector<std::string> _lines;                                           ///< Stores original file lines for integrity.
    std::map<std::string, std::map<std::string, size_t>> _index;               ///< Index mapping for fast lookups.

    /**
     * @brief Trims whitespace from a string.
     * @param str The string to trim.
     * @return Trimmed string.
     */
    static std::string trim(const std::string &str);

    /**
     * @brief Determines if a line is a comment.
     * @param line The line to check.
     * @return True if the line is a comment, false otherwise.
     */
    static bool is_comment(const std::string &line);

    /**
     * @brief Converts a boolean value to string.
     * @param value The boolean value.
     * @return "true" or "false".
     */
    static std::string bool_to_string(bool value);

    /**
     * @brief Converts a string to boolean.
     * @param value The string to convert.
     * @return True if the string represents a true value, false otherwise.
     */
    static bool string_to_bool(const std::string &value);
};

#endif // INI_FILE_HPP
