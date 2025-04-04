#include "utils.h"
#include <iomanip>
#include <sstream>
namespace armq
{
    std::string GetCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    void ValidateRoutingPattern(const std::string &pattern)
    {
        if (pattern.empty())
            throw std::invalid_argument("Pattern cannot be empty");

        // Check for invalid characters
        if (pattern.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.*#") != std::string::npos)
        {
            throw std::invalid_argument("Invalid characters in pattern");
        }

        // Check for multiple # symbols
        size_t firstHash = pattern.find('#');
        if (firstHash != std::string::npos && pattern.find('#', firstHash + 1) != std::string::npos)
        {
            throw std::invalid_argument("Pattern can contain only one # symbol");
        }

        // Check # position (must be at start or end)
        if (firstHash != std::string::npos &&
            firstHash != 0 &&
            firstHash != pattern.length() - 1)
        {
            throw std::invalid_argument("Wildcard # must be at start or end of pattern");
        }

        // Check * placement (must be a complete word)
        size_t pos = 0;
        while ((pos = pattern.find('*', pos)) != std::string::npos)
        {
            if (pos > 0 && pattern[pos - 1] != '.' && pos < pattern.length() - 1 && pattern[pos + 1] != '.')
            {
                throw std::invalid_argument("Wildcard * must be a complete word");
            }
            pos++;
        }
    }

    std::vector<std::string> SplitWords(const std::string &word, const std::string &splitter)
    {
        std::vector<std::string> words;
        std::string::size_type start = 0;
        std::string::size_type end = word.find(splitter);

        while (end != std::string::npos) {
            words.push_back(word.substr(start, end - start));
            start = end + splitter.length();
            end = word.find(splitter, start);
        }

        words.push_back(word.substr(start));
        return std::move(words);        
    }
}