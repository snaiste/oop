
#include <iostream>   // std
#include <cstdlib>    // EXIT_FAILURE, EXIT_SUCCESS
#include <vector>     // vector
#include <memory>     // smart pointers
#include <iterator>   // iteration through vectors, etc.
#include <string>     // string
#include <sstream>    // stringstream

#include "version.hpp"   // contains version constants
#include "menus.hpp"	 // classes providing info
#include "maps.hpp"      // classes managing data maps
#include "dataInput.hpp"      // classes managing data maps

/* ------------------------------------------------------------------------
* POLYMORPHISM : INITIALISE GENERAL INFO AND HELP MENUS
* -----------------------------------------------------------------------*/

std::string getWelcomeScreen() {
  std::ostringstream stringStream;
  stringStream << std::endl
      << WELCOME << std::endl
      << VERSION << std::endl
      << LAST_UPDATE << std::endl   
      << AUTHOR  << std::endl << std::endl                                    
      << DESCRIPTION_1 << std::endl
      << DESCRIPTION_2 << std::endl
      << DESCRIPTION_3 << std::endl;
    return stringStream.str();
}

/* ------------------------------------------------------------------------
* ANALYSIS MENU MANAGER
* -----------------------------------------------------------------------*/

template <typename T> void analysisChoiceManager(const std::string& DataHeroPath, MainMenu& mainMenu, DataManager<T>& data) {
    // show menu
    mainMenu.dataManageCmdsShow();
    // declare some strings for later
    std::string choice, fileName, staffName, projectName, output;
    // loop until user decides to exit
    for (;;) {
        choice = mainMenu.getMenuInput();
        if (choice == "EXIT") break;
        else if (choice == "STAFF") {
            // print reference staff database
            ScreenMsg::print(data.staffDatabaseShow());
        } else if (choice == "PROJECT") {
            // print reference project database
            ScreenMsg::print(data.projectDatabaseShow());
        // print data
        } else if (choice == "F-DATA" || choice == "S-DATA") {
            ScreenMsg::print("\nExisting staff list:\n");
            // print reference staff database
            ScreenMsg::print(data.staffDatabaseShow());
            ScreenMsg::print("\nType staff name or <all> to extract all staff data >> "); 
            staffName = getInput<std::string>();
            // convert to upper case letters just in case
            std::transform(staffName.begin(), staffName.end(), staffName.begin(), ::toupper);
            ScreenMsg::print("\nType project name or <all> to extract all project data >> "); 
            projectName = getInput<std::string>(); 
            // convert to upper case letters just in case
            std::transform(projectName.begin(), projectName.end(), projectName.begin(), ::toupper);
            if (staffName == "ALL" && projectName == "ALL") {
                // print all data
                output = data.fullDatabaseShow();
            }
            // print particular data
            else if (staffName != "ALL" && projectName == "ALL") { 
                output = data.staffDatabaseShow(staffName);
            }
            else if (staffName == "ALL" && projectName != "ALL") { 
                output = data.projectDatabaseShow(projectName);
            }
            else if (staffName != "ALL" && projectName != "ALL") {
                output = data.fullDatabaseShow(staffName, projectName);
            }
			if (!output.empty()) {
				if (choice == "F-DATA") {
					// print data to file
					ScreenMsg::print("\n");
					ScreenMsg::print("N.B. Saving files to a particular directory requires already existing directory!");
					ScreenMsg::print("Enter file name (e.g. <data\\all.txt>) >> ");
					fileName = getInput<std::string>();
					FileMsg::print(output, DataHeroPath + fileName);
				}
				else if (choice == "S-DATA") {
					// print data to screen
					ScreenMsg::print(output);
				}
			}
        } else if (choice == "F-REPORT" || choice == "S-REPORT") {
            ScreenMsg::print("\nExisting staff list:\n");
            // print reference staff database
            ScreenMsg::print(data.staffDatabaseShow());
            ScreenMsg::print("\nType staff name or <all> to extract all staff data >> "); 
            staffName = getInput<std::string>();
            // convert to upper case letters just in case
            std::transform(staffName.begin(), staffName.end(), staffName.begin(), ::toupper);
            ScreenMsg::print("\nType project name or <all> to extract all project data >> "); 
            projectName = getInput<std::string>(); 
            // convert to upper case letters just in case
            std::transform(projectName.begin(), projectName.end(), projectName.begin(), ::toupper);
            // generate report for all data
            if (staffName == "ALL" && projectName == "ALL") {
                output = data.getReport();
            }
            // generate report for particular data
            else if (staffName != "ALL" && projectName == "ALL") { 
                output = data.getStaffReport(staffName);
            }
            else if (staffName == "ALL" && projectName != "ALL") { 
                output = data.getProjectReport(projectName);
            }
            else if (staffName != "ALL" && projectName != "ALL") {
                output = data.getReport(staffName, projectName);
            }
			if (!output.empty()) {
				if (choice == "F-REPORT") {
					// print report to file
					ScreenMsg::print("\n");
					ScreenMsg::print("N.B. Saving files to a particular directory requires already existing directory!");
					ScreenMsg::print("Enter file name (e.g. <reports\\all.txt>) >> ");
					fileName = getInput<std::string>();
					FileMsg::print(output, DataHeroPath + fileName);
				}
				else if (choice == "S-REPORT") {
					// print report to screen
					ScreenMsg::print(output);
				}
			}
        } else if (choice == "DEL") {
            ScreenMsg::print("\nExisting staff list:\n");
            // print reference staff database
            ScreenMsg::print(data.staffDatabaseShow());
            ScreenMsg::print("\n");
            ScreenMsg::print("Type name of the staff member to be deleted from the list >> ");
            staffName = getInput<std::string>();
            // convert to upper case letters just in case
            std::transform(staffName.begin(), staffName.end(), staffName.begin(), ::toupper);
            ScreenMsg::print("Type name of his/her project >> ");        
            projectName = getInput<std::string>();  
            // convert to upper case letters just in case
            std::transform(projectName.begin(), projectName.end(), projectName.begin(), ::toupper);
            bool success = data.deleteEntry(staffName, projectName);  
            if (success) {
                ScreenMsg::print("Updated project: ");
                output = data.fullDatabaseShow(staffName, projectName);
                ScreenMsg::print(output);
            }
        } else if (choice == "DEL-VAL") {
            ScreenMsg::print("\nExisting staff list:\n");
            // print reference staff database
            ScreenMsg::print(data.staffDatabaseShow());
            // declare variables to specify timestamp ranges 
            // (unsigned because can only be positive!)
            unsigned startRange, endRange;
            ScreenMsg::print("\n");
            ScreenMsg::print("Type name of the staff member >> ");
            staffName = getInput<std::string>();
            // convert to upper case letters just in case
            std::transform(staffName.begin(), staffName.end(), staffName.begin(), ::toupper);
            ScreenMsg::print("Type name of his/her project >> ");        
            projectName = getInput<std::string>();  
            // convert to upper case letters just in case
            std::transform(projectName.begin(), projectName.end(), projectName.begin(), ::toupper);
            // show data for this name and project so that user can pick timestamps
            output = data.fullDatabaseShow(staffName, projectName);
            ScreenMsg::print(output);
            if (!output.empty()) {
                // requested project does not exist
                // error thrown directly from database, so no need to handle here
                ScreenMsg::print("\n");
                ScreenMsg::print("Now you will have to provide timestamp range of values to be deleted\n"); 
                ScreenMsg::print("Type start time from which values should be deleted >> ");        
                startRange = getInput<unsigned>();
                ScreenMsg::print("Type end time to which values should be deleted >> ");        
                endRange = getInput<unsigned>();
                bool success = data.deleteMeasurementRange(staffName, projectName, startRange, endRange);
                if (success) {
                    // if not success, error gets printed directly from experiment class
                    ScreenMsg::print("\nUpdated project:\n");                  
                    output = data.fullDatabaseShow(staffName, projectName);
                    ScreenMsg::print(output);                    
                }
            }
        }
        mainMenu.dataManageCmdsShow();
    }
}

