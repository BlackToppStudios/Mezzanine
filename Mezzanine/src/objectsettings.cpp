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
#ifndef _objectsettings_cpp
#define _objectsettings_cpp

#include "objectsettings.h"
#include "resourcemanager.h"
#include "Resource/datastream.h"
#include "stringtool.h"

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
    // ObjectSettingGroup Methods

    ObjectSettingGroup::ObjectSettingGroup(const String& Name)
        : ObjectSettingSetContainer(Name)
    {
    }

    ObjectSettingGroup::~ObjectSettingGroup()
    {
    }

    void ObjectSettingGroup::SetOptionalFileName(const String& FileName)
    {
        OptionalFileName = FileName;
    }

    const String& ObjectSettingGroup::GetOptionalFileName() const
    {
        return OptionalFileName;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectSettingFile Methods

    ObjectSettingFile::ObjectSettingFile(const String& FileName)
        : File(FileName)
    {
    }

    ObjectSettingFile::~ObjectSettingFile()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // File and Group Management
    const String& ObjectSettingFile::GetFileName() const
    {
        return File;
    }

    void ObjectSettingFile::AddGroup(ObjectSettingGroup* Group)
    {
        Group->SetOptionalFileName(this->File);
        SaveGroups.push_back(Group);
    }

    ObjectSettingGroup* ObjectSettingFile::GetGroup(const String& Name) const
    {
        for( ConstSaveGroupsIterator SaveIt = SaveGroups.begin() ; SaveIt != SaveGroups.end() ; ++SaveIt )
        {
            if( Name == (*SaveIt)->GetName() )
                return (*SaveIt);
        }
    }

    void ObjectSettingFile::RemoveGroup(ObjectSettingGroup* Group)
    {
        for( SaveGroupsIterator SaveIt = SaveGroups.begin() ; SaveIt != SaveGroups.end() ; ++SaveIt )
        {
            if( Group == (*SaveIt) )
            {
                Group->SetOptionalFileName("");
                SaveGroups.erase(SaveIt);
                return;
            }
        }
    }

    ObjectSettingFile::SaveGroupsContainer& ObjectSettingFile::GetGroups()
    {
        return SaveGroups;
    }

    ObjectSettingFile::SaveGroupsIterator ObjectSettingFile::SaveGroupsBegin()
    {
        return SaveGroups.begin();
    }

    ObjectSettingFile::ConstSaveGroupsIterator ObjectSettingFile::SaveGroupsBegin() const
    {
        return SaveGroups.begin();
    }

    ObjectSettingFile::SaveGroupsIterator ObjectSettingFile::SaveGroupsEnd()
    {
        return SaveGroups.end();
    }

    ObjectSettingFile::ConstSaveGroupsIterator ObjectSettingFile::SaveGroupsEnd() const
    {
        return SaveGroups.end();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectSettingsHandler Methods

    ObjectSettingsHandler::ObjectSettingsHandler()
        : AutoGenPath(true),
          AutoGenFiles(true)
#ifdef MEZZXML
        , SaveCurrent(true)
#endif
    {
    }

    ObjectSettingsHandler::~ObjectSettingsHandler()
    {
    }
#ifdef MEZZXML
    CountedPtr<ObjectSettingsHandler::SettingGroupVector> ObjectSettingsHandler::LoadSettingsFromFile(const String& FileName, const String& Path)
    {
        CountedPtr<SettingGroupVector> RetVec;
        xml::Document SettingsDoc;
        // Open the stream and load the document
        try
        {
            Resource::FileStreamDataStream SettingsStream(FileName,Path);
            SettingsDoc.Load(SettingsStream);
        }
        catch( FileNotFoundException Ex )
        {
            // File doesn't exist, just exit.  If the user wants to autogen it will be autogen'd later.
            return RetVec;
        }

        // Load and parse the settings
        xml::Node RootNode = SettingsDoc.GetChild(GetObjectRootNodeName());
        if(!RootNode.Empty())
        {
            // Store the settings for a moment, since we have some extra stuff to do with them.
            RetVec = LoadSettingsFromXML(RootNode);

            ObjectSettingFile* NewSetFile = new ObjectSettingFile(FileName);
            SettingFiles.insert( std::pair<String,ObjectSettingFile*>(FileName,NewSetFile) );
            for( SettingGroupVector::iterator It = RetVec->begin() ; It != RetVec->end() ; ++It )
                NewSetFile->AddGroup( (*It) );
            return RetVec;
        }else{
            StringStream exceptionstream;
            exceptionstream << "Attempting to parse XML which does not contain valid ObjectRootNodeName.  Searching for \"" << GetObjectRootNodeName() << "\", not found.  In ObjectSettingsHandler::LoadSettingsFromFile.";
            World::GetWorldPointer()->LogAndThrow(exceptionstream.str());
        }
    }

    void ObjectSettingsHandler::SaveSettingsToFile(StringVector& GroupNames, const String& FileName, const String& Path)
    {
        xml::Document SettingsDoc;
        xml::Node RootNode = SettingsDoc.AppendChild(GetObjectRootNodeName());
        if(!GroupNames.empty()) SaveSettingsToXML(GroupNames,RootNode);
        else SaveSettingsToXML(RootNode);
        // Open a stream to the saving file
        Resource::FileStreamDataStream SettingsStream(FileName,Path,Resource::DataStream::SF_Truncate);
        SettingsDoc.Save(SettingsStream,"\t",xml::FormatIndent);
    }

    void ObjectSettingsHandler::LoadSettingSetFromXML(xml::Node& XMLNode, ObjectSettingSet* Set)
    {
        for( xml::AttributeIterator SettingIt = XMLNode.attributes_begin() ; SettingIt != XMLNode.attributes_end() ; ++SettingIt )
        {
            xml::Attribute CurrAttrib = (*SettingIt);
            if(!CurrAttrib.Empty())
                Set->SetSettingValue(CurrAttrib.Name(),CurrAttrib.AsString());
        }
        for( xml::NodeIterator SubSetIt = XMLNode.begin() ; SubSetIt != XMLNode.end() ; ++SubSetIt )
        {
            xml::Node SubSetNode = (*SubSetIt);
            ObjectSettingSet* NewSubSet = Set->CreateChildObjectSettingSet( SubSetNode.Name() );
            LoadSettingSetFromXML( SubSetNode, NewSubSet );
        }
    }

    void ObjectSettingsHandler::SaveSettingSetToXML(xml::Node& XMLNode, ObjectSettingSet* Set)
    {
        for( ObjectSettingSet::SettingsIterator SetIt = Set->SettingsBegin() ; SetIt != Set->SettingsEnd() ; ++SetIt )
        {
            xml::Attribute CurrAttrib = XMLNode.AppendAttribute( (*SetIt).first );
            CurrAttrib.SetValue( (*SetIt).second );
        }
        for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Set->SubSetBegin() ; SubSetIt != Set->SubSetEnd() ; ++SubSetIt )
        {
            xml::Node SubSetNode = XMLNode.AppendChild( (*SubSetIt)->GetName() );
            SaveSettingSetToXML( SubSetNode, (*SubSetIt) );
        }
    }
#endif
    ///////////////////////////////////////////////////////////////////////////////
    // Setting Group Handling
    void ObjectSettingsHandler::ApplySettingGroup(const String& GroupName)
    {
        SettingGroupIterator SetIt = SettingGroups.find(GroupName);
        ApplySettingGroupImpl( (*SetIt).second );
    }

    ObjectSettingGroup* ObjectSettingsHandler::CreateSettingGroup(const String& Name)
    {
        ObjectSettingGroup* NewGroup = new ObjectSettingGroup(Name);
        SettingGroups.insert(std::pair<String,ObjectSettingGroup*>(Name,NewGroup));
    }

    ObjectSettingGroup* ObjectSettingsHandler::GetSettingGroup(const String& Name) const
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

    void ObjectSettingsHandler::DestroySettingGroup(ObjectSettingGroup* ToBeDestroyed)
    {
        this->DestroySettingGroup(ToBeDestroyed->GetName());
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
    // Setting File Handling
    ObjectSettingFile* ObjectSettingsHandler::CreateSettingFile(const String& FileName)
    {
        ObjectSettingFile* NewFile = new ObjectSettingFile(FileName);
        SettingFiles.insert( std::pair<String,ObjectSettingFile*>(FileName,NewFile) );
        return NewFile;
    }

    ObjectSettingFile* ObjectSettingsHandler::GetSettingFile(const String& FileName)
    {
        SettingFilesIterator SettingFileIt = SettingFiles.find(FileName);
        if( SettingFileIt != SettingFiles.end() ) return (*SettingFileIt).second;
        else return NULL;
    }

    void ObjectSettingsHandler::DestroySettingFile(const String& FileName)
    {
        SettingFilesIterator SettingFileIt = SettingFiles.find(FileName);
        if( SettingFileIt != SettingFiles.end() )
        {
            ObjectSettingFile* ToDestroy = (*SettingFileIt).second;
            delete ToDestroy;
            SettingFiles.erase(SettingFileIt);
        }
    }

    void ObjectSettingsHandler::DestroySettingFile(ObjectSettingFile* ToBeDestroyed)
    {
        this->DestroySettingFile(ToBeDestroyed->GetFileName());
    }

    void ObjectSettingsHandler::DestroyAllSettingFiles()
    {
        ObjectSettingFile* ToDestroy = NULL;
        for( SettingFilesIterator SettingFileIt = SettingFiles.begin() ; SettingFileIt != SettingFiles.end() ; ++SettingFileIt )
        {
            ToDestroy = (*SettingFileIt).second;
            delete ToDestroy;
        }
        SettingFiles.clear();
    }

    void ObjectSettingsHandler::AddGroupToFile(ObjectSettingGroup* Group, const String& FileName)
    {
        ObjectSettingFile* GroupFile = NULL;
        if(FileName.empty())
            return;

        SettingFilesIterator SettingFileIt = SettingFiles.find(FileName);
        if( SettingFileIt != SettingFiles.end() ) GroupFile = (*SettingFileIt).second;
        else
        {
            GroupFile = new ObjectSettingFile(FileName);
            SettingFiles.insert( std::pair<String,ObjectSettingFile*>(FileName,GroupFile) );
        }

        GroupFile->AddGroup(Group);
    }

    void ObjectSettingsHandler::RemoveGroupFromFile(ObjectSettingGroup* Group, const String& FileName)
    {
        if(FileName.empty())
            return;

        SettingFilesIterator SettingFileIt = SettingFiles.find(FileName);
        if( SettingFileIt != SettingFiles.end() ) (*SettingFileIt).second->RemoveGroup(Group);
        else return;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Setting Path Handling
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

        if(AutoGenPath)
            ResourceManager::GetSingletonPtr()->CreateDirectoryPath(SettingsFilePath);
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
    CountedPtr<ObjectSettingsHandler::SettingGroupVector> ObjectSettingsHandler::LoadSettingsFromGroup(const String& FileName, const String& Group)
    {
        String FilePath = ResourceManager::GetSingletonPtr()->GetAssetPath(FileName,Group);
        if( FilePath.empty() )
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Failed to find path for file \"" + FileName + "\", in group \"" + Group + "\".");
        }
        return LoadSettingsFromFile(FileName,FilePath);
    }

    CountedPtr<ObjectSettingsHandler::SettingGroupVector> ObjectSettingsHandler::LoadSettings(const String& FileName, const String& Path)
    {
        return LoadSettingsFromFile(FileName,Path);
    }

    CountedPtr<ObjectSettingsHandler::SettingGroupVector> ObjectSettingsHandler::LoadSettings(const String& FileName)
    {
        if( SettingsFilePath.empty() )
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempting to use a preset path that hasn't been set.");
        }
        return LoadSettingsFromFile(FileName,SettingsFilePath);
    }

    CountedPtr<ObjectSettingsHandler::SettingGroupVector> ObjectSettingsHandler::LoadSettingsFromXML(xml::Node& RootSettings)
    {
        ObjectSettingGroup* SetAfter = NULL;
        CountedPtr<SettingGroupVector> RetVec(new SettingGroupVector());
        for( xml::NodeIterator CurrGroupIt = RootSettings.begin() ; CurrGroupIt != RootSettings.end() ; ++CurrGroupIt )
        {
            xml::Node CurrGroupNode = (*CurrGroupIt);
            String SettingGroupName = CurrGroupNode.Name();
            ObjectSettingGroup* NewGroup = new ObjectSettingGroup(SettingGroupName);
            if( "Current" != SettingGroupName )
            {
                SettingGroups.insert(std::pair<String,ObjectSettingGroup*>(SettingGroupName,NewGroup));
                RetVec->push_back(NewGroup);
            }else{
                if(!SetAfter) SetAfter = NewGroup;
                else { MEZZ_EXCEPTION(Exception::II_DUPLICATE_IDENTITY_EXCEPTION,"Multiple \"Current\" setting groups detected while loading settings."); }
            }
            for( xml::NodeIterator CurrSetIt = CurrGroupNode.begin() ; CurrSetIt != CurrGroupNode.end() ; ++CurrSetIt )
            {
                xml::Node CurrSetNode = (*CurrSetIt);
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
        return RetVec;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Saving Utilities
    void ObjectSettingsHandler::SaveAllSettings()
    {
        if( SettingsFilePath.empty() )
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempting to use a preset path that hasn't been set.");
        }

        StringVector GroupNames;
        GroupNames.push_back( "Current" );

        for( SettingFilesIterator SettingFileIt = SettingFiles.begin() ; SettingFileIt != SettingFiles.end() ; ++SettingFileIt )
        {
            GroupNames.clear();
            for( ObjectSettingFile::SaveGroupsIterator SetGroupIt = (*SettingFileIt).second->SaveGroupsBegin() ; SetGroupIt != (*SettingFileIt).second->SaveGroupsEnd() ; ++SetGroupIt )
            {
                GroupNames.push_back( (*SetGroupIt)->GetName() );
            }
            SaveSettingsToFile(GroupNames,(*SettingFileIt).second->GetFileName(),SettingsFilePath);
        }
    }

    void ObjectSettingsHandler::SaveSettingsByFile(const String& FileName, const String& Path)
    {
        StringVector GroupNames;
        GroupNames.push_back( "Current" );

        SettingFilesIterator SetFileIt = SettingFiles.find(FileName);
        if( SetFileIt == SettingFiles.end() )
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Setting file \"" + FileName + "\" was not found when attempting to save.");
        }

        for( ObjectSettingFile::SaveGroupsIterator SetGroupIt = (*SetFileIt).second->SaveGroupsBegin() ; SetGroupIt != (*SetFileIt).second->SaveGroupsEnd() ; ++SetGroupIt )
        {
            GroupNames.push_back( (*SetGroupIt)->GetName() );
        }
        SaveSettingsToFile(GroupNames,FileName,Path);
    }

    void ObjectSettingsHandler::SaveSettingsByFile(const String& FileName)
    {
        if( SettingsFilePath.empty() )
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempting to use a preset path that hasn't been set.");
        }

        StringVector GroupNames;
        GroupNames.push_back( "Current" );

        SettingFilesIterator SetFileIt = SettingFiles.find(FileName);
        if( SetFileIt == SettingFiles.end() )
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Setting file \"" + FileName + "\" was not found when attempting to save.");
        }

        for( ObjectSettingFile::SaveGroupsIterator SetGroupIt = (*SetFileIt).second->SaveGroupsBegin() ; SetGroupIt != (*SetFileIt).second->SaveGroupsEnd() ; ++SetGroupIt )
        {
            GroupNames.push_back( (*SetGroupIt)->GetName() );
        }
        SaveSettingsToFile(GroupNames,FileName,SettingsFilePath);
    }

    void ObjectSettingsHandler::SaveSettingGroups(StringVector& GroupNames, const String& FileName, const String& Path)
    {
        SaveSettingsToFile(GroupNames,FileName,Path);
    }

    void ObjectSettingsHandler::SaveSettingGroups(StringVector& GroupNames, const String& FileName)
    {
        if( SettingsFilePath.empty() )
        {
            MEZZ_EXCEPTION(Exception::INVALID_PARAMETERS_EXCEPTION,"Attempting to use a preset path that hasn't been set.");
        }

        SaveSettingsToFile(GroupNames,FileName,SettingsFilePath);
    }

    void ObjectSettingsHandler::SaveSettingsToXML(xml::Node& RootSettings)
    {
        StringVector GroupNames;
        GroupNames.push_back( "Current" );
        for( SettingGroupIterator GroupNameIt = SettingGroups.begin() ; GroupNameIt != SettingGroups.end() ; ++GroupNameIt )
        {
            GroupNames.push_back( (*GroupNameIt).first );
        }
        SaveSettingsToXML(GroupNames,RootSettings);
    }

    void ObjectSettingsHandler::SaveSettingsToXML(StringVector& GroupNames, xml::Node& RootSettings)
    {
        for( StringVector::iterator StrIt = GroupNames.begin() ; StrIt != GroupNames.end() ; ++StrIt )
        {
            if( SaveCurrent && "Current" == (*StrIt) )
            {
                xml::Node CurrentNode = CreateCurrentSettings();
                RootSettings.AppendCopy(CurrentNode);
            }else{
                SettingGroupIterator GroupIt = SettingGroups.find( (*StrIt) );
                if( GroupIt == SettingGroups.end() )
                {
                    MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,"Attempting to save setting group \"" + (*StrIt) + "\", which does not exist.");
                }
                ObjectSettingSetContainer* SaveSet = (*GroupIt).second;
                xml::Node GroupNode = RootSettings.AppendChild( (*GroupIt).first );
                for( ObjectSettingSetContainer::SubSetIterator SubSetIt = SaveSet->SubSetBegin() ; SubSetIt != SaveSet->SubSetEnd() ; ++SubSetIt )
                {
                    xml::Node SetNode = GroupNode.AppendChild( (*SubSetIt)->GetName() );
                    SaveSettingSetToXML( SetNode, (*SubSetIt) );
                }
            }
        }
    }
#endif
}

#endif

