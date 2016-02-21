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
#ifndef _graphicsprocedural_h
#define _graphicsprocedural_h

namespace Mezzanine
{
    namespace Graphics
    {
        /// @namespace Mezzanine::Graphics::Procedural
        /// @brief This namespace is for all the classes that facilitate the procedural creation of art assets.
        /// @details Our Procedural Graphics system is based on the OgreProcedural library.
        namespace Procedural
        {

        }//Procedural
    }//Graphics
}//Mezzanine

#include "Graphics/Procedural/proceduraldatatypes.h"
#include "Graphics/Procedural/proceduralenumerations.h"
#include "Graphics/Procedural/proceduralutilities.h"

#include "Graphics/Procedural/multishape.h"
#include "Graphics/Procedural/path.h"
#include "Graphics/Procedural/shape.h"
#include "Graphics/Procedural/triangulator.h"

#include "Graphics/Procedural/Mesh/booleanmeshgenerator.h"
#include "Graphics/Procedural/Mesh/boxcornergenerator.h"
#include "Graphics/Procedural/Mesh/boxgenerator.h"
#include "Graphics/Procedural/Mesh/boxuvmodifier.h"
#include "Graphics/Procedural/Mesh/capsulegenerator.h"
#include "Graphics/Procedural/Mesh/conegenerator.h"
#include "Graphics/Procedural/Mesh/cylindergenerator.h"
#include "Graphics/Procedural/Mesh/cylinderuvmodifier.h"
#include "Graphics/Procedural/Mesh/extruder.h"
#include "Graphics/Procedural/Mesh/hemisphereuvmodifier.h"
#include "Graphics/Procedural/Mesh/icospheregenerator.h"
#include "Graphics/Procedural/Mesh/lathe.h"
#include "Graphics/Procedural/Mesh/meshgenerator.h"
#include "Graphics/Procedural/Mesh/planegenerator.h"
#include "Graphics/Procedural/Mesh/planeuvmodifier.h"
#include "Graphics/Procedural/Mesh/roundedboxgenerator.h"
#include "Graphics/Procedural/Mesh/spheregenerator.h"
#include "Graphics/Procedural/Mesh/sphereuvmodifier.h"
#include "Graphics/Procedural/Mesh/spherifymodifier.h"
#include "Graphics/Procedural/Mesh/springgenerator.h"
#include "Graphics/Procedural/Mesh/torusgenerator.h"
#include "Graphics/Procedural/Mesh/torusknotgenerator.h"
#include "Graphics/Procedural/Mesh/trianglebuffer.h"
#include "Graphics/Procedural/Mesh/tubegenerator.h"
#include "Graphics/Procedural/Mesh/vertexnormalsmodifier.h"
#include "Graphics/Procedural/Mesh/vertextransformmodifier.h"
#include "Graphics/Procedural/Mesh/vertexuvmodifier.h"
#include "Graphics/Procedural/Mesh/weldverticesmodifier.h"

#include "Graphics/Procedural/Texture/alphamodifier.h"
#include "Graphics/Procedural/Texture/alphamaskmodifier.h"
#include "Graphics/Procedural/Texture/atlasgenerator.h"
#include "Graphics/Procedural/Texture/blitmodifier.h"
#include "Graphics/Procedural/Texture/blurmodifier.h"
#include "Graphics/Procedural/Texture/cellgenerator.h"
#include "Graphics/Procedural/Texture/channelmodifier.h"
#include "Graphics/Procedural/Texture/circlemodifier.h"
#include "Graphics/Procedural/Texture/cloudgenerator.h"
#include "Graphics/Procedural/Texture/coloursmodifier.h"
#include "Graphics/Procedural/Texture/combinemodifier.h"
#include "Graphics/Procedural/Texture/convolutionmodifier.h"
#include "Graphics/Procedural/Texture/crackmodifier.h"
#include "Graphics/Procedural/Texture/cycloidmodifier.h"
#include "Graphics/Procedural/Texture/dilatemodifier.h"
#include "Graphics/Procedural/Texture/edgedetectionmodifier.h"
#include "Graphics/Procedural/Texture/ellipsemodifier.h"
#include "Graphics/Procedural/Texture/flaremodifier.h"
#include "Graphics/Procedural/Texture/flipmodifier.h"
#include "Graphics/Procedural/Texture/gradientgenerator.h"
#include "Graphics/Procedural/Texture/imagegenerator.h"
#include "Graphics/Procedural/Texture/invertmodifier.h"
#include "Graphics/Procedural/Texture/jittermodifier.h"
#include "Graphics/Procedural/Texture/labyrinthgenerator.h"
#include "Graphics/Procedural/Texture/lerpgenerator.h"
#include "Graphics/Procedural/Texture/lookupmodifier.h"
#include "Graphics/Procedural/Texture/marblegenerator.h"
#include "Graphics/Procedural/Texture/noisegenerator.h"
#include "Graphics/Procedural/Texture/normalsmodifier.h"
#include "Graphics/Procedural/Texture/oilpaintmodifier.h"
#include "Graphics/Procedural/Texture/randompixelsmodifier.h"
#include "Graphics/Procedural/Texture/rectanglemodifier.h"
#include "Graphics/Procedural/Texture/rotationzoommodifier.h"
#include "Graphics/Procedural/Texture/segmentmodifier.h"
#include "Graphics/Procedural/Texture/sharpenmodifier.h"
#include "Graphics/Procedural/Texture/solidgenerator.h"
#include "Graphics/Procedural/Texture/textilegenerator.h"
#include "Graphics/Procedural/Texture/texttexturemodifier.h"
#include "Graphics/Procedural/Texture/texturebuffer.h"
#include "Graphics/Procedural/Texture/texturegenerator.h"
#include "Graphics/Procedural/Texture/texturemodifier.h"
#include "Graphics/Procedural/Texture/thresholdmodifier.h"
#include "Graphics/Procedural/Texture/vortexmodifier.h"
#include "Graphics/Procedural/Texture/weighedlerpmodifier.h"
#include "Graphics/Procedural/Texture/woodgenerator.h"

#endif
