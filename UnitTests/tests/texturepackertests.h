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
#ifndef _texturepackertests_h
#define _texturepackertests_h

#include "mezztest.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file performs all the basic testing and verification of the Graphics::TexturePacker derived classes.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A simple dummy callback used for bulk image packing.
class DummyPackHandler : public Graphics::PackResultHandler
{
    /// @copydoc Graphics::ProcessTexturePlacement(const ImageIDPair&, const PlacementResult&)
    void ProcessTexturePlacement(const Graphics::ImageIDPair& ImgPair, const PlacementResult& PlacedRect)
    {
        std::cout << "Image: " << ImgPair.ImgName <<
                      " Size: " << PlacedRect.first.GetSize().Width << "x" << PlacedRect.first.GetSize().Height <<
                      ", placed at"
                      " L:" << PlacedRect.first.Left <<
                      " T:" << PlacedRect.first.Top <<
                      " R:" << PlacedRect.first.Right <<
                      " B:" << PlacedRect.first.Bottom << std::endl;
    }
};//DummyPackHandler

/// @brief A series of tests to verify basic functionality of Graphics::TexturePacker derived classes.
class texturepackertests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "TexturePacker"
    virtual String Name()
        { return String("TexturePacker"); }

    /// @brief Creates a fresh image with each pixel being a single colour.
    /// @param Width The width of the image in pixels.
    /// @param Height The height of the image in pixels.
    /// @param Colour The colour to fill the image with.
    /// @return Returns the created Image.  Auto-delete is not enabled on the return.
    Graphics::Image* CreateFillImage(const Whole Width, const Whole Height, const ColourValue& Colour)
    {
        Whole NumPixels = Width * Height;
        Whole NumChannels = NumPixels * 4;
        UInt8* ImageBuf = new UInt8[NumChannels];
        for( Whole CurrChannel = 0 ; CurrChannel < NumChannels ; CurrChannel += 4 )
        {
        #ifdef MEZZ_BIG_ENDIAN
            ImageBuf[CurrChannel + 0] = static_cast<UInt8>( 255 * Colour.RedChannel );
            ImageBuf[CurrChannel + 1] = static_cast<UInt8>( 255 * Colour.GreenChannel );
            ImageBuf[CurrChannel + 2] = static_cast<UInt8>( 255 * Colour.BlueChannel );
            ImageBuf[CurrChannel + 3] = static_cast<UInt8>( 255 * Colour.AlphaChannel );
        #endif
        #ifdef MEZZ_LITTLE_ENDIAN
            ImageBuf[CurrChannel + 0] = static_cast<UInt8>( 255 * Colour.AlphaChannel );
            ImageBuf[CurrChannel + 1] = static_cast<UInt8>( 255 * Colour.BlueChannel );
            ImageBuf[CurrChannel + 2] = static_cast<UInt8>( 255 * Colour.GreenChannel );
            ImageBuf[CurrChannel + 3] = static_cast<UInt8>( 255 * Colour.RedChannel );
        #endif
        }
        Graphics::Image* Filled = new Graphics::Image(ImageBuf,Width,Height,Graphics::PF_R8G8B8A8);
        return Filled;
    }

    /// @brief Verifies that two images are the same.
    /// @param Generated A pointer to the generated image.
    /// @param Loaded A pointer to the reference image loaded from disk.
    /// @return Returns true if the two images are the same, false otherwise.
    Boole VerifyImage(const Graphics::Image* Generated, const Graphics::Image* Loaded)
    {
        const Whole Width = Loaded->GetWidth();
        const Whole Height = Loaded->GetHeight();
        const Whole Depth = Loaded->GetDepth();
        if( Generated->GetWidth() != Width || Generated->GetHeight() != Height ||
            Generated->GetDepth() != Depth || Depth != 1 )
        {
            return false;
        }

        for( Whole YPix = 0 ; YPix < Height ; ++YPix )
        {
            for( Whole XPix = 0 ; XPix < Width ; ++XPix )
            {
                ColourValue GeneratedColour = Generated->GetColourAt(XPix,YPix,0);
                ColourValue LoadedColour = Generated->GetColourAt(XPix,YPix,0);
                if( GeneratedColour != LoadedColour ) {
                    return false;
                }
            }
        }
        return true;
    }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        // Entresol being created exclusively for the registering of image codecs.
        Entresol TheEntresol;
        // Convenience string for loading images
        const String ImagePath("./data/images/");

        DummyPackHandler Dummy;
        Graphics::TexturePacker::ImageContainer TestImages;
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(64,64,ColourValue::DarkOliveGreen()), "01-DarkOliveGreen" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(128,128,ColourValue::GoldenRod()), "02-GoldenRod" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(96,96,ColourValue::Yellow()), "03-Yellow" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(256,128,ColourValue::LightSeaGreen()), "04-LightSeaGreen" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(64,96,ColourValue::MidnightBlue()), "05-MidnightBlue" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(256,64,ColourValue::Maroon()), "06-Maroon" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(128,96,ColourValue::RosyBrown()), "07-RosyBrown" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(64,128,ColourValue::SkyBlue()), "08-SkyBlue" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(192,128,ColourValue::YellowGreen()), "09-YellowGreen" ) );
        TestImages.push_back( Graphics::ImageIDPair( this->CreateFillImage(192,64,ColourValue::PeachPuff()), "10-PeachPuff" ) );

        {//Guillotine - GP_BestAreaFit/GS_ShorterLeftoverAxis
            const String ImageName("TexturePacker-Guillotine-GS_ShorterLeftoverAxis.png");

            // Generate
            std::cout << "Starting packer: Guillotine-GP_BestAreaFit/GS_ShorterLeftoverAxis" << std::endl;
            Graphics::GuillotineTexturePacker Generated(512,512);
            Generated.Insert(TestImages,&Dummy,true,Graphics::GP_BestAreaFit,Graphics::GS_ShorterLeftoverAxis);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Guillotine-GP_BestAreaFit/GS_ShorterLeftoverAxis");
        }//Guillotine - GP_BestAreaFit/GS_ShorterLeftoverAxis

        {//Guillotine - GP_BestShortSideFit/GS_LongerLeftoverAxis
            const String ImageName("TexturePacker-Guillotine-GS_LongerLeftoverAxis.png");

            // Generate
            std::cout << "Starting packer: Guillotine-GP_BestShortSideFit/GS_LongerLeftoverAxis" << std::endl;
            Graphics::GuillotineTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.Insert(TestImages,&Dummy,true,Graphics::GP_BestShortSideFit,Graphics::GS_LongerLeftoverAxis);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Guillotine-GP_BestShortSideFit/GS_LongerLeftoverAxis");
        }//Guillotine - GP_BestShortSideFit/GS_LongerLeftoverAxis

        {//Guillotine - GP_BestLongSideFit/GS_MinimizeArea
            const String ImageName("TexturePacker-Guillotine-GS_MinimizeArea.png");

            // Generate
            std::cout << "Starting packer: Guillotine-GP_BestLongSideFit/GS_MinimizeArea" << std::endl;
            Graphics::GuillotineTexturePacker Generated(512,512);
            Generated.SetAllowRotated(true);
            Generated.Insert(TestImages,&Dummy,true,Graphics::GP_BestLongSideFit,Graphics::GS_MinimizeArea);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Guillotine-GP_BestLongSideFit/GS_MinimizeArea");
        }//Guillotine - GP_BestLongSideFit/GS_MinimizeArea

        {//Guillotine - GP_WorstAreaFit/GS_MaximizeArea
            const String ImageName("TexturePacker-Guillotine-GS_MaximizeArea.png");

            // Generate
            std::cout << "Starting packer: Guillotine-GP_WorstAreaFit/GS_MaximizeArea" << std::endl;
            Graphics::GuillotineTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.Insert(TestImages,&Dummy,true,Graphics::GP_WorstAreaFit,Graphics::GS_MaximizeArea);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Guillotine-GP_WorstAreaFit/GS_MaximizeArea");
        }//Guillotine - GP_WorstAreaFit/GS_MaximizeArea

        {//Guillotine - GP_WorstShortSideFit/GS_ShorterAxis
            const String ImageName("TexturePacker-Guillotine-GS_ShorterAxis.png");

            // Generate
            std::cout << "Starting packer: Guillotine-GP_WorstShortSideFit/GS_ShorterAxis" << std::endl;
            Graphics::GuillotineTexturePacker Generated(512,512);
            Generated.Insert(TestImages,&Dummy,true,Graphics::GP_WorstShortSideFit,Graphics::GS_ShorterAxis);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Guillotine-GP_WorstShortSideFit/GS_ShorterAxis");
        }//Guillotine - GP_WorstShortSideFit/GS_ShorterAxis

        {//Guillotine - GP_WorstLongSideFit/GS_LongerAxis
            const String ImageName("TexturePacker-Guillotine-GS_LongerAxis.png");

            // Generate
            std::cout << "Starting packer: Guillotine-GP_WorstLongSideFit/GS_LongerAxis" << std::endl;
            Graphics::GuillotineTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.SetAllowRotated(true);
            Generated.Insert(TestImages,&Dummy,true,Graphics::GP_WorstLongSideFit,Graphics::GS_LongerAxis);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Guillotine-GP_WorstLongSideFit/GS_LongerAxis");
        }//Guillotine - GP_WorstLongSideFit/GS_LongerAxis

        {//Guillotine - Single - GP_WorstLongSideFit/GS_LongerAxis
            const String ImageName("TexturePacker-Guillotine-Single-GS_LongerAxis.png");

            // Generate
            std::cout << "Starting packer: Guillotine-Single-GP_WorstLongSideFit/GS_LongerAxis" << std::endl;
            Graphics::GuillotineTexturePacker Generated(512,512);
            for( Graphics::TexturePacker::ImageIterator ImageIt = TestImages.begin() ; ImageIt != TestImages.end() ; ++ImageIt )
                { Generated.Insert((*ImageIt).Img,true,Graphics::GP_WorstLongSideFit,Graphics::GS_LongerAxis); }
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Guillotine-Single-GP_WorstLongSideFit/GS_LongerAxis");
        }//Guillotine - GP_WorstLongSideFit/GS_LongerAxis

        {//MaxRects - MRP_BestShortSideFit
            const String ImageName("TexturePacker-MaxRects-MRP_BestShortSideFit.png");

            // Generate
            std::cout << "Starting packer: MaxRects-MRP_BestShortSideFit" << std::endl;
            Graphics::MaxRectsTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.Insert(TestImages,&Dummy,true,Graphics::MRP_BestShortSideFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"MaxRects-MRP_BestShortSideFit");
        }//MaxRects - MRP_BestShortSideFit

        {//MaxRects - MRP_BestLongSideFit
            const String ImageName("TexturePacker-MaxRects-MRP_BestLongSideFit.png");

            // Generate
            std::cout << "Starting packer: MaxRects-MRP_BestLongSideFit" << std::endl;
            Graphics::MaxRectsTexturePacker Generated(512,512);
            Generated.Insert(TestImages,&Dummy,true,Graphics::MRP_BestLongSideFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"MaxRects-MRP_BestLongSideFit");
        }//MaxRects - MRP_BestLongSideFit

        {//MaxRects - MRP_BestAreaFit
            const String ImageName("TexturePacker-MaxRects-MRP_BestAreaFit.png");

            // Generate
            std::cout << "Starting packer: MaxRects-MRP_BestAreaFit" << std::endl;
            Graphics::MaxRectsTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.Insert(TestImages,&Dummy,true,Graphics::MRP_BestAreaFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"MaxRects-MRP_BestAreaFit");
        }//MaxRects - MRP_BestAreaFit

        {//MaxRects - MRP_TopLeftRule
            const String ImageName("TexturePacker-MaxRects-MRP_TopLeftRule.png");

            // Generate
            std::cout << "Starting packer: MaxRects-MRP_TopLeftRule" << std::endl;
            Graphics::MaxRectsTexturePacker Generated(512,512);
            Generated.SetAllowRotated(true);
            Generated.Insert(TestImages,&Dummy,true,Graphics::MRP_TopLeftRule);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"MaxRects-MRP_TopLeftRule");
        }//MaxRects - MRP_TopLeftRule

        {//MaxRects - MRP_ContactPointRule
            const String ImageName("TexturePacker-MaxRects-MRP_ContactPointRule.png");

            // Generate
            std::cout << "Starting packer: MaxRects-MRP_ContactPointRule" << std::endl;
            Graphics::MaxRectsTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.SetAllowRotated(true);
            Generated.Insert(TestImages,&Dummy,true,Graphics::MRP_ContactPointRule);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"MaxRects-MRP_ContactPointRule");
        }//MaxRects - MRP_ContactPointRule

        {//MaxRects - Single - MRP_ContactPointRule
            const String ImageName("TexturePacker-MaxRects-Single-MRP_ContactPointRule.png");

            // Generate
            std::cout << "Starting packer: MaxRects-Single-MRP_ContactPointRule" << std::endl;
            Graphics::MaxRectsTexturePacker Generated(512,512);
            for( Graphics::TexturePacker::ImageIterator ImageIt = TestImages.begin() ; ImageIt != TestImages.end() ; ++ImageIt )
                { Generated.Insert((*ImageIt).Img,true,Graphics::MRP_ContactPointRule); }
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"MaxRects-Single-MRP_ContactPointRule");
        }//MaxRects - Single - MRP_ContactPointRule

        {//Shelf - SP_NextFit
            const String ImageName("TexturePacker-Shelf-SP_NextFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-SP_NextFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            Generated.SetAllowRotated(true);
            Generated.Insert(TestImages,&Dummy,Graphics::SP_NextFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-SP_NextFit");
        }//Shelf - SP_NextFit

        {//Shelf - SP_FirstFit
            const String ImageName("TexturePacker-Shelf-SP_FirstFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-SP_FirstFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.Insert(TestImages,&Dummy,Graphics::SP_FirstFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-SP_FirstFit");
        }//Shelf - SP_FirstFit

        {//Shelf - SP_BestAreaFit
            const String ImageName("TexturePacker-Shelf-SP_BestAreaFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-SP_BestAreaFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            Generated.Insert(TestImages,&Dummy,Graphics::SP_BestAreaFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-SP_BestAreaFit");
        }//Shelf - SP_BestAreaFit

        {//Shelf - SP_WorstAreaFit
            const String ImageName("TexturePacker-Shelf-SP_WorstAreaFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-SP_WorstAreaFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.SetAllowRotated(true);
            Generated.Insert(TestImages,&Dummy,Graphics::SP_WorstAreaFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-SP_WorstAreaFit");
        }//Shelf - SP_WorstAreaFit

        {//Shelf - SP_BestHeightFit
            const String ImageName("TexturePacker-Shelf-SP_BestHeightFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-SP_BestHeightFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            Generated.Insert(TestImages,&Dummy,Graphics::SP_BestHeightFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-SP_BestHeightFit");
        }//Shelf - SP_BestHeightFit

        {//Shelf - SP_BestWidthFit
            const String ImageName("TexturePacker-Shelf-SP_BestWidthFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-SP_BestWidthFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.Insert(TestImages,&Dummy,Graphics::SP_BestWidthFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-SP_BestWidthFit");
        }//Shelf - SP_BestWidthFit

        {//Shelf - SP_WorstWidthFit
            const String ImageName("TexturePacker-Shelf-SP_WorstWidthFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-SP_WorstWidthFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            Generated.Insert(TestImages,&Dummy,Graphics::SP_WorstWidthFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-SP_WorstWidthFit");
        }//Shelf - SP_WorstWidthFit

        {//Shelf - Single - SP_WorstWidthFit
            const String ImageName("TexturePacker-Shelf-Single-SP_WorstWidthFit.png");

            // Generate
            std::cout << "Starting packer: Shelf-Single-SP_WorstWidthFit" << std::endl;
            Graphics::ShelfTexturePacker Generated(512,512);
            for( Graphics::TexturePacker::ImageIterator ImageIt = TestImages.begin() ; ImageIt != TestImages.end() ; ++ImageIt )
                { Generated.Insert((*ImageIt).Img,Graphics::SP_WorstWidthFit); }
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Shelf-Single-SP_WorstWidthFit");
        }//Shelf - Single - SP_WorstWidthFit

        {//Skyline - SLP_TopLeft
            const String ImageName("TexturePacker-Skyline-SLP_TopLeft.png");

            // Generate
            std::cout << "Starting packer: Skyline-SLP_TopLeft" << std::endl;
            Graphics::SkylineTexturePacker Generated(512,512);
            Generated.SetPadding(1);
            Generated.Insert(TestImages,&Dummy,Graphics::SLP_TopLeft);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Skyline-SLP_TopLeft");
        }//Skyline - SLP_TopLeft

        {//Skyline - SLP_BestAreaFit
            const String ImageName("TexturePacker-Skyline-SLP_BestAreaFit.png");

            // Generate
            std::cout << "Starting packer: Skyline-SLP_BestAreaFit" << std::endl;
            Graphics::SkylineTexturePacker Generated(512,512);
            Generated.SetAllowRotated(true);
            Generated.Insert(TestImages,&Dummy,Graphics::SLP_BestAreaFit);
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Skyline-SLP_BestAreaFit");
        }//Skyline - SLP_BestAreaFit

        {//Skyline - Single - SLP_BestAreaFit
            const String ImageName("TexturePacker-Skyline-Single-SLP_BestAreaFit.png");

            // Generate
            std::cout << "Starting packer: Skyline-Single-SLP_BestAreaFit" << std::endl;
            Graphics::SkylineTexturePacker Generated(512,512);
            for( Graphics::TexturePacker::ImageIterator ImageIt = TestImages.begin() ; ImageIt != TestImages.end() ; ++ImageIt )
                { Generated.Insert((*ImageIt).Img,Graphics::SLP_BestAreaFit); }
            //Generated.GetAtlasImage().Save(ImageName);

            // Load
            Graphics::Image Loaded;
            Loaded.Load(ImagePath + ImageName);

            //Test
            TEST(this->VerifyImage(&Generated.GetAtlasImage(),&Loaded),"Skyline-Single-SLP_BestAreaFit");
        }//Skyline - Single - SLP_BestAreaFit

        //Cleanup
        for( Whole ImgIdx = 0 ; ImgIdx < TestImages.size() ; ++ImgIdx )
            { delete TestImages[ImgIdx].Img; }
    }
    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//texturepackertests

#endif
