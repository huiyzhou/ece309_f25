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

int main() {
    Foo x("hello");           // #1
    x = makeFoo("world");   // #2
    
    cout << x.str << endl;
    
    Foo x1 = makeFoo("ECE309");  //#1
    
    cout << x1.str << endl;
}
