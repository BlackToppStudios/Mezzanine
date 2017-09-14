// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
    that running the build process creates. When running doxygen you might redirect the errors to a
    file and review the file to assist in troubleshooting syntax errors in documentation. Here is a
    simple way to make "results.txt" from the warnings and errors doxygen emits:

    @verbatim
    $ doxygen doxygen.config 2> results.txt
    @endverbatim

    Another way to get good visibility is to read the doxygen logfile, but is needs to be set in the
    doygen configuration. Use the "WARN_LOGFILE" directive to set this. You can also set the
    "WARN_FORMAT" directive to something your IDE understands and get warnings your IDE understands.
    With Qt Creator and Code::Blocks this allows a developer to click on the warnings in the build
    results panel or windows and have an editor windows show them the offending file and line. Here
    is the configuration required to do that:

    @verbatim
    WARN_FORMAT            = "$file:$line: $text"
    WARN_LOGFILE           = ../html/DoxygenWarnings.txt
    @endverbatim

    @section best_practices_doxygen_minimum D2 Minimum Amount to Document
    It is easy to document too much or too little. Too much and it will easily fall out of date when
    the code changes and developers ignore the wall of text. Too little and people using the code
    and documentation go WTF?! and use some other code or write from scratch.

    @n
    Here is what we do. We probably tend to go a little too heavily, but we are writing an API and
    we feel clarity is worth some maintanence cost.

    @subsection best_practices_doxygen_minimum_function Function/Methods Minimum Amount to Document
    Each function/method should get:

    - \@brief Every function and method should get one of these unless the \@return or \@param
    describe it so completely adding this would hurt. (leave \@brief off the int GetX() method on
    2D point class for example).
    - One \@param for each parameter describing its type, accepted values and any default values.
    - \@return Unless it returns void then the return tty and value shoud be documented.
    - \@throw If an exception is thrown, its type and conditions for causing the throw should be
    noted.
    - \@details or add \@remarks Should be added anytime the function has special preconditions.
    Changes state on a class that is not obvious or is otherwise complex.

    @n
    Here are some examples of Bad and Good doxygen comments on bad code. This code is terrible to
    highlight how much of a difference good and bad documentation can make. In a real situation it
    it likely that even good code with good variable names might still be complex enough to allow
    misconceptions in.

    @n
    Bad Dox:
    @code
    int sqrt(int x);

    /// @brief calculates absolute value
    int abs(int x);

    /// @param Accepts a float.
    /// @return Returns a float.
    float log(float x)
    @endcode

    Good Dox:
    @code
    /// @param x An Integer to take the square root of
    /// @return An int containing the square root of x. Any floating part is truncated
    int sqrt(int x);

    /// @brief Calculates absolute value, the distance from zero
    /// @param x An int that might be postive or negative.
    /// @return A positive int (or 0) that is the same as x but with an minus sign removed.
    int abs(int x);

    /// @brief Calculate the natural logorithm of a value.
    /// @param x A float to calculate the log2 of.
    /// @return Returns a floatX.
    float log(float x)
    @endcode

    @subsection best_practices_doxygen_minimum_enum Enum Minimum Amount to Document
    Most enums just need an \@brief and if the options are not self explanatory a using ///< seems
    to be the easiest way to document them. You don't need \@enum unless you are placing the dox
    far away from the enum. Here is an example:


    @n
    Bad Dox:
    @code
    /// @enum dir A direction
    enum dir
    {
        MN, MS, MW, ME,
        N, W, S, E
    };
    @endcode

    Good Dox:
    @code
    /// @brief A Compass direction
    enum dir
    {
        MN, ///< Magnetic North
        MS, ///< Magnetic South
        MW, ///< Magnetic West
        ME, ///< Magnetic East

        N, W, S, E
    };
    @endcode

    Good Code and Dox:
    @code
    /// @brief A Compass direction
    enum CompassDirection
    {
        MagneticNorth,  ///< Magnetic North
        MagneticSouth,  ///< Magnetic South
        MagneticWest,   ///< Magnetic West
        MagneticEast,   ///< Magnetic East

        North,          ///< Geographic North
        South,          ///< Geographic South
        West,           ///< Geographic West
        East            ///< Geographic East
    };
    @endcode


    @subsection best_practices_doxygen_minimum_class Class Minimum Amount to Document
    Most classes should have an \@brief providing a simple overview, an \@details providing
    additional rationle and perhaps a few examples in \@code/\@endcode or \@verbatim/\@endverbatim
    sections if their use is complicated. Use \@code when you want systax highlighting and a
    monospace font, use \@verbatim when you don't need syntax hightlighting. Since every method
    should be well named and well documented hopefully class documentation can be small.

    @n
    Try to document classes in the header file just ahead of the class keyword and \@class will not
    be needed.

    @n
    Example:
    @code
    /// @brief A 3x3 matrix intended for use in rotating 3d points.
    /// @details This supports normal math operations via operator overloads and is contructable
    /// to a unit matrix or accepts all 9 values in row major order:
    /// @verbatim
    /// Matrix UnitMatrix;
    /// Matrix OtherMatrix(1,2,3,4,5,6,7,8,9);
    /// Matrix Product = UnitMatrix * OtherMatrix;
    /// @endverbatim
    class Matrix
    {
    // Omitted
    @endcode

    @subsection best_practices_doxygen_minimum_file File Minimum Amount to Document
    All Files get an \@file and an \@brief describing a little more than the name of the file could
    desribe. This mostly provides a link in doxygen that might not otherwise have been generated.
    The \@file command accepts a filename, leave it out. If specified doxygen will interpret that
    block as documentation for some other file which might not existor otherwise break when the file
    is renamed.

    @n
    Example in foo.h:
    @code
        /// @file
        /// @brief This defines the class foo intended for use with @ref bar.
    @endcode

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
    /// @brief All lizards have 4 legs.
    /// @return An integer of value 4
    int Lizard::LegCount() const;

    /// @copydoc Lizard::LegCount() const
    int Cat::LegCount() const;
    @endcode

    @n
    Good:
    @code
    /// @return All Mammals have 4 limbs so this returns an integer of value 4.
    int Mammal::LegCount() const;

    /// @copydoc Mammal::LegCount() const
    int Cat::LegCount() const;
    @endcode


    @section best_practices_doxygen_identifier D4 Identifier Commands
    \@enum, \@typedef, \@class, and other specifiers of things to be documented should be used
    when documenting things out of line. If the documentation comments precede what you are
    documenting then these can be omitted. This makes documentation shorter and easier to maintain.

    @n
    Some of our older and more stale dox have some of these dangling from when we did not know
    better.

    @section best_practices_doxygen_ref D5 References
    Use \@ref often. Anytime you define a class, page, function, section or any other code or
    documentation structure you can refer to it with \@ref. If you simple put \@ref followed by the
    class, function, section or page name you will get a link with the text of the name text from
    the refferred item. If you want different text include link text in quotes. See some examples:

    To make:
    @n
    @ref Mezzanine::Vector3 and @ref best_practices_doxygen
    @n
    Use:
    @code
    @ref Mezzanine::Vector3 and @ref best_practices_doxygen
    @endcode

    @n
    To provide custom text like this:
    @n
    @ref Mezzanine::Vector3 "X/Y/Z Thingy" and
    @ref best_practices_doxygen  "Lexicographer's hate him"
    @n
    Use:
    @code
    @ref Mezzanine::Vector3 "X/Y/Z Thingy" and
    @ref best_practices_doxygen "Lexicographer's hate him"
    @endcode
*/

#endif
