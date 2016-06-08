/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++
*   Date: 22 February 2016
*   Author: Aiste Norkute
*
* ----------------------------------------------------------------------*/

#include <iostream> // cout, endl
#include <string>   // string
#include <vector>   // vector
#include <cmath>    // pow
#include <stdlib.h> // rand()
#include <regex>    // regular expressions

using namespace std;

// define constants
const double massSun{1.989 * pow(10, 30.)}; // mass of the Sun in kg

// define enum type and corresponding array and strings
enum hubbleType{E0, E1, E2, E3, E4, E5, E6, E7, S0, Sa, Sb, Sc, SBa, SBb, SBc, Irr};
static const hubbleType hTypes[] = {E0, E1, E2, E3, E4, E5, E6, E7, S0, Sa, Sb, Sc, SBa, SBb, SBc, Irr};
static const char * hubbleTypeStrings[] = {"E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "S0", "Sa", "Sb", "Sc", "SBa", "SBb", "SBc", "Irr"};

// define error / warning messages
const string INPUT_FORMAT_ERROR{ "Input is invalid! " };
const string NO_DATA_ERROR{"ERROR: There is no data to print! "};
const string TRY_AGAIN{ "Please try again: " };

/* ------------------------------------------------------------------------
* DEFINE A CLASS
* ----------------------------------------------------------------------*/

class Galaxy {
private:
	hubbleType hType; // Hubble Type of enum type
	double redShift, totalMass, stellarMassFraction; // other object properties
	vector<Galaxy> satellites; // vector containing satellites

public:
	// default constructor
	Galaxy() : hType{E0}, redShift{0.001}, totalMass{pow(10, 12.) * massSun}, 
			   stellarMassFraction{0.025} {}

	// parametrised constructor
	Galaxy(hubbleType phubbleType, double predShift, double ptotalMass, double pstellarMassFraction) :
		   hType{phubbleType}, redShift{predShift}, totalMass{ptotalMass * massSun}, stellarMassFraction{pstellarMassFraction} {
	}
	// destructor
	~Galaxy() {} 
	// return stellar mass
	double stellarMass() {return stellarMassFraction * totalMass;}
	// change ga = laxy's Hubble type
	void changeHubbleType(hubbleType newType) {hType = newType;}
	// prototype for function to print out an object's data to screen
	void printToScreen(); 
	// add satellite galaxy
	void addSatellite() { 
		int arrayLength = sizeof(hTypes) / sizeof(hTypes[0]); // find array length
		int index = rand() % arrayLength; // there are arrayLength Hubble types, pick random index
		double redshift = (double) (rand() % 10); // generate redshifts: [0; 10]
		int powerOfTen = rand() % 7 + 12; // for total masses: [10e7,10e12] * Msun
		double fraction = rand() % 5; // for stellar mass fractions: [0,0.05]
		satellites.push_back(Galaxy(hTypes[index], redshift, (double)pow(10, powerOfTen), fraction / 100.0)); 
	}
};

/* ------------------------------------------------------------------------
* CONVERT ENUM TO STRING
* ----------------------------------------------------------------------*/

const char * getStringFromEnum(int hubbleTypeEnum) {
 	// need to convert enum to string to see proper values
	return hubbleTypeStrings[hubbleTypeEnum];
}
	
/* ------------------------------------------------------------------------
* PRINT TO SCREEN
* ----------------------------------------------------------------------*/

void Galaxy::printToScreen() {
	// print out an object's data	
    cout.precision(3);
    cout << "Galaxy"          	      << "\t" << "\t"
 	     << getStringFromEnum(hType)  << "\t" << "\t"
         << redShift                  << "\t" << "\t"
         << totalMass                 << "\t" << "\t" << "\t"
    	 << stellarMassFraction       << "\t" << "\t" << "\t" << "\t"
    	 << stellarMass()             << "\t" << "\t" << "\t" 
    	 << satellites.size()         << "\t"
    	 << endl; 
    // print all satellites for this object
    for (auto i = satellites.begin(); i < satellites.end(); ++i) {
	    cout << "Satell"                  << "\t" << "\t"
	 	     << getStringFromEnum(hType)  << "\t" << "\t"
	         << redShift                  << "\t" << "\t"
	         << totalMass                 << "\t" << "\t" << "\t"
	    	 << stellarMassFraction       << "\t" << "\t" << "\t" << "\t"
	    	 << stellarMass()             << "\t" << "\t" << "\t" 
	    	 << "-"				          << "\t"
	    	 << endl;
	}	
}

/* ------------------------------------------------------------------------
* TABLE HEADER
* ----------------------------------------------------------------------*/

void getTableHeader() {
	// get table fields names
	cout << endl;
	cout << "Object Type"           << "\t" 
		 << "Hubble Type"           << "\t" 
         << "Redshift"              << "\t" 
         << "Total Mass (kg)"       << "\t" << "\t"
    	 << "Stellar Mass fraction" << "\t" << "\t"
    	 << "Stellar Mass (kg)"     << "\t" 
    	 << "Number of Satellites"  << "\t" 
    	 << endl;
}

/* ------------------------------------------------------------------------
* GENERATE RANDOM GALAXY LIST AND SATELLITES
* ----------------------------------------------------------------------*/

vector<Galaxy> getGalaxyList(int howMany) {
	vector<Galaxy> galaxyList;
	int arrayLength = sizeof(hTypes) / sizeof(hTypes[0]); // find array length
	for (int i{0}; i < howMany; ++i) { 
		// generate random values
		int index = rand() % arrayLength; // there are arrayLength Hubble types, pick random index
		double redshift = (double) (rand() % 10); // generate redshifts: [0; 10]
		int powerOfTen = rand() % 12 + 7; // for total masses: [10e7,10e12] * Msun
		double fraction = rand() % 5 + 1; // for stellar mass fractions: [0,0.05]
		Galaxy g{hTypes[index], redshift, (double)pow(10, powerOfTen), fraction / 100.0};
		// assume up to 5 satellites	
		int noOfSatellites = rand() % 5;
		// generate satellites
		for (int i{0}; i < noOfSatellites; ++i) {
			g.addSatellite(); // add satellite to current object
		}
		galaxyList.push_back(g); // push object and all its data 			
	}
	return galaxyList;
}

/* ------------------------------------------------------------------------
* GET AND CHECK INTEGER INTPUT
* ----------------------------------------------------------------------*/

int getInputValues() {
	regex reg{0};
	// define format of input: 
	// "[[:s:]]*" = any number of spaces
	// "[[:digit:]]*" = any number of digits
	reg = "([[:s:]])*([[:digit:]])*";
	string input{}; // initialise empty string
	getline(cin, input);
	bool flag{ regex_match(input, reg) };
	while (!flag) { // check if value matches reg
		cout << INPUT_FORMAT_ERROR << TRY_AGAIN; // ask user to try again if it doesn't match
		getline(cin, input);
		flag = regex_match(input, reg);
	}
	return atoi(input.c_str()); // atoi() converts from string to integer
}

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* ----------------------------------------------------------------------*/

// Main program
int main() {

	// example using default constructor  
	Galaxy andromeda; // create new object, named as Andromeda
	// example of changing type function
	andromeda.changeHubbleType(Sb); // change type from E0 to Sb
    cout << endl << "Here is an example of an object (Andromeda) in this program: " << endl;
	getTableHeader(); // get table fields names
    andromeda.printToScreen(); // print data

    // example using parametrised constructor
	int noOfGalaxies{0}; // initialise number of galaxies
	cout << endl << "How many galaxies like above Andromeda would you like to generate? ";
	noOfGalaxies = getInputValues(); // check how many galaxies user wants to generate
	vector<Galaxy> galaxyList = getGalaxyList(noOfGalaxies); // get a random list of galaxies

    
	// print out data
	cout << endl << "List of galaxies (and their satellites) and their properties:" << endl;
	getTableHeader();
	for (auto i = galaxyList.begin(); i < galaxyList.end(); ++i) {
		// here we use the arrow -> operator to get a class member of a dereferenced pointer,
        // i->printToScreen() is the same as (*i).printToScreen(), but easier to read
        i->printToScreen();
    }

	return EXIT_SUCCESS;
}