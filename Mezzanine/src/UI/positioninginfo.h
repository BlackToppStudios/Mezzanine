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
#ifndef _uipositioninginfo_h
#define _uipositioninginfo_h

#include "uienumerations.h"
#include "UI/unifieddim.h"
#include "vector2.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a helper class designed to describe the behaviors of a quad when it needs to be repositioned.
        /// @details This struct contains all the information necessary to define complete behavior of
        /// positioning child quads within a quad.
        ///////////////////////////////////////
        class MEZZ_LIB PositioningInfo
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief Unified dimensions to be used if the resize rules permits it.
            UnifiedVec2 UPosition;
            /// @brief Rules for resizing on the X axis.
            UI::PositioningFlags PositionRules;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            PositioningInfo() :
                PositionRules(UI::PF_Unified_Pos) {  }
            /// @brief PositionFlags constructor.
            /// @param Rules The rules for determining the position of the object on a transform update.
            PositioningInfo(const UI::PositioningFlags Rules) :
                PositionRules(Rules) {  }
            /// @brief Position constructor.
            /// @param Position The unified position to use if the rules permit it.
            PositioningInfo(const UnifiedVec2& Position) :
                UPosition(Position), PositionRules(UI::PF_Unified_Pos) {  }
            /// @brief Descriptive constructor.
            /// @param Rules The rules for determining the position of the object on a transform update.
            /// @param Position The unified position to use if the rules permit it.
            PositioningInfo(const UI::PositioningFlags Rules, const UnifiedVec2& Position) :
                UPosition(Position), PositionRules(Rules) {  }
            /// @brief Copy constructor.
            /// @param Other The other PositioningInfo to copy from.
            PositioningInfo(const PositioningInfo& Other) :
                UPosition(Other.UPosition), PositionRules(Other.PositionRules) {  }
            /// @brief Class destructor.
            ~PositioningInfo() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            ///////////////////////////////////////////////////////////////////////////////
            // Comparison Operators

            /// @brief Equality comparison operator.
            /// @param Other The other PositioningInfo to compare to.
            /// @return Returns true if these PositioningInfo's are equal, false otherwise.
            inline bool operator==(const PositioningInfo& Other) const
            {
                return this->UPosition == Other.UPosition && this->PositionRules == Other.PositionRules;
            }
            /// @brief Inequality comparison operator.
            /// @param Other The other PositioningInfo to compare to.
            /// @return Returns true if these PositioningInfo's are not equal, false otherwise.
            inline bool operator!=(const PositioningInfo& Other) const
            {
                return this->UPosition != Other.UPosition || this->PositionRules != Other.PositionRules;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node PositioningNode = ParentNode.AppendChild( PositioningInfo::GetSerializableName() );

                if( PositioningNode.AppendAttribute("Version").SetValue("1") &&
                    PositioningNode.AppendAttribute("PositionRules").SetValue(static_cast<UInt32>(this->PositionRules)) )
                {
                    XML::Node UPositionNode = PositioningNode.AppendChild("UPosition");
                    this->UPosition.ProtoSerialize( UPositionNode );

                    return;
                }else{
                    SerializeError("Create XML Attribute Values",PositioningInfo::GetSerializableName(),true);
                }
            }
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;
                if( SelfRoot.Name() == PositioningInfo::GetSerializableName() ) {
                    if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = SelfRoot.GetAttribute("PositionRules");
                        if( !CurrAttrib.Empty() )
                            this->PositionRules = static_cast<UI::PositioningFlags>(CurrAttrib.AsUint());

                        XML::Node PositionNode = SelfRoot.GetChild("UPosition").GetFirstChild();
                        if( !PositionNode.Empty() )
                            this->UPosition.ProtoDeSerialize(PositionNode);
                    }else{
                        MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + PositioningInfo::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,PositioningInfo::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName()
            {
                return "PositioningInfo";
            }
        };//PositioningInfo
    }//UI
}//Mezzanine

#endif
