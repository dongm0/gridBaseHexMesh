#include <eigen3/Eigen/Eigen>
#include "simplemesh.h"
#include "gridBaseHexMesh.h"

#include <fstream>

int main() {
    SimpleMesh mesh, meshout;
    std::ifstream fin;
    fin.open("test.vtk");
    mesh.InputVtk(fin);
    fin.close();
    std::ofstream fout;
    fout.open("out.vtk");
    GridBaseHexMesh::Voxel(mesh, meshout, 100);
    meshout.OutputVtk(fout);
    fout.close();
}