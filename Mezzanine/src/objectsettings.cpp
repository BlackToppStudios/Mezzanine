// Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _objectsettings_cpp
#define _objectsettings_cpp

#include "objectsettings.h"
#include "resourcemanager.h"
#include "Resource/datastream.h"
#include "stringtool.h"
#include "world.h"

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // ObjectSettingSetContainer Methods

    ObjectSettingSetContainer::ObjectSettingSetContainer(const String& Name)
        : ContainerName(Name)
    {
    }

    ObjectSettingSetContainer::~ObjectSettingSetContainer()
    {
        DestroyAllObjectSettingSets();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility
    void ObjectSettingSetContainer::SetName(const String& Name)
    {
        ContainerName = Name;
    }

    ConstString& ObjectSettingSetContainer::GetName() const
    {
        return ContainerName;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Sub-Set Methods
    ObjectSettingSet* ObjectSettingSetContainer::CreateChildObjectSettingSet(const String& Name)
    {
        ObjectSettingSet* NewSubSet = new ObjectSettingSet(Name);
        SubSets.push_back(NewSubSet);
        return NewSubSet;
    }

    void ObjectSettingSetContainer::AddChildObjectSettingSet(ObjectSettingSet* ToBeAdded)
    {
        SubSets.push_back(ToBeAdded);
    }

    ObjectSettingSet* ObjectSettingSetContainer::GetChildObjectSettingSet(const String& Name)
    {
        for( SubSetIterator it = SubSets.begin() ; it != SubSets.end() ; ++it )
        {
            if( Name == (*it)->GetName() )
            {
                return (*it);
            }
        }
    }

    ObjectSettingSet* ObjectSettingSetContainer::GetChildObjectSettingSet(const Whole& Index)
    {
        return SubSets.at(Index);
    }

    void ObjectSettingSetContainer::DestroyChildObjectSettingSet(const String& Name)
    {
        for( SubSetIterator it = SubSets.begin() ; it != SubSets.end() ; ++it )
        {
            if( Name == (*it)->GetName() )
            {
                delete (*it);
                SubSets.erase(it);
                return;
            }
        }
    }

    void ObjectSettingSetContainer::DestroyAllObjectSettingSets()
    {
        for( SubSetIterator it = SubSets.begin() ; it != SubSets.end() ; ++it )
        {
            delete (*it);
        }
        SubSets.clear();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Iterator Access
    ObjectSettingSetContainer::SubSetIterator ObjectSettingSetContainer::SubSetBegin()
    {
        return SubSets.begin();
    }

    ObjectSettingSetContainer::ConstSubSetIterator ObjectSettingSetContainer::SubSetBegin() const
    {
        return SubSets.begin();
    }

    ObjectSettingSetContainer::SubSetIterator ObjectSettingSetContainer::SubSetEnd()
    {
        return SubSets.end();
    }

    ObjectSettingSetContainer::ConstSubSetIterator ObjectSettingSetContainer::SubSetEnd() const
    {
        return SubSets.end();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectSettingSet Methods

    ObjectSettingSet::ObjectSettingSet(const String& Name)
        : ObjectSettingSetContainer(Name),
          ParentSet(NULL)
    {
    }

    ObjectSettingSet::~ObjectSettingSet()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility
    ObjectSettingSet* ObjectSettingSet::GetParentSet() const
    {
        return ParentSet;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Setting Methods
    void ObjectSettingSet::SetSettingValue(const String& SettingName, const String& StringValue)
    {
        Settings.insert(std::pair<String,String>(SettingName,StringValue));
    }

    String ObjectSettingSet::GetSettingValue(const String& SettingName) const
    {
        ConstSettingsIterator SetIt = Settings.find(SettingName);
        if( SetIt != Settings.end() ) return (*SetIt).second;
        else return "";
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Sub-Set Methods
    ObjectSettingSet* ObjectSettingSet::CreateChildObjectSettingSet(const String& Name)
    {
        ObjectSettingSet* NewSet = ObjectSettingSetContainer::CreateChildObjectSettingSet(Name);
        NewSet->ParentSet = this;
        return NewSet;
    }

    void ObjectSettingSet::AddChildObjectSettingSet(ObjectSettingSet* ToBeAdded)
    {
        ObjectSettingSetContainer::AddChildObjectSettingSet(ToBeAdded);
        ToBeAdded->ParentSet = this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Iterator Access
    ObjectSettingSet::SettingsIterator ObjectSettingSet::SettingsBegin()
    {
        return Settings.begin();
    }

    ObjectSettingSet::ConstSettingsIterator ObjectSettingSet::SettingsBegin() const
    {
        return Settings.begin();
    }

    ObjectSettingSet::SettingsIterator ObjectSettingSet::SettingsEnd()
    {
        return Settings.end();
    }

    ObjectSettingSet::ConstSettingsIterator ObjectSettingSet::SettingsEnd() const
    {
        return Settings.end();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectSettingsHandler Methods

    ObjectSettingsHandler::ObjectSettingsHandler()
        : SaveCurrent(true)
    {
    }

    ObjectSettingsHandler::~ObjectSettingsHandler()
    {
    }
#ifdef MEZZXML
    void ObjectSettingsHandler::LoadSettingsFromFile(const String& FileName, const String& Path)
    {
        // Open the stream and load the document
        Resource::FileStreamDataStream SettingsStream(FileName,Path);
        XML::Document SettingsDoc;
        //SettingsDoc.Load(SettingsStream);
        // Load and parse the settings
        XML::Node RootNode = SettingsDoc.GetChild(GetObjectRootNodeName());
        if(!RootNode.Empty()) LoadSettings(RootNode);
        else
        {
            StringStream exceptionstream;
            exceptionstream << "Attempting to parse XML which does not contain valid ObjectRootNodeName.  Searching for \"" << GetObjectRootNodeName() << "\", not found.  In ObjectSettingsHandler::LoadSettingsFromFile.";
            World::GetWorldPointer()->LogAndThrow(exceptionstream.str());
        }
    }

    void ObjectSettingsHandler::SaveSettingsToFile(StringVector& GroupNames, const String& FileName, const String& Path)
    {
        XML::Document SettingsDoc;
        XML::Node RootNode = SettingsDoc.AppendChild(GetObjectRootNodeName());
        if(!GroupNames.empty()) SaveSettings(GroupNames,RootNode);
        else SaveSettings(RootNode);
        // Open a stream to the saving file
        Resource::FileStreamDataStream SettingsStream(FileName,Path,Resource::DataStream::SF_Truncate);
        SettingsDoc.Save(SettingsStream);
    }

    void ObjectSettingsHandler::LoadSettingSetFromXML(XML::Node& XMLNode, ObjectSettingSet* Set)
    {
        for( XML::AttributeIterator SettingIt = XMLNode.attributes_begin() ; SettingIt != XMLNode.attributes_end() ; ++SettingIt )
        {
            XML::Attribute CurrAttrib = (*SettingIt);
            if(!CurrAttrib.Empty())
                Set->SetSettingValue(CurrAttrib.Name(),CurrAttrib.AsString());
        }
        for( XML::NodeIterator SubSetIt = XMLNode.begin() ; SubSetIt != XMLNode.end() ; ++SubSetIt )
        {
            XML::Node SubSetNode = (*SubSetIt);
            ObjectSettingSet* NewSubSet = Set->CreateChildObjectSettingSet( SubSetNode.Name() );
            LoadSettingSetFromXML( SubSetNode, NewSubSet );
        }
    }

    void ObjectSettingsHandler::SaveSettingSetToXML(XML::Node& XMLNode, ObjectSettingSet* Set)
    {
        for( ObjectSettingSet::SettingsIterator SetIt = Set->SettingsBegin() ; SetIt != Set->SettingsEnd() ; ++SetIt )
        {
            XML::Attribute CurrAttrib = XMLNode.AppendAttribute( (*SetIt).first );
            CurrAttrib.SetValue( (*SetIt).second );
        }
        for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Set->SubSetBegin() ; SubSetIt != Set->SubSetEnd() ; ++SubSetIt )
        {
            XML::Node SubSetNode = XMLNode.AppendChild( (*SubSetIt)->GetName() );
            SaveSettingSetToXML( SubSetNode, (*SubSetIt) );
        }
    }
#endif
    void ObjectSettingsHandler::ApplySettingGroup(const String& GroupName)
    {
        SettingGroupIterator SetIt = SettingGroups.find(GroupName);
        ApplySettingGroupImpl( (*SetIt).second );
    }

    ObjectSettingSetContainer* ObjectSettingsHandler::CreateSettingGroup(const String& Name)
    {
        ObjectSettingSetContainer* NewGroup = new ObjectSettingSetContainer(Name);
        SettingGroups.insert(std::pair<String,ObjectSettingSetContainer*>(Name,NewGroup));
    }

    ObjectSettingSetContainer* ObjectSettingsHandler::GetSettingGroup(const String& Name) const
    {
        ConstSettingGroupIterator SetIt = SettingGroups.find(Name);
        if( SetIt != SettingGroups.end() ) return (*SetIt).second;
        else return NULL;
    }

    void ObjectSettingsHandler::DestroySettingGroup(const String& Name)
    {
        SettingGroupIterator SetIt = SettingGroups.find(Name);
        if( SetIt != SettingGroups.end() )
        {
            delete (*SetIt).second;
            SettingGroups.erase(SetIt);
        }
    }

    void ObjectSettingsHandler::DestroyAllSettingGroups()
    {
        for( SettingGroupIterator SetIt = SettingGroups.begin() ; SetIt != SettingGroups.end() ; ++SetIt )
        {
            delete (*SetIt).second;
        }
        SettingGroups.clear();
    }

#ifdef MEZZXML
    ///////////////////////////////////////////////////////////////////////////////
    // Setting Path Management
    void ObjectSettingsHandler::SetSettingsFilePath(const String& Path)
    {
        size_t FirstDol = Path.find_first_of('$');
        size_t SecondDol = Path.find_first_of('$',FirstDol+1);
        if(FirstDol != String::npos && SecondDol != String::npos)
        {
            String PathKeyWord = Path.substr(FirstDol+1,(SecondDol-FirstDol)-1);
            String RestOfPath = Path.substr(SecondDol+1);
            String ActualPath = ResourceManager::GetSingletonPtr()->ResolveDataPathFromString(PathKeyWord);
            this->SettingsFilePath = ActualPath+RestOfPath;
        }else{
            this->SettingsFilePath = Path;
        }
    }

    const String& ObjectSettingsHandler::GetSettingsFilePath() const
    {
        return this->SettingsFilePath;
    }

    void ObjectSettingsHandler::SetSaveCurrent(bool Save)
    {
        SaveCurrent = Save;
    }

    bool ObjectSettingsHandler::GetSaveCurrent() const
    {
        return SaveCurrent;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Loading Utilities
    void ObjectSettingsHandler::LoadSettingsFromGroup(const String& FileName, const String& Group)
    {
        String FilePath = ResourceManager::GetSingletonPtr()->GetAssetPath(FileName,Group);
        if( FilePath.empty() )
        {
            StringStream exceptionstream;
            exceptionstream << "Failed to find path for file \"" << FileName << "\", in group \"" << Group << "\".  In ObjectSettingsHandler::LoadSettings.";
            World::GetWorldPointer()->LogAndThrow(Exception(exceptionstream.str()));
        }
        LoadSettingsFromFile(FileName,FilePath);
    }

    void ObjectSettingsHandler::LoadSettings(const String& FileName, const String& Path)
    {
        LoadSettingsFromFile(FileName,Path);
    }

    void ObjectSettingsHandler::LoadSettings(const String& FileName)
    {
        if( SettingsFilePath.empty() )
        {
            StringStream exceptionstream;
            exceptionstream << "Attempting to use a preset path that hasn't been set.  In ObjectSettingsHandler::LoadSettings.";
            World::GetWorldPointer()->LogAndThrow(Exception(exceptionstream.str()));
        }
        LoadSettingsFromFile(FileName,SettingsFilePath);
    }

    void ObjectSettingsHandler::LoadSettings(XML::Node& RootSettings)
    {
        ObjectSettingSetContainer* SetAfter = NULL;
        for( XML::NodeIterator CurrGroupIt = RootSettings.begin() ; CurrGroupIt != RootSettings.end() ; ++CurrGroupIt )
        {
            XML::Node CurrGroupNode = (*CurrGroupIt);
            String SettingGroupName = CurrGroupNode.Name();
            ObjectSettingSetContainer* NewGroup = new ObjectSettingSetContainer(SettingGroupName);
            if( "Current" != SettingGroupName )
            {
                SettingGroups.insert(std::pair<String,ObjectSettingSetContainer*>(SettingGroupName,NewGroup));
            }else{
                if(!SetAfter) SetAfter = NewGroup;
                else World::GetWorldPointer()->LogAndThrow(Exception("Multiple \"Current\" setting groups detected while loading settings.  In ObjectSettingsHandler::LoadSettings."));
            }
            for( XML::NodeIterator CurrSetIt = CurrGroupNode.begin() ; CurrSetIt != CurrGroupNode.end() ; ++CurrSetIt )
            {
                XML::Node CurrSetNode = (*CurrSetIt);
                String SettingSetName = CurrSetNode.Name();
                ObjectSettingSet* NewSet = NewGroup->CreateChildObjectSettingSet(SettingSetName);
                LoadSettingSetFromXML( CurrSetNode, NewSet );
            }
        }
        if(SetAfter)
        {
            ApplySettingGroupImpl(SetAfter);
            delete SetAfter;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Saving Utilities
    void ObjectSettingsHandler::SaveSettings(const String& FileName, const String& Path)
    {
        StringVector GroupNames;
        GroupNames.push_back( "Current" );
        for( SettingGroupIterator SetGroupIt = SettingGroups.begin() ; SetGroupIt != SettingGroups.end() ; ++SetGroupIt )
        {
            GroupNames.push_back( (*SetGroupIt).first );
        }
        SaveSettingsToFile(GroupNames,FileName,Path);
    }

    void ObjectSettingsHandler::SaveSettings(const String& FileName)
    {
        if( SettingsFilePath.empty() )
        {
            StringStream exceptionstream;
            exceptionstream << "Attempting to use a preset path that hasn't been set.  In ObjectSettingsHandler::SaveSettings.";
            World::GetWorldPointer()->LogAndThrow(Exception(exceptionstream.str()));
        }

        StringVector GroupNames;
        GroupNames.push_back( "Current" );
        for( SettingGroupIterator SetGroupIt = SettingGroups.begin() ; SetGroupIt != SettingGroups.end() ; ++SetGroupIt )
        {
            GroupNames.push_back( (*SetGroupIt).first );
        }
        SaveSettingsToFile(GroupNames,FileName,SettingsFilePath);
    }

    void ObjectSettingsHandler::SaveSettings(StringVector& GroupNames, const String& FileName, const String& Path)
    {
        SaveSettingsToFile(GroupNames,FileName,Path);
    }

    void ObjectSettingsHandler::SaveSettings(StringVector& GroupNames, const String& FileName)
    {
        if( SettingsFilePath.empty() )
        {
            StringStream exceptionstream;
            exceptionstream << "Attempting to use a preset path that hasn't been set.  In ObjectSettingsHandler::SaveSettings.";
            World::GetWorldPointer()->LogAndThrow(Exception(exceptionstream.str()));
        }

        SaveSettingsToFile(GroupNames,FileName,SettingsFilePath);
    }

    void ObjectSettingsHandler::SaveSettings(XML::Node& RootSettings)
    {
        StringVector GroupNames;
        GroupNames.push_back( "Current" );
        for( SettingGroupIterator GroupNameIt = SettingGroups.begin() ; GroupNameIt != SettingGroups.end() ; ++GroupNameIt )
        {
            GroupNames.push_back( (*GroupNameIt).first );
        }
        SaveSettings(GroupNames,RootSettings);
    }

    void ObjectSettingsHandler::SaveSettings(StringVector& GroupNames, XML::Node& RootSettings)
    {
        for( StringVector::iterator StrIt = GroupNames.begin() ; StrIt != GroupNames.end() ; ++StrIt )
        {
            if( SaveCurrent && "Current" == (*StrIt) )
            {
                XML::Node CurrentNode = CreateCurrentSettings();
                RootSettings.AppendCopy(CurrentNode);
            }else{
                SettingGroupIterator GroupIt = SettingGroups.find( (*StrIt) );
                if( GroupIt == SettingGroups.end() )
                {
                    StringStream exceptionstream;
                    exceptionstream << "Attempting to save setting group \"" << (*StrIt) << "\", which does not exist.  In ObjectSettingsHandler::SaveSettings.";
                    World::GetWorldPointer()->LogAndThrow(Exception(exceptionstream.str()));
                }
                ObjectSettingSetContainer* SaveSet = (*GroupIt).second;
                XML::Node GroupNode = RootSettings.AppendChild( (*GroupIt).first );
                for( ObjectSettingSetContainer::SubSetIterator SubSetIt = SaveSet->SubSetBegin() ; SubSetIt != SaveSet->SubSetEnd() ; ++SubSetIt )
                {
                    XML::Node SetNode = GroupNode.AppendChild( (*SubSetIt)->GetName() );
                    SaveSettingSetToXML( SetNode, (*SubSetIt) );
                }
            }
        }
    }
#endif
}

#endif

