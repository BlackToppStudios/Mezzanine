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

First = false

Input = io.open("ExceptionDefinition.txt","r")
for line in Input:lines() do
	if string.sub(line,1,1) == "#" then
		-- Ignore remarked lines
	else
		if First == false then
			First = true			
		else
			EnumOutput = EnumOutput .. ", ///<\n"
		end

		EnumIdentifier = trim(line:sub(1,39))
		EnumValue = trim(line:sub(40,49))
		ClassName = trim(line:sub(53,94))
		InheritsFrom = trim(line:sub(95))


		-- print (EnumIdentifier .. ", " .. EnumValue .. ", " .. ClassName .. ", " .. InheritsFrom .. ", ")
		EnumOutput = EnumOutput .. EnumIdentifier

		SpaceCount = 50 - EnumOutput:len()
		for i = 1,SpaceCount, 1 do
			print (i)
			EnumOutput = EnumOutput .. " "
		end
		EnumOutput = EnumOutput .. " = " .. EnumValue

	end

end

EnumOutput = EnumOutput .. " ///<\n"
Input:close()


print(EnumOutput)
print("\n\n")
print(DefinitionOutput)
