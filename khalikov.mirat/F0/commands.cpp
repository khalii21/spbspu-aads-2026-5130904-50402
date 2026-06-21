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
}
