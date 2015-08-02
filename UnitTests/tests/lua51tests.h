// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _lua51tests_h
#define _lua51tests_h

#include "mezztest.h"

#include "Scripting/scripting.h"
#include "eventbase.h"

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief The group of tests for
class lua51tests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "lua51"
        virtual String Name()
            { return String("Lua51"); }


        void TestLuaScript(const String& Source, const String& FeatureName, const String& FunctionToCall,
                           Real Input = 10, Real ExpectedOutput = 10, Real Epsilon = 0.04,
                           Scripting::Lua::Lua51ScriptingEngine::Lua51Libraries Libset = Scripting::Lua::Lua51ScriptingEngine::DefaultLibs )
        {
            try
            {
                Scripting::Lua::Lua51ScriptingEngine LuaRuntimeSafe(Libset);
                TestOutput << "Testing " << FeatureName << " functionality, with parameter: " << Input << endl;
                Scripting::Lua::Lua51Script RealArgScript(Source, LuaRuntimeSafe);
                LuaRuntimeSafe.Execute(RealArgScript); // Since the script is a functions this readies it to be called
                Scripting::Lua::Lua51Script RealArgCall(FunctionToCall,LuaRuntimeSafe,true);
                RealArgCall.AddArgument(Input);
                LuaRuntimeSafe.Execute(RealArgCall); // This then calls that script with an argument
                //TestOutput << " Recieved " << RealReturn->GetReal() << " while expecting " << ExpectedOutput << endl;
                Real Returned = RealArgCall.GetReturn(0)->GetReal();
                TestOutput << " Recieved " << Returned << " while expecting " << ExpectedOutput << endl;

                TEST((ExpectedOutput==Returned || (Returned-Epsilon<ExpectedOutput && ExpectedOutput<Returned+Epsilon))
                     , String("SWIGWrapped::") + FeatureName);
            } catch (Exception& Error) {
                TestOutput << "Test Failed because of '" << Error.GetExceptionTypeName() << "'" << std::endl
                           << "With Message: " << Error.GetCompleteMessage() << std::endl;
                //Error.what()
                TEST_RESULT(Testing::Failed, String("SWIGWrapped::") + FeatureName);
            }
            TestOutput << "End " << FeatureName << " Test" << endl << endl;
        }

        void TestLuaScript(const String& Source, const String& FeatureName, const String& FunctionToCall,
                           Real Input, Real ExpectedOutput, Real Epsilon,
                           Whole Libset)
        {
            TestLuaScript(Source, FeatureName,FunctionToCall,
                          Input, ExpectedOutput, Epsilon,
                          static_cast<Scripting::Lua::Lua51ScriptingEngine::Lua51Libraries>(Libset));
        }

        void TestLuaLibraryExclusion(const String& Source, const String& LibName, const String& FeatureInLibName,
                                     Scripting::Lua::Lua51ScriptingEngine::Lua51Libraries Libset)
        {
            Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
            Scripting::Lua::Lua51Script FeatureScript(Source);

            TestOutput << endl << "Attempting usage of \"" << FeatureInLibName << "\" from the " << LibName << " Library in a Lua51 Script without that library being loaded." << endl;
            try
            {
                FeatureScript.Compile(LuaRuntimePartial);
                LuaRuntimePartial.Execute(FeatureScript);
                TEST_RESULT(Testing::Failed, String("LibraryLoad::") + LibName + "LibExclude");
            } catch (ScriptLuaException&) {
                TestOutput << endl << "It failed as it should." << endl;
                TEST_RESULT(Success, String("LibraryLoad::") + LibName + "LibExclude");
            }

            LuaRuntimePartial.OpenLibraries(Libset);

            TestOutput << "Attempting normal execution with properly loaded " << LibName << " library function." << endl;
            try
            {
                FeatureScript.Compile(LuaRuntimePartial);
                LuaRuntimePartial.Execute(FeatureScript);
                TEST_RESULT(Success, String("LibraryLoad::") + LibName + "LibInclude");
            } catch (ScriptLuaException& e) {
                TestOutput << "Test failed: " << e.what() << endl;
                TEST_RESULT(Testing::Failed, String("LibraryLoad::") + LibName + "LibInclude");
            }
            TestOutput << endl << endl;
        }

        void TestLuaLibraryExclusion(const String& Source, const String& LibName, const String& FeatureInLibName, int Libset)
        {
            TestLuaLibraryExclusion(Source, LibName, FeatureInLibName,
                                    static_cast<Scripting::Lua::Lua51ScriptingEngine::Lua51Libraries>(Libset));
        }

        /// @warning Basic tests that the Lua scripting system can do basic work.
        void RunAutomaticTests()
        {
            Scripting::Lua::Lua51ScriptingEngine LuaRuntimeSafe(Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);
            //Scripting::Lua::Lua51ScriptingEngine LuaRuntimeAll(Scripting::Lua::Lua51ScriptingEngine::AllLibs);

            String HelloWorldSource("print (\"Hello World, From Lua5.1\")");

            // Lua script Default constructor skipped
            // Skipped FlaggedBuffer completely.

            //////////////////////////////////////////////////////////////////////////////////////////
            // Static Data Tests
            {
                TEST(String("Lua51ScriptingEngine")==LuaRuntimeSafe.GetImplementationTypeName(), "Engine::ImplementationName");
                TestOutput << "Checking lib enum values:" << endl;
                Whole DefaultExpected = 541 + 2048 + 8192 + 32768 + 131072;
                TestOutput << "Default Expected: " << DefaultExpected << "\tActual:" << Scripting::Lua::Lua51ScriptingEngine::DefaultLibs << endl;
                TEST(DefaultExpected==Scripting::Lua::Lua51ScriptingEngine::DefaultLibs, "Engine::LuaLibEnumDefault");

                int TargetLib=0;
                for(int lib = Scripting::Lua::Lua51ScriptingEngine::FirstLib;
                    lib <= Scripting::Lua::Lua51ScriptingEngine::LastLib;
                    lib*=2)
                { TargetLib+=lib; }
                TestOutput << "All Expected: " << TargetLib << "\tActual:" << Scripting::Lua::Lua51ScriptingEngine::AllLibs << endl;
                TEST((int)Scripting::Lua::Lua51ScriptingEngine::AllLibs==TargetLib, "Engine::LuaLibEnumAll");

                String Valid1("a");
                String Valid2("S");
                String Valid3("So_This_is_Valid123124");
                String Valid4("_is_valid");
                String Invalid1("");
                String Invalid2("1_is_not_valid");
                String Invalid3("is_n%t_valid");
                String Invalid4("_is_not_valid$");
                String Invalid5("is.not_valid");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Valid1)==true, "Engine::ValidIdentifier1");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Valid2)==true, "Engine::ValidIdentifier2");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Valid3)==true, "Engine::ValidIdentifier3");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Valid4)==true, "Engine::ValidIdentifier4");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Invalid1)==false, "Engine::InvalidIdentifier1");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Invalid2)==false, "Engine::InvalidIdentifier2");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Invalid3)==false, "Engine::InvalidIdentifier3");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Invalid4)==false, "Engine::InvalidIdentifier4");
                TEST(Scripting::Lua::Lua51ScriptingEngine::IsValidIdentifier(Invalid5)==false, "Engine::InvalidIdentifier5");
            }

            //////////////////////////////////////////////////////////////////////////////////////////
            // Compile Tests
            {
                TEST_NO_THROW(Scripting::Lua::Lua51Script Hello(HelloWorldSource,&LuaRuntimeSafe), "Engine::ConstructCompilePointer");
                TEST_NO_THROW(Scripting::Lua::Lua51Script Hello(HelloWorldSource,LuaRuntimeSafe);, "Engine::ConstructCompileReference");
            }

            //////////////////////////////////////////////////////////////////////////////////////////
            // Execute Tests
            {
                TEST_NO_THROW(  Scripting::Lua::Lua51Script Hello(HelloWorldSource,LuaRuntimeSafe);
                                LuaRuntimeSafe.Execute(Hello);
                                ,"Engine::ExecuteFromReference" );

                TEST_NO_THROW(  Scripting::Lua::Lua51Script Hello(HelloWorldSource,LuaRuntimeSafe);
                                LuaRuntimeSafe.Execute(&Hello);
                                ,"Engine::ExecuteFromPointer" );

                TEST_NO_THROW(  CountedPtr<Scripting::Lua::Lua51Script> Hello(new Scripting::Lua::Lua51Script(HelloWorldSource,LuaRuntimeSafe));
                                LuaRuntimeSafe.Execute(Hello);
                                ,"Engine::ExecuteFromCountedPtr" );

                TEST_NO_THROW(  CountedPtr<Scripting::iScript> Hello(new Scripting::Lua::Lua51Script(HelloWorldSource,LuaRuntimeSafe));
                                LuaRuntimeSafe.Execute(Hello);
                                ,"Engine::ExecuteFromCountedPtrCovariant" );

                TEST_NO_THROW(  LuaRuntimeSafe.Execute(HelloWorldSource);
                                AddTestResult("Engine::ExecuteFromSource", Success);
                                ,"Engine::ExecuteFromSource" );
            }

            {
                //////////////////////////////////////////////////////////////////////////////////////////
                // Argument Tests
                try
                {
                    Scripting::Lua::Lua51Script IntArgScript("function TakeArgInt(x)\n"
                                                             "   return x * 2\n"
                                                             "end"
                                                             ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(IntArgScript);

                    Scripting::Lua::Lua51Script IntArgCall("TakeArgInt",LuaRuntimeSafe,true);
                    //IntArgCall.AddArgument(Scripting::Lua::Lua51IntegerArgument(9));
                    IntArgCall.AddArgument(9);
                    LuaRuntimeSafe.Execute(IntArgCall);

                    TEST(18==IntArgCall.GetReturn(0)->GetInteger(),"Argument::PassInt");

                } catch (ScriptLuaException&) {
                    TEST_RESULT(Testing::Failed,"Argument::PassInt");
                }

                try
                {
                    Scripting::Lua::Lua51Script WholeArgScript("function TakeArgWhole(x)\n"
                                                               "   return x * 2\n"
                                                               "end"
                                                               ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(WholeArgScript);

                    Scripting::Lua::Lua51Script WholeArgCall("TakeArgWhole",LuaRuntimeSafe,true);
                    WholeArgCall.AddArgument(9);
                    LuaRuntimeSafe.Execute(WholeArgCall);

                    TEST(18==WholeArgCall.GetReturn(0)->GetWhole(), "Argument::PassWhole");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Argument::PassWhole");
                }

                try
                {
                    Scripting::Lua::Lua51Script RealArgScript("function TakeArgReal(x)\n"
                                                              "   return x * 2\n"
                                                              "end"
                                                              ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(RealArgScript);

                    Scripting::Lua::Lua51Script RealArgCall("TakeArgReal",LuaRuntimeSafe,true);
                    RealArgCall.AddArgument(Real(9.5));
                    LuaRuntimeSafe.Execute(RealArgCall);

                    TEST(19.0==RealArgCall.GetReturn(0)->GetWhole(), "Argument::PassReal");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Argument::PassReal");
                }

                try
                {
                    Scripting::Lua::Lua51Script StringArgScript("function TakeArgString(x)\n"
                                                                "   results = x .. \"World!\"\n"
                                                                "   return results\n"
                                                                "end"
                                                                ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(StringArgScript);

                    Scripting::Lua::Lua51Script StringArgCall("TakeArgString",LuaRuntimeSafe,true);
                    StringArgCall.AddArgument((char*)"Hello ");
                    LuaRuntimeSafe.Execute(StringArgCall);

                    TEST(String("Hello World!")==StringArgCall.GetReturn(0)->GetString(), "Argument::PassString");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Argument::PassString");
                }

                try
                {
                    Scripting::Lua::Lua51Script BoolArgScript("function TakeArgBool(x)\n"
                                                              "   results = not x\n"
                                                              "   return results\n"
                                                              "end"
                                                              ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(BoolArgScript);

                    Scripting::Lua::Lua51Script BoolArgCall("TakeArgBool",LuaRuntimeSafe,true);
                    BoolArgCall.AddArgument(false);
                    LuaRuntimeSafe.Execute(BoolArgCall);

                    TEST(true==BoolArgCall.GetReturn(0)->GetBoole(), "Argument::PassBool");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Argument::PassBool");
                }


                try
                {
                    Scripting::Lua::Lua51Script NilArgScript("function TakeArgNil(x)\n"
                                                             "   if \"nil\"==type(x) then\n"
                                                             "      return nil\n"
                                                             "   else\n"
                                                             "      return 10\n"
                                                             "   end\n"
                                                             "end"
                                                             ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(NilArgScript);

                    Scripting::Lua::Lua51Script NilArgCall("TakeArgNil",LuaRuntimeSafe,true);
                    NilArgCall.AddArgument();
                    LuaRuntimeSafe.Execute(NilArgCall);

                    TEST(false==NilArgCall.GetReturn(0)->GetBoole(),"Argument::PassNil");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Argument::PassNil");
                }

                {
                    TestOutput << "Using some simple scripts to store basic values in Lua variables." << std::endl;
                    Integer StackSizeStart=LuaRuntimeSafe.GetStackCount();
                    LuaRuntimeSafe.Execute("String='A String in String'");
                    LuaRuntimeSafe.Execute("Number=42");
                    LuaRuntimeSafe.Execute("AFloatingPointNumber=4.2");
                    LuaRuntimeSafe.Execute("ANil=nil");
                    LuaRuntimeSafe.Execute("ABoolean=false");
                    TEST(LuaRuntimeSafe.GetStackCount()==StackSizeStart, "Engine::ExecutionDoesntLeakStackEntries");

                    StackSizeStart=LuaRuntimeSafe.GetStackCount();
                    TestOutput << "Retrieving String, expecting 'A String in String' but received: \t" << LuaRuntimeSafe.GetValue("String")->GetString() << std::endl;
                    TEST(String("A String in String")==LuaRuntimeSafe.GetValue("String")->GetString(), "Argument::RetrieveString");

                    TestOutput << "Retrieving Integer, expecting '42' but received: \t\t" << LuaRuntimeSafe.GetValue("Number")->GetInteger() << std::endl;
                    TEST(42==LuaRuntimeSafe.GetValue("Number")->GetInteger(), "Argument::RetrieveInteger");

                    TestOutput << "Retrieving Real, expecting '4.2' but received: \t\t" << LuaRuntimeSafe.GetValue("AFloatingPointNumber")->GetReal() << std::endl;
                    TEST_EQUAL_EPSILON(Real(4.2), (LuaRuntimeSafe.GetValue("AFloatingPointNumber")->GetReal()), "Argument::RetrieveReal");

                    TestOutput << "Retrieving Nil as a string, expecting 'Nil' but received: \t" << LuaRuntimeSafe.GetValue("ANil")->GetString() << std::endl;
                    TEST(String("Nil")==LuaRuntimeSafe.GetValue("ANil")->GetString(), "Argument::RetrieveNil");

                    TestOutput << "Retrieving Boole, expecting 'false' but received: \t\t" << LuaRuntimeSafe.GetValue("ABoolean")->GetReal() << std::endl;
                    TEST(false==LuaRuntimeSafe.GetValue("ABoolean")->GetBoole(), "Argument::RetrieveBoole");
                    TEST(LuaRuntimeSafe.GetStackCount()==StackSizeStart, "Argument::RetrievalDoesntLeakStackEntries");
                    TestOutput << "" << std::endl;


                }

                {
                    LuaRuntimeSafe.Execute("retrieval_number=1");
                    //Get argument from stack of arbitrary type and of specifictype
                }
            }


            //////////////////////////////////////////////////////////////////////////////////////////
            // Library Tests
            {
                TestLuaLibraryExclusion("assert(1)\n"
                                        "print(type(tostring(1)))\n"
                                        "coroutine.running()\n",
                                        "BaseLib", "Asserts, printing and coroutine,running",
                                        Scripting::Lua::Lua51ScriptingEngine::BaseLib);

                TestLuaLibraryExclusion("x = package.cpath",
                                        "PackageLib", "package.cpath",
                                        Scripting::Lua::Lua51ScriptingEngine::PackageLib);

                TestLuaLibraryExclusion("x = string.lower(\"A\")",
                                        "String", "string.lower",
                                        Scripting::Lua::Lua51ScriptingEngine::StringLib);

                TestLuaLibraryExclusion("num = {1,2,3}\n"
                                        "table.maxn(num)",
                                        "Table", "table.maxn",
                                        Scripting::Lua::Lua51ScriptingEngine::TableLib);

                TestLuaLibraryExclusion("x = math.abs(-10)",
                                        "Math", "math.abs",
                                        Scripting::Lua::Lua51ScriptingEngine::MathLib);

                TestLuaLibraryExclusion("io.type(\"foo.txt\")",
                                        "io", "io.type",
                                        Scripting::Lua::Lua51ScriptingEngine::IOLib);

                TestLuaLibraryExclusion("os.getenv(\"PATH\")",
                                        "os", "os.getenv",
                                        Scripting::Lua::Lua51ScriptingEngine::OSLib);

                TestLuaLibraryExclusion("debug.getfenv(1)",
                                        "debug", "debug.getfenv",
                                        Scripting::Lua::Lua51ScriptingEngine::DebugLib);

                TestLuaLibraryExclusion("x = 3\n"
                                        "Vec=MezzanineSafe.Vector3(3,2,1)\n"
                                        "Vec.X = 10.0\n"
                                        "HeroSizedVec = Vec * 2 --Should be 20,4,2\n"
                                        "HeroSizedVec.Y = 10\n"
                                        "HeroSizedVec.Z = 30\n"
                                        "CrossVec = Vec:CrossProduct(HeroSizedVec)\n",
                                        "MezzanineSafe", "Vector3",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib);

                TestLuaLibraryExclusion("x = 3\n"
                                        "Vec=Mezzanine.Vector3(3,2,1)\n"
                                        "Vec.X=10.0\n"
                                        "HeroSizedVec = Vec * 2 --Should be 20,4,2\n"
                                        "HeroSizedVec.Y = 10\n"
                                        "HeroSizedVec.Z = 30\n"
                                        "CrossVec = Vec:CrossProduct(HeroSizedVec)\n",
                                        "Mezzanine", "Vector3",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzLib);

                TestLuaLibraryExclusion("Doc1=MezzanineSafe.XML.Document()\n",
                                        "MezzanineXMLSafe", "XML.Document",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib|Scripting::Lua::Lua51ScriptingEngine::MezzXMLSafeLib);

                TestLuaLibraryExclusion("Doc1=Mezzanine.XML.Document()\n",
                                        "MezzanineXML", "XML.Document",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzLib|Scripting::Lua::Lua51ScriptingEngine::MezzXMLLib);

                TestLuaLibraryExclusion("mut=MezzanineSafe.Threading.Mutex()\n",
                                        "MezzanineThreadingSafe", "MezzanineSafe.Threading.Mutex",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib|Scripting::Lua::Lua51ScriptingEngine::MezzThreadingSafeLib);

                TestLuaLibraryExclusion("mut=Mezzanine.Threading.Mutex()\n",
                                        "MezzanineThreading", "MezzanineSafe.Threading.Mutex",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzLib|Scripting::Lua::Lua51ScriptingEngine::MezzThreadingLib);

                TestLuaLibraryExclusion("mut=MezzanineSafe.MathTools.AxisAlignedQuad(MezzanineSafe.Axis_X, MezzanineSafe.Vector3(8,0,0), MezzanineSafe.Vector3(8,1,1))\n",
                                        "MezzanineMathToolsSafe", "MezzanineSafe.MathTools.Mutex",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib|Scripting::Lua::Lua51ScriptingEngine::MezzMathToolsSafeLib);

                TestLuaLibraryExclusion("mut=Mezzanine.MathTools.AxisAlignedQuad(Mezzanine.Axis_X, Mezzanine.Vector3(8,0,0), Mezzanine.Vector3(8,1,1))\n",
                                        "MezzanineMathTools", "Mezzanine.MathTools.Mutex",
                                        Scripting::Lua::Lua51ScriptingEngine::MezzLib|Scripting::Lua::Lua51ScriptingEngine::MezzMathToolsLib);

            }

            {
                // Bad syntax tests
                try
                {
                    TestOutput << "Testing Old XML::Document functionality" << endl;
                    Scripting::Lua::Lua51Script RealArgScript("function MakeDoc(x)\n"
                                                              "   Doc1=MezzanineSafe.Document()\n"
                                                              "   return 3\n"
                                                              "end"
                                                              ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(RealArgScript);

                    Scripting::Lua::Lua51Script ScriptCall("MakeDoc",LuaRuntimeSafe,true);
                    ScriptCall.AddArgument(String("NameOfElement"));
                    LuaRuntimeSafe.Execute(ScriptCall);

                    TEST_RESULT(Testing::Failed, "Engine::XMLOldSyntax");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Success,"Engine::XMLOldSyntax");
                }
                TestOutput << "End XML::Document Test" << endl << endl;
            }

            {
                //void TestLuaScript(const String& Source, const String& FeatureName, const String& FunctionToCall,
                //                   Real Input = 10, Real ExpectedOutput = 10, Real Epsilon = 0.04,
                //                   Scripting::Lua::Lua51ScriptingEngine::Lua51Libraries Libset = Scripting::Lua::Lua51ScriptingEngine::DefaultLibs )

                try
                {
                    //Scripting::Lua::Lua51ScriptingEngine LuaRuntimeSafe;
                    TestOutput << "Dumping Contents of Selected SWIG Generated tables from Lua: " << endl;

                    // A Recursive Lua function that iterates of each entry in a Lua table and its subtables and its subtables...
                    String Source("function list_table(tabel, indent)\n"
                                  "    local results = \"\"\n"
                                  "    local indentStr = \"\"\n"
                                  "\n"
                                  "    if(indent == nil) then\n"
                                  "        return list_table(tabel, 0)\n"
                                  "    end\n"
                                  "\n"
                                  "    for i = 0, indent do\n"
                                  "        indentStr = indentStr..\". \"\n"
                                  "    end\n"
                                  "\n"
                                  "    for index,value in pairs(tabel) do\n"
                                  "        if type(value) == \"table\" then\n"
                                  "            results = results..indentStr..index..\": table\\n\"..list_table(value, (indent + 1))\n"
                                  "        elseif type(value) == \"function\" then\n"
                                  "            results = results..indentStr..index..\": function\\n\"\n"
                                  "        else\n"
                                  "            results = results..indentStr..index..\": \"..value..\"\\n\"\n"
                                  "        end\n"
                                  "    end\n"
                                  "    return results\n"
                                  "end\n"
                                  "\n"
                                  "function MezzanineDump(x)\n"
                                  "   results = \"Getting Table Contents of MezzanineSafe:\\n\"\n"
                                  "   results = results..list_table(MezzanineSafe)\n"
                                  "   --print \"Dumping Table Contents of Mezzanine\"\n"
                                  "   --print_table(Mezzanine)\n"
                                  "   return results\n"
                                  "end");

                    Scripting::Lua::Lua51Script DumpScript(Source, LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(DumpScript);
                    Scripting::Lua::Lua51Script DumpFunctionCall("MezzanineDump",LuaRuntimeSafe,true);
                    LuaRuntimeSafe.Execute(DumpFunctionCall);

                    TestOutput << DumpFunctionCall.GetReturn(0) << endl;

                    TEST_RESULT(Testing::Success, "SWIG_Dump");

                    LuaRuntimeSafe.Execute(DumpFunctionCall);

                    TEST_RESULT(Testing::Success, "SWIG_Dump_2ndExecution");


                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed, "SWIG_Dump");
                    TEST_RESULT(Testing::Failed, "SWIG_Dump_2ndFunctionExecution");
                }

            }

            {
                NameValuePairList Parms;
                Scripting::Lua::Lua51ScriptingEngine RuntimeDefault(Parms);

                // Not exhaustive
                TEST(RuntimeDefault.IsLibraryOpen(Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib) &&
                     RuntimeDefault.IsLibraryOpen(Scripting::Lua::Lua51ScriptingEngine::StringLib) &&
                     !RuntimeDefault.IsLibraryOpen(Scripting::Lua::Lua51ScriptingEngine::OSLib)
                    , "NameValuePairConstructionDefault");

                Parms.insert(Parms.begin(), NameValuePair("OS", "Load"));
                Scripting::Lua::Lua51ScriptingEngine RuntimeOnlyOS(Parms);
                TEST(!RuntimeOnlyOS.IsLibraryOpen(Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib) &&
                     !RuntimeOnlyOS.IsLibraryOpen(Scripting::Lua::Lua51ScriptingEngine::StringLib) &&
                     RuntimeOnlyOS.IsLibraryOpen(Scripting::Lua::Lua51ScriptingEngine::OSLib)
                    , "NameValuePairConstructionOS");
            }

            {
                TEST_NO_THROW(
                    Scripting::Lua::Lua51ScriptingEngine LuaRuntimeDoubleRunTest(Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);
                    Scripting::Lua::Lua51Script Att("print('Hello from a script about to be run twice')", &LuaRuntimeDoubleRunTest);
                    LuaRuntimeDoubleRunTest.Execute(Att);
                    LuaRuntimeDoubleRunTest.Execute(Att);
                    , "DoubleScriptExecution");
            }

            // Wrapped Test Selection
            {
                // Specific class swig wrapping tests
                TestLuaScript("function MakePlane(x)\n"
                              "   d=x*3\n"
                              "   Flat1=MezzanineSafe.Plane(\n"
                              "     MezzanineSafe.Vector3(d,0,0),\n"
                              "     MezzanineSafe.Vector3(d,1,0),\n"
                              "     MezzanineSafe.Vector3(d,0,1)\n"
                              "   )\n"
                              "   return Flat1.Distance\n"
                              "end",
                              "Plane", "MakePlane", -3, 9, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Vec1=MezzanineSafe.Vector2(x,3)\n"
                              "   Vec2=MezzanineSafe.Vector2(2,1)\n"
                              "   Vec3=Vec2*Vec1\n"
                              "   return Vec3.X\n"
                              "end",
                              "Vector2", "VecXMultiply", 9.5, 19, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Quat1=MezzanineSafe.Quaternion(x,0,0,0)\n"
                              "   return Quat1:Length()\n"
                              "end",
                              "Quaternion", "VecXMultiply", 10, 10, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Tranny=MezzanineSafe.Transform(\n"
                              "       MezzanineSafe.Vector3(x,0,0),\n"
                              "       MezzanineSafe.Quaternion(0,0,0,0)\n"
                              "   )\n"
                              "   return Tranny.Location:Length()\n"
                              "end",
                              "Transform", "VecXMultiply", 10, 10, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Charles=MezzanineSafe.Ray(\n"
                              "       MezzanineSafe.Vector3(0,0,0),\n"
                              "       MezzanineSafe.Vector3(x,0,0)\n"
                              "   )\n"
                              "   return Charles.Normal.X\n"
                              "end",
                              "Ray", "VecXMultiply", 10, 1, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   GymCoach=MezzanineSafe.StopWatchTimer()\n"
                              "   GymCoach:SetGoalTime(x)\n"
                              "   return GymCoach:GetGoalTime()\n"
                              "end",
                              "Timer", "VecXMultiply", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                // cannot be include until after engine startup can be include
                /*TestLuaScript("function RayDump(x)\n"
                              "   RayCaster=MezzanineSafe.RayQueryTool()\n"
                              "   return x\n"
                              "end",
                              "RayQueryTool", "RayDump", 100, 100,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);
                               */

                /// @todo We need to test that there isn't any mechanism in Lua that circumvents the basic purpose of basic threading mechanisms like Mutex, Barrier, etc...
                TestLuaScript("function TestFuncMutex(x)\n"
                              "   mut=MezzanineSafe.Threading.Mutex()\n"
                              "   mut:Lock()\n"
                              "   mut:Unlock()\n"
                              "   return x\n"
                              "end",
                              "Threading::Mutex", "TestFuncMutex", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestFuncBarrier(x)\n"
                              "   foo=MezzanineSafe.Threading.Barrier(1)\n"
                              "   --No Bar around here!!\n"
                              "   return x\n"
                              "end",
                              "Threading::Barrier", "TestFuncBarrier", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestFuncThread(x)\n"
                              "   MezzanineSafe.Threading.sleep_for(x)\n"
                              "   return x\n"
                              "end",
                              "Threading::Thread", "TestFuncThread", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestFuncAvg(x)\n"
                              "   Avg=MezzanineSafe.Threading.BufferedRollingAverageWhole(3)\n"
                              "   Avg:Insert( 9+x)\n"
                              "   Avg:Insert(10+x)\n"
                              "   Avg:Insert(11+x)\n"
                              "   return Avg:GetAverage()\n"
                              "end",
                              "Threading::BufferedRollingAverageWhole", "TestFuncAvg", 100, 110, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestFuncSyscall(x)\n"
                              "   return MezzanineSafe.Threading.GetCPUCount()\n"
                              "end",
                              "Threading::SystemCalls", "TestFuncSyscall", GetCPUCount(), GetCPUCount(), 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestFuncWorkUnitKey(x)\n"
                              "   key=MezzanineSafe.Threading.WorkUnitKey()\n"
                              "   return x\n"
                              "end",
                              "Threading::WorkUnitKey", "TestFuncWorkUnitKey", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestFuncFrameScheduler(x)\n"
                              "   FS=MezzanineSafe.Threading.FrameScheduler()\n"
                              "   FS:SetFrameRate(60)\n"
                              "   return FS:GetFrameLength()\n"
                              "end",
                              "Threading::FrameScheduler", "TestFuncFrameScheduler", 100, 16666, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

/* // I would like to figure this one out, by I suspect it is impossible.
                TestLuaScript("function TestFuncAtomicAdd(x)\n"
                              "   aa=MezzanineSafe.Threading.AtomicAdd(3,2)\n"
                              "   return 102 \n--MezzanineSafe.Threading.AtomicAdd(x,2)\n"
                              "end",
                              "Threading::AtomicOperations", "TestFuncAtomicAdd", 100, 102,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);
*/
                TestLuaScript("function TestFuncWorkUnitAsync(x)\n"
                              "   key=MezzanineSafe.Threading.AsynchronousFileLoadWorkUnit()\n"
                              "   return x\n"
                              "end",
                              "Threading::WorkUnitAsync", "TestFuncWorkUnitAsync", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestFuncWorkUnitLogAgg(x)\n"
                              "   key=MezzanineSafe.Threading.LogAggregator()\n"
                              "   return x\n"
                              "end",
                              "Threading::WorkUnitLogAggregator", "TestFuncWorkUnitLogAgg", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                //Kinda useless in garbage collected languages, Maybe useful for determining when collection happens
                TestLuaScript("function TestFuncScope(x)\n"
                              "   STimer=MezzanineSafe.Threading.ScopedTimer()"
                              "   return x\n"
                              "end",
                              "Threading::ScopedTimer", "TestFuncScope", 100, 100, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestAABB(x)\n"
                              "   aabb=MezzanineSafe.AxisAlignedBox("
                              "     MezzanineSafe.Vector3(0,0,0),"
                              "     MezzanineSafe.Vector3(x,x,x)"
                              "     )\n"
                              "   return aabb:GetVolume()\n"
                              "end",
                              "AABB", "TestAABB", 2, 8, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestBinaryBuffer(x)\n"
                              "   bb=MezzanineSafe.BinaryBuffer(x)\n"
                              "   return bb:GetSize()\n"
                              "end",
                              "BinaryBuffer", "TestBinaryBuffer", 2, 2, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestActorManager(x)\n"
                              "   bb=MezzanineSafe.ActorManager()\n"
                              "   return x\n"
                              "end",
                              "ActorManager", "TestActorManager", 2, 2, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestColourValue(x)\n"
                              "   cv=MezzanineSafe.ColourValue()\n"
                              "   alicedressbeforegunfight=MezzanineSafe.ColourValue_AliceBlue()\n"
                              "   return alicedressbeforegunfight:GetRedChannel()*255\n"
                              "end",
                              "ColourValue", "TestColourValue", 2, 240, 1.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestAE(x)\n"
                              "   thing=MezzanineSafe.AreaEffectUpdateWorkUnit\n"
                              "   return x\n"
                              "end",
                              "AreaEffect", "TestAE", 2, 2, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestStringTool(x)\n"
                              "   vec=MezzanineSafe.ConvertToVector3(\"3 4 6\")"
                              "   return vec.X\n"
                              "end",
                              "StringTool", "TestStringTool", 3, 3, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestSphere(x)\n"
                              "   bounds=MezzanineSafe.Sphere(MezzanineSafe.Vector3(0,0,0),x)\n"
                              "   if bounds:IsInside(MezzanineSafe.Vector3(1,1,1)) then\n"
                              "     return 5\n"
                              "   else\n"
                              "     return 4\n"
                              "   end\n"
                              "end",
                              "Sphere", "TestSphere", 3, 5, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestMathTool(x)\n"
                              "   return MezzanineMathToolsSafe.Ceil(x)\n"
                              "end",
                              "MathToolCeil", "TestMathTool", 3.5, 4.0, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);


                TestLuaScript("function TestRootEnumerations(x)\n"
                              "   return MezzanineSafe.AT_Zip\n"
                              "end",
                              "RootEnumerations", "TestRootEnumerations", 8, 1, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestEventCompilation(x)\n"
                              "   MezzanineSafe.Event(\"Test\")\n"
                              "   return 1\n"
                              "end",
                              "Event", "TestEventCompilation", 8, 1, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestEventArgCompilation(x)\n"
                              "   b=MezzanineSafe.EventArguments(\"Test\")\n"
                              "   return 1\n"
                              "end",
                              "EventArguments", "TestEventArgCompilation", 8, 1, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestException(x)\n"
                              "  return MezzanineSafe.Exception_IO_EXCEPTION\n"
                              "end",
                              "Exception", "TestException", 8, Mezzanine::Exception::IO_EXCEPTION, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function TestOldEventSystem(x)\n"
                              "  return MezzanineSafe.EventBase_GameWindow\n"
                              "end",
                              "OldEvent", "TestOldEventSystem", 8, Mezzanine::EventBase::GameWindow, 0.0,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

            }

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

