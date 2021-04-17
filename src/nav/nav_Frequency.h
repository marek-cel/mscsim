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
#ifndef NAV_FREQUENCY_H
#define NAV_FREQUENCY_H

////////////////////////////////////////////////////////////////////////////////

namespace nav
{

/**
 * @brief Navigation frequency class.
 *
 * @see ICAO Annex 10 Volume I: Radio Navigation Aids
 */
class Frequency
{
public:

    /**
     * @brief Returns DME frequency based on TACAN channel.
     * @param chan TACAN channel
     * @return [kHz] DME frequency
     */
    static int getFreqDME( const char *chan );

    /**
     * @brief Returns DME frequency based on ILS LOC frequency.
     * @param freq_ils [kHz] ILS LOC frequency
     * @return [kHz] DME frequency
     */
    static int getFreqDME( int freq_ils );

    /**
     * @brief Returns GS frequency based on ILS LOC frequency.
     * @param freq_ils [kHz] ILS LOC frequency
     * @return [kHz] DME frequencys
     */
    static int getFreqGS( int freq_ils );
};

} // end of nav namepsace

////////////////////////////////////////////////////////////////////////////////

#endif // NAV_FREQUENCY_H
