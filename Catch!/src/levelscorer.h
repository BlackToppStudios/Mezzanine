#ifndef _levelscorer_h
#define _levelscorer_h

#include "levelzones.h"
#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

class LevelScorer
{
    protected:
        World* TheWorld;
        CatchApp* GameApp;
        std::map<String,Whole> ItemScoreValues;
        std::map<String,Whole> ShopCostValues;
        std::vector<ScoreArea*> ScoreAreas;
        std::vector<ScoreArea*> BonusScoreAreas;
    public:
        LevelScorer();
        ~LevelScorer();
        void CalculateCurrentScore(Whole& Score);
        void CalculateFinalScore(Whole& FinalScore);

        ///////////////////////////////////////////////////////////////////////////////
        // Configuration
        void RegisterScoreArea(ScoreArea* Score);
        void PopulateScoreValues(xml::Node& ItemScoreNode);
        void PopulateShopValues(xml::Node& ItemValueNode);
        void ResetLevelData();
        void ResetAllData();
};//LevelScorer

#endif
