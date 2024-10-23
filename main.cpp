/**
 * @file main.cpp
 * @brief This file contains the main function to initialize and run the load balancer simulation.
 */

#include <queue>
#include <vector>
#include <iostream>
#include "LoadBalancer.h"

using std::cout, std::endl;

/**
 * @brief Main function that sets up and runs the load balancer simulation.
 *
 * Initializes a set number of web servers and generates a random set of requests.
 * The LoadBalancer then processes these requests over a specified duration of time.
 *
 * @return int Returns 0 upon successful execution of the simulation.
 */
int main() {
    int numServers = 100;
    int runTime = 0;
    int runDuration = 100000;
    
    std::cin >> numServers >> runTime >> runDuration;
    

    std::priority_queue<WebServer, std::vector<WebServer>, std::greater<>> serverQueue;

    std::ofstream logFile("server_log.txt");

    /**
     * @brief Loop to initialize the web servers.
     * 
     * Each server is initialized with an ID and a pointer to the log file for logging activities.
     * The servers are then added to the priority queue.
     */
    for (int i = 0; i < numServers; ++i) {
        WebServer server(i, 10, &logFile);
        serverQueue.push(server);
    }

    std::queue<Request> requestQueue;

    int totalRequests = 100 * numServers;

    std::srand(std::time(nullptr));

    /**
     * @brief Loop to generate and add random requests to the request queue.
     * 
     * Each request is given random input and output IP addresses and a random processing time.
     * Requests are logged in the log file.
     */
    for (int i = 0; i < totalRequests; ++i) {
        Request newRequest;
        newRequest.ipIn = "192.168.0." + std::to_string(std::rand() % 255);
        newRequest.ipOut = "10.0.0." + std::to_string(std::rand() % 255);
        newRequest.processingTime = 10 + std::rand() % 50;

        requestQueue.push(newRequest);

        logFile << "Generated request: " << newRequest.ipIn << " -> " 
                << newRequest.ipOut << " (Processing Time: " << newRequest.processingTime << ")\n";
    }

    LoadBalancer loadBalancer(serverQueue, requestQueue, runTime, runDuration);
    
    loadBalancer.go();

    std::cout << "Simulation completed!" << std::endl;
    
    return 0;
}
