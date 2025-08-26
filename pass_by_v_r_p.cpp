#include <iostream>
using namespace std;

struct BigBox {
    int x, y, z;
    double w, h, d;
    char label[256];  // simulate larger data; check the assembly code to see how the code changes when changing the array size
};

// Pass by value: receives a full copy of the struct
double byValue(BigBox b) {
    return b.x + b.y + b.z + b.w + b.h + b.d;
}

// Pass by pointer: receives address of the struct
double byPointer(const BigBox* b) {
    return b->x + b->y + b->z + b->w + b->h + b->d;
}

// Pass by reference: receives an alias to the struct
double byReference(const BigBox& b) {
    return b.x + b.y + b.z + b.w + b.h + b.d;
}

int main() {
    BigBox box{1, 2, 3, 1.0, 2.0, 3.0, "Box"};

    double sum1 = byValue(box);
    double sum2 = byPointer(&box);
    double sum3 = byReference(box);

    cout << "Sum byValue: " << sum1 << endl;
    cout << "Sum byPointer: " << sum2 << endl;
    cout << "Sum byReference: " << sum3 << endl;

    return 0;
}
