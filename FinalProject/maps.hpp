#ifndef MAPS_HPP
#define MAPS_HPP

#include <iostream> // std
#include <list>     // list
#include <map>      // map
#include <sstream>  // stringstream

#include "measurement.hpp" // classes managing measurements
#include "project.hpp"     // classes managing project

/* ------------------------------------------------------------------------
* DEFINE SOME TYPES
* -----------------------------------------------------------------------*/

// staff database and project database are multimaps of strings
using ProjectReferenceDbType = std::multimap<std::string, std::string>;
// project database key is a pair of staff and project
using ProjectDbKeyType = std::pair<std::string, std::string>;
// project reference database contains shared pointers to measurements
template <typename T> 
using ProjectDbType = std::map<ProjectDbKeyType, ProjectSharedPtr<T>>;
// project shadow database has weak pointers to project database measurements
template <typename T> 
using ProjectShadowDbType = std::map<ProjectDbKeyType, ProjectWeakPtr<T>>;

/* ------------------------------------------------------------------------
* PROJECT REFERENCE DATABASE CLASS
* -----------------------------------------------------------------------*/

template <typename T> class ProjectReferenceDb {
private:
    ProjectReferenceDbType database;
    std::string keys, values;
    // map of weak pointers
    ProjectShadowDbType<T> dataContainer;

public:

    // default constructor
    ProjectReferenceDb() : database{"", ""}, keys{""}, values{""} {
        DebugMsg::print("[PROJECT-REF-DB] Default constructor called\n");        
    }

    // parametrised constructor
    ProjectReferenceDb(const std::string& mapFrom, const std::string& mapTo) 
                      : keys(mapFrom), values(mapTo) {
        DebugMsg::print("[PROJECT-REF-DB] Parametrised constructor called\n");
    }

    // copy constructor for deep copying
    ProjectReferenceDb(const ProjectReferenceDb& userDatabase) {
        DebugMsg::print("[PROJECT-REF-DB] Copy constructor for deep copying called\n");
        this->database = userDatabase.database;
        this->keys = userDatabase.keys;
        this->values = userDatabase.values;
        this->dataContainer = userDatabase.dataContainer;
    }

    // move constructor
    ProjectReferenceDb(ProjectReferenceDb&& userDatabase) {
        DebugMsg::print("[[PROJECT-REF-DB] Move constructor called\n");
        // steal the data
        this->database = move(userDatabase.database);
        this->keys = move(userDatabase.keys);
        this->values = move(userDatabase.values);
        this->dataContainer = move(userDatabase.dataContainer);
    }

    // default destructor
    ~ProjectReferenceDb() { 
        DebugMsg::print("[PROJECT-REF-DB] Default destructor called\n");
    }

    // access functions
    ProjectReferenceDbType getDatabase() const { return this->database; }
    std::size_t getSize() const { return this->database.size(); }

    // copy assignment operator
    ProjectReferenceDb& operator=(const ProjectReferenceDb& userDatabase) {
        DebugMsg::print("[PROJECT-REF-DB] Copy assignment operator called\n");
        if (&userDatabase == this) { return *this; } // no self-assignment
        // first delete this object’s data
        this->database.clear();
        this->keys = "";
        this->values = "";
        this->dataContainer.clear();
        // declare new object
        this->database = userDatabase.database;
        this->keys = userDatabase.keys;
        this->values = userDatabase.values;
        this->dataContainer = userDatabase.dataContainer;
        return *this;
    }

    // move assignment operator
    ProjectReferenceDb& operator=(ProjectReferenceDb&& userDatabase) {
        DebugMsg::print("[PROJECT-REF-DB] Move assignment operator called\n");
        std::swap(this->database, userDatabase.database);  
        std::swap(this->keys, userDatabase.keys); 
        std::swap(this->values, userDatabase.values); 
        std::swap(this->dataContainer, userDatabase.dataContainer); 
        return *this;
    }

    // insert data to map
    void addEntry(const std::string& userKey, const std::string& userValue, 
                  ProjectWeakPtr<T> weakProject) {
        // find key
        auto nameList = database.equal_range(userKey);
        // key not found by default
        bool keyFound = false;
        // iterate through database
        for (auto it = nameList.first; it != nameList.second; ++it) {
            if (it->second == userValue) {
                // found that staff already has this project
                keyFound = true;
            }
        }
        if (!keyFound) {
           // key does not exist yet, hence insert
    	   database.insert(std::make_pair(userKey, userValue));
        }
        // make a key
        auto key = std::make_pair(userKey, userValue);
        // ensure that list is up-to-date
        dataContainer[key] = weakProject;
    }

    // return stringstream of all entries
    std::string show() {
        // print table header
        std::ostringstream stringStream;
        stringStream << std::endl
             << "-----------------------------" << std::endl
             << keys << "\t" << values          << std::endl
             << "-----------------------------" << std::endl;
        for (auto it = this->database.begin(); it != this->database.end(); ++it) {
            stringStream << it->first << "\t" << it->second  << std::endl;
        } 
        return stringStream.str();
    }

    // return stringstream of a specific entry
    std::string show(const std::string& userKey) {
        std::ostringstream stringStream;
        // find all values matching userKey
        auto ret = database.equal_range(userKey);
        // check if data associated with request exists
        if (ret.first == ret.second) {
            ErrorMsg::print("\n[PROJECT-REF-DB] No entry found!\n");
        } else {
            // iterate through that range of values
            for (auto it = ret.first; it != ret.second; ++it) {
                auto key = std::make_pair(it->first, it->second);
                // get table header
                stringStream << std::endl
                         << keys   << ": " << key.first  << std::endl
                         << values << ": " << key.second << std::endl
                         << "-----------------------------" << std::endl
                         << "Timestamp\tMeasurement       " << std::endl
                         << "-----------------------------" << std::endl;
                auto projectWeakPtr = this->dataContainer[key];
                // lock weak pointer while
                if (auto projectPtr = projectWeakPtr.lock()) { 
                    auto measurements = projectPtr.get()->getMeasurements();
                    for (auto iter = measurements.begin(); iter != measurements.end(); ++iter) {
                        stringStream << *iter << std::endl;
                    }
                    stringStream << std::endl;
                } else {
                    // if data was already deleted
                    stringStream << "[PROJECT-REF-DB] Data has expired!" << std::endl;
                }       
            }
        }
        return stringStream.str();
    }

    // return report stingstream matching specific key
    std::string getReport(const std::string& userKey) {
        std::ostringstream stringStream;
        // find all map entries with userKey
        auto ret = database.equal_range(userKey);
        // check if data associated with request exists
        if (ret.first == ret.second) {
            ErrorMsg::print("\n[PROJECT-REF-DB] No entry found!\n");
        } else {
            for (auto it = ret.first; it != ret.second; ++it) {
                // make new key from userKey and its corresponding value in map
                auto key = std::make_pair(it->first, it->second); 
                // get project weak pointers to data
                auto projectWeakPtr = this->dataContainer[key]; 
                // lock creates a shared_ptr that manages the referenced object 
                // so that we can access the data     
                if (auto projectPtr = projectWeakPtr.lock()) {
                    // get report associated with key
                    stringStream << projectPtr.get()->getReport();
                }
            }   
        }
        return stringStream.str(); 
    }
};

