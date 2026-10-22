# MPQ Android Tool

Android application for reading, extracting, editing, and searching MPQ files with support for multiple ARM architectures.

## ✨ Features

- 📖 **Read MPQ Contents** - Browse and view MPQ archive structure
- 📤 **Extract Files** - Extract individual files from MPQ archives
- ✏️ **Edit/Modify** - Edit and modify files within MPQ archives
- 🔎 **Search** - Search for files and content within MPQ archives
- 🏗️ **Multi-Architecture** - Support for armeabi-v7a, armeabi, and x86
- 💾 **Persistent Storage** - Save changes back to MPQ archives

## 🎯 Supported Architectures

- **armeabi-v7a** (32-bit ARM v7) - Most common, with NEON SIMD optimization
- **armeabi** (Legacy 32-bit ARM) - Older devices support
- **x86** (32-bit Intel) - Emulator and tablet support

## 🏗️ Project Structure

```
mpq-android-tool/
├── app/                          # Android application module
│   ├── src/main/
│   │   ├── java/                 # Java/Kotlin source code
│   │   │   └── com/mpq/android/tool/
│   │   │       └── MainActivity.kt
│   │   ├── cpp/                  # Native C++ code
│   │   ├── AndroidManifest.xml
│   │   └── res/                  # Resources
│   │       ├── layout/
│   │       ├── values/
│   │       └── drawable/
│   └── build.gradle
│
├── mpq-lib/                       # Native MPQ library
│   ├── src/main/cpp/
│   │   ├── CMakeLists.txt
│   │   ├── include/               # Header files
│   │   │   ├── mpq_reader.h
│   │   │   ├── mpq_writer.h
│   │   │   └── mpq_search.h
│   │   ├── mpq-core/              # Core MPQ handling
│   │   │   ├── mpq_reader.cpp
│   │   │   ├── mpq_writer.cpp
│   │   │   ├── mpq_search.cpp
│   │   │   └── mpq_parser.cpp
│   │   └── jni/                   # JNI bindings
│   │       └── mpq_jni.cpp
│   └── build.gradle
│
├── docs/                          # Documentation
│   ├── ARCHITECTURE.md
│   ├── BUILD.md
│   ├── CONTRIBUTING.md
│   └── API.md
│
├── build.gradle
├── CMakeLists.txt
├── settings.gradle
├── .gitignore
├── .gitattributes
└── README.md
```

## 📋 Technology Stack

- **Frontend**: Kotlin/Java with AndroidX
- **Backend**: C++ (Native library via JNI)
- **Build System**: CMake + Gradle
- **NDK**: Android NDK r21+
- **Minimum SDK**: Android 5.0 (API 21)
- **Target SDK**: Android 13 (API 33)
- **Database**: Optional SQLite for metadata

## 🚀 Quick Start

### Prerequisites

- Android Studio 4.0+
- Android NDK r21+
- CMake 3.10+
- Gradle 6.7+
- Java 11+

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/hooligan97/mpq-android-tool.git
   cd mpq-android-tool
   ```

2. **Build the project**
   ```bash
   ./gradlew assembleDebug
   ```

3. **Install on device**
   ```bash
   ./gradlew installDebug
   ```

4. **Run the application**
   ```bash
   adb shell am start -n com.mpq.android.tool/.MainActivity
   ```

## 🔨 Building

### Build All Architectures

```bash
./gradlew assembleDebug
```

### Build Specific Architecture

```bash
# ARM v7a only
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a

# Legacy ARM only
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi

# x86 only
./gradlew assembleDebug -PNDK_ARCHITECTURES=x86

# Multiple architectures
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a,x86
```

### Release Build

```bash
./gradlew assembleRelease
```

### Output Files

- **Debug APK**: `app/build/outputs/apk/debug/app-debug.apk`
- **Release APK**: `app/build/outputs/apk/release/app-release.apk`
- **Native Libraries**: `mpq-lib/build/intermediates/cmake/debug/obj/{abi}/`

## 📱 Usage

### Opening an MPQ File

1. Launch the application
2. Navigate to file browser
3. Select an MPQ archive
4. View file contents and metadata

### Extracting Files

1. Open an MPQ archive
2. Select files to extract
3. Choose destination folder
4. Click "Extract"

### Searching Files

1. Open an MPQ archive
2. Click "Search"
3. Enter search pattern or filename
4. View results

### Editing Files

1. Open an MPQ archive
2. Select a file
3. Click "Edit"
4. Make changes
5. Save and repackage

## 🏛️ Architecture

### MVVM Pattern

```
UI Layer (Activities, Fragments)
    ↓
ViewModel (Business Logic)
    ↓
Repository (Data Access)
    ↓
Native Library (C++ JNI)
    ↓
