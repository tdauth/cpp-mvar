#define BOOST_TEST_MODULE MVarTest

#include <thread>

#include <boost/test/included/unit_test.hpp>

#include "../mvar.hpp"

BOOST_AUTO_TEST_CASE(MVarInt)
{
	mvar::MVar<int> var;
	BOOST_REQUIRE(var.isEmpty());
	var.put(1);
	BOOST_REQUIRE(!var.isEmpty());
	BOOST_REQUIRE_EQUAL(1, var.read());
	BOOST_REQUIRE(!var.isEmpty());
	BOOST_REQUIRE_EQUAL(1, var.take());
	BOOST_REQUIRE(var.isEmpty());
	var.put(1);
	BOOST_REQUIRE(!var.isEmpty());
}

BOOST_AUTO_TEST_CASE(MVarIntMultipleThreads)
{
	mvar::MVar<int> var;
	BOOST_REQUIRE(var.isEmpty());

	std::vector<int> results;
	std::mutex m;

	std::thread t0([&var, &results, &m] {
		auto r = var.take();
		std::lock_guard<std::mutex> l(m);
		results.push_back(r);
	});

	std::thread t1([&var] { var.put(1); });

	std::thread t2([&var] { var.put(2); });

	std::thread t3([&var, &results, &m] {
		auto r = var.take();
		std::lock_guard<std::mutex> l(m);
		results.push_back(r);
	});

	t0.join();
	t1.join();
	t2.join();
	t3.join();

	BOOST_REQUIRE(var.isEmpty());
	BOOST_REQUIRE_EQUAL(2, results.size());
	BOOST_REQUIRE(std::find(results.begin(), results.end(), 1) != results.end());
	BOOST_REQUIRE(std::find(results.begin(), results.end(), 2) != results.end());
}

BOOST_AUTO_TEST_CASE(MVarVoid)
{
	mvar::MVar<void> var;
	BOOST_REQUIRE(var.isEmpty());
	var.put();
	BOOST_REQUIRE(!var.isEmpty());
	var.read();
	BOOST_REQUIRE(!var.isEmpty());
	var.take();
	BOOST_REQUIRE(var.isEmpty());
	var.put();
	BOOST_REQUIRE(!var.isEmpty());
}
