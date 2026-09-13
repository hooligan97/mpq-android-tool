# MPQ Android Tool - API Reference

## Java/Kotlin API

### MPQReader Class

```kotlin
class MPQReader(val filepath: String) {
    // Properties
    val isValid: Boolean
    val archiveSize: Long
    val fileCount: Int
    
    // Methods
    fun open(): Boolean
    fun close()
    fun listFiles(): List<String>
    fun listDirectories(): List<String>
    fun extractFile(filename: String, outputPath: String): Boolean
    fun extractAll(outputDir: String): Boolean
    fun getFileInfo(filename: String): FileInfo?
    fun readFileData(filename: String): ByteArray?
}

data class FileInfo(
    val filename: String,
    val fileSize: Long,
    val compressedSize: Long,
    val offset: Long,
    val isFile: Boolean,
    val flags: Int
)
```

### MPQWriter Class

```kotlin
class MPQWriter(val filepath: String) {
    // Methods
    fun createArchive(estimatedFiles: Int = 100): Boolean
    fun addFile(filename: String, data: ByteArray): Boolean
    fun addFileFromDisk(archivePath: String, diskPath: String): Boolean
    fun removeFile(filename: String): Boolean
    fun save(): Boolean
    fun saveAs(newFilepath: String): Boolean
    fun openExisting(sourceMpq: String): Boolean
}
```

### MPQSearch Class

```kotlin
class MPQSearch(val reader: MPQReader) {
    // Methods
    fun searchByName(pattern: String): List<SearchResult>
    fun searchByRegex(pattern: String): List<SearchResult>
    fun searchByExtension(extension: String): List<SearchResult>
    fun searchBySize(minSize: Long, maxSize: Long): List<SearchResult>
    fun fullTextSearch(needle: ByteArray): List<SearchResult>
}

data class SearchResult(
    val filename: String,
    val fileSize: Long,
    val offset: Long,
    val relevanceScore: Float
)
```

## Usage Examples

### Read MPQ File

```kotlin
val reader = MPQReader("/path/to/archive.mpq")
if (reader.open()) {
    // List all files
    val files = reader.listFiles()
    for (file in files) {
        println(file)
    }
    
    // Get file info
    val info = reader.getFileInfo("path/to/file.txt")
    println("Size: ${info?.fileSize}")
    
    reader.close()
} else {
    println("Failed to open MPQ")
}
```

### Extract Files

```kotlin
val reader = MPQReader("/path/to/archive.mpq")
if (reader.open()) {
    // Extract single file
    val success = reader.extractFile(
        "path/to/file.txt",
        "/storage/emulated/0/file.txt"
    )
    
    // Extract all files
    val allSuccess = reader.extractAll("/storage/emulated/0/mpq_contents")
    
    reader.close()
}
```

### Search in MPQ

```kotlin
val reader = MPQReader("/path/to/archive.mpq")
if (reader.open()) {
    val search = MPQSearch(reader)
    
    // Search by filename pattern
    val results = search.searchByName("*.txt")
    for (result in results) {
        println("${result.filename} - ${result.fileSize} bytes")
    }
    
    // Search by regex
    val regexResults = search.searchByRegex(".*\\.\\w{3}$")
    
    // Search by extension
    val txtFiles = search.searchByExtension("txt")
    
    reader.close()
}
```

### Create/Edit MPQ

```kotlin
val writer = MPQWriter("/path/to/new_archive.mpq")

// Create new archive
if (writer.createArchive(estimatedFiles = 50)) {
    // Add files
    val data = "Hello, World!".toByteArray()
    writer.addFile("hello.txt", data)
    
    // Add from disk
    writer.addFileFromDisk(
        "archive_path/file.bin",
        "/local/path/file.bin"
    )
    
    // Save archive
    writer.save()
}
```

## JNI Method Signatures

### MPQReader Native Methods

