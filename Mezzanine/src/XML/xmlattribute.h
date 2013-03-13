// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Pugixml parser - version 1.0
 * --------------------------------------------------------
 * Copyright © 2006-2012, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at http://pugixml.org/
 *
 * This library is distributed under the MIT License. See notice at the end
 * of this file.
 *
 * This work is based on the pugxml parser, which is:
 * Copyright © 2003, by Kristen Wegner (kristen@tima.net)
 */

#include "datatypes.h"

/// @file
/// @brief Contains the definition for the XML::Attribute class.

#ifndef _xmlattribute_h
#define _xmlattribute_h
namespace Mezzanine
{
    namespace XML
    {
        struct AttributeStruct;

        /// @brief A light-weight handle for manipulating attributes in DOM tree
        class MEZZ_LIB Attribute
        {
                friend class AttributeIterator;
                friend class Node;

            private:
                /// @internal
                /// @brief Stores pointers to the attribute data and some metadata about the attribute.
                AttributeStruct* _attr;

                #ifndef SWIG
                /// @brief Used to prevent casting to numerical types acccidentally.
                /// @note Not available in scripting languages because conversion is handled on a per langauge basis
                typedef void (*unspecified_bool_type)(Attribute***);
                #endif

            public:
                /// @brief Constructs an empty Attribute.
                Attribute();

                /// @brief Constructs attribute from internal pointer.
                /// @param attr An internal AttributeStruct pointer containing all the data to create an attribute.
                explicit Attribute(AttributeStruct* attr);

                #ifndef SWIG
                /// @brief Used to convert this to a boolean value in a safe way
                /// @return Returns true if the internal data is set and false otherwise.
                /// @note Not available in scripting languages because conversion is handled on a per langauge basis
                operator unspecified_bool_type() const;
                #endif

                /// @brief Used to convert this attribute the opposite of it's normal boolean value
                /// @details This is described in the PugiXML source a a workaround for a borland c++ issue.
                /// @return Returns false if the internal pointer AttributeStruct is set and true otherwise.
                bool operator!() const;

                /// @brief Compares the internal values to check equality.
                /// @param r The other @ref Attribute this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns true if all the internal values match between this and the other Attribute.
                bool operator==(const Attribute& r) const;

                /// @brief Compares the internal values to check inequality.
                /// @param r The other @ref Attribute this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns true if any of the internal values don't match between this and the other @ref Attribute.
                bool operator!=(const Attribute& r) const;

                /// @brief Compares the internal values to check for inequality.
                /// @param r The other @ref Attribute this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Attribute is greater than this one as per sequential comparison of internal pointers.
                bool operator<(const Attribute& r) const;

                /// @brief Compares the internal values to check for inequality.
                /// @param r The other @ref Attribute this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Attribute is less than this one as per sequential comparison of internal pointers.
                bool operator>(const Attribute& r) const;

                /// @brief Compares the internal values to check for inequality.
                /// @param r The other @ref Attribute this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Attribute is greater than or equal to this one as per sequential comparison of internal pointers.
                bool operator<=(const Attribute& r) const;

                /// @brief Compares the internal values to check for inequality.
                /// @param r The other @ref Attribute this is being compared to.
                /// @details Many of the internal values are pointers, and it is the addresses of these that are being compared.
                /// @return Returns True if the other @ref Attribute is less than or equal to this one as per sequential comparison of internal pointers.
                bool operator>=(const Attribute& r) const;

                /// @brief Is this storing anything at all?
                /// @return Returns True if this @ref Attribute is storing nothing. False if it is storing anything.
                bool Empty() const;

                /// @brief Get the name of this @ref Attribute.
                /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the name.
                /// @warning returns "" if attribute is empty.
                const Char8* Name() const;

                /// @brief Get the Value of this @ref Attribute.
                /// @return Returns A pointer to a const c-style array of the the character type (usually char or wchar_t) containing the value.
                /// @warning Returns "" if attribute is empty.
                const Char8* Value() const;

                /// @brief Attempts to convert the value of the attribute to a String and returns the results.
                /// @return If the value of this attribute can be converted to a Real by reading the character
                /// and interpretting them a number, that number is returned. Returns 0 on failure.
                /// @exception This can throw exception in certain overflow conditions
                const Char8* AsString(const Char8* def = "") const;

                /// @brief Attempts to convert the value of the attribute to an int and returns the results.
                /// @return If the value of this attribute can be convert to an int by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
                /// @todo Update Attribute::AsInt() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
                /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and int. Check "errno" and see if it is set to "ERANGE" to test for this condition.
                int AsInt(int def = 0) const;

