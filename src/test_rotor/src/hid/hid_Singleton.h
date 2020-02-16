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
#ifndef HID_SINGLETON_H
#define HID_SINGLETON_H

////////////////////////////////////////////////////////////////////////////////

namespace hid
{

/** Singleton template class. */
template < class TYPE >
class Singleton
{
public:

    /** Instace accessor. */
    inline static TYPE* instance()
    {
        if ( !m_instance )
        {
            m_instance = new TYPE();
        }

        return m_instance;
    }

public:

    /** Destructor. */
    virtual ~Singleton() {}

protected:

    /** Constructor. */
    Singleton() {}

    /** Constructor. */
    Singleton( const Singleton & ) {}

private:

    static TYPE *m_instance;    ///< instance
};

} // end of hid namespace

////////////////////////////////////////////////////////////////////////////////

template < class TYPE > TYPE* hid::Singleton< TYPE >::m_instance = 0;

////////////////////////////////////////////////////////////////////////////////

#endif // HID_SINGLETON_H
