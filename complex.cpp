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
    //ComplexNum(const ComplexNum& x) : re(x.re), im(x.im) {}

    // Addition operator
    ComplexNum operator+(const ComplexNum& a) const {
        return ComplexNum(re + a.re, im + a.im);
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
    c2 = c1 + c2;               // uses operator+
    cout << c2 << endl;         // uses operator<<
    return 0;
}
