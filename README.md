# PRNG
This is a small, simple and fast pseudo-random number generator for C++.

## Why use it over the C++11 standards?
The standards are more verbose, slower and the quality of the random numbers is empirically worse than this library. This library also acts as a class you would need to implement to mock out the rng to work with gmock.

This library will suite 99% of cases, however the standards have support for more distributions than just uniform, which is the only distribution supported here. You may also dislike the relatively unproven nature of the xorshift\* algorithm under the hood used over Mersenne Twister.

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
rng.setSeed();
```

You can get the internal state with getState() for future use as a seed if needed. If somehow you're still stuck a full list of functions can be obtained with doxygen or by simply scanning through the file, or using your IDE's auto complete.

The PRNG object is not thread safe, however seeding is. You should provide each thread with it's own PRNG. You may wish to use a singleton object pool for this. 

## How does it work?
It utilises the xorshift\* algorithm. You can find more information about why it's designed the way it is on my [blog](http://www.mleadbetter.com).

## Testing
To compile and run the tests you will need gtest installed. Once that is done you can run the tests from the project folder with:

``` bash
mkdir build
cd build
cmake ..
make
./PRNG
```

The test suite only tests for implementation details, it makes no attempt to comprehensively test the PRNG engine. Other people have already done [that](http://xorshift.di.unimi.it/).

