/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++
*   Date: 16 March 2016
*   Author: Aiste Norkute
*
* ----------------------------------------------------------------------*/

#include <iostream>  // cout, endl
#include <vector>    // vector
#include <cmath>     // pow
#include <cstdlib>   // EXIT_FAILURE, EXIT_SUCCESS

using namespace std;

// define constants
const double PI(atan(1) * 4);

/* ------------------------------------------------------------------------
* ABSTRACT BASE CLASS AS AN INTERFACE FOR ALL SHAPES
* -----------------------------------------------------------------------*/

class Shape {
	// shape base class becomes abstract base because it contains pure virtual functions
public:
	int objectId; // keep track of objects
	// pure virtual functions
	virtual ~Shape() { // destructor
		cout << "[SHAPE] Destructor called " << endl;
	}
	virtual void show() = 0; // printing
	virtual double getArea() = 0; // area calculation
	virtual double getVolume() = 0; // volume calculation
};

/* ------------------------------------------------------------------------
* ABSTRACT BASE CLASS FOR 2D SHAPES
* -----------------------------------------------------------------------*/

class Shape2D : public Shape {
public:
	// pure virtual function
	// destructor
	virtual ~Shape2D() {
		cout << "[SHAPE2D] Destructor called " << endl;
	}
	double getVolume() { return 0.; }
	virtual double getPerimeter() = 0;
};

/* ------------------------------------------------------------------------
* ABSTRACT BASE CLASS FOR 3D SHAPES
* -----------------------------------------------------------------------*/

