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
#ifndef _graphicsutilities_cpp
#define _graphicsutilities_cpp

#include "Graphics/graphicsutilities.h"

#include "exception.h"
#include "stringtool.h"

namespace Mezzanine
{
    namespace Graphics
    {
        Graphics::ImageFileFormat ConvertImageFileExtension(String Extension)
        {
            StringTools::ToLowerCase(Extension);
            // Check for the most commonly used formats first.
            if( Extension == "png" ) return Graphics::IFF_Ext_png;
            else if( Extension == "bmp" ) return Graphics::IFF_Ext_bmp;
            else if( Extension == "tga" ) return Graphics::IFF_Ext_tga;
            else if( Extension == "dds" ) return Graphics::IFF_Ext_dds;
            else if( Extension == "ktx" ) return Graphics::IFF_Ext_ktx;
            else if( Extension == "pkm" ) return Graphics::IFF_Ext_pkm;
            else if( Extension == "pvr" ) return Graphics::IFF_Ext_pvr;
            // And now the rest
            else if( Extension == "cut" ) return Graphics::IFF_Ext_cut;
            else if( Extension == "exr" ) return Graphics::IFF_Ext_exr;
            else if( Extension == "gif" ) return Graphics::IFF_Ext_gif;
            else if( Extension == "g3" ) return Graphics::IFF_Ext_g3;
            else if( Extension == "hdp" ) return Graphics::IFF_Ext_hdp;
            else if( Extension == "hdr" ) return Graphics::IFF_Ext_hdr;
            else if( Extension == "ico" ) return Graphics::IFF_Ext_ico;
            else if( Extension == "iff" ) return Graphics::IFF_Ext_iff;
            else if( Extension == "jif" ) return Graphics::IFF_Ext_jif;
            else if( Extension == "jng" ) return Graphics::IFF_Ext_jng;
            else if( Extension == "jpe" ) return Graphics::IFF_Ext_jpe;
            else if( Extension == "jpg" ) return Graphics::IFF_Ext_jpg;
            else if( Extension == "jpeg" ) return Graphics::IFF_Ext_jpeg;
            else if( Extension == "jp2" ) return Graphics::IFF_Ext_jp2;
            else if( Extension == "jxr" ) return Graphics::IFF_Ext_jxr;
            else if( Extension == "j2c" ) return Graphics::IFF_Ext_j2c;
            else if( Extension == "j2k" ) return Graphics::IFF_Ext_j2k;
            else if( Extension == "koa" ) return Graphics::IFF_Ext_koa;
            else if( Extension == "lbm" ) return Graphics::IFF_Ext_lbm;
            else if( Extension == "mng" ) return Graphics::IFF_Ext_mng;
            else if( Extension == "pbm" ) return Graphics::IFF_Ext_pbm;
            else if( Extension == "pcd" ) return Graphics::IFF_Ext_pcd;
            else if( Extension == "pct" ) return Graphics::IFF_Ext_pct;
            else if( Extension == "pcx" ) return Graphics::IFF_Ext_pcx;
            else if( Extension == "pfm" ) return Graphics::IFF_Ext_pfm;
            else if( Extension == "pgm" ) return Graphics::IFF_Ext_pgm;
            else if( Extension == "pic" ) return Graphics::IFF_Ext_pic;
            else if( Extension == "pict" ) return Graphics::IFF_Ext_pict;
            else if( Extension == "ppm" ) return Graphics::IFF_Ext_ppm;
            else if( Extension == "psd" ) return Graphics::IFF_Ext_psd;
            else if( Extension == "ras" ) return Graphics::IFF_Ext_ras;
            else if( Extension == "sgi" ) return Graphics::IFF_Ext_sgi;
            else if( Extension == "targa" ) return Graphics::IFF_Ext_targa;
            else if( Extension == "tif" ) return Graphics::IFF_Ext_tif;
            else if( Extension == "tiff" ) return Graphics::IFF_Ext_tiff;
            else if( Extension == "wap" ) return Graphics::IFF_Ext_wap;
            else if( Extension == "wbm" ) return Graphics::IFF_Ext_wbm;
            else if( Extension == "wbmp" ) return Graphics::IFF_Ext_wbmp;
            else if( Extension == "wdp" ) return Graphics::IFF_Ext_wdp;
            else if( Extension == "webp" ) return Graphics::IFF_Ext_webp;
            else if( Extension == "xbm" ) return Graphics::IFF_Ext_xbm;
            else if( Extension == "xpm" ) return Graphics::IFF_Ext_xpm;
            else{
                MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to convert unsupported image extension as string to ImageFileFormat enum.");
                return Graphics::IFF_Ext_Unknown;
            }
            return Graphics::IFF_Ext_Unknown;
        }

