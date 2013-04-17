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
#ifndef _binarytoolstests_h
#define _binarytoolstests_h

#include "main.h"

/// @file
/// @brief Test the BinaryBuffer and associate functions

using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::BinaryTools;

class binarytoolstests : public UnitTestGroup
{
    public:
        virtual String Name()
            { return String("binarytools"); }

        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            String Test1String("Test");
            String Test1Base64("VGVzdA==");

            String Test2String("Mover");
            String Test2Base64("TW92ZXI=");

            String Test3String("Shaker");
            String Test3Base64("U2hha2Vy");

            String GettysburgAddress("Four score and seven years ago our fathers brought forth on this continent a new nation, conceived in liberty, and dedicated to the proposition that all men are created equal. Now we are engaged in a great civil war, testing whether that nation, or any nation, so conceived and so dedicated, can long endure. We are met on a great battle-field of that war. We have come to dedicate a portion of that field, as a final resting place for those who here gave their lives that that nation might live. It is altogether fitting and proper that we should do this. But, in a larger sense, we can not dedicate, we can not consecrate, we can not hallow this ground. The brave men, living and dead, who struggled here, have consecrated it, far above our poor power to add or detract. The world will little note, nor long remember what we say here, but it can never forget what they did here. It is for us the living, rather, to be dedicated here to the unfinished work which they who fought here have thus far so nobly advanced. It is rather for us to be here dedicated to the great task remaining before us—that from these honored dead we take increased devotion to that cause for which they gave the last full measure of devotion—that we here highly resolve that these dead shall not have died in vain—that this nation, under God, shall have a new birth of freedom—and that government of the people, by the people, for the people, shall not perish from the earth.");
            String GettysburgAddressBase64("Rm91ciBzY29yZSBhbmQgc2V2ZW4geWVhcnMgYWdvIG91ciBmYXRoZXJzIGJyb3VnaHQgZm9ydGggb24gdGhpcyBjb250aW5lbnQgYSBuZXcgbmF0aW9uLCBjb25jZWl2ZWQgaW4gbGliZXJ0eSwgYW5kIGRlZGljYXRlZCB0byB0aGUgcHJvcG9zaXRpb24gdGhhdCBhbGwgbWVuIGFyZSBjcmVhdGVkIGVxdWFsLiBOb3cgd2UgYXJlIGVuZ2FnZWQgaW4gYSBncmVhdCBjaXZpbCB3YXIsIHRlc3Rpbmcgd2hldGhlciB0aGF0IG5hdGlvbiwgb3IgYW55IG5hdGlvbiwgc28gY29uY2VpdmVkIGFuZCBzbyBkZWRpY2F0ZWQsIGNhbiBsb25nIGVuZHVyZS4gV2UgYXJlIG1ldCBvbiBhIGdyZWF0IGJhdHRsZS1maWVsZCBvZiB0aGF0IHdhci4gV2UgaGF2ZSBjb21lIHRvIGRlZGljYXRlIGEgcG9ydGlvbiBvZiB0aGF0IGZpZWxkLCBhcyBhIGZpbmFsIHJlc3RpbmcgcGxhY2UgZm9yIHRob3NlIHdobyBoZXJlIGdhdmUgdGhlaXIgbGl2ZXMgdGhhdCB0aGF0IG5hdGlvbiBtaWdodCBsaXZlLiBJdCBpcyBhbHRvZ2V0aGVyIGZpdHRpbmcgYW5kIHByb3BlciB0aGF0IHdlIHNob3VsZCBkbyB0aGlzLiBCdXQsIGluIGEgbGFyZ2VyIHNlbnNlLCB3ZSBjYW4gbm90IGRlZGljYXRlLCB3ZSBjYW4gbm90IGNvbnNlY3JhdGUsIHdlIGNhbiBub3QgaGFsbG93IHRoaXMgZ3JvdW5kLiBUaGUgYnJhdmUgbWVuLCBsaXZpbmcgYW5kIGRlYWQsIHdobyBzdHJ1Z2dsZWQgaGVyZSwgaGF2ZSBjb25zZWNyYXRlZCBpdCwgZmFyIGFib3ZlIG91ciBwb29yIHBvd2VyIHRvIGFkZCBvciBkZXRyYWN0LiBUaGUgd29ybGQgd2lsbCBsaXR0bGUgbm90ZSwgbm9yIGxvbmcgcmVtZW1iZXIgd2hhdCB3ZSBzYXkgaGVyZSwgYnV0IGl0IGNhbiBuZXZlciBmb3JnZXQgd2hhdCB0aGV5IGRpZCBoZXJlLiBJdCBpcyBmb3IgdXMgdGhlIGxpdmluZywgcmF0aGVyLCB0byBiZSBkZWRpY2F0ZWQgaGVyZSB0byB0aGUgdW5maW5pc2hlZCB3b3JrIHdoaWNoIHRoZXkgd2hvIGZvdWdodCBoZXJlIGhhdmUgdGh1cyBmYXIgc28gbm9ibHkgYWR2YW5jZWQuIEl0IGlzIHJhdGhlciBmb3IgdXMgdG8gYmUgaGVyZSBkZWRpY2F0ZWQgdG8gdGhlIGdyZWF0IHRhc2sgcmVtYWluaW5nIGJlZm9yZSB1c+KAlHRoYXQgZnJvbSB0aGVzZSBob25vcmVkIGRlYWQgd2UgdGFrZSBpbmNyZWFzZWQgZGV2b3Rpb24gdG8gdGhhdCBjYXVzZSBmb3Igd2hpY2ggdGhleSBnYXZlIHRoZSBsYXN0IGZ1bGwgbWVhc3VyZSBvZiBkZXZvdGlvbuKAlHRoYXQgd2UgaGVyZSBoaWdobHkgcmVzb2x2ZSB0aGF0IHRoZXNlIGRlYWQgc2hhbGwgbm90IGhhdmUgZGllZCBpbiB2YWlu4oCUdGhhdCB0aGlzIG5hdGlvbiwgdW5kZXIgR29kLCBzaGFsbCBoYXZlIGEgbmV3IGJpcnRoIG9mIGZyZWVkb23igJRhbmQgdGhhdCBnb3Zlcm5tZW50IG9mIHRoZSBwZW9wbGUsIGJ5IHRoZSBwZW9wbGUsIGZvciB0aGUgcGVvcGxlLCBzaGFsbCBub3QgcGVyaXNoIGZyb20gdGhlIGVhcnRoLg==");
            //BinaryTools::BinaryBuffer TestBuffer(TestBase64);

