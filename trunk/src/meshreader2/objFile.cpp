#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "objFile.h"

using namespace std;

ObjFile::ObjFile()
{
}

ObjFile::~ObjFile()
{
}

Grid * ObjFile::loadFromFile( string fileName )
{
    grid = new Grid();
    indices.clear();
    texCoords.clear();

    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cout<<"*.obj load file error: "<<fileName<<endl;
        return NULL;
    }


    cerr<<"Reading elements..."<<endl;

    string line;
    ObjFragment frag;
    frag.indexCount=0;

    wasCoords = false;
    wasNormals = false;

    int lastType=-1;
    int groups[]={-1,-1,-1,-1};

    while (!file.eof() && getline(file,line))
    {
        int type=getDataType(line);

        if(type!=lastType)
        {
            if(type!=-1 && type<V_)
            {
                groups[type]++;
                if(groups[type]>=frags.size())
                    frags.push_back(frag);

                if(type==FACE_)
                {
                    if(frags.size()>1)
                    {
                        frags[ frags.size()-1 ].indexOffset = frags[ frags.size()-2 ].indexCount +
                                                              frags[ frags.size()-2 ].indexOffset;
                    }
                    else
                        frags[0].indexOffset=0;
                }
            }

            lastType=type;
        }

        switch(type)
        {
            case V_:
            {
                Node n(grid);
                try
                {
                    loadNodeVerts(line, n);
                }
                catch(string e)
                {
                    cout<<"Load obj verts error: " << e << endl;
                    return NULL;
                }

                grid->_nodes.push_back(n);
                break;
            }

            case VT_:
            {
                wasCoords = true;
                break;
            }

            case VN_:
            {
                wasNormals = true;
                break;
            }

            case FACE_:
            {
                loadFaceFromLineui(line,indices);
                frags[ groups[FACE_] ].indexCount += 3;
                break;
            }

            case NAMEo_:
            {
                if( groups[NAMEo_] > groups[NAMEg_] )
                    frags[ groups[NAMEo_] ].name.insert( frags[groups[NAMEo_] ].name.begin(), line.begin()+2, line.end() );
                break;
            }

            case NAMEg_:
            {
                if( groups[NAMEo_] < groups[NAMEg_] )
                    frags[ groups[NAMEg_] ].name.insert( frags[ groups[NAMEg_] ].name.begin(), line.begin()+2, line.end() );
                break;
            }

            default:
                break;
        }
    }

    file.close();

    optimalize();

    grid->_elems.resize( indices.size()/3 );

    for( int i=0; i<indices.size(); i+=3)
    {
        Elem *e = new ElemT3n2D( grid );
        ((ElemT3n2D*)e)->setNodes( indices[i]+1, indices[i+1]+1, indices[i+2]+1 );
        grid->_elems[ i/3 ] = e;
        e->nr = i/3 + 1;
        grid->addElemToSubdomain( i/3 + 1, 1);
    }


    grid->setGridName( fileName );
    grid->NumberOfSpaceDim = 3;
    grid->attr["etype"] = "T3n2D";
    cout << "In file found " << grid->_nodes.size() << " unique nodes!" << endl;

    return grid;
}

int ObjFile::getDataType(string line)
{
    if(line.find("v ")==0 || line.find("V ")==0 )
    {
        return V_;
    }

    if(line.find("vt ")==0 || line.find("VT ")==0)
        return VT_;

    if(line.find("vn")==0)
        return VN_;

    if(line.find("f ")==0)
        return FACE_;

    if(line.find("g ")==0)
    {
        return NAMEg_;
    }

    if(line.find("o ")==0 || line.find("O ")==0)
    {
        return NAMEo_;
    }

    return -1;
}

float strToFloat(string str)
{
    stringstream ss;
    float out;
    ss<<str;
    ss>>out;
    if(!ss)
        throw string("Unable to parse to float: "+str);
    return out;
}

void ObjFile::loadNodeVerts(string line, Node& n)
{
    vector<string>values;
    for(int i=0;i<line.length();i++)
    {
        if(isdigit(line[i]) || line[i]=='-')
        {
            values.push_back("");
            while(isdigit(line[i]) || line[i]=='.' || line[i]=='-' || line[i]==',' || line[i]=='e' || line[i]=='+')
            {
                if(line[i] == ',')
                    values[values.size()-1]+='.';
                else
                    values[values.size()-1]+=line[i];
                i++;
            }
        }
    }

    if( values.size() != 3)
        throw string("Bad line: " + line);

    for(int i=0;i<values.size();i++)
    {
        n[i] = strToFloat(values[i]);
    }

}