/* ------------------------------------------------------------------------
* GET DATA TYPE
* -----------------------------------------------------------------------*/

std::string getDataType() {
    std::string dataType;
    while (dataType != "DOUBLE" && dataType != "INT" && dataType != "COMPLEXDOUBLE") {
        // force user to select correct data type
        ScreenMsg::print("\nChoose data type: <double>, <int>, <complexdouble> >> ");
        dataType = getInput<std::string>();
        // convert to upper case letters just in case
        std::transform(dataType.begin(), dataType.end(), dataType.begin(), ::toupper);
        if (dataType != "DOUBLE" && dataType != "INT" && dataType != "COMPLEXDOUBLE") {
            // ask to try again if cannot recognise input
            ErrorMsg::print("\n[ERROR] Input not recognised, please try again\n"); 
        }
    }
    return dataType;
}

/* ------------------------------------------------------------------------
* GET DATA PATH
* -----------------------------------------------------------------------*/

// if input from simulation or file, get file path
std::string getDataPath(const std::string& DataHeroPath, const std::string& userOrSimFile, std::string dataType) {
	// convert reply to lower case letters
	std::transform(dataType.begin(), dataType.end(), dataType.begin(), ::tolower);
    if (userOrSimFile == "F") {
		ScreenMsg::print("\nEnter file(s) directory (e.g. <sim_" + dataType + ">:\n");
		ScreenMsg::print(">> ");
        return DataHeroPath + getInput<std::string>();
    } else if (userOrSimFile == "SIM") {
        ScreenMsg::print("Data for Staff1 and Staff2 projects A and B will be simulated.\n");
        ScreenMsg::print("Enter how many experiment files per staff to simulate (e.g. 3) >> ");
        unsigned noOfFiles{ getInput<unsigned>() };
		ScreenMsg::print("Confirm there is 'sim_" + dataType + "' folder within this code directory <y/n>: \n");
		ScreenMsg::print(">> ");        
        std::string reply{getInput<std::string>()};
        // convert reply to upper case letters just in case
        std::transform(reply.begin(), reply.end(), reply.begin(), ::toupper);
        if (reply == "Y") {
            // simulate data
			Simulation::simulateData(DataHeroPath, noOfFiles, dataType);
            // return data path
			return DataHeroPath + "sim_" + dataType;
        } else if (reply == "N") {
            // ask user to create a folder before doing simulation
            ScreenMsg::print("Please create folder 'sim_" + dataType + "' within this code directory.\n");            
        }
        return "";
    } else {
        return "";
    }
}

