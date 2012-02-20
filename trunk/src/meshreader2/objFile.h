#ifndef OBJFILE_H
#define OBJFILE_H

#include <grid.h>
#include <field.h>
#include <simu.h>
#include <QtDebug>

#include <cmath>

/*
 * @author Furry
 */

struct ObjFragment
{
    std::string name;
    unsigned int indexOffset,indexCount,vertexCount;
};

class ObjFile
{
public:
    //MTL - materiał, nazwa grupy, nazwa obiektu, trójkąt, wierchołek, koordynaty, normalne, MTLlib
    enum DATATYPE{MTL_,NAMEg_,NAMEo_,FACE_,V_,VT_,VN_,MTLlib_};
    ObjFile();
    ~ObjFile();
    Grid * loadFromFile( string fileName );

private:
    std::vector<ObjFragment> frags;
    std::vector<unsigned int> indices;
    std::vector<float> texCoords;
    bool wasCoords,wasNormals;
    Grid* grid;

    int getDataType(std::string fileName);
    void optimalize();
    void loadFaceFromLineui(std::string line, std::vector<unsigned int> &v);
    void loadNodeVerts(std::string line, Node& n);
    void loadNodeCoords(std::string line, std::vector<float>& coords);
};

#endif // OBJFILE_H
