//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _resourcemanager_h
#define _resourcemanager_h

#include "crossplatformexport.h"
#include "datatypes.h"
#include "enumerations.h"
#include "managerbase.h"
#include "managerfactory.h"
#include "singleton.h"
#include "Resource/datastream.h"
#include "Resource/inputstream.h"

/// @file
/// @brief The defintion of the Resource Manager.

namespace Ogre
{
    class ResourceGroupManager;
}

namespace Mezzanine
{
    class ResourceGroup;

    // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
    #ifdef SWIG
    %template(SingletonResourceManager) Singleton<ResourceManager>;
    #endif

    ///////////////////////////////////////////////////////////////////////////////
    /// @class ResourceManager
    /// @headerfile resourcemanager.h
    /// @brief This is the manager responsible for the loading and unloading of files.
    /// @details This class is responsible for the reading and writing of files of all kinds, be
    /// it graphical meshes, physics data, or XMl files.
    ///////////////////////////////////////
    class MEZZ_LIB ResourceManager : public ManagerBase, public Singleton<ResourceManager>
    {
    public:
        /// @brief Basic container type for @ref DataStream storage by this class.
        typedef std::vector<Resource::DataStreamPtr>       DataStreamContainer;
        /// @brief Iterator type for @ref DataStream instances stored by this class.
        typedef DataStreamContainer::iterator              DataStreamIterator;
        /// @brief Const Iterator type for @ref DataStream instances stored by this class.
        typedef DataStreamContainer::const_iterator        ConstDataStreamIterator;
        /// @brief Basic container type for named @ref DataStream storage by this class.
        typedef std::map<String,Resource::DataStreamPtr>   NamedDataStreamContainer;
        /// @brief Iterator type for named @ref DataStream instances stored by this class.
        typedef NamedDataStreamContainer::iterator         NamedDataStreamIterator;
        /// @brief Const Iterator type for named @ref DataStream instances stored by this class.
        typedef NamedDataStreamContainer::const_iterator   ConstNamedDataStreamIterator;
    protected:
        /// @internal
        /// @brief Encapsulates the functionality of the ogre resource group manager.
        Ogre::ResourceGroupManager* OgreResource;
        /// @internal
        /// @brief The location of engine data.
        String EngineDataDir;

        /// @internal
        /// @brief A container storing all un-named, un-grouped data streams known by the resource system.
        DataStreamContainer DataStreams;
        /// @internal
        /// @brief A container storing all named but un-grouped data streams known by the resource system.
        NamedDataStreamContainer NamedDataStreams;

        /// @internal
        /// @brief A vector of Pointers to streams created to delete periodically.
        std::vector<ResourceInputStream*> DeleteList;
        /// @internal
        /// @brief A vector of all the known internal Asset Groups.
        std::vector<String> ResourceGroups;

        /// @brief ArgC as it was passed into Main.
        /// @details This cannot be set statically, it must wait for main(int, char**) to
        /// be initialized, then call the appropriate function to set this.
        int ArgC;
        /// @brief ArgC as it was passed into Main.
        /// @details This cannot be set statically, it must wait for main(int, char**) to
        /// be initialized, then call the appropriate function to set this.
        char** ArgV;

        /// @internal
        /// @brief Adds an asset group name to the list of known AssetGroups.
        void AddAssetGroupName(String Name);
    public:
        /// @brief Class constructor.
        /// @details Standard manager constructor.
        /// @param EngineDataPath The directory for engine specific data.
        /// @param ArchiveType The name of the type of archive at this path.
        /// @param ArgCount How many arguments will be passed in ArgVars. Defaults to 0
        /// @param ArgVars A pointer to an array, with ArgCount elements, of char* which point to null terminated c strings. Defaults to NULL.
        ResourceManager(const String& EngineDataPath = ".", const ArchiveType ArchType = AT_FileSystem, int ArgCount=0, char** ArgVars=NULL);
        /// @brief XML constructor.
        /// @param XMLNode The node of the xml document to construct from.
        ResourceManager(XML::Node& XMLNode);
        /// @details Class Destructor.
        virtual ~ResourceManager();

        /// @brief Store the Main arguments for later use.
        /// @param ArgCount How many arguments will be passed in ArgVars.
        /// @param ArgVars A pointer to an array, with ArgCount elements, of char* which point to null terminated c strings.
        void SetMainArgs(int ArgCount, char** ArgVars);

        ///////////////////////////////////////////////////////////////////////////////
        // Directory/Path Management

