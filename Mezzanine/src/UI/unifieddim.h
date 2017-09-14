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

#ifndef _uiunifieddim_h
#define _uiunifieddim_h

#include "rect.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class UnifiedDim
        /// @headerfile unifieddim.h
        /// @brief This class represents both the relative and absolute values that can be expressed for the values on one dimension for a UI renderable.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB UnifiedDim
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The relative value on this dimension.
            Real Rel;
            /// @brief The absolute value on this dimension.
            Real Abs;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            UnifiedDim() : Rel(0), Abs(0) {}
            /// @brief Relative constructor.
            /// @param Relative The relative portion of this dimension.
            UnifiedDim(const Real& Relative)
                : Rel(Relative), Abs(0) {}
            /// @brief Descriptive constructor.
            /// @param Relative The relative portion of this dimension.
            /// @param Absolute The absolute portion of this dimension.
            UnifiedDim(const Real& Relative, const Real& Absolute)
                : Rel(Relative), Abs(Absolute) {}
            /// @brief Copy constructor.
            /// @param Other The other UnifiedDim to copy from.
            UnifiedDim(const UnifiedDim& Other)
                : Rel(Other.Rel), Abs(Other.Abs) {}
            /// @brief Class destructor.
            ~UnifiedDim() {}

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets the values of this dimension.
            /// @param Relative The relative portion of this dimension.
            /// @param Absolute The absolute portion of this dimension.
            inline void SetValues(const Real& Relative, const Real& Absolute)
            {
                this->Rel = Relative;
                this->Abs = Absolute;
            }
            /// @brief Sets all values of this dimension to zero.
            inline void SetIdentity()
            {
                this->Rel = 0;
                this->Abs = 0;
            }

            /// @brief Calculates the actual value when a Real in pixels has this unified dim applied to it.
            /// @param Actual A Real containing the actual(pixel) dimension to use as a base for the calculation.
            /// @return Returns a Real containing the result dimension in actual (pixel) units.
            inline Real CalculateActualDimension(const Real& Actual) const
            {
                return ( (this->Rel * Actual) + this->Abs );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic Operators

            /// @brief Addition operator.
            /// @param Other The other UnifiedDim to add to this.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedDim operator+(const UnifiedDim& Other) const
            {
                return UnifiedDim(this->Rel + Other.Rel, this->Abs + Other.Abs);
            }
            /// @brief Subtraction operator.
            /// @param Other The other UnifiedDim to subtract from this.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedDim operator-(const UnifiedDim& Other) const
            {
                return UnifiedDim(this->Rel - Other.Rel, this->Abs - Other.Abs);
            }
            /// @brief Multiplication operator.
            /// @param Other The other UnifiedDim to multiply by.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedDim operator*(const UnifiedDim& Other) const
            {
                return UnifiedDim(this->Rel * Other.Rel, this->Abs * Other.Abs);
            }
            /// @brief Division operator.
            /// @param Other The other UnifiedDim to divide by.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedDim operator/(const UnifiedDim& Other) const
            {
                return UnifiedDim(Other.Rel == 0.0f ? 0.0f : this->Rel / Other.Rel,
                                  Other.Abs == 0.0f ? 0.0f : this->Abs / Other.Abs);
            }

            /// @brief Addition assignment operator.
            /// @param Other The other UnifiedDim to add to this.
            /// @return Returns a reference to this.
            inline UnifiedDim& operator+=(const UnifiedDim& Other)
            {
                this->Rel += Other.Rel;
                this->Abs += Other.Abs;
                return *this;
            }
            /// @brief Subtraction assignment operator.
            /// @param Other The other UnifiedDim to subtract from this.
            /// @return Returns a reference to this.
            inline UnifiedDim& operator-=(const UnifiedDim& Other)
            {
                this->Rel -= Other.Rel;
                this->Abs -= Other.Abs;
                return *this;
            }
            /// @brief Multiplication assignment operator.
            /// @param Other The other UnifiedDim to multiply by.
            /// @return Returns a reference to this.
            inline UnifiedDim& operator*=(const UnifiedDim& Other)
            {
                this->Rel *= Other.Rel;
                this->Abs *= Other.Abs;
                return *this;
            }
            /// @brief Division assignment operator.
            /// @param Other The other UnifiedDim to divide by.
            /// @return Returns a reference to this.
            inline UnifiedDim& operator/=(const UnifiedDim& Other)
            {
                this->Rel = (Other.Rel == 0.0f ? 0.0f : this->Rel / Other.Rel);
                this->Abs = (Other.Abs == 0.0f ? 0.0f : this->Abs / Other.Abs);
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic with Real Operators

            /// @brief Multiplication with Real operator.
            /// @param Other The Real to multiply by.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedDim operator*(const Real& Other)
            {
                return UnifiedDim( this->Rel * Other, this->Abs * Other );
            }
            /// @brief Division with Real operator.
            /// @param Other The Real to divide by.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedDim operator/(const Real& Other)
            {
                return UnifiedDim( ( Other == 0 ? 0.0 : this->Rel / Other ),
                                   ( Other == 0 ? 0.0 : this->Abs / Other ) );
            }

            /// @brief Multiplication assignment with Real operator.
            /// @param Other The Real to multiply by.
            /// @return Returns a reference to this.
            inline UnifiedDim& operator*=(const Real& Other)
            {
                this->Rel *= Other;
                this->Abs *= Other;
                return *this;
            }
            /// @brief Division assignment with Real operator.
            /// @param Other The Real to divide by.
            /// @return Returns a reference to this.
            inline UnifiedDim& operator/=(const Real& Other)
            {
                this->Rel /= Other;
                this->Abs /= Other;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Comparison Operators

            /// @brief Equality comparison operator.
            /// @param Other The other UnifiedDim to compare to.
            /// @return Returns true if these UnifiedDim's are equal, false otherwise.
            inline Boole operator==(const UnifiedDim& Other) const
            {
                return this->Rel == Other.Rel && this->Abs == Other.Abs;
            }
            /// @brief Inequality comparison operator.
            /// @param Other The other UnifiedDim to compare to.
            /// @return Returns true if these UnifiedDim's are not equal, false otherwise.
            inline Boole operator!=(const UnifiedDim& Other) const
            {
                return this->Rel != Other.Rel || this->Abs != Other.Abs;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Other Operators

            /// @brief Assignment operator.
            /// @param Other The other UnifiedDim to be assign values from.
            /// @return Returns a reference to this.
            inline UnifiedDim& operator=(const UnifiedDim& Other)
            {
                this->Rel = Other.Rel;
                this->Abs = Other.Abs;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node DimNode = ParentNode.AppendChild( UnifiedDim::GetSerializableName() );

                if( DimNode.AppendAttribute("Version").SetValue("1") &&
                    DimNode.AppendAttribute("Rel").SetValue(this->Rel) &&
                    DimNode.AppendAttribute("Abs").SetValue(this->Abs) )
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values",UnifiedDim::GetSerializableName(),true);
                }
            }
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            void ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;
                if( SelfRoot.Name() == UnifiedDim::GetSerializableName() ) {
                    if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = SelfRoot.GetAttribute("Rel");
                        if( !CurrAttrib.Empty() )
                            this->Rel = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("Abs");
                        if( !CurrAttrib.Empty() )
                            this->Abs = CurrAttrib.AsReal();
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + UnifiedDim::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,UnifiedDim::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName()
            {
                return "UnifiedDim";
            }
        };//UnifiedDim

        ///////////////////////////////////////////////////////////////////////////////
        /// @class UnifiedVec2
        /// @headerfile unifieddim.h
        /// @brief This class represents a point in 2D space using UnifiedDim's.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB UnifiedVec2
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The dimension on the X plane.
            UnifiedDim X;
            /// @brief The dimension on the Y plane.
            UnifiedDim Y;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            UnifiedVec2()
            {
                this->X.SetIdentity();
                this->Y.SetIdentity();
            }
            /// @brief Real constructor.
            /// @param x The dimension on the X plane.
            /// @param y The dimension on the Y plane.
            UnifiedVec2(const Real& x, const Real& y)
                : X(x), Y(y) {}
            /// @brief UnifiedDim constructor.
            /// @param x The dimension on the X plane.
            /// @param y The dimension on the Y plane.
            UnifiedVec2(const UnifiedDim& x, const UnifiedDim& y)
                : X(x), Y(y) {}
            /// @brief Real constructor.
            /// @param Xrel The relative portion of the X dimension.
            /// @param Yrel The relative portion of the Y dimension.
            /// @param Xabs The absolute portion of the X dimension.
            /// @param Yabs the absolute portion of the Y dimension.
            UnifiedVec2(const Real& Xrel, const Real& Yrel, const Real& Xabs, const Real& Yabs)
            {
                this->X.SetValues(Xrel,Xabs);
                this->Y.SetValues(Yrel,Yabs);
            }
            /// @brief Copy constructor.
            /// @param Other The other UnifiedVec2 to copy from.
            UnifiedVec2(const UnifiedVec2& Other)
                : X(Other.X), Y(Other.Y) {}
            /// @brief Class destructor.
            ~UnifiedVec2() {}

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets all data members of this unified vector explicitly.
            /// @param x The dimension on the X plane.
            /// @param y The dimension on the Y plane.
            inline void SetValues(const UnifiedDim& x, const UnifiedDim& y)
            {
                this->X = x;
                this->Y = y;
            }
            /// @brief Sets all data members of this unified vector explicitly.
            /// @param Xrel The relative portion of the X dimension.
            /// @param Yrel The relative portion of the Y dimension.
            /// @param Xabs The absolute portion of the X dimension.
            /// @param Yabs the absolute portion of the Y dimension.
            inline void SetValues(const Real& Xrel, const Real& Yrel, const Real& Xabs, const Real& Yabs)
            {
                this->X.SetValues(Xrel,Xabs);
                this->Y.SetValues(Yrel,Yabs);
            }
            /// @brief Sets all members of this unified vector to zero.
            inline void SetIdentity()
            {
                this->X.SetIdentity();
                this->Y.SetIdentity();
            }

            /// @brief Calculates the actual values when a Vector2 with actual dimensions has this unified vector2 applied to it.
            /// @param Actual A Vector2 containing the actual(pixel) dimensions to use as a base for the calculation.
            /// @return Returns a Vector2 containing the result dimensions in actual (pixel) units.
            inline Vector2 CalculateActualDimensions(const Vector2& Actual) const
            {
                return Vector2( this->X.CalculateActualDimension(Actual.X),
                                this->Y.CalculateActualDimension(Actual.Y) );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic Operators

            /// @brief Addition operator.
            /// @param Other The other UnifiedVec2 to add to this.
            /// @return Returns a new UnifiedVec2 that is the result of this operation.
            inline UnifiedVec2 operator+(const UnifiedVec2& Other) const
            {
                return UnifiedVec2(this->X + Other.X, this->Y + Other.Y);
            }
            /// @brief Subtraction operator.
            /// @param Other The other UnifiedVec2 to subtract from this.
            /// @return Returns a new UnifiedVec2 that is the result of this operation.
            inline UnifiedVec2 operator-(const UnifiedVec2& Other) const
            {
                return UnifiedVec2(this->X - Other.X, this->Y - Other.Y);
            }
            /// @brief Multiplication operator.
            /// @param Other The other UnifiedVec2 to multiply by.
            /// @return Returns a new UnifiedVec2 that is the result of this operation.
            inline UnifiedVec2 operator*(const UnifiedVec2& Other) const
            {
                return UnifiedVec2(this->X * Other.X, this->Y * Other.Y);
            }
            /// @brief Division operator.
            /// @param Other The other UnifiedVec2 to divide by.
            /// @return Returns a new UnifiedVec2 that is the result of this operation.
            inline UnifiedVec2 operator/(const UnifiedVec2& Other) const
            {
                return UnifiedVec2(this->X / Other.X,
                                   this->Y / Other.Y);
            }

            /// @brief Addition assignment operator.
            /// @param Other The other UnifiedVec2 to add to this.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator+=(const UnifiedVec2& Other)
            {
                this->X += Other.X;
                this->Y += Other.Y;
                return *this;
            }
            /// @brief Subtraction assignment operator.
            /// @param Other The other UnifiedVec2 to subtract from this.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator-=(const UnifiedVec2& Other)
            {
                this->X -= Other.X;
                this->Y -= Other.Y;
                return *this;
            }
            /// @brief Multiplication assignment operator.
            /// @param Other The other UnifiedVec2 to multiply by.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator*=(const UnifiedVec2& Other)
            {
                this->X *= Other.X;
                this->Y *= Other.Y;
                return *this;
            }
            /// @brief Division assignment operator.
            /// @param Other The other UnifiedVec2 to divide by.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator/=(const UnifiedVec2& Other)
            {
                this->X = this->X / Other.X;
                this->Y = this->Y / Other.Y;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic with UnifiedDim Operators

            /// @brief Addition with UnifiedDim operator.
            /// @param Other The other UnifiedDim to add to this.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedVec2 operator+(const UnifiedDim& Other) const
            {
                return UnifiedVec2(this->X + Other, this->Y + Other);
            }
            /// @brief Subtraction with UnifiedDim operator.
            /// @param Other The other UnifiedDim to subtract from this.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedVec2 operator-(const UnifiedDim& Other) const
            {
                return UnifiedVec2(this->X - Other, this->Y - Other);
            }
            /// @brief Multiplication with UnifiedDim operator.
            /// @param Other The other UnifiedDim to multiply by.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedVec2 operator*(const UnifiedDim& Other) const
            {
                return UnifiedVec2(this->X * Other, this->Y * Other);
            }
            /// @brief Division with UnifiedDim operator.
            /// @param Other The other UnifiedDim to divide by.
            /// @return Returns a new UnifiedDim that is the result of this operation.
            inline UnifiedVec2 operator/(const UnifiedDim& Other) const
            {
                return UnifiedVec2(this->X / Other,
                                   this->Y / Other);
            }

            /// @brief Addition assignment with UnifiedDim operator.
            /// @param Other The other UnifiedDim to add to this.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator+=(const UnifiedDim& Other)
            {
                this->X += Other;
                this->Y += Other;
                return *this;
            }
            /// @brief Subtraction assignment with UnifiedDim operator.
            /// @param Other The other UnifiedDim to subtract from this.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator-=(const UnifiedDim& Other)
            {
                this->X -= Other;
                this->Y -= Other;
                return *this;
            }
            /// @brief Multiplication assignment with UnifiedDim operator.
            /// @param Other The other UnifiedDim to multiply by.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator*=(const UnifiedDim& Other)
            {
                this->X *= Other;
                this->Y *= Other;
                return *this;
            }
            /// @brief Division assignment with UnifiedDim operator.
            /// @param Other The other UnifiedDim to divide by.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator/=(const UnifiedDim& Other)
            {
                this->X = this->X / Other;
                this->Y = this->Y / Other;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Arithmetic with Real Operators

            /// @brief Multiplication with Real operator.
            /// @param Other The Real to multiply by.
            /// @return Returns a new UnifiedVec2 that is the result of this operation.
            inline UnifiedVec2 operator*(const Real& Other)
            {
                return UnifiedVec2( this->X * Other, this->Y * Other );
            }
            /// @brief Division with Real operator.
            /// @param Other The Real to divide by.
            /// @return Returns a new UnifiedVec2 that is the result of this operation.
            inline UnifiedVec2 operator/(const Real& Other)
            {
                return UnifiedVec2( this->X / Other, this->Y / Other );
            }

            /// @brief Multiplication assignment with Real operator.
            /// @param Other The Real to multiply by.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator*=(const Real& Other)
            {
                this->X *= Other;
                this->Y *= Other;
                return *this;
            }
            /// @brief Division assignment with Real operator.
            /// @param Other The Real to divide by.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator/=(const Real& Other)
            {
                this->X /= Other;
                this->Y /= Other;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Comparison Operators

            /// @brief Equality comparison operator.
            /// @param Other The other UnifiedVec2 to compare to.
            /// @return Returns true if these UnifiedVec2's are equal, false otherwise.
            inline Boole operator==(const UnifiedVec2& Other) const
            {
                return this->X == Other.X && this->Y == Other.Y;
            }
            /// @brief Inequality comparison operator.
            /// @param Other The other UnifiedVec2 to compare to.
            /// @return Returns true if these UnifiedVec2's are not equal, false otherwise.
            inline Boole operator!=(const UnifiedVec2& Other) const
            {
                return this->X != Other.X || this->Y != Other.Y;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Other Operators

            /// @brief Assignment operator.
            /// @param Other The other UnifiedVec2 to be assign values from.
            /// @return Returns a reference to this.
            inline UnifiedVec2& operator=(const UnifiedVec2& Other)
            {
                this->X = Other.X;
                this->Y = Other.Y;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node Vec2Node = ParentNode.AppendChild( UnifiedVec2::GetSerializableName() );

                if( Vec2Node.AppendAttribute("Version").SetValue("1") &&
                    Vec2Node.AppendAttribute("XRel").SetValue(this->X.Rel) &&
                    Vec2Node.AppendAttribute("YRel").SetValue(this->Y.Rel) &&
                    Vec2Node.AppendAttribute("XAbs").SetValue(this->X.Abs) &&
                    Vec2Node.AppendAttribute("YAbs").SetValue(this->Y.Abs) )
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values",UnifiedVec2::GetSerializableName(),true);
                }
            }
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            void ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;
                if( SelfRoot.Name() == UnifiedVec2::GetSerializableName() ) {
                    if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = SelfRoot.GetAttribute("XRel");
                        if( !CurrAttrib.Empty() )
                            this->X.Rel = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("YRel");
                        if( !CurrAttrib.Empty() )
                            this->Y.Rel = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("XAbs");
                        if( !CurrAttrib.Empty() )
                            this->X.Abs = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("YAbs");
                        if( !CurrAttrib.Empty() )
                            this->Y.Abs = CurrAttrib.AsReal();
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + UnifiedVec2::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,UnifiedVec2::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName()
            {
                return "UnifiedVec2";
            }
        };//UnifiedVec2

        ///////////////////////////////////////////////////////////////////////////////
        /// @class UnifiedRect
        /// @headerfile unifieddim.h
        /// @brief This class represents a 2D rect which can express the size and position of a renderable on screen.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB UnifiedRect
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief The top left position of this rect.
            UnifiedVec2 Position;
            /// @brief The width and height of this rect.
            UnifiedVec2 Size;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            UnifiedRect()
            {
                this->Position.SetIdentity();
                this->Size.SetIdentity();
            }
            /// @brief UnifiedVec2 constructor.
            /// @param Pos The screen position of the rect.
            /// @param Area The width and height of the rect.
            UnifiedRect(const UnifiedVec2& Pos, const UnifiedVec2& Area)
                : Position(Pos), Size(Area) {}
            /// @brief UnifiedDim constructor.
            /// @param PositionX The position on the X plane.
            /// @param PositionY The position on the Y plane.
            /// @param SizeX The size on the X plane.
            /// @param SizeY The size on the Y plane.
            UnifiedRect(const UnifiedDim& PositionX, const UnifiedDim& PositionY, const UnifiedDim& SizeX, const UnifiedDim& SizeY)
            {
                this->Position.SetValues(PositionX,PositionY);
                this->Size.SetValues(SizeX,SizeY);
            }
            /// @brief Relative Real constructor.
            /// @param PositionXrel The relative position portion of the X dimension.
            /// @param PositionYrel The relative position portion of the Y dimension.
            /// @param SizeXrel The relative size portion of the X dimension.
            /// @param SizeYrel The relative size portion of the Y dimension.
            UnifiedRect(const Real& PositionXrel, const Real& PositionYrel, const Real& SizeXrel, const Real& SizeYrel)
            {
                this->Position.SetValues(PositionXrel,PositionYrel,0.0,0.0);
                this->Size.SetValues(SizeXrel,SizeYrel,0.0,0.0);
            }
            /// @brief Real constructor.
            /// @param PositionXrel The relative position portion of the X dimension.
            /// @param PositionYrel The relative position portion of the Y dimension.
            /// @param SizeXrel The relative size portion of the X dimension.
            /// @param SizeYrel The relative size portion of the Y dimension.
            /// @param PositionXabs The absolute position portion of the X dimension.
            /// @param PositionYabs the absolute position portion of the Y dimension.
            /// @param SizeXabs The absolute size portion of the X dimension.
            /// @param SizeYabs the absolute size portion of the Y dimension.
            UnifiedRect(const Real& PositionXrel, const Real& PositionYrel, const Real& SizeXrel, const Real& SizeYrel,
                        const Real& PositionXabs, const Real& PositionYabs, const Real& SizeXabs, const Real& SizeYabs)
            {
                this->Position.SetValues(PositionXrel,PositionYrel,PositionXabs,PositionYabs);
                this->Size.SetValues(SizeXrel,SizeYrel,SizeXabs,SizeYabs);
            }
            /// @brief Class destructor.
            ~UnifiedRect() {}

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Sets all data members of this unified rect explicitly.
            /// @param Pos The screen position of the rect.
            /// @param Area The width and height of the rect.
            inline void SetValues(const UnifiedVec2& Pos, const UnifiedVec2& Area)
            {
                this->Position = Pos;
                this->Size = Area;
            }
            /// @brief Sets all data members of this unified rect explicitly.
            /// @param PositionX The position on the X plane.
            /// @param PositionY The position on the Y plane.
            /// @param SizeX The size on the X plane.
            /// @param SizeY The size on the Y plane.
            inline void SetValues(const UnifiedDim& PositionX, const UnifiedDim& PositionY, const UnifiedDim& SizeX, const UnifiedDim& SizeY)
            {
                this->Position.SetValues(PositionX,PositionY);
                this->Size.SetValues(SizeX,SizeY);
            }
            /// @brief Sets all data members of this unified rect explicitly.
            /// @param PositionXrel The relative position portion of the X dimension.
            /// @param PositionYrel The relative position portion of the Y dimension.
            /// @param SizeXrel The relative size portion of the X dimension.
            /// @param SizeYrel The relative size portion of the Y dimension.
            /// @param PositionXabs The absolute position portion of the X dimension.
            /// @param PositionYabs the absolute position portion of the Y dimension.
            /// @param SizeXabs The absolute size portion of the X dimension.
            /// @param SizeYabs the absolute size portion of the Y dimension.
            inline void SetValues(const Real& PositionXrel, const Real& PositionYrel, const Real& SizeXrel, const Real& SizeYrel,
                                  const Real& PositionXabs, const Real& PositionYabs, const Real& SizeXabs, const Real& SizeYabs)
            {
                this->Position.SetValues(PositionXrel,PositionYrel,PositionXabs,PositionYabs);
                this->Size.SetValues(SizeXrel,SizeYrel,SizeXabs,SizeYabs);
            }
            /// @brief Sets all members of this unified rect to zero.
            inline void SetIdentity()
            {
                this->Position.SetIdentity();
                this->Size.SetIdentity();
            }

            /// @brief Calculates the actual values when a Rect with actual dimensions has this unified rect applied to it.
            /// @param Actual A Rect containing the actual(pixel) position and size to use as a base for the calculation.
            /// @param AsChild Whether or not the dimensions should be calculated assuming this is a child rect.
            /// @return Returns a Rect containing the result position and size in actual (pixel) units.
            inline Rect CalculateActualDimensions(const Rect& Actual, Boole AsChild = true) const
            {
                Vector2 Pos = this->Position.CalculateActualDimensions(Actual.Size);
                Vector2 Size = this->Size.CalculateActualDimensions(Actual.Size);
                return Rect( ( AsChild ? Pos + Actual.Position : Pos ), Size );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Comparison Operators

            /// @brief Equality comparison operator.
            /// @param Other The other UnifiedRect to compare to.
            /// @return Returns true if these UnifiedRect's are equal, false otherwise.
            inline Boole operator==(const UnifiedRect& Other) const
            {
                return this->Position == Other.Position && this->Size == Other.Size;
            }
            /// @brief Inequality comparison operator.
            /// @param Other The other UnifiedRect to compare to.
            /// @return Returns true if these UnifiedRect's are not equal, false otherwise.
            inline Boole operator!=(const UnifiedRect& Other) const
            {
                return this->Position != Other.Position || this->Size != Other.Size;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Other Operators

            /// @brief Assignment operator.
            /// @param Other The other UnifiedRect to be assign values from.
            /// @return Returns a reference to this.
            inline UnifiedRect& operator=(const UnifiedRect& Other)
            {
                this->Position = Other.Position;
                this->Size = Other.Size;
                return *this;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node RectNode = ParentNode.AppendChild( UnifiedRect::GetSerializableName() );

                if( RectNode.AppendAttribute("Version").SetValue("1") &&
                    RectNode.AppendAttribute("PositionXRel").SetValue(this->Position.X.Rel) &&
                    RectNode.AppendAttribute("PositionYRel").SetValue(this->Position.Y.Rel) &&
                    RectNode.AppendAttribute("SizeXRel").SetValue(this->Size.X.Rel) &&
                    RectNode.AppendAttribute("SizeYRel").SetValue(this->Size.Y.Rel) &&
                    RectNode.AppendAttribute("PositionXAbs").SetValue(this->Position.X.Abs) &&
                    RectNode.AppendAttribute("PositionYAbs").SetValue(this->Position.Y.Abs) &&
                    RectNode.AppendAttribute("SizeXAbs").SetValue(this->Size.X.Abs) &&
                    RectNode.AppendAttribute("SizeYAbs").SetValue(this->Size.Y.Abs) )
                {
                    return;
                }else{
                    SerializeError("Create XML Attribute Values",UnifiedRect::GetSerializableName(),true);
                }
            }
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate the new instance with.
            void ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;
                if( SelfRoot.Name() == UnifiedRect::GetSerializableName() ) {
                    if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = SelfRoot.GetAttribute("PositionXRel");
                        if( !CurrAttrib.Empty() )
                            this->Position.X.Rel = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("PositionYRel");
                        if( !CurrAttrib.Empty() )
                            this->Position.Y.Rel = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("SizeXRel");
                        if( !CurrAttrib.Empty() )
                            this->Size.X.Rel = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("SizeYRel");
                        if( !CurrAttrib.Empty() )
                            this->Size.Y.Rel = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("PositionXAbs");
                        if( !CurrAttrib.Empty() )
                            this->Position.X.Abs = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("PositionYAbs");
                        if( !CurrAttrib.Empty() )
                            this->Position.Y.Abs = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("SizeXAbs");
                        if( !CurrAttrib.Empty() )
                            this->Size.X.Abs = CurrAttrib.AsReal();

                        CurrAttrib = SelfRoot.GetAttribute("SizeYAbs");
                        if( !CurrAttrib.Empty() )
                            this->Size.Y.Abs = CurrAttrib.AsReal();
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + UnifiedRect::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,UnifiedRect::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName()
            {
                return "UnifiedRect";
            }
        };//UnifiedRect
    }//UI
}//Mezzanine

#endif
