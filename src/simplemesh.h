#include <eigen3/Eigen/Eigen>

#include <fstream>
#include <iostream>
#include <string>

struct SimpleMesh {
public:
  Eigen::MatrixXd V;
  Eigen::MatrixXi C;

  SimpleMesh();
  SimpleMesh(const SimpleMesh &rhs);
  SimpleMesh(const Eigen::MatrixXd &V, const Eigen::MatrixXi &C);
  SimpleMesh(const std::string &filename);

  void OutputVtk(std::ostream &stream) {
    stream << "# vtk DataFile Version 3.0\ngenerate by dongmo\nASCII\nDATASET "
              "UNSTRUCTURED_GRID"
           << std::endl;
    stream << "POINTS " << V.rows() << " double" << std::endl;
    for (uint32_t i = 0; i < V.rows(); ++i) {
      stream << V(i, 0) << " " << V(i, 1) << " " << V(i, 2) << std::endl;
    }
    stream << "CELLS " << C.rows() << " " << 9 * C.rows() << std::endl;
    for (uint32_t i = 0; i < C.rows(); ++i) {
      stream << C(i, 0) << " " << C(i, 1) << " " << C(i, 2) << " " << C(i, 3)
             << " " << C(i, 4) << " " << C(i, 5) << " " << C(i, 6) << " "
             << C(i, 7) << std::endl;
    }
    stream << "CELL_TYPES " << C.rows() << std::endl;
    for (uint32_t i = 0; i < C.rows(); ++i) {
      stream << "12" << std::endl;
    }
  }
};
