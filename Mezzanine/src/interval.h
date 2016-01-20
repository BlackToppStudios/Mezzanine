// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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

#ifndef _interval_h
#define _interval_h

#include "exception.h"
#include "serialization.h"
#include "XML/xml.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @class Interval
    /// @brief This class will generate keep track of a pool of unique 32-bit ID's that can be used for distinct object instances.
    ///////////////////////////////////////
    template<class NumType>
    class MEZZ_LIB Interval
    {
    public:
        /// @brief Convenience type for the implemented type.
        typedef Interval<NumType> SelfType;

        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief The lower numeric boundry of the interval.
        NumType LowerBound;
        /// @brief The upper numeric boundry of the interval.
        NumType UpperBound;

        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Blank constructor.
        Interval() :
            LowerBound(0),
            UpperBound(0)
            {  }
        /// @brief Copy constructor.
        /// @param Other The other Interval to be copied.
        Interval(const SelfType& Other) :
            LowerBound(Other.LowerBound),
            UpperBound(Other.UpperBound)
            {  }
        /// @brief Descriptive constructor.
        /// @param Lower The lower numeric boundry of the interval.
        /// @param Upper The upper numeric boundry of the interval.
        Interval(const NumType& Lower, const NumType& Upper) :
            LowerBound(Lower),
            UpperBound(Upper)
            {  }
        /// @brief Class destructor.
        ~Interval()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Sets both values of this Interval.
        /// @param Lower The lower numeric boundry of the interval.
        /// @param Upper The upper numeric boundry of the interval.
        void SetBounds(const NumType& Lower, const NumType& Upper)
        {
            this->LowerBound = Lower;
            this->UpperBound = Upper;
        }

        /// @brief Gets whether or not a number is inside the bounds of this Interval.
        /// @param Num The number to check against this Interval.
        /// @return Returns true if the provided number is within the bounds of this interval, false otherwise.
        Boole IsWithinBounds(const NumType& Num) const
            { return this->LowerBound <= Num && this->UpperBound >= Num; }

        /// @brief Gets the lower boundry of this Interval.
        /// @return Returns a reference to the lower boundry of this Interval.
        NumType& GetLowerBound()
            { return this->LowerBound; }
        /// @brief Gets the lower boundry of this Interval.
        /// @return Returns a const reference to the lower boundry of this Interval.
        const NumType& GetLowerBound() const
            { return this->LowerBound; }
        /// @brief Gets the upper boundry of this Interval.
        /// @return Returns a reference to the upper boundry of this Interval.
        NumType& GetUpperBound()
            { return this->UpperBound; }
        /// @brief Gets the upper boundry of this Interval.
        /// @return Returns a const reference to the upper boundry of this Interval.
        const NumType& GetUpperBound() const
            { return this->UpperBound; }

        /// @brief Gets the numeric distance between the lower and upper bounds of this interval.
        /// @return Returns a NumType representing the difference between both bounds of this interval.
        NumType GetIntervalSize() const
            { return this->UpperBound - this->LowerBound; }

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Assignment operator.
        /// @param Other The other Interval to being assigned to this.
        /// @return Returns a reference to this.
        Interval& operator=(const SelfType& Other)
        {
            this->LowerBound = Other.LowerBound;
            this->UpperBound = Other.UpperBound;
            return *this;
        }

        /// @brief Equality comparison operator.
        /// @param Other The other Interval to be compared with.
        /// @return Returns true if both Intervals are the same, false otherwise.
        Boole operator==(const SelfType& Other) const
            { return this->LowerBound == Other.LowerBound && this->UpperBound == Other.UpperBound; }
        /// @brief Inequality comparison operator.
        /// @param Other The other Interval to be compared with.
        /// @return Returns true if both Intervals are not equal, false otherwise.
        Boole operator!=(const SelfType& Other) const
            { return this->LowerBound != Other.LowerBound || this->UpperBound != Other.UpperBound; }

        /// @brief Less-than comparison with NumType.
        /// @param Num The number to be compared with.
        /// @return Returns true if the highest bounds of this Interval is less than the specified number.
        Boole operator<(const NumType& Num) const
            { return this->UpperBound < Num; }
        /// @brief Greater-than comparison with NumType.
        /// @param Num The number to be compared with.
        /// @return Returns true if the lowest bounds of this Interval is greater than the specified number.
        Boole operator>(const NumType& Num) const
            { return this->LowerBound < Num; }

        /// @brief Less-than or equal comparison with NumType.
        /// @param Num The number to be compared with.
        /// @return Returns true if the highest bounds of this Interval is less than or equal to the specified number.
        Boole operator<=(const NumType& Num) const
            { return this->UpperBound <= Num; }
        /// @brief Greater-than or equal comparison with NumType.
        /// @param Num The number to be compared with.
        /// @return Returns true if the lowest bounds of this Interval is greater than or equal to the specified number.
        Boole operator>=(const NumType& Num) const
            { return this->LowerBound <= Num; }

        ///////////////////////////////////////////////////////////////////////////////
        // Sorting Operators

        /// @brief Less-than operator.
        /// @param Other The other Interval to compare to.
        /// @return Returns true if this Interval is to be sorted before the other Interval, false otherwise.
        Boole operator<(const SelfType& Other) const
            { return this->LowerBound < Other.LowerBound && this->UpperBound < Other.UpperBound; }
        /// @brief Greater-than operator.
        /// @param Other The other Interval to compare to.
        /// @return Returns true if this Interval is to be sorted after the other Interval, false otherwise.
        Boole operator>(const SelfType& Other) const
            { return this->LowerBound > Other.LowerBound && this->UpperBound > Other.UpperBound; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        /// @brief Convert this class to an XML::Node ready for serialization.
        /// @param ParentNode The point in the XML hierarchy that this Interval should be appended to.
        void ProtoSerialize(XML::Node& ParentNode) const
        {
            XML::Node SelfRoot = ParentNode.AppendChild( this->Interval::GetSerializableName() );

            if( SelfRoot.AppendAttribute("Version").SetValue("1") &&
                SelfRoot.AppendAttribute("UpperBound").SetValue( this->UpperBound ) &&
                SelfRoot.AppendAttribute("LowerBound").SetValue( this->LowerBound ) )
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values",Interval::GetSerializableName(),true);
            }
        }
        /// @brief Take the data stored in an XML Node and overwrite this object with it.
        /// @param SelfRoot An XML::Node containing the data to populate this class with.
        void ProtoDeSerialize(const XML::Node& SelfRoot)
        {
            XML::Attribute CurrAttrib;
            StringStream Converter;

            if( !SelfRoot.Empty() ) {
                if( SelfRoot.GetAttribute("Version").AsInt() == 1 ) {
                    CurrAttrib = SelfRoot.GetAttribute("UpperBound");
                    if( !CurrAttrib.Empty() ) {
                        Converter << CurrAttrib.AsString();
                        Converter >> this->UpperBound;
                    }

                    Converter.str("");
                    CurrAttrib = SelfRoot.GetAttribute("LowerBound");
                    if( !CurrAttrib.Empty() ) {
                        Converter << CurrAttrib.AsString();
                        Converter >> this->LowerBound;
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Interval::GetSerializableName() + ": Not Version 1.");
                }
            }else{
                MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Interval::GetSerializableName() + " was not found in the provided XML node, which was expected.");
            }
        }
        /// @brief Get the name of the the XML tag the Interval class will leave behind as its instances are serialized.
        /// @return A string containing the name of this class.
        static String GetSerializableName()
            { return "Interval"; }
    };//Interval
}//Mezzanine

#endif
