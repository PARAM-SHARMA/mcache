# MCache Project

## Overview
**MCache** is a simple in-memory key-value cache implemented in C++. It supports basic operations like storing, updating, retrieving, and deleting integer values associated with string keys. Additionally, it keeps usage statistics such as cache hits, misses, and the number of stored keys.  

A command-line interface (CLI) is provided to interact with the cache in real-time.

---

## Features
- Thread-safe cache using `std::mutex`.
- Basic cache operations:
  - `GET <key>` — Retrieve a value.
  - `SET <key> <value>` — Update an existing value.
  - `ADD <key> <value>` — Add a new key-value pair if it does not exist.
  - `DEL <key>` — Delete a key-value pair.
  - `STAT` — Show cache statistics (hits, misses, number of keys).
  - `EXIT` — Quit the CLI.
- Optional test suite using assertions to validate cache behavior.

---

## Project Structure

```
MCacheProject/
├─ CMakeLists.txt       # CMake build configuration
├─ include/             # Header files
│  ├─ MCache.h
│  └─ cli.h
├─ src/                 # Source files
│  ├─ MCache.cpp
│  ├─ cli.cpp
│  └─ main.cpp
└─ tests/               # Test files
└─ test_cache.cpp

```

---

## Requirements
- C++17 or higher
- [CMake](https://cmake.org/) 3.10+
- Compatible compiler: GCC, Clang, MSVC

---

## Build Instructions

1. Clone the repository:

```bash
git clone <repository-url>
cd MCacheProject
````

2. Create a build directory:

```bash
mkdir build
cd build
```

3. Generate the build system using CMake:

```bash
cmake ..
```

4. Build the project:

```bash
cmake --build .
```

5. Run the CLI application:

```bash
./MCacheApp       # Linux/macOS
MCacheApp.exe     # Windows
```

6. Run the tests:

```bash
ctest
```

---

## Example CLI Usage

```text
MCache > add key1 100
true 100
MCache > get key1
true 100
MCache > set key1 200
true 200
MCache > del key1
true
MCache > stat
hits -> 2
misses -> 0
keys -> 0
MCache > exit
```

---

## Future Improvements

* Add support for **TTL (time-to-live)** for keys to expire automatically.
* Implement **LRU (Least Recently Used) eviction policy** for memory-limited scenarios.
* Allow **values other than integers** (e.g., strings or custom objects).
* Add **persistent storage** option to save the cache between sessions.
* Implement **unit testing framework** (like Google Test) for more robust testing.
* Improve CLI experience with command history and auto-completion.

---

## Notes

* The `build/` directory is **not included in Git**; users should generate it locally using CMake.
* The cache is **thread-safe** using `std::mutex`, but operations are blocking and could be improved with more advanced concurrency primitives.
