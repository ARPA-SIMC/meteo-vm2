chunk = assert(loadfile(arg[1]))
source = chunk()

print("return {")
print("  stations={")

for idx, station in pairs(source.stations) do
	local s = string.format("    [%d]={B05001=%.5f,B06001=%.5f,B01194='%s',},", idx, station.lat/100000, station.lon/100000, station.rep)
	print(s)
end
print("  },")

print("  variables={")
for idx, variable in pairs(source.variables) do
	local s = string.format("    [%d]={unit='%s',bcode='%s',tr=%d,p1=%d,p2=%d,lt1=%d,", idx, variable.unit, variable.bcode, variable.tr, variable.p1, variable.p2, variable.lt1)
	if variable.l1 ~= nil then s = s .. string.format("l1=%d,", variable.l1) end
	if variable.lt2 ~= nil then s = s .. string.format("lt2=%d,", variable.lt2) end
	if variable.l2 ~= nil then s = s .. string.format("l2=%d,", variable.l2) end
	s = s .. "},"
	print(s)
end
print("  },")

print("}")