/* ------------------------------------------------------------------------
* PROJECT DATABASE CLASS
* -----------------------------------------------------------------------*/

template <typename T> class ProjectDb {
private:
    ProjectDbType<T> database;

public:

    // default constructor
    ProjectDb() {
        DebugMsg::print("[PROJECT] Default constructor called\n");        
    }

    // parametrised constructor
    ProjectDb(const ProjectDbType<T>& userDatabase) : database{userDatabase} {
        DebugMsg::print("[PROJECT] Parametrised constructor called\n");
    }

    // copy constructor for deep copying
    ProjectDb(const ProjectDb& userDatabase) {
        DebugMsg::print("[PROJECT] Copy constructor for deep copying called\n");
        this->database = userDatabase.database;
    }

    // move constructor
    ProjectDb(ProjectDb&& userDatabase) {
        DebugMsg::print("[PROJECT] Move constructor called\n");
        // steal the data
        this->database = move(userDatabase.database);
    }

    // default destructor
    ~ProjectDb() { 
        DebugMsg::print("[PROJECT-DB] Default destructor called\n");
    }
    // access functions
    ProjectDbType<T> getDatabase() const { return this->database; }
    std::size_t getSize() { return this->database.size(); }

    // copy assignment operator
    ProjectDb& operator=(const ProjectDb& userDatabase) {
        DebugMsg::print("[PROJECT] Copy assignment operator called\n");
        if (&userDatabase == this) { return *this; } // no self-assignment
        // first delete this object’s data
        this->database.clear();
        // declare new object
        this->database = userDatabase.database;
        return *this;
    }

    // move assignment operator
    ProjectDb& operator=(ProjectDb&& userDatabase) {
        DebugMsg::print("[PROJECT] Move assignment operator called\n");
        std::swap(this->database, userDatabase.database);  
        return *this;
    }

    // return shared pointers (shared with ProjectReferenceDb class)
    ProjectSharedPtr<T> addEntry(const std::string& staffName, 
                                 const std::string& projectName, 
                                 ExperimentSharedPtr<T> experimentSharedPtr) {
        // make a key
    	auto key = std::make_pair(staffName, projectName);
        // insert data into database
        auto dbProjectIterator = database.find(key);
        if (dbProjectIterator != database.end()) {
            DebugMsg::print("[PROJECT-DB] Existing project found, merging experiments\n");
            // if there already exist data mathing the key, add new data
            Project<T> newProject{experimentSharedPtr};
            (dbProjectIterator->second).get()->mergeExperiment(newProject);
            // database[key] gets automatically updated when the value the pointer is showing to changes
        } else {
            DebugMsg::print("[PROJECT-DB] No project found, adding experiment as project\n");
            // else insert new entry into the map
            Project<T> newProject{experimentSharedPtr};
            auto newProjectPtr = std::make_shared<Project<T>> (newProject);
            database[key] = newProjectPtr;
        }
        // return shared pointers
        return database[key];
    }

    // delete data from the map
    bool deleteEntry(const std::string& staff, const std::string& project) {
        // make key
        auto key = std::make_pair(staff, project);
        if (database.find(key) == database.end()) {
            // key does not exists
            ErrorMsg::print("\n[PROJECT-DB] No project found!\n");
            return false;
        } else {
            // delete data corresponding to a particular key
            database.erase(key);
            ScreenMsg::print("\n[PROJECT-DB] Project deleted\n");
        }
        return true;
    }

    // delete data from the map
    bool deleteMeasurementRange(const std::string& staff, const std::string& project, 
                                const unsigned& startTime, const unsigned& endTime) {
        // make key
        auto key = std::make_pair(staff, project);  
        // insert data into database
        auto dbProjectIterator = database.find(key);  
        if (dbProjectIterator != database.end()) {
            DebugMsg::print("[PROJECT-DB] Existing project found\n");
            Project<T> *theProject = (dbProjectIterator->second).get();
            bool success = theProject->deleteMeasurementRange(startTime, endTime);
			if (success) ScreenMsg::print("\n[PROJECT-DB] Measurements deleted\n");
			else return false;
        } else {
            ErrorMsg::print("\n[PROJECT-DB] No project found\n");
            return false;
        } 
        return true;  
    }

    // return stringstream of all entries    
    std::string show() {
        std::ostringstream stringStream;
        for (auto it = this->database.begin(); it != this->database.end(); ++it) {
            // key contains staff name and project name
            auto key = it->first; 
            // print table header
            stringStream << std::endl
                         << "Staff: "   << key.first        << std::endl
                         << "Project: " << key.second       << std::endl
                         << "-----------------------------" << std::endl
                         << "Timestamp\tMeasurement       " << std::endl
                         << "-----------------------------" << std::endl;
            // get data 
            auto experimentSharedPtr = it->second;
            auto measurements = experimentSharedPtr.get()->getMeasurements();
            for (auto iter = measurements.begin(); iter != measurements.end(); ++iter) {
                stringStream << *iter << std::endl;
            }
        }
        return stringStream.str();
    }

    // return stringstream of a specific entry
    std::string show(const std::string& staffName, const std::string& projectName) {
        std::ostringstream stringStream;
        // make a key
        auto key = std::make_pair(staffName, projectName);
        auto dbProjectIterator = database.find(key);
        // check if such entry exists
        if (dbProjectIterator != database.end()) {
            // found, so print table header
            stringStream << std::endl
                         << "Staff: "   << key.first        << std::endl
                         << "Project: " << key.second       << std::endl
                         << "-----------------------------" << std::endl
                         << "Timestamp\tMeasurement       " << std::endl
                         << "-----------------------------" << std::endl;
            // get data
            auto experimentSharedPtr = dbProjectIterator->second;
            auto measurements = experimentSharedPtr.get()->getMeasurements();
            for (auto iter = measurements.begin(); iter != measurements.end(); ++iter) {
                stringStream << *iter << std::endl;
            }
        } else {
            ErrorMsg::print("\n[PROJECT-DB] Data does not exist!\n");
            return "";
        }
        return stringStream.str();
    }

    // return all data report stingstream 
    std::string getReport() {
        std::ostringstream stringStream;
        for (auto it = this->database.begin(); it != this->database.end(); ++it) {
            auto experimentSharedPtr = it->second;
            stringStream << experimentSharedPtr.get()->getReport();    
        }
        return stringStream.str();    
    }

    // return report stingstream mathing particular key
    std::string getReport(const std::string& staffName, const std::string& projectName) {
        std::ostringstream stringStream;
        auto key = std::make_pair(staffName, projectName);
        auto dbProjectIterator = database.find(key);
        // check if exists
        if (dbProjectIterator != database.end()) {
            auto experimentSharedPtr = dbProjectIterator->second;
            stringStream << experimentSharedPtr.get()->getReport();  
        } else {
            ErrorMsg::print("\n[PROJECT-DB] Data does not exist!\n");
        }
        return stringStream.str();
    }
};

