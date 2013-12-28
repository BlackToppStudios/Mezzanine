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
#ifndef _graphicsenumerations_h
#define _graphicsenumerations_h
///////////////////////////////////////////////////////////////////////////////
//Any global enumerations shared between multiple classes in the Graphics namespace is to be declared here.
///////////////////////////////////////

namespace Mezzanine
{
    namespace Graphics
    {
        /// @enum AspectRatio
        /// @brief This is used primarily by the GraphicsManager to keep track of the supported and currently used aspect ratios.
        enum AspectRatio
        {
            AR_3_By_2    = 1,   ///< 1.5
            AR_4_By_3    = 2,   ///< 1.334
            AR_5_By_3    = 3,   ///< 1.667
            AR_5_By_4    = 4,   ///< 1.25
            AR_8_By_5    = 5,   ///< 1.6
            AR_16_By_9   = 6,   ///< 1.778
            AR_17_By_9   = 7,   ///< 1.889
            AR_1_85_By_1 = 8,   ///< 1.85
            AR_2_39_By_1 = 9    ///< 2.39
        };

        /// @enum BillboardOrigin
        /// @brief This is used by the BillboardSetProxy to describe which part of the billboard will be treated as the local origin for placement and rotation.
        enum BillboardOrigin
        {
            BBO_Top_Left,        ///< Position/Rotate around the Top-Left corner of the billboard.
            BBO_Top_Center,      ///< Position/Rotate around the Top-Center of the billboard.
            BBO_Top_Right,       ///< Position/Rotate around the Top-Right corner of the billboard.
            BBO_Center_Left,     ///< Position/Rotate around the Center-Left side of the billboard.
            BBO_Center,          ///< Position/Rotate around the Center of the billboard.
            BBO_Center_Right,    ///< Position/Rotate around the Center-Right side of the billboard.
            BBO_Bottom_Left,     ///< Position/Rotate around the Bottom-Left corner of the billboard.
            BBO_Bottom_Center,   ///< Position/Rotate around the Bottom-Center of the billboard.
            BBO_Bottom_Right     ///< Position/Rotate around the Bottom-Right corner of the billboard.
        };

        /// @enum BillboardRotation
        /// @brief This is used by the BillboardSetProxy to decide how billboards should be rotated when they are requested to rotate.
        enum BillboardRotation
        {
            BBR_Vertex,        ///< Rotates the vertices of the billboard when rotated.
            BBR_TexureCoord    ///< Rotates the texture coordinates of the billboard when rotated, preserving the vertex positions.
        };

        /// @enum BillboardType
        /// @brief This is used by BillboardSetProxies to describe how the billboards are to be oriented.
        enum BillboardType
        {
            BBT_Point,                  ///< Default setting.  Billboard direction always faces the camera and is always oriented to be upright.
            BBT_Oriented_Common,        ///< Billboards are oriented around a common axis provided to the BillboardSetProxy, which will act as their local Y axis. Common Direction needs to be defined for this setting.
            BBT_Oriented_Self,          ///< Billboards are oriented around their own individually set direction axis, which will act as their local Y axis.
            BBT_Perpendicular_Common,   ///< Billboards are oriented around a common axis provided to the BillboardSetProxy, which will act as their local Z axis.  Common Direction and Common Up Vector needs to be defined for this setting.
            BBT_Perpendicular_Self      ///< Billboards are oriented around their own individually set direction axis, which will act as their local Z axis.  Common Up Vector needs to be defined for this setting.
        };

        /// @enum CameraPolyMode
        /// @brief This is used by CameraProxies to quickly set a different render mode that is useful for debugging graphics renders.
        enum CameraPolyMode
        {
            CPM_Points    = 1,        ///< Only points for each of the vertices are rendered.
            CPM_WireFrame = 2,        ///< Wireframes for all the meshes and geometry are rendered.
            CPM_Solid     = 3         ///< Normal rendering.
        };

        /// @enum LightType
        /// @brief This is used by LightProxies to describe how light is emitted from the proxy source.
        enum LightType
        {
            LT_Directional = 0,    ///< From one direction, like sunlight.
            LT_Point       = 1,    ///< From a point in space, like a Torch, campfire, muzzle flash, Mutant Fireflies, bonfires, light bulbs, non-hooded lantern, the DnD D20 Light spell, explosions, and scotch tape separating from the roll in a unlit vacuum. There may be other uses, be creative.
            LT_Spotlight   = 2     ///< From a point emanating in a cone, like a flashlight, hooded lantern, really bright computer screens, flood lights, older style space heaters, Concert lights, camera flashes, etc...
        };

        /// @enum OrientationMode
        /// @brief This is used by CameraProxies to determine the orientation of the camera frustrum.
        enum OrientationMode
        {
            OM_Degree_0       = 0,
            OM_Degree_90      = 1,
            OM_Degree_180     = 2,
            OM_Degree_270     = 3,

            OM_Portrait       = OM_Degree_0,
            OM_LandscapeRight = OM_Degree_90,
            OM_LandscapeLeft  = OM_Degree_270
        };

        /// @enum ProjectionType
        /// @brief Values for storing how perspective should be interpretted
        enum ProjectionType
        {
            PT_Orthographic    = 0,        ///< No projection is used, objects appear to be flat.
            PT_Perspective     = 1         ///< Normal (realistic) perspective.
        };

        /// @enum RenderSystem
        /// @brief Used primarily by the graphics manager class during initialization.
        /// @details This enum specifies which Rendersystem is to be used for rendering.
        enum RenderSystem
        {
            RS_Invalid   = 0,
            RS_DirectX9  = 1,
            RS_DirectX10 = 2,
            RS_DirectX11 = 3,
            RS_OpenGL2   = 4,
            RS_OpenGL3   = 5,
            RS_OpenGL4   = 6,
            RS_OpenGLES1 = 7,
            RS_OpenGLES2 = 8
        };
    }//Graphics
}//Mezzanine

#endif
