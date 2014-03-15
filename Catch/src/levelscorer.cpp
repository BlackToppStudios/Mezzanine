#ifndef _levelscorer_cpp
#define _levelscorer_cpp

#include "levelscorer.h"
#include "catchapp.h"

LevelScorer::LevelScorer(CatchApp* App, Entresol* Engine) :
    TheEntresol(Engine),
    GameApp(App)
    {  }

LevelScorer::~LevelScorer()
    { this->ResetAllData(); }

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
    AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
    AreaEffectManager::ConstAreaEffectIterator End = AEMan->EndAreaEffect();
    for( AreaEffectManager::ConstAreaEffectIterator ScoreIt = AEMan->BeginAreaEffect() ; ScoreIt != End ; ++ScoreIt )
    {
        if( (*ScoreIt)->GetDerivedSerializableName() == "ScoreArea" ) {
            ScoreArea* CurrScore = static_cast<ScoreArea*>( *ScoreIt );
            Real ScoreMulti = CurrScore->GetScoreMultiplier();
            AreaEffect::ObjectContainer& Overlapping = CurrScore->GetOverlappingObjects();
            for( AreaEffect::ConstObjectIterator ObjIt = Overlapping.begin() ; ObjIt != Overlapping.end() ; ++ObjIt )
            {
                if( (*ObjIt) == Throwable && ScoreMulti > RetMulti ) {
                    RetMulti = ScoreMulti;
                }
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
}

Whole LevelScorer::CalculateTimerScore()
{
    // Setup the return
    Whole TimeScore = 0;
    // Calculate the Time Score
    Timer* LevelTimer = this->GameApp->GetLevelTimer();
    CatchLevel* CurrentLevel = GameApp->GetLevelManager()->GetCurrentLevel();
    Whole Time = LevelTimer->GetCurrentTimeInMilliseconds() * 0.001;
    Whole LevelTargetTime = ( CurrentLevel != NULL ? CurrentLevel->GetLevelTargetTime() : 0 );
    if(Time < LevelTargetTime) {
        ThrowableContainer& Throwables = this->GameApp->GetThrowables();
        Whole ActorsInScoreZones = 0;

        AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
        AreaEffectManager::ConstAreaEffectIterator End = AEMan->EndAreaEffect();
        for( AreaEffectManager::ConstAreaEffectIterator ScoreIt = AEMan->BeginAreaEffect() ; ScoreIt != End ; ++ScoreIt )
        {
            if( (*ScoreIt)->GetDerivedSerializableName() == "ScoreArea" ) {
                //ScoreArea* CurrScore = static_cast<ScoreArea*>( *ScoreIt );
                AreaEffect::ObjectContainer& Overlapping = (*ScoreIt)->GetOverlappingObjects();
                for( AreaEffect::ObjectIterator ObjIt = Overlapping.begin() ; ObjIt != Overlapping.end() ; ++ObjIt )
                {
                    if( this->GameApp->IsAThrowable( *ObjIt ) )
                        ActorsInScoreZones++;
                }
            }
        }

        Real ThrowableRatio = ((Real)ActorsInScoreZones / (Real)Throwables.size());
        Real SubTotal = (((Real)LevelTargetTime - (Real)Time) * 10) * ThrowableRatio;
        Real Margin = MathTools::Fmod(SubTotal,5.0);
        if( Margin >= 2.5 ) {
            SubTotal += (5.0 - Margin);
            TimeScore = (Whole)(SubTotal + 0.5);
        }else if( Margin < 2.5 ) {
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
    const String ScoreBreakdownFont = "Ubuntu-18";
    const Real ScoreBreakdownText = 0.8;

    ScorePair ThrowableScores = this->CalculateThrowableScore();
    Whole NormalScore = ThrowableScores.first;
    Whole BonusScore = ThrowableScores.second;
    Whole ShopScore = this->CalculateItemCashScore();
    Whole TimerScore = this->CalculateTimerScore();

    // Update the UI to reflect the calculated scores
    UI::Screen* GameScreen = this->TheEntresol->GetUIManager()->GetScreen("GameScreen");
    UI::VerticalContainer* BreakdownList = static_cast<UI::VerticalContainer*>( GameScreen->GetWidget("GS_LevelReportBreakdown") );

    // Create the widget container for the normal score display
    UI::Widget* GSNormalArea = GameScreen->CreateWidget("GS_NormalArea");
    // Create the widget for displaying the normal score explanation text
    UI::Widget* GSNormalAreaLabel = GameScreen->CreateWidget("GS_NormalAreaLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSNormalAreaLabel->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    GSNormalAreaLabel->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSNormalAreaLabelLayer = GSNormalAreaLabel->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSNormalAreaLabelLayer->SetText("Normal Score: ");
    GSNormalAreaLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    GSNormalAreaLabelLayer->VerticallyAlign(UI::LA_Center);
    GSNormalAreaLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSNormalArea->AddChild(GSNormalAreaLabel,1);
    // Create the widget for displaying the normal score value
    UI::Widget* GSNormalAreaValue = GameScreen->CreateWidget("GS_NormalAreaValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSNormalAreaValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    GSNormalAreaValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSNormalAreaValueLayer = GSNormalAreaValue->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSNormalAreaValueLayer->SetText( StringTools::ConvertToString( NormalScore ) );
    GSNormalAreaValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    GSNormalAreaValueLayer->VerticallyAlign(UI::LA_Center);
    GSNormalAreaValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSNormalArea->AddChild(GSNormalAreaValue,2);
    // Wrap up timer configuration
    BreakdownList->AddChild(GSNormalArea,1);

    // Create the widget container for the normal score display
    UI::Widget* GSBonusArea = GameScreen->CreateWidget("GS_BonusArea");
    // Create the widget for displaying the normal score explanation text
    UI::Widget* GSBonusAreaLabel = GameScreen->CreateWidget("GS_BonusAreaLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSBonusAreaLabel->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    GSBonusAreaLabel->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSBonusAreaLabelLayer = GSBonusAreaLabel->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSBonusAreaLabelLayer->SetText("Bonus Score: ");
    GSBonusAreaLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    GSBonusAreaLabelLayer->VerticallyAlign(UI::LA_Center);
    GSBonusAreaLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSBonusArea->AddChild(GSBonusAreaLabel,1);
    // Create the widget for displaying the normal score value
    UI::Widget* GSBonusAreaValue = GameScreen->CreateWidget("GS_BonusAreaValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSBonusAreaValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    GSBonusAreaValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSBonusAreaValueLayer = GSBonusAreaValue->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSBonusAreaValueLayer->SetText( StringTools::ConvertToString( BonusScore ) );
    GSBonusAreaValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    GSBonusAreaValueLayer->VerticallyAlign(UI::LA_Center);
    GSBonusAreaValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSBonusArea->AddChild(GSBonusAreaValue,2);
    // Wrap up timer configuration
    BreakdownList->AddChild(GSBonusArea,2);

    // Create the widget container for the normal score display
    UI::Widget* GSShopArea = GameScreen->CreateWidget("GS_ShopArea");
    // Create the widget for displaying the normal score explanation text
    UI::Widget* GSShopAreaLabel = GameScreen->CreateWidget("GS_ShopAreaLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSShopAreaLabel->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    GSShopAreaLabel->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSShopAreaLabelLayer = GSShopAreaLabel->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSShopAreaLabelLayer->SetText("Shop Score: ");
    GSShopAreaLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    GSShopAreaLabelLayer->VerticallyAlign(UI::LA_Center);
    GSShopAreaLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSShopArea->AddChild(GSShopAreaLabel,1);
    // Create the widget for displaying the normal score value
    UI::Widget* GSShopAreaValue = GameScreen->CreateWidget("GS_ShopAreaValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSShopAreaValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    GSShopAreaValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSShopAreaValueLayer = GSShopAreaValue->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSShopAreaValueLayer->SetText( StringTools::ConvertToString( ShopScore ) );
    GSShopAreaValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    GSShopAreaValueLayer->VerticallyAlign(UI::LA_Center);
    GSShopAreaValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSShopArea->AddChild(GSShopAreaValue,2);
    // Wrap up timer configuration
    BreakdownList->AddChild(GSShopArea,3);

    // Create the widget container for the normal score display
    UI::Widget* GSTimerArea = GameScreen->CreateWidget("GS_TimerArea");
    // Create the widget for displaying the normal score explanation text
    UI::Widget* GSTimerAreaLabel = GameScreen->CreateWidget("GS_TimerAreaLabel",UI::UnifiedRect(0.0,0.0,0.55,1.0));
    GSTimerAreaLabel->SetHorizontalPositioningRules(UI::PF_Anchor_Left);
    GSTimerAreaLabel->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSTimerAreaLabelLayer = GSTimerAreaLabel->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSTimerAreaLabelLayer->SetText("Timer Score: ");
    GSTimerAreaLabelLayer->HorizontallyAlign(UI::LA_BottomRight);
    GSTimerAreaLabelLayer->VerticallyAlign(UI::LA_Center);
    GSTimerAreaLabelLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSTimerArea->AddChild(GSTimerAreaLabel,1);
    // Create the widget for displaying the normal score value
    UI::Widget* GSTimerAreaValue = GameScreen->CreateWidget("GS_TimerAreaValue",UI::UnifiedRect(0.0,0.0,0.45,1.0));
    GSTimerAreaValue->SetHorizontalPositioningRules(UI::PF_Anchor_Right);
    GSTimerAreaValue->SetVerticalPositioningRules(UI::PF_Anchor_Top);
    UI::SingleLineTextLayer* GSTimerAreaValueLayer = GSTimerAreaValue->CreateSingleLineTextLayer(ScoreBreakdownFont,0,0);
    GSTimerAreaValueLayer->SetText( StringTools::ConvertToString( TimerScore ) );
    GSTimerAreaValueLayer->HorizontallyAlign(UI::LA_TopLeft);
    GSTimerAreaValueLayer->VerticallyAlign(UI::LA_Center);
    GSTimerAreaValueLayer->SetAutoTextScale(UI::TextLayer::SM_ParentRelative,ScoreBreakdownText);
    GSTimerArea->AddChild(GSTimerAreaValue,2);
    // Wrap up timer configuration
    BreakdownList->AddChild(GSTimerArea,4);

    Whole ObjectiveScore = 0;
    /*for(  ;  ;  )
    {
        //
        // Loop through special conditions from lua
        //
    }//*/

    Whole TotalScore = NormalScore+BonusScore+ShopScore+TimerScore+ObjectiveScore;
    UI::Widget* ScoreDisplay = GameScreen->GetWidget("GS_LevelReportScore");
    static_cast<UI::SingleLineTextLayer*>( ScoreDisplay->GetRenderLayer(0,UI::RLT_SingleLineText) )->SetText( StringTools::ConvertToString( TotalScore ) );

    BreakdownList->UpdateChildDimensions();
    GameScreen->GetWidget("GS_LevelReport")->Show();

    return TotalScore;
}

Whole LevelScorer::GetNumScoreAreas() const
{
    Whole ScoreAreaCount = 0;
    AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
    AreaEffectManager::ConstAreaEffectIterator End = AEMan->EndAreaEffect();
    for( AreaEffectManager::ConstAreaEffectIterator ScoreIt = AEMan->BeginAreaEffect() ; ScoreIt != End ; ++ScoreIt )
    {
        if( (*ScoreIt)->GetDerivedSerializableName() == "ScoreArea" ) {
            ++ScoreAreaCount;
        }
    }
    return ScoreAreaCount;
}

Whole LevelScorer::GetNumAddedThrowables() const
{
    Whole ThrowableCount = 0;
    AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
    AreaEffectManager::ConstAreaEffectIterator End = AEMan->EndAreaEffect();
    for( AreaEffectManager::ConstAreaEffectIterator ScoreIt = AEMan->BeginAreaEffect() ; ScoreIt != End ; ++ScoreIt )
    {
        if( (*ScoreIt)->GetDerivedSerializableName() == "ScoreArea" ) {
            //ScoreArea* CurrScore = static_cast<ScoreArea*>( *ScoreIt );
            AreaEffect::ObjectContainer& Added = (*ScoreIt)->GetAddedObjects();
            for( AreaEffect::ConstObjectIterator ObjIt = Added.begin() ; ObjIt != Added.end() ; ++ObjIt )
            {
                if( this->GameApp->IsAThrowable( *ObjIt ) )
                    ++ThrowableCount;
            }
        }
    }
    return ThrowableCount;
}

Whole LevelScorer::GetNumOverlappingThrowables() const
{
    Whole ThrowableCount = 0;
    AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
    AreaEffectManager::ConstAreaEffectIterator End = AEMan->EndAreaEffect();
    for( AreaEffectManager::ConstAreaEffectIterator ScoreIt = AEMan->BeginAreaEffect() ; ScoreIt != End ; ++ScoreIt )
    {
        if( (*ScoreIt)->GetDerivedSerializableName() == "ScoreArea" ) {
            //ScoreArea* CurrScore = static_cast<ScoreArea*>( *ScoreIt );
            AreaEffect::ObjectContainer& Overlapping = (*ScoreIt)->GetOverlappingObjects();
            for( AreaEffect::ConstObjectIterator ObjIt = Overlapping.begin() ; ObjIt != Overlapping.end() ; ++ObjIt )
            {
                if( this->GameApp->IsAThrowable( *ObjIt ) )
                    ++ThrowableCount;
            }
        }
    }
    return ThrowableCount;
}

Whole LevelScorer::GetNumRemovedThrowables() const
{
    Whole ThrowableCount = 0;
    AreaEffectManager* AEMan = this->TheEntresol->GetAreaEffectManager();
    AreaEffectManager::ConstAreaEffectIterator End = AEMan->EndAreaEffect();
    for( AreaEffectManager::ConstAreaEffectIterator ScoreIt = AEMan->BeginAreaEffect() ; ScoreIt != End ; ++ScoreIt )
    {
        if( (*ScoreIt)->GetDerivedSerializableName() == "ScoreArea" ) {
            //ScoreArea* CurrScore = static_cast<ScoreArea*>( *ScoreIt );
            AreaEffect::ObjectContainer& Removed = (*ScoreIt)->GetRemovedObjects();
            for( AreaEffect::ConstObjectIterator ObjIt = Removed.begin() ; ObjIt != Removed.end() ; ++ObjIt )
            {
                if( this->GameApp->IsAThrowable( *ObjIt ) )
                    ++ThrowableCount;
            }
        }
    }
    return ThrowableCount;
}

///////////////////////////////////////////////////////////////////////////////
// Special Conditions

///////////////////////////////////////////////////////////////////////////////
// Configuration

void LevelScorer::SetThrowableScore(const String& TypeName, const Whole& Score)
{
    this->ItemScoreValues[TypeName] = Score;
    //ItemScoreValues.insert(std::pair<String,Whole>(TypeName,Score));
}

void LevelScorer::ResetLevelData()
{
    UI::UIManager* UIMan = this->TheEntresol->GetUIManager();
    UI::Screen* GameScreen = UIMan->GetScreen("GameScreen");
    UI::VerticalContainer* BreakdownList = static_cast<UI::VerticalContainer*>( GameScreen->GetWidget("GS_LevelReportBreakdown") );
    BreakdownList->DestroyAllChildren();
}

void LevelScorer::ResetAllData()
{
    this->ResetLevelData();
    this->ItemScoreValues.clear();
}

#endif