        /// @brief Creates a single new directory.
        /// @remarks This function will only create the directory specified at the end of the path.
        /// @param DirectoryPath The path for the newly created directory.
        /// @return Returns true if the directory was created, false in the case of a non-critical error.
        static bool CreateDirectory(const String& DirectoryPath);
        /// @brief Checks to see if the given path exists and if it is a folder.
        /// @param DirectoryPath A String containing the path to test.
        /// @return True if the item indicated by DirectoryPath exists and it is a directory, false if it does not exist or exists but is a file.
        /// @throws On Error this might throw a Mezzanine::IOException with detail about why it failed
        static bool DoesDirectoryExist(const String& DirectoryPath);
        /// @brief Remove an empty directory.
        /// @param DirectoryPath Directory to remove.
        /// @throws On Error this might throw a Mezzanine::IOException with details about why it failed.
        static void RemoveDirectory(const String& DirectoryPath);

        /// @brief Get the directory portion of a string
        /// @param FileName A whole path and filename
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "/a/b/" or "c:\windirs\"
        static String DirName(const String& FileName);
        /// @brief Get the filename portion of a string
        /// @param FileName A whole path and filename
        /// @return If passed "/a/b/c.txt" or "c:\windirs\crash.exe" this will return "c.txt" or "crash.exe"
        static String BaseName(const String& FileName);

        /// @brief Get the character used to separate directories
        /// @return Backslash '\' on windows and Forward slash '/' on other operating systems.
        static char GetDirectorySeparator();
        /// @brief Get the character used to separate entries in the system PATH
        /// @return Semicolon ';' on windows and Forward slash ':' on other operating systems.
        static char GetPathSeparator();

        /// @brief Get the $PATH or %PATH% split and order for easy checking of how the OS does it.
        /// @param PATH Defaults to the PATH environment variable. But any value like a system path will be split the return of GetPathSeparator().
        /// @return A collection of directories that this system will for executables in the order they will be checked.
        static StringVector GetSystemPATH(const String& PATH = String(getenv("PATH")));
        /// @brief Search the system path the same way most systems do to find an executable.
        /// @param ExecutableName The executable to look for.
        /// @return If the executable is not found "" is returned otherwise the first directory in the PATH containing it is returned.
        /// @warning This function is case sensitive and not all operating systems are.
        /// @todo Add support for extension handling on windows. "cmd" should find "cmd.exe" in system32, but currently "cmd.exe" needs to be searched
        static String Which(String ExecutableName);

        /// @brief Attempt to get the executable directory from the a set of variables like those passed into Main.
        /// @details This is the fastest way to get the Executable location, but might not work on all platforms.
        /// @param ArgCount How many arguments will be passed in ArgVars.
        /// @param ArgVars A pointer to an array, with ArgCount elements, of char* which point to null terminated c strings.
        /// @warning If you pass bogus arguments to this bad things can and will happen. Infinite loops, segfaults etc... Just pass what main gives you
        /// @warning Not all system provide all the needed information to determine the executable directory
        /// @return If a whole path is present in ArgVars[0] this returns the directory part of that path, if this uses the executable file this returns '.', otherwise this with return "" indicating it is not usable.
        static String GetExecutableDirFromArg(int ArgCount, char** ArgVars);
        /// @brief Uses the main parameters stored on an instance of Mezzanine::ResourceManager to attempt determine executable directory
        /// @return Either a valid Path, '.' if the working dir is likely correct or "" if nothing could be determined.
        String GetExecutableDirFromArg() const;
        /// @brief Used a system call to get the curent Directory the executable is in. This make an external system call and is likely slower than GetExecutableDirFromArg
        /// @return This will return the current path this executable is stored in.
        static String GetExecutableDirFromSystem();
        /// @brief Get the Path to the current executable, fast from Args if Possible or from a system call otherwise.
        /// @param ArgCount How many arguments will be passed in ArgVars.
        /// @param ArgVars A pointer to an array, with ArgCount elements, of char* which point to null terminated c strings.
        /// @warning If you pass bogus arguments to this bad things can and will happen. Infinite loops, segfaults etc... Just pass what main gives you
        /// @warning Not all system provide all the needed information to determine the executable directory
        /// @return A String containing the path to the current executable.
        static String GetExecutableDir(int ArgCount, char** ArgVars);
        /// @brief Get the Path to the current executable, in a fast way if possible.
        /// @return A String containing the path to the current executable.
        String GetExecutableDir() const;

