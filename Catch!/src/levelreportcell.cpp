#ifndef _levelreportcell_cpp
#define _levelreportcell_cpp

#include "levelreportcell.h"

LevelReportCell::LevelReportCell(const String& name, const UI::RenderableRect& Rect, UI::Layer* parent)
    : UI::Cell(name,parent)
{
    const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
    UI::RenderableRect DescRect, ScoreRect;
    if(Rect.Relative)
    {
        RelPosition = Rect.Position;
        RelSize = Rect.Size;

        CalculateOffsets(Rect.Size * WinDim);

        DescRect.Position = Rect.Position;
        DescRect.Size = Vector2(Rect.Size.X * 0.49,Rect.Size.Y);
        DescRect.Relative = Rect.Relative;
        ScoreRect.Position = Rect.Position+(Vector2(ScoreOffset.X / WinDim.X,0));
        ScoreRect.Size = Vector2(Rect.Size.X * 0.49,Rect.Size.Y);
        ScoreRect.Relative = Rect.Relative;
    }else{
        RelPosition = Rect.Position / WinDim;
        RelSize = Rect.Size / WinDim;

        CalculateOffsets(Rect.Size);

        DescRect.Position = Rect.Position;
        DescRect.Size = Vector2(Rect.Size.X * 0.49,Rect.Size.Y);
        DescRect.Relative = Rect.Relative;
        ScoreRect.Position = Rect.Position+ScoreOffset;
        ScoreRect.Size = Vector2(Rect.Size.X * 0.49,Rect.Size.Y);
        ScoreRect.Relative = Rect.Relative;
    }

    Description = new UI::Caption(Name+"Desc",DescRect,Rect.Size.Y,"",ParentLayer);
    Score = new UI::Caption(Name+"Score",ScoreRect,Rect.Size.Y,"",ParentLayer);

    Description->HorizontallyAlign(Mezzanine::UI::Txt_Right);
    Score->HorizontallyAlign(Mezzanine::UI::Txt_Left);

    //Description->SetBackgroundColour(ColourValue::Transparent());
    //Score->SetBackgroundColour(ColourValue::Transparent());
    Description->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.2));
    Score->SetBackgroundColour(ColourValue(0.0,0.0,0.0,0.2));

    AddSubRenderable(0,RenderablePair(Description,NULL));
    AddSubRenderable(1,RenderablePair(Score,NULL));
}

LevelReportCell::~LevelReportCell()
{
    delete Description;
    delete Score;
}

void LevelReportCell::CalculateOffsets(const Vector2& Size)
{
    ScoreOffset = Vector2(Size.X * 0.51,0);
}

void LevelReportCell::UpdateImpl(bool Force)
{

}

void LevelReportCell::SetVisibleImpl(bool visible)
{
    Description->SetVisible(visible);
    Score->SetVisible(visible);
}

bool LevelReportCell::CheckMouseHoverImpl()
{
    if(Description->CheckMouseHover()) return true;
    else if(Score->CheckMouseHover()) return true;
    else return false;
}

void LevelReportCell::SetPosition(const Vector2& Position)
{
    const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
    RelPosition = Position;
    Description->SetPosition(Position);
    Score->SetPosition(Position+(Vector2(ScoreOffset.X / WinDim.X,0)));
}

void LevelReportCell::SetActualPosition(const Vector2& Position)
{
    const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
    RelPosition = Position / WinDim;
    Description->SetActualPosition(Position);
    Score->SetActualPosition(Position+ScoreOffset);
}

void LevelReportCell::SetSize(const Vector2& Size)
{
    RelSize = Size;
    Vector2 CapSize(Size.X * 0.49,Size.Y);
    Description->SetSize(CapSize);
    Score->SetSize(CapSize);
    CalculateOffsets(Size * ParentLayer->GetParent()->GetViewportDimensions());
    SetPosition(RelPosition);
}

void LevelReportCell::SetActualSize(const Vector2& Size)
{
    const Vector2& WinDim = ParentLayer->GetParent()->GetViewportDimensions();
    RelSize = Size / WinDim;
    Vector2 CapSize(Size.X * 0.49,Size.Y);
    Description->SetActualSize(CapSize);
    Score->SetActualSize(CapSize);
    CalculateOffsets(Size);
    SetPosition(RelPosition);
}

void LevelReportCell::UpdateDimensions()
{
    UI::WidgetResult Result = UI::ViewportUpdateTool::UpdateWidget(this);
    RelPosition = Result.first / UI::ViewportUpdateTool::GetNewSize();
    RelSize = Result.second / UI::ViewportUpdateTool::GetNewSize();
    Description->UpdateDimensions();
    Score->UpdateDimensions();
    CalculateOffsets(Result.second);
    SetPosition(RelPosition);
}

UI::Caption* LevelReportCell::GetDescription()
{
    return Description;
}

UI::Caption* LevelReportCell::GetScore()
{
    return Score;
}

#endif
