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
#ifndef _uirect_h
#define _uirect_h

#include "vector2.h"
#include "exception.h"
#include "serialization.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Rect
        /// @headerfile rect.h
        /// @brief This class represents a box shaped area on the screen.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB Rect
        {
        protected:
            /// @brief Simple check to see if a point is within a given range.
            /// @param Value The value to compare against the range.
            /// @param Min The minimum value of the range.
            /// @param Max The maximum value of the range.
            /// @return Returns true if the provided value is within the provided range, false otherwise.
            inline Boole ValueInRange(const Real& Value, const Real& Min, const Real& Max) const
            {
                return (Value >= Min) && (Value <= Max);
            }
        public:
            /// @brief Vector2 representing the top-left position of the rect.
            Vector2 Position;
            /// @brief Vector2 representing the width and height of the rect.
            Vector2 Size;
            /// @brief Boole representing whether this rect is represented in relative units or absolute units(pixels).
            Boole Relative;

            /// @brief Less Detailed Real Constructor.
            /// @param PosX The position of this rect on the X axis.
            /// @param PosY The position of this rect on the Y axis.
            /// @param SizeX The size of this rect on the X axis.
            /// @param SizeY The size of this rect on the Y axis.
            Rect(const Real& PosX, const Real& PosY, const Real& SizeX, const Real& SizeY)
            {
                this->Position.SetValues(PosX,PosY);
                this->Size.SetValues(SizeX,SizeY);
                this->Relative = false;
            }
            /// @brief Real Constructor.
            /// @param PosX The position of this rect on the X axis.
            /// @param PosY The position of this rect on the Y axis.
            /// @param SizeX The size of this rect on the X axis.
            /// @param SizeY The size of this rect on the Y axis.
            Rect(const Real& PosX, const Real& PosY, const Real& SizeX, const Real& SizeY, Boole Relative)
            {
                this->Position.SetValues(PosX,PosY);
                this->Size.SetValues(SizeX,SizeY);
                this->Relative = Relative;
            }
            /// @brief Less Detailed Vector2 Constructor.
            /// @details Sets all the data of the class.
            /// @param Position The position of the Renderable's Rect.
            /// @param Size The size of the Renderable's Rect.
            Rect(const Vector2& Position, const Vector2& Size)
            {
                this->Position = Position;
                this->Size = Size;
                this->Relative = false;
            }
            /// @brief Vector2 Constructor.
            /// @details Sets all the data of the class.
            /// @param Position The position of the Renderable's Rect.
            /// @param Size The size of the Renderable's Rect.
            /// @param Relative Whether or not this Rect is using relative(0-1) or absolute units(Pixels).
            Rect(const Vector2& Position, const Vector2& Size, Boole Relative)
            {
                this->Position = Position;
                this->Size = Size;
                this->Relative = Relative;
            }
            /// @brief Copy Constructor.
            /// @param Other The other Rect to copy.
            Rect(const Rect& Other)
            {
                this->Position = Other.Position;
                this->Size = Other.Size;
                this->Relative = Other.Relative;
            }
            /// @brief No Initialization Constructor.
            /// @details Sets all data members to zero's.
            Rect()
            {
                this->SetIdentity();
                this->Relative = false;
            }
            /// @brief Class destructor.
            ~Rect() {}

            ///////////////////////////////////////////////////////////////////////////////
            // Utility Methods

            /// @brief Sets all the values of this rect to zero.
            inline void SetIdentity()
            {
                this->Position.SetIdentity();
                this->Size.SetIdentity();
            }

            /// @brief Gets the value of the top edge of this rect.
            /// @return Returns a Real that is the point on the vertical axis this rect's top edge is located.
            inline Real GetTopEdge() const
            {
                return this->Position.Y;
            }
            /// @brief Gets the value of the bottom edge of this rect.
            /// @return Returns a Real that is the point on the vertical axis this rect's bottom edge is located.
            inline Real GetBottomEdge() const
            {
                return this->Position.Y + this->Size.Y;
            }
            /// @brief Gets the value of the left edge of this rect.
            /// @return Returns a Real that is the point on the horizontal axis this rect's left edge is located.
            inline Real GetLeftEdge() const
            {
                return this->Position.X;
            }
            /// @brief Gets the value of the right edge of this rect.
            /// @return Returns a Real that is the point on the horizontal axis this rect's right edge is located.
            inline Real GetRightEdge() const
            {
                return this->Position.X + this->Size.X;
            }

            /// @brief Gets the value of the horizontal center of this rect.
            /// @return Returns a Real that is the point on the horizontal axis that is this rect's center.
            inline Real GetHorizontalCenter() const
            {
                return this->Position.X + (this->Size.X * 0.5);
            }
            /// @brief Gets the value of the vertical center of this rect.
            /// @return Returns a Real that is the point on the vertical axis that is this rect's center.
            inline Real GetVerticalCenter() const
            {
                return this->Position.Y + (this->Size.Y * 0.5);
            }
            /// @brief Gets the coordinates to the center of this rect.
            /// @return Returns a vector2 containing the central point of this rect.
            inline Vector2 GetRectCenter() const
            {
                return Vector2(this->GetHorizontalCenter(),this->GetVerticalCenter());
            }

            /// @brief Checks to see if another Rect is overlapping with this one.
            /// @param OtherRect The other rect to compare against.
            /// @return Returns true if these rects overlap with each other, false otherwise.
            inline Boole CheckOverlap(const Rect& OtherRect) const
            {
                Boole XOverlap = ValueInRange(this->Position.X, OtherRect.Position.X, OtherRect.Position.X + OtherRect.Size.X) ||
                                   ValueInRange(OtherRect.Position.X, this->Position.X, this->Position.X + this->Size.X);

                Boole YOverlap = ValueInRange(this->Position.Y, OtherRect.Position.Y, OtherRect.Position.Y + OtherRect.Size.Y) ||
                                   ValueInRange(OtherRect.Position.Y, this->Position.Y, this->Position.Y + this->Size.Y);

                return (XOverlap && YOverlap);
            }
            /// @brief Checks to see if a point in 2D space is inside this rect.
            /// @param Point The point in 2D space to check.
            /// @return Returns true if the provided point is within this rect, false otherwise.
            inline Boole IsInside(const Vector2& Point) const
            {
                return ( ValueInRange(Point.X,this->Position.X,this->Position.X + this->Size.X) &&
                         ValueInRange(Point.Y,this->Position.Y,this->Position.Y + this->Size.Y) );
            }
            /// @brief Gets whether or point on the X axis is within the limits of this rect or not.
            /// @param Position The point on the X axis.
            /// @return Returns true in the provided position is within this rect's limits, false otherwise.
            inline Boole IsWithinWidth(const Real& Position) const
            {
                return ValueInRange(Position,this->Position.X,this->Position.X + this->Size.X);
            }
            /// @brief Gets whether or point on the Y axis is within the limits of this rect or not.
            /// @param Position The point on the Y axis.
            /// @return Returns true in the provided position is within this rect's limits, false otherwise.
            inline Boole IsWithinHeight(const Real& Position) const
            {
                return ValueInRange(Position,this->Position.Y,this->Position.Y + this->Size.Y);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            /// @brief Scales this rect.
            /// @note This will scale the rect based on it's center, not it's top-left position.  If you want to instead preserve the position of the Rect then apply the scaling manually.
            /// @param Scaling A Vector2 representing the X and Y scaling to be applied to this rect.
            /// @return Returns a reference to this.
            inline Rect& ApplyScaling(const Vector2& Scaling)
            {
                Vector2 PrevCenter = this->GetRectCenter();
                this->Size *= Scaling;

                this->Position.X = PrevCenter.X - ( this->Size.X * 0.5 );
                this->Position.Y = PrevCenter.Y - ( this->Size.Y * 0.5 );

                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Overloaded Operators

            /// @brief Assignment Operator.
            /// @details Copys the contents of a Rect to another.
            /// @param Other The other Rect to copy from.
            inline Rect& operator=(const Rect& Other)
            {
                this->Position = Other.Position;
                this->Size = Other.Size;
                this->Relative = Other.Relative;
                return *this;
            }
            /// @brief Equality Comparison Operator.
            /// @details Checks to see if the two Rects are equal.
            /// @param Other The other Rect to compare against.
            inline Boole operator==(const Rect& Other)
            {
                return ( this->Position == Other.Position && this->Size == Other.Size && this->Relative == Other.Relative );
            }
            /// @brief Inequality Comparison Operator.
            /// @details Checks to see if the two rects are different.
            /// @param Other The other Rect to compare against.
            inline Boole operator!=(const Rect& Other)
            {
                return ( this->Position != Other.Position || this->Size != Other.Size || this->Relative != Other.Relative );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node RectNode = ParentNode.AppendChild( Rect::GetSerializableName() );

                if( RectNode.AppendAttribute("Version").SetValue("1") &&
                    RectNode.AppendAttribute("PositionX").SetValue(this->Position.X) &&
                    RectNode.AppendAttribute("PositionY").SetValue(this->Position.Y) &&
                    RectNode.AppendAttribute("SizeX").SetValue(this->Size.X) &&
                    RectNode.AppendAttribute("SizeY").SetValue(this->Size.Y) )
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values",Rect::GetSerializableName(),true);
                }
            }
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;
                XML::Node RectNode = SelfRoot.GetChild( Rect::GetSerializableName() );

                if( !RectNode.Empty() ) {
                    if(RectNode.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = RectNode.GetAttribute("PositionX");
                        if( !CurrAttrib.Empty() )
                            this->Position.X = CurrAttrib.AsReal();

                        CurrAttrib = RectNode.GetAttribute("PositionY");
                        if( !CurrAttrib.Empty() )
                            this->Position.Y = CurrAttrib.AsReal();

                        CurrAttrib = RectNode.GetAttribute("SizeX");
                        if( !CurrAttrib.Empty() )
                            this->Size.X = CurrAttrib.AsReal();

                        CurrAttrib = RectNode.GetAttribute("SizeY");
                        if( !CurrAttrib.Empty() )
                            this->Size.X = CurrAttrib.AsReal();
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + Rect::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,Rect::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName()
            {
                return "Rect";
            }
        };//Rect
    }//UI
}//Mezzanine

#endif
