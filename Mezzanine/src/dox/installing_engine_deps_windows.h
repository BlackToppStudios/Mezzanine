// © Copyright 2010 - 2016 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

#ifndef installing_engine_deps_windows_h
#define installing_engine_deps_windows_h

/**
    @page installing_engine_deps_windows Installing Windows Engine Dependencies
    @section installing_engine_deps_windows_mingw MinGW32
    The following steps for additional library configuration with the Mezzanine only apply when
    using MinGW32.

    If you want to use OpenGL and WinMM as the video and audio backends for your application there
    are no additional steps you need to follow here. However if you want to use DirectX and/or
    DirectSound then you will need to install the DirectX SDK from
    @htmlonly<a href="http://www.microsoft.com/en-us/download/details.aspx?id=6812">@endhtmlonly
    Microsoft download page
    @htmlonly</a>@endhtmlonly
    . The Feburary 2010 version has been tested and is known to be bad/faulty with our
    configuration, so ensure you download the June 2010 or newer version.

    @n
    After installing the DirectX SDK you must add it’s install location to the system “Path”
    variable. During installation it will prompt you for where it will be installed, make note of
    that location when it does. In the event that you are reading this and already installed it, the
    typical locations for the DirectX SDK install are:

    - C:\\Program Files\\Microsoft DirectX SDK (June 2010)
    - C:\\Program Files(x86)\\Microsoft DirectX SDK (June 2010)

    Check those directories first and verify it’s location. Once you do copy that path to the system
    “Path” variable with the following the instruction on the @ref path_on_windows page.

    @n
    That is all the only step needed to get DirectX working. This also applies for DirectSound if
    you are using MSVC/Visual Studio. DirectSound on MinGW however, needs a little bit more help.
    You need to copy a header file available in a Microsoft Visual Studio install, and make it
    available to MinGW. So download and install the latest version of Microsoft Visual Studio
    express. The version I used and tested was Visual Studio 9.0. Once installed navigate to it’s
    install folder. For me this was in “C:\\Program Files\\Microsoft Visual Studio 9.0” (you may or
    may not need to append “x86” to the “Program Files” folder name). Once there you need to get to
    it’s include directory, to do that enter the “VC” folder, then the “include” folder. In there
    you should find a header file named “sal.h”. If you don’t, you may not have the proper version
    of Visual Studio installed. Once you find it you must copy it to your MinGW include folder
    which by default is located at “C:\\MinGW\\include”. This should enable the DirectSound backend
    for the audio subsystem with MinGW on Windows. You can check the output when you configure CMake
    to verify.

    @section installing_engine_deps_windows_mingw64 MinGW64
    The following steps for additional library configuration with the Mezzanine only apply when
    using MinGW64.

    Unlike MinGW32, MinGW64 package linked on the developers tools page comes with it’s own
    versions of all the necessary Windows and DirectX files that are used. The only real thing you
    need to do is point the libraries looking for DirectX in the right direction.To ensure that
    the correct libraries are paired with the correct headers and general compatibility, it is
    recommended that any DirectX SDK install you may have on your system be uninstalled. Once
    uninstalled you have to create an environment variable and set it to the necessary compiler
    directory.

    @subsection installing_engine_deps_windows_mingw64_xp Window XP
    1. Right-click “My Computer” or “Computer”, click “properties”.
    2. Click “Advanced” Tab.
    3. Click “Environment Variables” button.
    4. Click the “New…” button.
    5. In the “Variable name” field enter “DXSDK_DIR”.
    6. In the “Variable value” field enter “C:\\MinGW\\x86_64-w64-mingw32”.
    7. Click “OK” 4 times.
    @subsection installing_engine_deps_windows_mingw64_7 Vista and Windows 7.
    1. Right-click “My Computer” or “Computer”, click “properties”.
    2. Click “Advanced System Settings” Link.
    3. If a UAC Prompt requests you permission to continue click “Continue”.
    4. Click “Environment Variables” Button
    5. Click the “New…” button
    6. In the “Variable name” field enter “DXSDK_DIR”
    7. In the “Variable value” field enter “C:\\MinGW\\x86_64-w64-mingw32”
    8. Click “OK” 4 times

    @n
    Libraries using DirectX components should be able to detect and use the DirectX SDK provided by
    the compiler at this point. You may need to close and restart applications for this to take
    effect (such as CMake if you are using the GUI).
*/

#endif
