#!/bin/bash
# Takes in PugiXML and emits XML files appropriate for direct inclusion in the Mezzanine library
# This expects the PugiXML source files to be in the subdirectory call PugiSrc

# This script isn't magic, it was designed with version 1.0 of PugiXML in mind. If new features have been added, then new commands
# will need to be add to rename those as well. That being said, I did try to code it in such a way that it would still work with new versions of the source files...
# I hope this really does simplify things when upgrade time comes around, because that is the sole reason this exists.

#for reference this took 5 to 15 seconds to run on my i7 and 2+ minutes to my on my p4.

exit

#disable Bash filename expansion
set -f

#error codes - These match the Header prepending tool
E_BADARGS=1
E_MISSINGPUGIXML=3

#Input Files
PugiFolder="pugisrc"
PugiCppFile="$PugiFolder/pugixml.cpp"
PugiHFile="$PugiFolder/pugixml.hpp"
PugiConfigFile="$PugiFolder/pugiconfig.hpp"
PugiTempHFile="$PugiFolder/temp.hpp"
MezzxmlifdefFile="ifdef.txt"
MezzxmlendifFile="endif.txt"
DoxEnd="DoxEnd.txt"
DoxResume="DoxResume.txt"
echo "asdf"
if [ -d "$PugiFolder" ]; then
	echo "Found old copy of src, removing it."
fi

end


exit
echo "Checkout time"

#Output Files
OutDir="../../Mezzanine/src"
NewHFileOnly="xml.h"
NewCppFileOnly="xml.cpp"
NewHFile="$OutDir/$NewHFileOnly"
NewCppFile="$OutDir/$NewCppFileOnly"

#Some other variables we will be using
#Copyright Variables
Hardstring="This should not be found on any lineasdfasdfasdfasdf"	# this Should not appear in any of the Files, if it does, change it.
OurLicenseFile="fileheader.txt"
OurCopyright="\n * Software, Files, Libraries and all other items referenced in this clause refers only\n * to the contents of this file and associated documentation.\n *\n"
IncompleteCopyright1=" * Copyright (c) 2006-2010 Arseny Kapoulkine"
CompleteCopyright1="$OurCopyright * Copyright © 2006-2010 Arseny Kapoulkine"
IncompleteCopyright2=" * pugixml parser - version 1.0"
CompleteCopyright2="$OurCopyright * Pugixml parser - version 1.0"
CopyrightAccidentalDoxyTags='\/\*\*'
CopyrightCleantags='\/\*'

