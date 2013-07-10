#ifndef _levelscorer_h
#define _levelscorer_h

#include "levelzones.h"
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
    typedef std::vector<ActorBase*> ThrowableContainer;
    typedef std::vector<ScoreArea*> ScoreAreaContainer;
    typedef std::vector<ActorBase*> ActorVector;
    typedef std::list<ActorBase*> ActorList;
    typedef std::map<String,Whole> ItemScoreContainer;
protected:
    ItemScoreContainer ItemScoreValues;
    ScoreAreaContainer ScoreAreas;

    Entresol* TheEntresol;
    CatchApp* GameApp;
    Whole LevelTargetTime;

    Whole GetItemScoreValue(ActorBase* Item);
public:
    LevelScorer();
    ~LevelScorer();

    Whole CalculateNormalScore();
    Whole CalculateBonusScore();
    Whole CalculateTotalThrowableScore();
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
