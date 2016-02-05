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
#ifndef _uinineboxstrategy_h
#define _uinineboxstrategy_h

#include "UI/mousehoverstrategy.h"
#include "UI/rect.h"
#include <map>

namespace Mezzanine
{
    namespace UI
    {
        /// @brief Enum describing the different 9-Box partitions of a screen.
        enum NineBoxPartition
        {
            NBP_TopLeft        = 1,
            NBP_TopCenter      = 2,
            NBP_TopRight       = 4,
            NBP_RightCenter    = 8,
            NBP_Center         = 16,
            NBP_LeftCenter     = 32,
            NBP_BottomLeft     = 64,
            NBP_BottomCenter   = 128,
            NBP_BottomRight    = 256
        };//NineBoxPartition

        ///////////////////////////////////////////////////////////////////////////////
        /// @class PartitionData
        /// @headerfile nineboxstrategy.h
        /// @brief This is a helper class for storing metadata for partitions.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB PartitionData
        {
        public:
            /// @brief Basic container type for Widget storage by this class.
            typedef std::vector<Widget*>          WidgetContainer;
        public:
            /// @brief An enum value identifying which screen partition this is.
            NineBoxPartition PartitionID;
            /// @brief A Rect representing the pixel limits of this screen partition.
            Rect PartitionRect;
            /// @brief A container storing all the quads that overlap with this screen partition.
            WidgetContainer Widgets;

            /// @brief Class constructor.
            /// @param PartID An enum value identifying which screen partition this is.
            PartitionData(const NineBoxPartition& PartID) :
                PartitionID(PartID)
                {  }
            /// @brief Class destructor.
            ~PartitionData()
                {  }
        };//PartitionData

        ///////////////////////////////////////////////////////////////////////////////
        /// @class NineBoxStrategy
        /// @headerfile nineboxstrategy.h
        /// @brief This strategy partitions the screen into 9 area's that track which Quads are in them, allowing a smaller list of quads to be checked.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB NineBoxStrategy : public MouseHoverStrategy
        {
        public:
            /// @brief Basic container type for PartitionData storage by this class.
            typedef std::vector<PartitionData*> PartitionVec;
        protected:
            /// @brief A container storing all the quads that overlap with this screen partition.
            PartitionVec Partitions;

            /// @brief Updates the partition cache with the newest quad positions
            void UpdateCache();
            /// @brief Convenience method for calculating the actual size and position of a single partition.
            /// @param PartID The enum value to calculate the pixel size and position for.
            /// @param ScreenSize The current size of the screen the partition is on.
            /// @param Returns a Rect storing the actual size and position of the specified partition.
            Rect CalculatePartitionRect(const NineBoxPartition& PartID, const Vector2& ScreenSize);
            /// @brief Gets the partition that the provided position is in.
            /// @param MousePos The screen position used to determine which partition to retrieve.
            /// @return Returns a pointer to the PartitionData of the partition the provided mouse position is above.
            PartitionData* GetHoveredPartition(const Vector2& MousePos);
        public:
            /// @brief Class constructor.
            NineBoxStrategy();
            /// @brief Class destructor.
            virtual ~NineBoxStrategy();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc MouseHoverStrategy::FindHoveredWidget(const Vector2& MousePos)
            Widget* FindHoveredWidget(const Vector2& MousePos);
        };//NineBoxStrategy
    }//UI
}//Mezzanine

#endif
