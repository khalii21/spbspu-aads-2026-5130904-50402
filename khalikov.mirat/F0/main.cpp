#include <utility>
#include <functional>
#include <string>
#include "commands.hpp"
#include "RBTree.hpp"
#include "../common/hashtable.hpp"
#include "../common/hasher.hpp"

auto f = std::bind(
    std::less< std::string >(),
    std::bind(&pair_t::first, std::placeholders::_1),
    std::bind(&pair_t::first, std::placeholders::_2)
);

using hash_t = khalikov::HashTable< std::string, cmd_t, khalikov::SipHash< std::string >, std::equal_to< std::string > >;

int main()
{
  Storage mxs(f);
  hash_t cmds;
  cmds["create"] = khalikov::create;
  cmds["show"] = khalikov::show;
  cmds["fill"] = khalikov::fill;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cout, std::cin, mxs);
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