            std::cout << "Predicted Base64 size: " << PredictBase64StringSizeFromBinarySize(GettysburgAddress.length()) << std::endl
                      << "Base64 size: " << GettysburgAddressBase64.length() << std::endl
                      << "Predicted String size: " << PredictBinarySizeFromBase64String(GettysburgAddressBase64) << std::endl
                      << "String size: " << GettysburgAddress.length() << std::endl
                      << std::endl
                      << PredictBase64StringSizeFromBinarySize(Test1String.length())
                      << std::endl
                      << Base64Encode((UInt8 const*)Test1String.c_str(), Test1String.length())
                      << std::endl;



            if (RunAutomaticTests)
            {
                TestResult temp;

                if( 8 == PredictBase64StringSizeFromBinarySize(Test1String.length())    &&
                    8 == PredictBase64StringSizeFromBinarySize(Test2String.length())    &&
                    8 == PredictBase64StringSizeFromBinarySize(Test3String.length())    &&
                    1952 == PredictBase64StringSizeFromBinarySize(GettysburgAddress.length())
                  )
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("BinaryTools::PredictBase64StringSizeFromBinarySize", temp);

                if( 4 == PredictBinarySizeFromBase64String(Test1Base64)          &&
                    5 == PredictBinarySizeFromBase64String(Test2Base64)          &&
                    6 == PredictBinarySizeFromBase64String(Test3Base64)          &&
                    1462 == PredictBinarySizeFromBase64String(GettysburgAddressBase64)
                  )
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("BinaryTools::PredictBinarySizeFromBase64String", temp);
            }else{
                AddTestResult("BinaryTools::PredictBinarySizeFromBase64String", Skipped);
                AddTestResult("BinaryTools::PredictBase64StringSizeFromBinarySize", Skipped);
            }
        }
};

#endif

