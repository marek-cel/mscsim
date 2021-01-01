/****************************************************************************//*
 * Copyright (C) 2020 Marek M. Cel
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
#ifndef FDM_TABLE1_H
#define FDM_TABLE1_H

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 1D table and linear interpolation class.
 */
class FDMEXPORT Table1
{
public:

    /**
     * @brief Creates table with only one record.
     * @param val record value
     */
    static Table1 oneRecordTable( double val = 0.0 );

    /** @brief Constructor. */
    Table1();

    /** @brief Constructor. */
    Table1( const std::vector< double > &key_values,
            const std::vector< double > &table_data );

    /** @brief Copy constructor. */
    Table1( const Table1 &table );

    /** @brief Destructor. */
    virtual ~Table1();

    /**
     * @brief Returns key value for the given key index.
     * @param key_index key index
     * @return key value on success or NaN on failure
     */
    double getIndexValue( unsigned int key_index ) const;

    /** */
    inline unsigned int getSize() const { return _size; }

    /**
     * @brief Returns key of minimum table value.
     * @return key of minimum table value
     */
    double getKeyOfValueMin() const;

    /**
     * @brief Returns key of minimum table value within given range.
     * @param key_min range minimum
     * @param key_max range maximum
     * @return key of minimum table value
     */
    double getKeyOfValueMin( double key_min, double key_max ) const;

    /**
     * @brief Returns key of maximum table value.
     * @return key of maximum table value
     */
    double getKeyOfValueMax() const;

    /**
     * @brief Returns key of maximum table value within given range.
     * @param key_min range minimum
     * @param key_max range maximum
     * @return key of maximum table value
     */
    double getKeyOfValueMax( double key_min, double key_max ) const;

    /**
     * @brief Returns table value for the given key.
     * Returns table value for the given key value using linear interpolation
     * algorithm.
     * @param key_value key value
     * @return interpolated value on success or NaN on failure
     */
    double getValue( double key_value ) const;

    /**
     * @brief Returns table value for the given key index.
     * @param key_index key index
     * @return value on success or NaN on failure
     */
    double getValueByIndex( unsigned int key_index ) const;

    /**
     * @brief Returns table first value.
     * @return value on success or NaN on failure
     */
    double getFirstValue() const;

    /**
     * @brief Returns table last value.
     * @return value on success or NaN on failure
     */
    double getLastValue() const;

    /**
     * @brief Returns minimum table value.
     * @return minimum table value
     */
    double getValueMin() const;

    /**
     * @brief Returns maximum table value.
     * @return maximum table value
     */
    double getValueMax() const;

    /**
     * @brief Checks if table is valid.
     * @return returns true if size is greater than 0 and all data is valid
     */
    bool isValid() const;

    /**
     * @brief Multiplies keys by the given factor.
     * @param factor given factor
     */
    void multiplyKeys( double factor );

    /**
     * @brief Multiplies values by the given factor.
     * @param factor given factor
     */
    void multiplyValues( double factor );

    /**
     * @brief Returns string representation of the table.
     */
    std::string toString();

    /** @brief Assignment operator. */
    const Table1& operator= ( const Table1 &table );

    /** @brief Addition operator. */
    Table1 operator+ ( const Table1 &table ) const;

    /** @brief Multiplication operator (by scalar). */
    Table1 operator* ( double val ) const;

private:

    unsigned int _size;     ///< number of table elements

    double *_key_values;    ///< key values
    double *_table_data;    ///< table data

    double *_inter_data;    ///< interpolation data matrix

    /** Updates interpolation data due to table data. */
    void updateInterpolationData();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

/** @brief Multiplication operator (by scalar). */
inline fdm::Table1 operator* ( double val, const fdm::Table1 &table )
{
    return ( table * val );
}

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TABLE1_H
