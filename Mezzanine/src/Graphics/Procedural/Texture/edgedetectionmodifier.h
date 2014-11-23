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
/*
 -----------------------------------------------------------------------------
 This source file is part of ogre-procedural

 For the latest info, see http://code.google.com/p/ogre-procedural/

 Copyright (c) 2010-2013 Michael Broutin

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */
#ifndef _graphicsproceduraledgedetectionmodifier_h
#define _graphicsproceduraledgedetectionmodifier_h

#include "Graphics/Procedural/Texture/texturemodifier.h"

#include "vector3.h"

namespace Mezzanine
{
    namespace Graphics
    {
        namespace Procedural
        {
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief A modifier that will reduce the texture to an outline of the detected edges in the texture.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB EdgeDetectionModifier : public TextureModifier
            {
            public:
                /// @enum DetectionMode
                /// @brief An enum describing which algorithm to use for edge detection.
                enum DetectionMode
                {
                    DM_Homogenity     = 1,   ///< Detect edges based on difference in colour between the pixel being processed and all surrounding pixels.
                    DM_Difference     = 2,   ///< Detect edges based on difference in colour between opposite surrounding pixels.
                    DM_Sobel          = 3,   ///< Use the Sobel Operator algorithm to detect edges.
                    DM_Canny          = 4    ///< Use the Canny multi-stage algorthm to detect edges.
                };

                /// @typedef VectorContainer
                /// @brief Convenience typedef for an array of Vector3's.
                typedef std::vector<Vector3> VectorContainer;
                /// @typedef VectorBlock
                /// @brief Convenience typedef for returning a 3x3 matrix of Vector3's containing colour data.
                typedef VectorContainer VectorBlock;
            protected:
                /// @internal
                /// @brief The algorithm to use to detect edges.
                DetectionMode EdgeDetect;
                /// @internal
                /// @brief The lowest value for each colour channel that will be considered valid.
                UInt8 LowerThreshold;
                /// @internal
                /// @brief The highest value for each colour channel that will be considered valid.
                UInt8 UpperThreshold;
                /// @internal
                /// @brief The sigma to set for the blur step of the Canny filter.
                UInt8 EdgeDetectionSigma;

                /// @internal
                /// @brief Detect edges in the provided texture by processing all surrounding pixels.
                /// @param Buffer The texture buffer to be rendered to.
                void ProcessHomogenity(TextureBuffer& Buffer);
                /// @internal
                /// @brief Detect edges in the provided texture by processing pixels above and below the current pixel.
                /// @param Buffer The texture buffer to be rendered to.
                void ProcessDifference(TextureBuffer& Buffer);
                /// @internal
                /// @brief Detect edges using the Sobel Operator.
                /// @param Buffer The texture buffer to be rendered to.
                void ProcessSobel(TextureBuffer& Buffer);
                /// @internal
                /// @brief Detect edges using the Canny multi-stage algorithm.
                /// @param Buffer The texture buffer to be rendered to.
                void ProcessCanny(TextureBuffer& Buffer);
                /// @internal
                /// @brief Gets the colours of the specified pixel and all surrounding pixels.
                /// @param X The X coordinate of the center pixel to retrieve.
                /// @param Y The Y coordinate of the center pixel to retrieve.
                /// @param Buffer The texture buffer to retrieve pixel data from.
                /// @return Returns a pointer to an array of Vector3's that are the colours of the current and surrounding pixels.
                VectorBlock GetBlock(const Integer X, const Integer Y, TextureBuffer& Buffer) const;
            public:
                /// @brief Blank constructor.
                EdgeDetectionModifier();
                /// @brief Class destructor.
                virtual ~EdgeDetectionModifier();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc TextureModifier::Modify(TextureBuffer&)
                virtual void Modify(TextureBuffer& Buffer);
                /// @copydoc TextureModifier::GetName() const
                virtual String GetName() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Configuration

                /// @brief Sets the detection algorith to use for detecting edges.
                /// @param Mode The detection algorith to use.
                /// @return Returns a reference to this.
                EdgeDetectionModifier& SetDetectionType(const DetectionMode Mode);
                /// @brief Sets the lower threshold for the Canny filter.
                /// @remarks This variable is only used if DM_Canny is set as the detection method.
                /// @param Low The lowest value for each colour channel that will be considered valid.
                /// @return Returns a reference to this.
                EdgeDetectionModifier& SetLowerThreshold(const UInt8 Low);
                /// @brief Sets the upper threshold for the Canny filter.
                /// @remarks This variable is only used if DM_Canny is set as the detection method.
                /// @param High The highest value for each colour channel that will be considered valid.
                /// @return Returns a reference to this.
                EdgeDetectionModifier& SetUpperThreshold(const UInt8 High);
                /// @brief Sets the sigma for the blur stage of the Canny filter.
                /// @remarks This variable is only used if DM_Canny is set as the detection method.
                /// @param Sigma The sigma to set for the blur step of the Canny filter.
                /// @return Returns a reference to this.
                EdgeDetectionModifier& SetEdgeDetectionSigma(const UInt8 Sigma);
            };//EdgeDetectionModifier
        }//Procedural
    }//Graphics
}//Mezzanine

#endif
