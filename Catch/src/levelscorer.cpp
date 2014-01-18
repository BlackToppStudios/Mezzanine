#ifndef _levelscorer_cpp
#define _levelscorer_cpp

#include "levelscorer.h"
#include "catchapp.h"
#include "levelreportcell.h"

LevelScorer::LevelScorer() :
    LevelTargetTime(0)
{
    TheEntresol = Entresol::GetSingletonPtr();
    GameApp = CatchApp::GetCatchAppPointer();
}

LevelScorer::~LevelScorer()
{
    ResetAllData();
}

Whole LevelScorer::GetItemScoreValue(Debris* Item)
{
    String ItemName = Item->GetName();
    String TypeName = ItemName.substr(0,ItemName.find_first_of("0123456789"));
    ItemScoreContainer::iterator it = this->ItemScoreValues.find(TypeName);
    if( it != this->ItemScoreValues.end() ) return (*it).second;
    else return 0;
}

Real LevelScorer::FindHighestMultiplier(Debris* Throwable)
{
    Real RetMulti = 0.0;
    for( ScoreAreaContainer::const_iterator ScoreIt = this->ScoreAreas.begin() ; ScoreIt != this->ScoreAreas.end() ; ++ScoreIt )
    {
        Real ScoreMulti = (*ScoreIt)->GetScoreMultiplier();
        AreaEffect::ObjectContainer& Overlapping = (*ScoreIt)->GetOverlappingObjects();
        for( AreaEffect::ConstObjectIterator ObjIt = Overlapping.begin() ; ObjIt != Overlapping.end() ; ++ObjIt )
        {
            if( (*ObjIt) == Throwable && ScoreMulti > RetMulti ) {
                RetMulti = ScoreMulti;
            }
        }
    }
    return RetMulti;
}

LevelScorer::ScorePair LevelScorer::CalculateThrowableScore()
{
    ScorePair Ret;
    CatchApp::ThrowableContainer& Throwables = this->GameApp->GetThrowables();
    for( CatchApp::ThrowableContainer::iterator ThrowIt = Throwables.begin() ; ThrowIt != Throwables.end() ; ++ThrowIt )
    {
        Debris* Throwable = (*ThrowIt);
        Real Multiplier = this->FindHighestMultiplier( Throwable );
        if( Multiplier == 1.0 ) {
            Ret.first += this->GetItemScoreValue( Throwable );
        }else if( Multiplier > 1.0 ) {
            Ret.second += this->GetItemScoreValue( Throwable ) * Multiplier;
        }
    }
    return Ret;
    /*ScorePair Ret;
    for( ScoreAreaContainer::const_iterator ScoreIt = this->ScoreAreas.begin() ; ScoreIt != this->ScoreAreas.end() ; ++ScoreIt )
    {
        Real ScoreMulti = (*ScoreIt)->GetScoreMultiplier();
        ActorList& Overlapping = (*ScoreIt)->GetOverlappingActors();
        for( ActorList::const_iterator It = Overlapping.begin() ; It != Overlapping.end() ; ++It )
        {
            if( this->GameApp->IsAThrowable( *It ) ) {
                if( ScoreMulti == 1.0 ) {
                    Ret.first += this->GetItemScoreValue( *It );
                }else if( ScoreMulti > 1.0 ) {
                    Ret.second += ( this->GetItemScoreValue( *It ) * ScoreMulti );
                }
            }
        }
    }
    return Ret;//*/
}

