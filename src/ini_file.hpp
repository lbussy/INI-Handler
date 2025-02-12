#ifndef INI_FILE_HPP
#define INI_FILE_HPP

#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include "../src/LCBLog/lcblog.hpp"  // Include LCBLog

class IniFile {
public:
    IniFile(LCBLog &logger);  // Constructor without filename
    IniFile(const std::string &filename, LCBLog &logger);

    void set_filename(const std::string &filename);

    bool load();
    bool save();

    std::string get_value(const std::string &section, const std::string &key) const;
    bool get_bool_value(const std::string &section, const std::string &key) const;
    int get_int_value(const std::string &section, const std::string &key) const;
    double get_double_value(const std::string &section, const std::string &key) const;

    void set_value(const std::string &section, const std::string &key, const std::string &value);
    void set_bool_value(const std::string &section, const std::string &key, bool value);
    void set_int_value(const std::string &section, const std::string &key, int value);
    void set_double_value(const std::string &section, const std::string &key, double value);

    void commit_changes();

private:
    std::string _filename;
    std::map<std::string, std::unordered_map<std::string, std::string>> _data;
    std::vector<std::string> _lines;
    std::map<std::string, std::map<std::string, size_t>> _index;

    LCBLog &_logger;  // Reference to logger

    static std::string trim(const std::string &str);
    static bool is_comment(const std::string &line);
    static std::string bool_to_string(bool value);
    static bool string_to_bool(const std::string &value);
};

#endif // INI_FILE_HPP