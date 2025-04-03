#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace aratamq {
    class Logger {

        private:
            std::shared_ptr<spdlog::logger> logger;
            static Logger* instance;

            Logger(std::string, std::string, std::string, bool);

        public:
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;

            static Logger& Instance();
            static void Initialize(const std::string&, const std::string&, const std::string&, const bool&);
            static void cleanup();

            template<typename... Args>
            void Info(fmt::format_string<Args...> fmt, Args &&... args) {
                logger->info(fmt, std::forward<Args>(args)...);
            }

            template<typename... Args>
            void Error(fmt::format_string<Args...> fmt, Args &&... args) {
                logger->error(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            void Warning(fmt::format_string<Args...> fmt, Args &&... args) {
                logger->warn(fmt, std::forward<Args>(args)...);
            }
            
            
            ~Logger();

    };
}