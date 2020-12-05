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
#ifndef DataRef_H
#define DataRef_H

////////////////////////////////////////////////////////////////////////////////

#include <limits>

#include <fdm/utils/fdm_DataNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Data reference class.
 */
class FDMEXPORT DataRef
{
public:

    /** @brief Constructor. */
    DataRef( DataNode *dataNode = 0 ) : _dataNode( dataNode ) {}

    /** @brief Copy constructor. */
    DataRef( const DataRef &dataRef )
    {
        _dataNode = dataRef._dataNode;
    }

    /** @brief Destructor. */
    virtual ~DataRef() {}

    /**
     * @return returns data value on success or NaN on failure
     */
    inline bool getDatab( bool def = false ) const
    {
        if ( _dataNode )
        {
            return _dataNode->getDatab();
        }

        return def;
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline int getDatai( int def = std::numeric_limits< int >::quiet_NaN() ) const
    {
        if ( _dataNode )
        {
            return _dataNode->getDatai();
        }

        return def;
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline long getDatal( long def = std::numeric_limits< long >::quiet_NaN() ) const
    {
        if ( _dataNode )
        {
            return _dataNode->getDatal();
        }

        return def;
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline float getDataf( float def = std::numeric_limits< float >::quiet_NaN() ) const
    {
        if ( _dataNode )
        {
            return _dataNode->getDataf();
        }

        return def;
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline double getDatad( double def = std::numeric_limits< double >::quiet_NaN() ) const
    {
        if ( _dataNode )
        {
            return _dataNode->getDatad();
        }

        return def;
    }

    /**
     * @return data value on success or NaN on failure
     */
    inline double getValue( double def = std::numeric_limits< double >::quiet_NaN() ) const
    {
        if ( _dataNode )
        {
            return _dataNode->getValue();
        }

        return def;
    }

    /**
     * @return TRUE if refference is valid, FALSE otherwise
     */
    inline bool isValid() const
    {
        return ( _dataNode != 0 );
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatab( bool value )
    {
        if ( _dataNode )
        {
            return _dataNode->setDatab( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatai( int value )
    {
        if ( _dataNode )
        {
            return _dataNode->setDatai( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatal( long value )
    {
        if ( _dataNode )
        {
            return _dataNode->setDatal( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDataf( float value )
    {
        if ( _dataNode )
        {
            return _dataNode->setDataf( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatad( double value )
    {
        if ( _dataNode )
        {
            return _dataNode->setDatad( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setValue( double value )
    {
        if ( _dataNode )
        {
            return _dataNode->setValue( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @brief Resets data reference.
     */
    inline void reset() { _dataNode = 0; }

    /**
     * @brief Assignment operator.
     */
    const DataRef& operator= ( const DataRef &dataRef )
    {
        _dataNode = dataRef._dataNode;
        return (*this);
    }

private:

    DataNode *_dataNode;    ///< data node pointer
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // DataRef_H
