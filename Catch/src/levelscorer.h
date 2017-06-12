#ifndef _levelscorer_h
#define _levelscorer_h

#include "WorldEntities/scorearea.h"
#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

///////////////////////////////////////////////////////////////////////////////
/// @brief This keeps track of the available levels and is primarily responsible for their loading.
/// @details
///////////////////////////////////////
class LevelScorer
{
public:
    /// @brief Container type for Throwables stored and operated on within this class.
    typedef std::vector<Debris*>            ThrowableContainer;
    /// @brief Container type for storing the score values for each throwable type.
    typedef std::map<String,Whole>          ItemScoreContainer;
    /// @brief Convenience typedef for passing a throwable and the score associated with it.
    typedef std::pair<Debris*,Whole>        ThrowableScorePair;
    /// @brief Convenience typedef for passing both the normal and bonus throwable score.
    typedef std::pair<Whole,Whole>          ScorePair;
protected:
    /// @internal
    /// @brief Container storing all of the scores for known throwables.
    ItemScoreContainer ItemScoreValues;
    /// @internal
    /// @brief A pointer to the Mezzanine root.
    Entresol* TheEntresol;
    /// @internal
    /// @brief A pointer to the game root.
    CatchApp* GameApp;

    /// @internal
    /// @brief Gets the normal score value of a throwable.
    /// @param Item A pointer to the throwable to check for.
    /// @return Returns a Whole representing the normal score of the specified throwable.
    Whole GetItemScoreValue(Debris* Item);
    /// @internal
    /// @brief Gets the highest multiplier to be applied among all the score zones a throwable is in.
    /// @param Throwable A pointer to the throwable to check for.
    /// @return Returns a Real representing the highest bonus to be applied to the specified throwable.
    Real FindHighestMultiplier(Debris* Throwable);
public:
    /// @brief Class constructor.
    /// @param App A pointer to the game root.
    /// @param Engine A pointer to the Mezzanine root.
    LevelScorer(CatchApp* App, Entresol* Engine);
    /// @brief Class destructor.
    ~LevelScorer();

    /// @brief Calculates the current Normal and Bonus throwable score.
    /// @return Returns a ScorePair storing the Normal throwable score in the first member, and the bonus throwable score in the second member.
    ScorePair CalculateThrowableScore();
    /// @brief Calculates the current Timer score.
    /// @return Returns a Whole representing the bonus to the level score if the level were completed immediately.
    Whole CalculateTimerScore();
    /// @brief Calculates the Shop score.
    /// @return Returns a Whole representing the bonus to the level score if the level were completed immediately.
    Whole CalculateItemCashScore();

    /// @brief Calculates the final score of the level and presents the level report UI.
    /// @return Returns the total and final score of a level.
    Whole PresentFinalScore();

    /// @brief Gets the number of Score Areas known to this Scorer.
    /// @return Returns a Whole reprsenting the number of known Score Areas.
    Whole GetNumScoreAreas() const;
    /// @brief Gets the number of throwables that have entered known Score Areas since the last frame.
    /// @return Returns a Whole representing the number of throwables that have entered Score Areas since last frame.
    Whole GetNumAddedThrowables() const;
    /// @brief Gets the number of throwables currently inside known Score Areas.
    /// @return Returns a Whole representing the number of throwables that are currently inside known Score Areas.
    Whole GetNumOverlappingThrowables() const;
    /// @brief Gets the number of throwables that have left known Score Areas since the last frame.
    /// @return Returns a Whole representing the number of throwables that have left Score Areas since last frame.
    Whole GetNumRemovedThrowables() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Special Conditions

    ///////////////////////////////////////////////////////////////////////////////
    // Configuration

    /// @brief Sets the score value for a throwable type.
    /// @param TypeName The name of the type of throwable to be set.
    /// @param Score The score to be assigned to the throwable of the specified type.
    void SetThrowableScore(const String& TypeName, const Whole& Score);
    /// @brief Clears all the level specific data stored in this Scorer.
    void ResetLevelData();
    /// @brief Clears all data stored in this Scorer.
    void ResetAllData();
};//LevelScorer

#endif
