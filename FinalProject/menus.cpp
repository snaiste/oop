#include "menus.hpp"  // classes providing info

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR MAIN MENU COMMANDS
* -----------------------------------------------------------------------*/

std::string StartInfo::description() { 
    // return 'start' command desciption
    return "Type any letter to start analysis";
}

std::string HelpInfo::description() { 
    // return 'help' command desciption
    return "<help>    - access more info about DataHero usage";
}

std::string DebugInfo::description() {
    // return 'quit' command desciption
    return "<debug>   - turn on/off debug mode";
}

std::string RestartInfo::description() {
    // return 'restart' command desciption
    return "<restart> - restart DataHero";
}

std::string QuitInfo::description() {
    // return 'quit' command desciption
    return "<quit>    - quit DataHero";
}

std::string SimulateInputInfo::description() { 
    // return 'simulate' command desciption
    return "<sim> - simulate input";;
}

std::string FileInputInfo::description() { 
    // return file input command desciption
    return "<f>   - input from file(s)";
}

std::string ScreenInputInfo::description() { 
    // return screen input command desciption
    return "<s>   - input from screen";;
}

std::string ShowStaffInfo::description() { 
    // returns 'show staff' command desciption
    return "<staff>    - show available staff name list";
}

std::string ShowProjectInfo::description() { 
    // returns 'show project' command desciption
    return "<project>  - show available project name list";
}

std::string PrintDataInfo::description() { 
    // returns 'show data' command desciption
    return "<f-data>   - print all or particular data to file";
}

std::string ShowDataInfo::description() { 
    // returns 'print data' command desciption
    return "<s-data>   - show all or particular data";
}

std::string FileReportInfo::description() { 
    // returns 'report to file' command desciption
    return "<f-report> - generate analysis report to file";
}

std::string ScreenReportInfo::description() { 
    // returns 'report to screen' command desciption
    return "<s-report> - generate analysis report to screen";
}

std::string DelInfo::description() { 
    // returns 'delete project' command desciption
    return "<del>      - delete project";
}

std::string DelValInfo::description() { 
    // returns 'delete particular values' command desciption
    return "<del-val>  - delete particular measurement";
}

std::string ExitAnalysisInfo::description() { 
    // returns 'exit analysis mode' command desciption
    return "<exit>     - exit analysis mode";
}

/* ------------------------------------------------------------------------
* CLASS FOR MANAGING MAIN MENU COMMANDS
* -----------------------------------------------------------------------*/

// parametrised constructor
MainMenu::MainMenu(std::vector<CommandUniquePtr>& userCommands) 
                  : commands(std::move(userCommands)) {
    // unique_ptrs are not copyable, hence use move()
    // http://www.cplusplus.com/forum/general/157354
    DebugMsg::print("[MENUS] MainMenu parametrised constructor called");
}

// return general info at the start of the program
std::string MainMenu::infoMenuCmds() const {
    std::ostringstream stringStream;
    stringStream << std::endl
        << "------------------------------------------------------------" << std::endl
        << "GENERAL DATAHERO COMMANDS: press ENTER after each command!  " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "   " << commands[0]->description()                            << std::endl 
        << "   " << commands[1]->description()                            << std::endl 
        << "   " << commands[2]->description()                            << std::endl
        << "   " << commands[3]->description()                            << std::endl 
        << "   " << commands[4]->description()                            << std::endl 
        << "------------------------------------------------------------" << std::endl;
    return stringStream.str(); 
}

// return data input info 
std::string MainMenu::dataInputCmds() const {
    std::ostringstream stringStream;
    stringStream << std::endl
        << "------------------------------------------------------------" << std::endl
        << "DATA INPUT COMMANDS: press ENTER after each command!        " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "   " << commands[5]->description()                            << std::endl 
        << "   " << commands[6]->description()                            << std::endl 
        << "   " << commands[7]->description()                            << std::endl
        << "------------------------------------------------------------" << std::endl;
    return stringStream.str(); 
}

