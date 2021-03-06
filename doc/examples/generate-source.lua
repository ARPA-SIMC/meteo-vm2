--[[
This example shows how to create an VM2 attributes file, 
loading data from a postgresql database
The resulting file is in the form:
	
	return {
		stations={
			[123]={...},
			[456]={...},
			...
		},
		variables={
			[789]={...},
			[123]={...}
		}
	}

]]--

io.write("return {\n")
-- open database
require "luasql.postgres"
env = assert(luasql.postgres())
con = assert(env:connect("edigiacomo"))
-- fetch stations

io.write(" stations={\n")
cur = assert(con:execute("SELECT id, ST_X(geom) as longitude, ST_Y(geom) as latitude, report FROM core.stations ORDER BY id"))
row = cur:fetch({}, "a")
while row do
	s = string.format("  [%d]={lon=%d,lat=%d,rep=%q},\n",row.id, row.longitude*100000, row.latitude*100000, row.report)
	io.write(s)
	row = cur:fetch(row, "a")
end
cur:close()
io.write(" },\n")
-- fetch variables
io.write(" variables={\n")
cur = assert(con:execute("SELECT id, bcode, lt1, NULLIF(l1,'NaN') as l1, NULLIF(lt2,'NaN') as lt2, NULLIF(l2,'NaN') as l2, tr, p1, p2, unit FROM core.variables ORDER BY id"))
row = cur:fetch({}, "a")
while row do
	s = string.format("  [%d]={unit=%q,bcode=%q,tr=%d,p1=%d,p2=%d,lt1=%d,",row.id, row.unit, row.bcode, row.tr, row.p1, row.p2, row.lt1)
	if (row.l1 ~= nil) then s = s .. string.format("l1=%d,", row.l1) end
	if (row.lt2 ~= nil) then s = s .. string.format("l1=%d,", row.lt2) end
	if (row.l2 ~= nil) then s = s .. string.format("l1=%d,", row.l2) end
	s = s .. "},\n"
	io.write(s)
	row = cur:fetch(row, "a")
end
cur:close()
io.write(" }\n");
-- closing database
con:close()
env:close()

io.write("}\n")
