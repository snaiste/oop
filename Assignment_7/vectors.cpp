/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++
*   Date: 13 March 2016
*   Author: Aiste Norkute
*
* ----------------------------------------------------------------------*/

#include <iostream>  // cout, endl
#include <vector>    // vector
#include <cmath>     // pow
#include <string>    // string
#include <limits>    // numeric_limits<streamsize>::max()
#include <cstdlib>   // EXIT_FAILURE, EXIT_SUCCESS
#include <stdlib.h>  // rand()
#include <exception> // exception to catch and end C++ code nicely
#include <ctime>     // time

using namespace std;

// constants
const double c{ 299792458 }; // speed of light

// define object ID for debugging
int objectIdSpace = 0;

/* ------------------------------------------------------------------------
* ERROR HANDLING
* -----------------------------------------------------------------------*/

// declaration of warning messages
const string INVALID_INPUT_WARNING{ "Input is invalid! " };
const string TRY_AGAIN{ "Please try again: " };
const string INPUT_FORMAT_WARNING{ "WARNING: Ignoring bad input values" };
const string VECTORS_LENGTH_ERROR{ "ERROR: Cannot perform this operation for different length vectors!" };
const string NO_DATA_ERROR{ "ERROR: There is no data to print!" };
const string VECTOR_SIZE_ERROR{ "ERROR: Cannot perform this operation for different length vectors!" };

/* ------------------------------------------------------------------------
* BASE CLASS
* -----------------------------------------------------------------------*/

// A general vector class, that implements a Cartesian vector of length n
class Vector {

	// allow the overloaded operator<< function to be a friend of 3-vector class
	// returns a reference to an ostream, because it modifies an onstream
	friend ostream &operator<<(ostream &os, const Vector &vector);

	// access specifier "protected" allows base class members to be accessed in the 
	// derived class, but not outside the class hierarchy
protected:
	vector<double> coordinates;
	int dimension;

public:
	int objectId;

	// default constructor
	Vector() {
		objectId = objectIdSpace++;
		cout << "[VECTOR] Default constructor called " << "(Object ID = " << objectId << ")" << endl;
		this->coordinates = {}; // empty vector
		this->dimension = 0; // zero length
	}

	// parametrised constructor
	Vector(vector<double> pcoordinates, int dimension) {
		objectId = objectIdSpace++;
		cout << "[VECTOR] Parameterized constructor called " << "(Object ID = " << objectId << ")" << endl;
		this->dimension = dimension;
		this->coordinates = vector<double>(dimension);
		int index{ 0 };
		for (vector<double>::iterator i = pcoordinates.begin(); i < pcoordinates.end(); ++i, ++index) {
			this->coordinates[index] = *i;
		}
	}

	// copy constructor for deep copying
	Vector(const Vector &userVector) {
		objectId = objectIdSpace++;
		cout << "[VECTOR] Copy constructor for deep copying called " << "(Object ID = " << objectId << ")"
			<< ", copying from " << "object ID = " << userVector.objectId << endl;
		this->dimension = userVector.dimension;
		this->coordinates = vector<double>(dimension);
		for (int i{ 0 }; i < userVector.dimension; ++i) {
			this->coordinates[i] = userVector.coordinates[i];
		}
	}

	// move constructor
	Vector(Vector &&userVector) {
		objectId = objectIdSpace++;
		cout << "[VECTOR] Move constructor called " << "(Object ID = " << objectId << ")" << endl;
		// steal the data
		this->dimension = userVector.dimension;
		this->coordinates = vector<double>(dimension);
		for (int i{ 0 }; i < userVector.dimension; ++i) {
			this->coordinates[i] = userVector.coordinates[i];
		}
		// delete object's data
		userVector.coordinates = {}; // empty vector
		userVector.dimension = 0; // zero length
	}

	// default destructor
	~Vector() {
		cout << "[VECTOR] Destructor called " << "(Object ID = " << objectId << ")" << endl;
	}

	// access functions
	int getDimension() const {
		// return vector's dimension
		return this->dimension;
	}

	// overloaded element [] operator
	double & operator[] (int n) {
		cout << "[VECTOR] Operator[] called " << "(Object ID = " << objectId << ")" << endl;
		// check for addressing outside the length of the vector
		if (n > this->dimension) {
			throw out_of_range("Vector index bigger than dimension"); // catch out of range indices
		}
		return this->coordinates[n];
	}