// return data management info
std::string MainMenu::dataManageCmds() const {
    std::ostringstream stringStream;
    stringStream << std::endl
        << "------------------------------------------------------------" << std::endl
        << "DATA ANALYSIS MODE: press ENTER after each command!         " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "   " << commands[8] ->description()                           << std::endl 
        << "   " << commands[9] ->description()                           << std::endl 
        << "   " << commands[10]->description()                           << std::endl
        << "   " << commands[11]->description()                           << std::endl
        << "   " << commands[12]->description()                           << std::endl
        << "   " << commands[13]->description()                           << std::endl
        << "   " << commands[14]->description()                           << std::endl
        << "   " << commands[15]->description()                           << std::endl
        << "   " << commands[16]->description()                           << std::endl
        << "------------------------------------------------------------" << std::endl; 
    return stringStream.str(); 
}

// show either general info menu or data input menu
void MainMenu::infoCmdsShow(const std::string& whichMenu) const {
    if (whichMenu == "START") {
        ScreenMsg::print(this->infoMenuCmds());
    } else if (whichMenu == "INPUT") {
        ScreenMsg::print(dataInputCmds());        
    }
}

// show data management menu
void MainMenu::dataManageCmdsShow() const {
    ScreenMsg::print(this->dataManageCmds());
}

// return user's menu input
std::string MainMenu::getMenuInput() {
    ScreenMsg::print(">> ");  
    // get user's menu option choice
    std::string a = getInput<std::string>();
    // convert to uppercase in case user used upper case
    std::transform(a.begin(), a.end(), a.begin(), ::toupper);
    return a;
}

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR HELP MENU COMMANDS
* -----------------------------------------------------------------------*/

// return help command output
std::string Help::description() {
    std::ostringstream stringStream;
    stringStream << std::endl
        << "------------------------------------------------------------" << std::endl
        << "HELP INFO CHOICES: press ENTER after each command!          " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "   <help>     - call this menu                              " << std::endl
        << "   <exit>     - exit help                                   " << std::endl
        << "   <all>      - show all help instructions                  " << std::endl
        << "   <f-input>  - show supported data input file format       " << std::endl
        << "                and provide other relavant info             " << std::endl
        << "   <s-input>  - show supported data screen input format     " << std::endl
        << "                and provide other relavant info             " << std::endl
        << "   <func>     - show DataHero functionality info            " << std::endl
        << "------------------------------------------------------------" << std::endl;
    return stringStream.str();
}

