#!/usr/bin/lua
-- once done this should be able to read a file and emit some tedious non abstractable c++ code.
-- it will define a number of exception classes that can be later customized, but really just need 
-- to be separate types for throwing/catching purposes

function trim(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end

function GetEnumRemark(Class, Dox)
  return ( "   ///< When used a instance of " .. Class .. " is thrown. " .. Dox .. "\n" )
end


SectionSeparator = "///////////////////////////////////////////////////////////////////////////////\n"

EnumOutput = SectionSeparator
EnumOutput = EnumOutput .. "// Exception code values\n"
EnumOutput = EnumOutput .. "enum ExceptionCodes\n{\n"

DefinitionOutput = SectionSeparator
DefinitionOutput = DefinitionOutput .. "// Exception code class definitions.\n\n"

First = false
EnumIdentifier = ""
EnumValue = ""
ClassName = ""
InheritsFrom = ""
ClassBrief = ""

Input = io.open("ExceptionDefinition.txt","r")
for line in Input:lines() do
    if string.sub(line,1,1) == "#" then
        -- Ignore remarked lines
    else    
        if First == false then
            First = true            
        else
            EnumOutput = EnumOutput .. ", " .. GetEnumRemark(ClassName, ClassBrief)
        end

        EnumIdentifier = trim(line:sub(1,39))
        EnumValue = trim(line:sub(40,49))
        ClassName = trim(line:sub(53,94))
        InheritsFrom = trim(line:sub(95,128))
        ClassBrief = trim(line:sub(129))
        print (EnumIdentifier .. ", " .. EnumValue .. ", " .. ClassName .. ", " .. InheritsFrom .. ", " .. ClassBrief)

        CurrentEnumLine = "    " .. EnumIdentifier
        SpaceCount = 40 - CurrentEnumLine:len()
        for i = 1,SpaceCount, 1 do
            CurrentEnumLine = CurrentEnumLine .. " "
        end
        CurrentEnumLine = CurrentEnumLine .. " = " .. EnumValue
        EnumOutput = EnumOutput .. CurrentEnumLine

        DefinitionOutput = DefinitionOutput .. SectionSeparator
                DefinitionOutput = DefinitionOutput  .. "/// @brief " .. ClassBrief .. "\n"
                DefinitionOutput = DefinitionOutput  .. "///////////////////\n"
                DefinitionOutput = DefinitionOutput  .. "class MEZZ_LIB " .. ClassName .. " : public " .. InheritsFrom .. "\n"
                DefinitionOutput = DefinitionOutput  .. "{\n"
                DefinitionOutput = DefinitionOutput  .. "public:\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @brief " .. ClassBrief .. "\n"
                DefinitionOutput = DefinitionOutput  .. "    static const Whole ExceptionCode = Exception::" .. EnumIdentifier .. ";\n"
                DefinitionOutput = DefinitionOutput  .. "\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @brief " .. ClassName .. " constructor.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param TypeName The name of this class.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param Message A basic description of the error.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param SrcFunction The name of the function from which this originated.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param SrcFile The name of the file from which this originated.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param FileLine The line on the named file from which this originated.\n"
                DefinitionOutput = DefinitionOutput  .. "    " .. ClassName .. "(const String& TypeName, const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)\n"
                DefinitionOutput = DefinitionOutput  .. "        : " .. InheritsFrom .. "(\"" .. ClassName .. "\", Message, SrcFunction, SrcFile, FileLine)\n"
                DefinitionOutput = DefinitionOutput  .. "        {}\n"
                DefinitionOutput = DefinitionOutput  .. "\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @brief " .. ClassName .. " constructor.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param Message A basic description of the error.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param SrcFunction The name of the function from which this originated.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param SrcFile The name of the file from which this originated.\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @param FileLine The line on the named file from which this originated.\n"
                DefinitionOutput = DefinitionOutput  .. "    " .. ClassName .. "(const String& Message, const String& SrcFunction, const String& SrcFile, const Whole& FileLine)\n"
                DefinitionOutput = DefinitionOutput  .. "        : " .. InheritsFrom .. "(\"" .. ClassName .. "\", Message, SrcFunction, SrcFile, FileLine)\n"
                DefinitionOutput = DefinitionOutput  .. "        {}\n"
                DefinitionOutput = DefinitionOutput  .. "\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @brief " .. ClassName .. " destructor.\n"
                DefinitionOutput = DefinitionOutput  .. "    virtual ~" .. ClassName .. "() throw() {}\n"
                DefinitionOutput = DefinitionOutput  .. "\n"
                DefinitionOutput = DefinitionOutput  .. "    /// @copydoc Exception::GetExceptionCode()\n"
                DefinitionOutput = DefinitionOutput  .. "    virtual Whole GetExceptionCode() const throw()\n"
                DefinitionOutput = DefinitionOutput  .. "        { return " .. ClassName .. "::ExceptionCode; }\n"
                DefinitionOutput = DefinitionOutput  .. "}; //" .. ClassName .. "\n"
                DefinitionOutput = DefinitionOutput  .. "\n"
                DefinitionOutput = DefinitionOutput  .. "/// @brief A template metaprogramming construct used to ensure that all required " .. ClassName .. " can be created at compile time if the compiler is clever. \n"
                DefinitionOutput = DefinitionOutput  .. "template<>\n"
                DefinitionOutput = DefinitionOutput  .. "struct MEZZ_LIB ExceptionFactory<" .. ClassName .. "::ExceptionCode>\n"
                DefinitionOutput = DefinitionOutput  .. "    { typedef " .. ClassName .. " Type; };\n"
                DefinitionOutput = DefinitionOutput  .. "\n"
                DefinitionOutput = DefinitionOutput  .. "\n"
    end

end
Input:close()

EnumOutput = EnumOutput .. "  " .. GetEnumRemark(ClassName, ClassBrief) .. "};"



print(EnumOutput)
print("\n\n")
print(DefinitionOutput)























