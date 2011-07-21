#ifndef _levelselectcell_h
#define _levelselectcell_h

#include <physgame.h>

using namespace phys;

class LevelSelectCell : public UI::Cell
{
    protected:
        UI::Rectangle* CellBack;
        UI::Rectangle* PreviewImage;
        UI::Rectangle* PreviewBorder;
        UI::Caption* LevelTitle;
        UI::Caption* EarnedMaxScore;
        Vector2 PreviewImageOffset;
        Vector2 PreviewBorderOffset;
        Vector2 LevelTitleOffset;
        Vector2 EarnedMaxScoreOffset;
        /// @brief For use with widget update/automation.
        virtual void Update(bool Force = false);
    public:
        LevelSelectCell(const String& name, const UI::RenderableRect& Rect, UI::Layer* parent);
        virtual ~LevelSelectCell();
        virtual void SetVisible(bool visible);
        virtual bool IsVisible();
        virtual void Show();
        virtual void Hide();
        virtual bool CheckMouseHover();
        virtual void SetPosition(const Vector2& Position);
        virtual Vector2 GetPosition();
        virtual void SetActualPosition(const Vector2& Position);
        virtual Vector2 GetActualPosition();
        virtual void SetSize(const Vector2& Size);
        virtual Vector2 GetSize();
        virtual void SetActualSize(const Vector2& Size);
        virtual Vector2 GetActualSize();
        virtual void DoSelectedItems();
        virtual String GetLevelName();
        virtual UI::Rectangle* GetCellBack();
        virtual UI::Rectangle* GetPreviewImage();
        virtual UI::Rectangle* GetPreviewBorder();
        virtual UI::Caption* GetLevelTitle();
        virtual UI::Caption* GetEarnedMaxScore();
};

class LevelSelectCB : public UI::CellCallback
{
    protected:
        LevelSelectCell* LevelCell;
    public:
        LevelSelectCB(LevelSelectCell* CallerCell);
        ~LevelSelectCB();
        virtual void DoSelectedItems();
        virtual void DoUnselectedItems();
};

#endif
