#ifndef _profilemanager_cpp
#define _profilemanager_cpp

#include "profilemanager.h"

///////////////////////////////////////////////////////////////////////////////
// CatchProfile Methods

CatchProfile::CatchProfile(const String& Name) :
    ProfileName(Name)
    {  }

CatchProfile::CatchProfile(const XML::Document& ProfileDoc)
{
    XML::Node RootNode = ProfileDoc.GetChild("ProfileRoot");
    if( !RootNode.Empty() ) {
        this->DeSerializeProfileName(RootNode);
        this->DeSerializeLevelScores(RootNode);
    }
}

CatchProfile::~CatchProfile()
    {  }

void CatchProfile::SerializeProfileName(XML::Node& ProfileRoot) const
{
    XML::Node NameNode = ProfileRoot.AppendChild("ProfileName");
    XML::Attribute NameAttrib = NameNode.AppendAttribute("Name");
    NameAttrib.SetValue( this->ProfileName );
}

void CatchProfile::SerializeLevelScores(XML::Node& ProfileRoot) const
{
    XML::Node LevelsNode = ProfileRoot.AppendChild("LevelScores");
    for( ConstLevelScoreMapIterator SecIt = this->LevelScores.begin() ; SecIt != this->LevelScores.end() ; ++SecIt )
    {
        XML::Node LevelElement = LevelsNode.AppendChild( (*SecIt).first );
        XML::Attribute LevelHighScore = LevelElement.AppendAttribute("HighestScore");
        LevelHighScore.SetValue( (*SecIt).second );
    }
}

void CatchProfile::DeSerializeProfileName(const XML::Node& ProfileRoot)
{
    XML::Node NameNode = ProfileRoot.GetChild("ProfileName");
    XML::Attribute NameAttrib = NameNode.GetAttribute("Name");
    this->ProfileName = NameAttrib.AsString();
}

void CatchProfile::DeSerializeLevelScores(const XML::Node& ProfileRoot)
{
    XML::Node LevelsNode = ProfileRoot.GetChild("LevelScores");
    for( XML::NodeIterator NodeIt = LevelsNode.begin() ; NodeIt != LevelsNode.end() ; ++NodeIt )
    {
        std::pair<String,Whole> LevelPair;
        LevelPair.first = (*NodeIt).Name();
        LevelPair.second = (*NodeIt).GetAttribute("HighestScore").AsWhole();
        this->LevelScores.insert(LevelPair);
    }
}

void CatchProfile::Save(const String& ProfilesDir)
{
    XML::Document ProfileDoc;
    XML::Node DocDecl = ProfileDoc.AppendChild(XML::NodeDeclaration);
    DocDecl.SetName("xml");
    XML::Attribute Version = DocDecl.AppendAttribute("version");
    Version.SetValue("1.0");

    XML::Node RootNode = ProfileDoc.AppendChild("ProfileRoot");
    this->SerializeProfileName(RootNode);
    this->SerializeLevelScores(RootNode);

    /// @todo Possibly in the future instead of constructing the datastream direct, call on the resource manager to create it(future plans)
    Resource::FileStream SaveStream(ProfileName+".xml",ProfilesDir+"/",(Resource::DataStream::SF_Truncate | Resource::DataStream::SF_Write));
    ProfileDoc.Save(SaveStream,"\t",XML::FormatIndent);
}

ConstString& CatchProfile::GetName() const
{
    return this->ProfileName;
}

Whole CatchProfile::GetHighestScore(const String& LevelName) const
{
    ConstLevelScoreMapIterator it = this->LevelScores.find(LevelName);
    if( it != this->LevelScores.end() ) return (*it).second;
    else return 0;
}

void CatchProfile::SetNewHighScore(const String& LevelName, const Whole& NewHigh)
{
    this->LevelScores[LevelName] = NewHigh;
}

///////////////////////////////////////////////////////////////////////////////
// ProfileManager Methods

ProfileManager::ProfileManager(const String& ProfilesDir) :
    ProfilesDirectory(ProfilesDir)
{
    /*ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    XML::Document ProfileDoc;
    std::set<String> ProfileSet = ResourceMan->GetDirContents("./Profiles");
    for( std::set<String>::iterator it = ProfileSet.begin() ; it != ProfileSet.end() ; it++ )
    {
        const String& FileName = (*it);
        if(String::npos == FileName.find(".xml"))
            continue;

        ProfileDoc.Reset();

        Resource::FileStream LoadStream(FileName,ProfilesDir,Resource::DataStream::SF_Read);
        ProfileDoc.Load(LoadStream);
        LoadProfile(ProfileDoc);
    }//*/
}