```kotlin
@Suppress("FunctionName")
class MPQReader {
    external fun nativeOpenMPQ(filepath: String): Long
    external fun nativeCloseMPQ(handle: Long)
    external fun nativeListFiles(handle: Long): Array<String>
    external fun nativeExtractFile(handle: Long, filename: String, output: String): Boolean
    external fun nativeGetFileInfo(handle: Long, filename: String): FileInfo?
}
```

### MPQWriter Native Methods

```kotlin
@Suppress("FunctionName")
class MPQWriter {
    external fun nativeCreateArchive(filepath: String, estimatedFiles: Int): Long
    external fun nativeAddFile(handle: Long, filename: String, data: ByteArray): Boolean
    external fun nativeSave(handle: Long): Boolean
    external fun nativeClose(handle: Long)
}
```

### MPQSearch Native Methods

```kotlin
@Suppress("FunctionName")
class MPQSearch {
    external fun nativeSearchByName(handle: Long, pattern: String): Array<String>
    external fun nativeSearchByRegex(handle: Long, pattern: String): Array<String>
    external fun nativeSearchByExtension(handle: Long, extension: String): Array<String>
}
```

## Error Handling

### Return Codes

```cpp
enum class MPQError {
    SUCCESS = 0,
    FILE_NOT_FOUND = 1,
    INVALID_MPQ = 2,
    COMPRESSION_ERROR = 3,
    DECOMPRESSION_ERROR = 4,
    WRITE_ERROR = 5,
    READ_ERROR = 6,
    OUT_OF_MEMORY = 7,
    INVALID_PARAMETER = 8
};
```

### Exception Handling

```kotlin
try {
    val reader = MPQReader("/path/to/file.mpq")
    if (!reader.open()) {
        throw IOException("Failed to open MPQ file")
    }
    
    val files = reader.listFiles()
    // Process files
    
    reader.close()
} catch (e: IOException) {
    Log.e("MPQ", "Error: ${e.message}")
} catch (e: Exception) {
    Log.e("MPQ", "Unexpected error: ${e.message}")
}
```

## Constants

### Magic Numbers
```cpp
const char MPQ_MAGIC[4] = {'M', 'P', 'Q', 0x1A};
const uint32_t MPQ_HEADER_SIZE = 32;
const uint16_t MPQ_FORMAT_VERSION = 0;
```

### Flags
```cpp
enum class FileFlags : uint32_t {
    IMPLODE = 0x00000100,       // PKware imploded
    COMPRESS = 0x00000200,       // Compressed
    ENCRYPTED = 0x00010000,      // Encrypted
    FIX_KEY = 0x00020000,        // Key is fixed
    PATCH_FILE = 0x00100000,     // Patch file
    SINGLE_UNIT = 0x01000000,   // Single unit
    DELETE_MARKER = 0x02000000,  // Delete marker
    SECTOR_CRC = 0x04000000,    // Sector CRC
    UNKNOWN = 0x08000000         // Unknown
};
```

## Performance Considerations

### Memory Usage
- Large MPQ files: Stream reading to avoid loading entire file
- Hash tables: Cached in memory for fast lookups
- File extraction: Temporary buffer for decompression

### Thread Safety
- Reader is NOT thread-safe
- Create separate reader instances for parallel reads
- Use locks for concurrent access if needed

### Optimization Tips
1. Reuse reader instance for multiple operations
2. Use pattern search instead of regex when possible
3. Extract files in batches for better performance
4. Consider file size when planning memory allocation

## Limitations

1. **File Size**: Maximum 4GB (32-bit file offsets)
2. **Compression**: Only zlib compression supported initially
3. **Encryption**: Not supported in current version
4. **Platform**: Android 5.0+ only
5. **Architectures**: armeabi-v7a, armeabi, x86

## Future API Changes

- Support for async operations (Coroutines)
- Reactive API (RxJava)
- Better progress tracking
- File streaming support
- Multi-threaded extraction

## Migration Guide

### From v0.x to v1.0

```kotlin
// Old API
val reader = MPQReader()
reader.setFilepath("/path/to/file.mpq")
reader.open()

// New API
val reader = MPQReader("/path/to/file.mpq")
reader.open()
```
