#include <iostream>
#include <string>
using namespace std;

class A {
public:
    int val;
    char temp[32];
public:
    A(int i = 0): val(i){}
};

A bar(int i) {
    A obj(i);
    return obj;   // return by value
}

int fool_v(A obj){
    return obj.val;
}

int fool_r(A & obj){
    return obj.val;
}

int main() {
    A obj1;           
    A obj2(obj1);   
    
    obj1 = obj2;
    
    A obj3 = bar(3);    //return value optimization
    //A obj3;
    //obj3 = bar(3);
  
    int k = fool_v(obj3); //pass by value
    
    A &r_obj = obj3;
    
    int j = fool_r(r_obj); //pass by reference
  
    cout << "k = " << k << " j = " << j << endl;    
}
