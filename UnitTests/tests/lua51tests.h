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
                TEST(541==Scripting::Lua::Lua51ScriptingEngine::DefaultLibs, "Engine::LuaLibEnumDefault");
                TEST(511==Scripting::Lua::Lua51ScriptingEngine::AllLibs, "Engine::LuaLibEnumUnsafe");
            }

            //////////////////////////////////////////////////////////////////////////////////////////
            // Compile Tests
            {
                TEST_NO_THROW(Scripting::Lua::Lua51Script Hello(WorldWorldSource,&LuaRuntimeSafe), "Script::ConstructCompilePointer");
                TEST_NO_THROW(Scripting::Lua::Lua51Script Hello(WorldWorldSource,LuaRuntimeSafe);, "Script::ConstructCompileReference");
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
                                AddTestResult("Lua51::Engine::ExecuteFromSource", Success);
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
                String FeatureSource("assert(1)\n"
                                     "print(type(tostring(1)))\n"
                                     "coroutine.running()\n"
                                    );

                Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                cout << "Attempting execution of Base library function without that library being loaded." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Testing::Failed, "Lua51::Script::BaselibExclude"); // Why does this work?
                } catch (ScriptLuaException& e) {
                    cout << endl << "It failed as it should." << endl;
                    TEST_RESULT(Success, "Lua51::Script::BaselibExclude");
                }

                LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::BaseLib);
                //LuaRuntimePartial.OpenBaseLibrary();

                cout << "Attempting normal execution of properly loaded Base library function." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Success, "Lua51::Script::BaselibInclude");
                } catch (ScriptLuaException& e) {
                    TEST_RESULT(Testing::Failed,"Lua51::Script::BaselibInclude");
                }
                cout << endl;
            }

            //Skipped baselib, had some issues

            {
                String FeatureSource("x = string.lower(\"A\")");

                Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                cout << "Attempting execution of String library function without that library being loaded." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Testing::Failed, "Lua51::Script::StringlibExclude"); // Why does this work?
                } catch (ScriptLuaException& e) {
                    cout << endl << "It failed as it should." << endl;
                    TEST_RESULT(Success, "Lua51::Script::StringlibExclude");
                }

                LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::StringLib);
                //LuaRuntimePartial.OpenStringLibrary();

                cout << "Attempting normal execution of properly loaded String library function." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Success, "Lua51::Script::StringlibInclude");
                } catch (ScriptLuaException& e) {
                    TEST_RESULT(Testing::Failed, "Lua51::Script::StringlibInclude");
                }
                cout << endl;
            }


            {
                String FeatureSource("num = {1,2,3}\n"
                                     "table.maxn(num)");

                Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                cout << "Attempting execution of Table library function without that library being loaded." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Testing::Failed, "Lua51::Script::TablelibExclude"); // Why does this work?
                } catch (ScriptLuaException& e) {
                    cout << endl << "It failed as it should." << endl;
                    TEST_RESULT(Success, "Lua51::Script::TablelibExclude");
                }

                LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::TableLib);
                //LuaRuntimePartial.OpenTableLibrary();

                cout << "Attempting normal execution of properly loaded Table library function." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Success, "Lua51::Script::TablelibInclude");
                } catch (ScriptLuaException& e) {
                    TEST_RESULT(Testing::Failed, "Lua51::Script::TablelibInclude");
                }
                cout << endl;
            }

            {
                String FeatureSource("x = math.abs(-10)");

                Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                cout << "Attempting execution of Math library function without that library being loaded." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Testing::Failed, "Lua51::Script::MathlibExclude"); // Why does this work?
                } catch (ScriptLuaException& e) {
                    cout << endl << "It failed as it should." << endl;
                    TEST_RESULT(Success, "Lua51::Script::MathlibExclude");
                }

                LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::MathLib);
                //LuaRuntimePartial.OpenMathLibrary();

                cout << "Attempting normal execution of properly loaded Math library function." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Success, "Lua51::Script::MathlibInclude");
                } catch (ScriptLuaException& e) {
                    TEST_RESULT(Testing::Failed, "Lua51::Script::MathlibInclude");
                }
                cout << endl;
            }

            {
                String FeatureSource("x = 3\n"
                                     "Vec=MezzanineSafe.Vector3(3,2,1)\n"
                                     "Vec.X=10.0\n"
                                     //"print(Vec)\n" // would need to include io for this right?
                                    );

                Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                cout << endl << "Attempting creation of a Vector3 from the MezzanineSafe Library in a Lua51 Script without that library being loaded." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Testing::Failed, "Lua51::Script::MezzanineSafelibExclude"); // Why does this work?
                } catch (ScriptLuaException&) {
                    cout << endl << "It failed as it should." << endl;
                    TEST_RESULT(Success, "Lua51::Script::MezzanineSafelibExclude");
                }

                LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::MezzSafeLib);
                //LuaRuntimePartial.OpenMezzanineSafeLibrary();

                cout << "Attempting normal execution of properly loaded Mezzanine library function." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    cout << endl << "Attempting creation of a Vector3 from the MezzanineSafe Library in a Lua51 Script." << endl;
                    TEST_RESULT(Success, "Lua51::Script::MezzanineSafelibInclude");
                } catch (ScriptLuaException& e) {
                    TEST_RESULT(Testing::Failed,"Lua51::Script::MezzanineSafelibInclude");
                }
                cout << endl;
            }

            {
                String FeatureSource("x = 3\n"
                                     "Vec=Mezzanine.Vector3(3,2,1)\n"
                                     "Vec.X=10.0\n"
                                    );

                Scripting::Lua::Lua51ScriptingEngine LuaRuntimePartial(Scripting::Lua::Lua51ScriptingEngine::NoLib);
                Scripting::Lua::Lua51Script FeatureScript(FeatureSource);

                cout << endl << "Attempting creation of a Vector3 from the Mezzanine Library in a Lua51 Script without that library being loaded." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    TEST_RESULT(Testing::Failed, "Lua51::Script::MezzaninelibExclude"); // Why does this work?
                } catch (ScriptLuaException&) {
                    cout << endl << "It failed as it should." << endl;
                    TEST_RESULT(Success, "Lua51::Script::MezzaninelibExclude");
                }

                LuaRuntimePartial.OpenLibraries(Scripting::Lua::Lua51ScriptingEngine::MezzLib);
                //LuaRuntimePartial.OpenMezzanineLibrary();

                cout << "Attempting normal execution of properly loaded Mezzanine library function." << endl;
                try
                {
                    FeatureScript.Compile(LuaRuntimePartial);
                    LuaRuntimePartial.Execute(FeatureScript);
                    cout << endl << "Attempting creation of a Vector3 from the Mezzanine Library in a Lua51 Script." << endl;
                    TEST_RESULT(Success, "Lua51::Script::MezzaninelibInclude");
                } catch (ScriptLuaException& e) {
                    TEST_RESULT(Testing::Failed,"Lua51::Script::MezzaninelibInclude");
                }
                cout << endl;
            }

            /// @TODO still need to test OS, Debug, Mezz and MezzSafe
            /*
            virtual void OpenOSLibrary();

            /// Lua manual at http://www.lua.org/manual/5.1/manual.html#5.9 .
            virtual void OpenDebugLibrary();
            */
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif

