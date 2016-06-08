### Description
This program stores and prints physics courses. It allows user to enter an arbitrary number of courses or, alternatively, reads courses from specified file. A course code (integer, e.g. 30762) and title (string, e.g. Object-Oriented Programming in C++) should be provided on a single line.

### Method
A string stream is used to create a string containing the full course title, e.g. PHYS 30762 Object-Oriented Programming in C++. Each of these strings are stored in a vector. Printing is performed using an iterator. Program can also print out a list of courses for a particular year, as identified by the first digit of the course code. It is also possible to sort the list of courses by title or course code.

### Input
* File named as 'filename.dat' containing strings like, for example, 30762 Object-Oriented Programming in C++
* Courses can be provided by manual input to screen, typing course code and title on the same line as above

### Return
* Sorted full course list or course for a particular year

### Warnings and Errors
* Warning: ignoring bad input (e.g. no course code is specified)
* Error: there is no data to print (i.e. if data vector is empty)
* Error: cannot find / open specified input file
* Error: cannot recognise input (e.g. asks for 0-4, user enters 5)
* Error: something went wrong (this should not appear in general)