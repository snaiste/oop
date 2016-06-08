#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <iostream>  // std
#include <memory>    // smart pointers
#include <limits>    // numeric_limits<streamsize>::max()
#include <algorithm> // transform(), sort()
#include <fstream>   // fstream, ifstream, ofstream
#include <exception> // exceptions
#include <list>      // list
#include <string>    // string
#include <sstream>   // stringstream
#include <cmath>     // sqrt, pow
#include <iomanip>   // setprecision
#include <utility>   // move

#include "msg.hpp"         // classes managing output messages
#include "userInput.hpp"   // basic user input template function
#include "measurement.hpp" // classes containing measurements

/* ------------------------------------------------------------------------
* DECLARE PROJECT HEADER LINE CLASS
* -----------------------------------------------------------------------*/

class HeaderLine {
	friend std::istream& operator>>(std::istream& is, HeaderLine& userStaffProject);
	friend std::ostream& operator<<(std::ostream& os, const HeaderLine& userStaffProject);
private:
	std::string name;
public:
	// default constructor
	HeaderLine();
	// parametrised constructor
	HeaderLine(const std::string& userName);
	// copy constructor for deep copying
	HeaderLine(const HeaderLine& userStaffProject);
	// move constructor
	HeaderLine(HeaderLine&& userStaffProject);
	// destructor
	~HeaderLine();
	// pure virtual access function
	std::string getName() const;
	// copy assignment operator
	HeaderLine& operator=(const HeaderLine& userStaffProject);
	// move assignment operator
	HeaderLine& operator=(HeaderLine&& userStaffProject);
};

/* ------------------------------------------------------------------------
* DECLARE EXPERIMENT CLASS TEMPLATE: CONTAINS CONTENTS OF ONE DATA FILE
* -----------------------------------------------------------------------*/

