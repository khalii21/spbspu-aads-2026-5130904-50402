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
	  if (find(storage, name) != nullptr) {
	    throw std::runtime_error("Already exists");
	  }
	  khalikov::Matrix matrix(rows, cols, base);
	  storage.insert(std::make_pair(name, std::move(matrix)));
	}
}
