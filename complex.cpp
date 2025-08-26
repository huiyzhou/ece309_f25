#include <iostream>
using namespace std;

class ComplexNum {
private:
    float re;
    float im;

public:
    // Constructor
    ComplexNum(float a, float b) : re(a), im(b) {}

    // Copy constructor
    ComplexNum(const ComplexNum& x) : re(x.re), im(x.im) {
        cout << "Copy constructor called" << endl;
    }

    // Assignment operator
    ComplexNum& operator=(const ComplexNum& other) {
        cout << "Assignment operator called" << endl;
        if (this != &other) {   // protect against self-assignment
            re = other.re;
            im = other.im;
        }
        return *this;           // return reference to current object
    }

    // Destructor
    ~ComplexNum() {
        cout << "Destructor called for (" << re << ", " << im << ")" << endl;
    }

    // Addition operator
    ComplexNum operator+(const ComplexNum& a) const {
        return ComplexNum(re + a.re, im + a.im);     //return by value (in the cases when you need to create a new object)
    }
    
    ComplexNum & operator+=(const ComplexNum& a) {
        re += a.re;
        im += a.im;
        return *this;                               //return by reference (in the cases when there is an existing object)
    }
    

    // Output operator
    friend ostream& operator<<(ostream& stream, const ComplexNum& a) {
        stream << a.re << "  " << a.im;
        return stream;
    }
};

int main() {
    ComplexNum c1(1, 2);
    ComplexNum c2(c1);          // calls copy constructor
    c2 = c1 + c2;               // uses operator+ and assignment operator
    cout << c2 << endl;         // uses operator<<
    ComplexNum c3(c1);
    c3 += c2;                   // uses operator+=
    cout << c3 << endl;         // uses operator<<
    
    return 0;
}
