/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++ (PHYS30762)
*   School of Physics and Astronomy, The University of Manchester
*   Date: 20 February 2016
*   Author: Aiste Norkute
*
*   courses.cpp
*
*   DESCRIPTION: This program stores and prints physics courses. It allows user
*   to enter an arbitrary number of courses or, alternatively, reads courses from
*   specified file. A course code (integer, e.g. 30762) and title (string, e.g.
*   Object-Oriented Programming in C++) should be provided on a single line.
*
*   METHOD: A string stream is used to create a string containing the full
*   course title, e.g. PHYS 30762 Object-Oriented Programming in C++. Each of
*   these strings are stored in a vector. Printing is performed using an iterator.
*   Program can also print out a list of courses for a particular year, as identified
*   by the first digit of the course code. It is also possible to sort the list of
*   courses by title or course code.
*
*   INPUT:
*      - file named as <filename.dat> containing strings like, for example,
*        30762 Object-Oriented Programming in C++
*      - courses can be provided by manual input to screen, typing course code
*        and title on the same line as above
* 
*   RETURN:
*      - sorted full course list or course for a particular year
*
*   WARNINGS / ERRORS:
*      - warning: ignoring bad input (e.g. no course code is specified)
*      - error: there is no data to print (i.e. if data vector is empty)
*      - error: cannot find / open specified input file
*      - error: cannot recognise input (e.g. asks for 0-4, user enters 5)
*      - error: something went wrong (this should not appear in general)
*
* ----------------------------------------------------------------------*/

#include <iostream>  // cout, endl
#include <string>    // string
#include <sstream>	 // stringstream
#include <vector>    // vector
#include <algorithm> // sort and reverse
#include <cstdlib>   // EXIT_FAILURE, EXIT_SUCCESS
#include <exception> // exception to catch and end C++ code nicely
#include <fstream>   // fstream, ifstream, ofstream
#include <limits>    // numeric_limits<streamsize>::max()

using namespace std;

// define error / warning messages
const string TRY_AGAIN{ "Please try again or press <q> to quit. " };
const string INPUT_FORMAT_WARNING{ "WARNING: Ignoring bad input " };
const string NO_DATA_ERROR{ "ERROR: There is no data to print! " };
const string CANNOT_OPEN_FILE_ERROR{ "ERROR: Specified file could not be opened! " };
const string INPUT_UNRECOGNISED_ERROR{ "ERROR: Input was not recognised. " };
const string SOMETHING_WRONG_ERROR{ "ERROR: Something went wrong... " };

// define constants
const string degreeCodePrefix{ "PHYS" }; // default degree code prefix

/* ------------------------------------------------------------------------
* MENU - PRINT OPTIONS FOR USER TO SELECT
* ----------------------------------------------------------------------*/

void callMenu() {
	cout << endl
		<< "----------------------------------------------------" << endl
		<< "   DATA MANAGEMENT CHOICES                          " << endl
		<< "----------------------------------------------------" << endl
		<< "   <m> - call this menu                             " << endl
		<< "   <q> - quit program                               " << endl
		<< "   <0> - print all courses                          " << endl
		<< "   <n> - extract particular year courses            " << endl
		<< "   <a> - sort by name (a - z)                       " << endl
		<< "   <z> - sort by name (z - a)                       " << endl
		<< "   <l> - sort by code (low - high)                  " << endl
		<< "   <h> - sort by code (high - low)                  " << endl
		<< "----------------------------------------------------" << endl << endl;
}

/* ------------------------------------------------------------------------
* CONVERT STRING TO SRING THAT HAS ONLY UPPERCASE CHARS
* ----------------------------------------------------------------------*/

string strUpperCase(string s) {
	int i{ 0 };
	while (s[i]) { // go through all string characters
		s[i] = toupper(s[i]); // convert all string letters to uppercase
		i++;
	}
	return s;
}

/* ------------------------------------------------------------------------
* LET USER TO DECIDE IF INPUT FROM SCREEN OR FILE
* ----------------------------------------------------------------------*/

void fileOrScreen(string& ifFileOrScreen) {
	bool notFinished{ true };
	while (notFinished) {
		cout << endl << "To type input manually or load from file type <m> or <f> respectively: ";
		cin >> ifFileOrScreen;
		// convert string to string containing only uppercase letters
		ifFileOrScreen = strUpperCase(ifFileOrScreen);
		if (ifFileOrScreen == "Q") { // check if user wants to quit program
			notFinished = false;
			throw exception(); // finish
		}
		else if ((ifFileOrScreen != "F") && (ifFileOrScreen != "M")) {
			// user did not enter "Q", "F" or "M"
			cerr << INPUT_UNRECOGNISED_ERROR << TRY_AGAIN << endl;
		}
		else notFinished = false; // received either F or M
	}
}

/* ------------------------------------------------------------------------
* READ IN DATA FROM FILE
* ----------------------------------------------------------------------*/

