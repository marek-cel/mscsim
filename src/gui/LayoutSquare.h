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
#ifndef LAYOUTSQUARE_H
#define LAYOUTSQUARE_H

////////////////////////////////////////////////////////////////////////////////

#include <QLayout>
#include <QLayoutItem>
#include <QPointer>
#include <QRect>
#include <QWidgetItem>

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief This class provides constant aspect ration (square) layout for
 * a single widget.
 */
class LayoutSquare : public QLayout
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit LayoutSquare( QWidget *parent, int spacing = -1 );
    
    /** @brief Constructor. */
    explicit LayoutSquare( int spacing = -1 );
    
    /** @brief Destructor. */
    virtual ~LayoutSquare();

    /** */
    void addItem( QLayoutItem *item );
    
    /** */
    void addWidget( QWidget *widget );
    
    /** */
    int count() const;
    
    /** */
    Qt::Orientations expandingDirections() const;

    /** */    
    QRect geometry();
    
    /** */
    bool hasHeightForWidth() const;
    
    /** */
    bool hasItem() const;

    /** */
    QLayoutItem* itemAt( int index ) const;

    /** */
    QSize minimumSize() const;

    /** */
    QLayoutItem* replaceItem( QLayoutItem *item );

    /** */
    void setGeometry( const QRect &rect );
    
    /** */
    QSize sizeHint() const;

    /** */
    QLayoutItem * take();
        
    /** */
    QLayoutItem * takeAt( int index );

private:

    QLayoutItem *_item;     ///< item reference

    QRect *_rectLast;       ///< last received geometry
    QRect *_geometry;       ///< geometry

    bool areRectsEqual( const QRect &rect_1, const QRect &rect_2 ) const;

    QPoint calculateCenterPnt( QSize fromSize, QSize itemSize ) const;

    QSize calculateProperSize( QSize fromSize ) const;

    void init( int spacing );
    
    void setRectLast( const QRect &rect );
};

////////////////////////////////////////////////////////////////////////////////

#endif // LAYOUTSQUARE_H