	// assignment operator - similar to copy constructor except that we assume the object is
	// already constructed!
	// we must therefore delete existing data first before copying
	// copy assignment operator
	Vector & operator=(const Vector &userVector) {
		cout << "[VECTOR] Copy assignment operator called " << "(Object ID = " << objectId << ")" << endl;
		if (&userVector == this) { return *this; } // no self-assignment
		// first delete this object’s array
		this->coordinates = {}; // empty vector
		this->dimension = 0; // zero length
		// declare new object
		this->dimension = userVector.dimension;
		this->coordinates = vector<double>(dimension);
		for (int i{ 0 }; i < userVector.dimension; ++i) {
			this->coordinates[i] = userVector.coordinates[i];
		}
		return *this;
	}

	// move assignment operator
	Vector & operator=(Vector &&userVector) {
		cout << "[VECTOR] Move assignment operator called " << "(Object ID = " << objectId << ")" << endl;
		this->dimension = userVector.dimension;
		this->coordinates = userVector.coordinates;
		userVector.coordinates = {}; // empty vector
		userVector.dimension = 0; // zero length
		return *this;
	}

	// overload multiplication operator for two vectors
	double operator*(const Vector &userVector) const {
		cout << "[VECTOR] Multiplication operator called " << "(Object ID = " << objectId << ")"
			<< " and " << "(Object ID = " << userVector.objectId << ")" << endl;
		double dotProduct{ 0 };
		if (this->dimension == userVector.dimension) {
			// compute dot product
			for (int i{ 0 }; i < userVector.dimension; ++i) {
				dotProduct += this->coordinates[i] * userVector.coordinates[i];
			}
		}
		else {
			cerr << VECTORS_LENGTH_ERROR << endl;
			throw runtime_error("Vectors have different lengths");
		}
		return dotProduct;
	}
};

/* ------------------------------------------------------------------------
* INHERITED MINKOWSKI VECTOR CLASS FROM VECTOR BASE CLASS
* -----------------------------------------------------------------------*/

class MinkowskiVector : public Vector {
public:
	// default constructor - calling base class constructor
	MinkowskiVector() : Vector(vector<double>(4), 4) {
		objectId = objectIdSpace++;
		cout << "[MINKOWSKI] Default constructor called " << "(Object ID = " << objectId << ")" << endl;
	}

	// parametrized constructor taking four doubles
	MinkowskiVector(double x, double y, double z, double ct) : Vector(vector<double>(4), 4) {
		objectId = objectIdSpace++;
		cout << "[MINKOWSKI] Parameterized constructor called " << "(Object ID = " << objectId << ")" << endl;
		this->dimension = 4;
		this->coordinates[0] = x;
		this->coordinates[1] = y;
		this->coordinates[2] = z;
		this->coordinates[3] = ct;
	}

	// parametrized constructor taking one double and one length 3-vector
	MinkowskiVector(vector<double> pcoordinates, double ct) : Vector(vector<double>(4), 4) {
		if (pcoordinates.size() != 3) {
			throw runtime_error("Minkowski Vector must have 4 dimensions");
		}
		objectId = objectIdSpace++;
		cout << "[MINKOWSKI] Parameterized constructor called " << "(Object ID = " << objectId << ")" << endl;
		this->dimension = 4;
		this->coordinates[0] = pcoordinates[0];
		this->coordinates[1] = pcoordinates[1];
		this->coordinates[2] = pcoordinates[2];
		this->coordinates[3] = ct;
	}

	// copy constructor for deep copying
	MinkowskiVector(const MinkowskiVector &userVector) : Vector(userVector) {
		cout << "[MINKOWSKI] Copy constructor called " << "(Object ID = " << objectId << ")" << endl;
	}

	// move constructor
	MinkowskiVector(MinkowskiVector &&userVector) : Vector(userVector) {
		cout << "[MINKOWSKI] Move constructor called " << "(Object ID = " << objectId << ")" << endl;
	}

	// default destructor
	~MinkowskiVector() {
		cout << "[MINKOWSKI] Destructor called " << "(Object ID = " << objectId << ")" << endl;
	}

	MinkowskiVector & operator=(const MinkowskiVector &userVector) {
		Vector::operator=(userVector);
		return *this;
	}

	MinkowskiVector & operator=(MinkowskiVector &&userVector) {
		Vector::operator=(userVector);
		return *this;
	}

	double x() {
		return Vector::operator[](0);
	}

	// override multiplication operator for two Minkowski vectors
	double operator*(const MinkowskiVector &userVector) const {
		cout << "[MINKOWSKI] Multiplication operator called " << "(Object ID = " << objectId << ")"
			<< " and " << "(Object ID = " << userVector.objectId << ")" << endl;
		double dotProduct{ this->coordinates[3] * userVector.coordinates[3] };
		vector<double> r1(this->coordinates.begin(), this->coordinates.end() - 1);
		vector<double> r2(userVector.coordinates.begin(), userVector.coordinates.end() - 1);
		Vector R1{ r1, 3 };
		Vector R2{ r2, 3 };
		dotProduct -= R1 * R2;
		return dotProduct;
	}