        /// @brief Creates all directories that do not exist in the provided path.
        /// @param DirectoryPath The path for the newly created directory or directories.
        /// @return Returns true if all directories were created, false in the case of a non-critical error.
        bool CreateDirectoryPath(const String& DirectoryPath);

        /// @brief Get a Listing of the files and subdirectories in a directory.
        /// @details This follows normal command line conventions, "." is the current directory,
        /// ".." is the parent directory. To access the file system root you will need to use a
        /// leading "c:/", "c:\\", or "/" as appropriate for the operating system the software will run on.
        /// @return This will return a pointer to a set of Strings the caller is responsible for deleting or a null pointer on an error.
        /// @param Dir The directory to check.
        static StringSet GetDirContents(const String& Dir = ".");

        /// @brief Get the working directory as a Mezzanine::String
        /// @return The Directory the game was called from (not nescessarilly the location of the executable), as a Mezzanine::String
        String GetWorkingDirectory() const;
        /// @brief Get the pathname where engine data is stored
        /// @return A String that contains the pathname
        String GetEngineDataDirectory() const;

        /// @brief Resolves a string describing one of the platform data paths to the actual path it is.
        /// @remarks Currently there are only 4 preset path variables, and depending on platform they two or more may go to the same location.
        /// The valid variables are as follows: LocalAppData, ShareableAppData, CurrentUserData, and CommonUserData.  These are not case sensative.
        /// @param PathVar String containing the name of the path variable.
        /// @return Returns the actual path of the variable provided.
        String ResolveDataPathFromString(const String& PathVar);
        /// @brief Gets the path to the directory intended for game and engine config data that is not meant to be shared.
        /// @return Returns a string containing the path to the Local Application Data Directory.
        String GetLocalAppDataDir() const;
        /// @brief Gets the path to the directory intended for game and engine config data that is allowed to be shared.
        /// @return Returns a string containing the path to the Shareable Application Data Directory.
        String GetShareableAppDataDir() const;
        /// @brief Gets the path to the directory intended for game saves and user profile data for the current user.
        /// @return Returns a string containing the path to the Current User Data Directory.
        String GetCurrentUserDataDir() const;
        /// @brief Gets the path to the directory intended for game saves and user profile data for all users.
        /// @return Returns a string containing the path to the Common User Data Directory.
        String GetCommonUserDataDir() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Management

        /// @brief Opens a stream to an asset in an AssetGroup.
        /// @param AssetName The identity of the asset to be opened (commonly a file name).
        /// @param AssetGroup The name of the AssetGroup where the Asset can be found.
        Resource::DataStreamPtr OpenAssetStream(const String& AssetName, const String& AssetGroup);

        /// @brief Creates a stream from a memory buffer.
        /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
        /// @param Buffer A pointer to the memory to stream from.
        /// @param BufferSize The size of the provided buffer in bytes.
        /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
        Resource::DataStreamPtr CreateDataStream(void* Buffer, const UInt32 BufferSize);
        /// @brief Creates a named stream from a memory buffer.
        /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
        /// @param AssetName The name to be given to the created stream.
        /// @param Buffer A pointer to the memory to stream from.
        /// @param BufferSize The size of the provided buffer in bytes.
        /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
        Resource::DataStreamPtr CreateDataStream(const String& AssetName, void* Buffer, const UInt32 BufferSize);
        /// @brief Creates a named stream from a memory buffer and adds it to the named AssetGroup.
        /// @note The created stream will take ownership of the buffer you provide.  If you want it to have a separate buffer then create a copy and pass that in.
        /// @param AssetName The name to be given to the created stream.
        /// @param AssetGroup The name of the AssetGroup this stream will be added to.
        /// @param Buffer A pointer to the memory to stream from.
        /// @param BufferSize The size of the provided buffer in bytes.
        /// @return Returns a @ref CountedPtr to the stream to the provided buffer.
        Resource::DataStreamPtr CreateDataStream(const String& AssetName, const String& AssetGroup, void* Buffer, const UInt32 BufferSize);

        ///////////////////////////////////////////////////////////////////////////////
        // AssetGroup Management

