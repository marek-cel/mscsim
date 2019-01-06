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

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Units selection abstract combo box.
 */
class ComboUnits : public QComboBox
{
    Q_OBJECT

public:

    /** Constructor. */
    explicit ComboUnits( QWidget *parent = 0 );
    
    /** Destructor. */
    virtual ~ComboUnits();

    virtual float convert( float value );
    virtual float convertPrev( float value );

    virtual float invert( float value );
    virtual float invertPrev( float value );
    
protected:

    float m_factor;         ///< [-]
    float m_factor_prev;    ///< [-]

    int m_index;
    int m_index_prev;

    std::vector<  float  > m_coefs;   ///< [-] convertion factors
    std::vector< QString > m_names;   ///< units names

public slots:

    /** */
    virtual void on_currentIndexChanged( int index );
};

////////////////////////////////////////////////////////////////////////////////

#endif // COMBOUNITS_H