	// function returning a Lorentz-boosted 4-vector
	MinkowskiVector lorentzBoost(Vector &beta) {
		cout << "[MINKOWSKI] Lorentz Boost function called " << "(Object ID = " << objectId << ")" << endl;
		if (beta.getDimension() != 3) {
			throw runtime_error("Boost Vector must be a 3-vector");
		}
		double betaSquared = beta * beta;
		double gamma = 1 / sqrt(1 - betaSquared);

		vector<double> r(this->coordinates.begin(), this->coordinates.end() - 1);
		Vector R{ r, 3 };
		vector<double> R2(3);
		double ct = this->coordinates[3];

		// perform boost
		double ct2 = gamma * (ct - beta * R);
		double multiplier = (gamma - 1) * (R * beta / betaSquared) - gamma * ct;
		R2[0] = R[0] + multiplier * beta[0];
		R2[1] = R[1] + multiplier * beta[1];
		R2[2] = R[2] + multiplier * beta[2];

		return{ R2[0], R2[1], R2[2], ct2 };
	}
};

/* ------------------------------------------------------------------------
* INHERITED PARTICLE CLASS FROM VECTOR BASE CLASS
* -----------------------------------------------------------------------*/

class Particle {
protected:
	// position object for current position
	MinkowskiVector position; // initialised as 4-vector
	double mass;
	Vector beta; // initialised as 3-vector
	int objectId;
public:
	// default constructor - calling base class constructor
	Particle() : position{ 0., 0., 0., 0. }, mass{ 0 }, beta{ vector<double>(3), 3 } {
		objectId = objectIdSpace++;
		cout << "[PARTICLE] Default constructor called " << "(Object ID = " << objectId << ")" << endl;
	}

	// parametrized constructor
	Particle(MinkowskiVector &pposition, double pmass, Vector &pbeta) {
		if ((pposition.getDimension() != 4)) {
			throw runtime_error("Position vector should be a 4-vector!");
		}
		if ((pbeta.getDimension() != 3)) {
			throw runtime_error("Beta vector should be a 3-vector!");
		}
		objectId = objectIdSpace++;
		cout << "[PARTICLE] Parametrized constructor called " << "(Object ID = " << objectId << ")" << endl;
		this->position = pposition;
		this->mass = pmass;
		this->beta = pbeta;
	}

	// default destructor
	~Particle() {
		cout << "[PARTICLE] Destructor called " << "(Object ID = " << objectId << ")" << endl;
	}

	// access functions
	MinkowskiVector getPosition() const { return this->position; } // return particle position
	double getMass() const { return this->mass; } // return mass
	Vector getBeta() const { return this->beta; } // return beta

	// function to calculate gamma
	double gamma() { return 1 / sqrt(1 - this->beta * this->beta); }

	// function returning the total energy of the particle
	double energy() { return this->gamma() * this->mass; }

	// function returning the momentum of the particle
	Vector momentum() {
		// http://www.colorado.edu/physics/phys2170/phys2170_sp07/downloads/lorentz_transformation_E_p.pdf
		// https://en.wikipedia.org/wiki/Four-momentum
		vector<double> P(4);
		P[0] = this->energy();
		P[1] = this->gamma() * this->mass * this->beta[0];
		P[2] = this->gamma() * this->mass * this->beta[1];
		P[3] = this->gamma() * this->mass * this->beta[2];
		return{ P, 4 }; // return 4-vector
	}
};

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF CLASS TO OVERLOAD OSTREAM OPERATOR<<
* -----------------------------------------------------------------------*/

// overload insertion to output stream for matrices
ostream & operator<<(ostream &os, const Vector &userVector) {
	if (!userVector.coordinates.empty()) { // check if data vector is not empty
		os << "Vector" << userVector.objectId << " = " << "(";
		for (int i{ 0 }; i < userVector.dimension; ++i) {
			os << userVector.coordinates[i] << (i < userVector.dimension - 1 ? ", " : "");
		}
		os << ")";
	}
	else { cerr << NO_DATA_ERROR; }
	return os;
}

/* ------------------------------------------------------------------------
* GENERATE RANDOM VECTOR
* ----------------------------------------------------------------------*/

vector<double> getUserVector(int dimension) {
	vector<double> userVector;
	for (int i{ 0 }; i < dimension; ++i) {
		// generate random values
		userVector.push_back(rand() % 100); // push object and all its data             
	}
	return userVector;
}

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* -----------------------------------------------------------------------*/

