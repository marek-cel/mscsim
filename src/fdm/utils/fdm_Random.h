/****************************************************************************//*
 * Copyright (C) 2021 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef FDM_RANDOM_H
#define FDM_RANDOM_H

////////////////////////////////////////////////////////////////////////////////

#include <fdm/fdm_Defines.h>

#include <fdm/utils/fdm_Singleton.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Random number generator.
 */
class FDMEXPORT Random : public Singleton< Random >
{
    friend class Singleton< Random >;

public:

    /**
     * @brief Gets random number from the given range.
     * @param min minimum random number value
     * @param max maximum random number value
     * @return random value
     */
    inline static int get( int min, int max )
    {
        return Random::instance()->getRandom( min, max );
    }

    /**
     * @brief Gets random number from the given range.
     * @param min minimum random number value
     * @param max maximum random number value
     * @return random value
     */
    inline static float get( float min, float max )
    {
        return Random::instance()->getRandom( min, max );
    }

    /**
     * @brief Gets random number from the given range.
     * @param min minimum random number value
     * @param max maximum random number value
     * @return random value
     */
    inline static double get( double min, double max )
    {
        return Random::instance()->getRandom( min, max );
    }

    /** Destructor. */
    virtual ~Random();

    /**
     * @brief Gets random number from the given range.
     * @param min minimum random number value
     * @param max maximum random number value
     * @return random value
     */
    int getRandom( int min, int max );

    /**
     * @brief Gets random number from the given range.
     * @param min minimum random number value
     * @param max maximum random number value
     * @return random value
     */
    float getRandom( float min, float max );

    /**
     * @brief Gets random number from the given range.
     * @param min minimum random number value
     * @param max maximum random number value
     * @return random value
     */
    double getRandom( double min, double max );

private:

    /**
     * You should use static function instance() due to get refernce
     * to Random class instance.
     */
    Random();

    /** Using this constructor is forbidden. */
    Random( const Random & ) {}
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_RANDOM_H
