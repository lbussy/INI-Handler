/**
 * @file main.cpp
 * @brief Main file for testing the IniFile class
 * @details This file is used to test the IniFile class. It reads an INI file,
 *          performs some read and write operations, and tests exception
 *          handling.
 *
 * This software is distributed under the MIT License. See LICENSE.md for
 * details.
 *
 * Copyright (C) 2025 Lee C. Bussy (@LBussy). All rights reserved.
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

std::string filename = "../test/test.ini";

void test_malformed_entries(IniFile &config)
{
    std::cout << "\n⚠️ Testing Malformed INI Entries:\n";

    try
    {
        config.set_string_value("Common", "TX Power", "abc"); // Invalid integer
        int txPower = config.get_int_value("Common", "TX Power");
        std::cout << "TX Power after setting invalid value: " << txPower << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Caught exception for malformed TX Power: " << e.what() << "\n";
    }

    try
    {
        config.set_string_value("Extended", "PPM", "xyz"); // Invalid double
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
    std::cout << "✅ Call Sign: " << config.get_string_value("Common", "Call Sign") << "\n";
    std::cout << "✅ TX Power: " << config.get_int_value("Common", "TX Power") << "\n";
    std::cout << "✅ Power Level: " << config.get_int_value("Extended", "Power Level") << "\n";
    std::cout << "✅ PPM: " << config.get_double_value("Extended", "PPM") << "\n";
    std::cout << "✅ FREQ: " << config.get_string_value("Common", "Frequency") << "\n";

    try
    {
        std::cout << "❌ Non-existent Section: " << config.get_string_value("NonExistent", "Key") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "⚠️ Caught Exception: " << e.what() << "\n";
    }

    try
    {
        std::cout << "❌ Non-existent Key in Existing Section: "
                  << config.get_string_value("Control", "FakeKey") << "\n";
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
    config.set_string_value("Common", "Call Sign", "TEST123");

    // TODO: This is not working
    config.set_string_value("NewSection", "NewKey", "NewValue");

    config.commit_changes();

    std::cout << "✅ Test write complete." << std::endl;
}

void test_exceptions(IniFile &config)
{
    std::cout << "\n🔎 Testing Wsprry Pi INI Exception Processing\n";
    try
    {
        std::cout << "\n❌ Reading get_string_value() [Section Error]\n";
        config.get_string_value("Bad Section", "Bad Key");
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Caught Exception: " <<  e.what() << std::endl;
    }

    try
    {
        std::cout << "\n❌ Reading get_bool_value() [Section Error]\n";
        config.get_bool_value("Bad Section", "Bad Key");
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Caught Exception: " << e.what() << std::endl;
    }

    try
    {
        std::cout << "\n❌ Reading get_string_value() [Key Error]\n";
        config.get_string_value("Common", "Bad Key");
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Caught Exception: " << e.what() << std::endl;
    }

    try
    {
        std::cout << "\n❌ Reading get_int_value() [Key Error]\n";
        config.get_int_value("Common", "Bad Key");
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Caught Exception: " << e.what() << std::endl;
    }

    try
    {
        std::cout << "\n❌ Reading get_double_value(0 [Key Error]\n";
        config.get_double_value("Common", "Bad Key");
    }
    catch (const std::exception &e)
    {
        std::cerr << "ERROR: Caught Exception: " << e.what() << std::endl;
    }

    // Change PPM to "1e500" in the INI to simulate the error
    try
    {
        std::cout << "\n✅ PPM (see comments to force error): " << config.get_double_value("Extended", "PPM") << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "\n❌ Reading get_double_value() [stod() Error]\n";
        std::cerr << "ERROR: Caught Exception: " << e.what() << std::endl;
    }
}

int main()
{
    // Set the filename
    iniFile.set_filename(filename);

    // test_reading(ini);
    // test_writing(iniFile);
    // test_malformed_entries(ini);
    // test_exceptions(ini);

    return 0;
}
