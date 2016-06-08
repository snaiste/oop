/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++
*   Date: 12 February 2016
*   Author: Aiste Norkute
*
* ----------------------------------------------------------------------*/

#include <iostream>  // cout, endl
#include <iomanip>   // precision
#include <string>    // string
#include <cmath>     // pow
#include <cstdlib>   // EXIT_FAILURE, EXIT_SUCCESS
#include <fstream>   // fstream, ifstream, ofstream
#include <cstdint>   // size_t
#include <limits>    // numeric_limits<streamsize>::max()

using namespace std;

// define error / warning messages
const string INPUT_FORMAT_WARNING{ "Warning: ignoring unreadable value " };
const string CANNOT_OPEN_FILE_ERROR{ "Error: specified file could not be opened!" };

// define constants
const double eToCoulomb = pow(10, -19); // conversion from charge in electron charge to Coulombs

/* ------------------------------------------------------------------------
* COMPUTE MEAN
* ----------------------------------------------------------------------*/

void getMean(double *data, size_t N, double& mean) {
	double sum{ 0 }; // initialise sum
	// compute sum of data values
	for (size_t i{ 0 }; i < N; i++) {
		sum += data[i];
	}
	// compute data mean
	mean = (sum / N);
}

/* ------------------------------------------------------------------------
* COMPUTE STANDARD DEVIATION
* ----------------------------------------------------------------------*/

void getSD(double *data, size_t N, double mean, double& SD) {
	double sum{ 0 }; // initialise sum of differences squared
	// compute sum of differences from mean value squared
	for (size_t i{ 0 }; i < N; i++) {
		sum += pow((data[i] - mean), 2.);
	}
	// compute standard deviation of data
	SD = sqrt(sum / (N - 1));
}

/* ------------------------------------------------------------------------
* COMPUTE STANDARD ERROR IN THE MEAN
* ----------------------------------------------------------------------*/

void getError(size_t N, double SD, double& error) {
	// compute standard error in the mean
	error = (SD / sqrt(N));
}

/* ------------------------------------------------------------------------
* PRINT OUTPUT
* ----------------------------------------------------------------------*/

void printOutput(double& mean, double& SD, double& error) {
	// print output in Coulomb and format precision
	cout << endl;
	cout << "Mean = " << setprecision(5) << mean * eToCoulomb << " Coulomb" << endl;
	cout << "Standard Deviation = " << setprecision(5) << SD * eToCoulomb << " Coulomb" << endl;
	cout << "Standard Error on the Mean = " << setprecision(5) << error * eToCoulomb << " Coulomb" << endl;
	cout << endl;
}

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* ----------------------------------------------------------------------*/

int main() {

	// ask user to enter file name and open file
	string fileName{ "" }; // initialise empty file name
	cout << "\nEnter data file name, e.g. <millikan.dat>: "; // ask user to enter filename
	cin >> fileName;
	cout << "\nOpening file '" << fileName << "'... " << endl;
	ifstream file(fileName); // open input file
	// check if file opened successfully
	if (!file.good()) {
		// print error message and exit
		cerr << CANNOT_OPEN_FILE_ERROR << endl;
		exit(EXIT_FAILURE);
	}

	size_t N{ 10 }; // initialise data array size N = 10 to start with
	size_t good{ 0 }; // initialise good value counter
	size_t bad{ 0 }; // initialise bad value counter
	int line{ 0 }; // initialise line counter
	// allocate memory for dynamic data array
	// dynamically create pointers to doubles
	double *data = new double[N]; // point dynamic array to new array of N zeros

	// read data from file, ignoring any additional bad data
	cout << "Reading data... " << endl;
	double value{ 0 }; // initialise data value
	while (!file.eof()) {
		line++; // keep track of lines in file
		//cout << value << endl;
		file >> value;
		if (!file.fail() && !file.eof()) { // if read value is not of defined type
			data[good] = value;
			good++; // found good value, increment good value counter
			// dynamically resizing data array
			if (good == N-1) { // reached initial allocation size
				N = N * 2;
				double *temp = new double[N]; // create a new array of doubled size
				//cout << "Copying from data to temp, " << good << "values" << endl;
				for (int i{ 0 }; i < N; i++) {
					temp[i] = data[i];
				}

				delete[] data; // delete the old array - we do not need it anymore
				// change the pointer. We still want the array to be called "data" 
				// (its original name), so change the data pointer to the new address
				data = temp; // point data to the temp array
			}
		}
		else {
			file.clear(); // clear bad input
			file.ignore(numeric_limits<streamsize>::max(), '\n'); // skips to the next newline
			// will only skip up to numeric_limits<streamsize>::max() characters
			// this is to ignore anything else on the same line as the non-number so that it 
			// does not cause another parse failure.
			if (!file.eof()) {
				cerr << INPUT_FORMAT_WARNING << "(line number = " << line << ")" << endl;
				bad++;
			}
		}
	}
	// finished reading from file
	file.close(); // close file

	N = good; // update N
	// print number of measurements read in
	cout << "Number of floating point numbers read: " << N << endl;
	cout << "Number of total entries in the file: " << good + bad << endl;

	// debug: print data 
	/*
	for (int i{0}; i < N; i++) {
	cout << data[i] << endl;
	}
	*/

	// compute data stats
	double mean{ 0 }; // initialise data mean
	getMean(data, N, mean); // compute data mean
	double SD{ 0 }; // initialise standard deviation
	getSD(data, N, mean, SD); // compute standard deviation
	double error{ 0 }; // initialise standard error in the mean
	getError(N, SD, error);

	// print formatted output in Coulomb
	printOutput(mean, SD, error);

	delete[] data; // free memory allocated to data
	return EXIT_SUCCESS;
}