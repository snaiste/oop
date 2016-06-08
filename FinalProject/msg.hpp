#ifndef MSG_HPP
#define MSG_HPP

#include <iostream>  // std
#include <fstream>   // fstream, ifstream, ofstream
#include <exception> // exceptions
#include <string>    // string

/* ------------------------------------------------------------------------
* ABSTRACT BASE CLASS FOR OUTPUT MESSAGES
* -----------------------------------------------------------------------*/

class Msg {
public:
    // pure virtual function to print message
    virtual void print() = 0; 
};

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: DEBUG MESSAGE CLASS
* -----------------------------------------------------------------------*/

class DebugMsg : public Msg  {
public:
    // debug mode on/off
    static bool debugMode;
    // check if debug mode is on/off
    static bool getIfDebug();
    // print debug message to screen
    static void print(const std::string& userMessage);
};

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: ERROR MESSAGE CLASS
* -----------------------------------------------------------------------*/

class ErrorMsg : public Msg  {
public:
    // print error message to screen
    static void print(const std::string& userMessage);
};

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: SCREEN MESSAGE CLASS
* -----------------------------------------------------------------------*/

class ScreenMsg : public Msg { 
public: 
    // print message to screen
    static void print(const std::string& userMessage);
};

/* ------------------------------------------------------------------------
* DERIVED CLASSES FOR OUTPUT MESSAGES: FILE MESSAGE CLASS
* -----------------------------------------------------------------------*/

class FileMsg : public Msg {
public:
    // print message to file
    static void print(const std::string& message, const std::string& fileName);
};

#endif /* MSG_HPP */



