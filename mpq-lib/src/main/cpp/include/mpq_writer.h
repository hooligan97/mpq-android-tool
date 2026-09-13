#ifndef MPQ_WRITER_H
#define MPQ_WRITER_H

#include "mpq_reader.h"
#include <string>
#include <vector>
#include <map>

namespace mpq {

class MPQWriter {
public:
    explicit MPQWriter(const std::string& filepath);
    ~MPQWriter();

    // Write operations
    bool create_archive(uint32_t estimated_files = 100);
    bool add_file(const std::string& filename, const std::vector<uint8_t>& data);
    bool add_file_from_disk(const std::string& archive_path, const std::string& disk_path);
    bool remove_file(const std::string& filename);
    bool save();
    bool save_as(const std::string& new_filepath);
    
    // Modify existing MPQ
    bool open_existing(const std::string& source_mpq);
    
private:
    std::string filepath_;
    std::map<std::string, std::vector<uint8_t>> files_;
    MPQHeader header_;
    
    // Internal methods
    bool compress_file(const std::vector<uint8_t>& data,
                      std::vector<uint8_t>& compressed_data);
    void calculate_hashes();
};

} // namespace mpq

#endif // MPQ_WRITER_H