// return file input help command output
std::string FileInputHelp::description() {
    std::ostringstream stringStream;
    stringStream << std::endl
        << "------------------------------------------------------------" << std::endl
        << "DATA INPUT FROM FILE                                        " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "                                                            " << std::endl
        << "   - DataHero can read one or more files                    " << std::endl
        << "                                                            " << std::endl
        << "   - File(s) must be stored in a folder separate from       " << std::endl
        << "     the DataHero code and should only contain data files   " << std::endl
        << "                                                            " << std::endl
		<< "   - Data folders must be in DataHero working directory     " << std::endl
		<< "                                                            " << std::endl
        << "   - One project can have multiple files and DataHero will  " << std::endl
        << "     merge data files which have the same project name      " << std::endl
        << "     specified in the data file header (format shown below) " << std::endl
        << "                                                            " << std::endl
        << "   - Experimenter (staff member) name should be provided    " << std::endl
        << "     as single word (e.g. PaulSmith)                        " << std::endl
        << "                                                            " << std::endl
        << "   - Project name should be provided as single word         " << std::endl
        << "     (e.g. RandomData)                                      " << std::endl
        << "                                                            " << std::endl
        << "   - Each measured value should have a timestamp            " << std::endl
        << "     associated with it. You can use Unix timestamp         " << std::endl
        << "     e.g. 1462557490. For more information, please visit    " << std::endl
        << "     http://www.unixtimestamp.com. However, you can also    " << std::endl
        << "     use timestamp, e.g. 2016-05-01 17:10, expressed as,    " << std::endl
        << "     for egxample, 1605011710.                              " << std::endl
        << "                                                            " << std::endl
        << "   - Supported types of data:                               " << std::endl
        << "     -- double, e.g. 1.5                                    " << std::endl
        << "     -- integer, e.g. 5                                     " << std::endl
        << "     -- complex double, e.g. (1.5, 4.7)                     " << std::endl
        << "                                                            " << std::endl
        << "   - Real data (e.g. integer) file input format:            " << std::endl
        << "                                                            " << std::endl
        << "     Experimenter: YourName (as single word)                " << std::endl
        << "     Experiment: ExperimentName (as single word)            " << std::endl 
        << "     ------------------------------------------             " << std::endl
        << "     timestamp 1                                            " << std::endl
        << "     timestamp 5                                            " << std::endl
        << "     timestamp ..                                           " << std::endl
        << "     timestamp 2                                            " << std::endl
        << "                                                            " << std::endl
        << "   - Complex data (e.g. double) file input format:          " << std::endl
        << "                                                            " << std::endl
        << "     Experimenter: your name                                " << std::endl
        << "     Experiment: experiment name/code (e.g. A)              " << std::endl
        << "     -------------------------------------------            " << std::endl
        << "     timestamp (1.5, 3.77)                                  " << std::endl
        << "     timestamp (5.3, 6.2)                                   " << std::endl
        << "     timestamp ..                                           " << std::endl
        << "     timestamp (7.44, 10.1)                                 " << std::endl
        << "                                                            " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "Type <help> to see help options or exit help with <exit>    " << std::endl
        << "------------------------------------------------------------" << std::endl;
    return stringStream.str();
}

// return screen input help command output
std::string ScreenInputHelp::description() {
    std::ostringstream stringStream;
    stringStream << std::endl
        << "------------------------------------------------------------" << std::endl
        << "DATA INPUT FROM SCREEN                                      " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "                                                            " << std::endl
        << "   - DataHero can read data input from keyboard             " << std::endl
        << "                                                            " << std::endl
        << "   - Before inputing data, DataHero asks you your name      " << std::endl
        << "     as well as project name (e.g. A)                       " << std::endl
        << "                                                            " << std::endl
        << "   - Experimenter (staff member) name should be provided    " << std::endl
        << "     as single word (e.g. PaulSmith)                        " << std::endl
        << "                                                            " << std::endl
        << "   - Project name should be provided as single word         " << std::endl
        << "     (e.g. RandomData)                                      " << std::endl
        << "                                                            " << std::endl
        << "   - Each measured value should have a timestamp            " << std::endl
        << "     associated with it. You can use Unix timestamp         " << std::endl
        << "     e.g. 1462557490. For more information, please visit    " << std::endl
        << "     http://www.unixtimestamp.com. However, you can also    " << std::endl
        << "     use timestamp, e.g. 2016-05-01 17:10, expressed as,    " << std::endl
        << "     for egxample, 1605011710.                              " << std::endl
        << "                                                            " << std::endl
        << "   - Supported types of data:                               " << std::endl
        << "     -- double, e.g. 1.5                                    " << std::endl
        << "     -- integer, e.g. 5                                     " << std::endl
        << "     -- complex double, e.g. (1.5, 4.7)                     " << std::endl
        << "                                                            " << std::endl
        << "   - Real data (e.g. integer) screen input format:          " << std::endl
        << "                                                            " << std::endl
        << "     timestamp 1                                            " << std::endl
        << "     timestamp 5                                            " << std::endl
        << "     timestamp ..                                           " << std::endl
        << "     timestamp 2                                            " << std::endl
        << "                                                            " << std::endl
        << "   - Complex data (e.g. integer) screen input format:       " << std::endl
        << "                                                            " << std::endl
        << "     timestamp (1.5, 3.77)                                  " << std::endl
        << "     timestamp (5.3, 6.2)                                   " << std::endl
        << "     timestamp ..                                           " << std::endl
        << "     timestamp (7.44, 10.1)                                 " << std::endl
        << "                                                            " << std::endl    
        << "------------------------------------------------------------" << std::endl
        << "Type <help> to see help options or exit help with <exit>    " << std::endl
        << "------------------------------------------------------------" << std::endl;
    return stringStream.str();
}

