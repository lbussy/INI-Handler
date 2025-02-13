#include "ini_file.hpp"
#include <iostream>

LCBLog llog; // Create the logger instance

void test_malformed_entries(IniFile &config)
{
    std::cout << "\n⚠️ Testing Malformed INI Entries:\n";

    try
    {
        config.set_value("Common", "TX Power", "abc"); // Invalid integer
        int txPower = config.get_int_value("Common", "TX Power");
        std::cout << "TX Power after setting invalid value: " << txPower << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught exception for malformed TX Power: " << e.what() << "\n";
    }

    try
    {
        config.set_value("Extended", "PPM", "xyz"); // Invalid double
        double ppm = config.get_double_value("Extended", "PPM");
        std::cout << "PPM after setting invalid value: " << ppm << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught exception for malformed PPM: " << e.what() << "\n";
    }
}

void test_reading(IniFile &config)
{
    std::cout << "\n🔎 Testing Read Operations:\n";

    std::cout << "✅ Transmit Enabled: " << config.get_bool_value("Control", "Transmit") << "\n";
    std::cout << "✅ Call Sign: " << config.get_value("Common", "Call Sign") << "\n";
    std::cout << "✅ TX Power: " << config.get_int_value("Common", "TX Power") << "\n";
    std::cout << "✅ Power Level: " << config.get_int_value("Extended", "Power Level") << "\n";
    std::cout << "✅ PPM: " << config.get_double_value("Extended", "PPM") << "\n";
    std::cout << "✅ FREQ: " << config.get_value("Common", "Frequency") << "\n";

    try
    {
        std::cout << "❌ Non-existent Section: " << config.get_value("NonExistent", "Key") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "⚠️ Caught Exception: " << e.what() << "\n";
    }

    try
    {
        std::cout << "❌ Non-existent Key in Existing Section: "
                  << config.get_value("Control", "FakeKey") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "⚠️ Caught Exception: " << e.what() << "\n";
    }
}

void test_writing(IniFile &config)
{
    std::cout << "\n📝 Testing Write Operations:\n";

    config.set_bool_value("Control", "Transmit", true);
    config.set_int_value("Common", "TX Power", 30);
    config.set_double_value("Extended", "PPM", 1.23);
    config.set_value("Common", "Call Sign", "TEST123");

    config.set_value("NewSection", "NewKey", "NewValue");

    config.commit_changes();
}

void test_reading_wsprry_pi(IniFile &config)
{
    std::cout << "\n🔎 Testing Wsprry Pi INI Reads\n";

    try
    {
        std::cout << "\n🔎 Reading [Control]\n";
        std::cout << "✅ Transmit Enabled: " << config.get_bool_value("Control", "Transmit") << "\n";

        std::cout << "\n🔎 Reading [Common]\n";
        std::cout << "✅ Call Sign: " << config.get_value("Common", "Call Sign") << "\n";
        std::cout << "✅ Grid Square: " << config.get_value("Common", "Grid Square") << "\n";
        std::cout << "✅ TX Power: " << config.get_int_value("Common", "TX Power") << "\n";
        std::cout << "✅ FREQ: " << config.get_value("Common", "Frequency") << "\n";

        std::cout << "\n🔎 Reading [Extended]\n";
        std::cout << "✅ PPM: " << config.get_double_value("Extended", "PPM") << "\n";
        std::cout << "✅ Self Cal: " << config.get_bool_value("Extended", "Self Cal") << "\n";
        std::cout << "✅ Use Offset: " << config.get_bool_value("Extended", "Offset") << "\n";
        std::cout << "✅ Use LED: " << config.get_bool_value("Extended", "Use LED") << "\n";
        std::cout << "✅ Power Level: " << config.get_int_value("Extended", "Power Level") << "\n";

        std::cout << "\n🔎 Reading [Server]\n";
        std::cout << "✅ Port: " << config.get_int_value("Server", "Port") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "\n" << "Caught Exception: " << e.what() << "\n";
    }

    try
    {
        std::cout << "\n🔎 Reading get_value() [Section Error]\n";
        config.get_value("Bad Section", "Bad Key");
    }
    catch (const std::exception &e)
    {
        llog.logE(ERROR, "Caught Exception: ", e.what());
    }

    try
    {
        std::cout << "\n🔎 Reading get_value() [Key Error]\n";
        config.get_value("Common", "Bad Key");
    }
    catch (const std::exception &e)
    {
        llog.logE(ERROR, "Caught Exception: ", e.what());
    }

    try
    {
        std::cout << "\n🔎 Reading get_int_value() [Key Error]\n";
        config.get_int_value("Common", "Bad Key");
    }
    catch (const std::exception &e)
    {
        llog.logE(ERROR, "Caught Exception: ", e.what());
    }

    try
    {
        std::cout << "\n🔎 Reading get_double_value(0 [Key Error]\n";
        config.get_double_value("Common", "Bad Key");
    }
    catch (const std::exception &e)
    {
        llog.logE(ERROR, "Caught Exception: ", e.what());
    }
}

int main()
{
    // Create instance without a filename
    llog.setLogLevel(DEBUG);
    llog.enableTimestamps(true);

    // Create an instance of the IniFile class
    IniFile ini(llog);

    // Set the filename
    // ini.set_filename("wsprrypi.ini");
    ini.set_filename("/usr/local/etc/wsprrypi.ini");

    // test_reading(ini);
    // test_writing(ini);
    // test_malformed_entries(ini);

    test_reading_wsprry_pi(ini);

    return 0;
}
