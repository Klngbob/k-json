#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class value {
public:
    value() {cout<<"construct"<<endl;}
    ~value();
    vector<value> a;
};

value::~value() {
    cout<<"destruct"<<endl;
}

void test1() {
    value n;
    
    for(int i=0; i<2; ++i) {
        value b;
        n.a.push_back(b);
    }
}

class v {
public:
    vector<pair<string, v>> obj;
};

void test2() {
    int a = 1;
    int &b = a;
    cout<<&b<<endl;
    cout<<&a<<endl;
}

int main() {
    //test1();
    //test2();
    char b = '1' + 1;
    string a;
    a.push_back('1'+ 1);
    cout<<a<<endl;
    return 0;
}