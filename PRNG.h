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
            if(sizeof(unsigned int)*8  >= 64)
            {
                s = rd();
            }
            else if(sizeof(unsigned int)*8 >= 32) {
                s = rd() & 0xffffffff;
                s |= (uint64_t)rd() << 32;
            }
            else // Not sure what hardware/compiler you're coding for but I guess it's in the standards
            {
                s = rd() & 0xffff;
                s |= ((uint64_t)rd() << 16) & 0xffffffff;
                s |= ((uint64_t)rd() << 32) & 0xffffffffffff;
                s |= ((uint64_t)rd() << 48);
            }
        }
    }

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
    virtual void setSeed(const std::array<uint64_t, 16> &seed)
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
    virtual const std::array<uint64_t, 16> &getState() const
    {
        return state;
    }

    /**
     * @brief Generates a random char
     * @return a char containing a random number
     */
    virtual char getRandomChar()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random unsigned char
     * @return an unsigned char containing a random number
     */
    virtual unsigned char getRandomUnsignedChar()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed char containing a random number
     */
    virtual char getRandomChar(const char &minValue, const char &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned char containing a random number
     */
    virtual unsigned char getRandomUnsignedChar(const unsigned char &maxValue)
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
    virtual unsigned char getRandomUnsignedChar(const unsigned char &minValue, const unsigned char &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random int
     * @return an int containing a random number
     */
    virtual int getRandomInt()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random unsigned int
     * @return an unsigned int containing a random number
     */
    virtual unsigned int getRandomUnsignedInt()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed integer containing a random number
     */
    virtual int getRandomInt(const int &minValue, const int &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned integer containing a random number
     */
    virtual unsigned int getRandomUnsignedInt(const unsigned int &maxValue)
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
    virtual unsigned int getRandomUnsignedInt(const unsigned int &minValue, const unsigned int &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random long
     * @return a long containing a random number
     */
    virtual long getRandomLong()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random unsigned long
     * @return an unsigned long containing a random number
     */
    virtual unsigned long getRandomUnsignedLong()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed long containing a random number
     */
    virtual long getRandomLong(const long &minValue, const long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned long containing a random number
     */
    virtual unsigned long getRandomUnsignedLong(const unsigned long &maxValue)
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
    virtual unsigned long getRandomUnsignedLong(const unsigned long &minValue, const unsigned long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random long long
     * @return a long long containing a random number
     */
    virtual long long getRandomLongLong()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random unsigned long long
     * @return an unsigned long long containing a random number
     */
    virtual unsigned long long getRandomUnsignedLongLong()
    {
        return xorshift1024();
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @throw cassert ensuring minValue < maxValue
     * @return a signed long long containing a random number
     */
    virtual long long getRandomLongLong(const long long &minValue, const long long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return an unsigned long long containing a random number
     */
    virtual unsigned long long getRandomUnsignedLongLong(const unsigned long long &maxValue)
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
    virtual unsigned long long getRandomUnsignedLongLong(const unsigned long long &minValue, const unsigned long long &maxValue)
    {
        return getRandomIntType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and 1
     * @return a float containing a random number
     */
    virtual float getRandomFloat()
    {
        return getRandomFloatType<float>();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a float containing a random number
     */
    virtual float getRandomFloat(float maxValue)
    {
        return getRandomFloatType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @return a float containing a random number
     */
    virtual float getRandomFloat(float minValue, float maxValue)
    {
        return getRandomFloatType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and 1
     * @return a double containing a random number
     */
    virtual double getRandomDouble()
    {
        return getRandomFloatType<double>();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a double containing a random number
     */
    virtual double getRandomDouble(double maxValue)
    {
        return getRandomFloatType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @return a double containing a random number
     */
    virtual double getRandomDouble(double minValue, double maxValue)
    {
        return getRandomFloatType(minValue, maxValue);
    }

    /**
     * @brief Generates a random number between 0 and 1
     * @return a long double containing a random number
     */
    virtual long double getRandomLongDouble()
    {
        return getRandomFloatType<long double>();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a long double containing a random number
     */
    virtual long double getRandomLongDouble(long double maxValue)
    {
        return getRandomFloatType(maxValue);
    }

    /**
     * @brief Generates a random number between minValue and maxValue (inclusive)
     * @param minValue - The lowest value this should return
     * @param maxValue - The highest value this should return
     * @return a long double containing a random number
     */
    virtual long double getRandomLongDouble(long double minValue, long double maxValue)
    {
        return getRandomFloatType(minValue, maxValue);
    }

private:
    std::array<uint64_t, 16> state;
    int position;

    template <class T>
    T getRandomIntType(const T &minValue, const T &maxValue)
    {
        assert(minValue < maxValue);
        uint64_t range = maxValue - minValue;
        return getRandom_uint64(range) + minValue;
    }

    template <class T>
    T getRandomIntType(const T &maxValue)
    {
        assert(maxValue <= std::numeric_limits<uint64_t>::max());
        return getRandom_uint64(maxValue);
    }

    template <class T>
    T getRandomFloatType(const T &minValue, const T &maxValue)
    {
        T range = abs(maxValue - minValue);
        uint64_t rand = xorshift1024();
        return (T)rand * range / std::numeric_limits<uint64_t>::max() + minValue;
    }

    template <class T>
    T getRandomFloatType(const T &maxValue)
    {
        uint64_t rand = xorshift1024();
        return (T)rand * maxValue / std::numeric_limits<uint64_t>::max();
    }

    template <class T>
    T getRandomFloatType()
    {
        uint64_t rand = xorshift1024();
        return (T)rand / std::numeric_limits<uint64_t>::max();
    }

    /**
     * @brief Generates a random number between 0 and maxValue (inclusive)
     * @param maxValue - The highest value this should return
     * @return a uint64_t containing a random number
     */
    uint64_t getRandom_uint64(const uint64_t &maxValue)
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

    uint64_t xorshift1024(void)
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
        return __builtin_clzll(toCount) - (sizeof(unsigned long long)*8 - 64);
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

#endif // PRNG_H
