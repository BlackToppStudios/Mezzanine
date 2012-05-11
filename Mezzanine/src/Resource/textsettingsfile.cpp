//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _resourcetextsettingsfile_cpp
#define _resourcetextsettingsfile_cpp

#include "Resource/textsettingsfile.h"
#include "stringtool.h"
#include "world.h"

#include <OgreResourceGroupManager.h>

namespace Mezzanine
{
    namespace Resource
    {
        TextSettingsFile::TextSettingsFile()
        {
        }

        TextSettingsFile::~TextSettingsFile()
        {
            Clear();
        }

        void TextSettingsFile::Load(const String& Filename, const String& Group, const String& Separators, bool TrimWhitespace)
        {
            /// @todo Once the resource system has been updated to be independant of Ogre's, this function needs to be updated to access our methods instead of theirs.
            Clear();

            FileName = Filename;
            Ogre::DataStreamPtr Stream = Ogre::ResourceGroupManager::getSingletonPtr()->openResource(Filename,Group);
            String CurrentSectionName("");
            SettingsMap* CurrentSection = new SettingsMap();
            Sections[CurrentSectionName] = CurrentSection;

            String Line, SetName, SetVal;
            while(!Stream->eof())
            {
                Line = Stream->getLine();
                if (Line.length() > 0 && Line.at(0) != '#' && Line.at(0) != '@')
                {
                    if (Line.at(0) == '[' && Line.at(Line.length()-1) == ']')
                    {
                        CurrentSectionName = Line.substr(1, Line.length() - 2);
                        SectionIterator SecIt = Sections.find(CurrentSectionName);
                        if (SecIt == Sections.end())
                        {
                            CurrentSection = new SettingsMap();
                            Sections[CurrentSectionName] = CurrentSection;
                        }else{
                            CurrentSection = SecIt->second;
                        }
                    }else{
                        String::size_type SeparatorPos = Line.find_first_of(Separators,0);
                        if(SeparatorPos != String::npos)
                        {
                            SetName = Line.substr(0,SeparatorPos);
                            String::size_type NonSeparatorPos = Line.find_first_not_of(Separators,SeparatorPos);
                            SetVal = (NonSeparatorPos == String::npos) ? "" : Line.substr(NonSeparatorPos);
                            if (TrimWhitespace)
                            {
                                StringTool::Trim(SetVal);
                                StringTool::Trim(SetName);
                            }
                            CurrentSection->insert(SettingsMap::value_type(SetName,SetVal));
                        }
                    }
                }
            }
        }

        void TextSettingsFile::Clear()
        {
            if(Sections.empty())
                return;
            for( TextSettingsFile::SectionIterator SecIt = Sections.begin() ; SecIt != Sections.end() ; ++SecIt )
            {
                delete (*SecIt).second;
            }
            Sections.clear();
            FileName.clear();
        }

        String TextSettingsFile::GetSetting(const String& Key, const String& Section)
        {
            TextSettingsFile::SectionIterator SecIt = Sections.find(Section);
            if(SecIt == Sections.end())
            {
                std::stringstream logstream;
                logstream << "Attempting to access section \"" << Section << "\" in file \"" << FileName << "\" which does not exist.";
                World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
            }else{
                TextSettingsFile::SettingsIterator SetIt = (*SecIt).second->find(Key);
                if(SetIt == (*SecIt).second->end())
                {
                    std::stringstream logstream;
                    logstream << "Attempting to access setting \"" << Key << " in section \"" << Section << "\" in file \"" << FileName << "\" which does not exist.";
                    World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
                }else{
                    return (*SetIt).second;
                }
            }
        }

        std::vector<String> TextSettingsFile::GetMultiSetting(const String& Key, const String& Section)
        {
            std::vector<String> Ret;

            TextSettingsFile::SectionIterator SecIt = Sections.find(Section);
            if(SecIt != Sections.end())
            {
                TextSettingsFile::SettingsIterator SetIt = (*SecIt).second->find(Key);
                while(SetIt != SecIt->second->end() && SetIt->first == Key)
                {
                    Ret.push_back(SetIt->second);
                    ++SetIt;
                }
            }
            return Ret;
        }

        TextSettingsFile::SectionIterator TextSettingsFile::GetSectionBegin()
        {
            return Sections.begin();
        }

        TextSettingsFile::SectionIterator TextSettingsFile::GetSectionEnd()
        {
            return Sections.end();
        }

        TextSettingsFile::SettingsIterator TextSettingsFile::GetSettingsBegin(const String& Section)
        {
            TextSettingsFile::SectionIterator SecIt = Sections.find(Section);
            if(SecIt == Sections.end())
            {
                std::stringstream logstream;
                logstream << "Attempting to access section \"" << Section << "\" in file \"" << FileName << "\" which does not exist.";
                World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
            }else{
                return (*SecIt).second->begin();
            }
        }

        TextSettingsFile::SettingsIterator TextSettingsFile::GetSettingsEnd(const String& Section)
        {
            TextSettingsFile::SectionIterator SecIt = Sections.find(Section);
            if(SecIt == Sections.end())
            {
                std::stringstream logstream;
                logstream << "Attempting to access section \"" << Section << "\" in file \"" << FileName << "\" which does not exist.";
                World::GetWorldPointer()->LogAndThrow(Exception(logstream.str()));
            }else{
                return (*SecIt).second->end();
            }
        }
    }//Resource
}//Mezzanine

#endif
