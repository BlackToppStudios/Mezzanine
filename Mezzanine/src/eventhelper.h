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
#ifndef _eventhelper_h
#define _eventhelper_h

namespace Mezzanine
{
    /// @addtogroup Events
    /// @{

    /// @brief A simple collection of smaller tools event classes use.
    namespace EventHelper
    {
        /// @brief An empty template to be specialized from.
        template <class F>
        struct DeduceFunctTypes;

        /// @brief A struct designed to deduce the return type of a non-const member function.
        /// @tparam Return The return type of the function that is ultimately deduced.
        /// @tparam Object The object type holding the member function.
        /// @tparam Args A variadic type containing the arguments for the function.
        /// @pre Return has no expectations from this struct.
        /// @pre Object has no expectations from this struct.
        /// @pre Args has no expectations from this struct.
        template <class Return, class Object, class... Args>
        struct DeduceFunctTypes<Return(Object::*)(Args...)>
        {
            /// @brief The type of object the member function belongs to.
            using Obj_type = Object;
            /// @brief The type returned by the member function.
            using Ret_type = Return;
        };//DeduceFunctTypes

        /// @brief A struct designed to deduce the return type of a const member function.
        /// @tparam Return The return type of the function that is ultimately deduced.
        /// @tparam Object The object type holding the member function.
        /// @tparam Args A variadic type containing the arguments for the function.
        /// @pre Return has no expectations from this struct.
        /// @pre Object has no expectations from this struct.
        /// @pre Args has no expectations from this struct.
        template <class Return, class Object, class... Args>
        struct DeduceFunctTypes<Return(Object::*)(Args...) const>
        {
            /// @brief The type of object the member function belongs to.
            using Obj_type = Object;
            /// @brief The type returned by the member function.
            using Ret_type = Return;
        };//DeduceFunctTypes

        /// @brief Convenience type for retrieving the return type of a member function.
        template<typename T>
        using DeduceRetType = typename DeduceFunctTypes<T>::Ret_type;
        /// @brief Convenience type for retrieving the object type of a member function.
        template<typename T>
        using DeduceObjType = typename DeduceFunctTypes<T>::Obj_type;

        /// @brief One of two helper functions to ensure a type can be used as a pointer.
        /// @tparam AnyType The type that will be converted into a pointer to the same instance.  Can be anything.
        /// @pre AnyType does not have any requirements.
        /// @param ToConvert The reference to convert.
        /// @return Returns a pointer to the same instance passed into the function.
        template<class AnyType>
        AnyType* ToPointer(AnyType& ToConvert)
            { return &ToConvert; }
        /// @brief One of two helper functions to ensure a type can be used as a pointer.
        /// @tparam AnyType The type that will be immediately returned.  Can be anything.
        /// @pre AnyType does not have any requirements.
        /// @remarks This function doesn't do anything, but needs to exist to ensure the same semantic in the event templates.
        /// @param ToConvert The pointer to return.
        /// @return Returns a pointer to the same instance passed into the function.
        template<class AnyType>
        AnyType* ToPointer(AnyType* ToConvert)
            { return ToConvert; }
        /// @brief One of two helper functions to ensure a type can be used as a reference.
        /// @tparam AnyType The type that will be immediately returned.  Can be anything.
        /// @pre AnyType does not have any requirements.
        /// @remarks This function doesn't do anything, but needs to exist to ensure the same semantic in the event templates.
        /// @param ToConvert The reference to return.
        /// @return Returns a reference to the same instance passed into the function.
        template<class AnyType>
        AnyType& ToReference(AnyType& ToConvert)
            { return ToConvert; }
        /// @brief One of two helper functions to ensure a type can be used as a reference.
        /// @tparam AnyType The type that will be converted into a reference to the same instance.  Can be anything.
        /// @pre AnyType does not have any requirements.
        /// @param ToConvert The pointer to convert.
        /// @return Returns a reference to the same instance passed into the function.
        template<class AnyType>
        AnyType& ToReference(AnyType* ToConvert)
            { return *ToConvert; }
    }//EventHelper

    /// @}
}//Mezzanine

#endif // _eventhelper_h
