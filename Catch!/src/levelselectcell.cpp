#ifndef _levelselectcell_cpp
#define _levelselectcell_cpp

#include "levelselectcell.h"

LevelSelectCell::LevelSelectCell(const String& name, const UI::RenderableRect& Rect, UI::Layer* parent)
    : UI::Cell(name,parent)
      //PreviewImageOffset(Vector2(-0.005,-0.005)),
      //PreviewBorderOffset(Vector2(-0.01,-0.01)),
      //LevelTitleOffset(Vector2(0.115,0.01)),
      //EarnedScoreOffset(Vector2(0.18,0.05))
{
    const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
    UI::RenderableRect PIRect, PBRect, LTRect, ESRect;
    if(Rect.Relative)
    {
        RelPosition = Rect.Position;
        RelSize = Rect.Size;

        CalculateOffsets(Rect.Size * WinDim);

        PIRect.Position = Rect.Position+(PreviewImageOffset / WinDim);
        PIRect.Size = Vector2(Rect.Size.Y * 1.30,Rect.Size.Y * 1.05);
        PIRect.Relative = Rect.Relative;
        PBRect.Position = Rect.Position+(PreviewBorderOffset / WinDim);
        PBRect.Size = Vector2(Rect.Size.Y * 1.40,Rect.Size.Y * 1.15);
        PBRect.Relative = Rect.Relative;
        LTRect.Position = Rect.Position+(LevelTitleOffset / WinDim);
        LTRect.Size = Vector2(Rect.Size.X * 0.6,Rect.Size.Y * 0.36);
        LTRect.Relative = Rect.Relative;
        ESRect.Position = Rect.Position+(EarnedScoreOffset / WinDim);
        ESRect.Size = Vector2(Rect.Size.X * 0.4,Rect.Size.Y * 0.36);
        ESRect.Relative = Rect.Relative;
    }else{
        RelPosition = Rect.Position / WinDim;
        RelSize = Rect.Size / WinDim;

        CalculateOffsets(Rect.Size);

        PIRect.Position = Rect.Position+PreviewImageOffset;
        PIRect.Size = Vector2(Rect.Size.Y * 1.30,Rect.Size.Y * 1.05);
        PIRect.Relative = Rect.Relative;
        PBRect.Position = Rect.Position+PreviewBorderOffset;
        PBRect.Size = Vector2(Rect.Size.Y * 1.40,Rect.Size.Y * 1.15);
        PBRect.Relative = Rect.Relative;
        LTRect.Position = Rect.Position+LevelTitleOffset;
        LTRect.Size = Vector2(Rect.Size.X * 0.6,Rect.Size.Y * 0.36);
        LTRect.Relative = Rect.Relative;
        ESRect.Position = Rect.Position+EarnedScoreOffset;
        ESRect.Size = Vector2(Rect.Size.X * 0.4,Rect.Size.Y * 0.36);
        ESRect.Relative = Rect.Relative;
    }

    CellBack = new UI::Rectangle(Rect,ParentLayer);
    PreviewImage = new UI::Rectangle(PIRect,ParentLayer);
    PreviewBorder = new UI::Rectangle(PBRect,ParentLayer);
    LevelTitle = new UI::Caption(Name+"LT",LTRect,Real(Rect.Size.Y * 0.36),Name,ParentLayer);
    EarnedScore = new UI::Caption(Name+"ES",ESRect,Real(Rect.Size.Y * 0.36),"0/0",ParentLayer);
    AddSubRenderable(0,RenderablePair(CellBack,NULL));
    AddSubRenderable(1,RenderablePair(PreviewImage,NULL));
    AddSubRenderable(2,RenderablePair(PreviewBorder,NULL));
    AddSubRenderable(3,RenderablePair(LevelTitle,NULL));
    AddSubRenderable(4,RenderablePair(EarnedScore,NULL));
}

LevelSelectCell::~LevelSelectCell()
{
    delete CellBack;
    delete PreviewImage;
    delete PreviewBorder;
    delete LevelTitle;
    delete EarnedScore;
}

void LevelSelectCell::UpdateImpl(bool Force)
{

}

void LevelSelectCell::SetVisibleImpl(bool visible)
{
    CellBack->SetVisible(visible);
    PreviewImage->SetVisible(visible);
    PreviewBorder->SetVisible(visible);
    LevelTitle->SetVisible(visible);
    EarnedScore->SetVisible(visible);
}

