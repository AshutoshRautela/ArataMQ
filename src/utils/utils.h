#ifndef AR_UTILS_H
#define AR_UTILS_H

#include <string>

namespace armq {
    std::string GetCurrentTimestamp();
    void ValidateRoutingPattern(const std::string &pattern);
    void ValidateRoutingKey(const std::string &key);
    std::vector<std::string> SplitWords(const std::string &word, const std::string &splitter);
}

#endif
