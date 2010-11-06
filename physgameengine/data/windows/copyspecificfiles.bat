cmd /c if not exist ".\%1\data\windows\ogre" mkdir .\%1\data\windows\ogre
cmd /c copy /y /v .\data\windows\ogre\OgreMain.dll .\%1
cmd /c copy /y /v .\data\windows\sdl\SDL.dll .\%1
cmd /c copy /y /v .\data\windows\plugins.cfg .\%1\data\windows
cmd /c copy /y /v .\data\windows\settings.cfg .\%1\data\windows
cmd /c copy /y /v .\data\windows\ogre\RenderSystem_GL.dll .\%1\data\windows\ogre
cmd /c copy /y /v .\data\windows\caudio\cAudio.dll .\%1
cmd /c copy /y /v .\data\windows\caudio\OpenAL32.dll .\%1
cmd /c copy /y /v .\data\windows\caudio\wrap_oal.dll .\%1
