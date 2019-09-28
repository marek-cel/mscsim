/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
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

#include "gui_Defines.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Units selection abstract combo box.
 */
class ComboUnits : public QComboBox
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit ComboUnits( QWidget *parent = NULLPTR );
    
    /** Destructor. */
    virtual ~ComboUnits();

    virtual float convert( float value ) const;
    virtual float convertPrev( float value ) const;

    virtual float invert( float value ) const;
    virtual float invertPrev( float value ) const;

    virtual float getCoef( int index ) const;
    
protected:

    float _factor;          ///< [-]
    float _factor_prev;     ///< [-]

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