void readFromFile(vector<string>& courses) {
	string input, fileName;
	// ask user to enter file name and open file
	cout << endl << "Enter data file name or type <ok> for default 'courselist.dat': ";
	cin >> input;
	input = strUpperCase(input);
	if (input == "OK") {
		fileName = "courselist.dat"; // default file name
	}
	else fileName = input;
	// let user know which file will be opened
	cout << endl << "Opening file '" << fileName << "'... " << endl;
	ifstream inFile(fileName); // open input file
	// check if file opened successfully
	if (!inFile.good()) {
		// print error message and exit if cannot open / find file
		cerr << CANNOT_OPEN_FILE_ERROR << endl;
		throw exception(); // finish
	}
	// read data from file, store into string stream buffer
	cout << "Reading data... " << endl;
	// initialise line counter, course code number
	int lineNo{ 0 };
	string line;
	while (getline(inFile, line)) {
		lineNo++; // keep track of lines in file
		// ignore any empty lines, check if not end of file
		if (!line.empty() && !inFile.eof()) {
			// make vector of strings containing the full course title
			stringstream ifss{ line }; // input file string stream
			int courseCode; // used to check if there is integer course code
			if (ifss >> courseCode) {
				string courseNamePart; // used to gather title word by word
				// use a string stream to create a string containing the full course title
				// e.g. PHYS 30762 Object-Oriented Programming in C++, where "PHYS" - prefix
				stringstream oss;
				oss << degreeCodePrefix << " " << courseCode;
				// read course title word by word
				while (ifss >> courseNamePart) {
					oss << " " << courseNamePart;
				}
				// each of these strings are stored in a vector
				courses.push_back(oss.str());
			}
			else {
				// ignore bad input, print which line was bad
				cerr << INPUT_FORMAT_WARNING << "(line number = " << lineNo << ")" << endl;
			}
		}
	}
	cout << "Data was successfully stored " << endl << endl;
}

/* ------------------------------------------------------------------------
* READ IN DATA FROM SCREEN
* ----------------------------------------------------------------------*/

void readFromScreen(vector<string>& courses) {
	string line, input;
	cout << endl << "Please enter a course list. " << endl;
	cout << "NB: provide course code, e.g. 20312, and title, e.g. Wave Optics, on a single line." << endl;
	cout << "Press ENTER after each course and type <q> when finished: " << endl;
	while (input != "Q") {
		// read data from screen
		getline(cin, line);
		input = strUpperCase(line); // convert to capital letters
		if (!line.empty()) { // ignore any empty lines
			stringstream ifss{ line }; // input file string stream
			int courseCode; // used to check if there is integer course code
			if (ifss >> courseCode) {
				string courseNamePart; // used to gather title word by word
				// use a string stream to create a string containing the full course title
				// e.g. PHYS 30762 Object-Oriented Programming in C++, where "PHYS" - prefix
				stringstream oss;
				oss << degreeCodePrefix << " " << courseCode;
				// read course title word by word
				while (ifss >> courseNamePart) {
					oss << " " << courseNamePart;
				}
				// each of these strings are stored in a vector
				courses.push_back(oss.str());
			}
			else if (strUpperCase(line) != "Q") {
				// ignore bad input
				cerr << INPUT_FORMAT_WARNING << endl;
			}
		}
	}
	cout << "Data was successfully stored " << endl << endl;
}

/* ------------------------------------------------------------------------
* CALL READ FROM FILE OR SCREEN FUNCTIONS
* ----------------------------------------------------------------------*/

void callRead(string ifFileOrScreen, vector<string>& courses) {
	if (ifFileOrScreen == "F") { // selected file input
		readFromFile(courses);
	}
	else if (ifFileOrScreen == "M") { // selected manual screen input
		readFromScreen(courses);
	}
	else {
		cout << SOMETHING_WRONG_ERROR << endl;
		throw exception(); // finish
	}
}

/* ------------------------------------------------------------------------
* PRINT TO SCREEN
* ----------------------------------------------------------------------*/

void printToScreen(vector<string> courses) {
	cout << endl << "List of courses:" << endl << endl;
	if (!courses.empty()) { // check if data vector is not empty
		// print out using iterators
		for (vector<string>::iterator i = courses.begin(); i < courses.end(); ++i) {
			cout << *i << endl;
		}
		cout << endl << "Number of courses printed: " << courses.size() << endl;
	}
	else {
		cerr << NO_DATA_ERROR << endl; // data vector was empty
		throw exception();
	}
}

/* ------------------------------------------------------------------------
* GET USER MENU OPTIONS
* ----------------------------------------------------------------------*/

string getMenuOptions() {
	string reply;
	bool notFinished{ true };
	while (notFinished) {
		cout << "Select menu option: ";
		cin >> reply;
		//cout << "Option choice received: " << reply << endl;
		// get, check and convert string to string containing only uppercase letters
		reply = strUpperCase(reply);
		// check all the options
		if ((reply != "M") && (reply != "Q") && (reply != "A") && (reply != "Z") &&
			(reply != "L") && (reply != "H") && (reply != "0") && (reply != "N")) {
			// option was not recognised
			cerr << INPUT_UNRECOGNISED_ERROR << TRY_AGAIN << endl;
			cin.clear(); // clear bad input
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear bad input
		}
		else if (reply == "N") { // option was "N", get further options
			while (notFinished) {
				cout << "Enter a year of interest (1 - 4 or 0 for all): ";
				cin >> reply;
				//cout << "Option choice received: " << reply << endl;
				if ((reply != "0") && (reply != "1") && (reply != "2") && (reply != "3")
					&& (reply != "4")) {
					// option was not recognised, e.g. entered "5"
					cerr << INPUT_UNRECOGNISED_ERROR << TRY_AGAIN << endl;
					cin.clear(); // clear bad input
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear bad input
				}
				else notFinished = false;
			}
		}
		else notFinished = false;
	}
	return reply;
}

/* ------------------------------------------------------------------------
* COMPARE COURSE CODES FOR SORTING
* ----------------------------------------------------------------------*/

bool compareCourseCode(string a, string b) {
	// take two strings and compare
	stringstream ass{ a }, bss{ b };
	string aPrefix, bPrefix;
	int aCode, bCode;
	// read prefix, then code and compare codes in return
	ass >> aPrefix >> aCode;
	bss >> bPrefix >> bCode;
	return aCode < bCode;
}

/* ------------------------------------------------------------------------
* COMPARE COURSE TITLES FOR SORTING
* ----------------------------------------------------------------------*/

bool compareCourseTitle(string a, string b) {
	// take two strings and compare
	stringstream ass{ a }, bss{ b };
	string aPrefix, bPrefix, aTitleFirstPart, bTitleFirstPart;
	int aCode, bCode;
	// read prefix, code, then first word in title and compare first title words
	ass >> aPrefix >> aCode >> aTitleFirstPart;
	bss >> bPrefix >> bCode >> bTitleFirstPart;
	return aTitleFirstPart < bTitleFirstPart;
}

/* ------------------------------------------------------------------------
* MANAGE OPTIONS
* ----------------------------------------------------------------------*/

void manageOptions(vector<string> courses) {
	// make a copy of a vector for later procedures
	vector<string> allCourses = courses;
	bool notFinished{ true };
	while (notFinished) {
		string option = getMenuOptions(); // get user option from menu
		if (option == "M") {
			callMenu();
			continue; // start loop again
		}
		else if (option == "Q") {
			notFinished = false;
			throw exception(); // quit program
		}
		if (option == "A") { // sort by title, a - z
			sort(courses.begin(), courses.end(), compareCourseTitle);
			printToScreen(courses);
		}
		else if (option == "Z") { // sort by title, z - a
			sort(courses.rbegin(), courses.rend(), compareCourseTitle);
			printToScreen(courses);
		}
		else if (option == "L") { // sort by course code, low - high
			sort(courses.begin(), courses.end(), compareCourseCode);
			printToScreen(courses);
		}
		else if (option == "H") { // sort by course code, high - low
			sort(courses.rbegin(), courses.rend(), compareCourseCode);
			printToScreen(courses);
		}
		else if (option == "0") printToScreen(allCourses); // print all courses
		else {
			vector<string> someCourses; // stores courses of a particular year (temporary)
			// iterate through a copy of original list
			for (vector<string>::iterator i = allCourses.begin(); i < allCourses.end(); ++i) {
				stringstream ss{ *i };
				string prefix;
				int code;
				ss >> prefix >> code; // get course number
				// if course code first number matches user option, store to vector
				if (atoi(option.c_str()) == (code / 10000)) { // code is of type int, so ok to do this
					someCourses.push_back(*i);
				}
			}
			printToScreen(someCourses); // print out only courses of interest
			// give user a count of how many courses there are at a level of interest
			cout << endl << "Number of courses at level " << option << " is: " << someCourses.size()
				<< endl;
			courses = someCourses; // allows user to sort courses of a selected year
		}
		callMenu(); // remind user the options
	}
}

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* ----------------------------------------------------------------------*/

int main(void) {
	try {
		string ifFileOrScreen;
		// let user to decide if input from file or manual screen input
		fileOrScreen(ifFileOrScreen); // F = file, M = manual screen input, Q = quit
		// read in data from file or screen and store into vector of strings
		vector<string> courses;
		callRead(ifFileOrScreen, courses);
		cout << "Number of courses read: " << courses.size() << endl;
		callMenu(); // call menu showing choices
		manageOptions(courses);
	}
	catch (const exception&) { // caught exception in one of the functions
		cout << endl << "Bye!" << endl << endl;
		return EXIT_FAILURE;
	}
	cout << endl << "Bye!" << endl << endl;
	return EXIT_SUCCESS;
}