/*
    Copyright 2015 Matthew Leadbetter

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.


    If you have any queries I can be contacted by e-mail: matthew@mleadbetter.com
*/

#ifndef PRNG_H
#define PRNG_H

#ifdef TEST
#define TEST_VIRTUAL virtual
#else
#define TEST_VIRTUAL
#endif

#include <mutex>
#include <random>
#include <array>
#include <cmath>
#include <cassert>
#include <limits>

class PRNG
{
public:
    PRNG() : position(0)
    {
        static std::random_device rd;
        static std::mutex rngMutex;
        std::lock_guard<std::mutex> lock(rngMutex);
        for(uint64_t &s : state)
        {
            // Unfortunately random_device returns unsigned ints rather than uint64_ts
            if(std::numeric_limits<unsigned int>::digits >= 64)
            {
                s = rd();
            }
            else if(std::numeric_limits<unsigned int>::digits >= 32) {
                s = rd() & 0xffffffff;
                s |= static_cast<uint64_t>(rd()) << 32;
            }
            else // Not sure what hardware/compiler you're coding for but I guess it's in the standards
            {
                s = rd() & 0xffff;
                s |= (static_cast<uint64_t>(rd()) << 16) & 0xffffffff;
                s |= (static_cast<uint64_t>(rd()) << 32) & 0xffffffffffff;
                s |= (static_cast<uint64_t>(rd()) << 48);
            }
        }
    }

    TEST_VIRTUAL ~PRNG() {}

    /**
     * @brief Constructs a new PRNG with a set seed
     * @param seed - the value seed it should start with
     */
    PRNG(const std::array<uint64_t, 16> &seed) : state(seed), position(0) {}

    /**
     * @brief Sets the internal state to a new seed.
     *
     * PRNG will produce the same pseudo-random results every time after being provided with a fixed seed.
     *
     * @param seed - the value to change the internal state to
     */
    TEST_VIRTUAL void setSeed(const std::array<uint64_t, 16> &seed)
    {
        state = seed;
    }

    /**
     * @brief Returns the internal state.
     *
     * You might want to do this to find out what the current state is so you can seed with it again
     * to reproduce your results.
     *
     * @param seed - the value to change the internal state to
     */
    TEST_VIRTUAL const std::array<uint64_t, 16> &getState() const
    {
        return state;
    }

    /**
     * @brief Generates a random char
     * @return a char containing a random number
     */
    TEST_VIRTUAL char getRandomChar()
    {
        return static_cast<char>(xorshift1024());
    }

