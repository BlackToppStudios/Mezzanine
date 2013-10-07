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
#ifndef _lua51tests_h
#define _lua51tests_h

#include "mezztest.h"

#include "scripting.h"

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
                           Real Input = 10, Real ExpectedOutput = 10,
                           Scripting::Lua::Lua51ScriptingEngine::Lua51Libraries Libset = Scripting::Lua::Lua51ScriptingEngine::DefaultLibs )
        {
            try
            {
                Scripting::Lua::Lua51ScriptingEngine LuaRuntimeSafe(Libset);
                cout << "Testing " << FeatureName << " functionality" << endl;
                Scripting::Lua::Lua51Script RealArgScript(Source, LuaRuntimeSafe);
                LuaRuntimeSafe.Execute(RealArgScript);
                Scripting::Lua::Lua51Script RealArgCall(FunctionToCall,LuaRuntimeSafe,true);
                RealArgCall.AddArgument(Input);
                CountedPtr<Scripting::Lua::Lua51RealArgument> RealReturn(new Scripting::Lua::Lua51RealArgument);
                RealArgCall.AddReturn(RealReturn);
                LuaRuntimeSafe.Execute(RealArgCall);

                TEST(ExpectedOutput==RealReturn->GetReal(), String("SWIGWrapped::") + FeatureName);
            } catch (ScriptLuaException& ) {
                TEST_RESULT(Testing::Failed, String("SWIGWrapped::") + FeatureName);
            }
            cout << "End " << FeatureName << " Test" << endl << endl;
        }

        void TestLuaLibraryExclusion(const String& Source, const String& LibName, const String& FeatureInLibName,
                                     Scripting::Lua::Lua51ScriptingEngine::Lua51Libraries Libset)
        {
            Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
            Scripting::Lua::Lua51Script FeatureScript(Source);

            cout << endl << "Attempting usage of \"" << FeatureInLibName << "\" from the " << LibName << " Library in a Lua51 Script without that library being loaded." << endl;
            try
            {
                FeatureScript.Compile(LuaRuntimePartial);
                LuaRuntimePartial.Execute(FeatureScript);
                TEST_RESULT(Testing::Failed, String("LibraryLoad::") + LibName + "LibExclude");
            } catch (ScriptLuaException&) {
                cout << endl << "It failed as it should." << endl;
                TEST_RESULT(Success, String("LibraryLoad::") + LibName + "LibExclude");
            }

            LuaRuntimePartial.OpenLibraries(Libset);

            cout << "Attempting normal execution with properly loaded " << LibName << " library function." << endl;
            try
            {
                FeatureScript.Compile(LuaRuntimePartial);
                LuaRuntimePartial.Execute(FeatureScript);
                TEST_RESULT(Success, String("LibraryLoad::") + LibName + "LibInclude");
            } catch (ScriptLuaException& e) {
                cout << "Test failed: " << e.what() << endl;
                TEST_RESULT(Testing::Failed, String("LibraryLoad::") + LibName + "LibInclude");
            }
            cout << endl << endl;
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

            String WorldWorldSource("print (\"Hello World!\")");

            // Lua script Default constructor skipped
            // Skipped FlaggedBuffer completely.

            //////////////////////////////////////////////////////////////////////////////////////////
            // Static Data Tests
            {
                TEST(String("Lua51ScriptingEngine")==LuaRuntimeSafe.GetImplementationTypeName(), "Engine::ImplementationName");
                TEST(541+2048+8192==Scripting::Lua::Lua51ScriptingEngine::DefaultLibs, "Engine::LuaLibEnumDefault");
                TEST(511+1024+4096==Scripting::Lua::Lua51ScriptingEngine::AllLibs, "Engine::LuaLibEnumUnsafe");
            }

            //////////////////////////////////////////////////////////////////////////////////////////
            // Compile Tests
            {
                TEST_NO_THROW(Scripting::Lua::Lua51Script Hello(WorldWorldSource,&LuaRuntimeSafe), "Engine::ConstructCompilePointer");
                TEST_NO_THROW(Scripting::Lua::Lua51Script Hello(WorldWorldSource,LuaRuntimeSafe);, "Engine::ConstructCompileReference");
            }

            //////////////////////////////////////////////////////////////////////////////////////////
            // Execute Tests
            {
                TEST_NO_THROW(  Scripting::Lua::Lua51Script Hello(WorldWorldSource,LuaRuntimeSafe);
                                LuaRuntimeSafe.Execute(Hello);
                                ,"Engine::ExecuteFromReference" );

                TEST_NO_THROW(  Scripting::Lua::Lua51Script Hello(WorldWorldSource,LuaRuntimeSafe);
                                LuaRuntimeSafe.Execute(&Hello);
                                ,"Engine::ExecuteFromPointer" );

                TEST_NO_THROW(  CountedPtr<Scripting::Lua::Lua51Script> Hello(new Scripting::Lua::Lua51Script(WorldWorldSource,LuaRuntimeSafe));
                                LuaRuntimeSafe.Execute(Hello);
                                ,"Engine::ExecuteFromCountedPtr" );

                TEST_NO_THROW(  CountedPtr<Scripting::iScript> Hello(new Scripting::Lua::Lua51Script(WorldWorldSource,LuaRuntimeSafe));
                                LuaRuntimeSafe.Execute(Hello);
                                ,"Engine::ExecuteFromCountedPtrCovariant" );

                TEST_NO_THROW(  LuaRuntimeSafe.Execute(WorldWorldSource);
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
                    CountedPtr<Scripting::Lua::Lua51IntegerArgument> IntReturn(new Scripting::Lua::Lua51IntegerArgument);
                    IntArgCall.AddReturn(IntReturn);
                    LuaRuntimeSafe.Execute(IntArgCall);

                    TEST(18==IntReturn->GetInteger(),"Engine::PassInt");

                } catch (ScriptLuaException&) {
                    TEST_RESULT(Testing::Failed,"Engine::PassInt");
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
                    CountedPtr<Scripting::Lua::Lua51WholeArgument> WholeReturn(new Scripting::Lua::Lua51WholeArgument);
                    WholeArgCall.AddReturn(WholeReturn);
                    LuaRuntimeSafe.Execute(WholeArgCall);

                    TEST(18==WholeReturn->GetWhole(), "Engine::PassWhole");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Engine::PassWhole");
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
                    CountedPtr<Scripting::Lua::Lua51RealArgument> RealReturn(new Scripting::Lua::Lua51RealArgument);
                    RealArgCall.AddReturn(RealReturn);
                    LuaRuntimeSafe.Execute(RealArgCall);

                    TEST(19.0==RealReturn->GetWhole(), "Engine::PassReal");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Engine::PassReal");
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
                    CountedPtr<Scripting::Lua::Lua51StringArgument> StringReturn(new Scripting::Lua::Lua51StringArgument);
                    StringArgCall.AddReturn(StringReturn);
                    LuaRuntimeSafe.Execute(StringArgCall);

                    TEST(String("Hello World!")==StringReturn->GetString(), "Engine::PassString");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Engine::PassString");
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
                    CountedPtr<Scripting::Lua::Lua51BoolArgument> BoolReturn(new Scripting::Lua::Lua51BoolArgument);
                    BoolArgCall.AddReturn(BoolReturn);
                    LuaRuntimeSafe.Execute(BoolArgCall);

                    TEST(true==BoolReturn->GetBool(), "Engine::PassBool");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Engine::PassBool");
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
                    CountedPtr<Scripting::Lua::Lua51NilArgument> NilReturn(new Scripting::Lua::Lua51NilArgument);
                    NilArgCall.AddReturn(NilReturn);
                    LuaRuntimeSafe.Execute(NilArgCall);

                    TEST(false==NilReturn->GetBool(),"Engine::PassNil");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Failed,"Engine::PassNil");
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



                /// @TODO still need to test OS, Debug, Mezz and MezzSafe
                /*
                virtual void OpenOSLibrary();

                /// Lua manual at http://www.lua.org/manual/5.1/manual.html#5.9 .
                virtual void OpenDebugLibrary();
                */
            }

            {
                // Bad syntax tests
                try
                {
                    cout << "Testing Old XML::Document functionality" << endl;
                    Scripting::Lua::Lua51Script RealArgScript("function MakeDoc(x)\n"
                                                              "   Doc1=MezzanineSafe.Document()\n"
                                                              "   return 3\n"
                                                              "end"
                                                              ,LuaRuntimeSafe);
                    LuaRuntimeSafe.Execute(RealArgScript);

                    Scripting::Lua::Lua51Script ScriptCall("MakeDoc",LuaRuntimeSafe,true);
                    ScriptCall.AddArgument(String("NameOfElement"));
                    CountedPtr<Scripting::Lua::Lua51RealArgument> RealReturn(new Scripting::Lua::Lua51RealArgument);
                    ScriptCall.AddReturn(RealReturn);
                    LuaRuntimeSafe.Execute(ScriptCall);

                    TEST_RESULT(Testing::Failed, "Engine::XMLOldSyntax");
                } catch (ScriptLuaException& ) {
                    TEST_RESULT(Testing::Success,"Engine::XMLOldSyntax");
                }
                cout << "End XML::Document Test" << endl << endl;
            }


            {
                // files/Classes not tested
                // worldnode.h
                // worldobjectgraphicssettings.h
                // worldobject.h
                // worldobjectphysicssettings.h
                // Attachable

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
                              "Plane", "MakePlane", -3, 9,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Vec1=MezzanineSafe.Vector2(x,3)\n"
                              "   Vec2=MezzanineSafe.Vector2(2,1)\n"
                              "   Vec3=Vec2*Vec1\n"
                              "   return Vec3.X\n"
                              "end",
                              "Vector2", "VecXMultiply", 9.5, 19,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Quat1=MezzanineSafe.Quaternion(x,0,0,0)\n"
                              "   return Quat1:Length()\n"
                              "end",
                              "Quaternion", "VecXMultiply", 10, 10,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Tranny=MezzanineSafe.Transform(\n"
                              "       MezzanineSafe.Vector3(x,0,0),\n"
                              "       MezzanineSafe.Quaternion(0,0,0,0)\n"
                              "   )\n"
                              "   return Tranny.Location:Length()\n"
                              "end",
                              "Transform", "VecXMultiply", 10, 10,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   Charles=MezzanineSafe.Ray(\n"
                              "       MezzanineSafe.Vector3(0,0,0),\n"
                              "       MezzanineSafe.Vector3(x,0,0)\n"
                              "   )\n"
                              "   return Charles:Length()\n"
                              "end",
                              "Ray", "VecXMultiply", 10, 10,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                TestLuaScript("function VecXMultiply(x)\n"
                              "   GymCoach=MezzanineSafe.StopWatchTimer()\n"
                              "   GymCoach:SetGoalTime(x)\n"
                              "   return GymCoach:GetGoalTime()\n"
                              "end",
                              "Timer", "VecXMultiply", 100, 100,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);

                // cannot be include until after engine startup can be include
                /*TestLuaScript("function RayDump(x)\n"
                              "   RayCaster=MezzanineSafe.RayQueryTool()\n"
                              "   return x\n"
                              "end",
                              "RayQueryTool", "RayDump", 100, 100,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);
                               */

                TestLuaScript("function TestFunc(x)\n"
                              "   mut=MezzanineSafe.Threading.Mutex()\n"
                              "   mut:Lock()\n"
                              "   mut:Unlock()\n"
                              "   return x\n"
                              "end",
                              "Threading::Mutex", "TestFunc", 100, 100,
                               Scripting::Lua::Lua51ScriptingEngine::DefaultLibs);
            }

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

