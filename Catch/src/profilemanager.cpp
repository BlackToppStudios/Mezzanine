#ifndef _profilemanager_cpp
#define _profilemanager_cpp

#include "profilemanager.h"
#include "catchapp.h"

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
    Resource::FileStream SaveStream(ProfileName+".xml",ProfilesDir+"/",(Resource::SF_Truncate | Resource::SF_Write));
    ProfileDoc.Save(SaveStream,"\t",XML::FormatIndent);
}

ConstString& CatchProfile::GetName() const
{
    return this->ProfileName;
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
    ActiveProfile(NULL)
    { this->SetProfilesDirectory(ProfilesDir); }

ProfileManager::ProfileManager(Entresol* Ent, const XML::Node& XMLNode) :
    TheEntresol(Ent),
    ActiveProfile(NULL)
    { this->ObjectSettingsHandler::ProtoDeSerialize(XMLNode); }

ProfileManager::~ProfileManager()
{
    this->SaveAllProfiles();
    for( ProfilesIterator it = this->LoadedProfiles.begin() ; it != this->LoadedProfiles.end() ; ++it )
        { delete (*it); }
    this->LoadedProfiles.clear();
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
    XML::Node ActiveProfileNode = CurrentSettings.AppendChild("ActiveProfile");
    ActiveProfileNode.AppendAttribute("ProfileName").SetValue( this->ActiveProfile->GetName() );
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

    ObjectSettingSet* ActiveSet = Group->GetChildObjectSettingSet("ActiveProfile");
    if( ActiveSet != NULL ) {
        // Get the values
        CurrSettingValue = ActiveSet->GetSettingValue("ProfileName");
        if(!CurrSettingValue.empty())
            this->SetActiveProfile( CurrSettingValue );
    }
    /*for( ObjectSettingSetContainer::SubSetIterator SubSetIt = Group->SubSetBegin() ; SubSetIt != Group->SubSetEnd() ; ++SubSetIt )
    {
        String CurrSettingValue;
        if( "ProfilesDirectory" == (*SubSetIt)->GetName() ) {
            // Get the values
            CurrSettingValue = (*SubSetIt)->GetSettingValue("Path");
            if(!CurrSettingValue.empty())
                this->SetProfilesDirectory( CurrSettingValue );
        }else if( "ActiveProfile" == (*SubSetIt)->GetName() ) {
            // Get the values
            CurrSettingValue = (*SubSetIt)->GetSettingValue("ProfileName");
            if(!CurrSettingValue.empty())
                this->SetActiveProfile( CurrSettingValue );
        }
    }// */
}

///////////////////////////////////////////////////////////////////////////////
// Utility

Whole ProfileManager::DetectProfiles()
{
    // Save and then clear before attempting to detect.
    this->SaveAllProfiles();
    for( ProfilesIterator ProIt = this->LoadedProfiles.begin() ; ProIt != this->LoadedProfiles.end() ; ++ProIt )
        { delete (*ProIt); }

    // Now detect and load.
    Whole Detected = 0;
    XML::Document ProfileDoc;
    Resource::ResourceManager* ResourceMan = static_cast<Resource::ResourceManager*>( this->TheEntresol->GetManager(ManagerBase::MT_ResourceManager) );
    StringVector ProfileVec = Resource::GetDirContents(this->ProfilesDirectory);
    for( StringVector::iterator it = ProfileVec.begin() ; it != ProfileVec.end() ; it++ )
    {
        const String& FileName = (*it);
        if( String::npos != FileName.find(".xml") ) {
            ++Detected;

            ProfileDoc.Reset();
            Resource::DataStreamPtr ProfileStream = ResourceMan->OpenAssetStream(FileName,"Profiles");
            ProfileDoc.Load( *ProfileStream.Get() );

            GameProfile* NewProfile = new GameProfile(ProfileDoc);
            this->LoadedProfiles.push_back(NewProfile);
        }
    }
    return Detected;
}

void ProfileManager::ApplyProfileDataToProfileList()
{
    UI::Screen* MainMenuScreen = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) )->GetScreen("MainMenuScreen");
    UI::DropDownList* ProfilesDropList = static_cast<UI::DropDownList*>( MainMenuScreen->GetWidget("MS_ProfilesList") );
    UI::ListBox* ProfilesList = ProfilesDropList->GetSelectionList();

    ProfilesList->GetListContainer()->DestroyAllChildren();
    for( ConstProfilesIterator ProIt = this->LoadedProfiles.begin() ; ProIt != this->LoadedProfiles.end() ; ++ProIt )
    {
        const String& ProfileName = (*ProIt)->GetName();
        ProfilesList->CreateSingleLineListItem(ProfileName,ProfileName);
    }
    ProfilesList->UpdateChildDimensions();
    ProfilesDropList->UpdateCurrentSelection( ProfilesList->GetListItem( this->ActiveProfile->GetName() ) );// */
}

void ProfileManager::ApplyProfileDataToLevelSelect()
{
    if( this->ActiveProfile != NULL ) {
        this->ApplyProfileDataToLevelSelect( this->ActiveProfile );
    }
}

void ProfileManager::ApplyProfileDataToLevelSelect(GameProfile* Profile)
{
    // Get our pointers
    LevelManager* LevelMan = CatchApp::GetCatchAppPointer()->GetLevelManager();
    UI::Screen* MainMenuScreen = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) )->GetScreen("MainMenuScreen");

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

