#define BOOST_TEST_MODULE MVarTest

#include <boost/test/included/unit_test.hpp>

#include "../mvar.hpp"

BOOST_AUTO_TEST_CASE(MVarString)
{
	mvar::MVar<std::string> var("test");
	BOOST_REQUIRE(!var.isEmpty());
	BOOST_REQUIRE(!var.tryPut("10"));
	BOOST_REQUIRE_EQUAL("test", var.read());
	BOOST_REQUIRE_EQUAL("test", var.tryRead().value());
	BOOST_REQUIRE(!var.isEmpty());
	BOOST_REQUIRE_EQUAL("test", var.take());
	BOOST_REQUIRE(var.isEmpty());
	BOOST_REQUIRE(!var.tryRead().has_value());
	BOOST_REQUIRE(!var.tryTake().has_value());
	var.put("1");
	BOOST_REQUIRE(!var.isEmpty());
	BOOST_REQUIRE_EQUAL("1", var.tryTake().value());
	BOOST_REQUIRE(var.isEmpty());
	BOOST_REQUIRE(var.tryPut("1"));
	BOOST_REQUIRE_EQUAL("1", var.swap("11"));
	BOOST_REQUIRE_EQUAL("11", var.take());
}

BOOST_AUTO_TEST_CASE(MVarVoid)
{
	mvar::MVar<void> var;
	BOOST_REQUIRE(var.isEmpty());
	BOOST_REQUIRE(!var.tryRead());
	BOOST_REQUIRE(!var.tryTake());
	var.put();
	BOOST_REQUIRE(!var.isEmpty());
	BOOST_REQUIRE(!var.tryPut());
	var.read();
	BOOST_REQUIRE(var.tryRead());
	BOOST_REQUIRE(!var.isEmpty());
	var.take();
	BOOST_REQUIRE(var.isEmpty());
	BOOST_REQUIRE(var.tryPut());
	BOOST_REQUIRE(var.tryTake());
}
