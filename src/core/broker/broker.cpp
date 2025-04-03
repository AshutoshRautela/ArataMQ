#include "broker.h"
#include <iostream>
#include <fstream>

#include "../../utils/logger.h"

namespace armq {
    Broker::Broker(std::string queueDir, std::string queueFile): m_queueDir(queueDir), m_queueFile(queueFile) {
        this->initQueue();
    }

    void Broker::initQueue() {
        aratamq::Logger::Instance().Info("Initializing queue over Broker");
        aratamq::Logger::Instance().Info("Queue file: {}", m_queueFile);
        aratamq::Logger::Instance().Info("Queue file dir: {}", m_queueDir);

        std::ofstream queueFileStream(m_queueDir + m_queueFile, std::ios::app);

        if (!queueFileStream.is_open()) {
            aratamq::Logger::Instance().Error("Failed to open queue file");
            return;
        }

        queueFileStream << "Initializing queue over Broker" << std::endl;
        queueFileStream.close();
    }

    void Broker::Produce(const armq::Message& message) {
        aratamq::Logger::Instance().Info("Producing message over Broker {}", message.getMetadata().messageId);
        std::ofstream queueFileStream(m_queueDir + m_queueFile, std::ios::app);

         if (!queueFileStream.is_open()) {
            aratamq::Logger::Instance().Error("Failed to open queue file");
            return;
        }

        queueFileStream << message.Stringify() << std::endl;
        queueFileStream.close();
    }

    Broker::~Broker() {
    }
}