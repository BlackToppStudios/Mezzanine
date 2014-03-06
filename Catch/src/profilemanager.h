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
    typedef std::map<String,Whole> LevelScoreMap;
    typedef LevelScoreMap::iterator LevelScoreMapIterator;
    typedef LevelScoreMap::const_iterator ConstLevelScoreMapIterator;
protected:
    LevelScoreMap LevelScores;
    String ProfileName;

    void SerializeProfileName(XML::Node& ProfileRoot) const;
    void SerializeLevelScores(XML::Node& ProfileRoot) const;
    void DeSerializeProfileName(const XML::Node& ProfileRoot);
    void DeSerializeLevelScores(const XML::Node& ProfileRoot);
public:
    /// @brief Class constructor.
    CatchProfile(const String& Name);
    /// @brief XML constructor.
    /// @param LevelDoc The XML document containing profile data.
    CatchProfile(const XML::Document& ProfileDoc);
    /// @brief Class destructor.
    ~CatchProfile();

    void Save(const String& ProfilesDir);

    ConstString& GetName() const;
    Whole GetHighestScore(const String& LevelName) const;

    void SetNewHighScore(const String& LevelName, const Whole& NewHigh);
};//CatchProfile

/// @brief Convenience typedef allowing for possible reuse of code below.
typedef CatchProfile GameProfile;

class ProfileManager
{
public:
    typedef std::vector<GameProfile*> ProfilesContainer;
    typedef ProfilesContainer::iterator ProfilesIterator;
    typedef ProfilesContainer::const_iterator ConstProfilesIterator;
protected:
    ProfilesContainer LoadedProfiles;
    GameProfile* ActiveProfile;
    String ProfilesDirectory;

    void PopulateLevelList(GameProfile* Profile);
public:
    ProfileManager(const String& ProfilesDir);
    ~ProfileManager();

    GameProfile* CreateNewProfile(const String& Name);
    GameProfile* LoadProfile(const String& FileName);
    GameProfile* LoadProfile(XML::Document& ProfileDoc);
    GameProfile* GetProfile(const String& Name) const;
    GameProfile* GetProfile(const Whole& Index) const;
    Whole GetNumLoadedProfiles() const;
    void SaveProfile(const String& Name);
    void SaveAllProfiles();

    void SetActiveProfile(const String& Name);
    void SetActiveProfile(GameProfile* Profile);
    GameProfile* GetActiveProfile() const;
};//ProfileManager

#endif
