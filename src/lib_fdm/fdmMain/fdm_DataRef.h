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
#ifndef DataRef_H
#define DataRef_H

////////////////////////////////////////////////////////////////////////////////

#include <limits>

#include <fdmMain/fdm_DataNode.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Data reference class.
 */
class FDMEXPORT DataRef
{
public:

    /** Constructor. */
    DataRef( DataNode *dataNode = 0 ) : m_dataNode( dataNode ) {}

    /** Copy constructor. */
    DataRef( const DataRef &dataRef )
    {
        m_dataNode = dataRef.m_dataNode;
    }

    /** Destructor. */
    virtual ~DataRef() {}

    /**
     * @return returns data value on success or NaN on failure
     */
    inline bool getDatab() const
    {
        if ( m_dataNode )
        {
            return m_dataNode->getDatab();
        }

        return std::numeric_limits< bool >::quiet_NaN();
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline int getDatai() const
    {
        if ( m_dataNode )
        {
            return m_dataNode->getDatai();
        }

        return std::numeric_limits< int >::quiet_NaN();
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline long getDatal() const
    {
        if ( m_dataNode )
        {
            return m_dataNode->getDatal();
        }

        return std::numeric_limits< long >::quiet_NaN();
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline float getDataf() const
    {
        if ( m_dataNode )
        {
            return m_dataNode->getDataf();
        }

        return std::numeric_limits< float >::quiet_NaN();
    }

    /**
     * @return returns data value on success or NaN on failure
     */
    inline double getDatad() const
    {
        if ( m_dataNode )
        {
            return m_dataNode->getDatad();
        }

        return std::numeric_limits< double >::quiet_NaN();
    }

    /**
     * @return data value on success or NaN on failure
     */
    inline double getValue() const
    {
        if ( m_dataNode )
        {
            return m_dataNode->getValue();
        }

        return std::numeric_limits< double >::quiet_NaN();
    }

    /**
     * @return TRUE if refference is valid, FALSE otherwise
     */
    inline bool isValid() const
    {
        return ( m_dataNode != 0 );
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatab( bool value )
    {
        if ( m_dataNode )
        {
            return m_dataNode->setDatab( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatai( int value )
    {
        if ( m_dataNode )
        {
            return m_dataNode->setDatai( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatal( long value )
    {
        if ( m_dataNode )
        {
            return m_dataNode->setDatal( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDataf( float value )
    {
        if ( m_dataNode )
        {
            return m_dataNode->setDataf( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setDatad( double value )
    {
        if ( m_dataNode )
        {
            return m_dataNode->setDatad( value );
        }

        return FDM_FAILURE;
    }

    /**
     * @return FDM_SUCCESS on success or FDM_FAILURE on failure
     */
    inline int setValue( double value )
    {
        if ( m_dataNode )
        {
            return m_dataNode->setValue( value );
        }

        return FDM_FAILURE;
    }

    /**
     * Resets data reference.
     */
    inline void reset() { m_dataNode = 0; }

private:

    DataNode *m_dataNode;   ///< data node pointer
};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // DataRef_H
