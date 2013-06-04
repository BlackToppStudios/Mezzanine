//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _lua51scriptargument_h
#define _lua51scriptargument_h

#include "datatypes.h"

#ifdef MEZZLUA51

#include "Scripting/scriptargument.h"
#include "Scripting/script.h"
#include "Scripting/scriptingmanager.h"

/// @file
/// @brief This file has the definition of the Script Arguments for Lua 51

namespace Mezzanine
{
    namespace Scripting
    {
        namespace Lua
        {
            /// @brief No special care is required for Integer Lua Arguments, so a simple typedef is used.
            typedef ScriptArgumentGeneric<Integer> Lua51IntegerArgument;
            /// @brief Lua51IntegerArgument returns this value when checking GetTypeData() const.
            const Integer LuaInteger = GenericInteger;

            /// @brief No special care is required for Real number Lua Arguments, so a simple typedef is used.
            typedef ScriptArgumentGeneric<Real> Lua51RealArgument;
            /// @brief Lua51RealArgument returns this value when checking GetTypeData() const.
            const Integer LuaReal = GenericReal;

            /// @brief No special care is required for Whole number Lua Arguments, so a simple typedef is used.
            typedef ScriptArgumentGeneric<Whole> Lua51WholeArgument;
            /// @brief Lua51WholeArgument returns this value when checking GetTypeData() const.
            const Integer LuaWhole = GenericWhole;

            /// @brief No special care is required for String Lua Arguments, so a simple typedef is used.
            typedef ScriptArgumentGeneric<String> Lua51StringArgument;
            /// @brief Lua51StringArgument returns this value when checking GetTypeData() const.
            const Integer LuaString = GenericString;

            /// @brief No special care is required for Bool Lua Arguments, so a simple typedef is used.
            typedef ScriptArgumentGeneric<Bool> Lua51BoolArgument;
            /// @brief Lua51BoolArgument returns this value when checking GetTypeData() const.
            const Integer LuaBool = GenericBool;

            /// @brief A very simple class to allow a specialization of ScriptArgumentGeneric to correspond with Lua's Nil.
            class Lua51Nil {};
            /// @brief Lua51NilArgument returns this value when checking GetTypeData() const.
            const Integer LuaNil = GenericMax+1;
        } // Lua

        /// @brief A Lua51Nil implementation of a ScriptArgument that is suitable for only for passing Nil into Lua51 scripts
        template <>
        class MEZZ_LIB ScriptArgumentGeneric<Mezzanine::Scripting::Lua::Lua51Nil> : public iScriptArgument
        {
            private:
                /// @brief For propriety here is an actual Lua51Nil, it does nothing and will likely be optimized out by any sane compiler
                Mezzanine::Scripting::Lua::Lua51Nil Datum;

            public:
                /// @brief To make working with this easier.
                typedef Mezzanine::Scripting::Lua::Lua51Nil Type;

                /// @brief Create a Nil Argument
                ScriptArgumentGeneric()
                    {}

                /// @brief Overloadable Deconstructor
                virtual ~ScriptArgumentGeneric()
                    {}

                /// @brief Get "Nil".
                /// @return A string containing "Nil"
                virtual String GetString() const
                    { return String("Nil"); }

                /// @brief Get Nil as a Whole
                /// @return 0 because NaN does not work in C++
                virtual Whole GetWhole() const
                    { return 0; }

                /// @brief Get 0.
                /// @return 0 because NaN does not work in C++
                virtual Integer GetInteger() const
                    { return 0; }

                /// @brief Get the Nil as a 0.0.
                /// @return 0.0 because NaN does not work in C++
                virtual Real GetReal() const
                    { return 0.0; }

                /// @brief Get the Nil as a bool
                /// @return false.
                virtual Bool GetBool() const
                    { return false; }

                /// @brief Provide an overload point to change assignment that operators will use.
                /// @param NewValue The new value for this.
                virtual void SetValue(Mezzanine::Scripting::Lua::Lua51Nil NewValue)
                    { Datum=NewValue; }

                /// @brief Get the raw primitive to value.
                /// @return The internal value that meaningful operations can be performed on.
                virtual Mezzanine::Scripting::Lua::Lua51Nil GetValue() const
                    { return Datum; }

                /// @brief Get data about this being a LuaNil
                /// @return This will return an Integer containing LuaNil
                virtual Integer GetTypeData() const
                    { return Mezzanine::Scripting::Lua::LuaNil; }
        }; //ScriptArgumentSpecific<Lua51Nil>

        namespace Lua
        {
            /// @brief No special care is required for Bool Lua Arguments, so a simple typedef is used.
            typedef ScriptArgumentGeneric<Lua51Nil> Lua51NilArgument;

        } // Lua
    } // Scripting


} // Mezzanine


#endif // MEZZLUA51
#endif // \_scriptinglua_h
