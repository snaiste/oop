#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

#include <iostream> // std
#include <limits>   // numeric_limits<streamsize>::max()
#include "msg.hpp"  // classes managing message outputs

/* ------------------------------------------------------------------------
* DEFINE A TEMPLATE FOR FUNCTION MANAGING SIMPLE INPUTS FROM SCREEN
* -----------------------------------------------------------------------*/

// checks if input is of expected type and if not, runs loop until gets 
// input of expected type
template <typename T> T getInput() {
	T a{}; // variable of any type
	bool notFinished{true};
	while (notFinished) {
		std::cin >> a; // read in variable
		if (std::cin.fail()) { // entered variable is not of expected type
			ErrorMsg::print("[INPUT] Input was not recognised, please try again >> ");
			std::cin.clear(); // clear bad input
			// ignore bad input
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		} else {
			notFinished = false;
		}
	}
	return a;
}

#endif /* USER_INPUT_HPP */