File System / MPQ Archives
```

### Native C++ Components

#### MPQReader
- Opens and validates MPQ files
- Parses header and file tables
- Extracts file data
- Handles decompression

#### MPQWriter
- Creates new MPQ archives
- Adds/removes files
- Repackages archives
- Recalculates hashes

#### MPQSearch
- Pattern-based search
- Regex support
- File filtering
- Relevance scoring

## 📊 MPQ File Format

### Header Structure (32 bytes)

```
Offset  Size  Field
0       4     Magic ("MPQ\x1A")
4       4     Header Size
8       4     Archive Size
12      2     Format Version
14      2     Sector Size Shift
16      4     Hash Table Offset
20      4     Block Table Offset
24      4     Hash Table Entries
28      4     Block Table Entries
```

### File Entry Structure

```cpp
struct FileEntry {
    std::string filename;
    uint32_t fileSize;
    uint32_t compressedSize;
    uint32_t offset;
    bool isFile;
    uint32_t flags;
};
```

## 🔄 Data Flow

```
User Action (File Selection)
    ↓
Android UI (Activity/Fragment)
    ↓
ViewModel (Handles Logic)
    ↓
Repository (Manages Data)
    ↓
JNI Bridge
    ↓
C++ Native Library
    ↓
File System / MPQ Archive
    ↓
Update UI with Results
```

## 🧪 Testing

### Unit Tests

```bash
./gradlew testDebug
```

### Instrumented Tests

```bash
./gradlew connectedAndroidTest
```

### Native Tests (if configured)

```bash
./gradlew runCppUnit
```

## 📚 Documentation

Detailed documentation is available in the `docs/` directory:

- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** - System design and components
- **[BUILD.md](docs/BUILD.md)** - Building and compilation guide
- **[API.md](docs/API.md)** - API reference and examples
- **[CONTRIBUTING.md](docs/CONTRIBUTING.md)** - Contributing guidelines

## 🔒 Security

- File permissions properly managed
- Runtime permissions for Android 6.0+
- Scoped Storage support for Android 11+
- Safe buffer management in C++
- Input validation on all operations

## 🎨 User Interface

### Material Design 3
- Modern color scheme
- Responsive layouts
- Dark mode support
- Accessibility features

### Main Components
- File Browser
- Archive View
- Search Interface
- File Extraction Dialog
- Settings Screen

## ⚡ Performance Optimizations

### Architecture-Specific
- NEON SIMD on ARMv7a
- Optimized decompression
- Efficient memory management

### General
- Lazy loading of file tables
- Streaming large files
- Background processing with coroutines
- Efficient caching

## 🐛 Known Limitations

- Maximum file size: 4GB (32-bit offsets)
- Compression: Currently zlib only
- Encryption: Not yet supported
- Platform: Android 5.0+ only
- Architectures: ARMv7a, ARMv5te, x86

## 🚀 Future Roadmap

- [ ] Support for bzip2 compression
- [ ] File encryption/decryption
- [ ] 64-bit archive support
- [ ] Advanced search filters
- [ ] File preview functionality
- [ ] Batch processing
- [ ] Cloud sync support
- [ ] Multi-language support

## 📦 Dependencies

### Android Libraries
- androidx.core:core-ktx
- androidx.appcompat:appcompat
- com.google.android.material:material
- androidx.constraintlayout:constraintlayout
- androidx.lifecycle:lifecycle-*
- org.jetbrains.kotlinx:kotlinx-coroutines

### Native Libraries
- zlib (bundled with NDK)
- StormLib (MPQ format)
- bzip2 (optional)

## 🤝 Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](docs/CONTRIBUTING.md) for guidelines.

### How to Contribute

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Code Style

- Follow Google C++ Style Guide
- Use Kotlin naming conventions
- Write meaningful commit messages
- Add tests for new features
- Update documentation

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Majers (hooligan97)**
- GitHub: [@hooligan97](https://github.com/hooligan97)
- Email: [GitHub Contact](https://github.com/hooligan97)

## 🙏 Acknowledgments

- StormLib library authors
- Android Open Source Project
- Material Design team
- Community contributors

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/hooligan97/mpq-android-tool/issues)
- **Discussions**: [GitHub Discussions](https://github.com/hooligan97/mpq-android-tool/discussions)
- **Wiki**: [GitHub Wiki](https://github.com/hooligan97/mpq-android-tool/wiki)

## 🔗 Related Resources

- [MPQ Format Specification](https://en.wikipedia.org/wiki/MPQ_(file_format))
- [Android NDK Documentation](https://developer.android.com/ndk)
- [StormLib Documentation](https://github.com/ladislav-zezula/StormLib)
- [Material Design 3](https://m3.material.io/)

## 📊 Statistics

- **Lines of Code**: ~5,000+ (C++ + Kotlin)
- **Supported Architectures**: 3 (armeabi-v7a, armeabi, x86)
- **Minimum Android Version**: 5.0 (API 21)
- **Target Android Version**: 13 (API 33)
- **Dependencies**: 15+

## 🎓 Learning Resources

This project demonstrates:
- Android NDK development
- JNI integration
- CMake build system
- MVVM architecture
- Coroutines for async operations
- File I/O and binary parsing
- Material Design implementation

---

**Made with ❤️ by Majers**

If you find this project helpful, please consider giving it a ⭐ on GitHub!
