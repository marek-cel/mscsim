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
#ifndef COMBOUNITS_H
#define COMBOUNITS_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <QComboBox>

#include <defs.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Units selection abstract combo box.
 */
class ComboUnits : public QComboBox
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit ComboUnits( QWidget *parent = NULLPTR );
    
    /** @brief Destructor. */
    virtual ~ComboUnits();

    virtual double convert( double value ) const;
    virtual double convertPrev( double value ) const;

    virtual double invert( double value ) const;
    virtual double invertPrev( double value ) const;

    virtual double getCoef( int index ) const;

    inline double getFactor() const { return _factor; }

protected:

    double _factor;         ///< [-]
    double _factor_prev;    ///< [-]

    int _index;             ///<
    int _index_prev;        ///<

    std::vector<  float  > _coefs;  ///< [-] convertion factors
    std::vector< QString > _names;  ///< units names

public slots:

    /** */
    virtual void on_currentIndexChanged( int index );
};

////////////////////////////////////////////////////////////////////////////////

#endif // COMBOUNITS_H
