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
#ifndef _graphicstexturemanager_h
#define _graphicstexturemanager_h

#include "entresolmanager.h"
#include "entresolmanagerfactory.h"
#include "singleton.h"
#include "Graphics/graphicsenumerations.h"

namespace Ogre
{
    class Texture;
    template<typename T> class SharedPtr;
    typedef SharedPtr<Texture> TexturePtr;
    class TextureManager;
}

namespace Mezzanine
{
    namespace Graphics
    {
        class Texture;
        // Used by the scripting language binder to help create bindgings for this class. SWIG does know to creation template instances
        #ifdef SWIG
        %template(SingletonTextureManager) Singleton<TextureManager>;
        #endif

        ///////////////////////////////////////////////////////////////////////////////
        /// @class TextureManager
        /// @brief This manager handles the storage and query of of Graphics Textures.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB TextureManager : public EntresolManager, public Singleton<TextureManager>
        {
        public:
            /// @brief Basic container type for Texture storage in this class.
            typedef std::map< String, Texture* >           TextureContainer;
            /// @brief Iterator type for Texture instances stored in this class.
            typedef TextureContainer::iterator             TextureIterator;
            /// @brief Const Iterator type for Texture instances stored in this class.
            typedef TextureContainer::const_iterator       ConstTextureIterator;

            /// @brief A String containing the name of this manager implementation.
            static const String ImplementationName;
            /// @brief A ManagerType enum value used to describe the type of interface/functionality this manager provides.
            static const ManagerBase::ManagerType InterfaceType;
        protected:
            /// @internal
            /// @brief Container storing all of the currently loaded Textures.
            TextureContainer Textures;

            /// @internal
            /// @brief Adds a Texture to this manager.
            /// @exception If the name of the Texture being added is not unique a II_DUPLICATE_IDENTITY_EXCEPTION will be thrown.
            /// @param ToAdd The Texture to be added.
            virtual void AddTexture(Texture* ToAdd);
        public:
            /// @brief Class constructor.
            TextureManager();
            /// @brief XML constructor.
            /// @param XMLNode The node of the xml document to construct from.
            TextureManager(const XML::Node& XMLNode);
            /// @brief Class destructor.
            virtual ~TextureManager();

            ///////////////////////////////////////////////////////////////////////////////
            // Manual Creation

            /// @brief Creates a blank texture with a width and height.
            /// @param ResourceName The name of the resource to be created.
            /// @param ResourceGroup The name of the group the resource will be created in.
            /// @param Type The type of blank texture to be created.  See @ref TextureType enum for more details.
            /// @param Width The width of the texture to be loaded.
            /// @param Height The height of the texture to be loaded.
            /// @param NumMipMaps The number of mipmaps that exist for the texture.
            /// @param Format The pixel format to create the buffer for.
            /// @param Usage Indicates how the texture will be used so the buffer structure can be optimized.
            /// @param HWGammaCorrect Whether or not to enable hardware gamma correction (sRGB) on this texture.  The hardware will convert from gamma space to linear space when reading from this texture.  Only applicable for 8-bits per channel textures, will be ignored for other types.
            /// @param FSAA The level of multisampling to use if this is a render target. Ignored if usage does not include TU_RenderTarget or if the device does not support it.
            /// @return Returns a pointer to the created texture.
            Texture* CreateManualTexture(const String& ResourceName, const String& ResourceGroup, const Graphics::TextureType Type, const Whole Width, const Whole Height,
                                         const Integer NumMipMaps, const Graphics::PixelFormat Format, const Whole Usage = Graphics::TU_Default, const Boole HWGammaCorrect = false, const Whole FSAA = 0);
            /// @brief Creates a blank texture with a width, height, and depth.
            /// @param ResourceName The name of the resource to be created.
            /// @param ResourceGroup The name of the group the resource will be created in.
            /// @param Type The type of blank texture to be created.  See @ref TextureType enum for more details.
            /// @param Width The width of the texture to be loaded.
            /// @param Height The height of the texture to be loaded.
            /// @param NumMipMaps The number of mipmaps that exist for the texture.
            /// @param Format The pixel format to create the buffer for.
            /// @param Usage Indicates how the texture will be used so the buffer structure can be optimized.
            /// @param HWGammaCorrect Whether or not to enable hardware gamma correction (sRGB) on this texture.  The hardware will convert from gamma space to linear space when reading from this texture.  Only applicable for 8-bits per channel textures, will be ignored for other types.
            /// @param FSAA The level of multisampling to use if this is a render target. Ignored if usage does not include TU_RenderTarget or if the device does not support it.
            /// @return Returns a pointer to the created texture.
            Texture* CreateManualTexture(const String& ResourceName, const String& ResourceGroup, const Graphics::TextureType Type, const Whole Width, const Whole Height, const Whole Depth,
                                         const Integer NumMipMaps, const Graphics::PixelFormat Format, const Whole Usage = Graphics::TU_Default, const Boole HWGammaCorrect = false, const Whole FSAA = 0);

            ///////////////////////////////////////////////////////////////////////////////
            // Texture Management

            /// @brief Loads a Texture file from disk and prepares it for use.
            /// @param TextureName The name of the Texture file to be loaded.
            /// @param Group The resource group from which the Texture file should be loaded.
            /// @return Returns a pointer to the loaded Texture.
            virtual Texture* LoadTexture(const String& TextureName, const String& Group);
            /// @brief Unloads a Texture file.
            /// @param MeshName The name of the Texture to be unloaded.
            virtual void UnloadTexture(const String& TextureName);
            /// @brief Gets a Texture stored in this manager.
            /// @param MeshName The name of the Texture to retrieve.
            /// @return Returns a pointer to the requested Texture.
            Texture* GetTexture(const String& TextureName);
            /// @brief Gets the number of currently loaded Textures.
            /// @return Returns a Whole representing the number of Textures currently loaded.
            virtual Whole GetNumTextures();
            /// @brief Unloads every Texture that is currently loaded.
            virtual void UnloadAllTextures();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc ManagerBase::Initialize()
            virtual void Initialize();
            /// @copydoc ManagerBase::Deinitialize()
            virtual void Deinitialize();

            ///////////////////////////////////////////////////////////////////////////////
            // Type Identifier Methods

            /// @copydoc ManagerBase::GetInterfaceType()
            virtual ManagerType GetInterfaceType() const;
            /// @copydoc ManagerBase::GetImplementationTypeName()
            virtual String GetImplementationTypeName() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            /// @internal
            /// @brief Wraps and stores an Ogre Texture instance.
            /// @param ToWrap The Ogre Texture to get wrapped.
            /// @return Returns a pointer to the wrapped Texture.
            virtual Texture* _WrapInternalTexture(Ogre::TexturePtr ToWrap);
            /// @internal
            /// @brief Gets the internal TextureManager.
            /// @return Returns a pointer to the internal TextureManager.
            Ogre::TextureManager* _GetInternalManager() const;
        };//TextureManager

        ///////////////////////////////////////////////////////////////////////////////
        /// @class DefaultTextureManagerFactory
        /// @brief A factory responsible for the creation and destruction of the default TextureManager.
        ///////////////////////////////////////
        class MEZZ_LIB DefaultTextureManagerFactory : public EntresolManagerFactory
        {
        public:
            /// @brief Class constructor.
            DefaultTextureManagerFactory();
            /// @brief Class destructor.
            virtual ~DefaultTextureManagerFactory();

            /// @copydoc ManagerFactory::GetManagerImplName()
            String GetManagerImplName() const;
            /// @copydoc ManagerFactory::GetManagerType() const
            ManagerBase::ManagerType GetManagerType() const;

            /// @copydoc EntresolManagerFactory::CreateManager(const NameValuePairList&)
            EntresolManager* CreateManager(const NameValuePairList& Params);
            /// @copydoc EntresolManagerFactory::CreateManager(const XML::Node&)
            EntresolManager* CreateManager(const XML::Node& XMLNode);
            /// @copydoc EntresolManagerFactory::DestroyManager(EntresolManager*)
            void DestroyManager(EntresolManager* ToBeDestroyed);
        };//DefaultTextureManagerFactory
    }//Graphics
}//Mezzanine


#endif
