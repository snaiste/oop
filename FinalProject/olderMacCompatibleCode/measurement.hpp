#ifndef MEASUREMENT_HPP
#define MEASUREMENT_HPP

#include <iostream>  // std
#include <complex>   // complex numbers
#include "msg.hpp"   // classes managing message outputs

/* ------------------------------------------------------------------------
* ABSTRACT MEASUREMENT BASE CLASS TEMPLATE
* -----------------------------------------------------------------------*/

template <typename T> class BaseMeasurement {
protected:
    unsigned timestamp;
    T dataPoint;
public:
    // default constructor
    BaseMeasurement() : timestamp{}, dataPoint{} {
        DebugMsg::print("[BASE-MEASUREMENT] Default constructor called\n");
    }

    // parametrised constructor
    BaseMeasurement(const unsigned& userTimestamp, const T& userDataPoint) 
                   : timestamp{userTimestamp}, dataPoint{userDataPoint} {
        DebugMsg::print("[BASE-MEASUREMENT] Parametrised constructor called\n");
    }

    // copy constructor for deep copying
    BaseMeasurement(const BaseMeasurement& userMeasurement) {
        DebugMsg::print("[BASE-MEASUREMENT] Copy constructor for deep copying called\n");
        this->timestamp = userMeasurement.timestamp;
        this->dataPoint = userMeasurement.dataPoint;
    }

    // move constructor
    BaseMeasurement(BaseMeasurement&& userMeasurement) {
        DebugMsg::print("[BASE-MEASUREMENT] Move constructor called\n");
        // steal the data
        this->timestamp = userMeasurement.timestamp;
        this->dataPoint = userMeasurement.dataPoint;
        // delete user object's data
        userMeasurement.timestamp = {};
        userMeasurement.dataPoint = {};
    }

    // default virtual destructor
    virtual ~BaseMeasurement() = default;

    // timestamp access function
    unsigned getTimestamp() const { return this->timestamp; }
    // data point access function
    T getDataPoint() const { return this->dataPoint; }

    // pure virtual read and print functons
    virtual void read(std::istream& is) = 0;
    virtual void print(std::ostream& os) const = 0;

    // copy assignment operator
    BaseMeasurement& operator=(const BaseMeasurement& userMeasurement) {
        DebugMsg::print("[BASE-MEASUREMENT] Copy assignment operator called\n");
        if (&userMeasurement == this) { return *this; } // no self-assignment
        // first delete this object’s data
        this->timestamp = {};
        this->dataPoint = {};
        // declare new object
        this->timestamp = userMeasurement.timestamp;
        this->dataPoint = userMeasurement.dataPoint;
        return *this;
    }

    // move assignment operator
    BaseMeasurement& operator=(BaseMeasurement&& userMeasurement) {
        DebugMsg::print("[BASE-MEASUREMENT] Move assignment operator called\n");
        std::swap(this->timestamp, userMeasurement.timestamp);
        std::swap(this->dataPoint, userMeasurement.dataPoint);       
        return *this;
    }

    // comparison operator
    bool operator<(const BaseMeasurement<T>& otherMeasurement) const {
        DebugMsg::print("[BASE-MEASUREMENT] Operator< called\n");
        return (this->timestamp < otherMeasurement.timestamp);
    }
};

/* ------------------------------------------------------------------------
* DEFINE AN ABSTRAC MEASUREMENT BASE CLASS TEMPLATE
* -----------------------------------------------------------------------*/

// forward declarations needed when declaring template friend function of template class
// http://stackoverflow.com/questions/18792565/declare-template-friend-function-of-template-class
template <typename T> class Measurement;
template <typename T> 
// allow the overloaded operator>> function to be a friend of RealMeasurement class
// returns a reference to an istream, because it modifies an instream
std::istream& operator>>(std::istream& is, Measurement<T>& userMeasurement);
template <typename T> 
// allow the overloaded operator>> function to be a friend of RealMeasurement class
std::ostream& operator<<(std::ostream& os, const Measurement<T>& userMeasurement);

template <typename T> class Measurement : public BaseMeasurement<T> {
    friend std::istream& operator>> <>(std::istream& is, Measurement<T>& userMeasurement);
    // allow the overloaded operator>> function to be a friend of RealMeasurement class
    friend std::ostream& operator<< <>(std::ostream& os, const Measurement<T>& userMeasurement);
public:
    // default constructor - calling base class constructor
    Measurement() : BaseMeasurement<T>() {
        DebugMsg::print("[MEASUREMENT] Default constructor called\n");
    };

    // parametrised constructor - calling base class constructor
    Measurement(const unsigned& userTimestamp, const T& userDataPoint) 
                   : BaseMeasurement<T>(userTimestamp, userDataPoint) {
        DebugMsg::print("[MEASUREMENT] Parametrised constructor called\n");
    };

    // copy constructor for deep copying - calling base class copy constructor
    Measurement(const Measurement& userMeasurement) 
                   : BaseMeasurement<T>(userMeasurement) {
        DebugMsg::print("[MEASUREMENT] Copy constructor for deep copying called\n");
    }

    // move constructor - calling base class move constructor
    Measurement(Measurement&& userMeasurement) 
                   : BaseMeasurement<T>(userMeasurement) {
        DebugMsg::print("[MEASUREMENT] Move constructor called\n");
    }

    // read from input stream using friend operator>>
    void read(std::istream& is) override {
        is >> this->timestamp
           >> this->dataPoint;
    }

    // read from input stream using friend operator<<
    void print(std::ostream& os) const override {
        os << this->timestamp
           << "\t"
           << this->dataPoint;
    }

    // copy assignment operator - calling base class assignment operator
    Measurement& operator=(const Measurement& userMeasurement) {
        DebugMsg::print("[MEASUREMENT] Copy assignment operator called\n");
        BaseMeasurement<T>::operator=(userMeasurement);
        return *this;
    }

    // move assignment operator - calling base class move assignment operator
    Measurement& operator=(Measurement&& userMeasurement) {
        DebugMsg::print("[MEASUREMENT] Move assignment operator called\n");
        BaseMeasurement<T>::operator=(userMeasurement);
        return *this;
    }

    // comparison operator
    bool operator<(const Measurement<T>& otherMeasurement) const {
        DebugMsg::print("[MEASUREMENT] Operator< called\n");
        return BaseMeasurement<T>::operator<(otherMeasurement);
    }
};

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF MEASUREMENT CLASS TO OVERLOAD ISTREAM OPERATOR>>
* -----------------------------------------------------------------------*/

template <typename T> 
std::istream& operator>>(std::istream& is, Measurement<T>& userMeasurement) {
    userMeasurement.read(is);
    return is;
}

/* ------------------------------------------------------------------------
* DEFINE A FRIEND OF MEASUREMENT CLASS TO OVERLOAD OSTREAM OPERATOR<<
* -----------------------------------------------------------------------*/

template <typename T> 
std::ostream& operator<<(std::ostream& os, const Measurement<T>& userMeasurement) {
    userMeasurement.print(os);
    return os;
}

#endif /* MEASUREMENT_HPP */