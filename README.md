# INI-Handler

A lightweight and efficient C++ library for reading, writing, and managing **INI configuration files** with structured logging support using `LCBLog`.

## Features
- **Read and write INI files** with structured key-value sections.
- **Error handling and logging** via `LCBLog`.
- **Supports default values** when reading missing keys.
- **Batch writing & flush optimization** for efficient file updates.
- **Comment preservation** when saving INI files.

## Dependencies
- **C++17 or later**
- `LCBLog` for logging

## Installation

### Clone the Repository
```sh
$ git clone https://github.com/yourusername/INI-Handler.git
$ cd INI-Handler/src
```

### Build the Library
```sh
$ make
```

### Run Tests
```sh
$ make test
```

## Usage

### Initializing the INI File Handler
```cpp
#include "ini_file.hpp"
#include "LCBLog/lcblog.hpp"

int main() {
    LCBLog logger;
    IniFile config("config.ini", logger);

    return 0;
}
```

### Reading Values
```cpp
std::string callSign = config.get_string_value("Common", "Call Sign");
bool enabled = config.get_bool_value("Control", "Transmit");
int power = config.get_int_value("Common", "TX Power", 10); // Default: 10
double ppm = config.get_double_value("Extended", "PPM", 0.0); // Default: 0.0
```

### Writing Values
```cpp
config.set_string_value("Common", "Call Sign", "TEST123");
config.set_bool_value("Control", "Transmit", true);
config.set_int_value("Common", "TX Power", 30);
config.set_double_value("Extended", "PPM", 1.23);

config.commit_changes(); // Saves all changes
```

## Logging
- All errors and events are logged using `LCBLog`.
- Errors are prefixed with `[ERROR]` and informational messages with `[INFO]`.

Example log messages:
```
[INFO] Successfully loaded INI file: config.ini
[ERROR] Section not found: NonExistent
[ERROR] Key not found in section: FakeKey
```

## Makefile Commands
| Command      | Description                 |
|-------------|-----------------------------|
| `make`      | Compiles the library         |
| `make test` | Runs the test suite          |
| `make clean`| Removes compiled files       |

## Contributing
Pull requests are welcome! If you find any issues, please submit a report.

## License
This project is licensed under the MIT License.