Whole LevelScorer::CalculateTimerScore()
{
    // Setup the return
    Whole TimeScore = 0;
    // Calculate the Time Score
    Timer* LevelTimer = this->GameApp->GetLevelTimer();
    Whole Time = LevelTimer->GetCurrentTimeInMilliseconds() * 0.001;
    if(Time < this->LevelTargetTime)
    {
        ThrowableContainer& Throwables = this->GameApp->GetThrowables();
        Whole ActorsInScoreZones = 0;

        for( Whole Areas = 0 ; Areas < this->ScoreAreas.size() ; ++Areas )
        {
            AreaEffect::ObjectContainer& Overlapping = this->ScoreAreas[Areas]->GetOverlappingObjects();
            for( AreaEffect::ObjectIterator ObjIt = Overlapping.begin() ; ObjIt != Overlapping.end() ; ++ObjIt )
            {
                if( this->GameApp->IsAThrowable( *ObjIt ) )
                    ActorsInScoreZones++;
            }
        }

        Real ThrowableRatio = ((Real)ActorsInScoreZones / (Real)Throwables.size());
        Real SubTotal = (((Real)this->LevelTargetTime - (Real)Time) * 10) * ThrowableRatio;
        Real Margin = MathTools::Fmod(SubTotal,5.0);
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
    return TimeScore;
}

Whole LevelScorer::CalculateItemCashScore()
{
    // Calculate the Shop Score
    ItemShop* Shop = this->GameApp->GetItemShop();
    Real CashRatio = (Real)Shop->GetCurrentCash() / (Real)Shop->GetStarterCash();
    return ((Whole)(CashRatio * 10)) * 10;
}

Whole LevelScorer::PresentFinalScore()
{
    ScorePair ThrowableScores = this->CalculateThrowableScore();
    Whole NormalScore = ThrowableScores.first;
    Whole BonusScore = ThrowableScores.second;
    Whole ShopScore = this->CalculateItemCashScore();
    Whole TimeScore = this->CalculateTimerScore();

    // Update the UI to reflect the calculated scores
    /// @todo UI Update
    /*UI::Screen* GameScreen = UI::UIManager::GetSingletonPtr()->GetScreen("GameScreen");
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

    Whole ObjectiveScore = 0;
    /*for(  ;  ;  )
    {
        std::stringstream namestream;
        namestream << "Condition" << ;
        LevelReportCell* SpecialConditionCell = new LevelReportCell(namestream.str(),,GameScreen);
        SpecialConditionCell->GetDescription()->SetText("");
        SpecialConditionCell->GetScore()->SetText("");
        BreakdownList->AddCell(SpecialConditionCell);
        // For-Loop for special conditions from LUA.
    }

    BreakdownList->GenerateGrid();

    Whole TotalScore = NormalScore+BonusScore+ShopScore+TimeScore+ObjectiveScore;
    TotalDisplay->SetText(StringTools::ConvertToString(TotalScore));

    ReportWin->Show();//*/
    /// @todo UI Update
    return 0;
}

Whole LevelScorer::GetNumScoreAreas() const
{
    return this->ScoreAreas.size();
}

Whole LevelScorer::GetNumAddedThrowables() const
{
    Whole ThrowableCount = 0;
    for( ScoreAreaContainer::const_iterator ScoreIt = this->ScoreAreas.begin() ; ScoreIt != this->ScoreAreas.end() ; ++ScoreIt )
    {
        AreaEffect::ObjectContainer& Added = (*ScoreIt)->GetAddedObjects();
        for( AreaEffect::ConstObjectIterator ObjIt = Added.begin() ; ObjIt != Added.end() ; ++ObjIt )
        {
            if( this->GameApp->IsAThrowable( *ObjIt ) )
                ++ThrowableCount;
        }
    }
    return ThrowableCount;
}

Whole LevelScorer::GetNumOverlappingThrowables() const
{
    Whole ThrowableCount = 0;
    for( ScoreAreaContainer::const_iterator ScoreIt = this->ScoreAreas.begin() ; ScoreIt != this->ScoreAreas.end() ; ++ScoreIt )
    {
        AreaEffect::ObjectContainer& Overlapping = (*ScoreIt)->GetOverlappingObjects();
        for( AreaEffect::ConstObjectIterator ObjIt = Overlapping.begin() ; ObjIt != Overlapping.end() ; ++ObjIt )
        {
            if( this->GameApp->IsAThrowable( *ObjIt ) )
                ++ThrowableCount;
        }
    }
    return ThrowableCount;
}

Whole LevelScorer::GetNumRemovedThrowables() const
{
    Whole ThrowableCount = 0;
    for( ScoreAreaContainer::const_iterator ScoreIt = this->ScoreAreas.begin() ; ScoreIt != this->ScoreAreas.end() ; ++ScoreIt )
    {
        AreaEffect::ObjectContainer& Removed = (*ScoreIt)->GetRemovedObjects();
        for( AreaEffect::ConstObjectIterator ObjIt = Removed.begin() ; ObjIt != Removed.end() ; ++ObjIt )
        {
            if( this->GameApp->IsAThrowable( *ObjIt ) )
                ++ThrowableCount;
        }
    }
    return ThrowableCount;
}

///////////////////////////////////////////////////////////////////////////////
// Special Conditions

///////////////////////////////////////////////////////////////////////////////
// Configuration

void LevelScorer::RegisterScoreArea(ScoreArea* Score)
{
    this->ScoreAreas.push_back(Score);
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
    /// @todo UI Update
    /*UI::Screen* GameScreen = UI::UIManager::GetSingletonPtr()->GetScreen("GameScreen");
    UI::Window* ReportWin = static_cast<UI::Window*>(GameScreen->GetWidget("GS_LevelReport"));
    UI::ScrolledCellGrid* BreakdownList = static_cast<UI::ScrolledCellGrid*>(ReportWin->GetWidget("GS_ScoreBreakdown"));

    ScoreAreas.clear();
    LevelTargetTime = 0;

    BreakdownList->DestroyAllCells();//*/
}

void LevelScorer::ResetAllData()
{
    ResetLevelData();
    ItemScoreValues.clear();
}

#endif
