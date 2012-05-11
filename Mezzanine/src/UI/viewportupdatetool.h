//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _uiviewportupdatetool_h
#define _uiviewportupdatetool_h

#include "vector2.h"

namespace Mezzanine
{
    namespace UI
    {
        class BasicRenderable;
        class Rectangle;
        class Widget;
        typedef std::pair<Vector2,Vector2> WidgetResult;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class ViewportUpdateTool
        /// @headerfile uiviewportupdatetool.h
        /// @brief This class facilitates the resizing and repositioning of all renderables in the UI system during a viewport update.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB ViewportUpdateTool
        {
            public:
                /// @enum ResizeMode
                /// @brief This enum describes how this tool should go about resizing renderables during a viewport update.
                /// @details
                enum ResizeMode
                {
                    RSM_Nothing,         ///< Do not attempt to resize any renderables.
                    RSM_Scale_All,       ///< Scale all renderables by the change in viewport dimensions.  Warning:  This can cause stretching/compression if an aspect ratio change occurs.
                    RSM_Scale_By_Height  ///< Scale the width of each renderable by the same factor as it's height.  This setting will have identical output to "RSM_Scale" unless there was a change in aspect ratio.
                };
                /// @enum RepositionMode
                /// @brief This enum describes how this tool should go about repositioning renderables during a viewport update.
                /// @details
                enum RepositionMode
                {
                    RPM_Nothing,                    ///< Do not attempt to reposition any renderables.
                    RPM_Scale_All,                  ///< Position all renderables based on their previous relative position.
                    RPM_Scale_Quadrents,            ///< Position all renderables to match the same relative distance from each edge of the viewport as before.
                    RPM_Scale_Quadrents_By_Height,  ///< Same as "RPM_Scale_Quadrents", but the distance is to be the same as (px distance * Y factor).  This setting will have identical output to "RPM_Scale_Quadrents" unless there was a change in aspect ratio.
                    RPM_Absolute_Quadrents          ///< Position renderables to be the exact same distance from each edge as before.
                };
            protected:
                static Vector2 OldSize;
                static Vector2 NewSize;
                static Real MiddleTolerance;
                static ResizeMode RSM;
                static RepositionMode RPM;
                /// @internal
                /// @brief Gets the updated size according to the RSM_Scale_All resize mode.
                static Vector2 RSM_ScaleAll(const Vector2& RenderableSize);
                /// @internal
                /// @brief Gets the updated size according to the RSM_Scale_By_Height resize mode.
                static Vector2 RSM_ScaleByHeight(const Vector2& RenderableSize);
                /// @internal
                /// @brief GEts the updated position according to the RPM_Scale_All reposition mode.
                static Vector2 RPM_ScaleAll(const Vector2& RenderablePosition);
                /// @internal
                /// @brief Gets the updated position according to the RSM_Scale_Quadrents reposition mode.
                static Vector2 RPM_ScaleQuadrents(const Vector2& RenderablePosition, const Vector2& RenderableSize, const Vector2& UpdatedRendSize);
                /// @internal
                /// @brief Gets the updated position according to the RSM_Scale_Quadrents_By_Height reposition mode.
                static Vector2 RPM_ScaleQuadrentsByHeight(const Vector2& RenderablePosition, const Vector2& RenderableSize, const Vector2& UpdatedRendSize);
                /// @internal
                /// @brief Gets the updated position according to the RPM_Absolute_Quadrents reposition mode.
                static Vector2 RPM_AbsoluteQuadrents(const Vector2& RenderablePosition, const Vector2& RenderableSize, const Vector2& UpdatedRendSize);
                /// @internal
                /// @brief Gets the new size based on current settings.
                static Vector2 GetUpdatedSize(const Vector2& RendSize);
                /// @internal
                /// @brief Gets the new position based on current settings.
                static Vector2 GetUpdatedPosition(const Vector2& RendPosition, const Vector2& RendSize, const Vector2& UpdatedRendSize);
            public:
                /// @brief Class constructor.
                ViewportUpdateTool();
                /// @brief Class destructor.
                ~ViewportUpdateTool();

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Updates the internal sizes being used for calculating necessary resizes.
                /// @param OldSize The viewport size that is being transitioned away from.
                /// @param NewSize The viewport size that is currently being applied.
                static void ConfigureSizes(const Vector2& OldVSize, const Vector2& NewVSize);
                /// @brief Sets the resizing and repositioning behaviors to be used when updated the viewport.
                /// @param SizeMode The behavior to be used when resizing.  See ResizeMode enum for more details.  Default: RSM_Scale_By_Height.
                /// @param PositionMode The behavior to be used when repositioning.  See RepositionMode enum for more details.  Default:  RPM_Scale_Quadrents_By_Height.
                static void ConfigureModes(const ResizeMode& SizeMode, const RepositionMode& PositionMode);
                /// @brief Sets the relative size of the screen that is to be considered the "middle".
                /// @param MidSize The relative size to be applied on each axis(X,Y) for checking which renderables are in the middle.
                /// @remarks In some Reposition modes(particularly the ones dealing with scale) renderables that are in or near the middle of the screen will not be altered.
                /// This is useful for renderables being used for things such as a Crosshair or a Jet HUD.  This function sets how wide the TOTAL area, centered in the middle
                /// of the screen is to be, and will detect renderables based on their center on that axis.  For example:  Setting this to 1.0 will consider everything in the middle.
                /// However setting this to 0.3 will consider every renderable with it's center in the range of 0.35 to 0.65 (on both the X and Y axes) to be in the middle and won't
                /// alter them.  Default: 0.4.
                static void ConfigureMiddleSize(const Real& MidSize);

                ///////////////////////////////////////////////////////////////////////////////
                // Information

                /// @brief Gets the previous size of the viewport prior to this update.
                /// @return Returns a const Vector2 reference containing the old viewport size.
                static const Vector2& GetOldSize();
                /// @brief Gets the new size of the viewport being applied.
                /// @return Returns a const Vector2 reference containing the new viewport size.
                static const Vector2& GetNewSize();
                /// @brief Gets the size of the area to be considered the "middle" of the screen when repositioning renderables in some modes.
                /// @return Returns a const Real reference indicating the size of the middle area.
                static const Real& GetMiddleSize();
                /// @brief Gets the currently set resize mode.
                /// @return Returns a const ResizeMode reference enum value indicating the behavior being used when resizing renderables.
                static const ResizeMode& GetResizeMode();
                /// @brief Gets the currently set reposition mode.
                /// @return Returns a const RepositionMode reference enum value indicating the behavior being used when repositioning renderables.
                static const RepositionMode& GetRepositionMode();

                ///////////////////////////////////////////////////////////////////////////////
                // Updating

                /// @brief Updates a renderables size and position as needed.
                /// @param ToBeUpdated The Renderable to be updated.
                static void UpdateRectangleRenderable(Rectangle* ToBeUpdated);
                /// @brief Updates a widgets position as needed.
                /// @param ToBeUpdated The Widget to be updated.
                static WidgetResult UpdateWidget(Widget* ToBeUpdated);
        };//ViewportUpdateTool
    }//UI
}//Mezzanine

#endif
