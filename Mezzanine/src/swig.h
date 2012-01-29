//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _swig_h
#define _swig_h

///////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Used to give commands specifically to the swig preprocessor
/// @details This file exists primarily to make it easier for engine users to include
/// parts of the Mezzanine engine in their project with out needing to know or
/// care about the internals of our project.
///////////////////////////////////////////////////////////////////////////////

// Tell SWIG to create a module that scripting languages can use called "mezzanine"
#ifdef SWIG
    %module mezzanine


    // duplicate the std classes, because the std headers can be very difficult to parse.
    namespace std
    {
        class exception
        {
            public:
                exception();
                exception(const exception& rhs);
                virtual ~exception();
                virtual const char *what(void);
        };

        //template < class T, class Allocator = allocator<T> > class vector
        template < class T > class vector
        {
            public:
                explicit vector ( const Allocator& = Allocator() );
                explicit vector ( size_type n, const T& value= T(), const Allocator& = Allocator() );
                template <class InputIterator> vector ( InputIterator first, InputIterator last, const Allocator& = Allocator() );
                vector ( const vector<T,Allocator>& x );

                ~vector ( );

                vector<T,Allocator>& operator= (const vector<T,Allocator>& x);

        };

    }



#endif

// #define DISABLESWIG


#endif
