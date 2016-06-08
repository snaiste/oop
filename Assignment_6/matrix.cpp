/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++ (PHYS30762)
*   School of Physics and Astronomy, The University of Manchester
*   Date: 9 March 2016
*   Author: Aiste Norkute
*
*   matrix.cpp
*
*   DESCRIPTION: A class for matrices
*
*   METHOD:
*      - Private class data contains: the number of rows, the number of columns,
*        dimension of the matrix and a pointer for the matrix data of double type
*      - When constructing a matrix object, dynamic memory allocation is used
*        to store the matrix data in a 1D array
*      - For an m x n matrix A, the location of element Aij is
*        loc = (j - 1) + (i - 1) * n, where the (i - 1) and (j - 1) terms are
*        due to fact that array starts at zero
*      - Destructor deletes any dynamic memory when called
*      - Assignment operator and copy constructor functions perform deep copies
*        of data
*
*   STRUCTURE:
*      - Class has the following member functions:
*        -- Patametrized constructor (utilizing new)
*        -- Desctructor (utilizing delete)
*      - Member function to overload assignment operator that performs deep
*        copy of data. It also handles self-assignment.
*      - Both copy constructor and copy assignment perform deep copy of data
*      - Both move constructor and move assignment are efficiently coded
*      - There is a friend function to overload << producing nicely formatter
*        output for a matrix object
*      - There is a friend function to overload >> to input a matrix in a
*        sensible form
*      - Functions overloading + and - for matrix addition and subtraction
*        (functions check both matrices have same dimensions)
*      - Function overloading * for matrix multiplication (checks if operation
*        is possible)
*      - Function calculating the minor of a matrix (a minor is the determinant
*        of a matrix with the i th row and j th column deleted)
*      - Function to calculate recursively the determinant of a square matrix
*        (using the expansion in determinants of minors)
*      - Deep copying by changing the original matrix and showing the new matrix
*        unchanged is demonstrated for copy and assignment items
*
*   INPUT:
*      - User input of matrix dimensions and values
*
*   RETURN:
*      - Various matrix manipulations, including addition, subtraction and
*        multiplication
*      - Determinant of user matrix
*
*   ERRORS:
*      - Non-physical input error, e.g. user enters negative number for matrix 
*        number of rows or columns
*      - Program does not accept number of rows or columns equal to 0
*      - Out of range error: function which returns current indices checks if
*        indices are not out of range
*      - Invalid input warning: program expects integer for the number of rows 
*        and columns of the matrix; if input is not integer, program asks user to
*        try again
*      - Ignoring bad input warning: when entering matrix values, program ignores
*        bad input, e.g. letters
*      - Matrices size error: addition and subtraction of two matrices cannot be 
*        performed if the two matrices are not of the same dimension
*      - Matrices multiplication error: multiplication cannot be performed if
*        the number of rows of one matrix is not equal to the number of columns
*        of the second matrix
*      - Minor error: minor of 1 x 1 matrix cannot be found
*      - Determinant error: determinant of a non-square matrix cannot be found
*
* ----------------------------------------------------------------------*/

#include <iostream>  // cout, endl
#include <cstdlib>   // EXIT_FAILURE, EXIT_SUCCESS
#include <exception> // exception to catch and end C++ code nicely
#include <regex>     // regular expressions
#include <cmath>     // pow
#include <limits>    // numeric_limits<streamsize>::max()

using namespace std;

/* ------------------------------------------------------------------------
* ERROR HANDLING
* ----------------------------------------------------------------------*/

// decleare warning messages
const string INVALID_INPUT_WARNING{ "Input is invalid! " };
const string TRY_AGAIN{ "Please try again: " };
const string INPUT_FORMAT_WARNING{ "WARNING: Ignoring bad input values" };
const string MATRICES_SIZE_ERROR{ "ERROR: Cannot perform this operation for different size matrices!" };
const string MATRICES_MULTIPLICATION_ERROR{ "ERROR: Cannot multiply - column no. of first matrix != row no. of second." };
const string MINOR_ERROR{ "ERROR: Cannot find minor of 1 x 1 matrix!" };
const string DETERMINANT_ERROR{ "Cannot find determinant of a non-square matrix!" };

// classes defining exception returns
class BadInputEx : public exception {
	virtual const char* what() const throw() { return "ERROR: Non-physical input!"; }
} badInputEx;
class SthWrongEx : public exception {
	virtual const char* what() const throw() { return "ERROR: Something went wrong..."; }
} sthWrongEx;
class OutOfRangeEx : public exception {
	virtual const char* what() const throw() { return "ERROR: Out of range!"; }
} outOfRangeEx;

/* ------------------------------------------------------------------------
* DEFINE A CLASS
* -----------------------------------------------------------------------*/

