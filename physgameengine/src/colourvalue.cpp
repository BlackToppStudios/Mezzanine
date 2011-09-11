//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _colourvalue_cpp
#define _colourvalue_cpp

#include "colourvalue.h"
#include "serialization.h"

#include <Ogre.h>

#include <memory>
namespace phys
{
    /*ColourValue::ColourValue(Real red, Real green, Real blue)
    {
        this->Red = red;
        this->Green = green;
        this->Blue = blue;
        this->Alpha = 1.0;
    }*/

    ColourValue::ColourValue(Real Red, Real Green, Real Blue, Real Alpha)
    {
        this->R = Red;
        this->G = Green;
        this->B = Blue;
        this->A = Alpha;
    }

    ColourValue::ColourValue(const Ogre::ColourValue& OgreValues)
    {
        this->R = OgreValues.r;
        this->G = OgreValues.g;
        this->B = OgreValues.b;
        this->A = OgreValues.a;
    }

    ColourValue::ColourValue(const ColourValue& OtherColour)
    {
        this->R = OtherColour.R;
        this->G = OtherColour.G;
        this->B = OtherColour.B;
        this->A = OtherColour.A;
    }

    ColourValue::~ColourValue()
    {
    }

    Ogre::ColourValue ColourValue::GetOgreColourValue() const
    {
        Ogre::ColourValue OgreColour;
        OgreColour.r = this->R;
        OgreColour.g = this->G;
        OgreColour.b = this->B;
        OgreColour.a = this->A;
        return OgreColour;
    }

    bool ColourValue::operator== (const ColourValue &Colour)
        { return ( Colour.R == this->R && Colour.G == this->G && Colour.B == this->B && Colour.A == this->A ); }

    bool ColourValue::operator!= (const ColourValue &Colour)
        { return ( Colour.R != this->R || Colour.G != this->G || Colour.B != this->B || Colour.A != this->A ); }

    void ColourValue::operator= (const ColourValue &OtherColour)
    {
        this->R = OtherColour.R;
        this->G = OtherColour.G;
        this->B = OtherColour.B;
        this->A = OtherColour.A;
    }

    ColourValue ColourValue::Transparent()
    {
        ColourValue Colour(0.0,0.0,0.0,0.0);
        return Colour;
    }

    ColourValue ColourValue::White()
    {
        ColourValue Colour(1.0,1.0,1.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::Black()
    {
        ColourValue Colour(0.0,0.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::Red()
    {
        ColourValue Colour(1.0,0.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::Green()
    {
        ColourValue Colour(0.0,1.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::Blue()
    {
        ColourValue Colour(0.0,0.0,1.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::Yellow()
    {
        ColourValue Colour(1.0,1.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::Cyan()
    {
        ColourValue Colour(0.0,1.0,1.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::Magenta()
    {
        ColourValue Colour(1.0,0.0,1.0,1.0);
        return Colour;
    }

#ifdef PHYSXML
    // Serializable
    void ColourValue::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        phys::xml::Node VecNode = CurrentRoot.AppendChild(this->ColourValue::SerializableName());
        VecNode.SetName(this->ColourValue::SerializableName());

        phys::xml::Attribute VersionAttr = VecNode.AppendAttribute("Version");
        phys::xml::Attribute RAttr = VecNode.AppendAttribute("Red");
        phys::xml::Attribute GAttr = VecNode.AppendAttribute("Green");
        phys::xml::Attribute BAttr = VecNode.AppendAttribute("Blue");
        phys::xml::Attribute AAttr = VecNode.AppendAttribute("Alpha");
        if( VersionAttr && RAttr && BAttr && GAttr && AAttr)
        {
            if( VersionAttr.SetValue("1") && RAttr.SetValue(this->R) && BAttr.SetValue(this->B) && GAttr.SetValue(this->G) && AAttr.SetValue(this->A))
            {
                return;
            }else{
                SerializeError("Create XML Attribute Values", this->ColourValue::SerializableName(),true);
            }
        }else{
            SerializeError("Create XML Attributes", this->ColourValue::SerializableName(),true);
        }
    }

    // DeSerializable
    void ColourValue::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==this->ColourValue::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->R=OneNode.GetAttribute("R").AsReal();
                this->G=OneNode.GetAttribute("G").AsReal();
                this->B=OneNode.GetAttribute("B").AsReal();
                this->A=OneNode.GetAttribute("A").AsReal();
            }else{
                throw( phys::Exception(StringCat("Incompatible XML Version for ",this->ColourValue::SerializableName(),": Not Version 1")) );
            }
        }else{
            throw( phys::Exception(phys::StringCat("Attempting to deserialize a ",this->ColourValue::SerializableName(),", found a ", OneNode.Name())));
        }
    }

    String ColourValue::SerializableName()
        { return String("ColourValue"); }
#endif


}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::ColourValue& Ev)
{
    Serialize(stream,Ev);
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ColourValue& Ev)
    { return DeSerialize(stream, Ev); }

void operator >> (const phys::xml::Node& OneNode, phys::ColourValue& Ev)
    { Ev.ProtoDeSerialize(OneNode); }
#endif // \PHYSXML


#endif
