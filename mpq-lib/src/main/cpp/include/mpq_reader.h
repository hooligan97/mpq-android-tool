#ifndef MPQ_READER_H
#define MPQ_READER_H

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

namespace mpq {

struct MPQHeader {
    char magic[4];           // "MPQ\x1A"
    uint32_t header_size;
    uint32_t archive_size;
    uint16_t format_version;
    uint16_t sector_size_shift;
    uint32_t hash_table_offset;
    uint32_t block_table_offset;
    uint32_t hash_table_entries;
    uint32_t block_table_entries;
};

struct MPQFileEntry {
    std::string filename;
    uint32_t file_size;
    uint32_t compressed_size;
    uint32_t offset;
    bool is_file;
    uint32_t flags;
};

class MPQReader {
public:
    explicit MPQReader(const std::string& filepath);
    ~MPQReader();

    // Read operations
    bool open();
    void close();
    bool is_valid_mpq() const;
    
    // File listing
    std::vector<MPQFileEntry> list_files() const;
    std::vector<std::string> list_directories() const;
    
    // File extraction
    bool extract_file(const std::string& filename, const std::string& output_path);
    bool extract_all(const std::string& output_dir);
    std::vector<uint8_t> read_file_data(const std::string& filename);
    
    // File information
    const MPQFileEntry* find_file(const std::string& filename) const;
    uint32_t get_archive_size() const { return header_.archive_size; }
    
private:
    std::string filepath_;
    MPQHeader header_;
    std::vector<MPQFileEntry> file_entries_;
    bool is_open_;
    
    // Internal methods
    bool parse_header();
    bool parse_file_table();
    bool decompress_file(const std::vector<uint8_t>& compressed_data,
                        std::vector<uint8_t>& decompressed_data,
                        uint32_t decompressed_size);
};

} // namespace mpq

#endif // MPQ_READER_H