/* ------------------------------------------------------------------------
* DATA INPUT MENU MANAGER
* -----------------------------------------------------------------------*/

void dataInputChoiceManager(const std::string& DataHeroPath, MainMenu& mainMenu) {
    // get user choice of data type
    std::string dataType = getDataType();
    // variable storing data path if input from file(s)
    std::string dataPath;
    bool dataExists{false};
    mainMenu.infoCmdsShow("INPUT");
    // get user's menu option choice  
    std::string choice{mainMenu.getMenuInput()}; 
    // if choice is to simulate data or read from file
    if (choice == "SIM" || choice == "F") {
        dataPath = getDataPath(DataHeroPath, choice, dataType);
        if (dataType == "DOUBLE") {
            DataManager<double> data;
            dataExists = DataInput<double>::readFromFile(data, dataPath);
            if (dataExists) {
                // data exists, call analysis menu
                analysisChoiceManager<double>(DataHeroPath, mainMenu, data);                
            }
        } else if (dataType == "INT") {
            DataManager<int> data;
            dataExists = DataInput<int>::readFromFile(data, dataPath); 
            if (dataExists) {
                // data exists, call analysis menu
                analysisChoiceManager<int>(DataHeroPath, mainMenu, data); 
            }        
        } else if (dataType == "COMPLEXDOUBLE") {
            DataManager<std::complex<double>> data;
            dataExists = DataInput<std::complex<double>>::readFromFile(data, dataPath); 
            if (dataExists) {
                // data exists, call analysis menu
                analysisChoiceManager<std::complex<double>>(DataHeroPath, mainMenu, data); 
            }
        }   
    // if choice is to read from screen 
    } else if (choice == "S") {
        if (dataType == "DOUBLE") {
            DataManager<double> data;
            dataExists = DataInput<double>::readFromScreen(data);
            if (dataExists) {
                // data exists, call analysis menu
				analysisChoiceManager<double>(DataHeroPath, mainMenu, data);
            } 
        } else if (dataType == "INT") {
            DataManager<int> data;
            dataExists = DataInput<int>::readFromScreen(data); 
            if (dataExists) {
                // data exists, call analysis menu
				analysisChoiceManager<int>(DataHeroPath, mainMenu, data);
            }   
        } else if (dataType == "COMPLEXDOUBLE") {
            DataManager<std::complex<double>> data;
            dataExists = DataInput<std::complex<double>>::readFromScreen(data); 
            if (dataExists) {
                // data exists, call analysis menu
                analysisChoiceManager<std::complex<double>>(DataHeroPath, mainMenu, data); 
            } 
        }     
    } 
    else ErrorMsg::print("\n[ERRPR] Input not recognised, try again\n");
}