        String ConvertImageFileExtension(const Graphics::ImageFileFormat Extension)
        {
            switch( Extension )
            {
                case Graphics::IFF_Ext_png:    return "png";
                case Graphics::IFF_Ext_bmp:    return "bmp";
                case Graphics::IFF_Ext_tga:    return "tga";
                case Graphics::IFF_Ext_dds:    return "dds";
                case Graphics::IFF_Ext_ktx:    return "ktx";
                case Graphics::IFF_Ext_pkm:    return "pkm";
                case Graphics::IFF_Ext_pvr:    return "pvr";
                case Graphics::IFF_Ext_cut:    return "cut";
                case Graphics::IFF_Ext_exr:    return "exr";
                case Graphics::IFF_Ext_gif:    return "gif";
                case Graphics::IFF_Ext_g3:     return "g3";
                case Graphics::IFF_Ext_hdp:    return "hdp";
                case Graphics::IFF_Ext_hdr:    return "hdr";
                case Graphics::IFF_Ext_ico:    return "ico";
                case Graphics::IFF_Ext_iff:    return "iff";
                case Graphics::IFF_Ext_jif:    return "jif";
                case Graphics::IFF_Ext_jng:    return "jng";
                case Graphics::IFF_Ext_jpe:    return "jpe";
                case Graphics::IFF_Ext_jpg:    return "jpg";
                case Graphics::IFF_Ext_jpeg:   return "jpeg";
                case Graphics::IFF_Ext_jp2:    return "jp2";
                case Graphics::IFF_Ext_jxr:    return "jxr";
                case Graphics::IFF_Ext_j2c:    return "j2c";
                case Graphics::IFF_Ext_j2k:    return "j2k";
                case Graphics::IFF_Ext_koa:    return "koa";
                case Graphics::IFF_Ext_lbm:    return "lbm";
                case Graphics::IFF_Ext_mng:    return "mng";
                case Graphics::IFF_Ext_pbm:    return "pbm";
                case Graphics::IFF_Ext_pcd:    return "pcd";
                case Graphics::IFF_Ext_pct:    return "pct";
                case Graphics::IFF_Ext_pcx:    return "pcx";
                case Graphics::IFF_Ext_pfm:    return "pfm";
                case Graphics::IFF_Ext_pgm:    return "pgm";
                case Graphics::IFF_Ext_pic:    return "pic";
                case Graphics::IFF_Ext_pict:   return "pict";
                case Graphics::IFF_Ext_ppm:    return "ppm";
                case Graphics::IFF_Ext_psd:    return "psd";
                case Graphics::IFF_Ext_ras:    return "ras";
                case Graphics::IFF_Ext_sgi:    return "sgi";
                case Graphics::IFF_Ext_targa:  return "targa";
                case Graphics::IFF_Ext_tif:    return "tif";
                case Graphics::IFF_Ext_tiff:   return "tiff";
                case Graphics::IFF_Ext_wap:    return "wap";
                case Graphics::IFF_Ext_wbm:    return "wbm";
                case Graphics::IFF_Ext_wbmp:   return "wbmp";
                case Graphics::IFF_Ext_wdp:    return "wdp";
                case Graphics::IFF_Ext_webp:   return "webp";
                case Graphics::IFF_Ext_xbm:    return "xbm";
                case Graphics::IFF_Ext_xpm:    return "xpm";
                case Graphics::IFF_Ext_Unknown:
                default:
                {
                    MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Attempting to convert unsupported ImageFileFormat enum value to string.");
                    return "";
                }
            }
            return "";
        }
    }//Graphics
}//Mezzanine

#endif
