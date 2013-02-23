#!/usr/bin/lua
-- once done this should be able to read a file and emit some tedious non abstractable c++ code.
-- it will define a number of exception classes that can be later customized, but really just need 
-- to be separate types for throwing/catching purposes

function trim(s)
  return (s:gsub("^%s*(.-)%s*$", "%1"))
end

SectionSeparator = "///////////////////////////////////////////////////////////////////////////////\n"

EnumOutput = SectionSeparator
EnumOutput = EnumOutput .. "// Exception code values\n"
EnumOutput = EnumOutput .. "enum ExceptionCodes\n{\n"

DefinitionOutput = SectionSeparator


Input = io.open("ExceptionDefinition.txt","r")
for line in Input:lines() do
	if string.sub(line,1,1) == "#" then
		-- Ignore remarked lines
	else
		EnumIdentifier = trim(line:sub(1,39))


		EnumOutput = EnumOutput .. EnumIdentifier .. ",\n"
	end
	

end
Input:close()


print(EnumOutput)
print("\n\n")
print(DefinitionOutput)
