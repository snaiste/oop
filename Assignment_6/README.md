### Description
A class for matrices.

### Method
* Private class data contains: the number of rows, the number of columns, dimension of the matrix and a pointer for the matrix data of double type
* When constructing a matrix object, dynamic memory allocation is used to store the matrix data in a 1D array
* For an m x n matrix A, the location of element Aij is loc = (j - 1) + (i - 1) * n, where the (i - 1) and (j - 1) terms are due to fact that array starts at zero
* Destructor deletes any dynamic memory when called
* Assignment operator and copy constructor functions perform deep copies of data

### Code structure

* Class has the following member functions:
	* Patametrized constructor (utilizing new)
	* Desctructor (utilizing delete)
* Member function to overload assignment operator that performs deep copy of data. It also handles self-assignment.
* Both copy constructor and copy assignment perform deep copy of data
* Both move constructor and move assignment are efficiently coded
* There is a friend function to overload << producing nicely formatter output for a matrix object
* There is a friend function to overload >> to input a matrix in a sensible form
* Functions overloading + and - for matrix addition and subtraction (functions check both matrices have same dimensions)
* Function overloading * for matrix multiplication (checks if operation is possible)
* Function calculating the minor of a matrix (a minor is the determinant of a matrix with the i th row and j th column deleted)
* Function to calculate recursively the determinant of a square matrix (using the expansion in determinants of minors)
* Deep copying by changing the original matrix and showing the new matrix unchanged is demonstrated for copy and assignment items

### Input
* User input of matrix dimensions and values

### Return
* Various matrix manipulations, including addition, subtraction and multiplication
* Determinant of user matrix

### Warnings and Errors
* Non-physical input error, e.g. user enters negative number for matrix number of rows or columns
* Program does not accept number of rows or columns equal to 0
* Out of range error: function which returns current indices checks if indices are not out of range
* Invalid input warning: program expects integer for the number of rows and columns of the matrix; if input is not integer, program asks user to try again
* Ignoring bad input warning: when entering matrix values, program ignores bad input, e.g. letters
* Matrices size error: addition and subtraction of two matrices cannot be performed if the two matrices are not of the same dimension
* Matrices multiplication error: multiplication cannot be performed if the number of rows of one matrix is not equal to the number of columns of the second matrix
* Minor error: minor of 1 x 1 matrix cannot be found
* Determinant error: determinant of a non-square matrix cannot be found