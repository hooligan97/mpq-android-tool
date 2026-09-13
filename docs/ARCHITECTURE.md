# MPQ Android Tool - Documentation

## Overview

MPQ Android Tool is a complete Android application for reading, extracting, editing, and searching MPQ (MoPaQ) files on mobile devices. It provides support for multiple ARM architectures commonly used in Android devices.

## Architecture

### Module Structure

#### 1. **app/** - Android Application
Main Android application module containing:
- UI layer (Kotlin/Java)
- Activity and Fragment management
- User interface layouts and resources
- Application lifecycle management

**Key Files:**
- `MainActivity.kt` - Main activity entry point
- `activity_main.xml` - Main layout
- `AndroidManifest.xml` - App permissions and configuration

#### 2. **mpq-lib/** - Native C++ Library
Native Android library containing all MPQ file handling logic:
- Core MPQ parsing and reading
- File extraction and writing
- Search and indexing functionality
- JNI bindings for Java/Kotlin interaction

**Key Directories:**
- `include/` - Header files for C++ classes
- `mpq-core/` - Core MPQ implementation
- `jni/` - JNI bridge between Java and C++
- `CMakeLists.txt` - Build configuration

### Build System

**Gradle** - Used for Android build management
- `build.gradle` - Top-level build configuration
- `settings.gradle` - Module inclusion
- `app/build.gradle` - App-specific configuration
- `mpq-lib/build.gradle` - Library-specific configuration

**CMake** - Used for native C++ compilation
- `CMakeLists.txt` - Top-level CMake configuration
- `mpq-lib/src/main/cpp/CMakeLists.txt` - Library CMake configuration

## Supported Architectures

The project compiles for three main ARM architectures:

### 1. **armeabi-v7a** (32-bit ARM v7)
- Most common architecture
- Supports NEON SIMD instructions
- Optimized with: `-march=armv7-a -mfpu=neon`
- Compatible with: Most modern Android devices (2010+)

### 2. **armeabi** (Legacy 32-bit ARM)
- Older ARM architecture
- Optimized with: `-march=armv5te`
- Compatible with: Older Android devices
- Fallback for devices without v7a support

### 3. **x86** (32-bit Intel)
- Intel-based Android devices (rare)
- Optimized with: `-march=i686`
- Used in: Some emulators and Intel-based tablets

## C++ Classes and Components

### MPQReader Class
**File:** `mpq-core/mpq_reader.cpp` / `include/mpq_reader.h`

**Responsibilities:**
- Open and validate MPQ archives
- Parse MPQ header and file tables
- List files and directories
- Extract individual files
- Read file data
- Decompress file data using zlib

**Key Methods:**
```cpp
bool open();                                    // Open MPQ file
void close();                                   // Close MPQ file
bool is_valid_mpq() const;                      // Validate MPQ format
std::vector<MPQFileEntry> list_files() const;  // List all files
bool extract_file(filename, output_path);      // Extract single file
bool extract_all(output_dir);                  // Extract all files
```

### MPQWriter Class
**File:** `mpq-core/mpq_writer.cpp` / `include/mpq_writer.h`

**Responsibilities:**
- Create new MPQ archives
- Add files to archives
- Remove files from archives
- Modify existing MPQ files
- Compress and save archives

**Key Methods:**
```cpp
bool create_archive(estimated_files);                    // Create new MPQ
bool add_file(filename, data);                           // Add file
bool add_file_from_disk(archive_path, disk_path);       // Add from disk
bool remove_file(filename);                              // Remove file
bool save();                                             // Save archive
bool open_existing(source_mpq);                          // Modify existing
```

### MPQSearch Class
**File:** `mpq-core/mpq_search.cpp` / `include/mpq_search.h`

**Responsibilities:**
- Search files by name pattern
- Search using regular expressions
- Search by file extension
- Search by file size range
- Perform full-text search
- Calculate relevance scores

**Key Methods:**
```cpp
std::vector<SearchResult> search_by_name(pattern);       // Name search
std::vector<SearchResult> search_by_regex(pattern);      // Regex search
std::vector<SearchResult> search_by_extension(ext);      // Extension search
std::vector<SearchResult> search_by_size(min, max);      // Size search
std::vector<SearchResult> full_text_search(needle);      // Full-text search
```

### JNI Bindings
**File:** `jni/mpq_jni.cpp`

**Purpose:** Bridge between Java/Kotlin and C++ code

**Exported Functions:**
```cpp
// Reader operations
JNIEXPORT jlong Java_com_mpq_android_tool_MPQReader_nativeOpenMPQ(...);
JNIEXPORT void Java_com_mpq_android_tool_MPQReader_nativeCloseMPQ(...);
JNIEXPORT jobjectArray Java_com_mpq_android_tool_MPQReader_nativeListFiles(...);
JNIEXPORT jboolean Java_com_mpq_android_tool_MPQReader_nativeExtractFile(...);

// Search operations
JNIEXPORT jobjectArray Java_com_mpq_android_tool_MPQSearch_nativeSearchByName(...);
```

