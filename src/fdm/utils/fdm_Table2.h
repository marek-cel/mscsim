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
#ifndef FDM_TABLE2_H
#define FDM_TABLE2_H

////////////////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

#include <fdm/fdm_Defines.h>

#include <fdm/utils/fdm_Table1.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief 2D table and bilinear interpolation class.
 */
class FDMEXPORT Table2
{
public:

    /**
     * @brief Creates table with only one record.
     * @param val record value
     */
    static Table2 createOneRecordTable( double val = 0.0 );

    /** @brief Constructor. */
    Table2();

    /** @brief Constructor. */
    Table2( const std::vector< double > &row_values,
            const std::vector< double > &col_values,
            const std::vector< double > &table_data );

    /** @brief Copy constructor. */
    Table2( const Table2 &table );

    /** @brief Destructor. */
    virtual ~Table2();

    inline unsigned int getCols() const { return _cols; }
    inline unsigned int getRows() const { return _rows; }

    /**
     * @brief Returns 1-dimensional table for the given col value.
     * @param colValue column key value
     * @return 1-dimensional table
     */
    Table1 getTable( double col_value ) const;

    /**
     * @brief Returns table value for the given keys.
     * Returns table value for the given keys values using bilinear
     * interpolation algorithm.
     * @param rowValue row key value
     * @param colValue column key value
     * @return interpolated value on success or NaN on failure
     */
    double getValue( double row_value, double col_value ) const;

    /**
     * @brief Returns table value for the given key index.
     * @param rowIndex row index
     * @param colIndex col index
     * @return value on success or NaN on failure
     */
    double getValueByIndex( unsigned int row_index, unsigned int col_index ) const;

    /**
     * @brief Checks if table is valid.
     * @return returns true if size is greater than 0 and all data is valid
     */
    bool isValid() const;

    /**
     * @brief Returns string representation of the table.
     */
    std::string toString();

    /** @brief Assignment operator. */
    const Table2& operator= ( const Table2 &table );

private:

    unsigned int _rows;     ///< number of rows
    unsigned int _cols;     ///< number of columns
    unsigned int _size;     ///< number of table elements

    double *_row_values;    ///< rows keys values
    double *_col_values;    ///< columns keys values
    double *_table_data;    ///< table data

    double *_inter_data;    ///< interpolation data matrix

    /** @brief Updates interpolation data due to table data. */
    void updateInterpolationData();
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_TABLE2_H
