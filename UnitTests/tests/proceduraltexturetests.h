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
    /// @brief The width of the external image used to perform some of the tests.
    static const Whole SampleWidth = 615;
    /// @brief The height of the external image used to perform some of the tests.
    static const Whole SampleHeight = 410;
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
        Graphics::Image* TempImage = NULL;
        // Start by doing the generators in alphabetical order.
        {
            // Atlas Generator
        }

        {
            // Cell Generator - Grid
            /*TempImage = Graphics::Procedural::CellGenerator().SetMode(Graphics::Procedural::CellGenerator::CM_Grid).GenerateImage(SquareSize);
            TempImage->_SaveImage("./ProceduralTexture-CellGridGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image CellImage;
            CellImage._LoadImage("./data/images/ProceduralTexture-CellGridGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&CellImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetMode(Graphics::Procedural::CellGenerator::CM_Grid).AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "CellGenerator_Grid");
        }

        {
            // Cell Generator - Chessboard
            /*TempImage = Graphics::Procedural::CellGenerator().SetMode(Graphics::Procedural::CellGenerator::CM_Chessboard).GenerateImage(SquareSize);
            TempImage->_SaveImage("./ProceduralTexture-CellChessboardGenTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image CellImage;
            CellImage._LoadImage("./data/images/ProceduralTexture-CellChessboardGenTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&CellImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetMode(Graphics::Procedural::CellGenerator::CM_Chessboard).AddToTextureBuffer(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "CellGenerator_Chessboard");
        }

        {
            // Cloud Generator
            /*TempImage = Graphics::Procedural::CloudGenerator().GenerateImage(SquareSize);
            TempImage->_SaveImage("./ProceduralTexture-CloudGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-GradientGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-LabyrinthGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-LerpGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-MarbleGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-NoiseGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-SolidGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-TextileGenTest.png");
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
            TempImage->_SaveImage("./ProceduralTexture-WoodGenTest.png");
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
            // Abnormals Modifier - Suspect
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::AbnormalsModifier().SetAbnormalsTexture(&ParameterBuffer).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-AbnormalsModTest.png");
            this->Clean(TempImage);//*/
        }

        {
            // AlphaMask Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::AlphaMaskModifier().SetMaskTexture(&ParameterBuffer).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-AlphaMaskModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-AlphaMaskModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::AlphaMaskModifier().SetMaskTexture(&ParameterBuffer).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "AlphaMaskModifier_Defaults");
        }

        {
            // Alpha Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::AlphaModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-AlphaModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-AlphaModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::AlphaModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "AlphaModifier_Defaults");
        }

        {
            // Blit Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer SrcBuffer(SquareSize * 0.5);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(SrcBuffer);
            Graphics::Procedural::BlitModifier().SetSrcTexture(&SrcBuffer).SetSrcRect(0,0,SquareSize * 0.5,SquareSize * 0.5).SetDestRect(128,128,388,388).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-BlitModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-BlitModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer SrcBuffer(SquareSize * 0.5);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(SrcBuffer);
            Graphics::Procedural::BlitModifier().SetSrcTexture(&SrcBuffer).SetSrcRect(0,0,SquareSize * 0.5,SquareSize * 0.5).SetDestRect(128,128,388,388).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "BlitModifier_Defaults");
        }

        {
            // Blur Modifier
            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::BlurModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-BlurModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-BlurModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::BlurModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "BlurModifier_Defaults");
        }

        {
            // Channel Modifier
            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Whole Selection = Graphics::Procedural::ChannelModifier::CS_Green;
            Graphics::Procedural::ChannelModifier().SetSelection(Selection).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-ChannelModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-ChannelModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Whole Selection = Graphics::Procedural::ChannelModifier::CS_Green;
            Graphics::Procedural::ChannelModifier().SetSelection(Selection).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "ChannelModifier_NoGreen");
        }

        {
            // Circle Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CircleModifier().SetColour(1.0,0.0,0.0,1.0).SetPosition(0.5,0.5,0.0,0.0).SetRadius(SquareSize * 0.25).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CircleModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CircleModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CircleModifier().SetColour(1.0,0.0,0.0,1.0).SetPosition(0.5,0.5,0.0,0.0).SetRadius(SquareSize * 0.25).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "CircleModifier_Defaults");
        }

        {
            // Colours Modifier - Saturation
            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::ColoursModifier().SetSaturation(255).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-ColoursSaturationModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-ColoursSaturationModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::ColoursModifier().SetSaturation(255).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "ColoursModifier_Saturation");
        }

        {
            // Combine Modifier - AddClamp
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer TexBuffer1(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TexBuffer1);
            Graphics::Procedural::CombineModifier().AddTexture(&TexBuffer1,Graphics::Procedural::CombineModifier::CM_Add_Clamp).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CombineAddClampModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CombineAddClampModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer TexBuffer1(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TexBuffer1);
            Graphics::Procedural::CombineModifier().AddTexture(&TexBuffer1,Graphics::Procedural::CombineModifier::CM_Add_Clamp).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "CombineModifier_AddClamp");
        }

        {
            // Convolution Modifier - Motion Blur
            // This kernel simulates a motion blur with a camera movement from the top left to the bottom right.
            Integer Kernel[] = {
                1, 0, 0, 0, 0, 0, 0 , 0, 0,
                0, 1, 0, 0, 0, 0, 0 , 0, 0,
                0, 0, 1, 0, 0, 0, 0 , 0, 0,
                0, 0, 0, 1, 0, 0, 0 , 0, 0,
                0, 0, 0, 0, 1, 0, 0 , 0, 0,
                0, 0, 0, 0, 0, 1, 0 , 0, 0,
                0, 0, 0, 0, 0, 0, 1 , 0, 0,
                0, 0, 0, 0, 0, 0, 0 , 1, 0,
                0, 0, 0, 0, 0, 0, 0 , 0, 1,
            };

            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::ConvolutionModifier().SetKernel(9,&Kernel[0]).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-ConvolutionModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-ConvolutionModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::ConvolutionModifier().SetKernel(9,&Kernel[0]).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Convolution_MotionBlur");
        }

        {
            // Crack Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::CrackModifier().SetNormalsTexture(&ParameterBuffer).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CrackModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CrackModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::CrackModifier().SetNormalsTexture(&ParameterBuffer).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Crack_Defaults");
        }

        {
            // Cycloid Modifier - Hypocycloid
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Hypocycloid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CycloidHypocycloidModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CycloidHypocycloidModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Hypocycloid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Cycloid_Hypocycloid");
        }

        {
            // Cycloid Modifier - Hypotrochoid
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Hypotrochoid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CycloidHypotrochoidModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CycloidHypotrochoidModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Hypotrochoid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Cycloid_Hypotrochoid");
        }

        {
            // Cycloid Modifier - Epicycloid
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Epicycloid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CycloidEpicycloidModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CycloidEpicycloidModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Epicycloid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Cycloid_Epicycloid");
        }

        {
            // Cycloid Modifier - Epitrochoid
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Epitrochoid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CycloidEpitrochoidModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CycloidEpitrochoidModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_Epitrochoid).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Cycloid_Epitrochoid");
        }

        {
            // Cycloid Modifier - RoseCurve
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_RoseCurve).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CycloidRoseCurveModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CycloidRoseCurveModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_RoseCurve).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Cycloid_RoseCurve");
        }

        {
            // Cycloid Modifier - LissajousCurve
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_LissajousCurve).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-CycloidLissajousCurveModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-CycloidLissajousCurveModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CycloidModifier().SetColour(1.0,0.2,0.0,1.0).SetCycloidPenSize(3).SetCycloidType(Graphics::Procedural::CycloidModifier::CT_LissajousCurve).SetDefaultParameters(SquareSize).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Cycloid_LissajousCurve");
        }

        {
            // Dilate Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::DilateModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-DilateModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-DilateModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::DilateModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Dilate_Defaults");
        }

        {
            // Edge Detection Modifier - Sobel
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::EdgeDetectionModifier().SetDetectionType(Graphics::Procedural::EdgeDetectionModifier::DM_Sobel).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-EdgeDetectionSobelModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-EdgeDetectionSobelModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::EdgeDetectionModifier().SetDetectionType(Graphics::Procedural::EdgeDetectionModifier::DM_Sobel).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "EdgeDetection_Sobel");
        }

        {
            // Ellipse Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::EllipseModifier().SetColour(ColourValue(1.0,0.0,0.0,1.0)).SetPosition(0.5,0.5,0,0).SetRadius(0.4,0.2,0,0).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-EllipseModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-EllipseModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::EllipseModifier().SetColour(ColourValue(1.0,0.0,0.0,1.0)).SetPosition(0.5,0.5,0,0).SetRadius(0.4,0.2,0,0).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Ellipse_Defaults");
        }

        {
            // Flare Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::FlareModifier().SetFlareCenter(0.5,0.5).SetFlareRadius(0.4,0.4).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-FlareModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-FlareModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::FlareModifier().SetFlareCenter(0.5,0.5).SetFlareRadius(0.4,0.4).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Flare_Defaults");
        }

        {
            // Flip Modifier - Point
            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::FlipModifier().SetFlipAxis(Graphics::Procedural::FlipModifier::PFA_Point).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-FlipPointModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-FlipPointModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::FlipModifier().SetFlipAxis(Graphics::Procedural::FlipModifier::PFA_Point).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Flip_Defaults");
        }

        {
            // Invert Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::InvertModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-InvertModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-InvertModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::InvertModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Invert_Defaults");
        }

        {
            // Jitter Modifier
            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::JitterModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-JitterModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-JitterModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::JitterModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Jitter_Defaults");
        }

        {
            // Lookup Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::LookupModifier().SetLookupTexture(&ParameterBuffer).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-LookupModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-LookupModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::LookupModifier().SetLookupTexture(&ParameterBuffer).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Lookup_Defaults");
        }

        {
            // Normals Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::NormalsModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-NormalsModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-NormalsModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::NormalsModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Normals_Defaults");
        }

        {
            // Oilpaint Modifier
            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::OilPaintModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-OilpaintModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-OilpaintModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::OilPaintModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Oilpaint_Defaults");
        }

        {
            // Random Pixels Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::RandomPixelsModifier().SetColour(ColourValue(1.0,0.2,0.0,1.0)).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-RandomPixelsModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-RandomPixelsModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::SolidGenerator().SetColour(0.6,0.8,1.0,1.0).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::RandomPixelsModifier().SetColour(ColourValue(1.0,0.2,0.0,1.0)).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "RandomPixels_Defaults");
        }

        {
            // Rectangle Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::RectangleModifier().SetColour(ColourValue(1.0,0.0,0.0,1.0)).SetPosition(0.2,0.3,0,0).SetSize(0.6,0.4,0,0).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-RectangleModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-RectangleModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::RectangleModifier().SetColour(ColourValue(1.0,0.0,0.0,1.0)).SetPosition(0.2,0.3,0,0).SetSize(0.6,0.4,0,0).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Rectangle_Defaults");
        }

        {
            // Rotation Zoom Modifier - Rotation Only
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::RotationZoomModifier().SetRotation(MathTools::GetPi() * 0.25).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-RotationZoomRotationOnlyModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-RotationZoomRotationOnlyModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::RotationZoomModifier().SetRotation(MathTools::GetPi() * 0.25).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "RotationZoom_RotationOnly");
        }

        {
            // Segment Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::SegmentModifier().SetColourSource(&ParameterBuffer).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-SegmentModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-SegmentModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer ParameterBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(ParameterBuffer);
            Graphics::Procedural::SegmentModifier().SetColourSource(&ParameterBuffer).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Segment_Defaults");
        }

        {
            // Sharpen Modifier
            /*Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::SharpenModifier().Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-SharpenModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-SharpenModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Image SampleImage;
            SampleImage._LoadImage("./data/images/FlowerGarden-PublicDomain.jpg");
            Graphics::Procedural::TextureBuffer TestBuffer(proceduraltexturetests::SampleWidth,proceduraltexturetests::SampleHeight);
            Graphics::Procedural::ImageGenerator().SetImage(&SampleImage).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::SharpenModifier().Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Sharpen_Defaults");
        }

        {
            // TextTexture Modifier
        }

        {
            // Threshold Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::ThresholdModifier().SetLowerLimit(0.5).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-ThresholdModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-ThresholdModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::ThresholdModifier().SetLowerLimit(0.5).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Threshold_Defaults");
        }

        {
            // Vortex Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::VortexModifier().SetVortexCenter(0.5,0.5).SetVortexRadius(0.5,0.5).SetVortexTwist(MathTools::GetHalfPi()).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-VortexModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-VortexModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::VortexModifier().SetVortexCenter(0.5,0.5).SetVortexRadius(0.5,0.5).SetVortexTwist(MathTools::GetHalfPi()).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "Vortex_Defaults");
        }

        {
            // Weighed Lerp Modifier
            /*Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer FirstParamBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer SecondParamBuffer(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(FirstParamBuffer);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(SecondParamBuffer);
            Graphics::Procedural::WeighedLerpModifier().SetFirstTexture(&FirstParamBuffer).SetSecondTexture(&SecondParamBuffer).Modify(TestBuffer);
            TempImage = TestBuffer.GenerateImage();
            TempImage->_SaveImage("./ProceduralTexture-WeighedLerpModTest.png");
            this->Clean(TempImage);//*/

            // Get the image to compare to
            Graphics::Image TestImage;
            TestImage._LoadImage("./data/images/ProceduralTexture-WeighedLerpModTest.png");
            Graphics::Procedural::TextureBuffer LoadedBuffer(SquareSize);
            Graphics::Procedural::ImageGenerator().SetImage(&TestImage).AddToTextureBuffer(LoadedBuffer);
            // Generate the image fresh
            Graphics::Procedural::TextureBuffer TestBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer FirstParamBuffer(SquareSize);
            Graphics::Procedural::TextureBuffer SecondParamBuffer(SquareSize);
            Graphics::Procedural::CloudGenerator().AddToTextureBuffer(TestBuffer);
            Graphics::Procedural::GradientGenerator().AddToTextureBuffer(FirstParamBuffer);
            Graphics::Procedural::CellGenerator().SetDensity(4).SetRegularity(234).AddToTextureBuffer(SecondParamBuffer);
            Graphics::Procedural::WeighedLerpModifier().SetFirstTexture(&FirstParamBuffer).SetSecondTexture(&SecondParamBuffer).Modify(TestBuffer);
            // Do the test
            TEST(this->VerifyImage(TestBuffer,LoadedBuffer), "WeighedLerp_Defaults");
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