/* ------------------------------------------------------------------------
* MAIN PROGRAM
* -----------------------------------------------------------------------*/

int main() {

	/* ------------------------------------------------------------------------
	* POLYMORPHISM : INITIALISE MAIN MENU
	* -----------------------------------------------------------------------*/

	// create some commands
	CommandUniquePtr initGeneralCmds[] = {
		// firt main menu part: general
		CommandUniquePtr{ new StartInfo },
		CommandUniquePtr{ new HelpInfo },
		CommandUniquePtr{ new DebugInfo },
		CommandUniquePtr{ new RestartInfo },
		CommandUniquePtr{ new QuitInfo },
		// data main input menu: data input
		CommandUniquePtr{ new SimulateInputInfo },
		CommandUniquePtr{ new FileInputInfo },
		CommandUniquePtr{ new ScreenInputInfo },
		// third main manu part: analysis mode
		CommandUniquePtr{ new ShowStaffInfo },
		CommandUniquePtr{ new ShowProjectInfo },
		CommandUniquePtr{ new PrintDataInfo },
		CommandUniquePtr{ new ShowDataInfo },
		CommandUniquePtr{ new FileReportInfo },
		CommandUniquePtr{ new ScreenReportInfo },
		CommandUniquePtr{ new DelInfo },
		CommandUniquePtr{ new DelValInfo },
		CommandUniquePtr{ new ExitAnalysisInfo }
	};

	// http://en.cppreference.com/w/cpp/iterator/make_move_iterator
	std::vector<CommandUniquePtr> generalCmds{
		std::make_move_iterator(std::begin(initGeneralCmds)),
		std::make_move_iterator(std::end(initGeneralCmds))
	};
	// initialise menu
	MainMenu mainMenu{ generalCmds };

	/* ------------------------------------------------------------------------
	* POLYMORPHISM : INITIALISE HELP MENU
	* -----------------------------------------------------------------------*/

	// create some help menu commands
	CommandUniquePtr initHelpCmds[] = {
		CommandUniquePtr{ new Help },
		CommandUniquePtr{ new FileInputHelp },
		CommandUniquePtr{ new ScreenInputHelp },
		CommandUniquePtr{ new FunctionalityHelp }
	};
	// http://en.cppreference.com/w/cpp/iterator/make_move_iterator
	std::vector<CommandUniquePtr> helpCmds{
		std::make_move_iterator(std::begin(initHelpCmds)),
		std::make_move_iterator(std::end(initHelpCmds))
	};
	// initialise menu
	HelpMenu helpMenu{ helpCmds };

	/* ------------------------------------------------------------------------
	* MAIN PROGRAM LOOP
	* -----------------------------------------------------------------------*/

	// print welcome and DataHero version info
	ScreenMsg::print(getWelcomeScreen());

	// get working directory
	ScreenMsg::print("\nEnter full DataHero working directory, e.g. <P:\\Desktop\\project\\>:\n");
	ScreenMsg::print("N.B. Do not forget the backslash at the end!\n");
	ScreenMsg::print(">> ");
	std::string DataHeroPath{ getInput<std::string>() };
	ScreenMsg::print("\nDataHero directory set to " + DataHeroPath + "\n");
	ScreenMsg::print("N.B. DataHero can export files only to existing folders within this directory\n");

	bool quit{false};
    while (!quit) {
        // print short version info and show starting general info commands menu
        mainMenu.infoCmdsShow("START");
        // get user's menu option choice  
        std::string choice{mainMenu.getMenuInput()}; 
        // if user chose help               
        if (choice == "HELP") {
            // show help menu
            helpMenu.show();
            continue;
        }
        // check if user wants to see debug messages throughout analysis
        else if (choice == "DEBUG") {
            // enable/disable debug mode
            DebugMsg::debugMode = !DebugMsg::debugMode;
            if (DebugMsg::debugMode) {
                ScreenMsg::print("\n[MAIN] Debug mode is ON\n");                
            } else if (!DebugMsg::debugMode) {
                ScreenMsg::print("\n[MAIN] Debug mode is OFF\n");                
            }
            continue;
        } 
        // restart
        else if (choice == "RESTART") continue; 
        // quit
        else if (choice == "QUIT") {
            ScreenMsg::print("\n\nBye!\n\n");
            quit = true;
            break;
        }
        // call data input menu manager
        dataInputChoiceManager(DataHeroPath, mainMenu); 
    }
    return EXIT_SUCCESS;
}