#ifndef MPQ_SEARCH_H
#define MPQ_SEARCH_H

#include "mpq_reader.h"
#include <string>
#include <vector>
#include <regex>

namespace mpq {

struct SearchResult {
    std::string filename;
    uint32_t file_size;
    uint32_t offset;
    float relevance_score;
};

class MPQSearch {
public:
    explicit MPQSearch(const MPQReader* reader);

    // Search operations
    std::vector<SearchResult> search_by_name(const std::string& pattern);
    std::vector<SearchResult> search_by_regex(const std::string& regex_pattern);
    std::vector<SearchResult> search_by_extension(const std::string& extension);
    std::vector<SearchResult> search_by_size(uint32_t min_size, uint32_t max_size);
    
    // Full-text search
    std::vector<SearchResult> full_text_search(const std::vector<uint8_t>& needle);
    
private:
    const MPQReader* reader_;
    
    // Helper methods
    float calculate_relevance(const std::string& filename, const std::string& pattern);
};

} // namespace mpq

#endif // MPQ_SEARCH_H