void ObjFile::loadNodeCoords(string line, std::vector<float>& coords)
{
    vector<string>values;
    for(int i=0;i<line.length();i++)
    {
        if(isdigit(line[i]) || line[i]=='-')
        {
            values.push_back("");
            while(isdigit(line[i]) || line[i]=='.' || line[i]=='-' || line[i]==',' || line[i]=='e' || line[i]=='+')
            {
                if(line[i] == ',')
                    values[values.size()-1]+='.';
                else
                    values[values.size()-1]+=line[i];
                i++;
            }
        }
    }

    if( values.size() != 2)
        throw string("Bad line: " + line);

    for(int i=0;i<values.size();i++)
    {
        coords.push_back(strToFloat(values[i]));
    }
}

int strToInt(string str)
{
    stringstream ss;
    int out;
    ss<<str;
    ss>>out;
    return out;
}

void ObjFile::loadFaceFromLineui(string line, vector<unsigned int> &v)
{
    vector<string>values;
    for(int i=0; i<line.length(); i++)
    {
        if( isdigit( line[i] ) )
        {
            values.push_back("");

            while( isdigit(line[i]) )
            {
                values.back()+=line[i];
                i++;
            }
        }
    }

    for(int i=0; i<values.size(); i++)
    {
        v.push_back( ( (unsigned int)strToInt(values[i]) ) -1 );
    }

    if( values.size() != ( 1 + (int)wasCoords + (int)wasNormals ) *3 )
    {
        cout << "This line can generate mistakes in indexing nodes: " << line
             << "\nShould be count of numbers like: " << ( 1 + (int)wasCoords + (int)wasNormals ) *3 <<endl;

        if( values.size()%4 == 0 )
        {
            int step = values.size()/4;
            for(int i=0; i<values.size(); i++)
            {
                if( i<step || i>=step*2 )
                    v.push_back( ( (unsigned int)strToInt(values[i]) ) -1 );
            }
        }
    }
}

struct IndexInfo
{
    IndexInfo(unsigned int v_,unsigned int c_, unsigned int n_):v(v_),c(c_),n(n_),idx(0){}
    unsigned int v,n,c,idx;
    friend bool operator<(const IndexInfo& a,const IndexInfo& b)
    {
        return a.v<b.v || (a.v==b.v && a.c<b.c) || (a.v==b.v && a.c==b.c && a.n<b.n);
    }
    friend bool operator==(const IndexInfo& a,const IndexInfo& b)
    {
        return a.v==b.v && a.c==b.c && a.n==b.n;
    }
};

void ObjFile::optimalize()
{
    vector<IndexInfo> idx_uniq,indexx;
    int jumpSize = 1 + (int)wasCoords + (int)wasNormals;
    for(int i=0; i<indices.size(); i+=jumpSize)
    {
        idx_uniq.push_back(IndexInfo(indices[i], wasCoords ? indices[i+1]:0 , wasNormals ? indices[i+jumpSize-1]:0  ));
    }

    indexx=idx_uniq;

    sort(idx_uniq.begin(),idx_uniq.end());

    typedef vector<IndexInfo>::iterator Iter;

    Iter iter_uniq = std::unique(idx_uniq.begin(), idx_uniq.end());

    idx_uniq.erase(iter_uniq, idx_uniq.end());

    int id=0;
    for (Iter iter = idx_uniq.begin(); iter != idx_uniq.end(); iter++)
    {
        iter->idx = id++;
    }

    vector<unsigned int>newindex;
    for (Iter iter = indexx.begin(); iter != indexx.end(); iter++)
    {
        Iter i = std::find(idx_uniq.begin(), idx_uniq.end(), *iter);
        newindex.push_back(i->idx);
    };

    vector<float> vertices,coords;
    vertices.resize(idx_uniq.size()*3);

    for (Iter iter = idx_uniq.begin(); iter != idx_uniq.end(); iter++)
    {
        for( int i=0;i<3;i++)
        {
            vertices[ iter->idx*3+i ] = grid->_nodes[ iter->v ][i];

        }
    }

    grid->_nodes.resize(idx_uniq.size());

    for(int i=0; i<idx_uniq.size(); i++)
    {
        for(int j=0; j<3; j++)
        {
            grid->_nodes[i][j] = vertices[i*3+j];
        }

        grid->_nodes[i].nr = i+1;
    }

    indices=newindex;
}
