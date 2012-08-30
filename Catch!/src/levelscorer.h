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
    protected:
        Entresol* TheEntresol;
        CatchApp* GameApp;
        Whole NormalScore;
        Whole BonusScore;
        Whole LevelTargetTime;
        std::map<String,Whole> ItemScoreValues;
        std::vector<ScoreArea*> ScoreAreas;
        std::vector<ScoreArea*> BonusScoreAreas;
        std::map<ActorBase*,ScoreAreaCache> ScoreCache;
        Whole GetItemScoreValue(ActorBase* Item);
    public:
        LevelScorer();
        ~LevelScorer();
        void CalculateCurrentScore(Whole& Score);
        Whole CalculateFinalScore();

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