// return funtionality help command output
std::string FunctionalityHelp::description() {
    std::ostringstream stringStream;
    stringStream << std::endl
        << "------------------------------------------------------------" << std::endl
        << "FUNCTIONALITY                                               " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "                                                            " << std::endl
        << "   - DataHero reads in data from file(s) or screen          " << std::endl
        << "                                                            " << std::endl
        << "   - Data can be double, integer or complex double          " << std::endl
        << "                                                            " << std::endl
        << "   - You can input data for multiple experiments            " << std::endl
        << "                                                            " << std::endl
        << "   - If project contains multiple files, DataHero will      " << std::endl
        << "     merge them according to timestamp                      " << std::endl
        << "                                                            " << std::endl
        << "   - DataHero ignores bad input, but warns you about it     " << std::endl
        << "                                                            " << std::endl
        << "   - DataHero allows you to print all or selected data      " << std::endl
        << "     to screen or file                                      " << std::endl
        << "                                                            " << std::endl
        << "   - There are options to show staff and project name       " << std::endl
        << "     databases                                              " << std::endl
        << "                                                            " << std::endl
        << "   - DataHero should be used to analyse repeated            " << std::endl
        << "     measurements: it computes the mean and standard        " << std::endl
        << "     error in the mean                                      " << std::endl
        << "                                                            " << std::endl
        << "   - Calculation reports are generated to screen or file    " << std::endl
        << "                                                            " << std::endl
        << "   - You can choose which data to use for report: one data  " << std::endl
        << "     set or merge multiple sets and print report for all    " << std::endl
        << "                                                            " << std::endl
        << "   - You can exclude particular data sets from analysis     " << std::endl
        << "     by deleting particular values or whole projects        " << std::endl
        << "                                                            " << std::endl
        << "------------------------------------------------------------" << std::endl
        << "Type <help> to see help options or exit help with <exit>    " << std::endl
        << "------------------------------------------------------------" << std::endl;
    return stringStream.str();
}

/* ------------------------------------------------------------------------
* CLASS FOR MANAGING HELP MENU COMMANDS
* -----------------------------------------------------------------------*/

// parametrised constructor
HelpMenu::HelpMenu(std::vector<CommandUniquePtr>& userCommands) 
                  : commands(std::move(userCommands)) {
    // unique_ptrs are not copyable, hence use move()
    // http://www.cplusplus.com/forum/general/157354
    DebugMsg::print("[MENUS] HelpMenu parametrised constructor called");
}

// help menu manager and printer
void HelpMenu::show() {
    // print initial info
    ScreenMsg::print(commands[0]->description()); 
    std::string input{};
    for (;;) {
        input = getMenuInput();
        if (input == "EXIT") return;
        else if (input == "ALL") {
            // start with 1, i.e. do not print initial info again
            for (size_t i{1}; i < commands.size(); ++i)
                ScreenMsg::print(commands[i]->description());
        } else if (input == "HELP") {
            ScreenMsg::print(commands[0]->description());
        } else if (input == "F-INPUT") {
            ScreenMsg::print(commands[1]->description());
        } else if (input == "S-INPUT") {
            ScreenMsg::print(commands[2]->description());
        } else if (input == "FUNC") {
            ScreenMsg::print(commands[3]->description());
        } else {
            ErrorMsg::print("\n[MENUS] ERROR: Input was not recognised. Please try again ");
        }
    }
}

// get and return user's menu option choice
std::string HelpMenu::getMenuInput() {
    ScreenMsg::print(">> ");  
    // get and return user's menu option choice
    std::string a = getInput<std::string>();
    // convert to uppercase in case user used upper case
    std::transform(a.begin(), a.end(), a.begin(), ::toupper);
    return a;
}