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
#ifndef _proceduraltexturetests_h
#define _proceduraltexturetests_h

#include "mezztest.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file performs all the basic testing and verification of the Mezzanine engine's ability to generate textures procedurally from code.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class proceduraltexturetests : public UnitTestGroup
{
public:
    /// @brief The number of pixels the palette image is wide.
    static const Whole PalettePixelWidth = 10;
    /// @brief The number of pixels the palette image is tall.
    static const Whole PalettePixelHeight = 10;
    /// @brief The number of total pixels in the palette image.
    static const Whole PalettePixelCount = proceduraltexturetests::PalettePixelWidth * proceduraltexturetests::PalettePixelHeight;
    /// @brief The format for the pixels in the palette image in memory.
    static const Graphics::PixelFormat PalettePixelFormat = Graphics::PF_R8G8B8A8;
    /// @brief Static array of pixels to compare against for procedural textures.
    static UInt8 PaletteImageArray[];

    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "ProceduralTexture"
    virtual String Name()
        { return String("ProceduralTexture"); }

    /// @brief Writes the contents of a texture buffer to an xml document.
    /// @param Buff The texture buffer to be converted to human readable XML.
    /// @param OutputFile The path and name of the file to write with the pixel contents of the image.
    void OutputImageAsText(const Graphics::Procedural::TextureBuffer& Buff, const String& OutputFile)
    {
        Whole BuffWidth = Buff.GetWidth();
        Whole BuffHeight = Buff.GetHeight();
        StringStream ImageStream;
        String FilePath(String("./")+OutputFile);
        ImageStream << "UInt8 ImageArray = { ";
        for( Whole Y = 0 ; Y < BuffHeight ; ++Y )
        {
            for( Whole X = 0 ; X < BuffWidth ; ++X )
            {
                ImageStream << std::endl << "    ";
                ImageStream << Whole( Buff.GetRedByte(X,Y) ) << ", "
                            << Whole( Buff.GetGreenByte(X,Y) ) <<  ", "
                            << Whole( Buff.GetBlueByte(X,Y) ) << ", "
                            << Whole( Buff.GetAlphaByte(X,Y) );
                if( !(X == ( BuffWidth - 1 ) && Y == ( BuffHeight - 1 ) ) ) {
                    ImageStream << ",";
                }
            }
        }
        ImageStream << " };";
        std::fstream ImageOutput;
        ImageOutput.open(FilePath.c_str(),std::fstream::out | std::fstream::trunc);
        ImageOutput << ImageStream.rdbuf();
        ImageOutput.close();
    }

    /// @brief Verifies two images are the same.
    /// @param Generated The image generated during the test.
    /// @param Loaded The image loaded from an external source for verification.
    /// @return Returns true if the two provided images match, false otherwise.
    Boole VerifyImage(const Graphics::Procedural::TextureBuffer& Generated, const Graphics::Procedural::TextureBuffer& Loaded)
    {
        Whole GeneratedWidth = Generated.GetWidth();
        Whole GeneratedHeight = Generated.GetHeight();
        Whole LoadedWidth = Loaded.GetWidth();
        Whole LoadedHeight = Loaded.GetHeight();

        if( GeneratedWidth != LoadedWidth || GeneratedHeight != LoadedHeight ) {
            return false;
        }

        for( Whole Y = 0 ; Y < GeneratedHeight ; ++Y )
        {
            for( Whole X = 0 ; X < GeneratedWidth ; ++X )
            {
                Boole RedMatch = Generated.GetRedByte(X,Y) == Loaded.GetRedByte(X,Y);
                Boole GreenMatch = Generated.GetGreenByte(X,Y) == Loaded.GetGreenByte(X,Y);
                Boole BlueMatch = Generated.GetBlueByte(X,Y) == Loaded.GetBlueByte(X,Y);
                Boole AlphaMatch = Generated.GetAlphaByte(X,Y) == Loaded.GetAlphaByte(X,Y);
                if( !RedMatch || !GreenMatch || !BlueMatch || !AlphaMatch ) {
                    return false;
                }
            }
        }

        return true;
    }

    /// @brief Convenience method to delete and NULL a pointer.
    /// @param ToClean The pointer to be "cleaned".
    void Clean(Graphics::Image* ToClean)
    {
        if( ToClean != NULL ) {
            delete ToClean;
            ToClean = NULL;
        }
    }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        // For now we have to initialize the Entresol.
        // In future versions of the Mezzanine we should be able to get away with just the graphics manager.
        // The purpose to doing this is to ensure the internal code to initialize the texture codecs are registered.
        Entresol TheEntresol;
        // Other data that will be needed.
        Whole SquareSize = 512;
        //Graphics::Image* TempImage = NULL;
        // Start by doing the generators in alphabetical order.
        {
            // Atlas Generator
        }

        {
            // Cell Generator
            /*TempImage = Graphics::Procedural::CellGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-CellGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image CellImage;
            CellImage._LoadImage("./data/images/ProceduralTexture-CellGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&CellImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "CellGenerator_Defaults");
        }

        {
            // Cloud Generator
            /*TempImage = Graphics::Procedural::CloudGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-CloudGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image CloudImage;
            CloudImage._LoadImage("./data/images/ProceduralTexture-CloudGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&CloudImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "CloudGenerator_Defaults");
        }

        {
            // Gradient Generator
            /*TempImage = Graphics::Procedural::GradientGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-GradientGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image GradientImage;
            GradientImage._LoadImage("./data/images/ProceduralTexture-GradientGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&GradientImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "GradientGenerator_Defaults");
        }

        {
            // Image Generator
            /*Graphics::Image PaletteImage("./data/images/ProceduralTexture-ImageGenTest.png","EngineData");
            Graphics::Procedural::TextureBuffer PaletteBuff(10,10);
            Graphics::Procedural::ImageGenerator Gen;
            Gen.SetImage(&PaletteImage);
            Gen.AddToTextureBuffer(PaletteBuff);
            this->OutputImageAsText(PaletteBuff,"./ProceduralTexture-PaletteOutput.txt");//*/

            // Get the image to compare to
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::PalettePixelWidth,proceduraltexturetests::PalettePixelHeight);
            for( Whole Y = 0 ; Y < proceduraltexturetests::PalettePixelHeight ; ++Y )
            {
                for( Whole X = 0 ; X < proceduraltexturetests::PalettePixelWidth ; ++X )
                {
                    Whole ChannelIndex = ( proceduraltexturetests::PalettePixelHeight * Y + X ) * 4;
                    LoadedBuffer.SetRedByte( X, Y, proceduraltexturetests::PaletteImageArray[ ChannelIndex + 0 ] );
                    LoadedBuffer.SetGreenByte( X, Y, proceduraltexturetests::PaletteImageArray[ ChannelIndex + 1 ] );
                    LoadedBuffer.SetBlueByte( X, Y, proceduraltexturetests::PaletteImageArray[ ChannelIndex + 2 ] );
                    LoadedBuffer.SetAlphaByte( X, Y, proceduraltexturetests::PaletteImageArray[ ChannelIndex + 3 ] );
                }
            }
            // Generate the image fresh
            Graphics::Image LoadedImage;
            LoadedImage._LoadImage("./data/images/ProceduralTexture-ImageGenTest.png");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::PalettePixelWidth,proceduraltexturetests::PalettePixelHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&LoadedImage).AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "ImageGenerator");//*/
        }

        {
            // Labyrinth Generator
            /*TempImage = Graphics::Procedural::LabyrinthGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-LabyrinthGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image LabyrinthImage;
            LabyrinthImage._LoadImage("./data/images/ProceduralTexture-LabyrinthGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&LabyrinthImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::LabyrinthGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "LabyrinthGenerator_Defaults");
        }

        {
            // Lerp Generator
            /*Graphics::Procedural::TextureBuffer FirstBuff(SquareSize);
            Graphics::Procedural::TextureBuffer SecondBuff(SquareSize);
            Graphics::Procedural::RectangleModifier().SetColour(ColourValue(1.0,0.0,1.0,1.0)).SetPosition(0.38,0.15,0.0,0.0).SetSize(0.24,0.70,0.0,0.0).Modify(FirstBuff);
            Graphics::Procedural::RectangleModifier().SetColour(ColourValue(0.0,1.0,1.0,1.0)).SetPosition(0.15,0.38,0.0,0.0).SetSize(0.70,0.24,0.0,0.0).Modify(SecondBuff);
            TempImage = Graphics::Procedural::LerpGenerator().SetFirstTexture(&FirstBuff).SetSecondTexture(&SecondBuff).GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-LerpGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image LerpImage;
            LerpImage._LoadImage("./data/images/ProceduralTexture-LerpGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&LerpImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TempBuffer1(SquareSize);
            Graphics::Procedural::TextureBuffer TempBuffer2(SquareSize);
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::RectangleModifier().SetColour(ColourValue(1.0,0.0,1.0,1.0)).SetPosition(0.38,0.15,0.0,0.0).SetSize(0.24,0.70,0.0,0.0).Modify(TempBuffer1);
            Graphics::Procedural::RectangleModifier().SetColour(ColourValue(0.0,1.0,1.0,1.0)).SetPosition(0.15,0.38,0.0,0.0).SetSize(0.70,0.24,0.0,0.0).Modify(TempBuffer2);
            Graphics::Procedural::LerpGenerator().SetFirstTexture(&TempBuffer1).SetSecondTexture(&TempBuffer2).AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "LerpGenerator_Defaults");
        }

        {
            // Marble Generator
            /*TempImage = Graphics::Procedural::MarbleGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-MarbleGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image MarbleImage;
            MarbleImage._LoadImage("./data/images/ProceduralTexture-MarbleGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&MarbleImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::MarbleGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "MarbleGenerator_Defaults");
        }

        {
            // Noise Generator
            /*TempImage = Graphics::Procedural::NoiseGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-NoiseGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image NoiseImage;
            NoiseImage._LoadImage("./data/images/ProceduralTexture-NoiseGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&NoiseImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::NoiseGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "NoiseGenerator_Defaults");
        }

        {
            // Solid Generator
            /*TempImage = Graphics::Procedural::SolidGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-SolidGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image SolidImage;
            SolidImage._LoadImage("./data/images/ProceduralTexture-SolidGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&SolidImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "SolidGenerator_Defaults");
        }

        {
            // Textile Generator
            /*TempImage = Graphics::Procedural::TextileGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-TextileGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TextileImage;
            TextileImage._LoadImage("./data/images/ProceduralTexture-TextileGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TextileImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextileGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "TextileGenerator_Defaults");
        }

        {
            // Wood Generator
            /*TempImage = Graphics::Procedural::WoodGenerator().GenerateImage(SquareSize);
            TempImage->SaveImageExplicit("./ProceduralTexture-WoodGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image WoodImage;
            WoodImage._LoadImage("./data/images/ProceduralTexture-WoodGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&WoodImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::WoodGenerator().AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "WoodGenerator_Defaults");
        }
        // Now do the modifiers
        {
            // Abnormals Modifier
        }

        {
            // AlphaMask Modifier
        }

        {
            // Alpha Modifier
        }

        {
            // Blit Modifier
        }

        {
            // Blur Modifier
        }

        {
            // Channel Modifier
        }

        {
            // Circle Modifier
        }

        {
            // Colours Modifier
        }

        {
            // Combine Modifier
        }

        {
            // Convolution Modifier
        }

        {
            // Crack Modifier
        }

        {
            // Cycloid Modifier
        }

        {
            // Dilate Modifier
        }

        {
            // Edged Detection Modifier
        }

        {
            // Ellipse Modifier
        }

        {
            // Flare Modifier
        }

        {
            // Invert Modifier
        }

        {
            // Jitter Modifier
        }

        {
            // Lookup Modifier
        }

        {
            // Normals Modifier
        }

        {
            // Oilpaint Modifier
        }

        {
            // Random Pixels Modifier
        }

        {
            // Rectangle Modifier
        }

        {
            // Rotation Zoom Modifier
        }

        {
            // Segment Modifier
        }

        {
            // Sharpen Modifier
        }

        {
            // TextTexture Modifier
        }

        {
            // Threshold Modifier
        }

        {
            // Vortex Modifier
        }

        {
            // Weighed Lerp Modifier
        }
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual Boole HasAutomaticTests() const
        { return true; }
};//proceduraltexturetests

UInt8 proceduraltexturetests::PaletteImageArray[] = {
        0, 0, 0, 255,
        64, 64, 64, 255,
        255, 0, 0, 255,
        255, 106, 0, 255,
        255, 216, 0, 255,
        182, 255, 0, 255,
        76, 255, 0, 255,
        0, 255, 33, 255,
        0, 255, 144, 255,
        0, 255, 255, 255,
        0, 148, 255, 255,
        0, 38, 255, 255,
        72, 0, 255, 255,
        178, 0, 255, 255,
        255, 0, 220, 255,
        255, 0, 110, 255,
        255, 255, 255, 255,
        128, 128, 128, 255,
        127, 0, 0, 255,
        127, 51, 0, 255,
        127, 106, 0, 255,
        91, 127, 0, 255,
        38, 127, 0, 255,
        0, 127, 70, 255,
        0, 127, 127, 255,
        0, 74, 127, 255,
        0, 19, 127, 255,
        33, 0, 127, 255,
        87, 0, 127, 255,
        127, 0, 110, 255,
        127, 0, 55, 255,
        160, 160, 160, 255,
        48, 48, 48, 255,
        255, 127, 127, 255,
        255, 178, 127, 255,
        255, 233, 127, 255,
        165, 255, 127, 255,
        127, 255, 142, 255,
        127, 255, 197, 255,
        127, 255, 255, 255,
        127, 201, 255, 255,
        127, 146, 255, 255,
        161, 127, 255, 255,
        214, 127, 255, 255,
        255, 127, 237, 255,
        255, 127, 182, 255,
        192, 192, 192, 255,
        96, 96, 96, 255,
        127, 63, 63, 255,
        127, 89, 63, 255,
        127, 116, 63, 255,
        109, 127, 63, 255,
        82, 127, 63, 255,
        63, 127, 71, 255,
        63, 127, 98, 255,
        63, 127, 127, 255,
        63, 100, 127, 255,
        63, 73, 127, 255,
        80, 63, 127, 255,
        107, 63, 127, 255,
        127, 63, 118, 255,
        127, 63, 91, 255,
        255, 99, 56, 255,
        235, 255, 61, 255,
        107, 255, 215, 255,
        255, 135, 175, 255,
        212, 58, 255, 255,
        45, 255, 112, 255,
        164, 255, 73, 255,
        99, 164, 255, 255,
        255, 158, 56, 255,
        255, 104, 117, 255,
        227, 104, 255, 255,
        255, 125, 96, 255,
        196, 255, 119, 255,
        255, 96, 61, 255,
        97, 76, 255, 255,
        255, 201, 56, 255,
        255, 158, 238, 255,
        86, 255, 224, 255,
        149, 147, 255, 255,
        147, 255, 197, 255,
        112, 107, 255, 255,
        84, 255, 112, 255,
        155, 216, 255, 255,
        154, 12, 255, 255,
        255, 7, 85, 255,
        106, 255, 76, 255,
        174, 127, 255, 255,
        255, 144, 96, 255,
        20, 255, 71, 255,
        127, 255, 0, 255,
        255, 81, 182, 255,
        255, 165, 0, 255,
        89, 94, 255, 255,
        255, 153, 119, 255,
        0, 255, 195, 255,
        245, 255, 109, 255,
        0, 255, 46, 255,
        255, 55, 0, 255 };

#endif
