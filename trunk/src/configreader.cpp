#include "configreader.h"
#include <iostream>

ConfigReader::ConfigReader()
{
}

    int ConfigReader::ReadFromFile( string fname )
    {
        FILE * fin;
        char line[255];
        int i = 0;
        bool elementsRead=false;
        bool approxRead=false;

        if ( (fin = fopen( fname.c_str(), "r" )) == NULL ) {
            return(-1);
        } else {
            while (!feof(fin)) {
                if ( fgets(line,254,fin) == NULL){
                    return 0;
                }

                if ( strstr(line, "elements") != NULL ) {
//                     sscanf(line,"elements %d", &i);
                    elementsRead = true;
                    approxRead = false;
                    continue;
                }
                if ( strstr(line, "approx") != NULL ) {
                    elementsRead = false;
                    approxRead = true;
                    continue;
                }

                if (elementsRead)
                    elems.push_back(line);
                else if (approxRead)
                    approx.push_back(line);
                else
                    std::cerr << "ConfigReader: nothing to read in configuration file" << std::endl;
            }
            fclose(fin);
        }
        return(0);
    }

