#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <main.hpp>

BOOST_AUTO_TEST_SUITE(test_main)
   
BOOST_AUTO_TEST_CASE(test_testfunc)
{
    BOOST_CHECK_EQUAL(1,1);
}