class Matrix {
	// allow the overloaded operator<< function to be a friend of Matrix class
	friend ostream & operator<<(ostream &os, const Matrix &matrix);
	// returns a reference to an ostream, because it modifies an onstream
	friend istream & operator>>(istream &is, Matrix &matrix);
private:
	int rows, columns, dimension;
	double *data;
public:
	// default constructor
	Matrix() {
		cout << "Default constructor called" << endl;
		this->rows = this->columns = this->dimension = 0;
		this->data = NULL; // set pointing to nothing
	}
	// parameterized constructor
	Matrix(int prows, int pcolumns) {
		// cout << "Parameterized constructor called" << endl;
		// catch non-physical input
		if ((prows == 0) || (pcolumns == 0)) throw(badInputEx);
		this->rows = prows;
		this->columns = pcolumns;
		this->dimension = this->rows * this->columns;
		// multidimensional arrays only need one block of memory, 
		// they do not need one block per row. Allocating only one block makes 
		// cleanup simpler too
		this->data = new double[this->dimension]; // initialise 2D matrix
		// make all matrix elements equal to 1
		memset(this->data, 0, sizeof(this->data[this->dimension]) * this->dimension);
	}
	// copy constructor for deep copying
	Matrix(Matrix &matrix) { 
		// cout << "Copy constructor for deep copying called" << endl;
		// copy size and declare new array
		this->data = NULL; // reset pointing
		this->rows = matrix.rows;
		this->columns = matrix.columns;
		this->dimension = matrix.dimension;
		data = new double[this->dimension];
		// copy values into new array
		for (int i{ 0 }; i < this->dimension; ++i) this->data[i] = matrix.data[i];
	}
	// move constructor
	Matrix(Matrix &&matrix) {
		// cout << "Move constructor called" << endl;
		// steal the data
		this->rows = matrix.rows;
		this->columns = matrix.columns;
		this->dimension = matrix.dimension;
		for (int i{ 0 }; i < this->dimension; ++i) this->data[i] = matrix.data[i];
		matrix.rows = matrix.columns = matrix.dimension = 0;
		matrix.data = NULL; // reset pointing
	}
	// destructor
	~Matrix() {
		// cout << "Destructor called" << endl;
		delete[] this->data;
		this->data = NULL; // reset pointing
	}
	// access functions
	int getRows() const { return rows; } // return number of rows
	int getCols() const { return columns; } // return number of columns
	// return position in array of element (rowNo, columnNo)
	int index(int rowNo, int columnNo) const {
		if (rowNo > 0 && rowNo <= rows && columnNo > 0 && columnNo <= columns)
			// The (columnNo ? 1) and (rowNo ? 1 ) terms are due to fact that array starts at zero
			return (columnNo - 1) + (rowNo - 1) * columns;
		else throw(outOfRangeEx); // catch out of range indices
	}
	// other functions
	// return element [m * n]
	double & operator()(int m, int n) { return this->data[index(m, n)]; }
	// assignment operator - similar to copy constructor except that we assume the object is
	// already constructed!
	// we must therefore delete existing data first before copying
	// copy assignment operator
	Matrix & operator=(const Matrix &matrix) {
		// cout << "Copy assignment operator called" << endl;
		if (&matrix == this) return *this; // no self-assignment
		// first delete this object’s array
		delete[] this->data;
		this->rows = this->columns = 0;
		// now copy size and declare new matrix data array
		this->rows = matrix.rows;
		this->columns = matrix.columns;
		this->dimension = matrix.dimension;
		this->data = new double[this->dimension];
		// copy values into new array
		for (int i{ 0 }; i < this->dimension; ++i) this->data[i] = matrix.data[i];
		return *this;
	}
	// move assignment operator
	Matrix & operator=(Matrix &&matrix) {
		// cout << "Move assignment operator called" << endl;
		swap(this->rows, matrix.rows);
		swap(this->columns, matrix.columns);
		swap(this->dimension, matrix.dimension);
		swap(this->data, matrix.data);
		return *this;
	}
	// overload addition operator
	Matrix operator+(const Matrix &matrix) const {
		Matrix tempMatrix{ this->rows, this->columns };
		// can add only matrices of the same size
		if ((this->rows == matrix.rows) && (this->columns == matrix.columns)) {
			for (int i{ 0 }; i < this->dimension; ++i) {
				tempMatrix.data[i] = this->data[i] + matrix.data[i];
			}
		}
		else cerr << MATRICES_SIZE_ERROR << endl;
		return tempMatrix;
	}
	// overload subtraction operator
	Matrix operator-(const Matrix &matrix) const {
		Matrix tempMatrix{ this->rows, this->columns };
		// can subtract only matrices of the same size
		if ((this->rows == matrix.rows) && (this->columns == matrix.columns)) {
			for (int i{ 0 }; i < this->dimension; ++i) {
				tempMatrix.data[i] = this->data[i] - matrix.data[i];
			}
		}
		else cerr << MATRICES_SIZE_ERROR << endl;
		return tempMatrix;
	}
	// overload multiplication operator
	Matrix operator*(const Matrix &matrix) const {
		double tempResult;
		Matrix tempMatrix{ matrix.columns, this->rows };
		// check if multiplication can be performed
		if (this->columns == matrix.rows) {
			for (int i{ 0 }; i < this->rows; ++i) {
				for (int j{ 0 }; j < matrix.columns; ++j) {
					tempResult = 0; // make sure it is zero
					for (int k{ 0 }; k < matrix.rows; ++k) {
						tempResult += this->data[i * this->columns + k] * matrix.data[k * matrix.columns + j];
					}
					tempMatrix.data[i * matrix.columns + j] = tempResult;
				}
			}
		}
		else cerr << MATRICES_MULTIPLICATION_ERROR << endl;
		return tempMatrix;
	}
	// remove row q and column p from given matrix, i. e. get matrix minor
	Matrix getMinor(Matrix &matrix, const int q, const int p) const {
		Matrix tempMatrix{ matrix.columns - 1, matrix.rows - 1 };
		int columnCount{ 0 }; // column counters for later
		if ((matrix.rows > 1) && (matrix.columns > 1)) {
			for (int i{ 1 }; i < matrix.rows + 1; ++i) {
				if (i != q + 1) {
					for (int j{ 1 }; j < matrix.columns + 1; ++j) {
						if (j != p + 1) { // ignore ith row and jth column
							// take old matrix minus the i th row, j th column
							tempMatrix.data[columnCount] = matrix(i, j);
							columnCount++;
						}
					}
				}
			}
		}
		// cannot find minor if matrix is 1 x 1
		else cerr << MINOR_ERROR << endl;
		return tempMatrix;
	}
	// get determinant
	// determinant expansion by minors: 
	// http://mathworld.wolfram.com/DeterminantExpansionbyMinors.html
	double getDeterminant(const int dimension) {
		double det{ 0 };
		if (this->rows == this->columns) {
			if (dimension == 1) { // 1 x 1 matrix
				det += this->data[0];
			}
			else {
				Matrix tempMatrix{ this->columns - 1, this->rows - 1 };
				for (int i{ 0 }; i < dimension; ++i) {
					tempMatrix = this->getMinor(*this, 0, i);
					// NB: 0 in minor function for  minor to remove 0 + 1, i.e. 1st row
					det += pow(-1, i) * this->data[i] * tempMatrix.getDeterminant(dimension - 1);
				}
			}
		}
		else cerr << DETERMINANT_ERROR << endl; // it should be a square matrix
		return det;
	}
};

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF CLASS TO OVERLOAD OSTREAM OPERATOR<<
* -----------------------------------------------------------------------*/

