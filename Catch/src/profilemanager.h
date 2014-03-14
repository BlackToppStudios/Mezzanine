#ifndef _profilemanager_h
#define _profilemanager_h

#include <mezzanine.h>

using namespace Mezzanine;

///////////////////////////////////////////////////////////////////////////////
/// @brief This is used to store simple data related to a profile that can be loaded.
/// @details
///////////////////////////////////////
class CatchProfile
{
public:
    /// @brief Basic container type for level high scores stored in this class.
    typedef std::map<String,Whole> LevelScoreMap;
    /// @brief Iterator type for level high scores being stored in this class.
    typedef LevelScoreMap::iterator LevelScoreMapIterator;
    /// @brief Const Iterator type for level high scores being stored in this class.
    typedef LevelScoreMap::const_iterator ConstLevelScoreMapIterator;
protected:
    /// @internal
    /// @brief Container storing the saved high scores for this profile.
    LevelScoreMap LevelScores;
    /// @internal
    /// @brief A string containing the name of this profile.
    String ProfileName;

    /// @internal
    /// @brief Serializes the name of this profile.
    /// @param ProfileRoot The root XML node for this profile.
    void SerializeProfileName(XML::Node& ProfileRoot) const;
    /// @internal
    /// @brief Deserializes the name of this profile.
    /// @param ProfileRoot The root XML node for this profile.
    void SerializeLevelScores(XML::Node& ProfileRoot) const;
    /// @internal
    /// @brief Serializes the saved high scores of this profile.
    /// @param ProfileRoot The root XML node for this profile.
    void DeSerializeProfileName(const XML::Node& ProfileRoot);
    /// @internal
    /// @brief Deserializes the saved high scores of this profile.
    /// @param ProfileRoot The root XML node for this profile.
    void DeSerializeLevelScores(const XML::Node& ProfileRoot);
public:
    /// @brief Class constructor.
    CatchProfile(const String& Name);
    /// @brief XML constructor.
    /// @param LevelDoc The XML document containing profile data.
    CatchProfile(const XML::Document& ProfileDoc);
    /// @brief Class destructor.
    ~CatchProfile();

    /// @brief Gets the name of this profile.
    /// @return Returns a const String reference to the name of this profile.
    const String& GetName() const;

    /// @brief Saves this profile to disk.
    /// @param ProfilesDir The directory to save this profile to.
    void Save(const String& ProfilesDir);

    void SetNewHighScore(const String& LevelName, const Whole& NewHigh);
    /// @brief Gets the highest saved score
    Whole GetHighestScore(const String& LevelName) const;
};//CatchProfile

/// @brief Convenience typedef allowing for possible reuse of code below.
typedef CatchProfile GameProfile;

///////////////////////////////////////////////////////////////////////////////
/// @brief This is used to store all of the detected/loaded profiles.
/// @details
///////////////////////////////////////
class ProfileManager : public ObjectSettingsHandler
{
public:
    /// @brief Basic container type for GameProfile instances stored in this class.
    typedef std::vector<GameProfile*>            ProfilesContainer;
    /// @brief Iterator type for GameProfile instances stored in this class.
    typedef ProfilesContainer::iterator          ProfilesIterator;
    /// @brief Const Iterator type for GameProfile instances stored in this class.
    typedef ProfilesContainer::const_iterator    ConstProfilesIterator;
protected:
    /// @internal
    /// @brief Container storing all of the loaded game profiles.
    ProfilesContainer LoadedProfiles;
    /// @internal
    /// @brief A path to where the game profiles are stored.
    String ProfilesDirectory;
    /// @internal
    /// @brief A pointer to the initialized Entresol housing the utilities that are needed to load levels.
    Entresol* TheEntresol;
    /// @internal
    /// @brief A pointer to the currently active profile where new profile data is saved to.
    GameProfile* ActiveProfile;