template <typename T> class Experiment {
protected:
	HeaderLine staffName;
	HeaderLine projectName;
	std::list<Measurement<T>> measurements;
public:

	// default constructor
	Experiment() {
		DebugMsg::print("[EXPERIMENT] Default constructor called\n");
	}

	// parametrised constructor
	Experiment(const HeaderLine& userStaffName, const HeaderLine& userProjectName,
		const std::list<Measurement<T>>& userMeasurements) {
		DebugMsg::print("[EXPERIMENT] Parametrised constructor called\n");
		this->staffName = userStaffName;
		this->projectName = userProjectName;
		this->measurements = userMeasurements;
	}

	// copy constructor for deep copying
	Experiment(const Experiment& userExperiment) {
		DebugMsg::print("[EXPERIMENT] Copy constructor for deep copying called\n");
		this->staffName = userExperiment.staffName;
		this->projectName = userExperiment.projectName;
		this->measurements = userExperiment.measurements;
	}

	// move constructor
	Experiment(Experiment&& userExperiment) {
		DebugMsg::print("[EXPERIMENT] Move constructor called\n");
		// steal the data
		this->stafName = move(userExperiment.staffName);
		this->projectName = move(userExperiment.projectName);
		this->measurements = move(userExperiment.measurements);
	}

	// default destructor
	~Experiment() {
		DebugMsg::print("[EXPERIMENT] Default destructor called\n");
	}

	// copy assignment operator
	Experiment& operator=(const Experiment& userExperiment) {
		DebugMsg::print("[EXPERIMENT] Copy assignment operator called\n");
		if (&userExperiment == this) { return *this; } // no self-assignment
		// first delete this object’s data
		this->staffName = {};
		this->projectName = {};
		this->measurements.clear();
		// declare new object
		this->staffName = userExperiment.staffName;
		this->projectName = userExperiment.projectName;
		this->measurements = userExperiment.measurements;
		return *this;
	}

	// move assignment operator
	Experiment& operator=(Experiment&& userExperiment) {
		DebugMsg::print("[EXPERIMENT] Move assignment operator called\n");
		std::swap(this->staffName, userExperiment.staffName);
		std::swap(this->projectName, userExperiment.projectName);
		std::swap(this->measurements, userExperiment.measurements);
		return *this;
	}

	// access functions
	std::string getStaffName() const { return this->staffName.getName(); }
	std::string getProjectName() const { return this->projectName.getName(); }
	std::list<Measurement<T>> getMeasurements() const { return this->measurements; }
	size_t getNoOfMeasurements() const { return this->measurements.size(); }

	// reading from file function
	void readFromFile(const std::string& userFile) {
		DebugMsg::print("[EXPERIMENT] Reading from file '" + userFile + "'\n");
		std::ifstream inFile(userFile);
		try {
			if (!inFile.is_open()) throw std::ifstream::failure("[EXPERIMENT] Exception opening file '" + userFile + "'\n");
			else {
				// declare temporary string for skipping explanatory string in header
				// and horizontal line
				std::string temporary;
				// read from file
				inFile >> temporary >> staffName;
				inFile >> temporary >> projectName;
				inFile >> temporary;
				// declare new Measurement class object
				Measurement<T> measurement;
				// while input is readable, push back to measurements list
				while (inFile >> measurement) {
					measurements.push_back(measurement);
				}
			}
		}
		catch (const std::ifstream::failure& e) {
			ErrorMsg::print(e.what());
		}
		inFile.close();
	}

	// reading from screen function
	void readFromScreen() {
		ScreenMsg::print("Type staff member name >> ");
		staffName = getInput<std::string>();
		ScreenMsg::print("Type project name >> ");
		projectName = getInput<std::string>();
		ScreenMsg::print("Provide timestamp, e.g. 1462033286 (Unix timestamp)\n");
		ScreenMsg::print("followed by data value on one line\n");
		ScreenMsg::print("Press ENTER after each line and type any letter when finished:\n");
		// declare new Measurement class object
		Measurement<T> measurement;
		// while input is readable, push back to measurements list
		while (std::cin >> measurement) {
			measurements.push_back(measurement);
		}
		std::cin.clear(); // clear rubbish!
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore rubbish!
		if (!measurements.empty()) {
			ScreenMsg::print("[EXPERIMENT] Data was successfuly stored\n");
		}
		else {
			std::cin.clear(); // clear rubbish!
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore rubbish!
			throw std::invalid_argument("\n[EXPERIMENT] No data was stored!\n");
		}
	}

	// compute mean of data
	T getMean() const {
		T sum{};
		// iterate and sum over all elements
		for (auto it = measurements.begin(); it != measurements.end(); ++it) {
			sum += it->getDataPoint();
		}
		T measurementCount = this->getNoOfMeasurements();
		// return mean
		return sum / measurementCount;
	}

	// compute standard deviation
	T getStandardDeviation() const {
		T sum{};
		// iterate and sum over all elements minus means squared
		for (auto it = measurements.begin(); it != measurements.end(); ++it) {
			sum += (it->getDataPoint() - this->getMean()) * (it->getDataPoint() - this->getMean());
		}
		T measurementCount = this->getNoOfMeasurements() - 1;
		T sumOverCount = sum / measurementCount;
		// return standard deviation
		// cast to T, as returns double, but if T is int, then we want int!
		T standardDeviation = (T)std::sqrt(sumOverCount);
		return standardDeviation;
	}

	// compute error in mean
	T getErrorInMean() const {
		T measurementCount = this->getNoOfMeasurements();
		// cast to T, as returns double, but if T is int, then we want int!
		T standardError = (T)(this->getStandardDeviation() / std::sqrt(measurementCount));
		return standardError;
	}

	// return analysis report
	std::string getReport() const {
		int prec{ 5 }; // pick precision value
		std::ostringstream stringStream;
		stringStream << std::endl
			<< "Staff: " << this->staffName.getName() << std::endl
			<< "Project: " << this->projectName.getName() << std::endl
			<< "-----------------------------" << std::endl
			<< "Number of measurements: " << std::setprecision(prec)
			<< this->getNoOfMeasurements() << std::endl
			<< "Mean: " << std::setprecision(prec)
			<< this->getMean() << std::endl
			<< "Standard Deviation: " << std::setprecision(prec)
			<< this->getStandardDeviation() << std::endl
			<< "Error in the mean: " << std::setprecision(prec)
			<< this->getErrorInMean() << std::endl
			<< "-----------------------------" << std::endl;
		return stringStream.str();
	}

	// delete measurement range
	bool deleteMeasurementRange(const unsigned& startTime, const unsigned& endTime) {
		// not values matching the range found by default
		bool foundValues{ false };
		auto it = measurements.begin();
		// iterate through the list of measurements
		while (it != measurements.end()) {
			// if timestamp is within start-end times, erase
			if (it->getTimestamp() >= startTime && it->getTimestamp() <= endTime) {
				measurements.erase(it++);
				foundValues = true;
			}
			else ++it;
		}
		if (!foundValues) {
			// no values found
			ErrorMsg::print("\n[EXPERIMENT] Specified range does not exist\n");
			return false;
		}
		return true;
	}
};
// declare new type for accessing shared pointers to experiments
template <typename T>
using ExperimentSharedPtr = std::shared_ptr<Experiment<T>>;

