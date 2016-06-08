### Description
A class for complex numbers: it stores complex numbers z = x + yi.

### Method
The data for each object should contain two doubles to represent the real and imaginary parts. The class demonstrates usage of operator overloading for arithmetic operations (on complex numbers) and overload the insertion operator to insert complex numbers into an ostream, and an extraction operator from an istream. The latter functions are friends of the class. The main program demonstrates use of the class through declaring and using objects (or reading them from input!); for simplicity, a = 3 + 4i and b = 1 − 2i are used in tests. Function parameters are modified with const wherever appropriate.

### Code structure

* Four member functions, each returning real part, imaginary part, modulus and argument
* Member function to return complex conjugate
* Functions that overload the + and - operators to perform addition and subtraction of 2 complex numbers
* Functions that overload the * and / operators to perform multiplication and division of 2 complex numbers
* Consistent use of const modifier in function declarations for two purposes: 1 - prevent parameter being modified; 2 - prevent member data being modified
* A non-member function to allow insertion of complex objects into an ostream (must be a friend of the class and output should be appropriately formatted to handle complex numbers with negative imaginary parts)
* [Challenge] A non-member function to allow extraction of complex objects from an istream (must be a friend of the class and should be able to handle complex numbers of the form a+ib and a−ib . Assume a and b to be specfied even if zero)

### Input
* Complex numbers can be provided by manual input to screen

### Return
* Table of results obtained from 2 complex numbers and additional arithmetic operations

### Warnings and Errors
* Error: input values should be formatted correctly