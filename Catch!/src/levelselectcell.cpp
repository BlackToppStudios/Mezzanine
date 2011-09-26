#ifndef _levelselectcell_cpp
#define _levelselectcell_cpp

#include "levelselectcell.h"

LevelSelectCell::LevelSelectCell(const String& name, const UI::RenderableRect& Rect, UI::Layer* parent)
    : UI::Cell(name,parent),
      PreviewImageOffset(Vector2(-0.005,-0.005)),
      PreviewBorderOffset(Vector2(-0.01,-0.01)),
      LevelTitleOffset(Vector2(0.115,0.01)),
      EarnedMaxScoreOffset(Vector2(0.18,0.05))
{
    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
    std::pair<Whole,Real> Result;
    if(Rect.Relative)
    {
        RelPosition = Rect.Position;
        RelSize = Rect.Size;
        Result = World::GetWorldPointer()->GetUIManager()->SuggestGlyphIndex((Rect.Size.Y * 0.36) * WinDim.Y,Parent->GetParent()->GetPrimaryAtlas());

        CellBack = new UI::Rectangle(Rect,Parent);
        PreviewImage = new UI::Rectangle(UI::RenderableRect(Rect.Position+PreviewImageOffset,Vector2(Rect.Size.Y * 1.05,Rect.Size.Y * 1.05),true),Parent);
        PreviewBorder = new UI::Rectangle(UI::RenderableRect(Rect.Position+PreviewBorderOffset,Vector2(Rect.Size.Y * 1.15,Rect.Size.Y * 1.15),true),Parent);
        LevelTitle = new UI::Caption(Name+"LT",UI::RenderableRect(Rect.Position+LevelTitleOffset,Vector2(Rect.Size.X * 0.6,Rect.Size.Y * 0.36),true),Result.first,Name,Parent);
        EarnedMaxScore = new UI::Caption(Name+"EMS",UI::RenderableRect(Rect.Position+EarnedMaxScoreOffset,Vector2(Rect.Size.X * 0.4,Rect.Size.Y * 0.36),true),Result.first,"0/0",Parent);
    }else{
        RelPosition = Rect.Position / WinDim;
        RelSize = Rect.Size / WinDim;
        Result = World::GetWorldPointer()->GetUIManager()->SuggestGlyphIndex(Rect.Size.Y * 0.36,Parent->GetParent()->GetPrimaryAtlas());
    }

    if(1.f != Result.second)
    {
        //LevelTitle->SetTextScale(Result.second);
        //EarnedMaxScore->SetTextScale(Result.second);
    }
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

void LevelSelectCell::SetPosition(const Vector2& Position)
{
    RelPosition = Position;
    CellBack->SetPosition(Position);
    PreviewImage->SetPosition(Position + PreviewImageOffset);
    PreviewBorder->SetPosition(Position + PreviewBorderOffset);
    LevelTitle->SetPosition(Position + LevelTitleOffset);
    EarnedMaxScore->SetPosition(Position + EarnedMaxScoreOffset);
}

void LevelSelectCell::SetActualPosition(const Vector2& Position)
{
    SetPosition(Position / Parent->GetParent()->GetViewportDimensions());
}

void LevelSelectCell::SetSize(const Vector2& Size)
{
    RelSize = Size;
    CellBack->SetSize(Size);
    PreviewImage->SetSize(Vector2(Size.Y * 1.05,Size.Y * 1.05));
    PreviewBorder->SetSize(Vector2(Size.Y * 1.15,Size.Y * 1.15));
    LevelTitle->SetSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedMaxScore->SetSize(Vector2(Size.X * 0.4,Size.Y * 0.36));

    SetPosition(RelPosition);
}

void LevelSelectCell::SetActualSize(const Vector2& Size)
{
    const Vector2& WinDim = Parent->GetParent()->GetViewportDimensions();
    RelSize = Size / WinDim;
    CellBack->SetActualSize(Size);
    PreviewImage->SetActualSize(Vector2(Size.Y * 1.05,Size.Y * 1.05));
    PreviewBorder->SetActualSize(Vector2(Size.Y * 1.15,Size.Y * 1.15));
    LevelTitle->SetActualSize(Vector2(Size.X * 0.6,Size.Y * 0.36));
    EarnedMaxScore->SetActualSize(Vector2(Size.X * 0.4,Size.Y * 0.36));
    //SetSize(Size / WinDim);
    SetPosition(RelPosition);
}

void LevelSelectCell::DoSelectedItems()
{

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
