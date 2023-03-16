#include <iostream>
#include <string>
#include "json.h"

using namespace std;

int main(int argc, char* argv[])
{
    while(true)
    {
        k_json::KJson kjson;
        cout << "Enter json: ";
        string json, status;
        cin >> json;
        kjson.parse(json, status);
        cout << status << endl;
        json.clear();
        kjson.stringify(json);
        cout << "Generate to json: " << json << endl;
    }
    return 0;
}