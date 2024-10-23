#include "LoadBalancer.h"
#include <iostream>
#include <limits>

/**
 * @brief Constructs a LoadBalancer instance.
 * 
 * @param servers A priority queue of WebServer objects, representing the servers to handle requests.
 * @param requests A queue of Request objects that the load balancer will distribute.
 * @param runTime The starting time for the load balancer simulation.
 * @param runDuration The duration for which the simulation will run.
 */
LoadBalancer::LoadBalancer(std::priority_queue<WebServer, std::vector<WebServer>, std::greater<>> servers, 
                           std::queue<Request> requests, int runTime, int runDuration)
    : serverQueue(servers), requestQueue(requests), time(runTime), duration(runDuration) {
    std::srand(std::time(nullptr)); 

    minTaskTime = std::numeric_limits<int>::max();
    maxTaskTime = std::numeric_limits<int>::min();

    logFile.open("load_balancer_log.txt", std::ios::out);
    if (logFile.is_open()) {
        logFile << "Load Balancer Log\n";
        logFile << "-----------------\n";
        logFile << "Simulation with " << servers.size() << " servers, running for " << runDuration << " clock cycles.\n\n";
        logFile << "Starting queue size | Ending queue size | Task Time Range\n";
    }
}

/**
 * @brief Distributes requests to the available servers in the load balancer.
 * 
 * Attempts to assign each request in the request queue to the server with the least processing time.
 * If a server cannot accept a request due to a full queue, the request is retained for later processing.
 */
void LoadBalancer::distributeRequests() {
    std::queue<Request> tempRequestQueue;

    while (!requestQueue.empty()) {
        Request req = requestQueue.front();
        requestQueue.pop();

        bool assigned = false;

        WebServer topServer = serverQueue.top();
        serverQueue.pop();

        if (topServer.canAcceptRequest()) {
            topServer.addRequest(req);
            assigned = true;
        }

        serverQueue.push(topServer);

        if (!assigned) {
            tempRequestQueue.push(req);
        }
    }

    while (!tempRequestQueue.empty()) {
        requestQueue.push(tempRequestQueue.front());
        tempRequestQueue.pop();
    }
}

/**
 * @brief Adds a random request to the request queue at intervals.
 * 
 * A new request is generated with a random IP address and processing time. This occurs with a 20% probability.
 */
void LoadBalancer::addRandomRequests() {
    if (std::rand() % 5 == 0) {
        Request newRequest;
        newRequest.ipIn = "192.168.0." + std::to_string(std::rand() % 255);
        newRequest.ipOut = "10.0.0." + std::to_string(std::rand() % 255);
        newRequest.processingTime = 10 + std::rand() % 50;

        if (logFile.is_open()) {
            logFile << "Generated new request: " << newRequest.ipIn << " -> " 
                    << newRequest.ipOut << " (Processing Time: " << newRequest.processingTime << ")\n";
        }

        requestQueue.push(newRequest);
    }
}

/**
 * @brief Resets the minimum and maximum task time range.
 * 
 * This prepares the load balancer for a new cycle of task processing by clearing previous values.
 */
void LoadBalancer::resetTaskTimeRange() {
    minTaskTime = std::numeric_limits<int>::max();
    maxTaskTime = std::numeric_limits<int>::min();
}

/**
 * @brief Logs the cycle information for the current simulation step.
 * 
 * @param currentTime The current time in the simulation.
 * @param startingQueueSize The size of the request queue at the start of the cycle.
 * @param endingQueueSize The size of the request queue at the end of the cycle.
 */
void LoadBalancer::logCycleInfo(int currentTime, int startingQueueSize, int endingQueueSize) {
    if (minTaskTime == std::numeric_limits<int>::max() || maxTaskTime == std::numeric_limits<int>::min()) {
        logFile << startingQueueSize << " | " << endingQueueSize << " | " << "[No tasks]" << std::endl;
    } else {
        logFile << startingQueueSize << " | " << endingQueueSize << " | " 
                << "[" << minTaskTime << "-" << maxTaskTime << "] clock cycles" << std::endl;
    }
}

/**
 * @brief Logs the final status of the load balancer after the simulation ends.
 * 
 * This includes the remaining requests in the queue, the status of each server, and the number of discarded requests.
 */
void LoadBalancer::logFinalStatus() {
    logFile << "\n-- Final Status --\n";
    logFile << "Remaining requests in queue: " << requestQueue.size() << "\n";

    int activeServers = 0, inactiveServers = 0;
    while (!serverQueue.empty()) {
        WebServer server = serverQueue.top();
        serverQueue.pop();

        if (server.isActive()) {
            activeServers++;
        } else {
            inactiveServers++;
        }
    }

    logFile << "Active servers: " << activeServers << ", Inactive servers: " << inactiveServers << "\n";
    logFile << "Total discarded/rejected requests: " << discardedRequests << "\n";
}

/**
 * @brief Runs the load balancer simulation for a specified duration.
 * 
 * Each cycle includes distributing requests, processing jobs in servers, and logging information.
 */
void LoadBalancer::go() {
    for (int currentTime = time; currentTime < time + duration; ++currentTime) {
        int startingQueueSize = requestQueue.size();
        addRandomRequests();

        if (logFile.is_open()) {
            logFile << "\n-- Time: " << currentTime << " --\n";
            logFile << "Starting queue size: " << startingQueueSize << "\n";
        }

        distributeRequests();

        int activeServers = 0, inactiveServers = 0;

        std::vector<WebServer> servers;

        while (!serverQueue.empty()) {
            WebServer server = serverQueue.top();
            serverQueue.pop();

            server.processJobs();

            if (server.isActive()) {
                activeServers++;
            } else {
                inactiveServers++;
            }

            servers.push_back(server);
        }

        for (auto& server : servers) {
            serverQueue.push(server);
        }

        int endingQueueSize = requestQueue.size();
        logCycleInfo(currentTime, startingQueueSize, endingQueueSize);

        if (logFile.is_open()) {
            logFile << "Active servers: " << activeServers << ", Inactive servers: " << inactiveServers << "\n";
        }

        if ((currentTime + 1) % 10 == 0) {
            resetTaskTimeRange();
        }
    }

    logFinalStatus();
}
