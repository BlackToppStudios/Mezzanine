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
#ifndef cxx_training_h
#define cxx_training_h

/**
    @page cxx_training_and_tools C++ Training and Tools
    For many of the people on this team, these are tools and tutorials we learned with and use. We
    keep some links and information here to make it easy for people to jump into our team. Please
    don’t think that because you use different tools we don’t want your help, this is what we have
    used and might recommend, not what everyone should use for everything. If you have some info,
    particularly information about any tools or techniques not well covered we would love to include
    it.

    @section cxx_training_and_tools_tools Coding Tools
    Since we have an open policy towards helping non-developers get started, we
    want to make it easy to get started as a developer and learn about our tools about our tools.

    @subsection cxx_training_and_tools_compilers Compilers and Development Environments
    We primarily use G++/MinGW for a compiler, Code::blocks for our Development environment, and GDB
    as our debugger. Code::blocks makes it all pretty seamless. There are a few different platforms
    that we do not have comprehensive steps for working on because we have not ported Catch! and the
    Mezzanine Engine to them yet. Please make appropriate revisions here once you have the steps to
    do something new.

    - @ref dev_tools_windows — Complete for XP/Vista - Win7/10work but need details
    - @ref dev_tools_linux — Complete and easy on Ubuntu, Fedora and others need details
    - @ref dev_tools_mac — In Progress

    @n
    Eventually we will need Dev tool pages for Mac OS, Iphone, Android, Several Consoles, Other
    Mobile.

    @subsection cxx_training_and_tools_git Git - Source Revision Control
    Git is our source control system. This is how we keep everyone up to up to date. See above for
    instructions on getting Git for your workstation. To grab a copy of the code run the following
    command from at a command line, or “git bash” terminal. If you are using a graphical git client
    just use the URL portion of the command:

    @verbatim
    git clone git://github.com/BlackToppStudios/Mezzanine.git
    @endverbatim

    @n
    For more details on Git usage see some third party tutorials:

    - @htmlonly<a href="https://www.atlassian.com/git/tutorials/">@endhtmlonly
    Atlassian's Git tutorials
    @htmlonly</a>@endhtmlonly
    - @htmlonly<a href="http://git.or.cz/course/svn.html">@endhtmlonly
    Git Comparison to SVN
    @htmlonly</a>@endhtmlonly
    - @htmlonly<a href="http://linux.yyz.us/git-howto.html">@endhtmlonly
    Hacker’s guide to git
    @htmlonly</a>@endhtmlonly

    @section cxx_training_and_training Training
    Since this whole project is being built in standard C++ (or certain external libraries), we
    thought it would be prudent to include some resources for learning it:

    @subsection cxx_training_and_tutorials Standard C++ Tutorials
    If you have never used C++ and you need to somewhere to get started tutorials might be ideal:
    - @htmlonly<a href="http://www.cplusplus.com/doc/tutorial/introduction/">@endhtmlonly
    Cplusplus.com beginners Tutorial
    @htmlonly</a>@endhtmlonly
     — To help newer developers whet their appetite C++ Development.

    - @htmlonly<a href="https://www.google.com/search?q=c%2B%2B+tutorial">@endhtmlonly
    Google “C++ tutorial”
    @htmlonly</a>@endhtmlonly
    for some very useful information.

    @subsection cxx_training_and_beyond_tutorials Beyond Tutorials
    To expand on basic knowledge or round
    - @htmlonly<a href="https://github.com/isocpp/CppCoreGuidelines">@endhtmlonly
    C++ Core Guidelines
    @htmlonly</a>@endhtmlonly

    - @htmlonly
    <a href="http://www.justsoftwaresolutions.co.uk/threading/multithreading-in-c++0x-part-1-starting-threads.html">
    @endhtmlonly
    Just Software Threading tutorials
    @htmlonly</a>@endhtmlonly
     on the std::thread library.

    - @htmlonly<a href="http://www.learncpp.com/cpp-tutorial/a3-using-libraries-with-codeblocks/">
    @endhtmlonly
    How to link a library
    @htmlonly</a>@endhtmlonly

    - @htmlonly<a href="http://duramecho.com/ComputerInformation/WhyHowCppConst.html">@endhtmlonly
    Using Const How and Why
    @htmlonly</a>@endhtmlonly

    - Angelika Langer has a fantastic article about
    @htmlonly<a href="http://www.angelikalanger.com/IOStreams/Excerpt/excerpt.htm#Top">@endhtmlonly
    c++ streams and streambufs
    @htmlonly</a>@endhtmlonly
    . It explains how to connect a stream to any serializable data source.

    @subsection cxx_training_and_gaming Gaming Sources

    Make John add links to things like game dev subreddit and bullet/ogre forums.

    @todo John Add LINKS HERE!!!!!


*/


#endif
