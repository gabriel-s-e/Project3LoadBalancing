#include <string>
#include <iostream>
#pragma once
struct Request {
    std::string ipIn;
    std::string ipOut;
    int processingTime;
    std::string jobType;

    Request(const std::string& clientIP, const std::string& serverIP, int time, const std::string& type)
        : ipIn(clientIP), ipOut(serverIP), processingTime(time), jobType(type) {}

    Request() : ipIn("0.0.0.0"), ipOut("0.0.0.0"), processingTime(0), jobType("UNKNOWN") {}

    void print() const {
        std::cout << "Request from " << ipIn << " to " << ipOut
                  << " | Processing Time: " << processingTime
                  << " | Job Type: " << jobType << std::endl;
    }
};
