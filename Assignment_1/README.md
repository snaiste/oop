### Description
This program calculates transition energy using simple Bohr formula.

### Method
This program calculates the energy for a transition. It asks the user to enter the atomic number, initial and final quantum numbers, and then asks whether to print out the energy of the transition in J or eV. The code asks whether to repeat (“y/n”), and stops if the answer is “n”. Code checks for incorrect inputs at each stage (either in format, or violating some physical conditions).

### Input
* 3 integers: atomic number, initial and final quantum numbers
* String: J or eV (case insensitive) for units
* String: y or n (case insensitive) to repeat or stop the program

### Return
* Energy in J, eV or both

### Errors
* Input values set is unphysical:
	- Atomic number, initial and final quantum numbers are <= 0 NB: ground state is 1
	- Initial quantum number is > final quantum number
* Input values should be formatted correctly, i.e. if program is expecting integer, then it checks if it is integer
* Program accepts only "Y" or "N" (case insensitive) as user reply to repeat or stop the program