/* ------------------------------------------------------------------------
* DECLARE PROJECT CLASS TEMPLATE: CONTAINS CONTENTS OF ONE PROJECT
* -----------------------------------------------------------------------*/

template <typename T> class Project : public Experiment<T> {
public:

	// default constructor - calling base class constructor
	Project() : Experiment<T>() {
		DebugMsg::print("[PROJECT] Default constructor called\n");
	};

	// parametrised constructor
	Project(ExperimentSharedPtr<T> userExperimentPtr){
		DebugMsg::print("[PROJECT] Parametrised constructor called\n");
		this->staffName = HeaderLine{ userExperimentPtr.get()->getStaffName() };
		this->projectName = HeaderLine{ userExperimentPtr.get()->getProjectName() };
		this->measurements = userExperimentPtr.get()->getMeasurements();
	}

	// copy constructor for deep copying - calling base class copy constructor
	Project(const Project& userProject) : Experiment<T>(userProject) {
		DebugMsg::print("[PROJECT] Copy constructor for deep copying called\n");
	}

	// move constructor - calling base class move constructor
	Project(Project&& userProject) : Experiment<T>(userProject) {
		DebugMsg::print("[PROJECT] Move constructor called\n");
	}

	// default destructor
	~Project() {
		DebugMsg::print("[PROJECT] Default destructor called\n");
	}

	// copy assignment operator - calling base class assignment operator
	Project& operator=(const Project& userProject) {
		DebugMsg::print("[PROJECT] Copy assignment operator called\n");
		Experiment<T>::operator=(userProject);
		return *this;
	}

	// move assignment operator - calling base class move assignment operator
	Project& operator=(Project&& userProject) {
		DebugMsg::print("[PROJECT] Move assignment operator called\n");
		Experiment<T>::operator=(userProject);
		return *this;
	}

	// merge experiments if they belong to the same project
	void mergeExperiment(const Experiment<T>& userExperiment) {
		try {
			if (userExperiment.getProjectName() != this->getProjectName()
				|| userExperiment.getStaffName() != this->getStaffName()) {
				throw std::invalid_argument("[PROJECT] Cannot merge different projects!");
			}
			else {
				this->measurements.merge(userExperiment.getMeasurements());
			}
		}
		catch (const std::invalid_argument& e) {
			ErrorMsg::print(e.what());
		}
	}
};

// declare new type for accessing shared pointers to projects
template <typename T>
using ProjectSharedPtr = std::shared_ptr<Project<T>>;
// declare new type for accessing weak pointers to projects
template <typename T>
using ProjectWeakPtr = std::weak_ptr<Project<T>>;

#endif /* PROJECT_HPP */