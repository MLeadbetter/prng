# PRNG
This is a small, simple and fast pseudo-random number generator for C++.

## Why use it over the C++11 standards?
The standards are more verbose, slower and the quality of the random numbers is empirically worse than this library. This library also acts as a class you would need to implement to mock out the rng to work with gmock.

This library will suit 99% of cases, however the standards have support for more distributions than just uniform, which is the only distribution supported here.

## How to use it
For ease of use this is a header only library. Simply include the header where needed and:

``` cpp
PRNG rng;
cout << rng.getRandomInt(1, 5) << endl;
```

That's it! You can probably guess how to get a random long, unsigned long, float or double (all standard types are supported). You can set the seed in the constructor or with setSeed like so:

``` cpp
std::array<uint64_t, 16> seed = {...};
PRNG rng(seed);
rng.setSeed(seed);
```

You can get the internal state with getState() for future use as a seed if needed. If somehow you're still stuck a full list of functions can be obtained with doxygen or by simply scanning through the file, or using your IDE's auto complete.

The PRNG object is not thread safe, however seeding is. You should provide each thread with it's own PRNG. You may wish to use a singleton object pool for this. 

## How does it work?
It utilises the xorshift\* algorithm. You can find more information about why it's designed the way it is on my [blog](http://www.mleadbetter.com).

## Testing
You can run the tests from the project folder with:

``` bash
mkdir build
cd build
cmake ..
make
./PRNG
```

The test suite only tests for implementation details, it makes no attempt to comprehensively test the PRNG engine. Other people have already done [that](http://xorshift.di.unimi.it/).

## Virtual at test time
For performance reasons the functions are only made virtual at test time to allow for mocks. To do this the TEST macro should be defined. If the PRNG include is below the gtest include (as is common convention) you won't need to set up any additional compilation flags.
