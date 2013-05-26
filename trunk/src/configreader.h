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
    void cutEndline();

    vector<string> types;
    vector<string> elems;
    vector<string> approx;

    vector< vector<string> > map;
};

#endif // ConfigReader_H
