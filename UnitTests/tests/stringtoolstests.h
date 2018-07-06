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
#ifndef _stringtoolstests_h
#define _stringtoolstests_h

#include "mezztest.h"

#include "stringtool.h"

/// @file
/// @brief This file contains the series of tests to be used with the StringTools namespace.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests testing string manipulation utilities.
class stringtoolstests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "StringTools"
    virtual String Name()
        { return String("StringTools"); }

    /// @brief Test most features in StringTools.
    void RunAutomaticTests()
    {
        {//Character Manipulation and Checks
            TEST(StringTools::IsSpace(' '),"IsSpace(const_Char8)-Pass");
            TEST(!StringTools::IsSpace('\t'),"IsSpace(const_Char8)-Fail");
            TEST(StringTools::IsTab('\t'),"IsTab(const_Char8)-Pass1");
            TEST(StringTools::IsTab('\v'),"IsTab(const_Char8)-Pass2");
            TEST(!StringTools::IsTab('\r'),"IsTab(const_Char8)-Fail");
            TEST(StringTools::IsNewline('\r'),"IsNewline(const_Char8)-Pass1");
            TEST(StringTools::IsNewline('\n'),"IsNewline(const_Char8)-Pass2");
            TEST(!StringTools::IsNewline('!'),"IsNewline(const_Char8)-Fail");
            TEST(StringTools::IsWhitespace(' '),"IsWhitespace(const_Char8)-Pass1");
            TEST(StringTools::IsWhitespace('\t'),"IsWhitespace(const_Char8)-Pass2");
            TEST(StringTools::IsWhitespace('\n'),"IsWhitespace(const_Char8)-Pass3");
            TEST(!StringTools::IsWhitespace('&'),"IsWhitespace(const_Char8)-Fail");
            TEST(StringTools::IsDigit('5'),"IsDigit(const_Char8)-Pass");
            TEST(!StringTools::IsDigit('L'),"IsDigit(const_Char8)-Fail");
            TEST(StringTools::IsLowerAlphaLetter('a'),"IsLowerAlphaLetter(const_Char8)-Pass");
            TEST(!StringTools::IsLowerAlphaLetter('A'),"IsLowerAlphaLetter(const_Char8)-Fail");
            TEST(StringTools::IsUpperAlphaLetter('B'),"IsUpperAlphaLetter(const_Char8)-Pass");
            TEST(!StringTools::IsUpperAlphaLetter('b'),"IsUpperAlphaLetter(const_Char8)-Fail");
            TEST(StringTools::IsAlphaLetter('y'),"IsAlphaLetter(const_Char8)-Pass");
            TEST(!StringTools::IsAlphaLetter('9'),"IsAlphaLetter(const_Char8)-Fail");
            TEST(StringTools::IsLowerHexLetter('e'),"IsLowerHexLetter(const_Char8)-Pass");
            TEST(!StringTools::IsLowerHexLetter('g'),"IsLowerHexLetter(const_Char8)-Fail");
            TEST(StringTools::IsUpperHexLetter('F'),"IsUpperHexLetter(const_Char8)-Pass");
            TEST(!StringTools::IsUpperHexLetter('1'),"IsUpperHexLetter(const_Char8)-Fail");
            TEST(StringTools::IsHexLetter('B'),"IsHexLetter(const_Char8)-Pass");
            TEST(!StringTools::IsHexLetter('z'),"IsHexLetter(const_Char8)-Fail");
            TEST(StringTools::IsHexDigit('8'),"IsHexDigit(const_Char8)-Pass");
            TEST(!StringTools::IsHexDigit('t'),"IsHexDigit(const_Char8)-Fail");
            TEST(StringTools::IsAlphanumeric('R'),"IsAlphanumeric(const_Char8)-Pass");
            TEST(!StringTools::IsAlphanumeric(' '),"IsAlphanumeric(const_Char8)-Fail");
        }//Character Manipulation and Checks

        {//String Manipulation and Checks
            String TrimSourceString("    This is a trim test. \t\t");
            String TrimPassString1("    This is a trim test. \t\t");
            String TrimPassString2("    This is a trim test. \t\t");
            String TrimPassString3("    This is a trim test. \t\t");
            String TrimPassString4("    This is a trim test. \t\t");
            StringTools::Trim(TrimPassString1);//,true,true);
            StringTools::Trim(TrimPassString2,false,true);
            StringTools::Trim(TrimPassString3,true,false);
            StringTools::Trim(TrimPassString4,false,false);
            TEST(TrimPassString1 == "This is a trim test.","Trim(String&,const_Boole,const_Boole)-Pass1");
            TEST(TrimPassString2 == "    This is a trim test.","Trim(String&,const_Boole,const_Boole)-Pass2");
            TEST(TrimPassString3 == "This is a trim test. \t\t","Trim(String&,const_Boole,const_Boole)-Pass3");
            TEST(TrimPassString4 == TrimSourceString,"Trim(String&,const_Boole,const_Boole)-Pass4");

            String SplitSourceString("This is a string that will be split into 11 parts.");
            StringVector DefaultSplitResult = StringTools::Split(SplitSourceString);
            StringVector ISplitResult = StringTools::Split(SplitSourceString,"i",5);// Should normally split 7 times.
            TEST(DefaultSplitResult.size() == 11,"Split(const_String&,const_String&,const_Whole)-DefaultCount");
            TEST(DefaultSplitResult[0] == "This","Split(const_String&,const_String&,const_Whole)-DefaultElement1");
            TEST(DefaultSplitResult[1] == "is","Split(const_String&,const_String&,const_Whole)-DefaultElement2");
            TEST(DefaultSplitResult[2] == "a","Split(const_String&,const_String&,const_Whole)-DefaultElement3");
            TEST(DefaultSplitResult[3] == "string","Split(const_String&,const_String&,const_Whole)-DefaultElement4");
            TEST(DefaultSplitResult[4] == "that","Split(const_String&,const_String&,const_Whole)-DefaultElement5");
            TEST(DefaultSplitResult[5] == "will","Split(const_String&,const_String&,const_Whole)-DefaultElement6");
            TEST(DefaultSplitResult[6] == "be","Split(const_String&,const_String&,const_Whole)-DefaultElement7");
            TEST(DefaultSplitResult[7] == "split","Split(const_String&,const_String&,const_Whole)-DefaultElement8");
            TEST(DefaultSplitResult[8] == "into","Split(const_String&,const_String&,const_Whole)-DefaultElement9");
            TEST(DefaultSplitResult[9] == "11","Split(const_String&,const_String&,const_Whole)-DefaultElement10");
            TEST(DefaultSplitResult[10] == "parts.","Split(const_String&,const_String&,const_Whole)-DefaultElement11");
            TEST(ISplitResult.size() == 6,"Split(const_String&,const_String&,const_Whole)-ISplitCount");
            TEST(ISplitResult[0] == "Th","Split(const_String&,const_String&,const_Whole)-ISplitElement1");
            TEST(ISplitResult[1] == "s ","Split(const_String&,const_String&,const_Whole)-ISplitElement2");
            TEST(ISplitResult[2] == "s a str","Split(const_String&,const_String&,const_Whole)-ISplitElement3");
            TEST(ISplitResult[3] == "ng that w","Split(const_String&,const_String&,const_Whole)-ISplitElement4");
            TEST(ISplitResult[4] == "ll be spl","Split(const_String&,const_String&,const_Whole)-ISplitElement5");
            TEST(ISplitResult[5] == "t into 11 parts.","Split(const_String&,const_String&,const_Whole)-ISplitElement6");

            String ToUpperTestString("This is typed in a reasonable, soft-spoken tone.");
            String ToUpperTestResult("THIS IS TYPED IN A REASONABLE, SOFT-SPOKEN TONE.");
            StringTools::ToUpperCase(ToUpperTestString);
            TEST(ToUpperTestString == ToUpperTestResult,"ToUpperCase(String&)");

            String UpperCopyTestString("This Is An Odd Sentence Typed In Camel-Case; With An Unnecessary Semi-Colon.");
            String UpperCopyTestResult("THIS IS AN ODD SENTENCE TYPED IN CAMEL-CASE; WITH AN UNNECESSARY SEMI-COLON.");
            TEST(StringTools::UpperCaseCopy(UpperCopyTestString) == UpperCopyTestResult,"UpperCaseCopy(String)");

            String ToLowerTestString("CAPS LOCK IS CRUISE CONTROL FOR COOL!");
            String ToLowerTestResult("caps lock is cruise control for cool!");
            StringTools::ToLowerCase(ToLowerTestString);
            TEST(ToLowerTestString == ToLowerTestResult,"ToLowerCase(String&)");

            String LowerCopyTestString("I HAVE A VERY GOOD BRAIN AND HAVE SAID MANY THINGS.");
            String LowerCopyTestResult("i have a very good brain and have said many things.");
            TEST(StringTools::LowerCaseCopy(LowerCopyTestString) == LowerCopyTestResult,"LowerCaseCopy(String)");

            String ToCamelCaseTestString("Test string, do not reply.");
            String ToCamelCaseTestResult("Test String, Do Not Reply.");
            StringTools::ToCamelCase(ToCamelCaseTestString);
            TEST(ToCamelCaseTestString == ToCamelCaseTestResult,"ToCamelCase(String&)");

            String CamelCaseCopyTestString("I'm so meta, even this acronym.");
            String CamelCaseCopyTestResult("I'm So Meta, Even This Acronym.");
            TEST(StringTools::CamelCaseCopy(CamelCaseCopyTestString) == CamelCaseCopyTestResult,"CamelCaseCopy(String)");

            String StartsWithSourceString("Autocannibalism");
            TEST(StringTools::StartsWith(StartsWithSourceString,"auto",false),"StartsWith(const_String&,const_String&,const_Boole)-Pass");
            TEST(StringTools::StartsWith(StartsWithSourceString,"Auto",true),"StartsWith(const_String&,const_String&,const_Boole)-CasePass");
            TEST(!StringTools::StartsWith(StartsWithSourceString,"Self",false),"StartsWith(const_String&,const_String&,const_Boole)-Fail");
            TEST(!StringTools::StartsWith(StartsWithSourceString,"AUTO",true),"StartsWith(const_String&,const_String&,const_Boole)-CaseFail");

            String EndsWithSourceString("ThatsTheJoke.jpg");
            TEST(StringTools::EndsWith(EndsWithSourceString,".JPG",false),"EndsWith(const_String&,const_String&,const_Boole)-Pass");
            TEST(StringTools::EndsWith(EndsWithSourceString,".jpg",true),"EndsWith(const_String&,const_String&,const_Boole)-CasePass");
            TEST(!StringTools::EndsWith(EndsWithSourceString,".png",false),"EndsWith(const_String&,const_String&,const_Boole)-Fail");
            TEST(!StringTools::EndsWith(EndsWithSourceString,".JPG",true),"EndsWith(const_String&,const_String&,const_Boole)-CaseFail");

            String DupWhiteSpaceSourceString("This  is  a needlessly  wide sentence.   ");
            String DupWhiteSpaceResultString("This is a needlessly wide sentence. ");
            StringTools::RemoveDuplicateWhitespaces(DupWhiteSpaceSourceString);
            TEST(DupWhiteSpaceSourceString == DupWhiteSpaceResultString,"RemoveDuplicateWhitespaces(String&)");
        }//String Manipulation and Checks

        {//Pattern Matching
            String FirstSource("Test/Data/Results.txt");
            String SecondSource("WS_29_06_18.dat");
            String ThirdSource("Test\\Data\\Results.txt");
            String FourthSource("*Star*");

            String FirstPattern("Test/Data/*.txt");
            String SecondPattern("*.???");
            String ThirdPattern("*.dat");
            String FourthPattern("ws_[0-9][0-9]_??_*.dat");
            String FifthPattern("Test\\\\Data\\\\*.txt");
            String SixthPattern("\\**\\*");

            TEST(StringTools::PatternMatch(FirstSource,FirstPattern,false),"PatternMatch(const_String&,const_String&,const_Boole)-Pass1");
            TEST(StringTools::PatternMatch(FirstSource,SecondPattern,false),"PatternMatch(const_String&,const_String&,const_Boole)-Pass2");
            TEST(StringTools::PatternMatch(SecondSource,SecondPattern,true),"PatternMatch(const_String&,const_String&,const_Boole)-Pass3");
            TEST(StringTools::PatternMatch(SecondSource,FourthPattern,false),"PatternMatch(const_String&,const_String&,const_Boole)-Pass4");
            TEST(StringTools::PatternMatch(ThirdSource,FifthPattern,false),"PatternMatch(const_String&,const_String&,const_Boole)-Pass5");
            TEST(StringTools::PatternMatch(FourthSource,SixthPattern,true),"PatternMatch(const_String&,const_String&,const_Boole)-Pass6");

            TEST(!StringTools::PatternMatch(FirstSource,ThirdPattern,false),"PatternMatch(const_String&,const_String&,const_Boole)-Fail1");
            TEST(!StringTools::PatternMatch(SecondSource,FourthPattern,true),"PatternMatch(const_String&,const_String&,const_Boole)-Fail2");
            TEST(!StringTools::PatternMatch(FirstSource,ThirdPattern,false),"PatternMatch(const_String&,const_String&,const_Boole)-Fail3");
            TEST(!StringTools::PatternMatch(ThirdSource,SixthPattern,true),"PatternMatch(const_String&,const_String&,const_Boole)-Fail4");
        }//Pattern Matching

        {//Convert-To-Data Functions
            TEST(StringTools::ConvertToBool("Yes",false) == true,"ConvertToBool(const_String&,const_Boole)-Yes");
            TEST(StringTools::ConvertToBool("No",true) == false,"ConvertToBool(const_String&,const_Boole)-No");
            TEST(StringTools::ConvertToBool("1",false) == true,"ConvertToBool(const_String&,const_Boole)-1");
            TEST(StringTools::ConvertToBool("0",true) == false,"ConvertToBool(const_String&,const_Boole)-0");
            TEST(StringTools::ConvertToBool("True",false) == true,"ConvertToBool(const_String&,const_Boole)-True");
            TEST(StringTools::ConvertToBool("False",true) == false,"ConvertToBool(const_String&,const_Boole)-False");
            TEST(StringTools::ConvertToBool("Yes") == true,"ConvertToBool(const_String&,const_Boole)-Default");
            TEST(StringTools::ConvertToBool("Gibberish") == false,"ConvertToBool(const_String&,const_Boole)-Gibberish");
            TEST(StringTools::ConvertToReal("3.1415926") == Real(3.1415926),"ConvertToReal(const_String&)");
            TEST(StringTools::ConvertToInteger("-7272") == Integer(-7272),"ConvertToInteger(const_String&)");
            TEST(StringTools::ConvertToWhole("8383") == Whole(8383),"ConvertToWhole(const_String&)");
            TEST(StringTools::ConvertToInt8("-45") == Int8(-45),"ConvertToInt8(const_String&)");
            TEST(StringTools::ConvertToUInt8("66") == UInt8(66),"ConvertToUInt8(const_String&)");
            TEST(StringTools::ConvertToInt16("-30100") == Int16(-30100),"ConvertToInt16(const_String&)");
            TEST(StringTools::ConvertToUInt16("27015") == UInt16(27015),"ConvertToUInt16(const_String&)");
            TEST(StringTools::ConvertToInt32("-1256844") == Int32(-1256844),"ConvertToInt32(const_String&)");
            TEST(StringTools::ConvertToUInt32("3872423") == UInt32(3872423),"ConvertToUInt32(const_String&)");
        }//Convert-To-Data Functions

        {//Convert-To-String Functions
            TEST(StringTools::ConvertToString(Boole(true)) == "true","ConvertToString(const_T&)-BooleTrue");
            TEST(StringTools::ConvertToString(Boole(false)) == "false","ConvertToString(const_T&)-BooleFalse");
            TEST(StringTools::ConvertToString(Real(3.14)) == "3.14","ConvertToString(const_T&)-Real");
            TEST(StringTools::ConvertToString(Integer(-7272)) == "-7272","ConvertToString(const_T&)-Integer");
            TEST(StringTools::ConvertToString(Whole(8383)) == "8383","ConvertToString(const_T&)-Whole");
            TEST(StringTools::ConvertToString(Int8(-45)) == "-45","ConvertToString(const_T&)-Int8");
            TEST(StringTools::ConvertToString(UInt8(66)) == "66","ConvertToString(const_T&)-UInt8");
            TEST(StringTools::ConvertToString(Int16(-30100)) == "-30100","ConvertToString(const_T&)-Int16");
            TEST(StringTools::ConvertToString(UInt16(27015)) == "27015","ConvertToString(const_T&)-UInt16");
            TEST(StringTools::ConvertToString(Int32(-1256844)) == "-1256844","ConvertToString(const_T&)-Int32");
            TEST(StringTools::ConvertToString(UInt32(3872423)) == "3872423","ConvertToString(const_T&)-UInt32");
        }//Convert-To-String Functions
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//stringtoolstests

#endif
