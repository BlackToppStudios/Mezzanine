#ifndef _levelmanager_h
#define _levelmanager_h

#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

///////////////////////////////////////////////////////////////////////////////
/// @brief This is used to store simple data related to a level that can be loaded.
/// @details
///////////////////////////////////////
class CatchLevel
{
public:
    /// @brief Convenience typedef for handling return values from the Scripting subsystem.
    typedef CountedPtr< Scripting::iScript >    ScriptPtr;
    /// @brief Container type for the storage of score tiers.
    typedef std::vector<Whole>                  ScoreTierContainer;
    /// @brief Iterator type for score tiers of a level.
    typedef ScoreTierContainer::iterator        ScoreTierIterator;
    /// @brief Const Iterator type for score tiers of a level.
    typedef ScoreTierContainer::const_iterator  ConstScoreTierIterator;
    /// @brief Container type for the storage of level-specific scripts.
    typedef std::vector<ScriptPtr>              ScriptContainer;
    /// @brief Iterator type for loaded level-specific scripts.
    typedef ScriptContainer::iterator           ScriptIterator;
    /// @brief Const Iterator type for loaded level-specific scripts.
    typedef ScriptContainer::const_iterator     ConstScriptIterator;
protected:
    /// @internal
    /// @brief Container storing all the score tiers for this level.
    ScoreTierContainer ScoreTiers;
    /// @brief
    /// @brief Container storing all of the scripts that are to be run while this level is loaded.
    ScriptContainer LevelScripts;
    /// @internal
    /// @brief The name of the preview Atlas.
    String PreviewAtlasName;
    /// @internal
    /// @brief The name of the preview image.
    String PreviewImageName;
    /// @internal
    /// @brief The name of this level.
    String LevelName;
    /// @internal
    /// @brief The name of this levels Asset Group.
    String GroupName;

    /// @internal
    /// @brief Deserializes the relevant data about the level, and populates this class with it.
    /// @param LevelDoc The XML document containing the data to populate this class with.
    void DeSerializeLevelData(const XML::Document& LevelDoc);
public:
    /// @brief Simple name constructor.
    /// @remarks This is only used to create an entry for the mainmenu level, which is only needed temporarily.
    /// @param LvlName The name of the level to be created.  Should only ever be "MainMenu".
    CatchLevel(const String& LvlName);
    /// @brief XML constructor.
    /// @param GroupName The name of the Asset Group that was created for this level.
    /// @param LevelDoc The XML document containing level data.
    CatchLevel(const String& Group, const XML::Document& LevelDoc);
    /// @brief Class destructor.
    ~CatchLevel();

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    /// @brief Gets the name of this level.
    /// @return Returns a const String reference containing the name of this level.
    const String& GetName() const;
    /// @brief Gets the name of the Asset Group created for this level.
    /// @return Returns a const String reference containing the name of the Asset Group belonging to this level.
    const String& GetGroupName() const;
    /// @brief Gets the name of the Atlas storing additional UI data for this levels preview.
    /// @return Returns a const String reference containing the name of the preview atlas for this level.
    const String& GetPreviewAtlasName() const;
    /// @brief Gets the name of the preview image for this level.
    /// @return Returns a const String reference containing the name of the preview image for this level.
    const String& GetPreviewImageName() const;

    ///////////////////////////////////////////////////////////////////////////////
    // MetaData Access

    /// @brief Gets the score threshold for the specified tier for this level.
    /// @param Tier The tier to get the score threshold of.
    /// @return Returns a Whole representing the score needed to achieve that tier.
    Whole GetScoreThreshold(const Whole Tier) const;
    /// @brief Gets the number of score tiers for this level.
    /// @return Returns a Whole representing how many score tiers this level has.
    Whole GetNumScoreTiers() const;

    /// @brief Gets a script that is to be run while this level is loaded.
    /// @param Index The Index of the Script to retrieve.
    /// @return Returns a CountedPtr to a script run while this level is loaded.
    ScriptPtr GetLevelScript(const Whole Index) const;
    /// @brief Gets the number of scipts run while this level is loaded.
    /// @return Returns a Whole representing how many scipts are to be run while this level is loaded.
    Whole GetNumLevelScripts() const;
};//CatchLevel

/// @brief Convenience typedef allowing for possible reuse of code below.
typedef CatchLevel GameLevel;

///////////////////////////////////////////////////////////////////////////////
/// @brief This keeps track of the available levels and is primarily responsible for their loading.
/// @details
///////////////////////////////////////
class LevelManager
{
public:
    /// @brief Container type for detected game levels.
    typedef std::vector< GameLevel* >             GameLevelContainer;
    /// @brief Iterator type for detected game levels.
    typedef GameLevelContainer::iterator          GameLevelIterator;
    /// @brief Const Iterator type for detected game levels.
    typedef GameLevelContainer::const_iterator    ConstGameLevelIterator;
protected:
    /// @internal
    /// @brief Container storing all of the detected levels.
    GameLevelContainer GameLevels;
    /// @internal
    /// @brief Stores the path to the levels directory.
    String LevelPath;
    /// @internal
    /// @brief A pointer to the initialized Entresol housing the utilities that are needed to load levels.
    Entresol* TheEntresol;
    /// @internal
    /// @brief A pointer to the level that will be loaded next time "LoadLevel()" is called.
    GameLevel* LevelToLoad;
    /// @internal
    /// @brief A pointer to the currently loaded level.
    GameLevel* CurrentLevel;

    /// @internal
    /// @brief Convenience method for Resource Manager retrieval.
    /// @return Returns a pointer to the Resource Manager to be used for I/O.
    Resource::ResourceManager* GetResourceManager() const;
public:
    /// @brief Class constructor.
    /// @param Ent A pointer to the initialized Entresol.
    /// @param PathToLevels The path to the directory where the levels can be found.
    LevelManager(Entresol* Ent, const String& PathToLevels);
    /// @brief Class destructor.
    ~LevelManager();

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    /// @brief Uses the set path to search for all availble game levels that can be loaded.
    /// @return Returns the number of levels that were detected.
    Whole DetectLevels();
    /// @brief Populates the Level Select widget with all of the detected levels.
    void PopulateLevelSelectUI();

    /// @brief Gets a level by Index.
    /// @param Index The index of the level to retrieve.
    /// @return Returns a pointer to the level at the specified index.
    GameLevel* GetLevel(const Whole Index);
    /// @brief Gets a level by name.
    /// @param LevelName The name of the level to retrieve.
    /// @return Returns a pointer to the named level.
    GameLevel* GetLevel(const String& LevelName);
    /// @brief Gets the number of levels that were detected the last time "DetectLevels" was called.
    /// @return Returns a Whole representing the number of currenly known, loadable levels.
    Whole GetNumLevels() const;

    /// @brief Sets the next level to be loaded by name.
    /// @param LevelName A string containing the name of the next level that will be loaded.
    void SetNextLevel(const String& LevelName);
    /// @brief Sets the next level to be loaded.
    /// @param NextLevel A pointer to the level that will be loaded.
    void SetNextLevel(GameLevel* NextLevel);
    /// @brief Gets a pointer to the next level that will be loaded.
    /// @return Returns a pointer to the GameLevel that will be loaded next, or NULL if none are set.
    GameLevel* GetNextLevel() const;

    /// @brief Gets a pointer to the current level that is loaded.
    /// @return Returns a pointer to the currently loaded level, or NULL if something horribly, horribly wrong has happened.
    GameLevel* GetCurrentLevel() const;
    /// @brief Checks to ensure that another level will be loaded upon the current level.
    /// @return Returns true if another level is set for loading, false if no level is set.
    Boole HasALevelToLoad() const;

    /// @brief Sets the path to where the levels will be loaded from.
    /// @param PathToLevels A String containing the path to find levels to be loaded.
    void SetLevelPath(const String& PathToLevels);
    /// @brief Gets the path to where the levels will be loaded from.
    /// @return Returns a const String reference containing the path to the directory where the levels are stored.
    const String& GetLevelPath() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Loading and Unloading

    /// @brief Loads the level currently set as "LevelToLoad" and sets it as the current level.
    void LoadNextLevel();
    /// @brief Unloads the current level.
    void UnloadLevel();
};//LevelManager

#endif
