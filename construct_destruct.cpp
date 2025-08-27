#include <iostream>
#include <string>
using namespace std;

class Foo {
public:
    string str;
public:
    Foo(string str = ""): str(str){cout << str << " ctor\n"; }
    Foo(const Foo& a):str(a.str) { cout << str << " copy ctor\n"; }
    ~Foo() { cout << str << " dtor\n"; }
};

Foo makeFoo(string str) {
    Foo f(str);
    return f;   // return by value
}

//Example of pass by value (i.e., a copy of Foo is created and then deleted
//int SizeFoo(Foo f){
//    return(f.str.size());
//}

int main() {
    Foo x("hello");           // #1
    x = makeFoo("world");   // #2
    
    cout << x.str << endl;
    
    Foo x1 = makeFoo("ECE309");  //#1
    
    
    cout << x1.str << endl;
    
    //int size = SizeFoo(x1);
    
    //cout << size << endl;
}
