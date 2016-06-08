#include "project.hpp"   // classes managing outputs

/* ------------------------------------------------------------------------
* DEFINE PROJECT HEADER LINE CLASS
* -----------------------------------------------------------------------*/

// default constructor
HeaderLine::HeaderLine() : name{""} {
    DebugMsg::print("[HEADER-LINE] Default constructor called\n");
};

// parametrised constructor
HeaderLine::HeaderLine(const std::string& userName) : name{userName} {
    DebugMsg::print("[HEADER-LINE] Parametrised constructor called\n");
}

// copy constructor for deep copying
HeaderLine::HeaderLine(const HeaderLine& userHeaderLine) {
    DebugMsg::print("[HEADER-LINE] Copy constructor called\n");
    this->name = userHeaderLine.name;
}

// move constructor
HeaderLine::HeaderLine(HeaderLine&& userHeaderLine) {
    DebugMsg::print("[HEADER-LINE] Move constructor called\n");
    // steal the data
    this->name = userHeaderLine.name;
    // delete user object's data
    userHeaderLine.name = "";
}

// default destructor
HeaderLine::~HeaderLine() {
    DebugMsg::print("[HEADER-LINE] Default destructor called\n");
}

// access function
std::string HeaderLine::getName() const { 
    // convert to capital letters
    std::string tempName = this->name;
    std::transform(tempName.begin(), tempName.end(), tempName.begin(), ::toupper);
    return tempName; 
}

// copy assignment operator
HeaderLine& HeaderLine::operator=(const HeaderLine& userHeaderLine) {
    DebugMsg::print("[HEADER-LINE] Copy assignment operator called\n");
    if (&userHeaderLine == this) { return *this; } // no self-assignment
    // first delete this object’s data
    this->name = "";
    // declare new object
    this->name = userHeaderLine.name;
    return *this;
}

// move assignment operator
HeaderLine& HeaderLine::operator=(HeaderLine &&userHeaderLine) {
    DebugMsg::print("[HEADER-LINE] Move assignment operator called\n");
    std::swap(this->name, userHeaderLine.name);
    return *this;
}

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF HEADER LINE CLASS TO OVERLOAD OSTREAM OPERATOR>>
* -----------------------------------------------------------------------*/

std::istream& operator>>(std::istream& is, HeaderLine& userHeaderLine) {
        is >> userHeaderLine.name;
    return is;
}