// overload insertion to output stream for matrices
ostream & operator<<(ostream &os, const Matrix &matrix) {
	if (matrix.dimension > 1) { // matrix is bigger than 1 x 1
		for (int i{ 0 }; i < matrix.rows; ++i) { // loop over rows
			cout << "( ";
			for (int j{ 0 }; j < matrix.columns; ++j) { // loop over columns
				cout << matrix.data[i * matrix.columns + j] << " ";
			}
			cout << ")" << endl;
		}
	}
	else if (matrix.dimension == 1) { // matrix contains just 1 number
		cout << "( " << matrix.data[0] << " )" << endl;
	}
	else throw(sthWrongEx);
	return os;
}

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF CLASS TO OVERLOAD OSTREAM OPERATOR>>
* -----------------------------------------------------------------------*/

istream & operator>>(istream &is, Matrix &matrix) {
	for (int i{ 0 }; i < matrix.dimension; ++i) {
		is >> matrix.data[i];
		if (is.fail()) { // check if values were acceptable
			cout << INPUT_FORMAT_WARNING << endl;
			is.clear(); // clear bad input
			is.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore bad input
			is >> matrix.data[i];
		}
	}
	cin.clear(); // clear rubbish!
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore rubbish!
	return is;
}

/* ------------------------------------------------------------------------
* GET AND CHECK INTEGER INTPUT
* ----------------------------------------------------------------------*/

int getInputValues() {
	regex reg{ "\\s*(\\d+)" }; // integer
	string input{ "" }; // initialise empty string
	getline(cin, input);
	bool flag{ regex_match(input, reg) };
	while (!flag) { // check if value matches reg
		cout << INVALID_INPUT_WARNING << TRY_AGAIN;
		getline(cin, input);
		flag = regex_match(input, reg);
	}
	return atoi(input.c_str());
}

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* -----------------------------------------------------------------------*/