    /// @internal
    /// @brief Gets the name of the Root XML Node of this object when it is serialized.
    /// @return Returns a String containing the name of the XML Node for this manager.
    virtual String GetObjectRootNodeName() const;
    /// @internal
    /// @brief Adds all of this classes current configuration to an XML node.
    /// @param SettingsRootNode The XML Node the current settings will be applied to.
    virtual void AppendCurrentSettings(XML::Node& SettingsRootNode);
    /// @internal
    /// @brief Applies a group of configuration settings to this manager.
    /// @param Group The Settings Group to update this managers state with.
    virtual void ApplySettingGroupImpl(ObjectSettingGroup* Group);
public:
    /// @brief Class constructor.
    /// @param Ent A pointer to the initialized Entresol.
    /// @param ProfilesDir A path to where the game profiles are stored.
    ProfileManager(Entresol* Ent, const String& ProfilesDir);
    /// @brief XML constructor.
    /// @param Ent A pointer to the initialized Entresol.
    /// @param XMLNode The XML node containing all of the data to be deserialized for this managers configuration.
    ProfileManager(Entresol* Ent, const XML::Node& XMLNode);
    /// @brief Class destructor.
    virtual ~ProfileManager();

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    /// @brief Uses the set path to search for all available game profiles that can be loaded.
    /// @param Returns the number of levels that were detected.
    Whole DetectProfiles();
    /// @brief Applys the active profile data to the relevant widgets in the UI configuration.
    void ApplyProfileDataToUI();
    /// @brief Applys profile data to the relevant widgets in the UI configuration.
    /// @param Profile A pointer to the profile to be read when updating the UI configuration.
    void ApplyProfileDataToUI(GameProfile* Profile);

    /// @brief Updates all profile related data with the achievement of a new high score.
    /// @param LevelName The name of the level the high score was achieved in.
    /// @param LevelScore The new high score that was achieved.
    void SetNewHighScore(const String& LevelName, const Whole LevelScore);

    ///////////////////////////////////////////////////////////////////////////////
    // Profile Path Management

    /// @brief Sets the directory where the game profiles can be found.
    /// @param Path The path to where the profiles will be found and loaded.
    void SetProfilesDirectory(const String& Path);
    /// @brief Gets the directory where the game profiles can be found.
    /// @return Returns a const reference to a String storing where the game profiles can be found.
    const String& GetProfilesDirectory() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Profile Management

    /// @brief Creates a new blank profile.
    /// @param Name The name to be given to the created profile.
    /// @return Returns a pointer to the created profile.
    GameProfile* CreateNewProfile(const String& Name);
    /// @brief Loads a profile from the disk.
    /// @param FileName The name of the file to be loaded and parsed.
    /// @return Returns a pointer to the created profile.
    GameProfile* LoadProfile(const String& FileName);
    /// @brief Loads a profile from a parsed XML document.
    /// @param ProfileDoc The XML document to create a profile from.
    /// @return Returns a pointer to the created profile.
    GameProfile* LoadProfile(XML::Document& ProfileDoc);
    /// @brief Gets a profile by name.
    /// @param Name The name of the profile to retrieve.
    /// @return Returns a pointer to the requested profile.
    GameProfile* GetProfile(const String& Name) const;
    /// @brief Gets a profile by index.
    /// @param Index The index of the profile to retrieve.
    /// @return Returns a pointer to the requested profile.
    GameProfile* GetProfile(const Whole& Index) const;
    /// @brief Gets the number of currently loaded profiles.
    /// @return Returns a Whole representing the number of currently loaded profiles.
    Whole GetNumLoadedProfiles() const;
    /// @brief Saves a profile to the disk.
    /// @param Name The name of the profile to be saved to the disk.
    void SaveProfile(const String& Name);
    /// @brief Saves a profile to the disk.
    /// @param Profile A pointer to the profile to be saved to the disk.
    void SaveProfile(GameProfile* Profile);
    /// @brief Saves all currently loaded profiles to the disk.
    void SaveAllProfiles();

    ///////////////////////////////////////////////////////////////////////////////
    // ActiveProfile Management

    /// @brief Sets the currently active profile.
    /// @param Name The name of the profile to set.
    void SetActiveProfile(const String& Name);
    /// @brief Sets the currently active profile.
    /// @param Profile A pointer to the profile to set.
    void SetActiveProfile(GameProfile* Profile);
    /// @brief Gets the currently active profile.
    /// @return Returns a pointer to the profile that is currently being used and displayed.
    GameProfile* GetActiveProfile() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Initialization Methods

    /// @brief Configures this manager for use prior to entering the main loop.
    virtual void Initialize();
    /// @brief Removes this manager from any necessary configuration so it can be safely disposed of.
    virtual void Deinitialize();
};//ProfileManager

#endif
