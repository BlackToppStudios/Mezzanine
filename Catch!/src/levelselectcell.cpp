#ifndef _levelselectcell_cpp
#define _levelselectcell_cpp

#include "levelselectcell.h"

LevelSelectCell::LevelSelectCell(const String& name, const UI::RenderableRect& Rect, UI::Layer* parent)
    : UI::Cell(name,parent)
      //PreviewImageOffset(Vector2(-0.005,-0.005)),
      //PreviewBorderOffset(Vector2(-0.01,-0.01)),
      //LevelTitleOffset(Vector2(0.115,0.01)),
      //EarnedMaxScoreOffset(Vector2(0.18,0.05))
{
    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
    UI::RenderableRect PIRect, PBRect, LTRect, EMSRect;
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
        EMSRect.Position = Rect.Position+(EarnedMaxScoreOffset / WinDim);
        EMSRect.Size = Vector2(Rect.Size.X * 0.4,Rect.Size.Y * 0.36);
        EMSRect.Relative = Rect.Relative;
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
        EMSRect.Position = Rect.Position+EarnedMaxScoreOffset;
        EMSRect.Size = Vector2(Rect.Size.X * 0.4,Rect.Size.Y * 0.36);
        EMSRect.Relative = Rect.Relative;
    }

    CellBack = new UI::Rectangle(Rect,Parent);
    PreviewImage = new UI::Rectangle(PIRect,Parent);
    PreviewBorder = new UI::Rectangle(PBRect,Parent);
    LevelTitle = new UI::Caption(Name+"LT",LTRect,Real(Rect.Size.Y * 0.36),Name,Parent);
    EarnedMaxScore = new UI::Caption(Name+"EMS",EMSRect,Real(Rect.Size.Y * 0.36),"0/0",Parent);
}

LevelSelectCell::~LevelSelectCell()
{
    delete CellBack;
    delete PreviewImage;
    delete PreviewBorder;
    delete LevelTitle;
    delete EarnedMaxScore;
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
    EarnedMaxScore->SetVisible(visible);
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
    EarnedMaxScoreOffset = Vector2(Size.X*0.60,Size.Y*0.51);
}

void LevelSelectCell::SetPosition(const Vector2& Position)
{
    SetActualPosition(Position * Parent->GetParent()->GetViewportDimensions());
}

void LevelSelectCell::SetActualPosition(const Vector2& Position)
{
    RelPosition = Position / Parent->GetParent()->GetViewportDimensions();
    CellBack->SetActualPosition(Position);
    PreviewImage->SetActualPosition(Position + PreviewImageOffset);
    PreviewBorder->SetActualPosition(Position + PreviewBorderOffset);
    LevelTitle->SetActualPosition(Position + LevelTitleOffset);
    EarnedMaxScore->SetActualPosition(Position + EarnedMaxScoreOffset);
}

void LevelSelectCell::SetSize(const Vector2& Size)
{
    RelSize = Size;
    CellBack->SetSize(Size);
    PreviewImage->SetSize(Vector2(Size.Y * 1.30,Size.Y * 1.05));
    PreviewBorder->SetSize(Vector2(Size.Y * 1.40,Size.Y * 1.15));
    LevelTitle->SetSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedMaxScore->SetSize(Vector2(Size.X * 0.4,Size.Y * 0.36));

    SetPosition(RelPosition);
}

void LevelSelectCell::SetActualSize(const Vector2& Size)
{
    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
    RelSize = Size / WinDim;
    CellBack->SetActualSize(Size);
    PreviewImage->SetActualSize(Vector2(Size.Y * 1.30,Size.Y * 1.05));
    PreviewBorder->SetActualSize(Vector2(Size.Y * 1.40,Size.Y * 1.15));
    LevelTitle->SetActualSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedMaxScore->SetActualSize(Vector2(Size.X * 0.4,Size.Y * 0.36));
    SetPosition(RelPosition);
}

void LevelSelectCell::DoSelectedItems()
{

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
    EarnedMaxScore->UpdateDimensions();
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

UI::Caption* LevelSelectCell::GetEarnedMaxScore()
{
    return EarnedMaxScore;
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
