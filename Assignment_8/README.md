### Description
A class hierarchy describing properties of 2D and 3D shapes.

### Method
Class specification:
* An abstract base class used as an interface for all shapes
* An abstract derived class for a) 2D shapes and b) 3D shapes
* Derived classes for at least 4 2D shapes (rectangle,square, ellipse, circle)
* Derived classes for at least 4 3D shapes (cuboid, cube, ellipsoid, sphere)
* Use additional inheritance for specialisation (“is a”)
* Constructors and (virtual) destructors
* Pure virtual functions to return area and volume (can be zero for 2D shapes)

Main function:
* Implement an array or vector of at least 8 base class pointers, each pointing to a different shape
* Output the area (for 2D case) and volume (for 3D) of each shape
* Clean up by deleting objects and array when finished

### Code structure
* Abstract base class for shapes with appropriate pure virtual functions and virtual destructor
* Eight derived classes (4 2D, 4 3D), each overriding pure virtual functions
* Correct use of inheritance for specialization
* Code demonstrates polymorphism using an array (or vector) of base class pointers, each pointing to different shape
* Code correctly prints out area (2D) and area/volume (3D) of each shape
* Code correctly cleans up, deleting objects and array
* Extra prism class (again, a derived class!) whose constructor takes a double (depth of prism) and a base class pointer to any of the 2D shapes. Class must contain a function to return volume of the prism (prism depth × area from 2D shape)