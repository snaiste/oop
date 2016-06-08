#ifndef DATA_INPUT_HPP
#define DATA_INPUT_HPP

#include <iostream>  // std
#include <vector>    // vector
#include <string>    // string
#include <sstream>   // stringstream
#include "dirent.h"  // read all files in directory 
#include "msg.hpp"   // classes managing outputs
#include "maps.hpp"  // classes managing databases

/* ------------------------------------------------------------------------
* DEFINE TEMPLATE FOR GETTING DATA FROM FILE AND SCREEN
* -----------------------------------------------------------------------*/

template <typename T> class DataInput {
private:

	// function which gets a list of files from directory 
	// (except the ones that begin with a dot)
	static bool getFileList(std::vector<std::string>& fileList, 
						    const std::string& dataPath) {
		// pointer to directory
	    DIR *d;
	    struct dirent *dir;
	    // open directory
	    d = opendir(dataPath.c_str());
		if (d) { // if directory can be opened
		    int i{};
		    // read directory
		    while ((dir = readdir(d)) != NULL) {
		    	// if regular directory and file name does not start with "."
				if (dir->d_type == DT_REG && dir->d_name[0] != '.') {
					// save file name
					fileList.push_back(dataPath + "\\" + dir->d_name);
			    	++i;
			    }
			}
			// close directory
		    closedir(d);
		}
		if (fileList.empty()) {
			std::ostringstream stringStream;
			stringStream << std::endl
				<< "[DATA-INPUT] No readable files found!" << std::endl
				<< "[DATA-INPUT] Looked in directory: "<< dataPath << std::endl;
			throw std::invalid_argument(stringStream.str());
			return false;
		}
		return true;
	}
	
public:
	// function that reads in data from file into data maps
	static bool readFromFile(DataManager<T>& data, 
						     const std::string& dataPath) {
		// declare vector for saving file names
		std::vector<std::string> fileList;
        try {
			// get file list
			getFileList(fileList, dataPath);
			// count number of files
			size_t noOfFiles{fileList.size()};
			// loop through all files
			for (size_t i{}; i < noOfFiles; ++i) {
				// declare experiment object
				Experiment<T> userExperiment;
				// pass file name to experiment read from file function
				userExperiment.readFromFile(fileList[i]);
				// insert data into maps
				data.insertExperiment(userExperiment);
			}  
        }
        catch (const std::invalid_argument& e) {
            ErrorMsg::print(e.what());
            return false;
        }
        return true;
	}	

	// function that reads in data from screen into data maps
	static bool readFromScreen(DataManager<T>& data) {
		bool finished{false};
		while (!finished) {
			// declare experiment object
			Experiment<T> experiment;
			try {
				// call read from screen function
				experiment.readFromScreen();
			}
	        catch (const std::invalid_argument& e) {
	            ErrorMsg::print(e.what());
	            return false;
	        }
			// insert data into maps
			data.insertExperiment(experiment);
			ScreenMsg::print("Type <y> to add another experiment ");
			ScreenMsg::print("or any other letter to finish >> ");
	        std::string reply{getInput<std::string>()};
	        // convert to uppercase letters in case user used upper case
    		std::transform(reply.begin(), reply.end(), reply.begin(), ::toupper);
    		// check reply
	        if (reply == "Y") {
	            continue;
	        } else {
	            finished = true;
	            break;           
	        }
		}
		return true;
	}	
};

#endif /* DATA_INPUT_HPP */