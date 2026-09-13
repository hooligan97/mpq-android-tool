# MPQ Android Tool

Android application for reading, extracting, editing, and searching MPQ files with support for multiple ARM architectures.

## Features

- 📖 **Read MPQ Contents** - Browse and view MPQ archive structure
- 📤 **Extract Files** - Extract individual files from MPQ archives
- ✏️ **Edit/Modify** - Edit and modify files within MPQ archives
- 🔍 **Search** - Search for files and content within MPQ archives
- 🏗️ **Multi-Architecture** - Support for armeabi-v7a, armeabi, and x86

## Project Structure

```
mpq-android-tool/
├── app/                          # Android application module
│   ├── src/
│   │   ├── main/
│   │   │   ├── java/             # Java/Kotlin source code
│   │   │   ├── cpp/              # Native C++ code
│   │   │   ├── AndroidManifest.xml
│   │   │   └── res/              # Resources
│   │   └── test/
│   └── build.gradle
├── mpq-lib/                       # Native MPQ library
│   ├── src/
│   │   └── main/
│   │       └── cpp/
│   │           ├── CMakeLists.txt
│   │           ├── mpq-core/     # Core MPQ handling
│   │           ├── mpq-io/       # File I/O operations
│   │           ├── mpq-search/   # Search functionality
│   │           └── jni/          # JNI bindings
│   └── build.gradle
├── docs/                          # Documentation
├── build.gradle
├── settings.gradle
└── CMakeLists.txt
```

## Technology Stack

- **Language**: C++ (native library), Kotlin/Java (Android)
- **Build System**: CMake + Gradle
- **NDK**: Android NDK (for ARM compilation)
- **MPQ Library**: StormLib (based on public implementation)

## Supported Architectures

- armeabi-v7a (32-bit ARM)
- armeabi (legacy 32-bit ARM)
- x86 (32-bit Intel)

## Building

### Prerequisites

- Android Studio 4.0+
- Android NDK r21+
- CMake 3.10+
- Gradle 6.7+

### Build Instructions

```bash
# Clone repository
git clone https://github.com/hooligan97/mpq-android-tool.git
cd mpq-android-tool

# Build with Gradle
./gradlew build

# Build specific ABI
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a
```

## Features Details

### 📖 Read MPQ Contents
- List all files in MPQ archive
- View file metadata (size, offset, hash)
- Display directory structure

### 📤 Extract Files
- Extract single files
- Extract all files with structure
- Extract to storage

### ✏️ Edit/Modify
- Modify file attributes
- Repackage MPQ archives
- Hash recalculation

### 🔍 Search
- Full-text search
- Filename pattern matching
- File type filtering

## Dependencies

- StormLib - MPQ file format handling
- zlib - Compression support
- bzip2 - Additional compression

## License

MIT License
