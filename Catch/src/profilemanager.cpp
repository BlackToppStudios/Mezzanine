#ifndef _profilemanager_cpp
#define _profilemanager_cpp

#include "profilemanager.h"
#include "catchapp.h"

///////////////////////////////////////////////////////////////////////////////
// CatchProfile Methods

CatchProfile::CatchProfile(const String& Name, const Boole NeedsLoading) :
    ProfileName(Name),
    Loaded(!NeedsLoading)
    {  }

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

const String& CatchProfile::GetName() const
    { return this->ProfileName; }

Boole CatchProfile::IsLoaded() const
    { return this->Loaded; }

void CatchProfile::Load(const String& ProfilesDir)
{
    if( !this->Loaded ) {
        XML::Document ProfileDoc;
        /// @todo Possibly in the future instead of constructing the datastream direct, call on the resource manager to create it(future plans)
        Resource::FileStream LoadStream(this->ProfileName+".xml",ProfilesDir+"/",Resource::SF_Read);

        ProfileDoc.Load( LoadStream );
        XML::Node RootNode = ProfileDoc.GetChild("ProfileRoot");
        if( !RootNode.Empty() ) {
            this->DeSerializeProfileName(RootNode);
            this->DeSerializeLevelScores(RootNode);
        }
        this->Loaded = true;
    }
}