## MPQ File Format

### Header Structure (32 bytes)
```
Offset  Size  Field
0       4     Magic Number ("MPQ\x1A")
4       4     Header Size
8       4     Archive Size
12      2     Format Version
14      2     Sector Size Shift (2^n = sector size)
16      4     Hash Table Offset
20      4     Block Table Offset
24      4     Hash Table Entries
28      4     Block Table Entries
```

### File Entry Structure
```cpp
struct MPQFileEntry {
    std::string filename;      // File path/name
    uint32_t file_size;        // Original size
    uint32_t compressed_size;  // Compressed size
    uint32_t offset;           // Offset in archive
    bool is_file;              // Is regular file
    uint32_t flags;            // File flags
};
```

## Building the Project

### Prerequisites
1. Android Studio 4.0 or higher
2. Android NDK r21 or higher
3. CMake 3.10 or higher
4. Gradle 6.7 or higher
5. Android SDK API level 21+

### Build Steps

#### 1. Clone Repository
```bash
git clone https://github.com/hooligan97/mpq-android-tool.git
cd mpq-android-tool
```

#### 2. Build Debug APK (All architectures)
```bash
./gradlew assembleDebug
```

#### 3. Build for Specific Architecture
```bash
# armeabi-v7a only
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a

# x86 only
./gradlew assembleDebug -PNDK_ARCHITECTURES=x86

# Multiple architectures
./gradlew assembleDebug -PNDK_ARCHITECTURES=armeabi-v7a,x86
```

#### 4. Build Release APK
```bash
./gradlew assembleRelease
```

#### 5. Run on Device/Emulator
```bash
./gradlew installDebug
adb shell am start -n com.mpq.android.tool/.MainActivity
```

### Build Outputs
- Debug APK: `app/build/outputs/apk/debug/app-debug.apk`
- Release APK: `app/build/outputs/apk/release/app-release.apk`
- Native Libraries: `mpq-lib/build/intermediates/cmake/debug/obj/{abi}/`

## Dependencies

### Android Dependencies
- **androidx.core:core-ktx** - AndroidX core library
- **androidx.appcompat:appcompat** - Compatibility library
- **com.google.android.material:material** - Material Design components
- **androidx.constraintlayout:constraintlayout** - Layout management
- **androidx.lifecycle:lifecycle-runtime-ktx** - Lifecycle management
- **org.jetbrains.kotlinx:kotlinx-coroutines** - Async operations

### Native Dependencies
- **zlib** - Compression library (bundled with NDK)
- **StormLib** - MPQ format handling (to be integrated)
- **bzip2** - Additional compression (optional)

## Feature Implementation Roadmap

### Phase 1: Core Reading ✓
- [x] MPQ header parsing
- [x] File table parsing
- [x] File listing
- [ ] File data reading
- [ ] Decompression (zlib)

### Phase 2: Extraction
- [ ] Single file extraction
- [ ] Batch extraction
- [ ] Directory structure preservation
- [ ] Progress tracking

### Phase 3: Search & Browse
- [x] Name pattern search
- [x] Regex search
- [x] Extension filtering
- [x] Size filtering
- [ ] Full-text search
- [ ] Directory tree view

### Phase 4: Editing
- [ ] File addition
- [ ] File removal
- [ ] Archive recompression
- [ ] Hash recalculation
- [ ] Archive saving

### Phase 5: UI/UX
- [ ] File browser interface
- [ ] Search interface
- [ ] Preview functionality
- [ ] Settings screen
- [ ] Progress dialogs

## Testing

### Unit Tests
```bash
./gradlew testDebug
```

### Instrumented Tests
```bash
./gradlew connectedAndroidTest
```

### Native Tests (if added)
```bash
./gradlew runCppUnit
```

## Performance Considerations

### Architecture-Specific Optimizations

1. **armeabi-v7a**
   - NEON SIMD for bulk operations
   - Optimized decompression
   - Better performance on modern devices

2. **armeabi**
   - Baseline ARM5 compatibility
   - Slower but works on older devices
   - Used as fallback

3. **x86**
   - Intel Atom optimization
   - Used primarily for development/emulation

### Memory Management
- Streaming large files instead of loading fully
- Lazy loading of hash/block tables
- Efficient buffer reuse

## Security Considerations

1. **File Permissions**
   - Manage storage permissions properly
   - Handle runtime permissions on Android 6.0+
   - Use Scoped Storage on Android 11+

2. **Buffer Overflow Protection**
   - Validate file sizes before allocation
   - Boundary checks on memory operations
   - Safe string handling in C++

3. **Input Validation**
   - Validate MPQ headers
   - Check file table integrity
   - Verify compression data

## Contributing

1. Fork the repository
2. Create a feature branch
3. Implement changes with tests
4. Submit a pull request

## License

MIT License - See LICENSE file for details
