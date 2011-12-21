#ifndef _levelscorer_cpp
#define _levelscorer_cpp

#include "levelscorer.h"
#include "catchapp.h"

LevelScorer::LevelScorer()
{
    TheWorld = World::GetWorldPointer();
    GameApp = CatchApp::GetCatchAppPointer();
}

LevelScorer::~LevelScorer()
{
    ResetAllData();
}

void LevelScorer::CalculateCurrentScore(Whole& Score)
{
    for( Whole S = 0 ; S < ScoreAreas.size() ; S++ )
    {
        std::vector<ActorBase*>& Added = ScoreAreas[S]->GetAddedActors();
        std::vector<ActorBase*>& Removed = ScoreAreas[S]->GetRemovedActors();
        for( Whole A = 0 ; A < Added.size() ; A++ )
        {
            String ItemName = Added[A]->GetName();
            for( std::map<String,Whole>::iterator Ait = ItemScoreValues.begin() ; Ait != ItemScoreValues.end() ; Ait++ )
            {
                if(ItemName.find((*Ait).first) != String::npos)
                    Score += (*Ait).second;
            }
        }
        for( Whole R = 0 ; R < Removed.size() ; R++ )
        {
            String ItemName = Removed[R]->GetName();
            for( std::map<String,Whole>::iterator Rit = ItemScoreValues.begin() ; Rit != ItemScoreValues.end() ; Rit++ )
            {
                if(ItemName.find((*Rit).first) != String::npos)
                    Score -= (*Rit).second;
            }
        }
    }
}

void LevelScorer::CalculateFinalScore(Whole& FinalScore)
{

}

void LevelScorer::RegisterScoreArea(ScoreArea* Score)
{
    if( 1 < Score->GetScoreMultiplier() ) BonusScoreAreas.push_back(Score);
    else ScoreAreas.push_back(Score);
}

void LevelScorer::PopulateScoreValues(xml::Node& ItemScoreNode)
{
    /// @todo This needs to be updated to properly deserialize the values.
    ItemScoreValues["Gold"] = 100;
    ItemScoreValues["Pyrite"] = 50;
    ItemScoreValues["Clay"] = 10;
    ItemScoreValues["Uranium"] = 100;
    ItemScoreValues["Rubber"] = 30;
    ItemScoreValues["Lead"] = 30;
    ItemScoreValues["Styrofoam"] = 10;
    ItemScoreValues["Wood"] = 20;
}

void LevelScorer::PopulateShopValues(xml::Node& ItemValueNode)
{
    /// @todo This needs to be updated to properly deserialize the values.
    ShopCostValues["Wooden Plank"] = 50;
}

void LevelScorer::ResetLevelData()
{
    ScoreAreas.clear();
    BonusScoreAreas.clear();
}

void LevelScorer::ResetAllData()
{
    ResetLevelData();
    ItemScoreValues.clear();
    ShopCostValues.clear();
}


#endif