int main() {
	// initialize random seed
	srand(time(NULL));

	cout << endl;

	// example using default constructor  
	Vector defaultVector; // create new default vector object

	cout << endl;

	// example using parametrised constructor
	cout << "Enter dimension of a vector with random values: ";
	int dimension;
	cin >> dimension;
	while (cin.fail()) { // check if value was read
		cout << INVALID_INPUT_WARNING << TRY_AGAIN;
		cin.clear(); // clear bad input
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear bad input
		cin >> dimension;
	}
	vector<double> userVector = getUserVector(dimension); // get a random vector
	Vector vector1{ userVector, dimension };
	cout << "Your vector constructed:" << endl;
	cout << vector1 << endl;

	cout << endl;

	// deep copy construction demonstration
	Vector vector2{ vector1 }; // initialise vector2 as vector1
	cout << "Your vector was copied to a new vector using copy constructor:" << endl;
	cout << vector2 << endl;

	cout << endl;

	// deep copy assignment operator demonstration
	Vector vector3;
	vector3 = vector1;
	cout << "It was also copied to another new vector using copy assignment operator:" << endl;
	cout << vector3 << endl;

	cout << endl;

	// move copy construction and assignment demonstration
	Vector vector4;
	vector4 = move(vector1); // move vector1 to vector4
	cout << "I moved your vector contents to a new vector4: " << endl;
	cout << vector4 << endl;
	cout << "Now your vector1 is: " << endl;
	cout << vector1 << endl;

	cout << endl;

	// demonstration of the implementation of operator[] to access protected
	// elements of the class
	cout << "Type which vector4 element you would like to print: ";
	int elementNo{ 0 };
	cin >> elementNo;
	try {
		// subtract 1, since arrays in C++ start counting from 0
		cout << vector4[elementNo - 1] << endl;
	}
	catch (const out_of_range& e) {
		cerr << endl << "Vector index out of range: " << e.what() << endl;
	}

	cout << endl;

	// demonstration of dot product calculation
	// use 3D vectors
	dimension = 3;
	Vector vector5(getUserVector(dimension), dimension);
	Vector vector6(getUserVector(dimension), 3);
	cout << endl;
	cout << "Two 3-vectors constructed:" << endl;
	cout << vector5 << endl;
	cout << vector6 << endl;
	cout << endl;
	try {
		double dotProduct = vector5 * vector6;
		cout << endl;
		cout << "Dot product = " << dotProduct << endl;
	}
	catch (const runtime_error& e) {
		cerr << endl << "ERROR while doing vector dot product: " << e.what() << endl;
	}

	cout << endl;

	// demonstration of Minkowski 4-vector class
	MinkowskiVector vector7;
	MinkowskiVector vector8;
	MinkowskiVector vector9(getUserVector(3), c * 1); // t = 1
	MinkowskiVector vector10(getUserVector(3), c * 2); // t = 2
	try {
		vector7 = move(vector9);
		vector8 = move(vector10);
	}
	catch (const runtime_error& e) {
		cerr << endl << "ERROR while constructing vectors: " << e.what() << endl;
	}
	cout << endl;
	cout << "Two Minkowski 4-vectors (x, y, z, ct) constructed:" << endl;
	cout << vector7 << endl;
	cout << vector8 << endl;

	cout << endl;

	double dotProduct = vector7 * vector8;
	cout << endl;
	cout << "Dot product = " << dotProduct << endl;

	cout << endl;

	// assume moving at 0.7c in x direction
	vector<double> betaVector = { 0.7, 0., 0. };
	Vector vector11{ betaVector, 3 };
	cout << endl;
	cout << "Beta (v/c) = " << vector11 << endl;
	cout << endl;
	MinkowskiVector vector12 = vector8.lorentzBoost(vector11);
	cout << endl;
	cout << "Vector8 after the Lorentz Boost was performed: " << endl;
	cout << vector12 << endl;

	cout << endl;

	// demonstration of 3-vector and 4-vector particle class
	double mass{ 0.511 };
	Particle userParticle;
	Particle tempParticle(vector12, mass, vector11);
	try {
		userParticle = move(tempParticle);
	}
	catch (const runtime_error& e) {
		cerr << endl << "Error while constructing Particle object: " << e.what() << endl;
	}
	MinkowskiVector position = userParticle.getPosition();
	mass = userParticle.getMass();
	Vector beta = userParticle.getBeta();
	cout << endl;
	cout << "Particle (position 4-vector, mass and beta 3-vector):" << endl;
	cout << position << ", " << mass << ", " << beta << endl;

	cout << endl;

	double energy = userParticle.energy();
	Vector momentum = userParticle.momentum();
	cout << endl;
	cout << "Particle energy = " << energy << " MeV / c^2" << endl;
	cout << "Particle momentum = " << momentum << " MeV / c" << endl;

	cout << endl << "Bye!" << endl << endl;
	return EXIT_SUCCESS;
}


