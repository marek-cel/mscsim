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
#ifndef FDM_MAP_H
#define FDM_MAP_H

////////////////////////////////////////////////////////////////////////////////

#include <map>

#include <fdm/fdm_Defines.h>

////////////////////////////////////////////////////////////////////////////////

namespace fdm
{

/**
 * @brief Custom map-type container.
 */
template < typename TYPE_KEY, typename TYPE_ITEM >
class Map
{
public:

    typedef typename std::map< TYPE_KEY, TYPE_ITEM >::iterator iterator;

    /** @brief Constructor. */
    Map() {}

    /** @brief Copy constructor. */
    Map( const Map &map )
    {
        _map = map._map;
    }

    /**
     * @brief Adds item.
     * @param key
     * @param item
     * @return returns FDM_SUCCESS on success and FDM_FAILURE on failure
     */
    int addItem( TYPE_KEY key, TYPE_ITEM item )
    {
        std::pair< typename std::map< TYPE_KEY, TYPE_ITEM >::iterator, bool > temp =
                _map.insert( std::pair< TYPE_KEY, TYPE_ITEM >( key, item ) );

        if ( temp.second == true )
        {
            return FDM_SUCCESS;
        }

        return FDM_FAILURE;
    }

    /**
     * @brief Returns pinter of item by key value.
     * @param key
     * @return pinter of item or NULL
     */
    TYPE_ITEM* getItemByKey( TYPE_KEY key )
    {
        typename std::map< TYPE_KEY, TYPE_ITEM >::iterator it = _map.find( key );

        if ( it != _map.end() )
        {
            return &(it->second);
        }

        return FDM_NULLPTR;
    }


    inline iterator begin() { return _map.begin(); }
    inline iterator end()   { return _map.end();   }

private:

    std::map< std::string, TYPE_ITEM > _map;    ///< map

};

} // end of fdm namespace

////////////////////////////////////////////////////////////////////////////////

#endif // FDM_MAP_H
