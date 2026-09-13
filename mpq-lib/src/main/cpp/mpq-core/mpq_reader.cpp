#include "../include/mpq_reader.h"
#include <fstream>
#include <cstring>
#include <zlib.h>
#include <algorithm>

namespace mpq {

MPQReader::MPQReader(const std::string& filepath)
    : filepath_(filepath), is_open_(false) {
    std::memset(&header_, 0, sizeof(header_));
}

MPQReader::~MPQReader() {
    close();
}

bool MPQReader::open() {
    std::ifstream file(filepath_, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Read and validate header
    if (!parse_header()) {
        return false;
    }

    // Parse file table
    if (!parse_file_table()) {
        return false;
    }

    file.close();
    is_open_ = true;
    return true;
}

void MPQReader::close() {
    if (is_open_) {
        file_entries_.clear();
        is_open_ = false;
    }
}

bool MPQReader::is_valid_mpq() const {
    return is_open_ && std::strncmp(header_.magic, "MPQ\x1A", 4) == 0;
}

bool MPQReader::parse_header() {
    std::ifstream file(filepath_, std::ios::binary);
    if (!file.is_open()) return false;

    // Read header
    file.read(reinterpret_cast<char*>(&header_), sizeof(MPQHeader));
    if (file.gcount() != sizeof(MPQHeader)) {
        return false;
    }

    // Validate magic number
    if (std::strncmp(header_.magic, "MPQ\x1A", 4) != 0) {
        return false;
    }

    return true;
}

bool MPQReader::parse_file_table() {
    // TODO: Implement file table parsing
    return true;
}

bool MPQReader::decompress_file(const std::vector<uint8_t>& compressed_data,
                                std::vector<uint8_t>& decompressed_data,
                                uint32_t decompressed_size) {
    decompressed_data.resize(decompressed_size);
    
    z_stream stream = {};
    stream.avail_in = compressed_data.size();
    stream.next_in = (uint8_t*)compressed_data.data();
    stream.avail_out = decompressed_size;
    stream.next_out = decompressed_data.data();

    if (inflateInit2(&stream, -MAX_WBITS) != Z_OK) {
        return false;
    }

    int ret = inflate(&stream, Z_FINISH);
    inflateEnd(&stream);

    return ret == Z_STREAM_END;
}

std::vector<MPQFileEntry> MPQReader::list_files() const {
    return file_entries_;
}

std::vector<std::string> MPQReader::list_directories() const {
    std::vector<std::string> directories;
    // TODO: Extract unique directories from file paths
    return directories;
}

bool MPQReader::extract_file(const std::string& filename, const std::string& output_path) {
    // TODO: Implement file extraction
    return false;
}

bool MPQReader::extract_all(const std::string& output_dir) {
    // TODO: Implement batch extraction
    return false;
}

std::vector<uint8_t> MPQReader::read_file_data(const std::string& filename) {
    std::vector<uint8_t> data;
    // TODO: Implement file reading
    return data;
}

const MPQFileEntry* MPQReader::find_file(const std::string& filename) const {
    for (const auto& entry : file_entries_) {
        if (entry.filename == filename) {
            return &entry;
        }
    }
    return nullptr;
}

} // namespace mpq
