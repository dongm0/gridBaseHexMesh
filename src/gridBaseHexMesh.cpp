#include "gridBaseHexMesh.h"

#include "config.h"
#include <eigen3/Eigen/Eigen>

#include <optional>
#include <string>
#include <vector>

#include "simplemesh.h"
#include <igl/signed_distance.h>
#ifdef USE_OPENVOLUMEMESH
#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>
#endif

void GridBaseHexMesh::Voxel(const SimpleMesh &meshin, SimpleMesh &hexmesh,
                            int voxelnum) {
  using namespace Eigen;
  Vector3d minuv, maxuv;
  minuv = meshin.V.colwise().minCoeff();
  maxuv = meshin.V.colwise().maxCoeff();
}