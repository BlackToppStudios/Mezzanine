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
#ifndef _binarytoolstests_h
#define _binarytoolstests_h

#include "mezztest.h"
#include "binarybuffer.h"
#include "crossplatform.h"
#include "exception.h"

/// @file
/// @brief Test the BinaryBuffer and associate functions

using namespace std;
using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::BinaryTools;


// Code change to Match BTS naming conventions and formatting
// Copyright information in binarytools.cpp

/// @brief The library of characters used in Rene's Base64 implementation
static const String Base64Chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


// Code change to Match BTS naming conventions and formatting
// Copyright information in binarytools.cpp
/// @brief Rene's Base64 implementation for comparison purposes.
/// @param BytesToEncode A buffer of bytes which is to be encoded. NULL bytes are encoded and not used for termination
/// @param Length The Length of the buffer pointed to by BytesToEncode
/// @return A String containing the encoded value.
String ReneBase64Encode(UInt8 const* BytesToEncode, unsigned int Length)
{
    String ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (Length--)
    {
        char_array_3[i++] = *(BytesToEncode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                { ret += Base64Chars[char_array_4[i]]; }
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            { char_array_3[j] = '\0'; }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            { ret += Base64Chars[char_array_4[j]]; }

        while((i++ < 3))
        { ret += '='; }
    }

    return ret;
}

// Code change to Match BTS naming conventions and formatting
// Copyright information in binarytools.cpp
/// @brief A reference implementation of Base64 decoding as provide by Rene.
/// @param EncodedString The String to be decoded as provided by ReneBase64Encode()
/// @ref A string Containing the resulting binary.
String ReneBase64Decode(String const& EncodedString)
{
    int in_len = EncodedString.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    String ret;

    while (in_len-- && ( EncodedString[in_] != '=') && IsBase64(EncodedString[in_]))
    {
        char_array_4[i++] = EncodedString[in_]; in_++;
        if (i ==4)
        {
            for (i = 0; i <4; i++)
                { char_array_4[i] = Base64Chars.find(char_array_4[i]); }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                { ret += char_array_3[i]; }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j <4; j++)
            { char_array_4[j] = 0; }

        for (j = 0; j <4; j++)
            { char_array_4[j] = Base64Chars.find(char_array_4[j]); }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            { ret += char_array_3[j]; }
    }

    return ret;
    //return BinaryBuffer();
}


class binarytoolstests : public UnitTestGroup
{
    public:
        virtual String Name()
            { return String("BinaryTools"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            bool EveryChar =
                    IsBase64('a') && IsBase64('b') && IsBase64('c') && IsBase64('d') && IsBase64('e') && IsBase64('f') && IsBase64('g') && IsBase64('h') && IsBase64('i') && IsBase64('j') && IsBase64('k') && IsBase64('l') && IsBase64('m') &&
                    IsBase64('n') && IsBase64('o') && IsBase64('p') && IsBase64('q') && IsBase64('r') && IsBase64('s') && IsBase64('t') && IsBase64('u') && IsBase64('v') && IsBase64('w') && IsBase64('x') && IsBase64('y') && IsBase64('z') &&
                    IsBase64('A') && IsBase64('B') && IsBase64('C') && IsBase64('D') && IsBase64('E') && IsBase64('F') && IsBase64('G') && IsBase64('H') && IsBase64('I') && IsBase64('J') && IsBase64('K') && IsBase64('L') && IsBase64('M') &&
                    IsBase64('N') && IsBase64('O') && IsBase64('P') && IsBase64('Q') && IsBase64('R') && IsBase64('S') && IsBase64('T') && IsBase64('U') && IsBase64('V') && IsBase64('W') && IsBase64('X') && IsBase64('Y') && IsBase64('Z') &&
                    IsBase64('0') && IsBase64('1') && IsBase64('2') && IsBase64('3') && IsBase64('4') && IsBase64('5') && IsBase64('6') && IsBase64('7') && IsBase64('8') && IsBase64('9') &&
                    IsBase64('/') && IsBase64('=') && IsBase64('+') &&
                    !IsBase64('\n') && !IsBase64('.') &&!IsBase64('^') &&!IsBase64(' ') &&!IsBase64(':') &&!IsBase64('@') &&
                    !IsBase64('\t') && !IsBase64('`') &&!IsBase64('[') &&!IsBase64('<') &&!IsBase64('>') &&!IsBase64(']') &&
                    !IsBase64(0) && !IsBase64(127) &&!IsBase64(150) &&!IsBase64(255) &&!IsBase64(7) &&!IsBase64(10) &&
                    !IsBase64(16) && !IsBase64(27) &&!IsBase64(180) &&!IsBase64(128) &&!IsBase64(32) &&!IsBase64(34);
            TEST( EveryChar, "IsBase64");
            const String Test1String("Test");
            const String Test1Base64("VGVzdA==");

            {  // Basic Buffer Consistency
                BinaryBuffer DefaultTest;
                TEST( 0==DefaultTest.Size && 0==DefaultTest.Binary,"BinaryBuffer::DefaultConstructor");

                DefaultTest.Size=4;
                DefaultTest.Binary = new BinaryBuffer::Byte[4];
                *(DefaultTest.Binary+0) = 'T';
                *(DefaultTest.Binary+1) = 'e';
                *(DefaultTest.Binary+2) = 's';
                *(DefaultTest.Binary+3) = 't';
                BinaryBuffer Copied(DefaultTest);
                *(DefaultTest.Binary+0) = 'O';
                *(DefaultTest.Binary+1) = 'l';
                *(DefaultTest.Binary+2) = 'd';
                *(DefaultTest.Binary+3) = '.';
                TEST( 4==Copied.Size &&
                    (*(Copied.Binary+0) == 'T') &&
                    (*(Copied.Binary+1) == 'e') &&
                    (*(Copied.Binary+2) == 's') &&
                    (*(Copied.Binary+3) == 't')
                      ,"BinaryBuffer::CopyConstructor");

                BinaryBuffer StrinTest("Strin",false);
                TEST( 5==StrinTest.Size  &&
                        (*(StrinTest.Binary+0) == 'S') &&
                        (*(StrinTest.Binary+1) == 't') &&
                        (*(StrinTest.Binary+2) == 'r') &&
                        (*(StrinTest.Binary+3) == 'i') &&
                        (*(StrinTest.Binary+4) == 'n')
                      , "BinaryBuffer::StringConstructor");
                TEST( String("Strin") == StrinTest.ToString(), "BinaryBuffer::ToString")

                BinaryBuffer::Byte* CString = new BinaryBuffer::Byte[4];
                CString[0] = 'T';
                CString[1] = 'e';
                CString[2] = 's';
                CString[3] = 't';
                BinaryBuffer BufferConTest(CString,4);
                TEST( String("Test") == BufferConTest.ToString(), "BinaryBuffer::PointerConstructor");

                TEST( BinaryBuffer(200).Size == 200, "BinaryBuffer::SizeConstructor");

                BinaryBuffer AssignmentTest;
                AssignmentTest = BufferConTest;
                BufferConTest[0]='A';
                BufferConTest[1]='B';
                BufferConTest[2]='C';
                BufferConTest[3]='!';
                TEST( String("ABC!") == BufferConTest.ToString(), "BinaryBuffer::operator[]");

                TEST( String("Test") == AssignmentTest.ToString(), "BinaryBuffer::operator=");
                TEST( String("Test") == BinaryBuffer("VGVzdA==").ToString(), "BinaryBuffer::Base64Constructor");

                BinaryBuffer PartA(2);
                BinaryBuffer PartB(2);
                PartA[0]='a';
                PartA[1]='s';
                PartB[0]='d';
                PartB[1]='f';
                PartA+=PartB;
                TEST( String("asdf") == PartA.ToString(), "BinaryBuffer::Concatenation" );

                // Empty Behavior
                BinaryBuffer Blank1;
                TEST( 0 == Blank1.Binary && 0 == Blank1.Size, "BinaryBuffer::BlankDefaultConstruction");

                BinaryBuffer Blank2(Blank1);
                TEST( 0 == Blank2.Binary && 0 == Blank2.Size, "BinaryBuffer::BlankCopyConstruction");

                BinaryBuffer Blank3(4);
                Blank3[0]='F';
                Blank3[1]='u';
                Blank3[2]='l';
                Blank3[3]='l';
                Blank3=Blank1;
                TEST( 0 == Blank3.Binary && 0 == Blank3.Size, "BinaryBuffer::BlankAssignment");

                TestOutput << " Exception test: " << endl;
                TEST_THROW(InvalidStateException&, Blank3.CreateBuffer(), "BinaryBuffer::BlankBufferCreation");
                TestOutput << endl << " End Exception test. " << endl;
            } // \Basic Buffer Consistency

            const String Test2String("Mover");
            const String Test2Base64("TW92ZXI=");

            const String Test3String("Shaker");
            const String Test3Base64("U2hha2Vy");

            const String GettysburgAddress("Four score and seven years ago our fathers brought forth on this continent a new nation, conceived in liberty, and dedicated to the proposition that all men are created equal. Now we are engaged in a great civil war, testing whether that nation, or any nation, so conceived and so dedicated, can long endure. We are met on a great battle-field of that war. We have come to dedicate a portion of that field, as a final resting place for those who here gave their lives that that nation might live. It is altogether fitting and proper that we should do this. But, in a larger sense, we can not dedicate, we can not consecrate, we can not hallow this ground. The brave men, living and dead, who struggled here, have consecrated it, far above our poor power to add or detract. The world will little note, nor long remember what we say here, but it can never forget what they did here. It is for us the living, rather, to be dedicated here to the unfinished work which they who fought here have thus far so nobly advanced. It is rather for us to be here dedicated to the great task remaining before us-that from these honored dead we take increased devotion to that cause for which they gave the last full measure of devotion-that we here highly resolve that these dead shall not have died in vain-that this nation, under God, shall have a new birth of freedom-and that government of the people, by the people, for the people, shall not perish from the earth.");
            const String GettysburgAddressBase64("Rm91ciBzY29yZSBhbmQgc2V2ZW4geWVhcnMgYWdvIG91ciBmYXRoZXJzIGJyb3VnaHQgZm9ydGggb24gdGhpcyBjb250aW5lbnQgYSBuZXcgbmF0aW9uLCBjb25jZWl2ZWQgaW4gbGliZXJ0eSwgYW5kIGRlZGljYXRlZCB0byB0aGUgcHJvcG9zaXRpb24gdGhhdCBhbGwgbWVuIGFyZSBjcmVhdGVkIGVxdWFsLiBOb3cgd2UgYXJlIGVuZ2FnZWQgaW4gYSBncmVhdCBjaXZpbCB3YXIsIHRlc3Rpbmcgd2hldGhlciB0aGF0IG5hdGlvbiwgb3IgYW55IG5hdGlvbiwgc28gY29uY2VpdmVkIGFuZCBzbyBkZWRpY2F0ZWQsIGNhbiBsb25nIGVuZHVyZS4gV2UgYXJlIG1ldCBvbiBhIGdyZWF0IGJhdHRsZS1maWVsZCBvZiB0aGF0IHdhci4gV2UgaGF2ZSBjb21lIHRvIGRlZGljYXRlIGEgcG9ydGlvbiBvZiB0aGF0IGZpZWxkLCBhcyBhIGZpbmFsIHJlc3RpbmcgcGxhY2UgZm9yIHRob3NlIHdobyBoZXJlIGdhdmUgdGhlaXIgbGl2ZXMgdGhhdCB0aGF0IG5hdGlvbiBtaWdodCBsaXZlLiBJdCBpcyBhbHRvZ2V0aGVyIGZpdHRpbmcgYW5kIHByb3BlciB0aGF0IHdlIHNob3VsZCBkbyB0aGlzLiBCdXQsIGluIGEgbGFyZ2VyIHNlbnNlLCB3ZSBjYW4gbm90IGRlZGljYXRlLCB3ZSBjYW4gbm90IGNvbnNlY3JhdGUsIHdlIGNhbiBub3QgaGFsbG93IHRoaXMgZ3JvdW5kLiBUaGUgYnJhdmUgbWVuLCBsaXZpbmcgYW5kIGRlYWQsIHdobyBzdHJ1Z2dsZWQgaGVyZSwgaGF2ZSBjb25zZWNyYXRlZCBpdCwgZmFyIGFib3ZlIG91ciBwb29yIHBvd2VyIHRvIGFkZCBvciBkZXRyYWN0LiBUaGUgd29ybGQgd2lsbCBsaXR0bGUgbm90ZSwgbm9yIGxvbmcgcmVtZW1iZXIgd2hhdCB3ZSBzYXkgaGVyZSwgYnV0IGl0IGNhbiBuZXZlciBmb3JnZXQgd2hhdCB0aGV5IGRpZCBoZXJlLiBJdCBpcyBmb3IgdXMgdGhlIGxpdmluZywgcmF0aGVyLCB0byBiZSBkZWRpY2F0ZWQgaGVyZSB0byB0aGUgdW5maW5pc2hlZCB3b3JrIHdoaWNoIHRoZXkgd2hvIGZvdWdodCBoZXJlIGhhdmUgdGh1cyBmYXIgc28gbm9ibHkgYWR2YW5jZWQuIEl0IGlzIHJhdGhlciBmb3IgdXMgdG8gYmUgaGVyZSBkZWRpY2F0ZWQgdG8gdGhlIGdyZWF0IHRhc2sgcmVtYWluaW5nIGJlZm9yZSB1cy10aGF0IGZyb20gdGhlc2UgaG9ub3JlZCBkZWFkIHdlIHRha2UgaW5jcmVhc2VkIGRldm90aW9uIHRvIHRoYXQgY2F1c2UgZm9yIHdoaWNoIHRoZXkgZ2F2ZSB0aGUgbGFzdCBmdWxsIG1lYXN1cmUgb2YgZGV2b3Rpb24tdGhhdCB3ZSBoZXJlIGhpZ2hseSByZXNvbHZlIHRoYXQgdGhlc2UgZGVhZCBzaGFsbCBub3QgaGF2ZSBkaWVkIGluIHZhaW4tdGhhdCB0aGlzIG5hdGlvbiwgdW5kZXIgR29kLCBzaGFsbCBoYXZlIGEgbmV3IGJpcnRoIG9mIGZyZWVkb20tYW5kIHRoYXQgZ292ZXJubWVudCBvZiB0aGUgcGVvcGxlLCBieSB0aGUgcGVvcGxlLCBmb3IgdGhlIHBlb3BsZSwgc2hhbGwgbm90IHBlcmlzaCBmcm9tIHRoZSBlYXJ0aC4=");

            const String UnicodeString("Iｎｔèｒｎáｔìｏｎàｌïｚâｔｉòｎ");
            const String UnicodeBase64("Se+9ju+9lMOo772S772Ow6HvvZTDrO+9j++9jsOg772Mw6/vvZrDou+9lO+9icOy772O");

            { // Base64
                //Encoding
                TEST( Test1Base64 == Base64Encode(Test1String) &&
                      Test2Base64 == Base64Encode(Test2String) &&
                      Test3Base64 == Base64Encode(Test3String)
                      , "Base64Encode-Short");
                TEST( GettysburgAddressBase64 == Base64Encode(GettysburgAddress), "Base64Encode-Long" );
                TEST( UnicodeBase64 == Base64Encode(UnicodeString) , "Base64Encode-Unicode");

                // Decode
                TEST( Test1String == Base64Decode(Test1Base64).ToString() &&
                      Test2String == Base64Decode(Test2Base64).ToString() &&
                      Test3String == Base64Decode(Test3Base64).ToString()
                      , "Base64Decode-Short");
                BinaryBuffer GettyBuffAddress(Base64Decode(GettysburgAddressBase64));
                TEST(GettysburgAddress == GettyBuffAddress.ToString(), "Base64Decode-Long");
                TEST(UnicodeString == Base64Decode(UnicodeBase64).ToString(), "Base64Decode-Unicode");

                // Constant Time Size Prediction
                TEST( Test1Base64.size() == PredictBase64StringSizeFromBinarySize(Test1String.length()) &&
                      Test2Base64.size() == PredictBase64StringSizeFromBinarySize(Test2String.length()) &&
                      Test3Base64.size() == PredictBase64StringSizeFromBinarySize(Test3String.length())
                        , "PredictBase64StringSizeFromBinarySize-Short");

                TEST( GettysburgAddressBase64.size() == PredictBase64StringSizeFromBinarySize(GettysburgAddress.length())
                      , "PredictBase64StringSizeFromBinarySize-Long")

                TEST( UnicodeBase64.size() == PredictBase64StringSizeFromBinarySize(UnicodeString.length())
                      , "PredictBase64StringSizeFromBinarySize-Unicode")

                TEST( Test1String.size() == PredictBinarySizeFromBase64String(Test1Base64) &&
                      Test2String.size() == PredictBinarySizeFromBase64String(Test2Base64) &&
                      Test3String.size() == PredictBinarySizeFromBase64String(Test3Base64)
                      , "PredictBinarySizeFromBase64String-Short");

                TEST( GettysburgAddress.size() == PredictBinarySizeFromBase64String(GettysburgAddressBase64), "PredictBinarySizeFromBase64String-Long" );
                TEST( UnicodeString.size() == PredictBinarySizeFromBase64String(UnicodeBase64), "PredictBinarySizeFromBase64String-Unicode" );

            } // \ Base64

            { // Base64 performance
                Whole TestCount = 10000;

                MaxInt Begin;
                MaxInt End;
                //MaxInt BTSEncodeTime=0;
                MaxInt BTSDecodeTime=0;
                //MaxInt ReneEncodeTime=0;
                MaxInt ReneDecodeTime=0;
                String OutputS; //To prevent optimizing this whole chunk away.
                BinaryBuffer OutputB; //To prevent optimizing this whole chunk away.

                TestOutput << "Benchmarking this for the sake of performace is dumb and I know it. This is not performance sensitive code, I just wanted to test my ability to gauge subtle non-optimal performance in code when I saw it." << endl << endl;

                Begin = crossplatform::GetTimeStamp();
                for(Whole c=0; c<TestCount; c++)
                    { OutputS = ReneBase64Decode(GettysburgAddressBase64); }
                End = crossplatform::GetTimeStamp();
                ReneDecodeTime = End - Begin;
                TestOutput << "Decoding With Rene's algorithm took " << ReneDecodeTime << " microseconds for " << TestCount << " iterations and results like: " << OutputS.substr(0,20) << "..." << OutputS.substr(OutputS.size()-20,20) << endl;

                Begin = crossplatform::GetTimeStamp();
                for(Whole c=0; c<TestCount; c++)
                {
                    OutputB = Base64Decode(GettysburgAddressBase64);
                    //OutputS = Base64Decode(GettysburgAddressBase64).ToString(); // If you will really one use this for Strings, then unremark this line, remark the next line up and remark the OutputS= line a few lower. It does not really matter.
                }
                End = crossplatform::GetTimeStamp();
                BTSDecodeTime = End - Begin;
                OutputS=OutputB.ToString(); //The conversion to string does not get timed, the reason for this conversion was for use as a binary buffer, not an std::String we need to moved things that allow for direct control of memory.
                TestOutput << "Decoding With BTS algorithm took " << BTSDecodeTime << " microseconds for " << TestCount << " iterations and results like: " << OutputS.substr(0,20) << "..." << OutputS.substr(OutputS.size()-20,20) << endl;

                TestOutput << "The new decoding algorithm takes about " << double((BTSDecodeTime*1000)/(ReneDecodeTime))/10 << "% as long as the original." << endl;

                TEST_WARN( BTSDecodeTime < ReneDecodeTime, "DecodeTime" );

                //Debug on tachyon
                /*  Decoding With Rene's algorithm took 745425 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
                    Decoding With BTS algorithm took 887304 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
                    The new decoding algorithm takes about 119% as long as the original. */
                //Release on tachyon
                /*  Decoding With Rene's algorithm took 462390 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
                    Decoding With BTS algorithm took 344986 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
                    The new decoding algorithm takes about 74.6% as long as the original. */
            } // \Base64 performance
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif

