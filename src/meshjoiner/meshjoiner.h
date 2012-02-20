#ifndef MESHJOINER_H
#define MESHJOINER_H

#include <grid.h>
#include <tetgen.h>

class MeshJoiner
{
public:
    MeshJoiner();
    ~MeshJoiner();

    Grid* exGrid;
    vector<Grid*> holes;
    string flags;

    Grid* joinMeshes(bool exSplitting);
    Grid* getGrid(tetgenio* out);
    tetgenio* getTetGenInput();
    tetgenio* getTetGenInput(Grid* grid);
    tetgenio* addHolesToTetInput (tetgenio* oldIn);

    void addGrid(Grid* g);
    void addHole(Grid* g);
    int getNoOfAllNodes();
    int getNoOfAllElems();
    void clearJoiner();

    void getPointInHole(Grid* currentHole, double p[3]);
    int countCuts(Grid* currentHole, double* point1, double* point2);
};

#endif // MESHJOINER_H
