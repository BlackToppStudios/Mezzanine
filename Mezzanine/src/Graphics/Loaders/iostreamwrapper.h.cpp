// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
#ifndef _internaliostreamwrapper_h_cpp
#define _internaliostreamwrapper_h_cpp

// Keeps this file from being documented by doxygen
/// @cond DontDocumentInternal

#include "datastream.h"

#include <OgreDataStream.h>

namespace Mezzanine
{
    namespace Graphics
    {
        /*
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An std::fstream class that implements the Ogre Datastream interface.
        ///////////////////////////////////////
        class MEZZ_LIB FStreamAdapter : public Ogre::DataStream, public std::fstream
        {
        public:
            /// @brief Class constructor.
            FStreamAdapter() = default;
            /// @brief Opening constructor.
            /// @param FileName The name of the file to open.
            /// @param Mode The IO mode to open the file with.
            FStreamAdapter(const char* FileName, ios_base::openmode Mode = ios_base::in | ios_base::out);
            /// @brief Class destructor.
            virtual ~FStreamAdapter() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//FStreamAdapter

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An std::ifstream class that implements the Ogre Datastream interface.
        ///////////////////////////////////////
        class MEZZ_LIB IFStreamAdapter : public Ogre::DataStream, public std::ifstream
        {
        public:
            /// @brief Class constructor.
            IFStreamAdapter() = default;
            /// @brief Class constructor.
            /// @param FileName The name of the file to open.
            /// @param Mode The IO mode to open the file with.
            IFStreamAdapter(const char* FileName, ios_base::openmode Mode = ios_base::in);
            /// @brief Class destructor.
            virtual ~IFStreamAdapter() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//IFStreamAdapter

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An std::ofstream class that implements the Ogre Datastream interface.
        ///////////////////////////////////////
        class MEZZ_LIB OFStreamAdapter : public Ogre::DataStream, public std::ofstream
        {
        public:
            /// @brief Class constructor.
            OFStreamAdapter() = default;
            /// @brief Opening constructor.
            /// @param FileName The name of the file to open.
            /// @param Mode The IO mode to open the file with.
            OFStreamAdapter(const char* FileName, ios_base::openmode Mode = ios_base::out);
            /// @brief Class destructor.
            virtual ~OFStreamAdapter() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//OFStreamAdapter
        */

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A class for wrapping standard I/O wrappers to be used by the Ogre resource system.
        ///////////////////////////////////////
        class MEZZ_LIB IOStreamWrapper : public Ogre::DataStream
        {
        protected:
            /// @brief A pointer to the I/O stream being wrapped.
            DataStreamPtr Stream;
        public:
            /// @brief Class constructor.
            /// @param ToBeWrapped A pointer to the I/O stream to be wrapped.
            IOStreamWrapper(DataStreamPtr ToBeWrapped);
            /// @brief Class destructor.
            virtual ~IOStreamWrapper();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//IOStreamWrapper

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A class for wrapping standard Input streams to be used by the Ogre resource system.
        ///////////////////////////////////////
        class MEZZ_LIB IStreamWrapper : public Ogre::DataStream
        {
        protected:
            /// @brief A shared pointer to the Input stream being wrapped.
            IStreamPtr Stream;
        public:
            /// @brief Class constructor.
            /// @param ToBeWrapped A pointer to the Input stream to be wrapped.
            IStreamWrapper(IStreamPtr ToBeWrapped);
            /// @brief Class destructor.
            virtual ~IStreamWrapper();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//IStreamWrapper

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A class for wrapping standard Output streams to be used by the Ogre resource system.
        ///////////////////////////////////////
        class MEZZ_LIB OStreamWrapper : public Ogre::DataStream
        {
        protected:
            /// @brief A pointer to the Output stream being wrapped.
            OStreamPtr Stream;
        public:
            /// @brief Class constructor.
            /// @param ToBeWrapped A pointer to the Output stream to be wrapped.
            OStreamWrapper(OStreamPtr ToBeWrapped);
            /// @brief Class destructor.
            virtual ~OStreamWrapper();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//OStreamWrapper

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A class for wrapping standard I/O wrappers to be used by the Ogre resource system.
        ///////////////////////////////////////
        class MEZZ_LIB STDIOStreamWrapper : public Ogre::DataStream
        {
        protected:
            /// @brief A pointer to the I/O stream being wrapped.
            std::iostream* Stream;
            /// @brief Stores whether or not the stream should be deleted when this wrapper is destroyed.
            Boole CleanUp;
        public:
            /// @brief Class constructor.
            /// @param ToBeWrapped A pointer to the I/O stream to be wrapped.
            /// @param Clean Whether or not the stream should be deleted when this wrapper is destroyed.
            STDIOStreamWrapper(std::iostream* ToBeWrapped, const Boole Clean);
            /// @brief Class destructor.
            virtual ~STDIOStreamWrapper();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//STDIOStreamWrapper

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A class for wrapping standard Input streams to be used by the Ogre resource system.
        ///////////////////////////////////////
        class MEZZ_LIB STDIStreamWrapper : public Ogre::DataStream
        {
        protected:
            /// @brief A pointer to the Input stream being wrapped.
            std::istream* Stream;
            /// @brief Stores whether or not the stream should be deleted when this wrapper is destroyed.
            Boole CleanUp;
        public:
            /// @brief Class constructor.
            /// @param ToBeWrapped A pointer to the Input stream to be wrapped.
            /// @param Clean Whether or not the stream should be deleted when this wrapper is destroyed.
            STDIStreamWrapper(std::istream* ToBeWrapped, const Boole Clean);
            /// @brief Class destructor.
            virtual ~STDIStreamWrapper();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//STDIStreamWrapper

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A class for wrapping standard Output streams to be used by the Ogre resource system.
        ///////////////////////////////////////
        class MEZZ_LIB STDOStreamWrapper : public Ogre::DataStream
        {
        protected:
            /// @brief A pointer to the Output stream being wrapped.
            std::ostream* Stream;
            /// @brief Stores whether or not the stream should be deleted when this wrapper is destroyed.
            Boole CleanUp;
        public:
            /// @brief Class constructor.
            /// @param ToBeWrapped A pointer to the Output stream to be wrapped.
            /// @param Clean Whether or not the stream should be deleted when this wrapper is destroyed.
            STDOStreamWrapper(std::ostream* ToBeWrapped, const Boole Clean);
            /// @brief Class destructor.
            virtual ~STDOStreamWrapper();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc Ogre::DataStream::read(void*, size_t)
            size_t read(void* buf, size_t count) override;
            /// @copydoc Ogre::DataStream::write(const void*, size_t)
            size_t write(const void* buf, size_t count) override;

            /// @copydoc Ogre::DataStream::skip(long)
            void skip(long count) override;
            /// @copydoc Ogre::DataStream::seek(size_t)
            void seek(size_t pos) override;

            /// @copydoc Ogre::DataStream::tell() const
            size_t tell() const override;
            /// @copydoc Ogre::DataStream::eof() const
            bool eof() const override;
            /// @copydoc Ogre::DataStream::close()
            void close() override;
        };//STDOStreamWrapper
    }//Graphics
}//Nezzanine

/// @endcond

#endif
