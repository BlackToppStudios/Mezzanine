#ifndef _levelscorer_h
#define _levelscorer_h

#include "scorearea.h"
#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

struct ScoreAreaCache
{
    ScoreArea* Bonus;
    ScoreArea* Normal;
    ScoreAreaCache(ScoreArea* bonus,ScoreArea* normal) : Bonus(bonus), Normal(normal) {};
    ScoreAreaCache() : Bonus(NULL), Normal(NULL) {};
};

class LevelScorer
{
public:
    typedef std::vector<Debris*> ThrowableContainer;
    typedef std::vector<ScoreArea*> ScoreAreaContainer;
    typedef std::map<String,Whole> ItemScoreContainer;
    typedef std::pair<Debris*,Whole> ThrowableScorePair;
    typedef std::pair<Whole,Whole> ScorePair;
protected:
    ItemScoreContainer ItemScoreValues;
    ScoreAreaContainer ScoreAreas;

    Entresol* TheEntresol;
    CatchApp* GameApp;
    Whole LevelTargetTime;

    Whole GetItemScoreValue(Debris* Item);
    Real FindHighestMultiplier(Debris* Throwable);
public:
    LevelScorer();
    ~LevelScorer();

    ScorePair CalculateThrowableScore();
    Whole CalculateTimerScore();
    Whole CalculateItemCashScore();

    Whole PresentFinalScore();

    Whole GetNumScoreAreas() const;
    Whole GetNumAddedThrowables() const;
    Whole GetNumOverlappingThrowables() const;
    Whole GetNumRemovedThrowables() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Special Conditions

    ///////////////////////////////////////////////////////////////////////////////
    // Configuration

    void RegisterScoreArea(ScoreArea* Score);
    void SetThrowableScore(const String& TypeName, const Whole& Score);
    void SetLevelTargetTime(const Whole& TargetTime);
    void ResetLevelData();
    void ResetAllData();
};//LevelScorer

#endif
