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

Whole LevelScorer::GetItemScoreValue(ActorBase* Item)
{
    String ItemName = Item->GetName();
    String TypeName = ItemName.substr(0,ItemName.find_first_of("0123456789"));
    std::map<String,Whole>::iterator it = ItemScoreValues.find(TypeName);
    if( it != ItemScoreValues.end() ) return (*it).second;
    else return 0;
}

void LevelScorer::CalculateCurrentScore(Whole& Score)
{
    for( Whole B = 0 ; B < BonusScoreAreas.size() ; B++ )
    {
        ScoreArea* CurrArea = BonusScoreAreas.at(B);
        std::vector<ActorBase*>& Added = CurrArea->GetAddedActors();
        std::vector<ActorBase*>& Removed = CurrArea->GetRemovedActors();
        for( Whole A = 0 ; A < Added.size() ; A++ )
        {
            std::map<ActorBase*,ScoreAreaCache>::iterator it = ScoreCache.find(Added[A]);
            if( it != ScoreCache.end() ) (*it).second.Bonus = CurrArea;
            else ScoreCache.insert(std::pair<ActorBase*,ScoreAreaCache>(Added[A],ScoreAreaCache(CurrArea,NULL)));
        }
        for( Whole R = 0 ; R < Removed.size() ; R++ )
        {
            std::map<ActorBase*,ScoreAreaCache>::iterator it = ScoreCache.find(Removed[R]);
            if( it != ScoreCache.end() ) (*it).second.Bonus = NULL;
        }
    }

    for( Whole S = 0 ; S < ScoreAreas.size() ; S++ )
    {
        ScoreArea* CurrArea = ScoreAreas.at(S);
        std::vector<ActorBase*>& Added = CurrArea->GetAddedActors();
        std::vector<ActorBase*>& Removed = CurrArea->GetRemovedActors();
        for( Whole A = 0 ; A < Added.size() ; A++ )
        {
            std::map<ActorBase*,ScoreAreaCache>::iterator it = ScoreCache.find(Added[A]);
            if( it != ScoreCache.end() ) (*it).second.Normal = CurrArea;
            else ScoreCache.insert(std::pair<ActorBase*,ScoreAreaCache>(Added[A],ScoreAreaCache(NULL,CurrArea)));

        }
        for( Whole R = 0 ; R < Removed.size() ; R++ )
        {
            std::map<ActorBase*,ScoreAreaCache>::iterator it = ScoreCache.find(Removed[R]);
            if( it != ScoreCache.end() ) (*it).second.Normal = NULL;
        }
    }

    Whole UpdatedScore = 0;
    for( std::map<ActorBase*,ScoreAreaCache>::iterator SCit = ScoreCache.begin() ; SCit != ScoreCache.end() ;  )
    {
        if( (*SCit).second.Bonus )
        {
            UpdatedScore += (GetItemScoreValue( (*SCit).first ) * (*SCit).second.Bonus->GetScoreMultiplier() );
            ++SCit;
        }
        else if( (*SCit).second.Normal )
        {
            UpdatedScore += GetItemScoreValue( (*SCit).first );
            ++SCit;
        }
        else
        {
            std::map<ActorBase*,ScoreAreaCache>::iterator ToBeDestroyed = SCit;
            ++SCit;
            ScoreCache.erase(ToBeDestroyed);
        }
    }

    Score = UpdatedScore;
}

void LevelScorer::CalculateFinalScore(Whole& FinalScore)
{

}

void LevelScorer::RegisterScoreArea(ScoreArea* Score)
{
    if( 1 < Score->GetScoreMultiplier() ) BonusScoreAreas.push_back(Score);
    else ScoreAreas.push_back(Score);
}

void LevelScorer::SetThrowableScore(const String& TypeName, const Whole& Score)
{
    ItemScoreValues[TypeName] = Score;
    //ItemScoreValues.insert(std::pair<String,Whole>(TypeName,Score));
}

void LevelScorer::ResetLevelData()
{
    ScoreAreas.clear();
    BonusScoreAreas.clear();
    ScoreCache.clear();
}

void LevelScorer::ResetAllData()
{
    ResetLevelData();
    ItemScoreValues.clear();
}


#endif
