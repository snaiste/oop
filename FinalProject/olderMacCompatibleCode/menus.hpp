#ifndef MENUS_HPP
#define MENUS_HPP

#include <iostream>  // std
#include <vector>    // vector
#include <string>    // string
#include <sstream>   // stringstream
#include <memory>    // unique pointer
#include <algorithm> // transform

#include "userInput.hpp"  // classes managing user input
#include "msg.hpp"        // classes managing output messages
#include "simulation.hpp" // simulation class

/* ------------------------------------------------------------------------
* ABSTRACT BASE CLASS FOR GENERAL MENU AND HELPER COMMANDS
* -----------------------------------------------------------------------*/

class Command {
public:
	// description() returns help info associated with a command
	virtual std::string description() = 0; 
	virtual ~Command() = default;
    // http://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
    // "Virtual destructors are useful when you can delete an instance of a 
    // derived class through a pointer to base class. To sum up, always make base 
    // classes' destructors virtual when they are meant to be manipulated 
    // polymorphically."
};
using CommandUniquePtr = std::unique_ptr<Command>; 
// http://en.cppreference.com/w/cpp/memory/unique_ptr
// "std::unique_ptr is a smart pointer that retains sole ownership of an object 
// through a pointer and destroys that object when the unique_ptr goes out of 
// scope. No two unique_ptr instances can manage the same object."

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR GENERAL INFO COMMAND
* -----------------------------------------------------------------------*/

class StartInfo : public Command {
public:
    // tell how to start analysis
    std::string description();
};

class HelpInfo : public Command {
public:
    // tell how to access help
    std::string description();
};

class DebugInfo : public Command {
public:
    // tell how to enter debug mode
    std::string description();
};

class RestartInfo : public Command {
public:
    // tell how to restart DataHero
    std::string description();
};

class QuitInfo : public Command {
public:
    // tell how to quit DataHero
    std::string description();
};

class SimulateInputInfo : public Command {
public:
    // tell how to simulate data
    std::string description();
};

class FileInputInfo : public Command {
public:
    // tell how to input data from file
    std::string description();
};

class ScreenInputInfo : public Command {
public:
    // tell how to input data from screen
    std::string description();
};

class ShowStaffInfo : public Command {
public:
    // tell how to extract staff list
    std::string description();
};

class ShowProjectInfo : public Command {
public:
    // tell how to extract project list
    std::string description();
};

class PrintDataInfo : public Command {
public:
    // tell how to extract data to file
    std::string description();
};

class ShowDataInfo : public Command {
public:
    // tell how to extract data to screen
    std::string description();
};

class FileReportInfo : public Command {
public:
    // tell how to print report to file
    std::string description();
};

class ScreenReportInfo : public Command {
public:
    // tell how to print report to screen
    std::string description();
};

class DelInfo : public Command {
public:
    // tell how to delete project
    std::string description();
};

class DelValInfo : public Command {
public:
    // tell how to delete particular values
    std::string description();
};

class ExitAnalysisInfo : public Command {
public:
    // tell how to exit analysis mode
    std::string description();
};

/* ------------------------------------------------------------------------
* CLASS FOR MANAGING MAIN MENU COMMANDS
* -----------------------------------------------------------------------*/

class MainMenu {
private:
    // commands are saved in a vector of unique pointers to base class
    std::vector<CommandUniquePtr> commands;
public:
    // parametrised constructor
    MainMenu(std::vector<CommandUniquePtr>& userCommands);
    // return general info command menu
    std::string infoMenuCmds() const; 
    // return data input command menu
    std::string dataInputCmds() const;
    // return data management command menu
    std::string dataManageCmds() const;
    // show either general info menu or data input menu
    void infoCmdsShow(const std::string& whichMenu) const;
    // show data management menu
    void dataManageCmdsShow() const;
    // get user's menu input
    std::string getMenuInput();
};

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR HELP MENU COMMANDS
* -----------------------------------------------------------------------*/

class Help : public Command {
public:
    // tell how to call help
    std::string description();
};

// special cases of Help class
class DataInputHelp : public Help { 
public:
    // tell about data input
    std::string description();
};

class FileInputHelp : public Help { 
public:
    // tell about input from files
    std::string description();
};

class ScreenInputHelp : public Help {
public:
    // tell about input from screen
    std::string description();
};

class FunctionalityHelp : public Help { 
public:
    // tell about DataHero functionality
    std::string description();
};

/* ------------------------------------------------------------------------
* CLASS FOR MANAGING HELP MENU COMMANDS
* -----------------------------------------------------------------------*/

class HelpMenu {
private:
    // commands are saved in a vector of unique pointers to base class
    std::vector<CommandUniquePtr> commands;
public:
    // parametrised constructor
    HelpMenu(std::vector<CommandUniquePtr>& userCommands);
    // show help menu
    void show(); 
    // get user's input
    std::string getMenuInput();
};

#endif /* MENUS_HPP */