int main() {
	try {
		// example using default constructor  
		Matrix defaultMatrix; // create new default matrix object

		// example using parametrised constructor 
		int rowsNo{ 0 }, columnsNo{ 0 };
		cout << endl << "Input your matrix!" << endl;
		cout << "Enter number of rows: ";
		rowsNo = getInputValues();
		cout << "Enter number of columns: ";
		columnsNo = getInputValues();
		Matrix matrix1{ rowsNo, columnsNo };
		cout << "Input matrix row by row (either on one line or hit ENTER after each row):" << endl;
		cout << "(NB: this is " << rowsNo << " x " << columnsNo << " matrix)" << endl;
		cin >> matrix1;
		cout << endl;
		cout << "Your matrix received: " << endl;
		cout << matrix1 << endl; // print input matrix data

		cout << endl;

		// deep copy by assignment: define new matrix matrix2 then copy from matrix1 to matrix2
		Matrix matrix2{ rowsNo, columnsNo };
		cout << "I declared my matrix of zeros: " << endl;
		cout << matrix2 << endl;
		matrix2 = matrix1; // set matrix2 to equal matrix1
		cout << "I made my new matrix to be equal to your matrix: " << endl;
		cout << matrix2 << endl;
		// modify contents of original matrix and show assigned matrix is unchanged here
		cout << "Change values of your (" << rowsNo << " x " << columnsNo << ") matrix: " << endl;
		cin >> matrix1; // change the values of matrix1
		cout << endl;
		cout << "Your new matrix: " << endl;
		cout << matrix1 << endl;
		cout << "My matrix is still: " << endl;
		cout << matrix2 << endl;

		cout << endl;

		// deep copy using copy constructor
		Matrix matrix3{ matrix1 }; // initialise matrix3 as matrix1
		cout << "I declared another matrix as your matrix: " << endl;
		cout << matrix3 << endl;
		// modify contents of original matrix and show copied matrix is unchanged
		cout << "Change values of your (" << rowsNo << " x " << columnsNo << ") matrix: " << endl;
		cin >> matrix1; // change the values of matrix1
		cout << endl;
		cout << "Your new matrix: " << endl;
		cout << matrix1 << endl;
		cout << "My matrix is still: " << endl;
		cout << matrix3 << endl;

		cout << endl;

		// move copy construction and assignment demonstration
		Matrix matrix4{ rowsNo, columnsNo };
		cout << "I declared another matrix of zeros: " << endl;
		cout << matrix4 << endl;
		cout << "Your matrix: " << endl;
		cout << matrix1 << endl;
		matrix4 = move(matrix1); // move matrix1 to matrix5
		cout << "I moved your matrix contents to my matrix: " << endl;
		cout << matrix4 << endl;

		cout << endl;

		// addition, subtraction and multiplicaction of two matrices
		cout << "The matrices to be added, subtracted and multiplied: " << endl;
		cout << matrix3 << endl;
		cout << matrix4 << endl;
		cout << "Addition result is: " << endl;
		cout << matrix3 + matrix4 << endl;
		cout << "Subtraction result is: " << endl;
		cout << matrix3 - matrix4 << endl;
		cout << "Multiplication result is: " << endl;
		cout << matrix3 * matrix4 << endl;

		cout << endl;

		// determinant
		int rowsNoForDet{ 0 }, columnsNoForDet{ 0 };
		cout << "Now input your matrix for which you want to compute determinant!" << endl;
		cout << "Enter number of rows: ";
		rowsNoForDet = getInputValues();
		cout << "Enter number of columns: ";
		columnsNoForDet = getInputValues();
		Matrix matrix5{ rowsNoForDet, columnsNoForDet };
		cout << "Input matrix row by row (either on one line or hit ENTER after each row):" << endl;
		cout << "(NB: this is " << rowsNoForDet << " x " << columnsNoForDet << " matrix)" << endl;
		cin >> matrix5;
		cout << endl;
		cout << "Your matrix received: " << endl;
		cout << matrix5 << endl; // print input matrix data
		// remove first row and first column of the matrix
		// Matrix matrix6 = matrix5.getMinor(matrix5, 2, 2);
		// cout << matrix6 << endl;
		double determinant = matrix5.getDeterminant(rowsNoForDet);
		cout << "The determinant is: " << determinant << endl;
	}
	catch (exception& e) {
		cerr << endl << e.what() << endl;
		cout << endl << "Bye!" << endl << endl;
		return EXIT_FAILURE;
	}
	cout << endl << "Bye!" << endl << endl;
	return EXIT_SUCCESS;
}