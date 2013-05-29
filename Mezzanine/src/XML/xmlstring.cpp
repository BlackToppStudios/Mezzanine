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
#ifndef _xmlutfconversion_cpp
#define _xmlutfconversion_cpp

/// @file
/// @brief Describe file here

#include "XML/document.h"
#include "XML/xmlstring.h"

#include "exception.h"

namespace Mezzanine
{
    namespace XML
    {

        String GetOneTag(std::istream& stream )     // this function might be optimizes by counting characters, then copying several characters, or something like that.
        {
            Char8 ReadOne = 0;
            Char8 PrevOne = 0;
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
                    if (HaveSlash)                                      // if have passed an unescaped slash then we  are ending a tag
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
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Could not Deserialize XML Stream which should contain:" + ClassName + "\n XML looked Like: " + OneTag + ".");
            }

            Node InputNode = Doc->GetFirstChild();
            if (InputNode)
            {
                if( String(ClassName) == String(InputNode.Name()))
                {
                    return Doc;
                }else{
                    delete Doc;
                    MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,NameSpace + ClassName + " not next item in stream, failed to deserialize.");
                }
            }else{
                delete Doc;
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"No valid XML element in stream, when attempting to deserialize " + NameSpace + ClassName + ".");
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

    }
} // /namespace Mezzanine

#endif // Include guard


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
