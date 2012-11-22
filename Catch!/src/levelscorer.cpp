#ifndef _levelscorer_cpp
#define _levelscorer_cpp

#include "levelscorer.h"
#include "catchapp.h"
#include "levelreportcell.h"

LevelScorer::LevelScorer()
    : BonusScore(0),
      NormalScore(0),
      LevelTargetTime(0)
{
    TheEntresol = Entresol::GetSingletonPtr();
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

    NormalScore = 0;
    BonusScore = 0;
    for( std::map<ActorBase*,ScoreAreaCache>::iterator SCit = ScoreCache.begin() ; SCit != ScoreCache.end() ;  )
    {
        if( (*SCit).second.Bonus )
        {
            BonusScore += (GetItemScoreValue( (*SCit).first ) * (*SCit).second.Bonus->GetScoreMultiplier() );
            ++SCit;
        }
        else if( (*SCit).second.Normal )
        {
            NormalScore += GetItemScoreValue( (*SCit).first );
            ++SCit;
        }
        else
        {
            std::map<ActorBase*,ScoreAreaCache>::iterator ToBeDestroyed = SCit;
            ++SCit;
            ScoreCache.erase(ToBeDestroyed);
        }
    }

    Score = NormalScore + BonusScore;
}

Whole LevelScorer::CalculateFinalScore()
{
    // Init non-saved scores
    CatchApp* App = CatchApp::GetCatchAppPointer();
    Whole ShopScore = 0;
    Whole TimeScore = 0;

    // Calculate the Shop Score
    ItemShop* Shop = App->GetItemShop();
    Real CashRatio = (Real)Shop->GetCurrentCash() / (Real)Shop->GetStarterCash();
    ShopScore = ((Whole)(CashRatio * 10)) * 10;

    // Calculate the Time Score
    SimpleTimer* LevelTimer = App->GetLevelTimer();
    Whole Time = LevelTimer->GetCurrentTimeInMilli() * 0.001;
    if(Time < LevelTargetTime)
    {
        std::vector<ActorBase*>& Throwables = App->GetThrowables();
        Whole ActorsInScoreZones = 0;

        for( Whole Areas = 0 ; Areas < ScoreAreas.size() ; ++Areas )
        {
            std::list<ActorBase*>& Overlapping = ScoreAreas[Areas]->GetOverlappingActors();
            for( std::list<ActorBase*>::iterator It = Overlapping.begin() ; It != Overlapping.end() ; ++It )
            {
                if( App->IsAThrowable( *It ) )
                    ActorsInScoreZones++;
            }
        }

        Real ThrowableRatio = ((Real)ActorsInScoreZones / (Real)Throwables.size());
        Real SubTotal = (((Real)LevelTargetTime - (Real)Time) * 10) * ThrowableRatio;
        Real Margin = MathTool::Fmod(SubTotal,5.0);
        if( Margin >= 2.5 )
        {
            SubTotal += (5.0 - Margin);
            TimeScore = (Whole)(SubTotal + 0.5);
        }
        else if( Margin < 2.5 )
        {
            SubTotal -= Margin;
            TimeScore = (Whole)(SubTotal + 0.5);
        }
    }

    // Update the UI to reflect the calculated scores
    UI::Screen* GameScreen = UIManager::GetSingletonPtr()->GetScreen("GameScreen");
    UI::Window* ReportWin = static_cast<UI::Window*>(GameScreen->GetWidget("GS_LevelReport"));

    UI::Caption* TotalDisplay = static_cast<UI::Caption*>(ReportWin->GetAreaRenderable("GS_ScoreDisplay"));
    UI::ScrolledCellGrid* BreakdownList = static_cast<UI::ScrolledCellGrid*>(ReportWin->GetWidget("GS_ScoreBreakdown"));
    Vector2 BreakDownPosition(0.1,0.1);
    Vector2 BreakDownSize = BreakdownList->GetFixedCellSize();

    LevelReportCell* NormalScoreCell = new LevelReportCell("GS_NormalCell",UI::RenderableRect(BreakDownPosition,BreakDownSize,true),GameScreen);
    NormalScoreCell->GetDescription()->SetText("Normal Area Score:");
    NormalScoreCell->GetScore()->SetText(StringTools::ConvertToString(NormalScore));
    BreakdownList->AddCell(NormalScoreCell);

    LevelReportCell* BonusScoreCell = new LevelReportCell("GS_BonusCell",UI::RenderableRect(BreakDownPosition,BreakDownSize,true),GameScreen);
    BonusScoreCell->GetDescription()->SetText("Bonus Area Score:");
    BonusScoreCell->GetScore()->SetText(StringTools::ConvertToString(BonusScore));
    BreakdownList->AddCell(BonusScoreCell);

    LevelReportCell* RemainingCashBonusCell = new LevelReportCell("GS_RemainingCashBonusCell",UI::RenderableRect(BreakDownPosition,BreakDownSize,true),GameScreen);
    RemainingCashBonusCell->GetDescription()->SetText("Shop Cash Score:");
    RemainingCashBonusCell->GetScore()->SetText(StringTools::ConvertToString(ShopScore));
    BreakdownList->AddCell(RemainingCashBonusCell);

    LevelReportCell* TimeBonusCell = new LevelReportCell("GS_TimeBonusCell",UI::RenderableRect(BreakDownPosition,BreakDownSize,true),GameScreen);
    TimeBonusCell->GetDescription()->SetText("Time Score:");
    TimeBonusCell->GetScore()->SetText(StringTools::ConvertToString(TimeScore));
    BreakdownList->AddCell(TimeBonusCell);

    /*for(  ;  ;  )
    {
        std::stringstream namestream;
        namestream << "Condition" << ;
        LevelReportCell* SpecialConditionCell = new LevelReportCell(namestream.str(),,GameScreen);
        SpecialConditionCell->GetDescription()->SetText("");
        SpecialConditionCell->GetScore()->SetText("");
        BreakdownList->AddCell(SpecialConditionCell);
        // For-Loop for special conditions from LUA.
    }//*/

    BreakdownList->GenerateGrid();

    Whole TotalScore = NormalScore+BonusScore+ShopScore+TimeScore;
    TotalDisplay->SetText(StringTools::ConvertToString(TotalScore));

    ReportWin->Show();
    return TotalScore;
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

void LevelScorer::SetLevelTargetTime(const Whole& TargetTime)
{
    LevelTargetTime = TargetTime;
}

void LevelScorer::ResetLevelData()
{
    UI::Screen* GameScreen = UIManager::GetSingletonPtr()->GetScreen("GameScreen");
    UI::Window* ReportWin = static_cast<UI::Window*>(GameScreen->GetWidget("GS_LevelReport"));
    UI::ScrolledCellGrid* BreakdownList = static_cast<UI::ScrolledCellGrid*>(ReportWin->GetWidget("GS_ScoreBreakdown"));

    ScoreAreas.clear();
    BonusScoreAreas.clear();
    ScoreCache.clear();
    BonusScore = 0;
    NormalScore = 0;
    LevelTargetTime = 0;

    BreakdownList->DestroyAllCells();
}

void LevelScorer::ResetAllData()
{
    ResetLevelData();
    ItemScoreValues.clear();
}


#endif