        /// @brief Adds a location for graphical resources.
        /// @details This function will add a location on the disk to find files needed to create and
        /// manipulate graphical objects. Once an asset is added it must be initalized using
        /// ResourceManager::InitResourceGroup(String Group).
        /// @param Location The location on the file system the asset can be found.
        /// @param Type The kind of file system the location can be found in. @n
        /// Options are: filesystem, zip.
        /// @param Group The name of the group the resources at this location belong to.  If the group does not exist it will be created.
        /// @param Recursive Whether or not to search sub-directories.
        void AddAssetLocation(const String& Location, const ArchiveType Type, const String& Group, const bool Recursive = false);
        /// @brief Creates an asset group.
        /// @param GroupName The name to be given to the created asset group.
        void CreateAssetGroup(const String& GroupName);
        /// @brief Destroys an asset group, unloading all of it's resources.
        /// @param GroupName The name of the asset group to destroy.
        void DestroyAssetGroup(const String& GroupName);
        /// @brief Prepares the asset for use.
        /// @details This function can be thought of as a preloader.  This will prepare the defined
        /// asset located on the disk for use.
        /// @param Name Name of the file/asset to be 'prepared'.
        /// @param Type The type of asset that the file is. @n
        /// Options are: Font, GpuProgram, HighLevelGpuProgram, Material, Mesh, Skeleton, Texture.
        /// @param Group Name of the group the asset belongs to.
        void DeclareAsset(const String& Name, const String& Type, const String& Group);
        /// @brief Makes a asset group ready to use.
        /// @details After adding all of your assets and declaring them as nessessary, this function
        /// is the final step.  After calling this function any and all assets within the defined group
        /// will be ready to use.  Do not initialize any more groups then you need to however, as that will
        /// take up memory and drop performance.
        /// @param Name Name of the asset group.
        void InitAssetGroup(const String& Name);

        ///////////////////////////////////////////////////////////////////////////////
        // Asset Query

        /// @brief Gets the actual path to an asset.
        /// @note This function currently only returns the first match, and doesn't check for multiple matches.
        /// @param FileName The name of the file to search for.
        /// @param Group The asset group to search in for the file.
        /// @return Returns a string containing the path to the file.
        String GetAssetPath(const String& FileName, const String& Group);

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief Gets the dot-and-extention of this platforms plugins.
        /// @return Returns the platform appropriate extention for plugin files.
        String GetPluginExtension() const;
        /// @brief Get a stream to read from the specified file.
        /// @details The returned ResourceInputStream is the Caller's responsibility to deal with. If it is not deleted it is a memory leak.
        /// @param FileName The name of the File you want to stream data from.
        /// @return An derivative of std::istream a ResourceInputStream that will pull it's data from the desired resource.
        ResourceInputStream* GetResourceStream(const String& FileName);

        /// @copydoc ManagerBase::Initialize()
        virtual void Initialize();
        /// @copydoc ManagerBase::Deinitialize()
        virtual void Deinitialize();

        /// @brief Gets a string that describes an @ref ArchiveType.
        /// @param ArchType A @ref ArchiveType That you want to log or pass to Ogre, or just need a @ref String that represents it.
        /// @return A String that represents the @ref ArchiveType passed.
        static String GetStringFromArchiveType(const Mezzanine::ArchiveType ArchType);
        /// @brief Gets an @ref ArchiveType from a string.
        /// @param FromString The string to be converted to an archive type.
        /// @return Returns a @ref ArchiveType corresponding to the string provided, or AT_Invalid if it is invalid.
        static ArchiveType GetArchiveTypeFromString(const String& FromString);

        ///////////////////////////////////////////////////////////////////////////////
        // Type Identifier Methods

        /// @copydoc ManagerBase::GetInterfaceType()
        virtual ManagerType GetInterfaceType() const;
        /// @copydoc ManagerBase::GetImplementationTypeName()
        virtual String GetImplementationTypeName() const;
    };//ResourceManager

    ///////////////////////////////////////////////////////////////////////////////
    /// @class DefaultResourceManagerFactory
    /// @headerfile resourcemanager.h
    /// @brief A factory responsible for the creation and destruction of the default resourcemanager.
    ///////////////////////////////////////
    class MEZZ_LIB DefaultResourceManagerFactory : public ManagerFactory
    {
    public:
        /// @brief Class constructor.
        DefaultResourceManagerFactory();
        /// @brief Class destructor.
        virtual ~DefaultResourceManagerFactory();

        /// @copydoc ManagerFactory::GetManagerTypeName()
        String GetManagerTypeName() const;

        /// @copydoc ManagerFactory::CreateManager(NameValuePairList&)
        ManagerBase* CreateManager(NameValuePairList& Params);
        /// @copydoc ManagerFactory::CreateManager(XML::Node&)
        ManagerBase* CreateManager(XML::Node& XMLNode);
        /// @copydoc ManagerFactory::DestroyManager(ManagerBase*)
        void DestroyManager(ManagerBase* ToBeDestroyed);
    };//DefaultResourceManagerFactory
}//Mezzanine

#endif
