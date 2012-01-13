//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _colourvalue_cpp
#define _colourvalue_cpp

#include "colourvalue.h"
#include "stringtool.h"
#include "serialization.h"

#include <Ogre.h>

#include <memory>
namespace Mezzanine
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
        { return ColourValue(0.0,0.0,0.0,0.0); }

    ColourValue ColourValue::AliceBlue()
        { return ColourValue(0.941176,0.972549,1.0,1.0); }

    ColourValue ColourValue::AntiqueWhite()
        { return ColourValue(0.980392,0.921568,0.843137,1.0); }

    ColourValue ColourValue::Aqua()
        { return ColourValue(0.0,1.0,1.0,1.0); }

    ColourValue ColourValue::Aquamarine()
        { return ColourValue(0.498039,1.0,0.831372,1.0); }

    ColourValue ColourValue::Azure()
        { return ColourValue(0.941176,1.0,1.0,1.0); }

    ColourValue ColourValue::Beige()
        { return ColourValue(0.960784,0.960784,0.862745,1.0); }

    ColourValue ColourValue::Bisque()
        { return ColourValue(1.0,0.894117,0.768627,1.0); }

    ColourValue ColourValue::Black()
        { return ColourValue(0.0,0.0,0.0,1.0); }

    ColourValue ColourValue::Blanchedalmond()
        { return ColourValue(1.0,0.921568,0.803921,1.0); }

    ColourValue ColourValue::Blue()
        { return ColourValue(0.0,0.0,1.0,1.0); }

    ColourValue ColourValue::BlueViolet()
        { return ColourValue(0.541176,0.168627,0.886274,1.0); }

    ColourValue ColourValue::Brown()
        { return ColourValue(0.647058,0.164705,0.164705,1.0); }

    ColourValue ColourValue::BurlyWood()
        { return ColourValue(0.870588,0.721568,0.529411,1.0); }

    ColourValue ColourValue::CadetBlue()
        { return ColourValue(0.372549,0.619607,0.627450,1.0); }

    ColourValue ColourValue::Chartreuse()
        { return ColourValue(0.498039,1.0,0.0,1.0); }

    ColourValue ColourValue::Chocolate()
        { return ColourValue(0.823529,0.411764,0.117647,1.0); }

    ColourValue ColourValue::Coral()
        { return ColourValue(1.0,0.498039,0.313725,1.0); }

    ColourValue ColourValue::CornFlowerBlue()
        { return ColourValue(0.392116,0.584313,0.929411,1.0); }

    ColourValue ColourValue::CornSilk()
        { return ColourValue(1.0,0.972549,0.862745,1.0); }

    ColourValue ColourValue::Crimson()
        { return ColourValue(0.862745,0.078431,0.235294,1.0); }

    ColourValue ColourValue::Cyan()
        { return ColourValue(0.0,1.0,1.0,1.0); }

    ColourValue ColourValue::DarkBlue()
        { return ColourValue(0.0,0.0,0.545098,1.0); }

    ColourValue ColourValue::DarkCyan()
        { return ColourValue(0.0,0.545098,0.545098,1.0); }

    ColourValue ColourValue::DarkGoldenRod()
        { return ColourValue(0.721568,0.525490,0.043137,1.0); }

    ColourValue ColourValue::DarkGray()
        { return ColourValue(0.662745,0.662745,0.662745,1.0); }

    ColourValue ColourValue::DarkGreen()
        { return ColourValue(0.0,0.392116,0.0,1.0); }

    ColourValue ColourValue::DarkKhaki()
        { return ColourValue(0.741176,0.717647,0.419607,1.0); }

    ColourValue ColourValue::DarkMagenta()
        { return ColourValue(0.545098,0.0,0.545098,1.0); }

    ColourValue ColourValue::DarkOliveGreen()
        { return ColourValue(0.333333,0.419607,0.184313,1.0); }

    ColourValue ColourValue::DarkOrange()
        { return ColourValue(1.0,0.549019,0.0,1.0); }

    ColourValue ColourValue::DarkOrchid()
        { return ColourValue(0.6,0.196078,0.8,1.0); }

    ColourValue ColourValue::DarkRed()
        { return ColourValue(0.545098,0.0,0.0,1.0); }

    ColourValue ColourValue::DarkSalmon()
        { return ColourValue(0.913725,0.588235,0.478431,1.0); }

    ColourValue ColourValue::DarkSeaGreen()
        { return ColourValue(0.560784,0.737254,0.560784,1.0); }

    ColourValue ColourValue::DarkSlateBlue()
        { return ColourValue(0.282352,0.239215,0.545098,1.0); }

    ColourValue ColourValue::DarkSlateGray()
        { return ColourValue(0.184313,0.309803,0.309803,1.0); }

    ColourValue ColourValue::DarkTurquoise()
        { return ColourValue(0.0,0.807843,0.819607,1.0); }

    ColourValue ColourValue::DarkViolet()
        { return ColourValue(0.580392,0.0,0.827450,1.0); }

    ColourValue ColourValue::DeepPink()
        { return ColourValue(1.0,0.078431,0.576470,1.0); }

    ColourValue ColourValue::DeepSkyBlue()
        { return ColourValue(0.0,0.749019,1.0,1.0); }

    ColourValue ColourValue::DimGray()
        { return ColourValue(0.411764,0.411764,0.411764,1.0); }

    ColourValue ColourValue::DodgerBlue()
        { return ColourValue(0.117647,0.564705,1.0,1.0); }

    ColourValue ColourValue::FireBrick()
        { return ColourValue(0.698039,0.133333,0.133333,1.0); }

    ColourValue ColourValue::FloralWhite()
        { return ColourValue(1.0,0.980392,0.941176,1.0); }

    ColourValue ColourValue::ForestGreen()
        { return ColourValue(0.133333,0.545098,0.133333,1.0); }

    ColourValue ColourValue::Fuchsia()
        { return ColourValue(1.0,0.0,1.0,1.0); }

    ColourValue ColourValue::Gainsboro()
        { return ColourValue(0.862745,0.862745,0.862745,1.0); }

    ColourValue ColourValue::GhostWhite()
        { return ColourValue(0.972549,0.972549,1.0,1.0); }

    ColourValue ColourValue::Gold()
        { return ColourValue(1.0,0.843137,0.0,1.0); }

    ColourValue ColourValue::GoldenRod()
        { return ColourValue(0.854901,0.647058,0.125490,1.0); }

    ColourValue ColourValue::Gray()
        { return ColourValue(0.501960,0.501960,0.501960,1.0); }

    ColourValue ColourValue::Green()
        { return ColourValue(0.0,0.501960,0.0,1.0); }

    ColourValue ColourValue::GreenYellow()
        { return ColourValue(0.678431,1.0,0.184313,1.0); }

    ColourValue ColourValue::HoneyDew()
        { return ColourValue(0.941176,1.0,0.941176,1.0); }

    ColourValue ColourValue::HotPink()
        { return ColourValue(1.0,0.411764,0.705882,1.0); }

    ColourValue ColourValue::IndianRed()
        { return ColourValue(0.803921,0.360784,0.360784,1.0); }

    ColourValue ColourValue::Indigo()
        { return ColourValue(0.294117,0.0,0.509803,1.0); }

    ColourValue ColourValue::Ivory()
        { return ColourValue(1.0,1.0,0.941176,1.0); }

    ColourValue ColourValue::Khaki()
        { return ColourValue(0.941176,0.901960,0.549019,1.0); }

    ColourValue ColourValue::Lavender()
        { return ColourValue(0.901960,0.901960,0.980392,1.0); }

    ColourValue ColourValue::LavenderBlush()
        { return ColourValue(1.0,0.941176,0.960784,1.0); }

    ColourValue ColourValue::LawnGreen()
        { return ColourValue(0.486274,0.988235,0.0,1.0); }

    ColourValue ColourValue::LemonChiffon()
        { return ColourValue(1.0,0.980392,0.803921,1.0); }

    ColourValue ColourValue::LightBlue()
        { return ColourValue(0.678431,0.847058,0.901960,1.0); }

    ColourValue ColourValue::LightCoral()
        { return ColourValue(0.941176,0.501960,0.501960,1.0); }

    ColourValue ColourValue::LightCyan()
        { return ColourValue(0.878431,1.0,1.0,1.0); }

    ColourValue ColourValue::LightGoldenRodYellow()
        { return ColourValue(0.980392,0.980392,0.823529,1.0); }

    ColourValue ColourValue::LightGray()
        { return ColourValue(0.827450,0.827450,0.827450,1.0); }

    ColourValue ColourValue::LightGreen()
        { return ColourValue(0.564705,0.933333,0.564705,1.0); }

    ColourValue ColourValue::LightPink()
        { return ColourValue(1.0,0.713725,0.756862,1.0); }

    ColourValue ColourValue::LightSalmon()
        { return ColourValue(1.0,0.627450,0.478431,1.0); }

    ColourValue ColourValue::LightSeaGreen()
        { return ColourValue(0.125490,0.698039,0.666666,1.0); }

    ColourValue ColourValue::LightSkyBlue()
        { return ColourValue(0.529411,0.807843,0.980392,1.0); }

    ColourValue ColourValue::LightSlateGray()
        { return ColourValue(0.466666,0.533333,0.6,1.0); }

    ColourValue ColourValue::LightSteelBlue()
        { return ColourValue(0.690196,0.768627,0.870588,1.0); }

    ColourValue ColourValue::LightYellow()
        { return ColourValue(1.0,1.0,0.878431,1.0); }

    ColourValue ColourValue::Lime()
        { return ColourValue(0.0,1.0,0.0,1.0); }

    ColourValue ColourValue::LimeGreen()
        { return ColourValue(0.196078,0.803921,0.196078,1.0); }

    ColourValue ColourValue::Linen()
        { return ColourValue(0.980392,0.941176,0.901960,1.0); }

    ColourValue ColourValue::Magenta()
        { return ColourValue(1.0,0.0,1.0,1.0); }

    ColourValue ColourValue::Maroon()
        { return ColourValue(0.501960,0.0,0.0,1.0); }

    ColourValue ColourValue::MediumAquamarine()
        { return ColourValue(0.4,0.803921,0.666666,1.0); }

    ColourValue ColourValue::MediumBlue()
        { return ColourValue(0.0,0.0,0.803921,1.0); }

    ColourValue ColourValue::MediumOrchid()
        { return ColourValue(0.729411,0.333333,0.827450,1.0); }

    ColourValue ColourValue::MediumPurple()
        { return ColourValue(0.576470,0.439215,0.858823,1.0); }

    ColourValue ColourValue::MediumSeaGreen()
        { return ColourValue(0.235294,0.701960,0.443137,1.0); }

    ColourValue ColourValue::MediumSlateBlue()
        { return ColourValue(0.482352,0.407843,0.933333,1.0); }

    ColourValue ColourValue::MediumSpringGreen()
        { return ColourValue(0.0,0.980392,0.603921,1.0); }

    ColourValue ColourValue::MediumTurquoise()
        { return ColourValue(0.282352,0.819607,0.8,1.0); }

    ColourValue ColourValue::MediumVioletRed()
        { return ColourValue(0.780392,0.082352,0.521568,1.0); }

    ColourValue ColourValue::MidnightBlue()
        { return ColourValue(0.098039,0.098039,0.439215,1.0); }

    ColourValue ColourValue::MintCream()
        { return ColourValue(0.960784,1.0,0.980392,1.0); }

    ColourValue ColourValue::MistyRose()
        { return ColourValue(1.0,0.894117,0.882352,1.0); }

    ColourValue ColourValue::Moccasin()
        { return ColourValue(1.0,0.894117,0.709803,1.0); }

    ColourValue ColourValue::NavajoWhite()
        { return ColourValue(1.0,0.870588,0.678431,1.0); }

    ColourValue ColourValue::Navy()
        { return ColourValue(0.0,0.0,0.501960,1.0); }

    ColourValue ColourValue::OldLace()
        { return ColourValue(0.992156,0.960784,0.901960,1.0); }

    ColourValue ColourValue::Olive()
        { return ColourValue(0.501960,0.501960,0.0,1.0); }

    ColourValue ColourValue::OliveDrab()
        { return ColourValue(0.419607,0.556862,0.137254,1.0); }

    ColourValue ColourValue::Orange()
        { return ColourValue(1.0,0.647058,0.0,1.0); }

    ColourValue ColourValue::OrangeRed()
        { return ColourValue(1.0,0.270588,0.0,1.0); }

    ColourValue ColourValue::Orchid()
        { return ColourValue(0.854901,0.439215,0.839215,1.0); }

    ColourValue ColourValue::PaleGoldenRod()
        { return ColourValue(0.933333,0.909803,0.666666,1.0); }

    ColourValue ColourValue::PaleGreen()
        { return ColourValue(0.596078,0.984313,0.596078,1.0); }

    ColourValue ColourValue::PaleTurquoise()
        { return ColourValue(0.686274,0.933333,0.933333,1.0); }

    ColourValue ColourValue::PaleVioletRed()
        { return ColourValue(0.858823,0.439215,0.576470,1.0); }

    ColourValue ColourValue::PapayaWhip()
        { return ColourValue(1.0,0.937254,0.835294,1.0); }

    ColourValue ColourValue::PeachPuff()
        { return ColourValue(1.0,0.854901,0.725490,1.0); }

    ColourValue ColourValue::Peru()
        { return ColourValue(0.803921,0.521568,0.247058,1.0); }

    ColourValue ColourValue::Pink()
        { return ColourValue(1.0,0.752941,0.796078,1.0); }

    ColourValue ColourValue::Plum()
        { return ColourValue(0.866666,0.627450,0.866666,1.0); }

    ColourValue ColourValue::PowderBlue()
        { return ColourValue(0.690196,0.878431,0.901960,1.0); }

    ColourValue ColourValue::Purple()
        { return ColourValue(0.501960,0.0,0.501960,1.0); }

    ColourValue ColourValue::Red()
        { return ColourValue(1.0,0.0,0.0,1.0); }

    ColourValue ColourValue::RosyBrown()
        { return ColourValue(0.737254,0.560784,0.560784,1.0); }

    ColourValue ColourValue::RoyalBlue()
        { return ColourValue(0.254901,0.411764,0.882352,1.0); }

    ColourValue ColourValue::SaddleBrown()
        { return ColourValue(0.545098,0.270588,0.074509,1.0); }

    ColourValue ColourValue::Salmon()
        { return ColourValue(0.980392,0.501960,0.447058,1.0); }

    ColourValue ColourValue::SandyBrown()
        { return ColourValue(0.956862,0.643137,0.376470,1.0); }

    ColourValue ColourValue::SeaGreen()
        { return ColourValue(0.180392,0.545098,0.341176,1.0); }

    ColourValue ColourValue::SeaShell()
        { return ColourValue(1.0,0.960784,0.933333,1.0); }

    ColourValue ColourValue::Sienna()
        { return ColourValue(0.627450,0.311568,0.176470,1.0); }

    ColourValue ColourValue::Silver()
        { return ColourValue(0.752941,0.752941,0.752941,1.0); }

    ColourValue ColourValue::SkyBlue()
        { return ColourValue(0.529411,0.807843,0.921568,1.0); }

    ColourValue ColourValue::SlateBlue()
        { return ColourValue(0.415686,0.352941,0.803921,1.0); }

    ColourValue ColourValue::SlateGray()
        { return ColourValue(0.439215,0.501960,0.564705,1.0); }

    ColourValue ColourValue::Snow()
        { return ColourValue(1.0,0.980392,0.980392,1.0); }

    ColourValue ColourValue::SpringGreen()
        { return ColourValue(0.0,1.0,0.498039,1.0); }

    ColourValue ColourValue::SteelBlue()
        { return ColourValue(0.274509,0.509803,0.705882,1.0); }

    ColourValue ColourValue::Tan()
        { return ColourValue(0.823529,0.705882,0.549019,1.0); }

    ColourValue ColourValue::Teal()
        { return ColourValue(0.0,0.501960,0.501960,1.0); }

    ColourValue ColourValue::Thistle()
        { return ColourValue(0.847058,0.749019,0.847058,1.0); }

    ColourValue ColourValue::Tomato()
        { return ColourValue(1.0,0.388235,0.278431,1.0); }

    ColourValue ColourValue::Turquoise()
        { return ColourValue(0.250980,0.878431,0.815686,1.0); }

    ColourValue ColourValue::Violet()
        { return ColourValue(0.933333,0.509803,0.933333,1.0); }

    ColourValue ColourValue::Wheat()
        { return ColourValue(0.960784,0.870588,0.701960,1.0); }

    ColourValue ColourValue::White()
        { return ColourValue(1.0,1.0,1.0,1.0); }

    ColourValue ColourValue::WhiteSmoke()
        { return ColourValue(0.960784,0.960784,0.960784,1.0); }

    ColourValue ColourValue::Yellow()
        { return ColourValue(1.0,1.0,0.0,1.0); }

    ColourValue ColourValue::YellowGreen()
        { return ColourValue(0.603921,0.803921,0.196078,1.0); }

