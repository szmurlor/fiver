#ifndef ConfigReader_H
#define ConfigReader_H

#include <sstream>
#include <string.h>
#include <stdio.h>
#include <vector>

using namespace std;

class ConfigReader
{
public:
    ConfigReader();
    int ReadFromFile( string fname );

    vector<string> elems;
    vector<string> approx;
};

#endif // ConfigReader_H
