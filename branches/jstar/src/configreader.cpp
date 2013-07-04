#include "configreader.h"
#include <iostream>
#include <main.h>
#include <boost/algorithm/string.hpp>

ConfigReader::ConfigReader()
{
}

    int ConfigReader::ReadFromFile( string fname )
    {
        FILE * fin;
        char line[255];
        bool typesRead=false;
        bool elementsRead=false;
        bool approxRead=false;
        bool mapRead=false;

        std::cout << "ConfigReader: Reading configuration from: " << fname.c_str() << std::endl;

        if ( (fin = fopen( fname.c_str(), "r" )) == NULL ) {
            std::cerr << "ConfigReader: Could not open configuration file: " << fname.c_str() << std::endl;
            return(-1);
        } else {
            while (!feof(fin)) {
                if ( fgets(line,254,fin) == NULL){
                    std::cout << "ConfigReader: Reading configuration ended." << std::endl;
                    return 0;
                }

                if ( strstr(line, "types:") != NULL ) {
                    typesRead = true;
                    elementsRead = true;
                    approxRead = false;
                    mapRead = false;
                    continue;
                }
                if ( strstr(line, "elements:") != NULL ) {
                    typesRead = false;
                    elementsRead = true;
                    approxRead = false;
                    mapRead = false;
                    continue;
                }
                if ( strstr(line, "approx:") != NULL ) {
                    typesRead = false;
                    elementsRead = false;
                    approxRead = true;
                    mapRead = false;
                    continue;
                }
                if ( strstr(line, "map:") != NULL ) {
                    typesRead = false;
                    elementsRead = false;
                    approxRead = false;
                    mapRead = true;
                    continue;
                }
                if (typesRead){
                    types.push_back(line);
                }else if (elementsRead){
                    elems.push_back(line);
                }else if (approxRead){
                    approx.push_back(line);
                }else if (mapRead){
                    char fieldTypeVal[25];
                    char FETypeVal[25];
                    char ApproxVal[25];
                    char ClassName[255];
                    sscanf (line, "%s %s %s %s", fieldTypeVal, FETypeVal, ApproxVal, ClassName);

                    vector<string>* vec = new vector<string>();
                    vec->push_back(fieldTypeVal);
                    vec->push_back(FETypeVal);
                    vec->push_back(ApproxVal);
                    vec->push_back(ClassName);
                    map.push_back(*vec);
                }else{
                    std::cout << "ConfigReader: Reading configuration ended." << std::endl;
                }
            }
            fclose(fin);
        }
        std::cout << "ConfigReader: Reading configuration ended." << std::endl;
        return(0);
    }

    int ConfigReader::ReadConfig()
    {
        std::cout << "ConfigReader: Reading configuration." << std::endl;

        std::string s;
        QVariant var =fvsettings.value( QString("/RSoft/FViewer/field_types"));
        if (var != 0){
            s = var.toString().toStdString();
            std::cout << "field_types:" << std::endl;
            boost::split(types, s, boost::is_any_of(" "));
            for (std::vector<string>::iterator it = types.begin(); it != types.end(); it++)
                std::cout <<  (*it) << std::endl;
        }
        var =fvsettings.value( QString("/RSoft/FViewer/finite_elements_types"));
        if (var != 0){
            s = var.toString().toStdString();
            std::cout << "finite_elements_types:" << std::endl;
            boost::split(elems, s, boost::is_any_of(" "));
            for (std::vector<string>::iterator it = elems.begin(); it != elems.end(); it++)
                std::cout <<  (*it) << std::endl;
        }
        var =fvsettings.value( QString("/RSoft/FViewer/approximation_degrees"));
        if (var != 0){
            s = var.toString().toStdString();
            std::cout << "approximation_degrees:" << std::endl;
            boost::split(approx, s, boost::is_any_of(" "));
            for (std::vector<string>::iterator it = approx.begin(); it != approx.end(); it++)
                std::cout <<  (*it) << std::endl;
        }
        var =fvsettings.value( QString("/RSoft/FViewer/map"));
        if (var != 0){
            s = var.toString().toStdString();
            std::cout << "map: " << std::endl;
            vector<string> tmp;
            boost::split(tmp, s, boost::is_any_of("\n"));

            for (vector<string>::iterator it=tmp.begin(); it!= tmp.end(); it++){
                vector<string>* vec = new vector<string>();
                std::cout << (*it) << std::endl;
                boost::split((*vec), (*it) , boost::is_any_of(" "));
                map.push_back(*vec);
            }
        }

        return(0);
    }

    void ConfigReader::cutEndline()
    {
//        std::cout << "types;\n";
        for (int i=0; i < types.size(); i++ ){
            types[i] = types[i].substr(0,types[i].size()-1); //ucinanie entera
//            std::cout << types[i] << std::endl;
        }
//        std::cout << "elements:\n";
        for (int i=0; i < elems.size(); i++ ){
            elems[i] = elems[i].substr(0,elems[i].size()-1); //ucinanie entera
//            std::cout << elems[i] << std::endl;
        }
//        std::cout << "\napprox:\n";
        for (int i=0; i < approx.size(); i++ ){
            approx[i] = approx[i].substr(0,approx[i].size()-1); //ucinanie entera
//            std::cout << approx[i] << std::endl;
        }

//        for (int i=0; i< map.size(); i++ ){
//            vector<string> v = map[i];
//            std::cout << i << " mapowanie: ";
//            for (int j=0; j< v.size(); j++)
//                std::cout << v[j] << " ";
//            std::cout << std::endl;
//        }
    }

