#C++ MVar

[![Build Status](https://travis-ci.org/tdauth/cpp-mvar.svg?branch=master)](https://travis-ci.org/tdauth/cpp-mvar)
[![Code Coverage](https://img.shields.io/codecov/c/github/tdauth/cpp-mvar/master.svg)](https://codecov.io/github/tdauth/cpp-mvar?branch=master)

C++ header-only library which provides a lock-based [MVar](http://hackage.haskell.org/package/base-4.12.0.0/docs/Control-Concurrent-MVar.html) implementation.

MVar's allow the synchronization of values between multiple threads.
They are similar to buffered channels with a buffer size of 1.

The following example shows how a string value is synchronized between two threads:
```cpp
#include <iostream>
#include <string>
#include <thread>

#include <mvar.hpp>

int main() {
	mvar::MVar<std::string> mvar;

	// Thread 1
	std::thread t1([&mvar] {
		auto v = mvar.take();
		std::cerr << v << std::endl;
	});

	// Thread 2
	std::thread t2([&mvar] { mvar.put("Hello world!"); });

	t1.join();
	t2.join();

	return 0;
}
```

Thread 1 blocks until a value is available in the MVar and can be taken from it.
It prints the value to the error output stream.
The MVar remains empty afterwards.

Thread 2 blocks until the MVar is empty and puts the string value into the MVar.