bool LevelSelectCell::CheckMouseHoverImpl()
{
    if(CellBack->CheckMouseHover()) return true;
    else if(PreviewImage->CheckMouseHover()) return true;
    else if(PreviewBorder->CheckMouseHover()) return true;
    else return false;
}

void LevelSelectCell::CalculateOffsets(const Vector2& Size)
{
    PreviewImageOffset = Vector2(-(Size.Y*0.045),-(Size.Y*0.045));
    PreviewBorderOffset = Vector2(-(Size.Y*0.09),-(Size.Y*0.09));
    LevelTitleOffset = Vector2(Size.X*0.38,Size.Y*0.09);
    EarnedScoreOffset = Vector2(Size.X*0.60,Size.Y*0.51);
}

void LevelSelectCell::SetPosition(const Vector2& Position)
{
    SetActualPosition(Position * ParentLayer->GetParent()->GetViewportDimensions());
}

void LevelSelectCell::SetActualPosition(const Vector2& Position)
{
    RelPosition = Position / ParentLayer->GetParent()->GetViewportDimensions();
    CellBack->SetActualPosition(Position);
    PreviewImage->SetActualPosition(Position + PreviewImageOffset);
    PreviewBorder->SetActualPosition(Position + PreviewBorderOffset);
    LevelTitle->SetActualPosition(Position + LevelTitleOffset);
    EarnedScore->SetActualPosition(Position + EarnedScoreOffset);
}

void LevelSelectCell::SetSize(const Vector2& Size)
{
    RelSize = Size;
    CellBack->SetSize(Size);
    PreviewImage->SetSize(Vector2(Size.Y * 1.30,Size.Y * 1.05));
    PreviewBorder->SetSize(Vector2(Size.Y * 1.40,Size.Y * 1.15));
    LevelTitle->SetSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedScore->SetSize(Vector2(Size.X * 0.4,Size.Y * 0.36));
    //CalculateOffsets(Size * ParentLayer->GetParent()->GetViewportDimensions());
    SetPosition(RelPosition);
}

void LevelSelectCell::SetActualSize(const Vector2& Size)
{
    const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
    RelSize = Size / WinDim;
    CellBack->SetActualSize(Size);
    PreviewImage->SetActualSize(Vector2(Size.Y * 1.30,Size.Y * 1.05));
    PreviewBorder->SetActualSize(Vector2(Size.Y * 1.40,Size.Y * 1.15));
    LevelTitle->SetActualSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedScore->SetActualSize(Vector2(Size.X * 0.4,Size.Y * 0.36));
    //CalculateOffsets(Size);
    SetPosition(RelPosition);
}

void LevelSelectCell::UpdateDimensions()
{
    UI::WidgetResult Result = UI::ViewportUpdateTool::UpdateWidget(this);
    RelPosition = Result.first / UI::ViewportUpdateTool::GetNewSize();
    RelSize = Result.second / UI::ViewportUpdateTool::GetNewSize();
    CellBack->UpdateDimensions();
    PreviewImage->UpdateDimensions();
    PreviewBorder->UpdateDimensions();
    LevelTitle->UpdateDimensions();
    EarnedScore->UpdateDimensions();
    CalculateOffsets(Result.second);
    SetPosition(RelPosition);
}

String LevelSelectCell::GetLevelName()
{
    return LevelTitle->GetText();
}

UI::Rectangle* LevelSelectCell::GetCellBack()
{
    return CellBack;
}

UI::Rectangle* LevelSelectCell::GetPreviewImage()
{
    return PreviewImage;
}

UI::Rectangle* LevelSelectCell::GetPreviewBorder()
{
    return PreviewBorder;
}

UI::Caption* LevelSelectCell::GetLevelTitle()
{
    return LevelTitle;
}

UI::Caption* LevelSelectCell::GetEarnedScore()
{
    return EarnedScore;
}

LevelSelectCB::LevelSelectCB()
    : LevelCell(NULL)
{
}

LevelSelectCB::~LevelSelectCB()
{
}

void LevelSelectCB::SetCaller(UI::Cell* Caller)
{
    this->Caller = Caller;
    this->LevelCell = dynamic_cast<LevelSelectCell*>(Caller);
}

void LevelSelectCB::DoSelectedItems()
{
    LevelCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBackSelected");
}

void LevelSelectCB::DoUnselectedItems()
{
    LevelCell->GetCellBack()->SetBackgroundSprite("MMLevelCellBack");
}

#endif
