// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _uinineboxstrategy_cpp
#define _uinineboxstrategy_cpp

#include "UI/nineboxstrategy.h"
#include "UI/screen.h"
#include "UI/widget.h"

namespace Mezzanine
{
    namespace UI
    {
        ///////////////////////////////////////////////////////////////////////////////
        // NineBoxStrategy Functors

        ///////////////////////////////////////////////////////////////////////////////
        /// @class NineBoxCheck
        /// @headerfile nineboxstrategy.cpp
        /// @brief Simple functor for finding which renderable the mouse is hovered over.
        /// @details This is intended for reverse checks.
        ///////////////////////////////////////
        class NineBoxCheck
        {
        protected:
            /// @brief A pointer to the Vector of 9 partitions on the screen to use for spacial checks.
            NineBoxStrategy::PartitionVec* Partitions;
        public:
            /// @brief Functor constructor.
            /// @param StratPartitions A pointer to the Vector of 9 partitions on the screen to use for spacial checks.
            NineBoxCheck(NineBoxStrategy::PartitionVec* StratPartitions) :
                Partitions(StratPartitions)
                {  }
            /// @brief Class destructor.
            ~NineBoxCheck()
                {  }

            /// @brief Interface needed for processing child widgets of screens and widgets.
            /// @param Wid A pointer to the Widget that will be processed.
            /// @return Returns true if a valid result is found.
            Boole operator()(Widget* Wid)
            {
                for( NineBoxStrategy::PartitionVec::iterator It = Partitions->begin() ; It != Partitions->end() ; ++It )
                {
                    if( (*It)->PartitionRect.CheckOverlap(Wid->GetRect()) )
                        (*It)->Widgets.push_back(Wid);
                }
                return false;
            }
        };//NineBoxCheck

        ///////////////////////////////////////////////////////////////////////////////
        // NineBoxStrategy Methods

        NineBoxStrategy::NineBoxStrategy()
        {
            this->Partitions.push_back(new PartitionData(NBP_TopLeft));
            this->Partitions.push_back(new PartitionData(NBP_TopCenter));
            this->Partitions.push_back(new PartitionData(NBP_TopRight));
            this->Partitions.push_back(new PartitionData(NBP_LeftCenter));
            this->Partitions.push_back(new PartitionData(NBP_Center));
            this->Partitions.push_back(new PartitionData(NBP_RightCenter));
            this->Partitions.push_back(new PartitionData(NBP_BottomLeft));
            this->Partitions.push_back(new PartitionData(NBP_BottomCenter));
            this->Partitions.push_back(new PartitionData(NBP_BottomRight));
        }

        NineBoxStrategy::~NineBoxStrategy()
        {
            for( PartitionVec::iterator It = this->Partitions.begin() ; It != this->Partitions.end() ; ++It )
            {
                delete (*It);
            }
            this->Partitions.clear();
        }

        void NineBoxStrategy::UpdateCache()
        {
            if( this->ScreenDirty )
            {
                Vector2 ParentSize = this->ParentScreen->GetActualSize();
                for( PartitionVec::iterator It = this->Partitions.begin() ; It != this->Partitions.end() ; ++It )
                {
                    (*It)->PartitionRect = CalculatePartitionRect((*It)->PartitionID,ParentSize);
                    (*It)->Widgets.clear();
                }

                NineBoxCheck Check( &(this->Partitions) );
                this->ParentScreen->_ProcessAllChildren(&Check);

                this->ScreenDirty = false;
            }
        }

        Rect NineBoxStrategy::CalculatePartitionRect(const NineBoxPartition& PartID, const Vector2& ScreenSize)
        {
            Vector2 ThirdSize(ScreenSize.X / 3,ScreenSize.Y / 3);
            switch(PartID)
            {
                case NBP_TopLeft:       return Rect(Vector2(0,0),ThirdSize);                              break;
                case NBP_TopCenter:     return Rect(Vector2(ThirdSize.X * 1,0),ThirdSize);                break;
                case NBP_TopRight:      return Rect(Vector2(ThirdSize.X * 2,0),ThirdSize);                break;
                case NBP_LeftCenter:    return Rect(Vector2(0,ThirdSize.Y * 1),ThirdSize);                break;
                case NBP_Center:        return Rect(Vector2(ThirdSize.X * 1,ThirdSize.Y * 1),ThirdSize);  break;
                case NBP_RightCenter:   return Rect(Vector2(ThirdSize.X * 2,ThirdSize.Y * 1),ThirdSize);  break;
                case NBP_BottomLeft:    return Rect(Vector2(0,ThirdSize.Y * 2),ThirdSize);                break;
                case NBP_BottomCenter:  return Rect(Vector2(ThirdSize.X * 1,ThirdSize.Y * 2),ThirdSize);  break;
                case NBP_BottomRight:   return Rect(Vector2(ThirdSize.X * 2,ThirdSize.Y * 2),ThirdSize);  break;
                default:
                { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Invalid Partition ID used to calculate a Partition Rect.");  break; }
            }
        }

        PartitionData* NineBoxStrategy::GetHoveredPartition(const Vector2& MousePos)
        {
            for( PartitionVec::iterator It = this->Partitions.begin() ; It != this->Partitions.end() ; ++It )
            {
                if( (*It)->PartitionRect.IsInside(MousePos) )
                    return (*It);
            }
            return NULL;
        }

        Widget* NineBoxStrategy::FindHoveredWidget(const Vector2& MousePos)
        {
            this->UpdateCache();
            PartitionData* HoveredPartition = this->GetHoveredPartition(MousePos);
            if( HoveredPartition == NULL )
                return NULL;

            for( PartitionData::WidgetContainer::reverse_iterator WidIt = HoveredPartition->Widgets.rbegin() ; WidIt != HoveredPartition->Widgets.rend() ; ++WidIt )
            {
                // Can it be seen?
                if( (*WidIt)->IsVisible() && (*WidIt)->GetNumVisibleRenderLayers() ) {
                    // Is it inside?
                    if( (*WidIt)->IsInside(MousePos) ) {
                        // Has mouse picking been disabled?
                        if( !(*WidIt)->GetMousePassthrough() ) {
                            return (*WidIt);
                        }
                    }
                }
            }
            return NULL;
        }
    }//UI
}//Mezzanine

#endif
