#ifndef _levelloader_h
#define _levelloader_h

#include <mezzanine.h>

using namespace Mezzanine;

class CatchApp;

class LevelLoader
{
    protected:
        World* TheWorld;
        CatchApp* GameApp;
        String LevelToLoad;
        String CurrentLevel;

        /// @todo Once levels are finalized, their create functions need to be removed.
        void LoadFerris();
        void LoadBigCurve();
        void LoadBlowsNotSucks();
        void LoadJustice();
        void LoadRollers();
        void LoadJustBounce();
    public:
        LevelLoader();
        ~LevelLoader();
        bool HasALevelToLoad();
        void SetNextLevel(const String& LevelName);
        String GetNextLevel();
        String GetCurrentLevel();
        void LoadLevel();
};

#endif
