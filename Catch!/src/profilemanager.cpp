#ifndef _profilemanager_cpp
#define _profilemanager_cpp

#include "profilemanager.h"
#include "levelselectcell.h"

///////////////////////////////////////////////////////////////////////////////
// CatchProfile Methods

CatchProfile::CatchProfile(const String& Name)
    : ProfileName(Name)
{
}

CatchProfile::CatchProfile(xml::Document& ProfileDoc)
{
    DeSerializeProfileName(ProfileDoc);
    DeSerializeLevelScores(ProfileDoc);
}

CatchProfile::~CatchProfile()
{
}

void CatchProfile::SerializeProfileName(xml::Document& ProfileDoc)
{
    xml::Node NameNode = ProfileDoc.AppendChild("ProfileName");
    xml::Attribute NameAttrib = NameNode.AppendAttribute("Name");
    NameAttrib.SetValue(ProfileName);
}

void CatchProfile::SerializeLevelScores(xml::Document& ProfileDoc)
{
    xml::Node LevelsNode = ProfileDoc.AppendChild("LevelScores");
    for( LevelScoreMapIterator SecIt = LevelScores.begin() ; SecIt != LevelScores.end() ; ++SecIt )
    {
        xml::Node LevelElement = LevelsNode.AppendChild( (*SecIt).first );
        xml::Attribute LevelHighScore = LevelElement.AppendAttribute("HighestScore");
        LevelHighScore.SetValue( (*SecIt).second );
    }
}

void CatchProfile::DeSerializeProfileName(xml::Document& ProfileDoc)
{
    xml::Node NameNode = ProfileDoc.GetChild("ProfileName");
    xml::Attribute NameAttrib = NameNode.GetAttribute("Name");
    ProfileName = NameAttrib.AsString();
}

void CatchProfile::DeSerializeLevelScores(xml::Document& ProfileDoc)
{
    xml::Node LevelsNode = ProfileDoc.GetChild("LevelScores");
    for( xml::NodeIterator NodeIt = LevelsNode.begin() ; NodeIt != LevelsNode.end() ; ++NodeIt )
    {
        std::pair<String,Whole> LevelPair;
        LevelPair.first = (*NodeIt).Name();
        LevelPair.second = (*NodeIt).GetAttribute("HighestScore").AsWhole();
        LevelScores.insert(LevelPair);
    }
}

void CatchProfile::Save(const String& ProfilesDir)
{
    xml::Document ProfileDoc;
    xml::Node DocDecl = ProfileDoc.AppendChild(xml::NodeDeclaration);
    DocDecl.SetName("xml");
    xml::Attribute Version = DocDecl.AppendAttribute("version");
    Version.SetValue("1.0");

    SerializeProfileName(ProfileDoc);
    SerializeLevelScores(ProfileDoc);

    /// @todo Possibly in the future instead of constructing the datastream direct, call on the resource manager to create it(future plans)
    Resource::FileStreamDataStream SaveStream(ProfileName+".xml",ProfilesDir+"/",Resource::DataStream::SF_Truncate,Resource::DataStream::DS_Write);
    ProfileDoc.Save(SaveStream,"\t",xml::FormatIndent);
}

ConstString& CatchProfile::GetName() const
{
    return ProfileName;
}

Whole CatchProfile::GetHighestScore(const String& LevelName) const
{
    ConstLevelScoreMapIterator it = LevelScores.find(LevelName);
    if(it != LevelScores.end()) return (*it).second;
    else return 0;
}

void CatchProfile::SetNewHighScore(const String& LevelName, const Whole& NewHigh)
{
    LevelScores[LevelName] = NewHigh;
}

///////////////////////////////////////////////////////////////////////////////
// ProfileManager Methods

