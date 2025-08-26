#include <iostream>
#include <string>
using namespace std;

class Foo {
private:
    string str;
public:
    Foo(string str = ""): str(str){cout << str << " ctor\n"; }
    Foo(const Foo& a):str(a.str) { cout << str << " copy ctor\n"; }
    ~Foo() { cout << str << " dtor\n"; }
};

Foo makeFoo() {
    Foo f("world");
    return f;   // return by value
}

int main() {
    Foo x("hello");  // #1
    x = makeFoo();   // #2
    
    
    Foo x1 = makeFoo();  //#1         
}