void CatchProfile::Unload()
{
    if( this->Loaded ) {
        this->LevelScores.clear();
        this->Loaded = false;
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
    Resource::FileStream SaveStream(ProfileName+".xml",ProfilesDir+"/",(Resource::SF_Truncate | Resource::SF_Write));
    ProfileDoc.Save(SaveStream,"\t",XML::FormatIndent);
}

void CatchProfile::SetNewHighScore(const String& LevelName, const Whole& NewHigh)
{
    this->LevelScores[LevelName] = NewHigh;
}

Whole CatchProfile::GetHighestScore(const String& LevelName) const
{
    ConstLevelScoreMapIterator it = this->LevelScores.find(LevelName);
    if( it != this->LevelScores.end() ) return (*it).second;
    else return 0;
}

///////////////////////////////////////////////////////////////////////////////
// ProfileManager Methods

ProfileManager::ProfileManager(Entresol* Ent, const String& ProfilesDir) :
    TheEntresol(Ent),
    LastLoadedProfile(NULL)
    { this->SetProfilesDirectory(ProfilesDir); }

ProfileManager::ProfileManager(Entresol* Ent, const XML::Node& XMLNode) :
    TheEntresol(Ent),
    LastLoadedProfile(NULL)
    { this->ObjectSettingsHandler::ProtoDeSerialize(XMLNode); }

ProfileManager::~ProfileManager()
{
    //this->SaveAllProfiles();
    for( ProfilesIterator it = this->Profiles.begin() ; it != this->Profiles.end() ; ++it )
        { delete (*it); }
    this->Profiles.clear();
}

String ProfileManager::GetObjectRootNodeName() const
{
    return "ProfileManagerSettings";
}

void ProfileManager::AppendCurrentSettings(XML::Node& SettingsRootNode)
{
    // Create the Group node to be returned
    XML::Node CurrentSettings = SettingsRootNode.AppendChild("Current");
    // Create and initialize the device settings
    XML::Node ProfilesDirectoryNode = CurrentSettings.AppendChild("ProfilesDirectory");
    ProfilesDirectoryNode.AppendAttribute("Path").SetValue( this->ProfilesDirectory );
    // Create and initialize the master settings
    XML::Node ActiveProfileNode = CurrentSettings.AppendChild("LastLoadedProfile");
    ActiveProfileNode.AppendAttribute("ProfileName").SetValue( this->LastLoadedProfile->GetName() );
}

void ProfileManager::ApplySettingGroupImpl(ObjectSettingGroup* Group)
{
    String CurrSettingValue;
    ObjectSettingSet* DirectorySet = Group->GetChildObjectSettingSet("ProfilesDirectory");
    if( DirectorySet != NULL ) {
        // Get the values
        CurrSettingValue = DirectorySet->GetSettingValue("Path");
        if(!CurrSettingValue.empty())
            this->SetProfilesDirectory( CurrSettingValue );
    }

    this->DetectProfiles();

    ObjectSettingSet* ActiveSet = Group->GetChildObjectSettingSet("LastLoadedProfile");
    if( ActiveSet != NULL ) {
        // Get the values
        CurrSettingValue = ActiveSet->GetSettingValue("ProfileName");
        if(!CurrSettingValue.empty())
            this->LastLoadedProfile = this->GetProfile( CurrSettingValue );
    }

    if( this->LastLoadedProfile != NULL )
        this->LastLoadedProfile->Load(this->ProfilesDirectory);
}

///////////////////////////////////////////////////////////////////////////////
// Utility

Whole ProfileManager::DetectProfiles()
{
    // Save and then clear before attempting to detect.
    this->SaveAllProfiles();
    for( ProfilesIterator ProIt = this->Profiles.begin() ; ProIt != this->Profiles.end() ; ++ProIt )
        { delete (*ProIt); }

    // Now detect and load.
    Whole Detected = 0;
    XML::Document ProfileDoc;
    StringVector ProfileVec = Resource::GetDirContents(this->ProfilesDirectory);
    for( StringVector::iterator it = ProfileVec.begin() ; it != ProfileVec.end() ; it++ )
    {
        const String& FileName = (*it);
        size_t PeriodPos = FileName.find(".xml");
        if( PeriodPos != String::npos ) {
            ++Detected;

            GameProfile* NewProfile = new GameProfile(FileName.substr(0,PeriodPos),true);
            this->Profiles.push_back(NewProfile);
        }
    }
    return Detected;
}

void ProfileManager::ApplyProfileDataToProfileList(GameProfile* PlayerProfile)
{
    UI::Screen* MainMenuScreen = this->GetMainMenuScreen();
    UI::DropDownList* ProfilesDropList = static_cast<UI::DropDownList*>( MainMenuScreen->GetWidget("MS_ProfilesList") );
    UI::ListBox* ProfilesList = ProfilesDropList->GetSelectionList();

    ProfilesList->GetListContainer()->DestroyAllChildren();
    for( ConstProfilesIterator ProIt = this->Profiles.begin() ; ProIt != this->Profiles.end() ; ++ProIt )
    {
        const String& ProfileName = (*ProIt)->GetName();
        ProfilesList->CreateSingleLineListItem(ProfileName,ProfileName);
    }
    ProfilesList->UpdateChildDimensions();
    ProfilesDropList->UpdateCurrentSelection( ProfilesList->GetListItem( PlayerProfile->GetName() ) );// */
}

void ProfileManager::ApplyProfileDataToLevelSelect(GameProfile* Profile)
{
    // Get our pointers
    LevelManager* LevelMan = CatchApp::GetCatchAppPointer()->GetLevelManager();
    UI::Screen* MainMenuScreen = this->GetMainMenuScreen();

    for( LevelManager::ConstGameLevelIterator LevelIt = LevelMan->BeginGameLevel() ; LevelIt != LevelMan->EndGameLevel() ; ++LevelIt )
    {
        const String& LevelName = (*LevelIt)->GetName();
        Whole HighestScore = Profile->GetHighestScore( LevelName );
        UI::Widget* LevelScoreWid = MainMenuScreen->GetWidget( LevelName + ".Score" );
        if( LevelScoreWid != NULL ) {
            UI::MultiImageLayer* LevelScoreStars = static_cast<UI::MultiImageLayer*>( LevelScoreWid->GetRenderLayer(0,UI::RLT_MultiImage) );

            for( Whole ScoreTier = 0 ; ScoreTier < (*LevelIt)->GetNumScoreTiers() && ScoreTier < 5 ; ++ScoreTier )
            {
                Whole Threshold = (*LevelIt)->GetScoreThreshold(ScoreTier);
                if( HighestScore > Threshold ) {
                    LevelScoreStars->SetSprite(ScoreTier,"MMLevelScoreStarFilled");
                }else{
                    LevelScoreStars->SetSprite(ScoreTier,"MMLevelScoreStar");
                }
            }
        }
    }
}

UI::Screen* ProfileManager::GetMainMenuScreen() const
{
    UI::UIManager* UIMan = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) );
    return UIMan->GetScreen("MainMenuScreen");
}

UI::Widget* ProfileManager::GetProfileAccessButton() const
{
    UI::Screen* MainMenuScreen = this->GetMainMenuScreen();
    return MainMenuScreen->GetWidget("MS_ProfilesAccess");
}