    /**
     * @brief Generates a random unsigned char
     * @return an unsigned char containing a random number
     */
    TEST_VIRTUAL unsigned char getRandomUnsignedChar()
    {
        return static_cast<unsigned char>(xorshift1024());
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed char containing a random number
     */
    TEST_VIRTUAL char getRandomChar(const char &minValue, const char &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned char containing a random number
     */
    TEST_VIRTUAL unsigned char getRandomUnsignedChar(const unsigned char &maxValue)
    {
        return getRandomIntType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return an unsigned char containing a random number
     */
    TEST_VIRTUAL unsigned char getRandomUnsignedChar(const unsigned char &minValue, const unsigned char &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random int
     * @return an int containing a random number
     */
    TEST_VIRTUAL int getRandomInt()
    {
        return static_cast<int>(xorshift1024());
    }

    /**
     * @brief Generates a random unsigned int
     * @return an unsigned int containing a random number
     */
    TEST_VIRTUAL unsigned int getRandomUnsignedInt()
    {
        return static_cast<unsigned int>(xorshift1024());
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed integer containing a random number
     */
    TEST_VIRTUAL int getRandomInt(const int &minValue, const int &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned integer containing a random number
     */
    TEST_VIRTUAL unsigned int getRandomUnsignedInt(const unsigned int &maxValue)
    {
        return getRandomIntType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return an unsigned integer containing a random number
     */
    TEST_VIRTUAL unsigned int getRandomUnsignedInt(const unsigned int &minValue, const unsigned int &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random long
     * @return a long containing a random number
     */
    TEST_VIRTUAL long getRandomLong()
    {
        return static_cast<long>(xorshift1024());
    }

    /**
     * @brief Generates a random unsigned long
     * @return an unsigned long containing a random number
     */
    TEST_VIRTUAL unsigned long getRandomUnsignedLong()
    {
        return static_cast<unsigned long>(xorshift1024());
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed long containing a random number
     */
    TEST_VIRTUAL long getRandomLong(const long &minValue, const long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned long containing a random number
     */
    TEST_VIRTUAL unsigned long getRandomUnsignedLong(const unsigned long &maxValue)
    {
        return getRandomIntType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return an unsigned long containing a random number
     */
    TEST_VIRTUAL unsigned long getRandomUnsignedLong(const unsigned long &minValue, const unsigned long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random long long
     * @return a long long containing a random number
     */
    TEST_VIRTUAL long long getRandomLongLong()
    {
        return static_cast<long long>(xorshift1024());
    }

    /**
     * @brief Generates a random unsigned long long
     * @return an unsigned long long containing a random number
     */
    TEST_VIRTUAL unsigned long long getRandomUnsignedLongLong()
    {
        return static_cast<unsigned long long>(xorshift1024());
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed long long containing a random number
     */
    TEST_VIRTUAL long long getRandomLongLong(const long long &minValue, const long long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned long long containing a random number
     */
    TEST_VIRTUAL unsigned long long getRandomUnsignedLongLong(const unsigned long long &maxValue)
    {
        return getRandomIntType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return an unsigned long long containing a random number
     */
    TEST_VIRTUAL unsigned long long getRandomUnsignedLongLong(const unsigned long long &minValue, const unsigned long long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and 1
     * @return a float containing a random number
     */
    TEST_VIRTUAL float getRandomFloat()
    {
        return getRandomFloatType<float>();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a float containing a random number
     */
    TEST_VIRTUAL float getRandomFloat(float maxValue)
    {
        return getRandomFloatType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @return a float containing a random number
     */
    TEST_VIRTUAL float getRandomFloat(float minValue, float maxValue)
    {
        return getRandomFloatType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and 1
     * @return a double containing a random number
     */
    TEST_VIRTUAL double getRandomDouble()
    {
        return getRandomFloatType<double>();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a double containing a random number
     */
    TEST_VIRTUAL double getRandomDouble(double maxValue)
    {
        return getRandomFloatType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @return a double containing a random number
     */
    TEST_VIRTUAL double getRandomDouble(double minValue, double maxValue)
    {
        return getRandomFloatType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and 1
     * @return a long double containing a random number
     */
    TEST_VIRTUAL long double getRandomLongDouble()
    {
        return getRandomFloatType<long double>();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a long double containing a random number
     */
    TEST_VIRTUAL long double getRandomLongDouble(long double maxValue)
    {
        return getRandomFloatType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @return a long double containing a random number
     */
    TEST_VIRTUAL long double getRandomLongDouble(long double minValue, long double maxValue)
    {
        return getRandomFloatType(minValue, maxValue);
    }

    /**
     * @brief Generates a random uint64_t
     * @return a uint64_t containing a random number
     */
    TEST_VIRTUAL uint64_t getRandomUint64()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a uint64_t containing a random number
     */
    TEST_VIRTUAL uint64_t getRandomUint64(const uint64_t &maxValue)
    {
        assert(maxValue != 0);
        int leadingZeros = countLeadingZeros64(maxValue);

        uint64_t randomInt;
        do
        {
            randomInt = xorshift1024() >> leadingZeros;
        } while(randomInt > maxValue);

        return randomInt;
    }

private:
    std::array<uint64_t, 16> state;
    unsigned long position;

    template <class T>
    T getRandomIntType(const T &minValue, const T &maxValue)
    {
        assert(minValue < maxValue);
        uint64_t range = static_cast<uint64_t>(maxValue - minValue);
        T rand = static_cast<T>(getRandomUint64(range));
        return rand + minValue;
    }

    template <class T>
    T getRandomIntType(const T &maxValue)
    {
        assert(maxValue <= std::numeric_limits<uint64_t>::max());
        return static_cast<T>(getRandomUint64(maxValue));
    }

    template <class T>
    T getRandomFloatType(const T &minValue, const T &maxValue)
    {
        T range = std::abs(maxValue - minValue);
        return getRandomFloatType<T>(range) + minValue;
    }

    template <class T>
    T getRandomFloatType(const T &maxValue)
    {
        return getRandomFloatType<T>() * maxValue;
    }

    template <class T>
    T getRandomFloatType()
    {
        constexpr T epsilon = std::numeric_limits<T>::epsilon();
        constexpr int digitsToKeep = std::numeric_limits<T>::digits-1;
        constexpr int digitsToLose = std::numeric_limits<uint64_t>::digits - digitsToKeep;
        T rand = (xorshift1024() >> digitsToLose) * epsilon;
        return rand;
    }

    uint64_t xorshift1024()
    {
        uint64_t state0 = state[position];
        position = (position + 1) % 16;
        uint64_t state1 = state[position];
        state1 ^= state1 << 31;
        state1 ^= state1 >> 11;
        state0 ^= state0 >> 30;
        state[position] = state0 ^ state1;
        return state[position] * 1181783497276652981LL;
    }

    int countLeadingZeros64(const uint64_t &toCount)
    {
    #ifdef __GNUC__
        return __builtin_clzll(toCount) - (std::numeric_limits<unsigned long long>::digits - 64);
    #elif defined(_MSC_VER)
        return __lzcnt(toCount);
    #else
        static const int clz64Index[64] = {
            63, 16, 62,  7, 15, 36, 61,  3,
             6, 14, 22, 26, 35, 47, 60,  2,
             9,  5, 28, 11, 13, 21, 42, 19,
            25, 31, 34, 40, 46, 52, 59,  1,
            17,  8, 37,  4, 23, 27, 48, 10,
            29, 12, 43, 20, 32, 41, 53, 18,
            38, 24, 49, 30, 44, 33, 54, 39,
            50, 45, 55, 51, 56, 57, 58,  0
        };
        uint64_t bitset = toCount;
        bitset |= bitset >> 1;
        bitset |= bitset >> 2;
        bitset |= bitset >> 4;
        bitset |= bitset >> 8;
        bitset |= bitset >> 16;
        bitset |= bitset >> 32;
        return clz64Index[(bitset * 0x03f79d71b4cb0a89ULL) >> 58];
    #endif
    }

    // You really shouldn't copy this class.
    PRNG(PRNG const&) = delete;
    void operator=(PRNG const&) = delete;
};

#undef TEST_VIRTUAL
#endif // PRNG_H
