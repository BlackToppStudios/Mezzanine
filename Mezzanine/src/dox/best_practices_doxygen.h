// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef best_practices_doxygen_h
#define best_practices_doxygen_h

/**
    @page best_practices_doxygen Best Practices – Doxygen
    Doxygen is a tool for extracting API documentation from source code. Doxygen reads source code
    the same way a C, C++ or Java compiler would, but can also read special comments to include
    extra information. This page is about making the most of doxygen by entering high quality
    information. If you want to learn how to run doxygen see the page on @ref running_doxygen or if
    you want to know how to install it see the appropriate page about our @ref dev_tools_menu.

    @section best_practices_doxygen_visibility D1 Improve Errors visibilty during builds
    Errors can't be fixed if you cannot see them. Try to use tools that will show you the errors
    that running the build process creates. Using.... describe more about IDEs here.


    @section best_practices_doxygen_minimum D2 Minimum Amount to Document
    decribesom stuff.


    @subsection best_practices_doxygen_minimum_function Function/Methods Minimum Amount to Document
    Each function/method should get:

    - \@brief Every function and method should get one of these unless the \@return or \@param
    describe it so completely adding this would hurt. (leave \@brief off the int GetX() method on
    2D point class for example).
    - One \@param for each parameter describing its type, accepted values and any default values.
    - \@return Unless it returns void then the return tty and value shoud be documented.
    - \@throw If an exception is thrown, its type and conditions for causing the throw should be
    noted.
    - \@details Should be added anytime


    Add more describing how to document Enums, classes, files




    @section best_practices_doxygen_copydoc D3 \@copydoc Should Follow inheritance
    The \@copydoc directive should primarily be used by following up inheritance heiarchies. When
    used to in other ways it can creates logical dependencines in the documentation that do not
    match the code which increases future maintanence effort. Consider the follow inheritance tree
    of classes:

    @dot Sample inheritance tree
    digraph interop {
        Animal [shape=box];
        Mammal [shape=box];
        Reptile [shape=box];
        Cat [shape=box];
        Dog [shape=box];
        Lizard [shape=box];
        Animal -> Mammal -> Cat;
        Mammal -> Dog;
        Animal -> Reptile -> Lizard;
    }
    @enddot

    @n
    Because the Lizard and the Cat have little to do with each other in code they will change in
    different in the future. Even if they each have an int LegCount() const member function that
    return 4, they should not copy documentation from eachother. Someone updating the lizard to have
    6 legs is unlikely to be concerned the Cat class.

    @n
    A more reasonable solution is to have Lizard copy documentation from Reptile and Cat from
    Mammal. This makes it easy for someone changing code and documentation to know what other
    documentation should be updated. Following a rule like this also implies that someone changing
    a class has some responsibility to verify the documentation in each derived class.

    @n
    Bad:
    @code
    /// \@brief All lizards have 4 legs.
    /// \@return An integer of value 4
    int Lizard::LegCount() const;

    /// \@copydoc Lizard::LegCount() const
    int Cat::LegCount() const;
    @endcode

    @n
    Good:
    @code
    /// \@return All Mammals have 4 limbs so this returns an integer of value 4.
    int Mammal::LegCount() const;

    /// \@copydoc Mammal::LegCount() const
    int Cat::LegCount() const;
    @endcode


    @section best_practices_doxygen_identifier D4 Identifier Commands
    \@enum, \@typedef, \@class, and other specifiers of things to be documented should be used
    when documenting things out of line. If the documentation comments


*/

#endif
