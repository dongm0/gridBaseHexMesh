#include "config.h"
#include <eigen3/Eigen/Eigen>

#include <algorithm>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "simplemesh.h"
#include <igl/signed_distance.h>
#ifdef USE_OPENVOLUMEMESH
#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>
#endif

namespace GridBaseHexMesh {
static void Voxel(const SimpleMesh &meshin, SimpleMesh &hexmesh, int voxelnum) {
  using namespace Eigen;
  Vector3d minuv, maxuv;
  minuv = meshin.V.colwise().minCoeff();
  maxuv = meshin.V.colwise().maxCoeff();
  Vector3d edgelen = maxuv - minuv;
  double voxelsize = edgelen.maxCoeff() / voxelsize;
  minuv -= voxelsize * Vector3d(10, 10, 10);
  maxuv += voxelsize * Vector3d(10, 10, 10);
  edgelen = maxuv - minuv;

  int segnum_x = edgelen(0) / voxelsize, segnum_y = edgelen(1) / voxelsize,
      segnum_z = edgelen(2) / voxelsize;

  MatrixXd hc;
  MatrixXi hv;
  hv.resize((segnum_x + 1) * (segnum_y + 1) * (segnum_z + 1), 3);
  hc.resize(segnum_x * segnum_y * segnum_z, 8);
  // construct points
  for (uint32_t i = 0; i <= segnum_z; ++i) {
    for (uint32_t j = 0; j <= segnum_y; ++j) {
      for (uint32_t k = 0; k <= segnum_x; ++k) {
        hv.row(i * (segnum_x + 1) * (segnum_y + 1) + j * (segnum_x + 1) + k) =
            minuv + k * Vector3d(1, 0, 0) + j * Vector3d(0, 1, 0) +
            i * Vector3d(0, 0, 1);
      }
    }
  }
  // construct cells
  for (uint32_t i = 0; i < segnum_z; ++i) {
    for (uint32_t j = 0; j < segnum_y; ++j) {
      for (uint32_t k = 0; k < segnum_x; ++k) {
        int _stvnum =
            i * (segnum_x + 1) * (segnum_y + 1) + j * (segnum_x + 1) + k;
        int _c = i * segnum_x * segnum_y + j * segnum_x + k;
        hc.row(_c) << _stvnum, _stvnum + 1, _stvnum + segnum_x + 2,
            _stvnum + segnum_x + 1, _stvnum + (segnum_x + 1) * (segnum_y + 1),
            _stvnum + (segnum_x + 1) * (segnum_y + 1) + 1,
            _stvnum + (segnum_x + 1) * (segnum_y + 1) + segnum_x + 1,
            _stvnum + (segnum_x + 1) * (segnum_y + 1) + segnum_x + 1;
      }
    }
  }
  // remove extra cells
  MatrixXd hcenter;
  hcenter.resize(hc.rows(), 3);
  for (uint32_t i = 0; i < hc.rows(); ++i) {
    Vector3d tmp;
    for (uint32_t j = 0; j < 8; ++j) {
      tmp += hv.row(hc(i, j));
    }
    tmp /= 8;
    hcenter.row(i) = tmp;
  }
  MatrixXd C, N;
  VectorXi I;
  VectorXd S;
  igl::signed_distance(hcenter, meshin.V, meshin.C,
                       igl::SIGNED_DISTANCE_TYPE_FAST_WINDING_NUMBER, S, I, C,
                       N);
  std::vector<uint32_t> restc;
  restc.reserve(hc.rows());
  for (uint32_t i = 0; i < hc.rows(); ++i) {
    if (S(i) < 0) {
      restc.push_back(i);
    }
  }
  std::unordered_map<uint32_t, uint32_t> restc_rev;
  for (uint32_t i = 0; i < restc.size(); ++i) {
    restc_rev.insert({restc.at(i), i});
  }
  std::unordered_set<uint32_t> restv_hash;
  for (uint32_t i = 0; i < restc.size(); ++i) {
    for (uint32_t j = 0; j < 8; ++j) {
      restv_hash.insert(hc(i, j));
    }
  }
  std::vector<int> restv(restv_hash.begin(), restv_hash.end());
  MatrixXd newhv;
  MatrixXi newhc;
  newhv.resize(restv.size(), 3);
  newhc.resize(restc.size(), 8);
  for (uint32_t i = 0; i < restv.size(); ++i) {
    newhv.row(i) = hv.row(restv.at(i));
  }
  for (uint32_t i = 0; i < restc.size(); ++i) {
    // eigen 3.4 and newer versions begin supporting stl for_each() algorithm
    for (uint32_t j = 0; j < 8; ++j) {
      newhc(i, j) = restc_rev[newhc(restc.at(i), j)];
    }
  }
  hexmesh = SimpleMesh(newhc, newhv);
  return;
}
#ifdef USE_OPENVOLUMEMESH
template <typename HexMesh>
static void Voxel(const SimpleMesh &meshin, HexMesh &hexmesh, int voxelnum);
#endif
} // namespace GridBaseHexMesh
