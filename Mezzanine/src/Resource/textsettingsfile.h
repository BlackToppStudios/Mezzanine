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
#ifndef _resourcetextsettingsfile_h
#define _resourcetextsettingsfile_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Resource
    {
        /// @class TextSettingsFile
        /// @headerfile resourcetextsettingsfile.h
        /// @brief
        /// @details
        class MEZZ_LIB TextSettingsFile
        {
            public:
                typedef std::multimap<String,String> SettingsMap;
                typedef std::map<String,SettingsMap*> SettingsSectionMap;
                typedef std::multimap<String,String>::iterator SettingsIterator;
                typedef std::map<String,SettingsMap*>::iterator SectionIterator;
            protected:
                String FileName;
                SettingsSectionMap Sections;
            public:
                /// @brief Class constructor.
                TextSettingsFile();
                /// @brief Class destructor.
                ~TextSettingsFile();

                /// @brief Loads and parses a text file.
                /// @param Filename The name of the file to load.
                /// @param Group The resource group the file is in.
                /// @param Separators The characters that should be observed when separating settings and their values on a single line.
                /// @param TrimWhitespace Whether or not to remove any white spaces before or after settings or their values.
                void Load(const String& Filename, const String& Group, const String& Separators = " \t:=", bool TrimWhitespace = true);
                /// @brief Clears all data from this class.
                void Clear();

                /// @brief Gets the value of the specified setting in the specified section.
                /// @param Key The name of the setting to get the value for.
                /// @param Section The name of the section to search for the Key.
                /// @return Returns a string containing the value for the named setting.
                String GetSetting(const String& Key, const String& Section);
                /// @brief Gets all the values of the specified setting in the specified section.
                /// @param Key The name of the setting to get the value for.
                /// @param Section The name of the section to search for the Key.
                /// @return Returns a vector of strings, all of which are the values of the specified setting.
                std::vector<String> GetMultiSetting(const String& Key, const String& Section);
                /// @brief Gets an iterator to the first section in the file.
                /// @return Returns an iterator to the first section.
                SectionIterator GetSectionBegin();
                /// @brief Gets an iterator to the last section in the file.
                /// @return Returns an iterator to the last section.
                SectionIterator GetSectionEnd();
                /// @brief Gets an iterator to the first setting in the specified Section.
                /// @param Section The name of the section desired.
                /// @return Returns an iterator to the first setting in the named section.
                SettingsIterator GetSettingsBegin(const String& Section);
                /// @brief Gets an iterator to the last setting in the specified Section.
                /// @param Section The name of the section desired.
                /// @return Returns an iterator to the last setting in the named section.
                SettingsIterator GetSettingsEnd(const String& Section);
        };//TextSettingsFile
    }//Resource
}//Mezzanine

#endif
