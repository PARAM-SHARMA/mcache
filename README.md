# MCache Project

## Overview
**MCache** is a simple in-memory key-value cache implemented in C++.
It supports typed values (`int`, `float`, and `string`) associated with string keys.

The cache provides basic CRUD operations, usage statistics, and safe type validation.
Invalid type/value combinations (e.g., inserting `"hello"` as an `int`) do **not crash** the program — they fail gracefully.

A command-line interface (CLI) is provided to interact with the cache in real-time.

---

## Features
* Thread-safe cache using `std::mutex`
* Supports typed values:

  * `int`
  * `float`
  * `string` (supports multi-word strings)
* Safe parsing with error handling (no crashes on invalid input)
* Basic cache operations:

  * `GET <key>` — Retrieve a value
  * `SET <type> <key> <value>` — Update an existing value
  * `ADD <type> <key> <value>` — Add a new key-value pair if it does not exist
  * `DEL <key>` — Delete a key-value pair
  * `STAT` — Show cache statistics (hits, misses, number of keys)
  * `EXIT` — Quit the CLI
* Optional test suite using assertions to validate cache behavior

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
* C++17 or higher
* [CMake](https://cmake.org/) 3.10+
* Compatible compiler: GCC, Clang, MSVC

---

## Build Instructions

### 1. Clone the repository

```bash
git clone <repository-url>
cd MCacheProject
```

### 2. Create build directory

```bash
mkdir build
cd build
```

### 3. Generate build files

```bash
cmake ..
```

### 4. Build

```bash
cmake --build .
```

### 5. Run the CLI

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

### Add values

```text
MCache > add int a 3
true 3

MCache > add float pi 3.14
true 3.14

MCache > add string greeting Hello world from MCache
true Hello world from MCache
```

### Retrieve values

```text
MCache > get a
true {int} 3

MCache > get greeting
true {string} Hello world from MCache
```

### Update values

```text
MCache > set string a Updated value
true Updated value
```

### Invalid type handling (safe failure)

```text
MCache > add int x hello
false
```

The program does not crash. Invalid type/value combinations are rejected safely.

---

## Statistics Example

```text
MCache > stat
hits -> 2
misses -> 1
keys -> 3
```

---

## Future Improvements

* Add **TTL (time-to-live)** support for automatic key expiration
* Implement **LRU eviction policy**
* Convert CLI into a **TCP server**
* Add persistence (snapshot or append-only file)
* Integrate a unit testing framework (e.g., Google Test)
* Improve CLI with history and auto-completion

---

## Notes

* The `build/` directory is not included in Git.
* The cache is thread-safe via `std::mutex`, but operations are blocking.
* Designed as a learning project to explore data structures, system design, and modern C++.
