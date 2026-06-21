#include "commands.hpp"
#include <sstream>

namespace khalikov {
	const khalikov::Matrix *find(const Storage &storage, const std::string &name) {
	  for (auto it = storage.cbegin(); it != storage.cend(); ++it) {
	    if ((*it).first == name) {
	      return &((*it).second);
	    }
	  }
	  return nullptr;
	}

	khalikov::Matrix *find(Storage &storage, const std::string &name) {
	  for (auto it = storage.begin(); it != storage.end(); ++it) {
	    if ((*it).first == name) {
	      return &((*it).second);
	    }
	  }
	  return nullptr;
	}

  void checkInput(std::istream &in)
  {
    while (in.peek() != '\n' && in.peek() != EOF) {
      char ch = in.peek();
      if(!std::isspace(ch)) {
        throw std::invalid_argument("Signature of command is failed");
      }
      in.get();
    }
    if (in.peek() == '\n') {
      in.get();
    }
  }

	void create(std::ostream &, std::istream &in, Storage &storage)
	{
	  std::string line;
	  if (!std::getline(in, line)) {
	    throw std::invalid_argument("Input error");
	  }
	  std::stringstream ss(line);
	  std::string name;
	  size_t rows = 0, cols = 0;
	  int base = 10;
	  if (!(ss >> name >> rows >> cols)) {
	    throw std::invalid_argument("Input error");
	  }
	  ss >> base;
	  checkInput(ss);
	  if (find(storage, name) != nullptr) {
	    throw std::runtime_error("Already exists");
	  }
	  khalikov::Matrix matrix(rows, cols, base);
	  storage.insert(std::make_pair(name, std::move(matrix)));
	}

  void fill(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Unknown matrix");
    }
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    int base = matrix->getBase();
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        std::string token;
        if (!(in >> token)) {
          throw std::invalid_argument("Input error");
        }
        (*matrix)[i][j] = matrix->stol(token, base);
      }
    }
    checkInput(in);
  }

  void show(std::ostream &out, std::istream &in, Storage &storage)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::invalid_argument("Input error");
    }
    const Matrix *matrix = find(storage, name);
    if (!matrix) {
      throw std::runtime_error("Unknown matrix");
    }
    checkInput(in);
    size_t r = matrix->getRows();
    size_t c = matrix->getCols();
    int base = matrix->getBase();
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        out << matrix->ltos((*matrix)[i][j], base);
        if (j + 1 < c) {
          out << " ";
        }
      }
      out << "\n";
    }
  }

  void add(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string resName, m1Name, m2Name;
    int base = 10;
    if (!(in >> resName >> base >> m1Name >> m2Name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, m1Name);
    const Matrix *m2 = find(storage, m2Name);
    if (!m1 || !m2) {
      throw std::runtime_error("Unknown matrix");
    }
    if (m1->getRows() != m2->getRows() || m1->getCols() != m2->getCols()) {
      throw std::runtime_error("Incompatible sizes");
    }
    size_t r = m1->getRows();
    size_t c = m1->getCols();
    Matrix result(r, c, base);
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        Matrix::check((*m1)[i][j], (*m2)[i][j], '+');
        result[i][j] = (*m1)[i][j] + (*m2)[i][j];
      }
    }
    storage.insert(std::make_pair(resName, std::move(result)));
  }

  void sub(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string resName, m1Name, m2Name;
    int base = 10;
    if (!(in >> resName >> base >> m1Name >> m2Name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, m1Name);
    const Matrix *m2 = find(storage, m2Name);
    if (!m1 || !m2) {
      throw std::runtime_error("Unknown matrix");
    }
    if (m1->getRows() != m2->getRows() || m1->getCols() != m2->getCols()) {
      throw std::runtime_error("Incompatible sizes");
    }
    size_t r = m1->getRows();
    size_t c = m1->getCols();
    Matrix result(r, c, base);
    for (size_t i = 0; i < r; ++i) {
      for (size_t j = 0; j < c; ++j) {
        Matrix::check((*m1)[i][j], (*m2)[i][j], '-');
        result[i][j] = (*m1)[i][j] - (*m2)[i][j];
      }
    }
    storage.insert(std::make_pair(resName, std::move(result)));
  }

  void mul(std::ostream &, std::istream &in, Storage &storage)
  {
    std::string resName, m1Name, m2Name;
    int base = 10;
    if (!(in >> resName >> base >> m1Name >> m2Name)) {
      throw std::invalid_argument("Input error");
    }
    checkInput(in);
    const Matrix *m1 = find(storage, m1Name);
    const Matrix *m2 = find(storage, m2Name);
    if (!m1 || !m2) {
      throw std::runtime_error("Matrix not found");
    }
    if (m1->getCols() != m2->getRows()) {
      throw std::runtime_error("Incompatible sizes for multiplication");
    }
    size_t r1 = m1->getRows();
    size_t c1 = m1->getCols();
    size_t c2 = m2->getCols();
    Matrix result(r1, c2, base);
    for (size_t i = 0; i < r1; ++i) {
      for (size_t k = 0; k < c1; ++k) {
        long long a = (*m1)[i][k];
        if (a == 0) {
          continue;
        }
        for (size_t j = 0; j < c2; ++j) {
          long long b = (*m2)[k][j];
          Matrix::check(a, b, '*');
          long long prod = a * b;
          Matrix::check(result[i][j], prod, '+');
          result[i][j] += prod;
        }
      }
    }
    storage.insert(std::make_pair(resName, std::move(result)));
  }

}
