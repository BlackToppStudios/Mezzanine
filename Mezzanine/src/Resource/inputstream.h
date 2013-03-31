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
#ifndef _resourceinputstream_h
#define _resourceinputstream_h

#include "crossplatformexport.h"
#include "datatypes.h"


namespace Mezzanine
{
	//Forward Declarations
	namespace Internal
	{
		class OgreDataStreamBuf;
	}
	class ResourceManager;

	///////////////////////////////////////////////////////////////////////////////
    /// @brief A stream from a file, can read from zip files as well
	/// @warning This currently only supports reading from the stream, and does not support any pointer navigation, or any other non-reading functions
	/// @details
	class MEZZ_LIB ResourceInputStream : public std::istream
	{
		private:
			Internal::OgreDataStreamBuf* OgreBuffer;
			ResourceManager* Manager;

			/// @internal
			/// @brief Called by the constructors to actually construct this class
			/// @param InputBuffer A pointer to an Internal::OgreDataStreamBuf. A buffer to read from the disk access subsystem (which happens to be part of Ogre). This Stream will assume ownership of this buffer and will handle deleting it.
			/// @param ResourceManager_ Currently unused, future functionality may tuse this.
			void Construct(std::streambuf *InputBuffer, ResourceManager* ResourceManager_);

		protected:

		public:
			/// @brief Descriptive Constructor
			/// @param InputBuffer A pointer to an Internal::OgreDataStreamBuf. A buffer to read from the disk access subsystem (which happens to be part of Ogre). This Stream will assume ownership of this buffer and will handle deleting it.
			/// @param ResourceManager_ Currently unused, future functionality may tuse this.
			/// @warning Do not delete the InputBuffer you pass in, this class will assume owner ship and delete it on it's own
			ResourceInputStream(std::streambuf *InputBuffer, ResourceManager* ResourceManager_) :
				std::istream(InputBuffer)
				{ this->Construct(InputBuffer, ResourceManager_); }

			/// @brief Tears down the Stream, and Delete the Buffer Passed in.
			virtual ~ResourceInputStream();

			/// @brief Can this be read from
			/// @return A bool true if it can be read from
			bool Readable();

			/// @brief Can this be written to
			/// @return A bool true if it can be written to
			bool Writeable();

			template<class T> ResourceInputStream& operator >> ( T& Data )
			{
                std::istream* temp = static_cast<std::istream*>(this);
				(*temp) >> Data;
				return *this;
			}
	}; // \ResourceInputStream
} // \Mezz

#endif

