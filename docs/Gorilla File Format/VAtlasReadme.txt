VAtlas 0.3
By Scott McDonald
scottmc29@gmail.com


This program is meant to solve several problems with the nVidia atlas tool, mainly that its output is 24-bit instead of 32 
and it can't handle different sized textures when using a volume texture. 

You will need .Net 2.0 and Managed DirectX 9 installed to use the program.

No portions of this program used the nVidia source code.


// Command list

-help, -?		prints this command list
-volume, -v		uses a volume texture to store layers
-p, -p2, -power2	forces sizes to be a power of 2
-png			outputs single textures as pngs instead of dds
-split, -s <x> <y>	splits images into seperate x by y images
						
-width, -w <w>		sets width to <w>, default is 512
-height, -h <h>		sets height to <h>, default is 512
-depth, -d <d>		sets the maximum depth to <d>, default is 512
-levels, -l <l>		sets number of levels to <l>, default is 1
-ckey, -c <r> <g> <b>	sets color key to <r><g><b>, default is 0
-remove, -r <l> <t> <r> <b> removes a border from the image(s)
			
-output, -o <o>		sets output name to <o>, default is "Default"

-image, -img, -i <i>		adds the file <i> to the atlas
-directory, -dir, -dr <dir>	adds the files using the search string <dir> to the atlas

Example:
VAtlas.exe -w 1024 -h 1024 -o Grass -i Grass.bmp -dir "Textures\*.png"

Creates a 1024x1024 atlas using "Grass.bmp" and the PNG files in the directory "Textures". The atlas is stored in Grass.vaf and Grass_<l>.dds, where <l> is\nthe layer.
	

		
// Help on commands 

- Depth and layers are basically the same thing. 

- When not using a volume texture the program will output each layer as <name>_<l>.dds where <name> is the output name specified with the -o option and <l> is the layer number starting at 0.

- When using a volume texture the whole thing is stored in the file <name.dds>

- "Levels" is the number of mipmap levels, for volume textures I suggest leaving this at the default.

- PNG output is 24-bit, not 32. This is due to a limitation in Direct3D. If you want an alpha channel stick to DDS.



// Possible errors

Error: "no value specified"

Cause: on of the options that reads in a value like -width or -height is missing the value


Error: "can not parse value"

Cause: the program expected a numeric value, but got something else


Error: "generated atlas contains <x> layers, but depth is set to <d>"

Cause: too many images. Increase the width, height or depth.


// note that any of the errors below could be caused by a lack of memory

Error: "could not create Direct3D device"

Cause: Crappy video card or drivers. 


Error: "could not create volume texture"

Cause: Your video card may not support volume textures or not ones to the size specified. Try adjusting the width, height and depth and they may need to be a power of 2( use the -p option )


Error: "could not create texture"

Cause: Try adjusting the width and height and they may need to be a power of 2( use the -p option )

// Note for the last 2 errors, you should check the capabilities of your card using the the DirectX caps viewer.
// Your card will need to support software usage, the A8R8G8B8 format and the managed pool.


Error: "could not create output file <f>"

Cause: an error occured while trying to create or write to file <f>, it may be in use by another application.



// output format

The VAF format is a simple text format, it should be easy enough to understand. 
If not, here is how the file is laid out:

// start of file
VAtlas 	<version>	// 0.1, 0.2 or 0.3
Name: 	<name>		// the output name specified with the -o option
Width:	<width>		// width of each layer
Height:	<height>	// height of each layer
Depth:	<depth>		// number of layers
Flags:	<flags>		// possible flags are, None, Volume, PowerOfTwo, PNG and Split
Images: <images>	// number of images in the atlas
// next comes the list of images in this format
<X> <Y> <Z> : <Width> <Height> : <Frame> : <Path>
// note that coordinates are in pixels, not texels
// the Frame parameter only appears when using the split command.
// when using volume textures the Z coordinate is the layer. When using the Tw coordinate, divide Z by the depth
// when not using volume textures the Z coordinate is the file name, name_<Z>.dds



