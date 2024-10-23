#ifndef WEBSERVER_H
#include "Request.h"
#include <queue>
#include <string>
#include <iostream>

class WebServer {
private:
    int id;
    int currentLoad;
    std::queue<Request> jobQueue;
    int maxQueueSize;
    std::ofstream* logFile;  // Pointer to a log file for logging server activities

public:
    WebServer(int serverID, int maxQueueSize, std::ofstream* logFilePtr = nullptr);

    void addRequest(const Request& req);
    void processJobs();
    bool canAcceptRequest() const;

    int getID() const { return id; }
    int getCurrentLoad() const { return currentLoad; }
    size_t getQueueSize() const { return jobQueue.size(); }
    bool isActive() const;

    bool operator>(const WebServer& other) const {
        return currentLoad > other.currentLoad;
    }

    bool operator<(const WebServer& other) const {
        return currentLoad < other.currentLoad;
    }

    bool operator>=(const WebServer& other) const {
        return currentLoad >= other.currentLoad;
    }

    bool operator<=(const WebServer& other) const {
        return currentLoad <= other.currentLoad;
    }
};

#endif