ProfileManager::~ProfileManager()
{
    SaveAllProfiles();
    for( ProfilesIterator it = LoadedProfiles.begin() ; it != LoadedProfiles.end() ; ++it )
    {
        delete (*it);
    }
    LoadedProfiles.clear();
}

void ProfileManager::PopulateLevelList(GameProfile* Profile)
{
    /// @todo UI Update
    /*std::set<String> Previews(ResourceMan->GetDirContents("./Previews"));
    for( std::set<String>::iterator it = Files.begin() ; it != Files.end() ; it++ )
    {
        const String& FileName = (*it);
        if(String::npos == FileName.find(".lvl"))
            continue;

        String LevelName = FileName.substr(0,(*it).size() - 4);
        LevelSelectCell* CurrCell = new LevelSelectCell(LevelName,CellRect,ParentScreen);
        CurrCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBack");
        CurrCell->GetPreviewBorder()->SetBackgroundSprite("MMLevelPreviewBox");
        CurrCell->GetLevelTitle()->SetText(LevelName);
        CurrCell->GetLevelTitle()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));

        /// @todo This need to be changed to a sprite with stars or something to show the progression instead of the number.
        CurrCell->GetEarnedScore()->SetText( StringTools::ConvertToString(Profile->GetHighestScore(LevelName)) );

        CurrCell->GetEarnedScore()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
        if(Previews.count(LevelName+".mta"))
        {
            UIMan->LoadMTA(LevelName);
            CurrCell->GetPreviewImage()->SetBackgroundSprite("LevelPreview",LevelName);
        }
        CurrCell->SetCellCallback(new LevelSelectCB());
        Grid->AddCell(CurrCell);
    }
    Grid->GenerateGrid();//*/
}

GameProfile* ProfileManager::CreateNewProfile(const String& Name)
{
    GameProfile* Profile = new GameProfile(Name);
    LoadedProfiles.push_back(Profile);
    return Profile;
}

GameProfile* ProfileManager::LoadProfile(const String& FileName)
{
    XML::Document ProfileDoc;

    return LoadProfile(ProfileDoc);
}

GameProfile* ProfileManager::LoadProfile(XML::Document& ProfileDoc)
{
    GameProfile* Profile = NULL;
    try{
        Profile = new GameProfile(ProfileDoc);
    }catch(IOException Ex){
        //Handle somehow?
    }
    LoadedProfiles.push_back(Profile);
    return Profile;
}

GameProfile* ProfileManager::GetProfile(const String& Name) const
{
    for( ConstProfilesIterator it = LoadedProfiles.begin() ; it != LoadedProfiles.end() ; ++it )
    {
        if( Name == (*it)->GetName() )
        {
            return (*it);
        }
    }
    return NULL;
}

GameProfile* ProfileManager::GetProfile(const Whole& Index) const
{
    return LoadedProfiles.at(Index);
}

Whole ProfileManager::GetNumLoadedProfiles() const
{
    return LoadedProfiles.size();
}

void ProfileManager::SaveProfile(const String& Name)
{
    for( ProfilesIterator it = LoadedProfiles.begin() ; it != LoadedProfiles.end() ; ++it )
    {
        if( Name == (*it)->GetName() )
        {
            (*it)->Save(ProfilesDirectory);
            break;
        }
    }
}

void ProfileManager::SaveAllProfiles()
{
    for( ProfilesIterator it = LoadedProfiles.begin() ; it != LoadedProfiles.end() ; ++it )
    {
        (*it)->Save(ProfilesDirectory);
    }
}

void ProfileManager::SetActiveProfile(const String& Name)
{
    for( ProfilesIterator it = LoadedProfiles.begin() ; it != LoadedProfiles.end() ; ++it )
    {
        if( Name == (*it)->GetName() )
        {
            SetActiveProfile(*it);
            break;
        }
    }
}

void ProfileManager::SetActiveProfile(GameProfile* Profile)
{
    ActiveProfile = Profile;
    PopulateLevelList(ActiveProfile);
}

GameProfile* ProfileManager::GetActiveProfile() const
{
    return ActiveProfile;
}

/// @todo UI Update
/*UI::PagedCellGrid* ProfileManager::GetLevelGrid() const
{
    UI::Menu* MainMenu = static_cast<UI::Menu*>( UI::UIManager::GetSingletonPtr()->GetScreen("MainMenuScreen")->GetWidget("MS_Menu") );
    UI::PagedCellGrid* Grid = static_cast<UI::PagedCellGrid*>( MainMenu->GetRootWindow()->GetChildMenuWindow("MS_LevelSelectWin")->GetWidget("MS_LevelGrid") );
    return Grid;
}//*/

#endif
