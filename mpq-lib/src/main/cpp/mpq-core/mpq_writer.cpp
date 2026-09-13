#include "../include/mpq_writer.h"
#include <fstream>
#include <zlib.h>

namespace mpq {

MPQWriter::MPQWriter(const std::string& filepath)
    : filepath_(filepath) {
    std::memset(&header_, 0, sizeof(header_));
}

MPQWriter::~MPQWriter() {
}

bool MPQWriter::create_archive(uint32_t estimated_files) {
    // Initialize header
    std::memcpy(header_.magic, "MPQ\x1A", 4);
    header_.header_size = 32;
    header_.format_version = 0;
    header_.sector_size_shift = 3; // 2^3 = 8 bytes
    header_.hash_table_entries = estimated_files * 2;
    header_.block_table_entries = estimated_files;
    
    return true;
}

bool MPQWriter::add_file(const std::string& filename, const std::vector<uint8_t>& data) {
    files_[filename] = data;
    return true;
}

bool MPQWriter::add_file_from_disk(const std::string& archive_path, const std::string& disk_path) {
    std::ifstream file(disk_path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
    file.close();

    return add_file(archive_path, data);
}

bool MPQWriter::remove_file(const std::string& filename) {
    files_.erase(filename);
    return true;
}

bool MPQWriter::save() {
    return save_as(filepath_);
}

bool MPQWriter::save_as(const std::string& new_filepath) {
    // TODO: Implement MPQ archive writing
    return false;
}

bool MPQWriter::open_existing(const std::string& source_mpq) {
    MPQReader reader(source_mpq);
    if (!reader.open()) {
        return false;
    }

    auto entries = reader.list_files();
    for (const auto& entry : entries) {
        auto data = reader.read_file_data(entry.filename);
        files_[entry.filename] = data;
    }

    return true;
}

bool MPQWriter::compress_file(const std::vector<uint8_t>& data,
                              std::vector<uint8_t>& compressed_data) {
    // TODO: Implement compression
    return false;
}

void MPQWriter::calculate_hashes() {
    // TODO: Calculate hash table entries
}

} // namespace mpq
