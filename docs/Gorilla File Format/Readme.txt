This is a series of files and tools designed to help a new user learn what is needed to know about the .gorilla file format, and make one of their own.

The .gorilla file is a basic text file with information layed out similar to what you would find in a Bitmap Font file, and a Texture Atlas.  The gorilla file is basicly just a clash of these other two file types in a plain text format.  You can create multiple atlases if you want, or make one large one.  The primary use of the texture atlas is to reduce the batch queue by bundling all the font and UI sprites together, so they render at the same time.  This is what allows(among a couple other things) Gorilla to overpower native Ogre overlays in performance.

Included in this folder is a example gorilla file, called Dejavu.gorilla, and Dejavu.png.  The png file stores all the textures, and even though you can't see them, also the fonts.  You can have multiple texture files per atlas, but keep in mind that one png will be rendered at a time.  So for best results, get all of what you want in one png file.  Also included is the Gorilla file Schema explaination.  Read it carefully, it took me studying the explaination and looking at the Gorilla file while doing so to make it click.  The actual information in the file is fairly straight forward.  Says where to find the fonts and sprites in the atlas, and how big they are...for the most part.

When making a .gorilla file of your own, you should start with bmfontgen.exe , provided in this folder.  This is a windows commandline program(sorry mac and linux users) that will generate a Font Bitmap in the form of a .png, and .xml.  Closest thing to instructions I found for it is here:

http://blogs.msdn.com/b/garykac/archive/2006/08/30/732007.aspx

I personally just used the command " bmfontgen -fontdialog -output "font" ".  This opened a handy gui that displays all the supported fonts it could detect on your machine.  Even has a little preview area so you know what it looks like before exporting it.

Note:  You have to have a commandline open to the directory of this exe, and call on it's commands.  Double clicking it will do nothing.

Here's where the tedium begins.  The Author of Gorilla has a text editor that supports regex(Regular Expression), so for him it's a breeze to alter the .xml file accordingly.  I had no such luck.  I had to copy the contents of the xml file, to a plain text file, and change the formatting to match what is expected in the .gorilla file.  In the future, I don't recommend the same series of steps I took.  Rather then deleting all the info between the useful info over and over for about 140 lines, I recommend using the Gorilla template file and inputting the numbers accordingly.  In the case of Kernings it uses Ascii codes for the characters, and I have also provided a file that has a listing of codes to characters.

Basic idea when converting from the xml file to the gorilla file:

Take lines that look like this in the XML file:
<glyph ch="!" code="0021" bm="0" origin="0,0" size="6x20" aw="5" lsb="0"/>

And turn them into something that looks more like this(lines are equivalent):
glyph_33 0 0 6 20 5

The glyphs should already be layed out in the same order as ascii character codes are.  We don't care about the "ch", "code", "bm", or "lsb" fields.  We only care about the origin, size, and aw fields.  Keep the order the same, just provide spaces between each number.

Kernings are slightly more complicated, but not by much:

Take lines that look like this in the XML file:
<kernpair left="-" right="T" adjust="-1"/>

And turn them into something that looks more like this(lines are equivalent):
kerning_45 84 -1

Here we care about all the data fields.  Sadly though, we have to translate the glyph to it's ascii code.  Here is where the ascii reference file comes in.  Convert them as necessary, and add the adjustment at the end, and thats it for kernings.  All thats left is the info at the top.  Half are copy pasta from the XMl file, other half you can just guess.  Goes something like this:

"height" in the XML file becomes "lineheight" in the Gorilla file.
"aw" for the space glyph becomes "spacelength" in the Gorilla file.
"base" in the XML file becomes "baseline" in the Gorilla file.
"kerning" and "monowidth" don't have countparts in the XML file, so just guess here.  I usually just make them "-1" and "15" respectively.
"range" should stay "33 126" if you are only using standard ascii characters.  If you aren't, I can't really help you...cause I'm keeping this easy.

Only other thing for info at the top, is the actual name.  It has to be in the "[Font.X]" format, where X is any number you want.  This comes into play when making a caption or button and want to select the text to use.  Thats it for font info.

Next for the actual textures for the Atlas, use another .exe provided, Texture Atlas Generator.exe(again, this is windows only, sorry).  