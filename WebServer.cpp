#include "WebServer.h"
#include <iostream>
#include <fstream>

/**
 * @brief Constructs a WebServer instance.
 * 
 * @param serverID Unique identifier for the server.
 * @param maxQueueSize Maximum number of requests the server can handle in its queue.
 * @param logFilePtr Pointer to an ofstream object for logging purposes.
 */
WebServer::WebServer(int serverID, int maxQueueSize, std::ofstream* logFilePtr)
    : id(serverID), currentLoad(0), maxQueueSize(maxQueueSize), logFile(logFilePtr) {
}

/**
 * @brief Checks if the server can accept a new request.
 * 
 * @return true If the server's job queue has space for a new request.
 * @return false If the server's job queue is full.
 */
bool WebServer::canAcceptRequest() const {
    return jobQueue.size() < maxQueueSize;
}

/**
 * @brief Processes jobs in the server's job queue.
 * 
 * If the server has jobs in its queue, it will process the job at the front of the queue,
 * decrementing its processing time and current load. If the job is completed, it will be removed from the queue.
 */
void WebServer::processJobs() {
    if (!jobQueue.empty()) {
        Request& currentJob = jobQueue.front();

        if (logFile && logFile->is_open()) {
            *logFile << "Server " << id << " processing job from " << currentJob.ipIn
                     << " (Time remaining: " << currentJob.processingTime << ")\n";
        } else {
            std::cout << "Server " << id << " processing job from " << currentJob.ipIn
                      << " (Time remaining: " << currentJob.processingTime << ")" << std::endl;
        }

        if (currentJob.processingTime > 0) {
            --currentJob.processingTime;
            --currentLoad;

            if (currentLoad < 0) currentLoad = 0;
        }

        if (currentJob.processingTime <= 0) {
            if (logFile && logFile->is_open()) {
                *logFile << "Server " << id << " completed job from " << currentJob.ipIn << "\n";
            } else {
                std::cout << "Server " << id << " completed job from " << currentJob.ipIn << std::endl;
            }
            jobQueue.pop();
        }
    }
}

/**
 * @brief Checks if the server has any active jobs.
 * 
 * @return true If the server has jobs in its queue.
 * @return false If the server's job queue is empty.
 */
bool WebServer::isActive() const {
    return !jobQueue.empty();
}

/**
 * @brief Adds a request to the server's job queue.
 * 
 * If the server can accept the request, it adds it to the job queue and increments the current load.
 * If the queue is full, the request is rejected and logged.
 * 
 * @param req The request to be added to the server's queue.
 */
void WebServer::addRequest(const Request& req) {
    if (canAcceptRequest()) {
        jobQueue.push(req);
        currentLoad += req.processingTime;
    } else {
        if (logFile && logFile->is_open()) {
            *logFile << "Server " << id << " queue is full, rejected request from "
                     << req.ipIn << "\n";
        } else {
            std::cout << "Server " << id << " queue is full, rejected request from "
                      << req.ipIn << std::endl;
        }
    }
}
