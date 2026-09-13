#include "../include/mpq_search.h"
#include <algorithm>
#include <cctype>

namespace mpq {

MPQSearch::MPQSearch(const MPQReader* reader)
    : reader_(reader) {
}

std::vector<SearchResult> MPQSearch::search_by_name(const std::string& pattern) {
    std::vector<SearchResult> results;
    auto files = reader_->list_files();

    for (const auto& file : files) {
        if (file.filename.find(pattern) != std::string::npos) {
            SearchResult result{
                file.filename,
                file.file_size,
                file.offset,
                calculate_relevance(file.filename, pattern)
            };
            results.push_back(result);
        }
    }

    // Sort by relevance score
    std::sort(results.begin(), results.end(),
              [](const SearchResult& a, const SearchResult& b) {
                  return a.relevance_score > b.relevance_score;
              });

    return results;
}

std::vector<SearchResult> MPQSearch::search_by_regex(const std::string& regex_pattern) {
    std::vector<SearchResult> results;
    auto files = reader_->list_files();
    std::regex pattern(regex_pattern);

    for (const auto& file : files) {
        if (std::regex_search(file.filename, pattern)) {
            SearchResult result{
                file.filename,
                file.file_size,
                file.offset,
                1.0f
            };
            results.push_back(result);
        }
    }

    return results;
}

std::vector<SearchResult> MPQSearch::search_by_extension(const std::string& extension) {
    std::vector<SearchResult> results;
    auto files = reader_->list_files();
    std::string ext = extension;
    
    if (ext[0] != '.') {
        ext = "." + ext;
    }

    for (const auto& file : files) {
        if (file.filename.length() >= ext.length() &&
            file.filename.compare(file.filename.length() - ext.length(),
                                ext.length(), ext) == 0) {
            SearchResult result{
                file.filename,
                file.file_size,
                file.offset,
                1.0f
            };
            results.push_back(result);
        }
    }

    return results;
}

std::vector<SearchResult> MPQSearch::search_by_size(uint32_t min_size, uint32_t max_size) {
    std::vector<SearchResult> results;
    auto files = reader_->list_files();

    for (const auto& file : files) {
        if (file.file_size >= min_size && file.file_size <= max_size) {
            SearchResult result{
                file.filename,
                file.file_size,
                file.offset,
                1.0f
            };
            results.push_back(result);
        }
    }

    return results;
}

std::vector<SearchResult> MPQSearch::full_text_search(const std::vector<uint8_t>& needle) {
    std::vector<SearchResult> results;
    // TODO: Implement full-text search
    return results;
}

float MPQSearch::calculate_relevance(const std::string& filename, const std::string& pattern) {
    // Simple relevance calculation: position of match and case sensitivity
    size_t pos = filename.find(pattern);
    if (pos == std::string::npos) {
        return 0.0f;
    }

    // Exact match or match at beginning gets higher score
    if (filename == pattern) {
        return 1.0f;
    }
    if (pos == 0) {
        return 0.9f;
    }

    return 0.5f + (1.0f - (float)pos / filename.length()) * 0.5f;
}

} // namespace mpq
