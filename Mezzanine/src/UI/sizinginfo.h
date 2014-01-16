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
#ifndef _uisizinginfo_h
#define _uisizinginfo_h

#include "uienumerations.h"
#include "UI/unifieddim.h"
#include "vector2.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a helper class designed to describe the behaviors of a quad when it needs to be resized.
        /// @details This struct contains all the information necessary to define complete behavior of
        /// sizing child quads within a quad.
        ///////////////////////////////////////
        class MEZZ_LIB SizingInfo
        {
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Public Data Members

            /// @brief Unified dimensions to be used if the resize rules permits it.
            UnifiedVec2 USize;
            /// @brief The maximum permitted size.
            UnifiedVec2 MaxSize;
            /// @brief The minumum permitted size.
            UnifiedVec2 MinSize;
            /// @brief Rule for determining aspect ratio lock.
            UI::AspectRatioLock RatioLock;
            /// @brief Rules for resizing on the Y axis.
            UI::SizingRules VerticalRules;
            /// @brief Rules for resizing on the X axis.
            UI::SizingRules HorizontalRules;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and Destruction

            /// @brief Class constructor.
            SizingInfo() :
                RatioLock(UI::ARL_Ratio_Unlocked), VerticalRules(UI::SR_Unified_Dims), HorizontalRules(UI::SR_Unified_Dims) {  }
            /// @brief Size constructor.
            /// @param Size A UnifiedVector2 to be used if the rules provided permit it on sizing.
            SizingInfo(const UnifiedVec2& Size) :
                USize(Size), RatioLock(UI::ARL_Ratio_Unlocked), VerticalRules(UI::SR_Unified_Dims), HorizontalRules(UI::SR_Unified_Dims) {  }
            /// @brief Rules constructor.
            /// @param HRules The rules to be used for resizing on the X axis.
            /// @param VRules The rules to be used for resizing on the Y axis.
            SizingInfo(const UI::SizingRules HRules, const UI::SizingRules VRules) :
                RatioLock(UI::ARL_Ratio_Unlocked), VerticalRules(VRules), HorizontalRules(HRules) {  }
            /// @brief Descriptive constructor.
            /// @param HRules The rules to be used for resizing on the X axis.
            /// @param VRules The rules to be used for resizing on the Y axis.
            /// @param Size A UnifiedVector2 to be used if the rules provided permit it on sizing.
            SizingInfo(const UI::SizingRules HRules, const UI::SizingRules VRules, const UnifiedVec2& Size) :
                USize(Size), RatioLock(UI::ARL_Ratio_Unlocked), VerticalRules(VRules), HorizontalRules(HRules) {  }
            /// @brief Copy constructor.
            /// @param Other The other SizingInfo to copy from.
            SizingInfo(const SizingInfo& Other) :
                USize(Other.USize), MaxSize(Other.MaxSize), MinSize(Other.MinSize), RatioLock(Other.RatioLock),
                VerticalRules(Other.VerticalRules), HorizontalRules(Other.HorizontalRules) {  }
            /// @brief Class destructor.
            ~SizingInfo() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Checks to see if this will expand horizontally.
            /// @return Returns true if this will expand on the X axis.
            inline Boolean CanExpandHorizontally() const
                { return (this->HorizontalRules & UI::SR_Fill_Available); }
            /// @brief Checks to see if this will expand vertically.
            /// @return Returns true if this will expand on the Y axis.
            inline Boolean CanExpandVertically() const
                { return (this->VerticalRules & UI::SR_Fill_Available); }
            /// @brief Cheacks to see if this will expand horizontally or vertically.
            /// @return Returns true if this will expand on either axis.
            inline Boolean CanExpand() const
                { return (this->CanExpandHorizontally() || this->CanExpandVertically()); }

            ///////////////////////////////////////////////////////////////////////////////
            // Comparison Operators

            /// @brief Equality comparison operator.
            /// @param Other The other SizingInfo to compare to.
            /// @return Returns true if these SizingInfo's are equal, false otherwise.
            inline Boolean operator==(const SizingInfo& Other) const
            {
                return ( this->USize == Other.USize &&
                         this->MaxSize == Other.MaxSize &&
                         this->MinSize == Other.MinSize &&
                         this->RatioLock == Other.RatioLock &&
                         this->VerticalRules == Other.VerticalRules &&
                         this->HorizontalRules == Other.HorizontalRules );
            }
            /// @brief Inequality comparison operator.
            /// @param Other The other SizingInfo to compare to.
            /// @return Returns true if these SizingInfo's are not equal, false otherwise.
            inline Boolean operator!=(const SizingInfo& Other) const
            {
                return ( this->USize != Other.USize ||
                         this->MaxSize != Other.MaxSize ||
                         this->MinSize != Other.MinSize ||
                         this->RatioLock != Other.RatioLock ||
                         this->VerticalRules != Other.VerticalRules ||
                         this->HorizontalRules != Other.HorizontalRules );
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            /// @brief Convert this class to an XML::Node ready for serialization.
            /// @param ParentNode The point in the XML hierarchy that all this renderable should be appended to.
            void ProtoSerialize(XML::Node& ParentNode) const
            {
                XML::Node SizingNode = ParentNode.AppendChild( SizingInfo::GetSerializableName() );

                if( SizingNode.AppendAttribute("Version").SetValue("1") &&
                    SizingNode.AppendAttribute("HorizontalRules").SetValue(static_cast<UInt32>(this->HorizontalRules)) &&
                    SizingNode.AppendAttribute("VerticalRules").SetValue(static_cast<UInt32>(this->VerticalRules)) &&
                    SizingNode.AppendAttribute("RatioLock").SetValue(static_cast<UInt32>(this->RatioLock)) )
                {
                    XML::Node MinSizeNode = SizingNode.AppendChild("MinSize");
                    this->MinSize.ProtoSerialize( MinSizeNode );

                    XML::Node MaxSizeNode = SizingNode.AppendChild("MaxSize");
                    this->MaxSize.ProtoSerialize( MaxSizeNode );

                    XML::Node USizeNode = SizingNode.AppendChild("USize");
                    this->USize.ProtoSerialize( USizeNode );

                    return;
                }else{
                    SerializeError("Create XML Attribute Values",SizingInfo::GetSerializableName(),true);
                }
            }
            /// @brief Take the data stored in an XML Node and overwrite this object with it.
            /// @param SelfRoot An XML::Node containing the data to populate this class with.
            void ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                XML::Attribute CurrAttrib;
                if( SelfRoot.Name() == SizingInfo::GetSerializableName() ) {
                    if(SelfRoot.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = SelfRoot.GetAttribute("HorizontalRules");
                        if( !CurrAttrib.Empty() )
                            this->HorizontalRules = static_cast<UI::SizingRules>(CurrAttrib.AsUint());

                        CurrAttrib = SelfRoot.GetAttribute("VerticalRules");
                        if( !CurrAttrib.Empty() )
                            this->VerticalRules = static_cast<UI::SizingRules>(CurrAttrib.AsUint());

                        CurrAttrib = SelfRoot.GetAttribute("RatioLock");
                        if( !CurrAttrib.Empty() )
                            this->RatioLock = static_cast<UI::AspectRatioLock>(CurrAttrib.AsUint());

                        XML::Node MinSizeNode = SelfRoot.GetChild("MinSize").GetFirstChild();
                        if( !MinSizeNode.Empty() )
                            this->MinSize.ProtoDeSerialize(MinSizeNode);

                        XML::Node MaxSizeNode = SelfRoot.GetChild("MaxSize").GetFirstChild();
                        if( !MaxSizeNode.Empty() )
                            this->MaxSize.ProtoDeSerialize(MaxSizeNode);

                        XML::Node USizeNode = SelfRoot.GetChild("USize").GetFirstChild();
                        if( !USizeNode.Empty() )
                            this->USize.ProtoDeSerialize(USizeNode);
                    }else{
                        MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SizingInfo::GetSerializableName() + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(Exception::II_IDENTITY_NOT_FOUND_EXCEPTION,SizingInfo::GetSerializableName() + " was not found in the provided XML node, which was expected.");
                }
            }
            /// @brief Get the name of the the XML tag the Renderable class will leave behind as its instances are serialized.
            /// @return A string containing the name of this class.
            static String GetSerializableName()
            {
                return "SizingInfo";
            }
        };//SizingInfo
    }//UI
}//Mezzanine

#endif
