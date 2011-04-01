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

#include <Ogre.h>

#include <memory>
namespace phys
{
    ColourValue::ColourValue(Real red, Real green, Real blue)
    {
        this->Red = red;
        this->Green = green;
        this->Blue = blue;
        this->Alpha = 1.0;
    }

    ColourValue::ColourValue(Real red, Real green, Real blue, Real alpha)
    {
        this->Red = red;
        this->Green = green;
        this->Blue = blue;
        this->Alpha = alpha;
    }

    ColourValue::ColourValue(const Ogre::ColourValue& OgreValues)
    {
        this->Red = OgreValues.r;
        this->Green = OgreValues.g;
        this->Blue = OgreValues.b;
        this->Alpha = OgreValues.a;
    }

    ColourValue::ColourValue(const ColourValue& OtherColour)
    {
        this->Red = OtherColour.Red;
        this->Green = OtherColour.Green;
        this->Blue = OtherColour.Blue;
        this->Alpha = OtherColour.Alpha;
    }

    ColourValue::~ColourValue()
    {
    }

    Ogre::ColourValue ColourValue::GetOgreColourValue() const
    {
        Ogre::ColourValue OgreColour;
        OgreColour.r = this->Red;
        OgreColour.g = this->Green;
        OgreColour.b = this->Blue;
        OgreColour.a = this->Alpha;
        return OgreColour;
    }

    bool ColourValue::operator== (const ColourValue &Colour)
    {
        return ( Colour.Red == this->Red && Colour.Green == this->Green && Colour.Blue == this->Blue && Colour.Alpha == this->Alpha );
        /*if ( Colour.Red == this->Red && Colour.Green == this->Green && Colour.Blue == this->Blue && Colour.Alpha == this->Alpha )
            { return true; }
        return false;*/
    }

    bool ColourValue::operator!= (const ColourValue &Colour)
    {
        return ( Colour.Red != this->Red || Colour.Green != this->Green || Colour.Blue != this->Blue || Colour.Alpha != this->Alpha );
        /*if ( Colour.Red != this->Red || Colour.Green != this->Green || Colour.Blue != this->Blue || Colour.Alpha != this->Alpha )
            { return true; }
        return false;*/
    }

    void ColourValue::operator= (const ColourValue &OtherColour)
    {
        this->Red = OtherColour.Red;
        this->Green = OtherColour.Green;
        this->Blue = OtherColour.Blue;
        this->Alpha = OtherColour.Alpha;
    }

    ColourValue ColourValue::GetBlank()
    {
        ColourValue Colour(0.0,0.0,0.0,0.0);
        return Colour;
    }

    ColourValue ColourValue::GetWhite()
    {
        ColourValue Colour(1.0,1.0,1.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::GetBlack()
    {
        ColourValue Colour(0.0,0.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::GetRed()
    {
        ColourValue Colour(1.0,0.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::GetGreen()
    {
        ColourValue Colour(0.0,1.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::GetBlue()
    {
        ColourValue Colour(0.0,0.0,1.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::GetYellow()
    {
        ColourValue Colour(1.0,1.0,0.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::GetCyan()
    {
        ColourValue Colour(0.0,1.0,1.0,1.0);
        return Colour;
    }

    ColourValue ColourValue::GetMagenta()
    {
        ColourValue Colour(1.0,0.0,1.0,1.0);
        return Colour;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef PHYSXML
std::ostream& operator << (std::ostream& stream, const phys::ColourValue& Ev)
{
    stream << "<ColourValue Version=\"1\" Red=\"" << Ev.Red << "\" Green=\"" << Ev.Green << "\" Blue=\"" << Ev.Blue << "\" Alpha=\"" << Ev.Alpha << "\" />";
    return stream;
}

std::istream& PHYS_LIB operator >> (std::istream& stream, phys::ColourValue& Ev)
{
    phys::String OneTag( phys::xml::GetOneTag(stream) );
    std::auto_ptr<phys::xml::Document> Doc( phys::xml::PreParseClassFromSingleTag("phys::", "ColourValue", OneTag) );

    Doc->GetFirstChild() >> Ev;

    return stream;
}

phys::xml::Node& operator >> (const phys::xml::Node& OneNode, phys::ColourValue& Ev)
{
    if ( phys::String(OneNode.Name())==phys::String("ColourValue") )
    {
        if(OneNode.GetAttribute("Version").AsInt() == 1)
        {
            Ev.Red      = OneNode.GetAttribute("Red").AsReal();
            Ev.Green    = OneNode.GetAttribute("Green").AsReal();
            Ev.Blue     = OneNode.GetAttribute("Blue").AsReal();
            Ev.Alpha    = OneNode.GetAttribute("Alpha").AsReal();
        }else{
            throw( phys::Exception("Incompatible XML Version for ColourValue: Not Version 1"));
        }
    }else{
        throw( phys::Exception(phys::StringCat("Attempting to deserialize a ColourValue, found a ", OneNode.Name())));
    }
}
#endif // \PHYSXML


#endif
