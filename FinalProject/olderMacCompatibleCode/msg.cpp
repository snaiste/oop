#include "msg.hpp" // classes managing outputs

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: DEBUG MESSAGE CLASS
* -----------------------------------------------------------------------*/

// http://stackoverflow.com/questions/9282354/static-variable-link-error
// debug mode is off by default
bool DebugMsg::debugMode{false};

// check if debug mode is on/off
bool DebugMsg::getIfDebug() { return debugMode; }

// print debug message to screen
void DebugMsg::print(const std::string& message) {
    if (getIfDebug()) {
    	std::cout << message;
    }
}

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: ERROR MESSAGE CLASS
* -----------------------------------------------------------------------*/

void ErrorMsg::print(const std::string& message) {
    // print message
    std::cout << message;
}

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: SCREEN MESSAGE CLASS
* -----------------------------------------------------------------------*/

void ScreenMsg::print(const std::string& message) {
    // print message
	std::cout << message;
}

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: FILE MESSAGE CLASS
* -----------------------------------------------------------------------*/

void FileMsg::print(const std::string& message, const std::string& fileName) {
    try {
        // check if file can be opened
        std::ofstream outFile(fileName);
        outFile.exceptions(std::ifstream::failbit);
        // http://en.cppreference.com/w/cpp/io/ios_base/failure  
        outFile << message; // print message
        ScreenMsg::print("[FILE-MSG] Data file '" + fileName + "' created\n");
        outFile.close();
    } 
    catch (const std::ios_base::failure&) {
        ErrorMsg::print("[FILE-MSG] Exception opening/writing/closing file '" + fileName + "'\n");
    }
}