                /// @brief Attempts to convert the value of the attribute to an unsigned int and returns the results.
                /// @return If the value of this attribute can be convert to an unsigned int by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
                /// @todo Update Attribute::AsUint() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
                /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and int. Check "errno" and see if it is set to "ERANGE" to test for this condition.
                unsigned int AsUint(unsigned int def = 0) const;

                /// @brief Attempts to convert the value of the attribute to a double and returns the results.
                /// @return If the value of this attribute can be convert to a double by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
                /// @todo Update Attribute::AsDouble() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
                /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and double. Check "errno" and see if it is set to "ERANGE" to test for this condition.
                double AsDouble(double def = 0) const;

                /// @brief Attempts to convert the value of the attribute to a float and returns the results.
                /// @return If the value of this attribute can be convert to a float by reading the character and interpretting them a number, that numberis returned. Returns 0 on failure.
                /// @todo Update Attribute::AsFloat() to check errno and throw exceptions were appropriate, and throw a exception on failure instead of producing a valid return value.
                /// @warning This may silently fail if the value of the attribute exceeds the maximum value that can be stored in and float. Check "errno" and see if it is set to "ERANGE" to test for this condition.
                float AsFloat(float def = 0) const;

                /// @brief Attempts to convert the value of the attribute to a Real and returns the results.
                /// @return If the value of this attribute can be converted to a Real by reading the character
                /// and interpretting them a number, that number is returned. Returns 0 on failure.
                /// @exception This can throw exception in certain overflow conditions
                Real AsReal(Real def = 0) const;

                /// @brief Attempts to convert the value of the attribute to a Whole and returns the results.
                /// @return If the value of this attribute can be converted to a Whole by reading the character
                /// and interpretting them a number, that number is returned. Returns 0 on failure.
                /// @exception This can throw exception in certain overflow conditions
                Whole AsWhole(Whole def = 0) const;

                /// @brief Attempts to convert the value of the attribute to a Integer and returns the results.
                /// @return If the value of this attribute can be converted to a Integer by reading the character
                /// and interpretting them a number, that number is returned. Returns 0 on failure.
                /// @exception This can throw exception in certain overflow conditions
                Integer AsInteger(Integer def = 0) const;

                /// @brief Attempts to convert the value of the attribute to a float and returns the results.
                /// @return Value as bool (returns true if first character is in '1tTyY' set), or false if attribute is empty
                bool AsBool(bool def = false) const;

                #ifndef SWIG
                /// @brief Set the name of .
                /// @param rhs The desired name.
                /// @return True if successful, returns false if the name cannot be stored or there is not enough memory.
                /// @note Not available in scripting languages the Overload which accepts a Mezzanine::String is used instead, for enhanced robustness in the event corrupt String are created.
                bool SetName(const Char8* rhs);
                #endif

                /// @brief Set the name of this object
                /// @param rhs The desired name .
                /// @return True if successful, returns false if the name cannot be stored or there is not enough memory.
                /// @note This overload is used in scripting languages.
                bool SetName(const String& rhs)
                    { return SetName(rhs.c_str()); }

                #ifndef SWIG
                /// @brief Set the value of this.
                /// @param rhs The new Value.
                /// @return True if successful, returns false if this is empty or there is not enough memory.
                /// @todo update this to make the error return code redundant and use an exception instead.
                /// @todo Review for possiblity of buffer overflow.
                /// @note Not available in scripting languages the Overload which accepts a Mezzanine::String is used instead, for enhanced robustness in the event corrupt String are created.
                bool SetValue(const Char8* rhs);

                // Set GetAttribute Value with Type conversion (numbers are converted to strings, boolean is converted to "true"/"false")
                /// @brief Convert rhs to a character array that contains rhs, then use that as the new value.
                /// @param rhs The new value as an int.
                /// @return True if successful, returns false if Attribute is empty or there is not enough memory.
                /// @todo update this to make the error return code redundant and use an exception instead.
                /// @todo Review for possiblity of buffer overflow.
                /// @note Not available in scripting languages the Overload which accepts a Mezzanine::String is used instead.
                bool SetValue(int rhs);

