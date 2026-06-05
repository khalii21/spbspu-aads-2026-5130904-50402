#include "hashtable.hpp"
#include "hasher.hpp"
#include <boost/test/unit_test.hpp>
#include <functional>

using namespace khalikov;
using h_t = HashTable< int, int, SipHash< int >, std::equal_to< int > >;


BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  h_t ht;
  BOOST_CHECK(ht.isEmpty());
  BOOST_CHECK_EQUAL(ht.getCap(), 8);
  BOOST_CHECK_EQUAL(ht.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  h_t ht;
  h_t yaht(ht);
  BOOST_CHECK(yaht.isEmpty());
  BOOST_CHECK_EQUAL(yaht.getSize(), ht.getSize());
  BOOST_CHECK_EQUAL(yaht.getCap(), ht.getCap());
}


BOOST_AUTO_TEST_CASE(move_constructor_test)
{
  h_t ht;
  ht.insert(1, 1);
  BOOST_CHECK_EQUAL(ht.getSize(), 1);
  h_t yaht;
  BOOST_CHECK_EQUAL(yaht.getSize(), 0);
  yaht = std::move(ht);
  BOOST_CHECK(ht.isEmpty());
  BOOST_CHECK_EQUAL(yaht.getSize(), 1);
}

BOOST_AUTO_TEST_SUITE_END()