class Shape3D : public Shape {
public:
	// pure virtual functions
	// destructor
	virtual ~Shape3D() {
		cout << "[SHAPE3D] Destructor called " << endl;
	}
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR RECTANGLES
* -----------------------------------------------------------------------*/

class Rectangle : public Shape2D {
protected:
	// sides a and b
	double a, b;
public:
	// default constructor
	Rectangle() { this->a = this->b = 0.; }
	// parametrised constructor
	Rectangle(double pa, double pb) {
		this->a = pa;
		this->b = pb;
	}
	// destructor
	~Rectangle() {}
	// functions
	double getArea() { return this->a * this->b; }
	void show() {
		cout.precision(3);
		cout << "Rectangle:"
			<< "  a = " << this->a
			<< ", b = " << this->b
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
	double getPerimeter() { return 2 * (this->a + this->b); }
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR SQUARES - SPECIAL CASE OF RECTANGLES
* -----------------------------------------------------------------------*/

class Square : public Rectangle {
public:
	// default constructor
	Square() : Rectangle() {}
	// parametrised constructor
	Square(double a) : Rectangle(a, a) {}
	// destructor
	~Square() {}
	void show() {
		cout.precision(3);
		cout << "Square:"
			<< "  a = " << this->a
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR ELLIPSES
* -----------------------------------------------------------------------*/

class Ellipse : public Shape2D {
protected:
	// semi-minor and semi-major axes a and b
	double a, b;
public:
	// default constructor
	Ellipse() { this->a = this->b = 0; }
	// parameterised constructor
	Ellipse(double pa, double pb) {
		this->a = pa;
		this->b = pb;
	}
	// destructor
	~Ellipse() {}
	// functions
	double getArea() { return PI * this->a * this->b; }
	void show() {
		cout.precision(3);
		cout << "Ellipse:"
			<< "  a = " << this->a
			<< "  b = " << this->b
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
	double getPerimeter() { return PI * (3 * (this->a + this->b) - sqrt((3 * this->a + this->b) * (this->a + 3 * this->b))); }
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR CIRCLES - SPECIAL CASE OF ELLIPSES
* -----------------------------------------------------------------------*/

class Circle : public Ellipse {
public:
	// default constructor
	Circle() : Ellipse() {}
	// parameterised constructor
	Circle(double a) : Ellipse(a, a) {};
	// destructor
	~Circle() {}
	// functions
	void show() {
		cout.precision(3);
		cout << "Circle:"
			<< "  a = " << this->a
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR CUBOIDS
* -----------------------------------------------------------------------*/

class Cuboid : public Shape3D {
protected:
	// sides a and b, height h
	double a, b, h;
public:
	// default constructor
	Cuboid() { this->a = this->b = this->h = 0.; }
	// parametrised constructor
	Cuboid(double pa, double pb, double ph) {
		this->a = pa;
		this->b = pb;
		this->h = ph;
	}
	// destructor
	~Cuboid() {}
	// functions
	double getArea() { return 2. * (this->a * this->b + this->a * this->h + this->b * this->h); }
	double getVolume() { return this->a * this->b * h; }
	void show() {
		cout.precision(3);
		cout << "Cuboid:"
			<< "  a = " << this->a
			<< "  b = " << this->b
			<< "  h = " << this->h
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR CUBES - SPECIAL CASE OF CUBOIDS
* -----------------------------------------------------------------------*/

class Cube : public Cuboid {
public:
	// default constructor
	Cube() : Cuboid() {}
	// parametrised constructor
	Cube(double a) : Cuboid(a, a, a) {}
	// destructor
	~Cube() {}
	// functions
	void show() {
		cout.precision(3);
		cout << "Cube:"
			<< "  a = " << this->a
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR ELLIPSOIDS
* -----------------------------------------------------------------------*/

class Ellipsoid : public Shape3D {
protected:
	double a, b, h;
public:
	// default constructor
	Ellipsoid() { this->a = this->b = this->h = 0.; }
	// parameterised constructor
	Ellipsoid(double pa, double pb, double ph) {
		this->a = pa;
		this->b = pb;
		this->h = ph;
	}
	// destructor
	~Ellipsoid() {}
	// functions
	double getArea() {
		// approximation
		double numerator{ (pow(this->a * this->b, 1.6) + pow(this->a * this->h, 1.6) + pow(this->b * this->h, 1.6)) / 3 };
		double power{ 1. / 1.6 };
		return 4. * PI * pow(numerator, power);
	}
	double getVolume() { return 4. * PI * this->a * this->b * this->h / 3.; }
	void show() {
		cout.precision(3);
		cout << "Ellipsoid:"
			<< "  a = " << a
			<< "  b = " << b
			<< "  h = " << h
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR SPHERES - SPECIAL CASE OF ELLIPSOIDS
* -----------------------------------------------------------------------*/

// Create a sphere class
class Sphere : public Ellipsoid {
public:
	// default constructor
	Sphere() : Ellipsoid() {}
	// parametrised constructor
	Sphere(double a) : Ellipsoid(a, a, a) {}
	// destructor
	~Sphere() {}
	// functions
	void show() {
		cout.precision(3);
		cout << "Sphere:"
			<< "  a = " << a
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
};

/* ------------------------------------------------------------------------
* DERIVED CLASS FOR PRISMS
* -----------------------------------------------------------------------*/

// Create a prism class
class Prism : public Shape3D {
	// Declare cross sectional area & height
protected:
	// area from 2D shape, h = depth
	double h;
	Shape2D *baseShape;
public:
	// default constructor
	Prism() {}
	// parametrised constructor
	// use base class pointer to any of the 2D shapes
	Prism(Shape2D *base, double ph) : baseShape{ base }, h{ ph } {}
	// destructor
	~Prism(){}
	// functions
	double getArea() {
		return 2 * baseShape->getArea() + baseShape->getPerimeter() * this->h;
	}
	double getVolume() {
		return baseShape->getArea() * h;
	}
	void show() {
		cout.precision(3);
		cout << "Prism:"
			<< "  h = " << h
			<< ", area = " << getArea()
			<< ", volume = " << getVolume()
			<< endl;
	}
};

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* -----------------------------------------------------------------------*/

int main() {

	cout << endl;
	// define polymorphic arrays as arrays of base class pointers
	// arrays of 4 2D and 4 3D base class pointers
	int shapes = 10;
	Shape **shapeList = new Shape*[shapes];
	shapeList[0] = new Rectangle(2, 4);
	shapeList[1] = new Square(2);
	shapeList[2] = new Ellipse(2, 4);
	shapeList[3] = new Circle(2);
	shapeList[4] = new Cuboid(2, 4, 8);
	shapeList[5] = new Cube(2);
	shapeList[6] = new Ellipsoid(2, 4, 8);
	shapeList[7] = new Sphere(2);
	shapeList[8] = new Prism(new Circle(2), 3);
	shapeList[9] = new Prism(new Rectangle(3, 3), 3);
	for (size_t i{ 0 }; i < shapes; ++i) {
		shapeList[i]->show();
	}

	cout << endl;

	// clean up
	for (size_t i{ 0 }; i < shapes; ++i) {
		delete shapeList[i];
	}
	delete[] shapeList;

	cout << endl << "Bye!" << endl << endl;
	return EXIT_SUCCESS;
}


















