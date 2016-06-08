#include "simulation.hpp" // simulation class

/* ------------------------------------------------------------------------
* CLASS SIMULATING INPUT DATA
* -----------------------------------------------------------------------*/

void Simulation::simulateData(const std::string& dataHeroPath, const unsigned& noOfFiles, const std::string& dataType) {

	ScreenMsg::print("[SIMULATION] Simulating data...\n");

	// declare some variables
	std::vector<std::string> staff{"Staff1", "Staff2"};
	std::vector<std::string> projects{"A", "B"};

	// initialize random seed
	std::mt19937_64 eng{std::random_device{}()};
	// files wll contain 5-10 measurements
	std::uniform_int_distribution<> randNoOfValues{5, 10};
	// files will contain values from 50 to 100
	std::uniform_real_distribution<> randValuesReal{50, 100};
	std::uniform_int_distribution<> randValuesInt{50, 100};	
	// datastams will be simulated by making loop to sleep for some random time
	std::uniform_int_distribution<> randSleepTime{100, 1000};

	// simulate data
  	for (unsigned n{}; n < noOfFiles; ++n) {
	  	for (size_t k{}; k < staff.size(); ++k) {
	  		for (size_t l{}; l < projects.size(); ++l) {
			    std::ostringstream stringStream;
			    // print table header
			    stringStream << "Staff: "   << staff[k]     	<< std::endl 
							 << "Project: " << projects[l]   	<< std::endl
							 << "-----------------------------" << std::endl;
				int noOfPoints{randNoOfValues(eng)}; 
				for (int i{0}; i < noOfPoints; ++i) {
					std::time_t time_now = std::time(NULL);
					stringStream << time_now; // timestamp
					if (dataType == "double" || dataType == "int") { 
						stringStream << " " << (dataType == "double" ? randValuesReal(eng) : randValuesInt(eng));
					}
					if (dataType == "complexdouble") { 
						stringStream << " (" << randValuesReal(eng) << ", " << randValuesReal(eng) << ")";
					}				 
					stringStream << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds{randSleepTime(eng)}); // sleep
				}
				std::string fileName = dataHeroPath + "sim_" + dataType + "\\" + staff[k] + "_" + projects[l] + std::to_string(n) + ".txt";
				FileMsg::print(stringStream.str(), fileName);
			}
		}	
	}
}