void ProfileManager::SetNewHighScoreInUI(const String& LevelName, const Whole LevelScore)
{
    // Get our pointers
    LevelManager* LevelMan = CatchApp::GetCatchAppPointer()->GetLevelManager();
    UI::Screen* MainMenuScreen = this->GetMainMenuScreen();

    GameLevel* HighScoreLevel = LevelMan->GetLevel(LevelName);
    UI::Widget* LevelScoreWid = MainMenuScreen->GetWidget( LevelName + ".Score" );
    UI::MultiImageLayer* LevelScoreStars = static_cast<UI::MultiImageLayer*>( LevelScoreWid->GetRenderLayer(0,UI::RLT_MultiImage) );

    for( Whole ScoreTier = 0 ; ScoreTier < HighScoreLevel->GetNumScoreTiers() && ScoreTier < 5 ; ++ScoreTier )
    {
        Whole Threshold = HighScoreLevel->GetScoreThreshold(ScoreTier);
        if( LevelScore > Threshold ) {
            LevelScoreStars->SetSprite(ScoreTier,"MMLevelScoreStarFilled");
        }else{
            LevelScoreStars->SetSprite(ScoreTier,"MMLevelScoreStar");
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Profile Path Management

void ProfileManager::SetProfilesDirectory(const String& Path)
{
    size_t FirstDol = Path.find_first_of('$');
    size_t SecondDol = Path.find_first_of('$',FirstDol+1);
    if(FirstDol != String::npos && SecondDol != String::npos) {
        String PathKeyWord = Path.substr( FirstDol + 1, ( SecondDol - FirstDol ) - 1 );
        String RestOfPath = Path.substr( SecondDol + 1 );
        String ActualPath = Resource::ResolveDataPathFromString(PathKeyWord);
        this->ProfilesDirectory = ActualPath + RestOfPath;
    }else{
        this->ProfilesDirectory = Path;
    }
    Resource::CreateDirectoryPath(this->ProfilesDirectory);

    /*
    Resource::ResourceManager* ResourceMan = static_cast<Resource::ResourceManager*>( this->TheEntresol->GetManager(ManagerBase::MT_ResourceManager) );
    Resource::AssetGroup* ProfilesGroup = ResourceMan->GetAssetGroup("Profiles");
    if( ProfilesGroup != NULL ) {
        ResourceMan->DestroyAssetGroup("Profiles");
    }
    ResourceMan->AddAssetLocation(this->ProfilesDirectory,Resource::AT_FileSystem,"Profiles");//*/
}

const String& ProfileManager::GetProfilesDirectory() const
{
    return this->ProfilesDirectory;
}

///////////////////////////////////////////////////////////////////////////////
// Profile Management

GameProfile* ProfileManager::CreateNewProfile(const String& Name)
{
    GameProfile* Profile = this->GetProfile(Name);
    if( Profile != NULL ) {
        return Profile;
    }
    Profile = new GameProfile(Name,false);
    this->Profiles.push_back(Profile);
    this->LastLoadedProfile = Profile;
    return Profile;
}

GameProfile* ProfileManager::LoadProfile(const String& Name)
{
    GameProfile* Profile = this->GetProfile(Name);
    if( Profile == NULL ) return NULL;
    else return this->LoadProfile(Profile);
}

GameProfile* ProfileManager::LoadProfile(GameProfile* Profile)
{
    Profile->Load(this->ProfilesDirectory);
    this->LastLoadedProfile = Profile;
    return Profile;
}

GameProfile* ProfileManager::GetProfile(const String& Name) const
{
    for( ConstProfilesIterator it = this->Profiles.begin() ; it != this->Profiles.end() ; ++it )
    {
        if( Name == (*it)->GetName() ) {
            return (*it);
        }
    }
    return NULL;
}

GameProfile* ProfileManager::GetProfile(const Whole Index) const
    { return this->Profiles.at(Index); }

Whole ProfileManager::GetNumProfiles() const
    { return this->Profiles.size(); }

GameProfile* ProfileManager::GetLastLoadedProfile() const
    { return this->LastLoadedProfile; }

void ProfileManager::SaveProfile(const String& Name)
    { this->SaveProfile( this->GetProfile(Name) ); }

void ProfileManager::SaveProfile(GameProfile* Profile)
    { Profile->Save(this->ProfilesDirectory); }

void ProfileManager::SaveAllProfiles()
{
    for( ProfilesIterator it = this->Profiles.begin() ; it != this->Profiles.end() ; ++it )
        { (*it)->Save(this->ProfilesDirectory); }
}

///////////////////////////////////////////////////////////////////////////////
// Initialization Methods

void ProfileManager::Initialize()
{
    if( this->LastLoadedProfile == NULL ) {
        String Default("Default");
        GameProfile* DefaultProfile = this->GetProfile(Default);
        if( DefaultProfile == NULL ) {
            DefaultProfile = this->CreateNewProfile(Default);
        }
        this->LastLoadedProfile = DefaultProfile;
    }

    this->SaveAllProfiles();

    if( this->AutoGenFiles )
        this->SaveAllSettings();

    //this->Initialized = true;
}

void ProfileManager::Deinitialize()
{
    this->SaveAllProfiles();

    if( this->AutoGenFiles )
        this->SaveAllSettings();

    //this->Initialized = false;
}

/// @todo UI Update
/*UI::PagedCellGrid* ProfileManager::GetLevelGrid() const
{
    UI::Menu* MainMenu = static_cast<UI::Menu*>( UI::UIManager::GetSingletonPtr()->GetScreen("MainMenuScreen")->GetWidget("MS_Menu") );
    UI::PagedCellGrid* Grid = static_cast<UI::PagedCellGrid*>( MainMenu->GetRootWindow()->GetChildMenuWindow("MS_LevelSelectWin")->GetWidget("MS_LevelGrid") );
    return Grid;
}// */

#endif
