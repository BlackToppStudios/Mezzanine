@echo off
rem Check if bullet exists
if exist data\windows\bullet\libBullet.a goto EndBulletExistIf 
	cd libincludes\common\bulletsrc
	if exist "c:\Program Files (x86)\" goto BuildBullet64
		:BuildBullet32
		"c:\Program Files\CodeBlocks\codeblocks.exe" --build --target=Release BulletWorkspace.workspace
		goto EndBulletBuildIf
		:BuildBullet64
		"c:\Program Files (x86)\CodeBlocks\codeblocks.exe" --build --target=Release BulletWorkspace.workspace
		goto EndBulletBuildIf
	:EndBulletBuildIf
	copy libBullet.a ..\..\..\data\windows\bullet\
	cd ..\..\..\
:EndBulletExistIf

rem Check if ticpp exists
if exist data\windows\tinyxml\libTinyXMLpp.a goto EndTicppExistIf 
	cd libincludes\common\tinyxmlppsrc
 	if exist "c:\Program Files (x86)\" goto BuildTicpp64
 		:BuildTicpp32
 		"c:\Program Files\CodeBlocks\codeblocks.exe" --build --target=Release ticpp.workspace
 		goto EndTicppBuildIf
 		:BuildTicpp64
 		"c:\Program Files (x86)\CodeBlocks\codeblocks.exe" --build --target=Release ticpp.workspace
 		goto EndTicppBuildIf
 	:EndTicppBuildIf
em 	copy libticpp.a ..\..\..\data\windows\tinyxml\libTinyXMLpp.a
 	cd ..\..\..\
:EndTicppExistIf

cmd /c if not exist ".\%1\data\windows\ogre" mkdir .\%1\data\windows\ogre
cmd /c copy /y /v .\data\windows\ogre\OgreMain.dll .\%1
cmd /c copy /y /v .\data\windows\sdl\SDL.dll .\%1
cmd /c copy /y /v .\data\windows\plugins.cfg .\%1\data\windows
cmd /c copy /y /v .\data\windows\settings.cfg .\%1\data\windows
cmd /c copy /y /v .\data\windows\ogre\RenderSystem_GL.dll .\%1\data\windows\ogre
cmd /c copy /y /v .\data\windows\caudio\cAudio.dll .\%1
cmd /c copy /y /v .\data\windows\caudio\OpenAL32.dll .\%1
cmd /c copy /y /v .\data\windows\caudio\wrap_oal.dll .\%1