#ifdef MEZZXML
    // Serializable
    void ColourValue::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        Mezzanine::xml::Node VecNode = CurrentRoot.AppendChild(this->ColourValue::SerializableName());
        VecNode.SetName(this->ColourValue::SerializableName());

        Mezzanine::xml::Attribute VersionAttr = VecNode.AppendAttribute("Version");
        Mezzanine::xml::Attribute RAttr = VecNode.AppendAttribute("Red");
        Mezzanine::xml::Attribute GAttr = VecNode.AppendAttribute("Green");
        Mezzanine::xml::Attribute BAttr = VecNode.AppendAttribute("Blue");
        Mezzanine::xml::Attribute AAttr = VecNode.AppendAttribute("Alpha");
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
        if ( Mezzanine::String(OneNode.Name())==this->ColourValue::SerializableName() )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->R=OneNode.GetAttribute("Red").AsReal();
                this->G=OneNode.GetAttribute("Green").AsReal();
                this->B=OneNode.GetAttribute("Blue").AsReal();
                this->A=OneNode.GetAttribute("Alpha").AsReal();
            }else{
                throw( Mezzanine::Exception(StringTool::StringCat("Incompatible XML Version for ",this->ColourValue::SerializableName(),": Not Version 1")) );
            }
        }else{
            throw( Mezzanine::Exception(Mezzanine::StringTool::StringCat("Attempting to deserialize a ",this->ColourValue::SerializableName(),", found a ", OneNode.Name())));
        }
    }

    String ColourValue::SerializableName()
        { return String("ColourValue"); }
#endif


}

///////////////////////////////////////////////////////////////////////////////
// Class External << Operators for streaming or assignment
#ifdef MEZZXML
std::ostream& operator << (std::ostream& stream, const Mezzanine::ColourValue& Ev)
{
    Serialize(stream,Ev);
    return stream;
}

std::istream& MEZZ_LIB operator >> (std::istream& stream, Mezzanine::ColourValue& Ev)
    { return DeSerialize(stream, Ev); }

void operator >> (const Mezzanine::xml::Node& OneNode, Mezzanine::ColourValue& Ev)
    { Ev.ProtoDeSerialize(OneNode); }
#endif // \MEZZXML


#endif
