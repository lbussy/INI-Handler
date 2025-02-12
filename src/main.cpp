#include "ini_file.hpp"
#include <iostream>

void test_malformed_entries(IniFile& config) {
    std::cout << "\n⚠️ Testing Malformed INI Entries:\n";

    try {
        config.set_value("Common", "TX Power", "abc");  // Invalid integer
        int txPower = config.get_int_value("Common", "TX Power");
        std::cout << "TX Power after setting invalid value: " << txPower << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Caught exception for malformed TX Power: " << e.what() << "\n";
    }

    try {
        config.set_value("Extended", "PPM", "xyz");  // Invalid double
        double ppm = config.get_double_value("Extended", "PPM");
        std::cout << "PPM after setting invalid value: " << ppm << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Caught exception for malformed PPM: " << e.what() << "\n";
    }
}

void test_reading(IniFile& config) {
    std::cout << "\n🔎 Testing Read Operations:\n";
    
    std::cout << "✅ Transmit Enabled: " << config.get_bool_value("Control", "Transmit") << "\n";
    std::cout << "✅ Call Sign: " << config.get_value("Common", "Call Sign") << "\n";
    std::cout << "✅ TX Power: " << config.get_int_value("Common", "TX Power") << "\n";
    std::cout << "✅ PPM: " << config.get_double_value("Extended", "PPM") << "\n";

    try {
        std::cout << "❌ Non-existent Section: " << config.get_value("NonExistent", "Key") << "\n";
    } catch (const std::exception& e) {
        std::cerr << "⚠️ Caught Exception: " << e.what() << "\n";
    }

    try {
        std::cout << "❌ Non-existent Key in Existing Section: " 
                  << config.get_value("Control", "FakeKey") << "\n";
    } catch (const std::exception& e) {
        std::cerr << "⚠️ Caught Exception: " << e.what() << "\n";
    }
}

void test_writing(IniFile& config) {
    std::cout << "\n📝 Testing Write Operations:\n";

    config.set_bool_value("Control", "Transmit", true);
    config.set_int_value("Common", "TX Power", 30);
    config.set_double_value("Extended", "PPM", 1.23);
    config.set_value("Common", "Call Sign", "TEST123");

    config.set_value("NewSection", "NewKey", "NewValue");

    config.commit_changes();
}

int main() {
    LCBLog logger;  // Create the logger instance

    // Create instance without a filename
    IniFile ini(logger);
    
    // Set the filename later
    ini.set_filename("wsprrypi.ini");

    test_reading(ini);
    test_writing(ini);
    test_malformed_entries(ini);

    return 0;
}