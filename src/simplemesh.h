#include <eigen3/Eigen/Eigen>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct SimpleMesh {
public:
  enum class MeshType {
    Tri,
    Quad,
    Tet,
    Hex
  };    

  Eigen::MatrixXd V;
  Eigen::MatrixXi C;

  SimpleMesh();
  SimpleMesh(const SimpleMesh &rhs);
  SimpleMesh(const Eigen::MatrixXd &V, const Eigen::MatrixXi &C);
  SimpleMesh(const std::string &filename) {}

  void InputVtk(std::istream &stream) {
    std::stringstream ss;
    std::string line;
    std::string stmp;
    uint32_t vnum = 0, cnum = 0;
    std::getline(stream, line);
    std::getline(stream, line);
    std::getline(stream, line);
    if (line != "ASCII") {
      throw std::runtime_error("ascii only!");
    }
    std::getline(stream, line);
    std::getline(stream, line);
    ss.clear();
    ss.str(line);
    ss >> stmp >> vnum >> stmp;
    V.resize(vnum, 3);

    std::vector<double> v;
    for (uint32_t i = 0; i < vnum; ++i) {
      std::getline(stream, line);
      ss.clear();
      ss.str(line);
      ss >> v[0] >> v[1] >> v[2];
      V.row(i) << v[0], v[1], v[2];
    }

    std::getline(stream, line);
    ss.clear();
    ss.str(line);
    ss >> stmp >> cnum >> stmp;
    C.resize(cnum, 3);
    uint32_t _r[3];

    for (uint32_t i = 0; i < cnum; ++i) {
      std::getline(stream, line);
      ss.clear();
      ss.str(line);
      ss >> stmp >> _r[0] >> _r[1] >> _r[2];
      C.row(i) << _r[0], _r[1], _r[2];
    }

    std::getline(stream, line);
    ss.clear();
    ss.str(line);
    for (uint32_t i=0; i<cnum; ++i) {
      int itmp;
      std::getline(stream, line);
      ss.clear();
      ss.str(line);
      ss >> itmp;
      if (itmp!=5) {
        throw std::runtime_error("triangle only!");
      }
    }
  }

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
