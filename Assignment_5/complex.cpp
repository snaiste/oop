/* ------------------------------------------------------------------------
*
*   Object-Oriented Programming in C++
*   Date: 2 March 2016
*   Author: Aiste Norkute
*
* ----------------------------------------------------------------------*/

#include <iostream>  // cout, endl
#include <cmath>     // pow
#include <cstdlib>   // EXIT_FAILURE, EXIT_SUCCESS
#include <string>    // string
#include <exception> // exception to catch and end C++ code nicely
#include <regex>     // regular expressions

using namespace std;

// define error / warning messages
const string INPUT_FORMAT_ERROR{"Input is invalid!"};

/* ------------------------------------------------------------------------
* DEFINE A CLASS
* ----------------------------------------------------------------------*/

class Complex {
    // allow the overloaded operator<< function to be a friend of Complex class
    // private access permission is granted for member data of object parameter
    friend ostream &operator<<(ostream &os, const Complex &z); 
    friend istream &operator>>(istream &input, Complex &z);
    // returns a reference to an onstream, because it modifies an onstream
private:
    double Re, Im; // for real and imaginary parts of complex numbers
public:
    // constructors and destructor
    Complex() {Re = Im = 0;}
    Complex(double r, double i) {Re = r; Im = i;}
    ~Complex() {/*cout << "Destroying complex number" << endl;*/}   
    void showObjectData() const; // print object data to screen
    // const shows that function does not change any object in the class
    double getRe() const {return Re;} // return real component
    double getIm() const {return Im;} // return imaginary component
    double getModulus() const {return sqrt(pow(Re, 2.0) + pow(Im, 2.0));} // return modulus
    double getArgument() const {return atan(Im / Re);} // return argument
    Complex getConjugate() const { // return complex conjugate
        return Complex (Re, -1 * Im);
    } 
    // overload + operator for addition
    Complex operator+(const Complex &z) const {
        return Complex (this->Re + z.Re, this->Im + z.Im);
    }
    // overload - operator for subtraction
    Complex operator-(const Complex &z) const {
        return Complex (this->Re - z.Re, this->Im - z.Im);
    }
    // overload * operator for multiplication, z1 * z2
    Complex operator*(const Complex &z) const {
        return Complex (this->Re * z.Re - this->Im * z.Im, this->Re * z.Im + this->Im * z.Re); 
    }
    // overload / operator for division, z1/z2
    Complex operator/(const Complex &z) const {
        Complex temp; // temporary object of Complex class type
        // could return all at once, but return line would be too long, hence split and store in temp
        temp.Re = (this->Re * z.Re + this->Im * z.Im) / (z.Re * z.Re + z.Im * z.Im);
        temp.Im = (this->Im * z.Re - this->Re * z.Im) / (z.Re * z.Re + z.Im * z.Im);
        return temp; 
    }
};

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF CLASS TO OVERLOAD OSTREAM OPERATOR<<
* ----------------------------------------------------------------------*/

ostream &operator<<(ostream &os, const Complex &z) {
    string sign{"+"}; // declare positive sign for imaginary part by default
    if (z.Im < 0) sign = "";
    os << z.Re << sign << z.Im << "i";
    return os;
}

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF CLASS TO OVERLOAD ISTREAM OPERATOR>>
* ----------------------------------------------------------------------*/

istream &operator>>(istream &input, Complex &z) {
    double Re{0}, Im{0};
    // done according to this:
    // http://codereview.stackexchange.com/questions/110783/simple-complex-number-class
    // regular expression for complex number
    regex bothRegex("^((-)?\\s*(\\d+(\\.\\d+)?))?\\s*(([-+])?\\s*(\\d+(\\.\\d+)?)i)?$");   
    string number;
    getline(input, number);
    smatch match;
    if (regex_match(number, match, bothRegex)) {
        // check if can find double values for real and imaginary parts
        Re = atof(match[3].str().c_str()); 
        Im = atof(match[7].str().c_str());
        // check if can find minus sign and if can, deal accordingly
        if (match[2].matched) {
            Re = -Re;
        }
        if (match[6].str() == "-") {
            Im = -Im;
        }
    } else {
        cerr << INPUT_FORMAT_ERROR << endl;
        throw exception(); // finish
    }
    // store obtained Re and Im values into Complex z number
    z.Re = Re;
    z.Im = Im;
    return input;
}

/* ------------------------------------------------------------------------
* TABLE HEADER
* ----------------------------------------------------------------------*/

void getTableHeader() {
    // get table fields names
    cout << endl;
    cout << "Complex No"     << "\t" 
         << "Real Part"      << "\t" 
         << "Imag Part"      << "\t" 
         << "Modulus"        << "\t" << "\t"
         << "Argument (rad)" << "\t" << "\t"
         << "Conjugate"      << "\t"
         << endl;
}

/* ------------------------------------------------------------------------
* PRINT OBJECT DATA TO SCREEN
* ----------------------------------------------------------------------*/

void Complex::showObjectData() const {
    // print out an object's data   
    cout.precision(3);
    cout << *this          << "\t" << "\t"
         << getRe()        << "\t" << "\t"
         << getIm()        << "\t" << "\t"
         << getModulus()   << "\t" << "\t"
         << getArgument()  << "\t" << "\t" << "\t"
         << getConjugate() << "\t" 
         << endl;
}

/* ------------------------------------------------------------------------
* DO ALL POSSIBLE OPERATIONS WITH TWO COMPLEX NUMBERS
* ----------------------------------------------------------------------*/

void doOperations(const Complex &a, const Complex &b) {
    getTableHeader(); // get table fields names
    // print real and imaginary components, modulus, argument and conjugate
    // also demonstrating use of overloaded operator<<
    a.showObjectData();
    b.showObjectData();
    // get sum, difference, product and quotient of a and b
    cout << endl << "Basic arithmetic operations: " << endl;
    cout << "a + b = " << a + b << endl;
    cout << "a - b = " << a - b << endl;
    cout << "a * b = " << a * b << endl;
    cout << "a / b = " << a / b << endl;
    cout << endl;
}

/* ------------------------------------------------------------------------
* MAIN PROGRAM
* ----------------------------------------------------------------------*/

int main() {  
    try {
        // Create two complex numbers
        Complex a(3, 4), b(1, -2);
        // demonstrate use of overloaded operator<<
        cout << endl << "Here is an example of outputs of this program." << endl;
        cout << "Assume two complex numbers: a = " << a << " and b = " << b << endl;
        cout << "Outputs: " << endl;
        doOperations(a, b); 

        Complex x, y;
        cout << "Please enter two complex numbers:" << endl;
        cout << endl << "First complex number: ";
        cin >> x;
        cout << endl << "Second complex number: ";
        cin >> y;
        doOperations(x, y);
    }
    catch (const exception&) { // caught exception in one of the functions
        cout << endl << "Bye!" << endl << endl;
        return EXIT_FAILURE;
    }
    cout << endl << "Bye!" << endl << endl;
    return EXIT_SUCCESS;
}