ProfileManager::ProfileManager(const String& ProfilesDir)
    : ProfilesDirectory(ProfilesDir)
{
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    xml::Document ProfileDoc;
    std::set<String>* ProfileSet = ResourceMan->GetDirContents("./Profiles");
    for( std::set<String>::iterator it = ProfileSet->begin() ; it != ProfileSet->end() ; it++ )
    {
        const String& FileName = (*it);
        if(String::npos == FileName.find(".xml"))
            continue;

        ProfileDoc.Reset();

        Resource::FileStreamDataStream LoadStream(FileName,ProfilesDir,Resource::DataStream::SF_None);
        ProfileDoc.Load(LoadStream);
        LoadProfile(ProfileDoc);
    }
    delete ProfileSet;
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
    ResourceManager* ResourceMan = ResourceManager::GetSingletonPtr();
    UIManager* UIMan = UIManager::GetSingletonPtr();
    UI::PagedCellGrid* Grid = GetLevelGrid();

    Grid->DestroyAllCells();
    UI::RenderableRect CellRect(Vector2(0.1,0.1),Vector2(0.33,0.11),true);
    Vector2 CellSpacing(0.06,0.04);
    Grid->SetFixedCellSize(CellRect.Size);
    Grid->SetCellSpacing(CellSpacing);
    UI::Layer* ParentLayer = Grid->GetLayer();

    std::set<String>* Files = ResourceMan->GetDirContents("./Levels");
    if(Files->empty())
        return;
    std::set<String>* Previews = ResourceMan->GetDirContents("./Previews");
    for( std::set<String>::iterator it = Files->begin() ; it != Files->end() ; it++ )
    {
        const String& FileName = (*it);
        if(String::npos == FileName.find(".lvl"))
            continue;

        String LevelName = FileName.substr(0,(*it).size() - 4);
        LevelSelectCell* CurrCell = new LevelSelectCell(LevelName,CellRect,ParentLayer);
        CurrCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBack");
        CurrCell->GetPreviewBorder()->SetBackgroundSprite("MMLevelPreviewBox");
        CurrCell->GetLevelTitle()->SetText(LevelName);
        CurrCell->GetLevelTitle()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));

        /// @todo This need to be changed to a sprite with stars or something to show the progression instead of the number.
        CurrCell->GetEarnedScore()->SetText( StringTool::ConvertToString(Profile->GetHighestScore(LevelName)) );// */

        CurrCell->GetEarnedScore()->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.0));
        if(Previews->count(LevelName+".mta"))
        {
            UIMan->LoadMTA(LevelName);
            CurrCell->GetPreviewImage()->SetBackgroundSprite("LevelPreview",LevelName);
        }
        CurrCell->SetCellCallback(new LevelSelectCB());
        Grid->AddCell(CurrCell);
    }
    Grid->GenerateGrid();
    delete Files;
    delete Previews;
}

GameProfile* ProfileManager::CreateNewProfile(const String& Name)
{
    GameProfile* Profile = new GameProfile(Name);
    LoadedProfiles.push_back(Profile);
    return Profile;
}

GameProfile* ProfileManager::LoadProfile(const String& FileName)
{
    xml::Document ProfileDoc;

    return LoadProfile(ProfileDoc);
}

GameProfile* ProfileManager::LoadProfile(xml::Document& ProfileDoc)
{
    GameProfile* Profile = NULL;
    try{
        Profile = new GameProfile(ProfileDoc);
    }catch(std::exception x){
        World::GetWorldPointer()->LogAndThrow(Exception("Attempting to Parse improperly formatted XML file."));
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

UI::PagedCellGrid* ProfileManager::GetLevelGrid() const
{
    UI::Menu* MainMenu = static_cast<UI::Menu*>( UIManager::GetSingletonPtr()->GetLayer("MainMenuLayer")->GetWidget("MS_Menu") );
    UI::PagedCellGrid* Grid = static_cast<UI::PagedCellGrid*>( MainMenu->GetRootWindow()->GetChildMenuWindow("MS_LevelSelect")->GetWidget("MS_LevelGrid") );
    return Grid;
}

#endif