                /// @brief Convert rhs to a character array that contains rhs, then use that as the new value.
                /// @param rhs The new value as an unsigned int.
                /// @return True if successful, returns false if Attribute is empty or there is not enough memory.
                /// @todo update this to make the error return code redundant and use an exception instead.
                /// @todo Review for possiblity of buffer overflow.
                /// @note Not available in scripting languages the Overload which accepts a Mezzanine::String is used instead.
                bool SetValue(unsigned int rhs);

                /// @brief Convert rhs to a character array that contains rhs, then use that as the new value.
                /// @param rhs The new value as a double.
                /// @return True if successful, returns false if Attribute is empty or there is not enough memory.
                /// @todo update this to make the error return code redundant and use an exception instead.
                /// @todo Review for possiblity of buffer overflow.
                /// @note Not available in scripting languages the Overload which accepts a Mezzanine::String.
                bool SetValue(double rhs);

                /// @brief Convert rhs to a character array that contains the meaning of rhs, then use that as the new value.
                /// @param rhs This with be interpretted, then converted to "true" or "false"  and used as the new value.
                /// @return True if successful, returns false if Attribute is empty or there is not enough memory.
                /// @todo update this to make the error return code redundant and use an exception instead.
                /// @todo Review for possiblity of buffer overflow.
                /// @note Not available in scripting languages the Overload which accepts a Mezzanine::String is used instead.
                bool SetValue(bool rhs);

                /// @brief Convert rhs to a character array that contains the meaning of rhs, then use that as the new value.
                /// @param rhs This with be converted to a character array using the appropriate streaming operator <<, then used as the new value.
                /// @return True if successful, returns false if Attribute is empty or there is not enough memory.
                /// @warning You should not pass classes that stream/serialize to xml into this function, the result will be invalid XML. If you must, find a way to strip out the ">" character, then you can reinsert it later
                /// @todo Strip ">" automatically and provide a method to reconsitute it.
                template <class T> bool SetValue(T rhs)
                    { return SetValue(ToString(rhs).c_str()); }
                #endif

                /// @brief Set the value of this.
                /// @param rhs The new Value.
                /// @return True if successful, returns false if this is empty or there is not enough memory.
                /// @todo update this to make the error return code redundant and use an exception instead.
                /// @note This overload is used in scripting languages.
                bool SetValue(const String& rhs)
                    { return SetValue(rhs.c_str()); }



                /// @param rhs The new value as an c-style string.
                /// @brief The same as @ref Attribute::SetValue(); without the error return
                /// @return An reference to this attribute.
                Attribute& operator=(const Char8* rhs);

                /// @param rhs The new value as an int.
                /// @brief The same as @ref Attribute::SetValue(); without the error return.
                /// @return An reference to this attribute.
                Attribute& operator=(int rhs);

                /// @param rhs The new value as an unsigned int.
                /// @brief The same as @ref Attribute::SetValue(); without the error return.
                /// @return An reference to this attribute.
                Attribute& operator=(unsigned int rhs);

                /// @brief The same as @ref Attribute::SetValue(); without the error return.
                /// @param rhs The new value as a double.
                /// @return An reference to this attribute.
                Attribute& operator=(double rhs);

                /// @brief The same as @ref Attribute::SetValue(); without the error return.
                /// @param rhs This with be interpretted, then converted to "true" or "false" and used as the new value.
                /// @return An reference to this attribute.
                Attribute& operator=(bool rhs);

                /// @brief Get the next attribute.
                /// @details This will get the next sibling attribute. That is, another Attribute on the same node as this attribute. This is internally a circular linked list, so once you reach the end, you simply be given the first node. If this attribute is empty this will return a empty attribute.
                /// @return Either the next attribute or if this attribute is empty an empty attribute.
                Attribute GetNextAttribute() const;

                /// @brief Get the previous attribute.
                /// @details This will get the previous sibling attribute. That is, another Attribute on the same node as this attribute. This is internally a circular linked list, so once you reach the beginning, you simply be given the last node. If this attribute is empty this will return a empty attribute.
                /// @return Either the previous attribute or if this attribute is empty an empty attribute.
                Attribute GetPreviousAttribute() const;

                /// @brief Get a unique identifying value for the Attribute this represents
                /// @return A size_t that is unique per Attribute that an attribute could represent.
                size_t HashValue() const;

                /// @brief Retrieve a pointer to the internal data.
                /// @return A void pointer to the internal data.
                /// @internal
                AttributeStruct* InternalObject() const;
        };


    }
}
#endif

/*
 *
 * Software, Files, Libraries and all other items referenced in this clause refers only
 * to the contents of this file and associated documentation.
 *
 * Copyright © 2006-2012 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
