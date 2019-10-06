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

#include <gui/LayoutSquare.h>

#include <gui/gui_Defines.h>

////////////////////////////////////////////////////////////////////////////////

LayoutSquare::LayoutSquare( QWidget * parent, int spacing ) :
    QLayout ( parent ),

    _item ( NULLPTR ),
    _rectLast ( NULLPTR ),
    _geometry ( NULLPTR )
{
    init( spacing );
}

////////////////////////////////////////////////////////////////////////////////

LayoutSquare::LayoutSquare( int spacing ) :
    QLayout ( NULLPTR ),

    _item ( NULLPTR ),
    _rectLast ( NULLPTR ),
    _geometry ( NULLPTR )
{
    init( spacing );
}

////////////////////////////////////////////////////////////////////////////////

LayoutSquare::~LayoutSquare()
{
    DELPTR( _item );
    DELPTR( _rectLast );
    DELPTR( _geometry );
}

////////////////////////////////////////////////////////////////////////////////

void LayoutSquare::addItem( QLayoutItem * item )
{
    if ( !hasItem() ) replaceItem( item );
}

////////////////////////////////////////////////////////////////////////////////

void LayoutSquare::addWidget( QWidget * widget )
{
    if ( !hasItem() ) replaceItem( new QWidgetItem( widget ) );
}

////////////////////////////////////////////////////////////////////////////////

int LayoutSquare::count() const
{
    if ( hasItem() ) return 1;
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

Qt::Orientations LayoutSquare::expandingDirections() const
{
    return ( Qt::Horizontal | Qt::Vertical );
}

////////////////////////////////////////////////////////////////////////////////

QRect LayoutSquare::geometry()
{
    return (QRect)(*_geometry);
}

////////////////////////////////////////////////////////////////////////////////

bool LayoutSquare::hasHeightForWidth() const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////

bool LayoutSquare::hasItem() const
{
    return ( _item != 0 );
}

////////////////////////////////////////////////////////////////////////////////

QLayoutItem* LayoutSquare::itemAt( int index ) const
{
    if ( index == 0 )
    {
        if ( hasItem() ) return _item;
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

QSize LayoutSquare::minimumSize() const
{
    return _item->minimumSize();
}

////////////////////////////////////////////////////////////////////////////////

QLayoutItem* LayoutSquare::replaceItem( QLayoutItem * item )
{
    QLayoutItem * tempItem = 0;
    
    if ( hasItem() ) tempItem = _item;
    
    _item = item;
    
    setGeometry( *_geometry );
    
    return tempItem;
}

////////////////////////////////////////////////////////////////////////////////

void LayoutSquare::setGeometry( const QRect & rect )
{

    if ( !hasItem() || areRectsEqual( *_rectLast, rect ) ) return;
    
    setRectLast( rect );
    
    QSize  properSize  = calculateProperSize( rect.size() );
    QPoint centerPoint = calculateCenterPnt( rect.size(), properSize );
    
    _item->setGeometry( QRect( centerPoint, properSize ) );
    
    QRect *tempRect = _geometry;
    
    _geometry = new QRect( centerPoint, properSize );
    
    delete tempRect;

    ///////////////////////////////////
    QLayout::setGeometry( *_geometry );
    ///////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////

QSize LayoutSquare::sizeHint() const
{
    return _item->minimumSize();
}

////////////////////////////////////////////////////////////////////////////////

QLayoutItem* LayoutSquare::take()
{
    QLayoutItem * tempItem = 0;

    if ( hasItem() )
    {
        tempItem = _item;
        _item = 0;
    }
    
    return tempItem;
}

////////////////////////////////////////////////////////////////////////////////

QLayoutItem* LayoutSquare::takeAt( int index )
{
    if( index == 0 ) return take();
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

bool LayoutSquare::areRectsEqual( const QRect & rect_1, const QRect & rect_2 ) const
{
    bool result = false;

    if (    ( rect_1.x()      == rect_2.x()      )
         && ( rect_1.y()      == rect_2.y()      )
         && ( rect_1.height() == rect_2.height() )
         && ( rect_1.width()  == rect_2.width()  ) )
    {
        result = true;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////

QPoint LayoutSquare::calculateCenterPnt( QSize fromSize, QSize itemSize ) const
{
    QPoint centerPoint;
    
    if ( ( fromSize.width() - fromSize.width() / 2.0 - itemSize.width() / 2.0 ) > 0.0 )
    {
        centerPoint.setX( fromSize.width() - fromSize.width() / 2.0 - itemSize.width() / 2.0 );
    }
    
    if ( ( fromSize.height() - fromSize.height() / 2.0 - itemSize.height() / 2.0 ) > 0.0 )
    {
        centerPoint.setY( fromSize.height() - fromSize.height() / 2.0 - itemSize.height() / 2.0 );
    }
    
    return centerPoint;
}

////////////////////////////////////////////////////////////////////////////////

QSize LayoutSquare::calculateProperSize( QSize fromSize ) const
{
    QSize resultSize;
    
    if ( fromSize.height() < fromSize.width() )
    {
        resultSize.setHeight ( fromSize.height() - margin() );
        resultSize.setWidth  ( fromSize.height() - margin() );
    }
    else
    {
        resultSize.setHeight ( fromSize.width() - margin() );
        resultSize.setWidth  ( fromSize.width() - margin() );
    }
    
    return resultSize;
}

////////////////////////////////////////////////////////////////////////////////

void LayoutSquare::init( int spacing )
{
    _item = 0;
    
    _rectLast = new QRect( 0, 0, 0, 0 );
    _geometry = new QRect( 0, 0, 0, 0 );
    
    setSpacing( spacing );
}

////////////////////////////////////////////////////////////////////////////////

void LayoutSquare::setRectLast( const QRect & rect )
{
    QRect *tempRect = _rectLast;
    
    _rectLast = new QRect( rect.topLeft(), rect.size() );

    delete tempRect;
}
