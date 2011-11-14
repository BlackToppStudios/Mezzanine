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
/// @cond 0
// To provide compilation option via cmake

#ifdef PHYSXML

#ifndef _xmldoc_cpp
#define _xmldoc_cpp

#include "xml.h"
#include "stringtool.h"
#include "world.h"

namespace phys
{

    namespace xml
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @file xmldoc.cpp
        /// @brief Minor tools and addition to assist with the XML system
        /// @details The integration process for PugiXML requires that our changes all be stored in shell script.
        /// This is not condusive to good clean documentation and complicated code chucks and is why the doxygen documention
        /// for the XML parser exist here instead of xml.h.
        ///////////////////////////////////////

        String GetOneTag(std::istream& stream )
        {
            Character ReadOne = 0;
            Character PrevOne = 0;
            Integer TagCount = 0;
            bool HaveSlash = false;
            bool InText = false;

            String OneTag;
            while (!stream.get(ReadOne).fail() && !stream.eof())        //Read one character and if you didn't fail continue the loop
            {
                OneTag.push_back(ReadOne);

                if ('<' == ReadOne && !InText)                          //If a new tag starts
                {
                    ++TagCount;
                }

                if ( '"' == ReadOne && '\\' != PrevOne )                //If we find an unescaped " the we are either leaving or entering a quote
                    { InText = !InText; }

                if ( !InText && '/' == ReadOne )                        //If we are not In a quote then a / means the next > will end a tag
                    { HaveSlash=true; }

                if ( '>' == ReadOne )                                   // if are we at a > then the amount of nested tags we are in changes
                {
                    if (HaveSlash)                                      // if have passed an unescaped slash then we  are ending a tage
                    {
                        --TagCount;
                        if ( 0>=TagCount )                              // if we are ending the last tag then we are done
                        { break; }
                    }else{
                        //++TagCount;                                     // if the tag does not have a slash then we are going one deeper
                    }
                    HaveSlash=false;
                }

                PrevOne=ReadOne;                                        //Store the Previous Character to check for escapes
            }

            return OneTag;

        }

        Document* PreParseClassFromSingleTag(const String& NameSpace, const String& ClassName, const String& OneTag)
        {
            Document* Doc = new Document();
            if(!Doc->Load(OneTag.c_str()))
            {
                delete Doc;
                World::GetWorldPointer()->LogAndThrow(Exception(StringTool::StringCat("Could not Deserialize XML Stream which should contain:", ClassName, "\n XML looked Like: ", OneTag) ));
            }

            Node InputNode = Doc->GetFirstChild();
            if (InputNode)
            {
                if( String(ClassName) == String(InputNode.Name()))
                {
                    return Doc;
                }else{
                    delete Doc;
                    World::GetWorldPointer()->LogAndThrow(Exception(StringTool::StringCat(NameSpace, ClassName, " not next item in stream, failed to deserialize.")));
                }
            }else{
                delete Doc;
                World::GetWorldPointer()->LogAndThrow(Exception(StringTool::StringCat("No valid XML element in stream, when attempting to deserialize ", NameSpace, ClassName)));
            }
        }

        Document* PreParseClassFromSingleTag(const String& ClassName, const String& OneTag)
            { return PreParseClassFromSingleTag("",ClassName,OneTag); }

        String EscapeXML(const String& XMLText)
        {
            String Results;
            for(Whole Counter=0; Counter<XMLText.size(); ++Counter)
            {
                switch(XMLText.at(Counter))
                {
                    case '"':
                        Results.append("&quot;");
                        break;
                    case '&':
                        Results.append("&amp;");
                        break;
                    case '<':
                        Results.append("&lt;");
                        break;
                    case '>':
                        Results.append("&gt;");
                        break;
                    default:
                        Results.append(1,XMLText.at(Counter));
                }
            }
            return Results;
        }

    } // \xml
}   // \phys

#endif

#endif // \PHYSXML
/// @endcond
