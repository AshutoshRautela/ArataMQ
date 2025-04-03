#ifndef AR_ARATAMQCONFIG_H
#define AR_ARATAMQCONFIG_H

#include <string>
#include <filesystem>

namespace armq {
    class ArataMQConfig {
        private:

        public:
            ArataMQConfig() = default;
            ~ArataMQConfig() = default;

            const std::string logFile = "/aratamq.log";
            const std::string logFileDir = std::filesystem::current_path().string() + "/files";

            const std::string queueFile = "/queue.config";
            const std::string queueFileDir = std::filesystem::current_path().string() + "/files";
    };
}
#endif  