#Namespace Variables
PugiUsingNamespace="using namespace pugi"
PugiNamespace="namespace pugi"
OurUsingNamespace="using namespace Mezzanine::xml"
#OurUsingNamespace="using namespace Mezzanine"
#OurUsingNamespace="using namespace xml"
OurNamespace="namespace xml"
OurExtraNamespace="
namespace Mezzanine
{"
OurExtraNamespaceClosing="} // \Mezzanine"


#Make the Files to begin appending Source code to.
cat  $OurLicenseFile $DoxEnd > $NewCppFile
cat  $OurLicenseFile > $PugiTempHFile

# This lops of the end copyright because we can use the ending from the other file we are merging with
PugiConfigGrabUntil="#endif"
				# The Copyright notices, and other string to search for in the PugiConfig File
				#This next line uses grep to get the line number of the token we are searching for and then sends the results to
				#sed to strip of the : and the token appended to the value we want.
PugiConfigGrabLineCount=`grep "$PugiConfigGrabUntil" $PugiConfigFile -n | head -n1 | sed s/:.*//g`
head -n$PugiConfigGrabLineCount $PugiConfigFile  >> $PugiTempHFile

# This Lops of the first copyright notice by ommision (since this is the last part of the file we will keep the tail copyright) and appends the results to the results of the Config Transformation
PugiHeaderGrabUntil="*/"						# This ends the copyright header in the pugiheader file
PugiHeaderGrabLineCount=`grep "$PugiHeaderGrabUntil" $PugiHFile -n | head -n1 | sed s/:.*//g`
PugiHeaderTotalLineCount=`grep "$Hardstring" $PugiHFile -vc`
PugiHeaderTailLineCount=$[PugiHeaderTotalLineCount-PugiHeaderGrabLineCount]
tail -n$PugiHeaderTailLineCount $PugiHFile | sed -e s/'#include "pugiconfig.hpp"'/'#include "crossplatform.h"\n#include "xmldoc.h"\n#include "exception.h"'/g  >> $PugiTempHFile

#This Does a number of transformations on the PugiHeader file before placing the complete converted parts onto the new headerfile
#	- Replaces series of 4 spaces with tabs (seems to be an issue with the original source Files)
#	- Remove reference to old filenames
#	- Appends a notice to Arseny's Copyright that it only covers items arseny wrote.
#	- clean up Copyright Symbols as (c) is meaningless in some jurisdications (as per RMS)
#	- Replace multiple inclusion with names matching our own
#		HEADER_PUGIXML_HPP -> _xml_h
#		HEADER_PUGICONFIG_HPP	-> _xmlconfig_h
#	- Put the results in the final place they belong
cat $MezzxmlifdefFile $PugiTempHFile $MezzxmlendifFile| sed  	-e s/'    '/'	'/g 	-e s/"$IncompleteCopyright1"/"$CompleteCopyright1"/g 	-e s/$CopyrightAccidentalDoxyTags/$CopyrightCleantags/g	-e s/"$IncompleteCopyright2"/"$CompleteCopyright2"/g 	-e s/\([cC]\)/©/g 	-e s/HEADER_PUGIXML_HPP/_XML_H/g 	-e s/HEADER_PUGICONFIG_HPP/_XMLCONFIG_H/g	-e s/PUGIXML_/XML_/g 	-e s/"$PugiUsingNamespace"/"$OurUsingNamespace"/g	> $NewHFile

# This does mostly the same thing that the header transfortmation does, a bunch of cleanup and putting the code where it belongs
cat $MezzxmlifdefFile $PugiCppFile $MezzxmlendifFile $DoxResume | sed 	-e s/'#include "pugixml.hpp"'/"#include \"$NewHFileOnly\""/g 	-e s/'    '/'	'/g 	-e s/"$IncompleteCopyright1"/"$CompleteCopyright1"/g 	-e s/"$IncompleteCopyright2"/"$CompleteCopyright2"/g 	-e s/$CopyrightAccidentalDoxyTags/$CopyrightCleantags/g	-e s/\([C]\)/©/g 	-e s/PUGIXML_/XML_/g	-e s/"$PugiUsingNamespace"/"$OurUsingNamespace"/g	>> $NewCppFile

# This will look in file $1 for the bracket that should end the statement that starts with $2 and ends the statement with $4
# while doing this, old instances of $2 and replace them with $3
# In other words
# $1 is the filename
# $2 is Old namespace
# $3 is the new double namespace
# $4 is the item to end the namespace, use "}" if you don't wantanything fancy.
function FixNamespaces
{
	TempFileName="$1.tmp"
	echo -n "" > $TempFileName
	LB="{"						#The character that begins a codeblock
	RB="}"						#The character that ends a codeblock
	LookingForEndBracket=-1	#if this is set to -1 then we are looking for the line of code to start search for our bracket at.
								#if this is set to a positive number, then it is the amount of unclosed left brackets we have found.

	LineCount=0
	OIFS="$IFS"
	IFS=$'\n'
	FileContent=`cat $1`

	#echo -E "$FileContent" > $1.txt.cpp
	FileContent=`echo -E "$FileContent" | sed -e 's/\(.*$\)/\1 /'`
	#echo -E "$FileContent" > $1.txt2.cpp

	for fn in $FileContent; do
		#LineCount=$[LineCount+1]
		#echo $LineCount #: $fn
		if [ $LookingForEndBracket -eq -1 ]; then
			#looking for Namespace
			if [ "$fn" = "${fn/$2/asdf}" ]; then
				#Line has no match in it, so we send it to temp file
				echo -E "$fn" >> $TempFileName
			else
				#This line does match, send to replacement to tempfile and start counting
				LookingForEndBracket=0
				echo -E "$3" >> $TempFileName
			fi
		else
			echo -E "$fn" >> $TempFileName
			#We are looking for a Brackets now
			if [ "$fn" != "${fn/$LB/asdf}" ]; then
				#We found a left bracket we are going deeper
				LookingForEndBracket=$[1+LookingForEndBracket]
			fi
			if [ "$fn" != "${fn/$RB/asdf}" ]; then
				#We found a right bracket we are coming out
				LookingForEndBracket=$[LookingForEndBracket-1]
				if [ $LookingForEndBracket == 0 ]; then
					LookingForEndBracket=-1
					echo -E "$4" >> $TempFileName
				fi
			fi
		fi
	done

	mv $TempFileName $1
	IFS="$OIFS"
}

FixNamespaces $NewHFile "$PugiNamespace" "$OurExtraNamespace $OurNamespace" "$OurExtraNamespaceClosing"
FixNamespaces $NewCppFile "$PugiNamespace" "$OurExtraNamespace $OurNamespace" "$OurExtraNamespaceClosing"

#This will replace all the old PugiXML names in the file providesd in $1 with the new BTS Mezzanine::xml names
function FixNames
{
	#FileContent=`cat $1`
	#TempFileName="$1.tmp"
	#echo -E "$FileContent" > $TempFileName

	#A clean line to copy and paste 
	#sed -i -e 's/needle/replacement/'`

	#######################################################################
	#This is the list of items to find/replace to convert the Pugi API into Mezzanine API

	#Remove the original and grossly incomplete doxygen documentations
	sed -i -e 's|///|////|g' $1								

	#Adjust the names of two word type to variables to use CamelCase
	sed -i -e 's/xml_\(.\)\([a-z]*\)_\(.\)/\U\1\L\2\U\3/g' $1		
	sed -i -e 's/xml_\(.\)/\U\1/g' $1

	#These configure the XML API to be exported to a DLL on windows
	sed -i -e 's/XML_CLASS/MEZZ_LIB/g' $1					
	sed -i -e 's/XML_FUNCTION/MEZZ_LIB/g' $1
	sed -i -e 's/XML_API/MEZZ_LIB/g' $1

	#Status and error codes
	sed -i -e 's/status_ok/StatusOk/g' $1										
	sed -i -e 's/status_file_not_found/StatusFileNotFound/g' $1
	sed -i -e 's/status_io_error/StatusIOError/g' $1
	sed -i -e 's/status_out_of_memory/StatusOutOfMemory/g' $1
	sed -i -e 's/status_unrecognized_tag/StatusUnrecognizedTag/g' $1
	sed -i -e 's/status_bad_pi/StatusBadPi/g' $1
	sed -i -e 's/status_bad_comment/StatusBadComment/g' $1
	sed -i -e 's/status_bad_cdata/StatusBadCdata/g' $1
	sed -i -e 's/status_bad_doctype/StatusBadDoctype/g' $1
	sed -i -e 's/status_bad_pcdata/StatusBadPcdata/g' $1
	sed -i -e 's/status_bad_start_element/StatusBadStartElement/g' $1
	sed -i -e 's/status_bad_attribute/StatusBadAttribute/g' $1
	sed -i -e 's/status_bad_end_element/StatusBadEndElement/g' $1
	sed -i -e 's/status_end_element_mismatch/StatusEndElementMismatch/g' $1
	sed -i -e 's/status_internal_error/StatusInternalError/g' $1

	#Fix member ParseStatus.Status, ParseStatus.DocumentEncoding, ParseStatus.Offset and ParseStatus.Description()
	sed -i -e 's/status/Status/g' $1
	sed -i -e 's/encoding/DocumentEncoding/g' $1
	sed -i -e 's/offset\([^o]\)/Offset\1/g' $1
	sed -i -e 's/description/Description/g' $1

	#Fix The Encoding Enum All 1,2 and 3 part variable names
	sed -i -e 's/\(DocumentEncoding\)_\([au][tf]*\)\([a-z0-9]*\)_\([ble]*\)/Encoding\U\2\3\U\4/g' $1
	sed -i -e 's/\(DocumentEncoding\)_\([au][tf]*\)\([a-z0-9]*\)/Encoding\U\2\L\3/g' $1
	sed -i -e 's/DocumentEncoding_wchar/Encodingwchar_t/g' $1

	#Fix the NodeType Enum
	sed -i -e 's|\(n\)\(ode\)_\(.\)\([a-z]*\)|\U\1\L\2\U\3\L\4|g' $1

	#More tightly integrating Character into the XML parser
	sed -i -e 's|typedef XML_CHAR char_t;|typedef Character char_t;|g' $1
	sed -i -e 's|typedef std::basic_string<XML_CHAR, std::char_traits<XML_CHAR>, std::allocator<XML_CHAR> >;|//typedef std::basic_string<Character, std::char_traits<Character>, std::allocator<Character> > string_t;|g' $1
	sed -i -e 's|string_t|String|g' $1
	
	#sed -i -e 's|\([^w]\)char_t|\1Character|g' $1 #This very breaks the code

	#Renaming 1 and2 part Parse options
	sed -i -e 's|parse_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Parse\U\1\L\2\U\3\L\4|g' $1				
	sed -i -e 's|parse_\(.\)\([a-z]*\)|Parse\U\1\L\2|g' $1

	#Renaming 1,2 and 3 part Format options
	sed -i -e 's|format_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Format\U\1\L\2\U\3\L\4\U\5\L\6|g' $1	
	sed -i -e 's|format_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Format\U\1\L\2\U\3\L\4|g' $1
	sed -i -e 's|format_\(.\)\([a-z]*\)|Format\U\1\L\2|g' $1				
	sed -i -e 's|const unsigned int FormatDefault = FormatIndent|const unsigned int FormatDefault = FormatRaw|g' $1		

	#Renaming 1 and 2 part Xpath Class Names
	sed -i -e 's|xpath_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|XPath\U\1\L\2\U\3\L\4|g' $1	
	sed -i -e 's|xpath_\(.\)\([a-z]*\)|XPath\U\1\L\2|g' $1				

	#The Writer class and compatibility options
	sed -i -e 's|writer|WriterInstance|g' $1
	sed -i -e 's|write|Write|g' $1
	sed -i -e 's|_stream->Write|_stream->write|g' $1	#These 2 lines fixe mistakes made by the above line
	sed -i -e 's|fWrite|fwrite|g' $1

	#sed -i -e 's|struct AttributeStruct;|struct AttributeStruct\n	{\n		AttributeStruct(MemoryPage* page): header(reinterpret_cast<uintptr_t>(page)), Name(0), Value(0), prev_attribute_c(0), GetNextAttribute(0) {} \n		uintptr_t header;\n		char_t* Name;\n		char_t*	Value;\n		AttributeStruct* prev_attribute_c;\n		AttributeStruct* GetNextAttribute;\n	}; |g' $1

	#Common class member
	sed -i -e 's|set_\(.\)\([a-z]*\)|Set\U\1\L\2|g' $1
	sed -i -e 's|as_\(.\)\([a-z]*\)|As\U\1\L\2|g' $1
	sed -i -e 's|next_\(.\)\([a-z]*\)|Next\U\1\L\2|g' $1
	sed -i -e 's|previous_\(.\)\([a-z]*\)|Previous\U\1\L\2|g' $1
	sed -i -e 's|first_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|First\U\1\L\2\U\3\L\4\U\5\L\6\U\7\L\8|g' $1
	sed -i -e 's|first_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|First\U\1\L\2\U\3\L\4\U\5\L\6|g' $1
	sed -i -e 's|first_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|First\U\1\L\2\U\3\L\4|g' $1
	sed -i -e 's|first_\(.\)\([a-z]*\)|First\U\1\L\2|g' $1
	sed -i -e 's|last_\(.\)\([a-z]*\)|Last\U\1\L\2|g' $1
	sed -i -e 's|hash_value|HashValue|g' $1								
	sed -i -e 's|internal_object|InternalObject|g' $1								

	#The Attribute class
	sed -i -e 's|empty(|Empty(|g' $1
	sed -i -e 's|\([^e]\)name\([^s]\)|\1Name\2|g' $1
	sed -i -e 's|value|Value|g' $1
	sed -i -e 's|NextAttribute|GetNextAttribute|g' $1
	sed -i -e 's|PreviousAttribute|GetPreviousAttribute|g' $1

	sed -i -e 's|float AsFloat() const;|float AsFloat() const;\n		Real AsReal() const;\n		Whole AsWhole() const;\n		Integer AsInteger() const;\n		String AsString() const;|g' $1
	sed -i -e 's|float Attribute::AsFloat() const|Whole Attribute::AsWhole() const\n	{\n		if (!_attr \|\| !_attr->Value) return 0;\n		return ToWhole(_attr->Value);\n	}\n\n	Integer Attribute::AsInteger() const\n	{\n		if (!_attr \|\| !_attr->Value) return 0;\n		return ToInteger(_attr->Value);\n	}\n\n	String Attribute::AsString() const\n	{\n		if (!_attr \|\| !_attr->Value) return 0;\n		return ToString(_attr->Value);\n	}\n\n	Real Attribute::AsReal() const\n	{\n		if (!_attr \|\| !_attr->Value) return 0;\n		return ToReal(_attr->Value);\n	}\n\n	float Attribute::AsFloat() const|g' $1

	#The Node Class
	sed -i -e 's|child_\(.\)\([a-z]*\)|Child\U\1\L\2|g' $1
	sed -i -e 's|append_\(.\)\([a-z]*\)|Append\U\1\L\2|g' $1
	sed -i -e 's|prepend_\(.\)\([a-z]*\)|Prepend\U\1\L\2|g' $1
	sed -i -e 's|insert_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Insert\U\1\L\2\U\3\L\4|g' $1
	sed -i -e 's|insert_\(.\)\([a-z]*\)|Insert\U\1\L\2|g' $1
	sed -i -e 's|append_\(.\)\([a-z]*\)|Append\U\1\L\2|g' $1
	sed -i -e 's|remove_\(.\)\([a-z]*\)|Remove\U\1\L\2|g' $1
	sed -i -e 's|find_\(.\)\([a-zA-Z]*\)_\(.\)\([a-z]*\)|Find\U\1\L\2\U\3\L\4|g' $1
	sed -i -e 's|find_\(.\)\([a-z]*\)|Find\U\1\L\2|g' $1
	sed -i -e 's|select_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Find\U\1\L\2\U\3\L\4\U\5\L\6|g' $1
	sed -i -e 's|select_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Find\U\1\L\2\U\3\L\4|g' $1
	sed -i -e 's|select_\(.\)\([a-z]*\)|Find\U\1\L\2|g' $1
	sed -i -e 's|attr_\(.\)\([a-z]*\)|Attr\U\1\L\2|g' $1


	sed -i -e 's|\([^_]\)type\([^dn]\)|\1Type\2|g' $1
	sed -i -e 's|\([^_]\)attribute\([^_s]\)|\1GetAttribute\2|g' $1
	sed -i -e 's|FirstAttribute|GetFirstAttribute|g' $1
	sed -i -e 's|LastAttribute|GetLastAttribute|g' $1
	sed -i -e 's|\([^s]\)print\([^f]\)|\1Print\2|g' $1
	sed -i -e 's|FirstChild|GetFirstChild|g' $1
	sed -i -e 's|LastChild|GetLastChild|g' $1
	sed -i -e 's|parent|GetParent|g' $1
	sed -i -e 's|root|GetRoot|g' $1
	sed -i -e 's|child|GetChild|g' $1
	sed -i -e 's|NextSibling|GetNextSibling|g' $1
	sed -i -e 's|PreviousSibling|GetPreviousSibling|g' $1
	sed -i -e 's|traverse|Traverse|g' $1
	sed -i -e 's|path|Path|g' $1

	# NodeIterator
	sed -i -e 's|Value_type|value_type|g' $1
	
	# Document
	sed -i -e 's|reset|Reset|g' $1
	sed -i -e 's|load_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Format\U\1\L\2\U\3\L\4\U\5\L\6\U\7\L\8|g' $1	
	sed -i -e 's|load_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Load\U\1\L\2\U\3\L\4\U\5\L\6|g' $1	
	sed -i -e 's|load_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Load\U\1\L\2\U\3\L\4|g' $1
	sed -i -e 's|load_\(.\)\([a-z]*\)|Load\U\1\L\2|g' $1				
	sed -i -e 's|load|Load|g' $1
	sed -i -e 's|save_\(.\)\([a-z]*\)_\(.\)\([a-z]*\)|Save\U\1\L\2\U\3\L\4|g' $1
	sed -i -e 's|save_\(.\)\([a-z]*\)|Save\U\1\L\2|g' $1				
	sed -i -e 's|save|Save|g' $1
	sed -i -e 's|document_element|DocumentElement|g' $1

	#The TreeWalker Class
	sed -i -e 's|depth|Depth|g' $1

	#XPathVariable
	sed -i -e 's|get_\(.\)\([a-zA-Z]*\)|Get\U\1\L\2|g' $1
	sed -i -e 's|\([^f]\)set(|\1Set(|g' $1

	#XPathVariableSet
	sed -i -e 's|add(|Add(|g' $1
	sed -i -e 's|\([^r]\)get(|\1Get(|g' $1

	#XPathQuery
	sed -i -e 's|evaluate_\(.\)\([a-z]*\)|Evaluate\U\1\L\2|g' $1				
	sed -i -e 's|return_type|ReturnType|g' $1
	sed -i -e 's|result|Result|g' $1
	sed -i -e 's|std::exception|Mezzanine::Exception|g' $1

	#XPathNodeSet
	sed -i -e 's|Type_unsorted|TypeUnsorted|g' $1
	sed -i -e 's|Type_sorted_reverse|TypeSortedReverse|g' $1
	sed -i -e 's|Type_sorted|TypeSorted|g' $1
	sed -i -e 's|Type_t|CollectionType|g' $1

	#XPathNode
	sed -i -e 's|\([^_]\)node(|\1GetNode(|g' $1

	#typings and functions
	sed -i -e 's|GetMemory_allocation_function|GetMemoryAllocationFunction|g' $1
	sed -i -e 's|GetMemory_deallocation_function|GetMemoryDeallocationFunction|g' $1
	sed -i -e 's|allocation_function|AllocationFunction|g' $1
	sed -i -e 's|deallocation_function|DeallocationFunction|g' $1
	sed -i -e 's|pugi::|Mezzanine::xml::|g' $1

	#Common	
	sed -i -e 's|Nodeset|NodeSet|g' $1
	sed -i -e 's|ReSet|Reset|g' $1

	###############################
	#Documentation that was not put in xmldoc.h

	#Typedefs
	sed -i -e 's|// Character Type used for all internal storage and operations; depends on XML_WCHAR_MODE|/// @brief Used to represent a UTF8 code point, char, or wchar_t depending on how compiled. \n		/// @details compatibility with Mezzanine::Character is guaranteed. The exact type of this depends on depends on XML_WCHAR_MODE .\n		|' $1
	sed -i -e 's|// String Type used for operations that work with STL string; depends on XML_WCHAR_MODE|/// @brief Used to represent a string of UTF8 code point, char, or wchar_t depending on how compiled.\n		/// @details compatibility with Mezzanine::String is guaranteed. The exact type of this depends on depends on XML_WCHAR_MODE .\n		|' $1

	#Parsing status items
	sed -i -e 's|// Parsing Status (see ParseStatus)|/// @brief Parsing status ( see @ref ParseStatus )|' $1
	sed -i -e 's|// Last parsed Offset (in char_t units from start of input data)|/// @brief Last parsed offset (in char_t units from start of input data)|' $1
	sed -i -e 's|// Source document DocumentEncoding|/// @brief Source document encoding ( see @ref Encoding )|' $1
	sed -i -e 's|// Default constructor, initializes object to failed state|/// @brief Default constructor, initializes object to failed state.|' $1
	sed -i -e 's|// Cast to bool operator|/// @brief Cast to bool operator \n		/// @return This returns true if the ParseResult::Status member is set to ParseStatus::StatusOk, otherwise this returns false.|' $1
	sed -i -e 's|// Get error Description |/// @brief Creates a text description of the error situation, if any exists.\n		/// @return A const char* with a brief error description based on the ParseResult::Status |' $1
	sed -i -e 's|// Character type used for all internal storage and operations; depends on XML_WCHAR_MODE|/// @brief The character type used the internals of the XML parser, this is defined in datatypes.h.|' $1

	#Writer Interface class
	sed -i -e 's|		virtual ~Writer() {} |		/// @brief Empty Virtual Destructor\n		virtual ~Writer() {} |' $1
	sed -i -e 's|// Write memory chunk into stream/file/whatever |/// @brief This will be used to output xml to an external source such as a file or stream\n		/// @param data This is a pointer to something that will get written to the output\n		/// @param size The size in bytes of whatever was passed in.|' $1

	#WriterStream Class
	sed -i -e 's|// Construct WriterInstance from an output stream object |/// @brief A constructor that accepts a stream of characters\n		/// @param stream A stream to send stuff to.|' $1
	sed -i -e 's|WriterStream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream);|\n		/// @brief A constructor that accepts a stream of wide characters\n		/// @param stream A stream to send stuff to.\n		WriterStream(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >\& stream);|' $1
	sed -i -e 's|// Safe bool conversion operator|/// @brief Used to convert this to a boolean value in a safe way\n		/// @return Returns true if the internal data is set and false otherwise.|' $1
	sed -i -e 's|std::basic_ostream<wchar_t, std::char_traits<wchar_t> >\* wide_stream;|/// @internal\n		std::basic_ostream<wchar_t, std::char_traits<wchar_t> >* wide_stream;|' $1
	#Attribute
	sed -i -e 's|bool SetValue(const char_t\* rhs);|/// @brief Set the value of this.\n		/// @param rhs The new Value.\n		/// @return True if successful, returns false if this is empty or there is not enough memory.\n		/// @todo update this to make the error return code redundant and use an exception instead.\n		/// @todo Review for possiblity of buffer overflow.\n		bool SetValue(const char_t* rhs);|' $1
	sed -i -e 's|bool SetValue(int rhs);|/// @brief Convert rhs to a character array that contains rhs, then use that as the new value.\n		/// @param rhs The new value as an int.\n		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.\n		/// @todo update this to make the error return code redundant and use an exception instead.\n		/// @todo Review for possiblity of buffer overflow.\n		bool SetValue(int rhs);|' $1
	sed -i -e 's|bool SetValue(unsigned int rhs);|/// @brief Convert rhs to a character array that contains rhs, then use that as the new value.\n		/// @param rhs The new value as an unsigned int.\n		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.\n		/// @todo update this to make the error return code redundant and use an exception instead.\n		/// @todo Review for possiblity of buffer overflow.\n		bool SetValue(unsigned int rhs);|' $1
	sed -i -e 's|bool SetValue(double rhs);|/// @brief Convert rhs to a character array that contains rhs, then use that as the new value.\n		/// @param rhs The new value as a double.\n		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.\n		/// @todo update this to make the error return code redundant and use an exception instead.\n		/// @todo Review for possiblity of buffer overflow.\n		bool SetValue(double rhs);|' $1
	sed -i -e 's|bool SetValue(bool rhs);|/// @brief Convert rhs to a character array that contains the meaning of rhs, then use that as the new value.\n		/// @param rhs This with be interpretted, then converted to "true" or "false"  and used as the new value.\n		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.\n		/// @todo update this to make the error return code redundant and use an exception instead.\n		/// @todo Review for possiblity of buffer overflow.\n		bool SetValue(bool rhs);\n\n		/// @brief Convert rhs to a character array that contains the meaning of rhs, then use that as the new value.\n		/// @param rhs This with be converted to a character array using the appropriate streaming operator <<, then used as the new value.\n		/// @return True if successful, returns false if Attribute is empty or there is not enough memory.\n		/// @warning You should not pass classes that stream/serialize to xml into this function, the result will be invalid XML. If you must, find a way to strip out the ">" character, then you can reinsert it later\n		/// @todo Strip ">" automatically and provide a method to reconsitute it.\n		template <class T> bool SetValue(T rhs)\n		{\n			return SetValue(ToString(rhs).c_str());\n		}|' $1
	sed -i -e 's|typedef AttributeStruct\* Attribute::\*unspecified_bool_type;|/// @internal\n		/// @brief Used when testing an Attribute as a bool\n		typedef AttributeStruct* Attribute::*unspecified_bool_type;|' $1
#Attribute& operator=(const char_t* rhs);			// template + ToString

	#Node
	sed -i -e  's|// Constructs node from internal pointer|/// @brief Constructs node from internal pointer\n		/// @param p An internal node pointer.|' $1
	sed -i -e  's|Node GetNextSibling(const char_t\* Name) const; |/// @brief Attempt to retrieve the next sibling of this Node with a matching name.\n		/// @param Name A c-string that has the name of the node you to find.\n		/// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this iterates through Nodes until a sibling with a Matching name is found or all siblings are checked. If a Match is found this retrieves that Node, otherwise this return an empty Node.\n		/// @return A Node that represents a sibling with a matching name, or an empty Node on failure.\n		Node GetNextSibling(const char_t* Name) const; |' $1
	sed -i -e  's|Node GetPreviousSibling(const char_t\* Name) const; |/// @brief Attempt to retrieve the first previous sibling of this Node with a matching name.\n		/// @param Name A c-string that has the name of the node you to find.\n		/// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this iterates through Nodes until a sibling with a Matching name is found or all siblings are checked. If a Match is found this retrieves that Node, otherwise this return an empty Node.\n		/// @return A Node that represents a sibling with a matching name, or an empty Node on failure.\n		Node GetPreviousSibling(const char_t* Name) const; |' $1
	sed -i -e  's|Node GetPreviousSibling() const; |/// @brief Attempt to retrieve the previous sibling of this Node.\n		/// @details A sibling of a Node is another Node that shares the same parent. If this is and the sibling nodes are valid, this retrieves that Node, otherwise this return an empty Node.\n		/// @return A Node that represents a sibling, or an empty Node on failure.\n		Node GetPreviousSibling() const; |' $1
	sed -i -e  's|Attribute AppendAttribute(const char_t\* Name);|\n		/// @brief Creates an Attribute and puts it at the end of this Nodes attributes.\n		/// @param Name The name of the New attribute to be created\n		/// @details This attempts to create an Attribute and stick it at the end of the list of attribute on the current\n		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will\n		/// fail and return an empty attribute if this Node is empty.\n		/// @return The created Attribute or an empty Attribute on Failure.\n		Attribute AppendAttribute(const char_t* Name);\n		/// @brief Creates an Attribute and puts it at the end of this Nodes attributes.\n		/// @param Name The name of the New attribute to be created\n		/// @return The created Attribute or an empty Attribute on Failure.\n		Attribute AppendAttribute(const String\& Name)\n			{ return AppendAttribute(Name.c_str()); }|' $1
	sed -i -e  's|Attribute AppendCopy(const Attribute& proto);|\n		/// @brief Copies an Attribute and puts the copy at the end of this Nodes attributes.\n		/// @param proto The attribute to be copied.\n		/// @details This attempts to create a copy of an attribute Attribute and stick it at the end of the list of attribute on the current\n		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will\n		/// fail and return an empty attribute if this Node is empty.\n		/// @return The created Attribute or an empty Attribute on Failure.\n		Attribute AppendCopy(const Attribute\& proto); |' $1
	sed -i -e  's|Attribute PrependCopy(const Attribute& proto);|\n		/// @brief Copies an Attribute and puts the copy at the beginning of this Nodes attributes.\n		/// @param proto The attribute to be copied.\n		/// @details This attempts to create a copy of an attribute Attribute and stick it at the beginning of the list of attribute on the current\n		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will\n		/// fail and return an empty attribute if this Node is empty.\n		/// @return The created Attribute or an empty Attribute on Failure.\n		Attribute PrependCopy(const Attribute\& proto); |' $1
	sed -i -e  's|Attribute InsertCopyAfter(const Attribute& proto, const Attribute& attr);|\n		/// @brief Copies an Attribute and puts the copy into the list of this Nodes attributes.\n		/// @param proto The attribute to be copied.\n		/// @param attr An Attribute that represents an Attribute on this Node, and is just before where you want the new copy of proto.\n		/// @details This attempts to create a copy of an attribute Attribute and stick it in the middle of the list of attributes, just after a selected attribute, on the current\n		/// Node. This will fail and return an Empty Attribute if this Node is neither an Element nor a Declaration. This will\n		/// fail and return an empty attribute if this Node is empty.\n		/// @return The created Attribute or an empty Attribute on Failure.\n		Attribute InsertCopyAfter(const Attribute\& proto, const Attribute\& attr); |' $1
	sed -i -e  's|Node AppendChild(NodeType Type = NodeElement);|/// @brief Creates a Node and makes it a child of this one.\n		/// @param Type The NodeType of the Node to be added to list of child Nodes.\n		/// @return A Node representing the freshly added Node, or an empty Node if there was an error.\n		Node AppendChild(NodeType Type = NodeElement);|' $1
	sed -i -e  's|Node PrependChild(const char_t\* Name);|/// @brief Creates an element Node as a child of this Node, with the given name at the beginning of the children\n		/// @param Name The name of the Node to be created.\n		/// @details Calls @ref Node::PrependChild(NodeType); using NodeElement as the NodeType.\n		/// @return The desired Node on success, an empty Node on failure.\n		Node PrependChild(const char_t* Name);\n		/// @brief Creates an element Node as a child of this Node, with the given name at the beginning of the children\n		/// @param Name The name of the Node to be created.\n		/// @details Calls @ref Node::PrependChild(const char_t*);\n		/// @return The desired Node on success, an empty Node on failure.\n		Node PrependChild(const String\& Name)\n			{ return PrependChild(Name.c_str()); }|' $1
	sed -i -e  's|Node InsertChildBefore(const char_t\* Name, const Node& node);|/// @brief Creates an element Node as a child of this Node, with the given name at the middle of the children\n		/// @param Name The name of the Node to be created.\n		/// @param node The node just after were the Create node is to be placed.\n		/// @details Calls Node::InsertChildBefore(NodeType, Node); using NodeElement as the NodeType.\n		/// @return The desired Node on success, an empty Node on failure.\n		Node InsertChildBefore(const char_t* Name, const Node\& node);|' $1
	sed -i -e  's|Node InsertChildAfter(const char_t\* Name, const Node& node);|\n		/// @brief Creates an element Node as a child of this Node, with the given name at the middle of the children\n		/// @param Name The name of the Node to be created.\n		/// @param node The node just before were the Create node is to be placed.\n		/// @details Calls Node::InsertChildAfter(NodeType, Node); using NodeElement as the NodeType.\n		/// @return The desired Node on success, an empty Node on failure.\n		Node InsertChildAfter(const char_t* Name, const Node\& node);|' $1
	sed -i -e  's|Node PrependCopy(const Node& proto);|\n		/// @brief Copies a Node and puts the copy at the start of the list of this Nodes Childrem.\n		/// @param proto The Node to be copied. If this is emptry, no work is performed.\n		/// @return The copied Node on success, an empty Node on failure.\n		Node PrependCopy(const Node\& proto);|' $1
	sed -i -e  's|bool RemoveChild(const char_t\* Name);|\n		/// @brief Remove child element as specified by name.\n		/// @param Name The name of the Node to remove.\n		/// @return True if the removal was successful, false otherwise\n		bool RemoveChild(const char_t* Name);|' $1
	sed -i -e  's|XPathNodeSet FindNodes(const char_t\* query, XPathVariableSet\* variables = 0) const;|\n		/// @brief Select a group of nodes by evaluating an XPath query.\n		/// @param query The XPath query as a c-string to be evaluated.\n		/// @param variables undocumented.\n		/// @return An XPathNodeSet with the Matchs of the XPath query.\n		XPathNodeSet FindNodes(const char_t* query, XPathVariableSet* variables = 0) const; |' $1
	sed -i -e  's|XPathNodeSet FindNodes(const XPathQuery& query) const;|\n		/// @brief Select a group of nodes by evaluating an XPath query.\n		/// @param query The XPath query XPathQuery class instance.\n		/// @return An XPathNodeSet with the Matchs of the XPath query.\n		/// @param query The XPath query XPathQuery class instance.\n		XPathNodeSet FindNodes(const XPathQuery\& query) const; |' $1
	sed -i -e  's|void Print(Writer& WriterInstance, const char_t\* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;|\n		/// @brief Output the XML document using a Writer.\n		/// @param WriterInstance The Writer that will be used to output the xml text.\n		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.\n		/// @param DocumentEncoding The xml::Encoding of the document, whichs defaults to EncodingAuto\n		/// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.\n		/// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.\n		void Print(Writer\& WriterInstance, const char_t* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;|' $1
	sed -i -e  's|void Print(std::basic_ostream<char, std::char_traits<char> >& os, const char_t\* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;|\n		/// @brief Output the XML document using a Output Stream.\n		/// @param os An output stream to send xml text to.\n		/// @param indent The Character(s) used to represent a tab in the outpput, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault\n		/// @param DocumentEncoding The xml::Encoding of the document, whichs defaults to EncodingAuto\n		/// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.\n		/// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.\n		void Print(std::basic_ostream<char, std::char_traits<char> >\& os, const char_t* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;|' $1
	sed -i -e  's|void Print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const char_t\* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, unsigned int Depth = 0) const;|\n		/// @brief Output the XML document using a Output Stream.\n		/// @param os An output stream to send xml text to.\n		/// @param indent The Character(s) used to represent a tab in the outpput, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault\n		/// @param Depth This defaults to 0. The amount of times to prepend the indentation to the beginning of each output line.\n		/// @details This will never write a Byte Order Mark(BOM), and will default to not outputing a document declaration.\n		void Print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >\& os, const char_t* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, unsigned int Depth = 0) const;|' $1 
	sed -i -e 's|typedef NodeStruct\* Node::\*unspecified_bool_type;|/// @internal\n		/// @brief Used when testing an Attribute as a bool\n		typedef NodeStruct* Node::*unspecified_bool_type;|' $1
	sed -i -e 's|// Add a copy of the specified node as a GetChild. Returns added node, or empty node on errors.|/// @brief Copies a Node and puts the copy at the end of the list of this Nodes Childrem.\n		/// @param proto The Node to be copied. If this is emptry, no work is performed.\n		/// @return The copied Node on success, an empty Node on failure.|' $1
	sed -i -e 's|Node AppendChild(const char_t\* Name);|/// @brief Creates an element Node as a child of this Node, with the given name.\n		/// @param Name The name of the Node to be created.\n		/// @details Calls @ref Node::AppendChild(NodeType); using NodeElement as the NodeType.\n		/// @return The desired Node on success, an empty Node on failure.\n		Node AppendChild(const char_t* Name);\n		/// @brief Creates an element Node as a child of this Node, with the given name.\n		/// @param Name The name of the Node to be created.\n		/// @details Calls @ref Node::AppendChild(const char_t*)\n		/// @return The desired Node on success, an empty Node on failure.\n		Node AppendChild(const String\& Name)\n			{ return AppendChild(Name.c_str()); }|' $1
	sed -i -e 's|// Find GetAttribute using predicate. Returns first GetAttribute for which predicate returned true.|/// @brief Search for an Attribute using a function to check each Attribute individually.\n		/// @param pred a pointer to a function that accepts an Attribute, and returns bool.\n		/// @details This iterates through each Attribute on this node, from begining to end and calls the Predicate function passing\n		/// an Attribute to it. If the Predicate returns true the Node it was just passed is returned. If Precdicate never returns\n		/// true, it is called on every Node and a blank Node is returned. The Predicate is never called with a null value.\n		/// @return This returns the first Attribute that causes Predicate to return true.|' $1
	sed -i -e 's|// Find GetChild node using predicate. Returns first GetChild for which predicate returned true.|\n		/// @brief Search for an child ( only direct children ) Node using a function to check each Node individually.\n		/// @param pred a pointer to a function that accepts an Node, and returns bool.\n		/// @details This iterates through all immediate children of this Node and calls the Predicate function passing a Node to it. If\n		/// the Predicate returns true the Node it was just passed is returned. If Predicate never returns true, it is called\n		/// on every Node and a blank Node is returned. The Predicate is never called with a null value.\n		/// @return This returns the first Node that causes Predicate to return true.|' $1
	sed -i -e 's|// Find node from subtree using predicate. Returns first node from subtree (Depth-first), for which predicate returned true.|/// @brief Search for any Node descended from this Node using a function to check each Node individually.\n		/// @param pred a pointer to a function that accepts an Node, and returns bool.\n		/// @details This iterates through all children of this Node, and their children ( and so on), and calls the Predicate function\n		/// passing each Node to it. This iterates through all Nodes using a depth first algorithm. If the Predicate returns true the\n		/// Node it was just passed is returned. If Predicate never returns true, it is called on every Node and a blank Node is\n		/// returned. The Predicate is never called with a null value.\n		/// @return This returns the first Node that causes Predicate to return true.|' $1
	sed -i -e  's|Node InsertChildBefore(NodeType Type, const Node& node);|/// @brief Creates a Node and makes it a child of this one, and puts at the middle of the Child Nodes.\n		/// @param Type The NodeType of the Node to be added, just before another specific node.\n		/// @param node The specific node to add the new one before.\n		/// @return A Node representing the freshly added Node, or an empty Node if there was an error.\n		/// @todo Not all nodes can be added to other nodes, we need to figure it out and put it here.\n		Node InsertChildBefore(NodeType Type, const Node\& node);|' $1
	sed -i -e  's|Node InsertCopyBefore(const Node& proto, const Node& node);|/// @brief Copies a Node and puts the copy in the middle the list of this Nodes Childrem.\n		/// @param proto The Node to be copied. If this is emptry, no work is performed.\n		/// @param node The Node just after the desired place in the list of children to insert the copied node.\n		/// @return The copied Node on success, an empty Node on failure.\n		Node InsertCopyBefore(const Node\& proto, const Node\& node);|' $1
	sed -i -e  's|Node InsertCopyAfter(const Node& proto, const Node& node);|\n		/// @brief Copies a Node and puts the copy in the middle the list of this Nodes Childrem.\n		/// @param proto The Node to be copied. If this is emptry, no work is performed.\n		/// @param node The Node just before the desired place in the list of children to insert the copied node.\n		/// @return The copied Node on success, an empty Node on failure.\n		Node InsertCopyAfter(const Node\& proto, const Node\& node);|' $1
	sed -i -e  's|Node FindChildbyAttribute(const char_t\* AttrName, const char_t\* AttrValue) const;|/// @brief Find a Node by an Attribute it has.\n		/// @param AttrName The name of the matching Attribute.\n		/// @param AttrValue The value of the matching Attribute.\n		/// @details Any Null pointers instead of character arrays passed in will cause undefined behavior. All Matching is Case sensitive.\n		/// @return The First matching xml::Node\n		Node FindChildbyAttribute(const char_t* AttrName, const char_t* AttrValue) const;|' $1
	sed -i -e  's|ParseResult LoadFile(const char\* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|/// @brief Load document from file\n		/// @param Path An c-style char array that contains the path and filename of the xml document to load.\n		/// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto\n		/// @return A ParseResult that stores the the outcome of attempting to load the document.\n		ParseResult LoadFile(const char* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|' $1
	sed -i -e  's|ParseResult LoadFile(const wchar_t\* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|/// @brief Load document from file\n		/// @param Path An c-style wide char array that contains the path and filename of the xml document to load.\n		/// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto\n		/// @return A ParseResult that stores the the outcome of attempting to load the document.\n		ParseResult LoadFile(const wchar_t* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|' $1
	sed -i -e  's|bool SetName(const char_t\* rhs);|/// @brief Set the name of .\n		/// @param rhs The desired name.\n		/// @return True if successful, returns false if the name cannot be stored or there is not enough memory.\n		bool SetName(const char_t* rhs);\n		/// @brief Set the name of this object\n		/// @param rhs The desired name .\n		/// @return True if successful, returns false if the name cannot be stored or there is not enough memory.\n		bool SetName(const String\& rhs)\n			{ return SetName(rhs.c_str()); }|' $1
	sed -i -e  's|Node GetChild(const char_t\* Name) const;|/// @brief Attempt to get a child Node with a given name.\n		/// @param Name The name of the desired child Node.\n		/// @return A Node that represents the first desired child, or an empty Node on failure.\n		Node GetChild(const char_t* Name) const;\n		/// @brief Attempt to get a child Node with a given name.\n		/// @param Name The name of the desired child Node.\n		/// @return A Node that represents the first desired child, or an empty Node on failure.\n		Node GetChild(const String\& Name) const\n			{ return GetChild(Name.c_str()); }\n|' $1
#Attribute GetAttribute(const char_t* Name) const;
#Node GetNextSibling(const char_t* Name) const;
#Node GetPreviousSibling(const char_t* Name) const;
#const char_t* ChildValue(const char_t* Name) const;
#bool SetValue(const char_t* rhs);
#Attribute PrependAttribute(const char_t* Name); 
#Attribute InsertAttributeAfter(const char_t* Name, const Attribute& attr); 
#Attribute InsertAttributeBefore(const char_t* Name, const Attribute& attr); 
#Node InsertChildAfter(const char_t* Name, const Node& node);
#Node InsertChildBefore(const char_t* Name, const Node& node);
#bool RemoveAttribute(const char_t* Name);
#bool RemoveChild(const char_t* Name);
#Node FindChildbyAttribute(const char_t* Name, const char_t* AttrName, const char_t* AttrValue) const;
#Node FindChildbyAttribute(const char_t* AttrName, const char_t* AttrValue) const;
#XPathNode FindSingleNode(const char_t* query, XPathVariableSet* variables = 0) const;
#XPathNodeSet FindNodes(const char_t* query, XPathVariableSet* variables = 0) const;
#void Print(Writer& WriterInstance, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;
#void Print(std::basic_ostream<char, std::char_traits<char> >& os, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto, unsigned int Depth = 0) const;
#void Print(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& os, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, unsigned int Depth = 0) const;

	#Document
	sed -i -e  's|void Save(std::basic_ostream<char, std::char_traits<char> >& stream, const char_t\* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const; |/// @brief Save XML document to a stream.\n		/// @param stream The output stream to send the XML document to.\n		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.\n		void Save(std::basic_ostream<char, std::char_traits<char> >\& stream, const char_t* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;  |' $1 
	sed -i -e  's|void Save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t\* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault) const;|/// @brief Save XML document to a stream of wide characters.\n		/// @param stream The output stream of wide characters to send the XML document to.\n		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.\n		void Save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >\& stream, const char_t* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault) const; |' $1 
	sed -i -e  's|Node DocumentElement() const;|/// @brief Get document element\n		/// @return An xml::Node that is the root element of the xml Document\n		Node DocumentElement() const;|' $1 
	sed -i -e  's|// Save XML document to WriterInstance (semantics is slightly different from Node::Print, see documentation for details).|/// @brief Save XML document to WriterInstance.\n		/// @param WriterInstance The Writer that will be used to output the xml text.\n		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.|' $1 
	sed -i -e  's|// Save XML to file|/// @brief Save XML to file.\n		/// @param Path A c-style array of chars that contain the filename (and any path) of the file to be output.\n		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.\n		/// @return False if the target file could not be opened for writing|' $1 
	sed -i -e  's|ParseResult LoadBufferInplace(void\* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|/// @brief Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).\n		/// @details You should ensure that buffer data will persist throughout the documents lifetime, and free the buffer memory manually once document is destroyed.\n		/// @param contents A pointer to buffer containing the xml document to be parsed, that must remain for the lifecycle of the xml::Document.\n		/// @param size The size of the buffer.\n		/// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto\n		/// @return A ParseResult that stores the the outcome of attempting to load the document.\n		ParseResult LoadBufferInplace(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|' $1
	sed -i -e  's|ParseResult LoadBufferInplaceOwn(void\* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|/// @brief Load document from buffer, using the buffer for in-place parsing (the buffer is modified and used for storage of document data).\n		/// @details You should allocate the buffer with pugixml allocation function; xml::Document will free the buffer when it is no longer needed (you can not use it anymore).\n		/// @param contents A pointer to buffer containing the xml document to be parsed.\n		/// @param size The size of the buffer.\n		/// @param options A bitset of parse options that should be set using the Parse variables. This Defaults to ParseDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.\n		/// @return A ParseResult that stores the the outcome of attempting to load the document.\n		ParseResult LoadBufferInplaceOwn(void* contents, size_t size, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);|' $1 
	sed -i -e  's|bool SaveFile(const wchar_t\* Path, const char_t\* indent = XML_TEXT("\\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;|/// @brief Save XML to file.\n		/// @param Path A c-style array of wide chars that contain the filename (and any path) of the file to be output.\n		/// @param indent The Character(s) used to represent a tab in the output, this defaults to one tab character.\n		/// @param flags The output format flags, this is a bitfield that defaults to xml::FormatDefault.\n		/// @param DocumentEncoding What kind of text is in the stream, this defaults to Encoding::EncodingAuto.\n		/// @return False if the target file could not be opened for writing\n		bool SaveFile(const wchar_t* Path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;|' $1 
#ParseResult LoadFile(const char* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
#ParseResult LoadFile(const wchar_t* Path, unsigned int options = ParseDefault, Encoding DocumentEncoding = EncodingAuto);
#void Save(Writer& WriterInstance, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;
#void Save(std::basic_ostream<wchar_t, std::char_traits<wchar_t> >& stream, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault) const;
#bool SaveFile(const char* Path, const char_t* indent = XML_TEXT("\t"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;
#bool SaveFile(const wchar_t* Path, const char_t* indent = XML_TEXT("	"), unsigned int flags = FormatDefault, Encoding DocumentEncoding = EncodingAuto) const;

	#XPathVariable
	sed -i -e  's|// Get variable Name|/// @brief Retrieve the name of this variable.\n		/// @return A const c-style string made of char_t, that contains the name of this variable|' $1
	sed -i -e  's|// Get variable Type|/// @brief Get the variable type.\n		/// @return An XPathValueType specifying the kind of data stored in this variable.|' $1 
	sed -i -e  's|bool Set(double Value);|/// @brief Set variable Value; no Type conversion is performed.\n		/// @param Value The value as a double to attempt to put into this.\n		/// @return True is return, false is returned on Type mismatch error.\n		bool Set(double Value); |' $1 
	sed -i -e  's|bool Set(const char_t\* Value);|/// @brief Set variable Value; no Type conversion is performed.\n		/// @param Value The value as a char_t array to attempt to put into this.\n		/// @return True is return, false is returned on Type mismatch error.\n		bool Set(const char_t* Value); |' $1 
	sed -i -e  's|bool Set(const XPathNodeSet& Value);|/// @brief Set variable Value; no Type conversion is performed.\n		/// @param Value The value to attempt to put into this.\n		/// @return True is return, false is returned on Type mismatch error.\n		bool Set(const XPathNodeSet\& Value); |' $1 
	sed -i -e  's|bool GetBoolean() const;|/// @brief Get this as a bool.\n		/// @details Get variable Value; Minimal Type conversion is performed, default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error\n		/// @return This as a bool, with any appropriate downcasting.\n		bool GetBoolean() const;|' $1 
	sed -i -e  's|double GetNumber() const;|/// @details Get variable Value; default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error\n		/// @return This as a double.\n		/// @brief Get this as a double.\n		double GetNumber() const;|' $1 
	sed -i -e  's|const char_t\* GetString() const;|/// @brief Get this as a c-string.\n		/// @details Get variable Value; default Value (false, NaN, empty string, empty node set) is returned on Type mismatch error\n		/// @return This as a c-string of char_t, without conversion.\n		const char_t* GetString() const;|' $1 
	sed -i -e  's|const XPathNodeSet& GetNodeSet() const;|/// @brief Get this as a XPathNodeSet.\n		/// @details Get variable Value; no Type conversion is performed, if type of variable is not a XPathNodeSet then an XPathException is thrown\n		/// @throw XPathException on type mismatch or allocation error\n		/// @return A This as an XPathNodeSet, without conversion.\n		const XPathNodeSet\& GetNodeSet() const;|' $1 
#bool Set(const char_t* Value); //Get the Template ToString treatment

	#XPathVariableSet
	sed -i -e  's|bool Set(const char_t\* Name, double Value);|/// @brief Set contained variable Value; no Type conversion is performed.\n		/// @param Name The name of variable to change.\n		/// @param Value The value to attempt to put into the named variable.\n		/// @return True is return, false is returned if there is no such variable or on Type mismatch error.\n		bool Set(const char_t* Name, double Value);|' $1
	sed -i -e  's|bool Set(const char_t\* Name, const char_t\* Value);|/// @brief Set contained variable Value; no Type conversion is performed.\n		/// @param Name The name of variable to change.\n		/// @param Value The value to attempt to put into the named variable.\n		/// @return True is return, false is returned if there is no such variable or on Type mismatch error.\n		bool Set(const char_t* Name, const char_t* Value);|' $1 
	sed -i -e  's|bool Set(const char_t\* Name, const XPathNodeSet& Value);|/// @brief Set contained variable Value; no Type conversion is performed.\n		/// @param Name The name of variable to change.\n		/// @param Value The value to attempt to put into the named variable.\n		/// @return True is return, false is returned if there is no such variable or on Type mismatch error.\n		bool Set(const char_t* Name, const XPathNodeSet\& Value);|' $1 
	sed -i -e  's|// Set Value of an existing variable; no Type conversion is performed, false is returned if there is no such variable or if Types mismatch|/// @brief Set contained variable Value; no Type conversion is performed.\n		/// @param Name The name of variable to change.\n		/// @param Value The value to attempt to put into the named variable.\n		/// @return True is return, false is returned if there is no such variable or on Type mismatch error.|' $1
#bool Set(const char_t* Name, bool Value); 				//Get the Template ToString treatment
#bool Set(const char_t* Name, double Value);
#bool Set(const char_t* Name, const char_t* Value);
#bool Set(const char_t* Name, const XPathNodeSet& Value);
#XPathVariable* Get(const char_t* Name);
#const XPathVariable* Get(const char_t* Name) const;

	#XPathQuery
	sed -i -e  's|explicit XPathQuery(const char_t\* query, XPathVariableSet\* variables = 0);|\n		/// @brief Construct a compiled object from XPath expression.\n		/// @param query The query in the form of a c-string style char_t array.\n		/// @param variables Any extra data the query might need, passing a null pointer simply omits passing any arguments.\n		/// @throw If XML_NO_EXCEPTIONS is not defined (which is the default), throws XPathException on compilation errors.\n		explicit XPathQuery(const char_t* query, XPathVariableSet* variables = 0);|' $1
	sed -i -e 's|~XPathQuery();|/// @brief Destructor\n		~XPathQuery();|' $1
	sed -i -e 's|double EvaluateNumber(const XPathNode& n) const;|/// @brief Evaluate expression as double value in the specified context; performs Type conversion if necessary.\n		/// @param n The XPathNode that will serve as the context for the query.\n		/// @throw If XML_NO_EXCEPTIONS is not defined (by default it is not defined), throws std::bad_alloc on out of memory errors.\n		/// @return A result as a double from evaluating the expression.\n		double EvaluateNumber(const XPathNode\& n) const;|' $1
	sed -i -e 's|String EvaluateString(const XPathNode& n) const;|/// @brief Evaluate expression as string value in the specified context; performs Type conversion if necessary.\n		/// @param n The XPathNode that will serve as the context for the query.\n		/// @throw If XML_NO_EXCEPTIONS is not defined (by default it is not defined), throws std::bad_alloc on out of memory errors.\n		/// @return A result as a String from evaluating the expression.\n		String EvaluateString(const XPathNode\& n) const;|' $1
	sed -i -e 's|size_t EvaluateString(char_t\* buffer, size_t capacity, const XPathNode& n) const;|/// @brief Evaluate expression as string value in the specified context; performs Type conversion if necessary.\n		/// @param buffer The place to store the c-style Character array\n		/// @param capacity At most capacity characters are written to the destination buffer.\n		/// @param n The XPathNode that with serve as the context for the query.\n		/// @throw If XML_NO_EXCEPTIONS is not defined (by default it is not defined), throws std::bad_alloc on out of memory errors. If XML_NO_EXCEPTIONS is defined, this returns empty  set instead.\n		/// @return Full result size is returned (includes terminating zero).\n		size_t EvaluateString(char_t* buffer, size_t capacity, const XPathNode\& n) const;|' $1
	sed -i -e 's|XPathNodeSet EvaluateNodeSet(const XPathNode& n) const;|/// @brief Evaluate expression as node set in the specified context.\n		/// @param n The XPathNode that with serve as the context for the query.\n		/// @throw If XML_NO_EXCEPTIONS is not defined (by default it is not defined), throws throws XPathException on Type mismatch and std::bad_alloc on out of memory errors. If XML_NO_EXCEPTIONS is defined, returns empty node set instead.\n		/// @return An XPathNodeSet.\n		XPathNodeSet EvaluateNodeSet(const XPathNode\& n) const;|' $1
#explicit XPathQuery(const char_t* query, XPathVariableSet* variables = 0);			//ewww constructor
#size_t EvaluateString(char_t* buffer, size_t capacity, const XPathNode& n) const;

	#XPathException
	sed -i -e 's|explicit XPathException(const XPathParseResult& Result);|/// @brief Construct exception from parse Result\n		explicit XPathException(const XPathParseResult\& Result);|' $1

	#XPathNode
	sed -i -e 's|// Default constructor; constructs empty XPath node|/// @brief Default constructor; constructs empty XPath node|' $1
	sed -i -e 's|XPathNode(const Attribute& GetAttribute, const Node& GetParent);|/// @brief Construct From a xml::Attribute.\n		/// @param GetAttribute The xml::Attribute this handle should reference.\n		/// @param GetParent The xml::Node the xml::Attribute is on.\n		XPathNode(const Attribute\& GetAttribute, const Node\& GetParent); |' $1
	sed -i -e 's|bool operator==(const XPathNode& n) const;|/// @brief Called when comparing two XPathNode instances for equality.\n		/// @param n The other xml::XPathNode to compare this Node against for equality.\n		/// @return True if this and the other xml::XPathNode are referencing the same item, false otherwise;\n		bool operator==(const XPathNode\& n) const; |' $1
	sed -i -e 's|bool operator!=(const XPathNode& n) const;|\n		/// @brief Called when comparing two XPathNode instances for inequality.\n		/// @param n The other xml::XPathNode to compare this Node against for inequality.\n		/// @return True if this and the other xml::XPathNode are referencing the same item, false otherwise;\n		bool operator!=(const XPathNode\& n) const; |' $1

	#XPathNodeSet
	sed -i -e 's|XPathNodeSet(const XPathNodeSet& ns); |/// @brief Copy constructor.\n		/// @param ns The XPathNodeSet to copy.\n		XPathNodeSet(const XPathNodeSet\& ns); |' $1
sed -i -e 's|size_t size() const;|/// @brief Get collection size.\n		/// @return The number of XPathNode instances as a size_t in the set\n		size_t size() const; |' $1

	#other
	sed -i -e 's|//// @cond 0|/// @cond 0|' $1
	sed -i -e 's|//// @endcond|/// @endcond|' $1
		



}

FixNames $NewHFile
FixNames $NewCppFile

#we unset this, so we will set it again, even though on *Most* systems this isn't required
set +f
