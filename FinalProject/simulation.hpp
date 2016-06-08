#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <iostream> // std
#include <sstream>  // stringstream
#include <random>   // random number generator
#include <thread>   // sleeping thread
#include <ctime>    // time
#include <vector>   // vector

#include "msg.hpp"  // classes managing outputs

class Simulation {
public:
    // generate simulation data
	static void simulateData(const std::string& dataHeroPath, const unsigned& noOfFiles, const std::string& dataType);
};

#endif /* SIMULATION_HPP */