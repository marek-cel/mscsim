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
#ifndef G1000_MODELS_H
#define G1000_MODELS_H

////////////////////////////////////////////////////////////////////////////////

#include <vector>

#include <osg/Node>

#include <g1000/utils/g1000_Singleton.h>

////////////////////////////////////////////////////////////////////////////////

namespace g1000
{

/**
 * @brief 3D models container class.
 */
class Models : public Singleton< Models >
{
    friend class Singleton< Models >;

public:

    typedef std::vector< osg::ref_ptr<osg::Node> > List;

    /**
     * @param objectFile
     * @return
     */
    static osg::Node* get( const char *objectFile );

    /** Destructor. */
    virtual ~Models();

private:

    List _objects;                          ///< objects list
    std::vector< std::string > _fileNames;  ///< file names

    /**
     * You should use static function instance() due to get refernce
     * to Models class instance.
     */
    Models();

    /** Using this constructor is forbidden. */
    Models( const Models & ) : Singleton< Models >() {}
};

} // end of g1000 namespace

////////////////////////////////////////////////////////////////////////////////

#endif // G1000_MODELS_H
