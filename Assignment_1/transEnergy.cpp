/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++
*   Date: 5 February 2016
*   Author: Aiste Norkute
*
* ----------------------------------------------------------------------*/

#include <iostream>  // cout, endl
#include <iomanip>   // precision
#include <string.h>  // string
#include <math.h>    // pow
#include <regex>

using namespace std;

// define error messages
const string INPUT_FORMAT_ERROR{ "Input is invalid! " };
const string INPUT_NONPHYSICAL_ERROR{ "Non-physical input! " };
const string NONPHYSICAL_HELP_TEXT{ "Input cannot be -ve, atomic no. cannot be > 180 or initial q.no. < final q.no." };
const string YN_REPLY_ERROR{ "Only <Y> or <N> are accepted! " };
const string TRY_AGAIN{ "Please try again: " };

// define constants
const double Ry{ 13.605692 }; // Rydberg constant in eV
const double eVtoJoules{ 1.602177 * pow(10, -19) }; // conversion from eV to Joules factor

/* ------------------------------------------------------------------------
* GET AND CHECK INTEGER INTPUT
* ----------------------------------------------------------------------*/

int getInputValues() {
	regex reg{ 0 };
	// define format of input: 
	// "[[:s:]]*" = any number of spaces
	// "[[:digit:]]*" = any number of digits
	reg = "([[:s:]])*([[:digit:]])*";
	string input{ "" }; // initialise empty string
	getline(cin, input);
	bool flag{ regex_match(input, reg) };
	while (!flag) { // check if value matches reg
		cout << INPUT_FORMAT_ERROR << TRY_AGAIN;
		getline(cin, input);
		flag = regex_match(input, reg);
	}
	return atoi(input.c_str()); // atoi() converts from string to integer
}

/* ------------------------------------------------------------------------
* CHECK IF INPUT VIOLATES PHYSICS
* ----------------------------------------------------------------------*/

bool checkPhysics(int Z, int N1, int N2) {
	bool ifCorrect{ true }; // assume correct input
	// check if not negative and if initial q.n. (N1) > final q.n (N2)
	// also, biggest possible Z is 180, so check for that too
	if ((Z <= 0) || (Z > 180) || (N1 <= 0) || (N2 <= 0) || (N1 < N2)) {
		ifCorrect = false;
		cout << INPUT_NONPHYSICAL_ERROR << endl; // input is non-physical
		cout << NONPHYSICAL_HELP_TEXT << endl; // explain why
		cout << TRY_AGAIN << endl;
	}
	return ifCorrect;
}

/* ------------------------------------------------------------------------
* COMPUTE ENERGY
* ----------------------------------------------------------------------*/

double computeEnergy(int Z, int N1, int N2) {
	double E{ 0 }; // initialise energy
	// compute photon energy in eV
	E = Ry * pow(Z, 2.) * (pow(N2, -2.) - pow(N1, -2.));
	return E;
}

/* ------------------------------------------------------------------------
* CONVERT STRING TO SRING THAT HAS ONLY UPPERCASE CHARS
* ----------------------------------------------------------------------*/

string strUpperCase(string s) {
	int i{ 0 };
	while (s[i]) // go through all string characters
	{
		s[i] = toupper(s[i]); // convert all string letters to uppercase
		i++;
	}
	return s;
}

/* ------------------------------------------------------------------------
* PRINT OUTPUT
* ----------------------------------------------------------------------*/

void printOutput(double E, string units) {
	// print output and format precision
	if (units == "J") {
		cout << "\nEnergy is: " << setprecision(2) << E * eVtoJoules << " Joules" << endl;
	}
	else if (units == "EV") {
		cout << "\nEnergy is: " << setprecision(2) << E << " eV" << endl;
	}
	else {
		// user pressed something else, not "eV" or "J"
		cout << "\nYou did not specify units correctly, hence: " << endl;
		cout << "Energy is: " << setprecision(2) << E << " eV or "
			<< setprecision(2) << E * eVtoJoules << " Joules" << endl;
	}
}

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* ----------------------------------------------------------------------*/

int main() {

	int Z{ 0 }, N1{ 0 }, N2{ 0 }; // atomic, initial and final quantum numbers

	string str{ "Y" }; // initialise string
	// "Y" sets while loop to proceed and repeat
	// proceed with loop until s == "N"
	while (str == "Y") {
		bool ifHaveValues{ false };
		// proceed with loop until get correct values both in terms of format and
		// such that physics is not violated
		while (!ifHaveValues) {
			// get and check input values
			cout << endl;
			cout << "Enter atomic number: ";
			Z = getInputValues();
			cout << "Enter initial quantum number (NB: ground state = 1): ";
			N1 = getInputValues();
			cout << "Enter final quantum number (NB: ground state = 1): ";
			N2 = getInputValues();
			// check if input does not violate physics
			if (!checkPhysics(Z, N1, N2)) {
				ifHaveValues = false;
				continue; // continue loop until get correct values
			}
			else {
				ifHaveValues = true;
				break; // stop loop if all is good, i.e. have good values
			}
		}

		string units{ "" }; // initialise empty string
		cout << "Type <J> or <eV> to get energy in Joules or Electronvolts: ";
		cin >> units;
		// convert string to string containing only uppercase letters
		units = strUpperCase(units);

		// compute energy in eV and print it in units that user requested
		printOutput(computeEnergy(Z, N1, N2), units);

		string reply{ "" }; // initialise emptry string
		cout << endl;
		while ((reply != "Y") || (reply != "N")) {
			cout << "Would you like to repeat? <Y/N>: ";
			cin >> reply;
			// get, check and convert string to string containing only uppercase letters
			reply = strUpperCase(reply);
			if ((reply != "Y") && (reply != "N")) {
				cout << YN_REPLY_ERROR << TRY_AGAIN << endl;
			}
			else {
				str = reply;
				cin.clear(); // clear bad input
				cin.ignore(); // clear bad input
				break;
			}
		}
	}

	cout << "\nBye!\n" << endl;
	return 0;
}