#ifndef AR_ARATAMQCONFIG_H
#define AR_ARATAMQCONFIG_H

#include <string>
#include <filesystem>

#define ARATA_DIR "/Users/arautela/Documents/CursorTest/ArataMQ"

namespace armq {
    class ArataMQConfig {
        private:

        public:
            ArataMQConfig() = default;
            ~ArataMQConfig() = default;

            const std::string logFile = "/aratamq.log";
            const std::string logFileDir = std::string(ARATA_DIR) + "/files";

            const std::string queueFile = "/queue.config";
            const std::string queueFileDir = std::string(ARATA_DIR) + "/files";
    };
}
#endif  