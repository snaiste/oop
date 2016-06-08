### Description
This program is a simple demonstration of a C++ class.

### Method
A galaxy object contains the following (private) data: 
* Hubble Type: string (or enum). Simplified classification in terms of E0-7, S0, Sa, Sb, Sc, SBa, SBb, SBc and Irr are allowed. (i.e. Hubble types: E[0-7], S0, S[a-c], SB[a-c], Irr)
* Redshift: double z in the range [0, 10].
* Total Mass: double Mtot in the range [10^7, 10^12] * Msun.
* A Stellar Mass Fraction: double Fstar = Mstar / Mtot in the range [0, 0.05].

The class also contains several member functions: 
* A default constructor; a parametrised constructor; a destructor; and a member function, defined outside class, to print out an object's data. 
* A member function to change the galaxy's Hubble type.
* A member function to return the stellar mass Mstar = Fstar * Mtot.

Main program uses a vector to store at least 2 different objects and use an iterator to access these objects and print their content.

Challenge: galaxies contain satellites (e.g. Milky Way has large and small Magellanic Clouds). Hence class is modified so each galaxy object can have satellites as follows:
* A private vector object inside class to store satellites (which are galaxy objects) is included.
* A new member function to add a satellite galaxy is added.
* Print function is modified to print the number of satellites and, if non-zero, print their properties as well.

### Input
* Asks user how many objects to generate and generates galaxies with random properties, which fit into the range given in the task

### Return
* An example of an object constructed by default constructor
* Object list constructed with parametrised constructor

### Warnings and Errors
* Error: input values should be formatted correctly, i.e. if program is expecting integer, then it checks if it is integer
* Error: there is no data to print (i.e. if data vector is empty)