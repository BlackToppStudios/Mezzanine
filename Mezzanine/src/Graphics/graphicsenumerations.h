// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

        /// @brief This is used by the BillboardSetProxy to decide how billboards should be rotated when they are requested to rotate.
        enum BillboardRotation
        {
            BBR_Vertex,        ///< Rotates the vertices of the billboard when rotated.
            BBR_TexureCoord    ///< Rotates the texture coordinates of the billboard when rotated, preserving the vertex positions.
        };

        /// @brief This is used by BillboardSetProxies to describe how the billboards are to be oriented.
        enum BillboardType
        {
            BBT_Point,                  ///< Default setting.  Billboard direction always faces the camera and is always oriented to be upright.
            BBT_Oriented_Common,        ///< Billboards are oriented around a common axis provided to the BillboardSetProxy, which will act as their local Y axis. Common Direction needs to be defined for this setting.
            BBT_Oriented_Self,          ///< Billboards are oriented around their own individually set direction axis, which will act as their local Y axis.
            BBT_Perpendicular_Common,   ///< Billboards are oriented around a common axis provided to the BillboardSetProxy, which will act as their local Z axis.  Common Direction and Common Up Vector needs to be defined for this setting.
            BBT_Perpendicular_Self      ///< Billboards are oriented around their own individually set direction axis, which will act as their local Z axis.  Common Up Vector needs to be defined for this setting.
        };

        /// @brief This is used by CameraProxies to quickly set a different render mode that is useful for debugging graphics renders.
        enum CameraPolyMode
        {
            CPM_Points    = 1,        ///< Only points for each of the vertices are rendered.
            CPM_WireFrame = 2,        ///< Wireframes for all the meshes and geometry are rendered.
            CPM_Solid     = 3         ///< Normal rendering.
        };

        /// @brief This enum describes the various file formats for images supported by the Mezzanine.
        /// @details Every format mentioned here has read support.  Many do not have write support.  Those that do will say so explicitly. @n @n
        /// In addition to the formats listed here there are also a TON of unlisted raw image formats made and used by digital camera's that can be read.  The list for those formats is extremely large
        /// and this list may one day be populated with them, but for now they will be left off of the list.  LibRAW provides that functionality and you can look at it's documentation if you need to know
        /// which camera formats are supported.  @n @n
        /// This is NOT an exhaustive list of what each format supports.  If you attempt to load or save an image the information here indicates it should work but does not, then some detail not mentioned here is
        /// likely preventing it from working.  Or the information here could be wrong.  Regardless information here should not be taken as a final authority.  Most of these formats have quirks.
        enum ImageFileFormat
        {
            IFF_Ext_Unknown = 0,  ///< Used exclusively in error conditions.
            // FreeImage formats
            IFF_Ext_bmp   = 1,    ///< Bitmap image.  Lossless compression format that can support most non-float pixel formats.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_cut   = 2,    ///< Acronym unknown.  Lossless compression format that supports 8-bits-per-pixel integer pixel format if it has an accompanying .PAL (Palette) file defining the colours it contains.  Otherwise .cut files are treated as grey scale images.  Does not support Alpha channel.
            IFF_Ext_exr   = 3,    ///< Acronym unknown.  Supports Lossless or Lossy compression with 16-bit float, 32-bit float, or 32-bit integer colour channels with an arbitrary number of colour channels per pixel.  Intended for HDR images.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_gif   = 4,    ///< Graphics Interchange Format image.  Lossless compression format that supports 8-bits-per-pixel and animations.  Does not support Alpha channel.  Writing is supported.
            IFF_Ext_g3    = 5,    ///< Acronym unknown.  Lossless compression format that supports 1-bit-per-pixel integer pixel format.  Does not support Alpha channel.
            IFF_Ext_hdp   = 6,    ///< Alternative name for jxr.  See jxr extension information for more information.
            IFF_Ext_hdr   = 7,    ///< High Dynamic Ranging image.  Lossless compression format that stores RGB in 8-bit colour channels with an 8-bit shared exponent taken from the brightest colour for a 32-bits-per-pixel float pixel format.  Intended for HDR images. Does not support Alpha channel.  Writing is supported.
            IFF_Ext_ico   = 8,    ///< Icon image.  This format isn't an image so much as it is a container for a bmp or png image.  It's limitations and capabilities are the same as those images, with one exception: .ico images are limited to 256x256 sizes.  Writing is supported.
            IFF_Ext_iff   = 9,    ///< Interchange File Format image.  Lossless compression format that supports 8-bit integer colour channels with an optional 8-bit Alpha channel, allowing either 24-bit or 32-bit pixels.  Supports Alpha channel.
            IFF_Ext_jif   = 10,   ///< JPEG Interchange Format image, subtle alternative in how pixels are packed to the jpeg extension.  See jpeg extension information for more information..  Writing is supported.
            IFF_Ext_jng   = 11,   ///< JPEG Network Graphics image.  This format extends the JPEG format by supporting Lossless or Lossy compression, 8-bit or 12-bit integer colour channels, and supports an Alpha channel.  Writing is supported.
            IFF_Ext_jpe   = 12,   ///< Shorthand for jpeg.  See jpeg extension information for more information.
            IFF_Ext_jpg   = 13,   ///< Shorthand for jpeg.  See jpeg extension information for more information.
            IFF_Ext_jpeg  = 14,   ///< Joint Photographic Experts Group image.  Lossy compression format that supports 24-bits-per-pixel integer pixel format.  Does not support Alpha channel.  Writing is supported.
            IFF_Ext_jp2   = 15,   ///< JPEG 2000 image.  This format extends the JPEG format by improving the underlying compression algorithm, supporting arbitrary bit depths up to 32-bit colour channels as integers or floats including an Alpha channel, and enabling lossy or lossless compression.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_jxr   = 16,   ///< JPEG XR image.  This extends the the JPEG standard by adding support for lossless compression and enabling 16-bit integer colour channels, enabling an Alpha channel, and enabling 16-bit (half-precision) or 32-bit (full precision) float pixel formats.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_j2c   = 17,   ///< JPEG 2000 Codestream image.  This is an image format that is generally embedded inside a jp2 or j2k image file, and shouldn't be seen independently, but might for debugging or special use cases.  Has the same limitations as jp2/j2k image files.  Writing is supported.
            IFF_Ext_j2k   = 18,   ///< JPEG 2000 image, an alternative name for jp2 image files.  See jp2 extension information for more information.
            IFF_Ext_koa   = 19,   ///< Koala Paint image.  Gonna level with you, when I looked up this format I found results that references 8-bit computers and Commodore 64's.  I also believe this format only supports 16 colours.  You probably shouldn't be using this.  Ever.
            IFF_Ext_lbm   = 20,   ///< Interleaved Bitmap image, which is an alternative name to the iff image format.  See iff extension information for more information.
            IFF_Ext_mng   = 21,   ///< Multiple-image Network Graphics image.  This is an animated image format that can have frames encoded as png or jng images.  See png or jng extension information for more information..
            IFF_Ext_pbm   = 22,   ///< Portable Bitmap, a Netpbm format image.  Uncompressed format that supports only 1-bit-per-pixel.  Does not support Alpha channel.  Writing is supported.
            IFF_Ext_pcd   = 23,   ///< (Kodak) Photo CD image.  Lossless compression format that supports 24-bits-per-pixel.  Does not support Alpha channel.
            IFF_Ext_pct   = 24,   ///< (Macintosh QuickDraw) Picture image, an alternative name for pict image files.  See pict extension information for more information.
            IFF_Ext_pcx   = 25,   ///< Personal Computer Exchange image.  This was the first widely accepted DOS image format!  In the off chance you need to hear more; the format in it's current form uses lossless compression, supports 1-bit and 24-bit pixels, and everything sensical in between.  Does not support Alpha channel.
            IFF_Ext_pfm   = 26,   ///< Portable Float Map, an unofficial Netpbm format image.  Lossless compression format that supports 8-bit or 24-bit float pixel format.  Does not support Alpha channel.  Writing is supported.
            IFF_Ext_pgm   = 27,   ///< Portable Graymap, a Netpbm format image.  Uncompressed format that can support 8-bit or 16-bit pixels in a single channel (greyscale).  Does not support Alpha channel.  Writing is supported.
            IFF_Ext_pic   = 28,   ///< (Macintosh QuickDraw) Picture image, an alternative name for pict image files.  See pict extension information for more information.
            IFF_Ext_pict  = 29,   ///< (Macintosh QuickDraw) Picture image.  This is a file that stores 16-bit opcodes that provide instructions for how to draw it's contents to QuickDraw.  This makes it a Vector image format rather than a Raster format.  Support for this format was discontinued with MacOSX.
            IFF_Ext_png   = 30,   ///< Portable Network Graphics image.  Lossless compression format that generally is 32-bits-per-pixel integer pixel format, but can be configured to have anything between 1-bit and 64-bits per pixel.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_ppm   = 31,   ///< Portable Pixmap, a Netpbm format image.  Uncompressed format that can support 8-bit or 16-bit integer colour channels.  Does not support Alpha channel.  Writing is supported.
            IFF_Ext_psd   = 32,   ///< Photoshop Document image.  Normal image format properties are unknown, but psd files are designed to store normal image data plus tons of additional Adobe metadata.
            IFF_Ext_ras   = 33,   ///< Sun Raster image.  Lossless compression format that supports 1-bit, 8-bit, and 24-bit integer pixel formats.  Does not support Alpha channel.
            IFF_Ext_sgi   = 34,   ///< Silicon Graphics Image.  Lossless compression format that supports 24-bit or 48-bit integer pixel formats.  Does not support Alpha channel.
            IFF_Ext_targa = 35,   ///< Truevision Advanced Raster Graphics Adapter image, an alternative name for tga image files.  See tga extension information for more information.
            IFF_Ext_tga   = 36,   ///< Truevision Graphics Adapter image.  Lossless compression format that supports 24-bit or 32-bit integer pixel formats.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_tif   = 37,   ///< Shorthand for tiff.  See tiff extension information for more information.
            IFF_Ext_tiff  = 38,   ///< Tagged Image File Format image.  Lossless compression format that supports 1-bit, 2-bit, 8-bit, or 32-bit integer pixels formats, as well as 32-bit or 64-bit float or double pixel formats.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_wap   = 39,   ///< Shorthand for wbmp.  See wbmp extension information for more information.
            IFF_Ext_wbm   = 40,   ///< Shorthand for wbmp.  See wbmp extension information for more information.
            IFF_Ext_wbmp  = 41,   ///< Wireless Bitmap image.  Lossless compression format that supports 1-bit-per-pixel integer pixel format.  Does not support Alpha channel.  Writing is supported.
            IFF_Ext_wdp   = 42,   ///< Alternative name for jxr.  See jxr extension information for more information.
            IFF_Ext_webp  = 43,   ///< (Google) Webp image.  Lossless or lossy compression format that supports 8-bit integer colour channels.  Supports Alpha channel.  Writing is supported.
            IFF_Ext_xbm   = 44,   ///< X11 Bitmap image.  Uncompressed format that uses c-style syntax to provide instructions on how to construct the image.
            IFF_Ext_xpm   = 45,   ///< X11 Pixmap image.  Lossless compression format that supports 24-bit integer pixel formats.  Does not support Alpha channel.  Writing is supported.

            // Non-FreeImage formats
            IFF_Ext_dds   = 46,   ///< DirectDraw Surface image.  Lossy or Lossless compression format that supports a wide range of pixel formats, as well as texture arrays, cube maps, and volume maps.  Despite it's name some GL implementations support it's use as well, allowing it to stay compressed in video memory.  Supports Alpha channel.
            IFF_Ext_ktx   = 47,   ///< Acronym unknown.  Uses Ericsson Texture Compression which is lossy and supports 8-bit colour channels.  This is dominately used on Android devices and in some cases can stay compressed in video memory.  Supports Alpha channel.
            IFF_Ext_pkm   = 48,   ///< Alternative name for ktx.  See ktx extension information for more information.
            IFF_Ext_pvr   = 49    ///< PowerVR image.  Lossy compression format that supports 2-bit or 4-bit integer pixels formats.  This is dominately used on iOS devices to store simple images and in some cases can stay compressed in video memory.  Does not support Alpha channel.
        };

        /// @brief This is used by LightProxies to describe how light is emitted from the proxy source.
        enum LightType
        {
            LT_Directional = 0,    ///< From one direction, like sunlight.
            LT_Point       = 1,    ///< From a point in space, like a Torch, campfire, muzzle flash, Mutant Fireflies, bonfires, light bulbs, non-hooded lantern, the DnD D20 Light spell, explosions, and scotch tape separating from the roll in a unlit vacuum. There may be other uses, be creative.
            LT_Spotlight   = 2     ///< From a point emanating in a cone, like a flashlight, hooded lantern, really bright computer screens, flood lights, older style space heaters, Concert lights, camera flashes, etc...
        };

        /// @brief This is used to describe simple rotations in views or textures.
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

        /// @brief This is used to describe how bits are arranged for each pixel in an image.
        enum PixelFormat
        {
            PF_Unknown            = 0,
            PF_L8                 = 1,
            PF_Byte_L             = PF_L8,
            PF_L16                = 2,
            PF_Short_L            = PF_L16,
            PF_A8                 = 3,
            PF_Byte_A             = PF_A8,
            PF_A4L4               = 4,
            PF_Byte_LA            = 5,
            PF_R5G6B5             = 6,
            PF_B5G6R5             = 7,
            PF_R3G3B2             = 31,
            PF_A4R4G4B4           = 8,
            PF_A1R5G5B5           = 9,
            PF_R8G8B8             = 10,
            PF_B8G8R8             = 11,
            PF_A8R8G8B8           = 12,
            PF_A8B8G8R8           = 13,
            PF_B8G8R8A8           = 14,
            PF_R8G8B8A8           = 28,
            PF_X8R8G8B8           = 26,
            PF_X8B8G8R8           = 27,
#ifdef MEZZ_BIG_ENDIAN
            PF_Byte_RGB           = PF_R8G8B8,
            PF_Byte_BGR           = PF_B8G8R8,
            PF_Byte_BGRA          = PF_B8G8R8A8,
            PF_Byte_RGBA          = PF_R8G8B8A8,
#endif
#ifdef MEZZ_LITTLE_ENDIAN
            PF_Byte_RGB           = PF_B8G8R8,
            PF_Byte_BGR           = PF_R8G8B8,
            PF_Byte_BGRA          = PF_A8R8G8B8,
            PF_Byte_RGBA          = PF_A8B8G8R8,
#endif
            PF_A2R10G10B10        = 15,
            PF_A2B10G10R10        = 16,
            PF_DXT1               = 17,
            PF_DXT2               = 18,
            PF_DXT3               = 19,
            PF_DXT4               = 20,
            PF_DXT5               = 21,
            PF_Float16_R          = 32,
            PF_Float16_RGB        = 22,
            PF_Float16_RGBA       = 23,
            // 32-bit pixel format, 32 bits (float) for red
            PF_Float32_R          = 33,
            PF_Float32_RGB        = 24,
            PF_Float32_RGBA       = 25,
            PF_Float16_GR         = 35,
            PF_Float32_GR         = 36,
            PF_Depth              = 29,
            PF_Short_RGBA         = 30,
            PF_Short_GR           = 34,
            PF_Short_RGB          = 37,
            PF_PVRTC_RGB2         = 38,
            PF_PVRTC_RGBA2        = 39,
            PF_PVRTC_RGB4         = 40,
            PF_PVRTC_RGBA4        = 41,
            PF_PVRTC2_2BPP        = 42,
            PF_PVRTC2_4BPP        = 43,
            PF_R11G11B10_Float    = 44,
            PF_R8_UInt            = 45,
            PF_R8G8_UInt          = 46,
            PF_R8G8B8_UInt        = 47,
            PF_R8G8B8A8_UInt      = 48,
            PF_R16_UInt           = 49,
            PF_R16G16_UInt        = 50,
            PF_R16G16B16_UInt     = 51,
            PF_R16G16B16A16_UInt  = 52,
            PF_R32_UInt           = 53,
            PF_R32G32_UInt        = 54,
            PF_R32G32B32_UInt     = 55,
            PF_R32G32B32A32_UInt  = 56,
            PF_R8_SInt            = 57,
            PF_R8G8_SInt          = 58,
            PF_R8G8B8_SInt        = 59,
            PF_R8G8B8A8_SInt      = 60,
            PF_R16_SInt           = 61,
            PF_R16G16_SInt        = 62,
            PF_R16G16B16_SInt     = 63,
            PF_R16G16B16A16_SInt  = 64,
            PF_R32_SInt           = 65,
            PF_R32G32_SInt        = 66,
            PF_R32G32B32_SInt     = 67,
            PF_R32G32B32A32_SInt  = 68,
            PF_R9G9B9E5_SharedExp = 69,
            PF_BC4_UNorm          = 70,
            PF_BC4_SNorm          = 71,
            PF_BC5_UNorm          = 72,
            PF_BC5_SNorm          = 73,
            PF_BC6H_UF16          = 74,
            PF_BC6H_SF16          = 75,
            PF_BC7_UNorm          = 76,
            PF_BC7_UNorm_SRGB     = 77,
            PF_R8                 = 78,
            PF_RG8                = 79,
            PF_R8_SNorm           = 80,
            PF_R8G8_SNorm         = 81,
            PF_R8G8B8_SNorm       = 82,
            PF_R8G8B8A8_SNorm     = 83,
            PF_R16_SNorm          = 84,
            PF_R16G16_SNorm       = 85,
            PF_R16G16B16_SNorm    = 86,
            PF_R16G16B16A16_SNorm = 87,
            PF_ETC1_RGB8          = 88,
            PF_ETC2_RGB8          = 89,
            PF_ETC2_RGBA8         = 90,
            PF_ETC2_RGB8A1        = 91,
            PF_ATC_RGB            = 92,
            PF_ATC_RGBA_Explicit_Alpha = 93,
            PF_ATC_RGBA_Interpolated_Alpha = 94,
            // Number of pixel formats currently defined
            PF_Count              = 95
        };

        /// @brief Values for storing how perspective should be interpretted
        enum ProjectionType
        {
            PT_Orthographic    = 0,        ///< No projection is used, objects appear to be flat.
            PT_Perspective     = 1         ///< Normal (realistic) perspective.
        };

        /// @brief Used to describe low level assembly of Meshes.  Mostly for internal use.
        enum RenderOperation
        {
            RO_PointList      = 1,  ///< Vertices are just a bunch of points.  Do not form polygons.
            RO_LineList       = 2,  ///< Vertices form lines.  Each line segment has it's own distinct pair of Vertices.  Vertices are not expected to be reused.
            RO_LineStrip      = 3,  ///< Vertices form lines.  The first line is defined with 2 Vertices and each line after requires one additional Vertex, reusing the previous lines end Vertex as the current lines start Vertex.
            RO_TriangleList   = 4,  ///< Vertices form triangles.  Each triangle has it's own distinct set of Vertices.  Vertices are not expected to be reused.
            RO_TriangleStrip  = 5,  ///< Vertices form triangles.  The first triangle is defined with 3 Vertices and each triangle after requires one additional Vertex, reusing the previous two Vertices for the start of the triangle.
            RO_TriangleFan    = 6   ///< Similar to RO_TriangleString, first triangle defined with 3 Vertices and each triangle after requires one additional Vertex.
        };

        /// @brief An enum that specifies which RenderSystem is to be used for rendering.
        enum RenderSystem
        {
            RS_Invalid     = 0,
            RS_DirectX9    = 1,
            RS_DirectX10   = 2,
            RS_DirectX11   = 3,
            RS_OpenGL2     = 4,
            RS_OpenGL3Plus = 5,
            RS_OpenGLES1   = 6,
            RS_OpenGLES2   = 7
        };

        /// @brief An enum used to describe the various types of supported textures.
        enum TextureType
        {
            TT_Invalid  = 0,  ///< Used to report error conditions.
            TT_1D       = 1,  ///< A strip of pixels.
            TT_2D       = 2,  ///< A normal 2 dimensional texture.
            TT_3D       = 3,  ///< A volume texture.
            TT_Cube_Map = 4,  ///< A texture used in conjunction with 2D coordinates to be applied to the faces of a cube.
            TT_2D_Array = 5,  ///< A 2D texture array.
            TT_2D_Rect  = 6   ///< A non-square 2D texture used in conjunction with 2D coordinates.
        };

        /// @brief An enum used to describe how the buffer of a texture will be used.
        enum TextureUsage
        {
            TU_Static                           = 1,              ///< The texture will rarely be modified by the client application.
            TU_Dynamic                          = 2,              ///< The texture may be changed most or every frame by the client application.
            TU_Write_Only                       = 4,              ///< The texture will never be read by the client application, only written to.
            TU_Static_Write_Only   = TU_Static | TU_Write_Only,   ///< A combination of TU_Static and TU_Write_Only.
            TU_Dynamic_Write_Only  = TU_Dynamic | TU_Write_Only,  ///< A combination of TU_Dynamic and TU_Write_Only.
            TU_Dynamic_Write_Only_Discardable   = 14,             ///< Like TU_Dynamic_Write_Only, but also the contents of the texture buffer may be completely regenerated by the client application.
            TU_AutoMipMap                       = 16,             ///< Indicates MipMaps will be automatically generated for the texture.
            TU_RenderTarget                     = 32,             ///< Indicated this texture will be used each frame for scene rendering.
            TU_Default = TU_AutoMipMap | TU_Static_Write_Only     ///< By default textures are TU_Static_Write_Only and TU_AutoMipMap.
        };

        /// @brief An enum used to describe what the vertex element is defining.
        /// @remarks At a minimum all vertices need a position.  Meshes with textures applied to them also need
        /// texture coordinates.  Meshes that are skeletally animated need blend semantics.  The rest are additional
        /// data for fancy shader effects.
        enum VertexElementSemantic
        {
            VES_Position       = 1,  ///< The local space position of the Vertex.  Commonly represented by 3 floats.
            VES_Blend_Weights  = 2,  ///< Used in Skeletal Animation.  The amount this vertex should move with a bone in the skeleton.  Commonly represented by 1 float.
            VES_Blend_Indices  = 3,  ///< Used in Skeletal Animation.  The bone in the skeleton this vertex should follow.  Commonly represented by 1 float or int.
            VES_Normal         = 4,  ///< The outward facing direction of the Vertex.  Commonly represented by 3 floats.
            VES_Diffuse        = 5,  ///< The Diffuse colour of the Vertex.  Commonly represented by 4 floats.
            VES_Specular       = 6,  ///< The Specular colour of the Vertex.  Commonly represented by 4 floats.
            VES_Texture_Coords = 7,  ///< The coordinates in texture space that should be bound to the Vertex.  Commonly represented by 2 floats.
            VES_Binormal       = 8,  ///< Additional directional data along side the normal.  Y axis if normal is Z.  Commonly represented by 3 floats.
            VES_Tangent        = 9,  ///< Additional directional data along side the normal.  X axis if normal is Z.  Commonly represented by 3 floats.
            VES_Blend_Weights2 = 10, ///< Same as VES_Blend_Weights, but the weight for a second bone to move with.
            VES_Blend_Indices2 = 11, ///< Same as VES_Blend_Indices, but the index of a second bone to move with.
        };

        /// @brief An enum used to define the data type for the vertex element.
        /// @remarks All types must be able to be aligned to 4 byte boundaries.
        enum VertexElementType
        {
            VET_Float1        = 0,  ///< A single precision float.
            VET_Float2        = 1,  ///< 2 single precision floats.
            VET_Float3        = 2,  ///< 3 single precision floats.
            VET_Float4        = 3,  ///< 4 single precision floats.
            VET_Colour        = 4,  ///< 4 single precision floats without the channels defined.
            VET_Short2        = 5,  ///< 2 signed short ints.
            VET_Short4        = 6,  ///< 4 signed short ints.
            VET_UByte4        = 7,  ///< 4 unsigned short ints.
            VET_Colour_ARGB   = 8,  ///< 4 single precision floats with channels defined: Alpha, Red, Green, and Blue (in that order).
            VET_Colour_ABGR   = 9,  ///< 4 single precision floats with channels defined: Alpha, Blue, Green, and Red (in that order).
            VET_Double1       = 10, ///< A double precision float.
            VET_Double2       = 11, ///< 2 double precision floats.
            VET_Double3       = 12, ///< 3 double precision floats.
            VET_Double4       = 13, ///< 4 double precision floats.
            VET_UShort2       = 14, ///< 2 unsigned short ints.
            VET_UShort4       = 15, ///< 4 unsigned short ints.
            VET_Int1          = 16, ///< A signed int.
            VET_Int2          = 17, ///< 2 signed ints.
            VET_Int3          = 18, ///< 3 signed ints.
            VET_Int4          = 19, ///< 4 signed ints.
            VET_UInt1         = 20, ///< An unsigned int.
            VET_UInt2         = 21, ///< 2 unsigned ints.
            VET_UInt3         = 22, ///< 3 unsigned ints.
            VET_UInt4         = 23, ///< 4 unsigned ints.
            VET_Byte4         = 24, ///< 4 signed 8-bit ints.
            VET_Byte4_SNorm   = 25, ///< 4 signed normalized short ints.
            VET_UByte4_Norm   = 26, ///< 4 unsigned normalized short ints.
            VET_Short2_SNorm  = 27, ///< 2 signed normalized short ints.
            VET_Short4_SNorm  = 28, ///< 4 signed normalized short ints.
            VET_UShort2_Norm  = 29, ///< 2 unsigned normalized short ints.
            VET_UShort4_Norm  = 30, ///< 4 unsigned normalized short ints.
            VET_Half2         = 31, ///< 2 half-precision floats.
            VET_Half4         = 32, ///< 4 half-precision floats.
        };
    }//Graphics
}//Mezzanine

#endif
