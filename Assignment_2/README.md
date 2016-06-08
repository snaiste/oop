### Description
This program calculates the mean, standard deviation and standard error of the mean of data saved in .dat file, which is saved within the same directory as the program.

### Method
There are two files of synthetic data (millikan.dat and millikan2.dat) for the charge of an electron (Millikan’s experiment). The first file contains 50 floating point numbers. The program, however does not ask user for a number of entries in the file and it rather dynamically resizes the data array. The second file is identical but also contains a rogue word. This program reads in these data and compute the mean, standard deviation and standard error of the mean for the electronic charge in Coulomb. This program, however, be used for any other data, as long as it is saved in .dat file and data are floating point numbers.

### Input
* File named as <filename.dat>, containing floating point numbers
* Data file should be saved in the same directory as 'millikan.cpp'

### Return
* Mean, standard deviation and error on the mean in Coulombs
* Number of floating point numbers read and total entries in the file

### Warnings and Errors
* Warning: data file contains values that are not floating point numbers
* Error: could not open specified file