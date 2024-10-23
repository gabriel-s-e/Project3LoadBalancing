#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "WebServer.h"
#include "Request.h"
#include <queue>
#include <fstream>
#include <vector>
#include <limits>

class LoadBalancer {
public:
    LoadBalancer(std::priority_queue<WebServer, std::vector<WebServer>, std::greater<>> servers, 
                 std::queue<Request> requests, int runTime, int runDuration);


    void addRandomRequests();

    void logFinalStatus();

    void distributeRequests();

    void logCycleInfo(int currentTime, int startingQueueSize, int endingQueueSize);

    void go();

private:
    void resetTaskTimeRange();
    std::priority_queue<WebServer, std::vector<WebServer>, std::greater<>> serverQueue;
    std::queue<Request> requestQueue;
    int time;
    int duration;
    int minTaskTime;
    int maxTaskTime;
    int discardedRequests = 0;
    std::ofstream logFile;
};

#endif