void ProfileManager::SetNewHighScore(const String& LevelName, const Whole LevelScore)
{
    // Get our pointers
    LevelManager* LevelMan = CatchApp::GetCatchAppPointer()->GetLevelManager();
    UI::Screen* MainMenuScreen = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) )->GetScreen("MainMenuScreen");

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

    this->ActiveProfile->SetNewHighScore(LevelName,LevelScore);
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

    Resource::ResourceManager* ResourceMan = static_cast<Resource::ResourceManager*>( this->TheEntresol->GetManager(ManagerBase::MT_ResourceManager) );
    Resource::AssetGroup* ProfilesGroup = ResourceMan->GetAssetGroup("Profiles");
    if( ProfilesGroup != NULL ) {
        ResourceMan->DestroyAssetGroup("Profiles");
    }
    ResourceMan->AddAssetLocation(this->ProfilesDirectory,Resource::AT_FileSystem,"Profiles");
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
    Profile = new GameProfile(Name);
    this->LoadedProfiles.push_back(Profile);
    return Profile;
}

GameProfile* ProfileManager::LoadProfile(const String& FileName)
{
    XML::Document ProfileDoc;

    return this->LoadProfile(ProfileDoc);
}

GameProfile* ProfileManager::LoadProfile(XML::Document& ProfileDoc)
{
    GameProfile* Profile = NULL;
    try{
        Profile = new GameProfile(ProfileDoc);
    }catch(IOException Ex){
        //Handle somehow?
    }
    this->LoadedProfiles.push_back(Profile);
    return Profile;
}

GameProfile* ProfileManager::GetProfile(const String& Name) const
{
    for( ConstProfilesIterator it = this->LoadedProfiles.begin() ; it != this->LoadedProfiles.end() ; ++it )
    {
        if( Name == (*it)->GetName() ) {
            return (*it);
        }
    }
    return NULL;
}

GameProfile* ProfileManager::GetProfile(const Whole& Index) const
{
    return this->LoadedProfiles.at(Index);
}

Whole ProfileManager::GetNumLoadedProfiles() const
{
    return this->LoadedProfiles.size();
}

void ProfileManager::SaveProfile(const String& Name)
{
    GameProfile* Profile = this->GetProfile(Name);
    this->SaveProfile(Profile);
}

void ProfileManager::SaveProfile(GameProfile* Profile)
{
    Profile->Save(this->ProfilesDirectory);
}

void ProfileManager::SaveAllProfiles()
{
    for( ProfilesIterator it = this->LoadedProfiles.begin() ; it != this->LoadedProfiles.end() ; ++it )
    {
        (*it)->Save(this->ProfilesDirectory);
    }
}

/*void ProfileManager::DestroyProfile(const String& Name)
{
    GameProfile* Profile = this->GetProfile(Name);
    this->DestroyProfile(Profile);
}

void ProfileManager::DestroyProfile(GameProfile* Profile)
{
    // Clear it from our container
    ProfilesIterator ProIt = std::find(this->LoadedProfiles.begin(),this->LoadedProfiles.end(),Profile);
    if( ProIt != this->LoadedProfiles.end() ) {
        this->LoadedProfiles.erase(ProIt);
    }
    // Delete it from the disk
    Resource::RemoveFile( this->ProfilesDirectory + Profile->GetName() );
    // Clean up the profile itself
    delete Profile;
}// */

///////////////////////////////////////////////////////////////////////////////
// ActiveProfile Management

void ProfileManager::SetActiveProfile(const String& Name)
{
    for( ProfilesIterator it = this->LoadedProfiles.begin() ; it != this->LoadedProfiles.end() ; ++it )
    {
        if( Name == (*it)->GetName() ) {
            this->SetActiveProfile(*it);
            break;
        }
    }
}

void ProfileManager::SetActiveProfile(GameProfile* Profile)
{
    this->ActiveProfile = Profile;
    //this->ApplyProfileDataToUI( this->ActiveProfile );

    if( this->ActiveProfile != NULL ) {
        UI::Screen* MainMenuScreen = static_cast<UI::UIManager*>( this->TheEntresol->GetManager(ManagerBase::MT_UIManager) )->GetScreen("MainMenuScreen");
        if( MainMenuScreen != NULL ) {
            UI::Widget* ProfileDisplay = MainMenuScreen->GetWidget("MS_ProfilesAccess");
            UI::SingleLineTextLayer* ProfileDisplayText = static_cast<UI::SingleLineTextLayer*>( ProfileDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) );
            ProfileDisplayText->SetText( this->ActiveProfile->GetName() );
        }
    }
}

String ProfileManager::GetActiveProfileName() const
{
    if( this->ActiveProfile != NULL ) {
        return this->ActiveProfile->GetName();
    }
    return "";
}

GameProfile* ProfileManager::GetActiveProfile() const
{
    return this->ActiveProfile;
}

///////////////////////////////////////////////////////////////////////////////
// Initialization Methods

void ProfileManager::Initialize()
{
    if( this->ActiveProfile == NULL ) {
        String Default("Default");
        GameProfile* DefaultProfile = this->GetProfile(Default);
        if( DefaultProfile == NULL ) {
            DefaultProfile = this->CreateNewProfile(Default);
        }
        this->SetActiveProfile(DefaultProfile);
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
