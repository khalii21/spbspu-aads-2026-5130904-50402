#include "graph.hpp"
#include "../S1/list.hpp"
#include <stdexcept>
#include <string>
#include <utility>

namespace khalikov {

  void swap(Graph& gr1, Graph& gr2) noexcept {
    gr1.connections.swap(gr2.connections);
    gr1.vertexes.swap(gr2.vertexes);
    gr1.pairs.swap(gr2.pairs);
  }

  graphTable parse(std::istream& in) {
    graphTable gt;
    std::string name;
    while (in >> name) {
      size_t count = 0;
      if (!(in >> count)) {
        break;
      }
      Graph graph;
      for (size_t i = 0; i < count; ++i) {
        std::string v1, v2;
        size_t weight = 0;
        in >> v1 >> v2 >> weight;
        pairOfVertexes edge{v1, v2};
        graph.connections[edge].pushBack(weight);
        graph.pairs.pushBack(edge);
        graph.vertexes.pushBack(v1);
        graph.vertexes.pushBack(v2);
      }
      graph.pairs.uniqueSort();
      graph.vertexes.uniqueSort();
      gt.insert(name, std::move(graph));
  }

  return gt;
  }

  bool contains(const List< std::string >& list, const std::string& value) {
    if (list.isEmpty()) {
      return false;
    }
    auto it = list.cbegin();
    auto start = it;
    do {
      if (*it == value) return true;
      ++it;
    }
    while (it != start);
    return false;
  }

  void graphs(std::ostream& out, std::istream&, graphTable& table) {
    List< std::string > names;
    for (auto it = table.begin(); it != table.end(); ++it) {
      names.pushBack(it->key);
    }
    if (names.isEmpty()) {
      return;
    }
    names.sort();
    auto it = names.cbegin();
    auto start = it;
    do {
      out << *it << '\n';
      ++it;
    }
    while (it != start);
  }

  void vertexes(std::ostream& out, std::istream& in, graphTable& table) {
    std::string graph;
    in >> graph;
    auto it = table.find(graph);
    if (it == table.end()) {
      throw std::runtime_error("not found");
    }
    List< std::string > vxs = it->value.vertexes;
    if (vxs.isEmpty()) {
      return;
    }
    auto vit = vxs.cbegin();
    auto vstart = vit;
    do {
      out << *vit << '\n';
      ++vit;
    }
    while (vit != vstart);
  }

  void outbound(std::ostream& out, std::istream& in, graphTable& table) {
    std::string graph, vertex;
    in >> graph >> vertex;
    auto git = table.find(graph);
    if (git == table.end() || !contains(git->value.vertexes, vertex)) {
      throw std::runtime_error("Not found");
    }
    List< std::string > targets;
    if (!git->value.pairs.isEmpty()) {
      auto pit = git->value.pairs.cbegin();
      auto pstart = pit;
      do {
        if (pit->first == vertex) {
          if (!contains(targets, pit->second)) {
            targets.pushBack(pit->second);
          }
        }
        ++pit;
      }
      while (pit != pstart);
      targets.sort();
      if (targets.isEmpty()) return;

		  auto tit = targets.cbegin();
		  auto tstart = tit;
		  do {
		    std::string tov = *tit;
		    out << tov;
		    auto eit = git->value.connections.find({vertex, tov});
		    List< size_t > weights = eit->value;
		    weights.sort();
		    auto wit = weights.cbegin();
		    auto wstart = wit;
		    do {
		      out << " " << *wit;
		      ++wit;
		    } while (wit != wstart);
		    out << "\n";
		    ++tit;
		  }
      while (tit != tstart);
		}
  }
}
