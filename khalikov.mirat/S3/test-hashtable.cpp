#include "hashtable.hpp"
#include "hasher.hpp"
#include <boost/test/unit_test.hpp>
#include <functional>

using namespace khalikov;

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(default_constructor_test)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > ht;
  BOOST_CHECK(ht.isEmpty());
  BOOST_CHECK_EQUAL(ht.getCap(), 8);
  BOOST_CHECK_EQUAL(ht.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(copy_constructor_test)
{
  HashTable< int, int, SipHash< int >, std::equal_to< int > > ht;
  HashTable< int, int, SipHash< int >, std::equal_to< int > > yaht(ht);
  BOOST_CHECK(yaht.isEmpty());
  BOOST_CHECK_EQUAL(yaht.getSize(), ht.getSize());
  BOOST_CHECK_EQUAL(yaht.getCap(), ht.getCap());
}

BOOST_AUTO_TEST_SUITE_END()