/* ------------------------------------------------------------------------
* DEFINE A DATA MAP CLASS
* -----------------------------------------------------------------------*/

template <typename T> class DataManager {
private:
	ProjectDb<T> fullDatabase;
    ProjectReferenceDb<T> staffDatabase{"Staff", "Project"};
    ProjectReferenceDb<T> projectDatabase{"Project", "Staff"};

public:
	// default constructor
	DataManager() {
		DebugMsg::print("[DATA-MANAGER] Default constructor called\n");       
	}

    // copy constructor for deep copying
    DataManager(const DataManager& userDataManager) {
        DebugMsg::print("[DATA-MANAGER] Copy constructor for deep copying called\n");
        this->fullDatabase = userDataManager.fullDatabase;
        this->staffDatabase = userDataManager.staffDatabase;
        this->projectDatabase = userDataManager.projectDatabase;
    }

    // move constructor
    DataManager(DataManager&& userDataManager) {
        DebugMsg::print("[DATA-MANAGER] Move constructor called\n");
        // steal the data
        this->fullDatabase = move(userDataManager.fullDatabase);
        this->staffDatabase = move(userDataManager.staffDatabase);
        this->projectDatabase = move(userDataManager.projectDatabase);
    }

	// default destructor
	~DataManager() { 
		DebugMsg::print("[DATA-MANAGER] Default destructor called\n");
	}

	// access functions
    DataManager<T> getFullDatabase() const { 
        return this->fullDatabase.getDatabase(); 
    }
    ProjectReferenceDb<T> getStaffDatabase() const { 
        return this->staffDatabase.getDatabase(); 
    }
    ProjectReferenceDb<T> getProjectDatabase() const { 
        return this->projectDatabase.getDatabase(); 
    }

    // copy assignment operator
    DataManager& operator=(const DataManager& userDataManager) {
        DebugMsg::print("[DATA-MANAGER] Copy assignment operator called\n");
        if (&userDataManager == this) { return *this; } // no self-assignment
        // first delete this object’s data
        this->fullDatabase.clear();
        this->staffDatabase.clear();
        this->projectDatabase.clear();
        // declare new object
        this->fullDatabase = userDataManager.fullDatabase;
        this->staffDatabase = userDataManager.staffDatabase;
        this->projectDatabase = userDataManager.projectDatabase;
        return *this;
    }

    // move assignment operator
    DataManager& operator=(DataManager&& userDatabase) {
        DebugMsg::print("[DATA-MANAGER] Move assignment operator called\n");
        std::swap(this->fullDatabase, userDatabase.dfullDatabase);  
        std::swap(this->staffDatabase, userDatabase.staffDatabase); 
        std::swap(this->projectDatabase, userDatabase.projectDatabase); 
        return *this;
    }

    // printing functions
    std::string fullDatabaseShow() { 
        return this->fullDatabase.show(); 
    }
    std::string fullDatabaseShow(const std::string& staffName, 
                                 const std::string& projectName) { 
        return this->fullDatabase.show(staffName, projectName); 
    }
    std::string staffDatabaseShow() { 
        return this->staffDatabase.show(); 
    }
    std::string staffDatabaseShow(const std::string& staffName) { 
        return this->staffDatabase.show(staffName); 
    }
    std::string projectDatabaseShow() { 
        return this->projectDatabase.show(); 
    }
    std::string projectDatabaseShow(const std::string& projectName) { 
        return this->projectDatabase.show(projectName); 
    }
    std::string getReport() { 
        return this->fullDatabase.getReport(); 
    }
    std::string getReport(const std::string& staffName, 
                          const std::string& projectName) { 
        return this->fullDatabase.getReport(staffName, projectName); 
    }
    std::string getStaffReport(const std::string& staffName) { 
        return this->staffDatabase.getReport(staffName); 
    }
    std::string getProjectReport(const std::string& projectName) { 
        return this->projectDatabase.getReport(projectName); 
    }

    // delete project from the map
    bool deleteEntry(const std::string& staff, const std::string& project) { 
        return this->fullDatabase.deleteEntry(staff, project); 
    }

    // delete measurements from the map
    bool deleteMeasurementRange(const std::string& staff, const std::string& project, 
                                const unsigned& startRange, const unsigned& endRange) {
        return this->fullDatabase.deleteMeasurementRange(staff, project, startRange, endRange);
    }

    // insert experiment
	void insertExperiment(const Experiment<T>& userExperiment) {
        // make shared pointers to experiments
        auto experimentPointer = std::make_shared<Experiment<T>>(userExperiment);
        // extract staff name
        std::string staffName = userExperiment.getStaffName();
        // extract project name
        std::string projectName = userExperiment.getProjectName();
        // add entry to full database, get weak pointer to updated project
		auto updatedProject = fullDatabase.addEntry(staffName, projectName, experimentPointer);
        // create weak pointer to project
        ProjectWeakPtr<T> weakProject = updatedProject;
        // add updated entry to staff database and project database
        staffDatabase.addEntry(staffName, projectName, weakProject);
		projectDatabase.addEntry(projectName, staffName, weakProject);
	}
};

#endif /* MAPS_HPP */