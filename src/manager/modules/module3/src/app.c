#include <thenext.h>

#include <iostream>
using namespace std;


class Person {
public:
    int a;
    int b;
    string str;
    Person();
    ~Person();
};

Person::Person() {
    this.a = 10;
    this.b = 20;
    this.str = "hola";
}
Person::~Person() {
    
}
char * Person::toJSON() {
    return (char *) str.c_str();
}


extern "C" {

    int action1(int argc, char *argv[]) {
        
        Person person;
        
        _print(_system(argv), person.toJSON());
        return HTTP_200;
    }

    int action3(int argc, char *argv[]) {
        _print(_system(argv), "action3");
        return HTTP_200;
    }
    
    int action1(int argc, char *argv[]) {
        
        
        
        
        
        string str;
        
        str = "hola1";
        str += " hola2";
                
        _print(_system(argv), (char *) str.c_str());
                
        //_print(_system(argv), "action1");
        
        return HTTP_200;
    }
    
}