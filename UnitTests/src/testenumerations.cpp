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
#ifndef _unittestenumerations_cpp
#define _unittestenumerations_cpp

/// @file
/// @brief The implmentation of a few functions used with the enumuration in the unit tests

#include "mezzanine.h"

#include "testenumerations.h"

namespace Mezzanine
{
    namespace Testing
    {
        Mezzanine::String TestResultToString(TestResult Convertable)
        {
            switch(Convertable)
            {
                case Success:
                    return SuccessString;
                case Warning:
                    return WarningString;
                case Skipped:
                    return SkippedString;
                case Cancelled:
                    return CancelledString;
                case Inconclusive:
                    return InconclusiveString;
                case Failed:
                    return FailedString;
                case Unknown:
                    return UnknownString;
                case NotApplicable:
                    return NotApplicableString;
                default:
                    { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to String from TestResult " + Mezzanine::ToString(Convertable)); }
            }
        }

        TestResult StringToTestResult(Mezzanine::String Text)
        {
            if(Text.size()==0)
                { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from empty String"); }

            switch(Text.at(0))
            {
                case 'S':
                    if ( SuccessString == Text )
                        { return Success; }
                    else if ( SkippedString == Text )
                        { return Skipped; }
                    else
                        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text(S) " + Text); }
                case 'W':
                    if ( WarningString == Text )
                        { return Warning;}
                    else
                        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text(W) " + Text); }
                case 'C':
                    if ( CancelledString == Text )
                        { return Cancelled;}
                    else
                        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text(C) " + Text); }
                case 'I':
                    if ( InconclusiveString == Text )
                        { return Inconclusive;}
                    else
                        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text(I) " + Text); }
                case 'U':
                    if ( UnknownString == Text )
                        { return Unknown;}
                    else
                        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text(U) " + Text); }
                case 'F':
                    return Failed;
                    if ( FailedString == Text )
                        { return Failed;}
                    else
                        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text(F) " + Text); }
                case 'N':
                    if ( NotApplicableString == Text )
                        { return NotApplicable;}
                    else
                        { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text(N) " + Text); }
                default:
                    { MEZZ_EXCEPTION(Mezzanine::Exception::PARAMETERS_EXCEPTION,"Cannot convert to TestResult from text() " + Text); }
            }
            return Unknown;
        }

    }// Testing
}// Mezzanine

#endif
