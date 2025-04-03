#include <iostream>
#include <memory>
#include "nlohmann/json.hpp"

#include "config/aratamqconfig.h"
#include "core/broker/broker.h"
#include "core/message/message.h"
#include "utils/logger.h"
#include "utils/utils.h"

int main()
{
    try
    {
        armq::ArataMQConfig config;
        aratamq::Logger::Initialize("ArataMQ", config.logFileDir, config.logFile, true);
        aratamq::Logger::Instance().Info("Starting ArataMQ");

        bool running = true;
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
            // Check for input without blocking
            if (std::cin.peek() != EOF) {
                char input;
                std::cin >> input;
                if (input == 'q' or input == 'Q') {
                    running = false;
                }
            }
        }

        aratamq::Logger::Instance().Info("Shutting down ArataMQ");
        aratamq::Logger::cleanup();
        return 0;
    }
    catch (const std::exception &e)
    {
        aratamq::Logger::Instance().Error("Error {}",std::string(e.what()));
        return 1;
    }
}