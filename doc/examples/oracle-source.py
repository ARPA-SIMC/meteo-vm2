#!/usr/bin/python
import cx_Oracle
import sys

oracle = cx_Oracle.Connection(sys.argv[1]).cursor()

print("return {")
print(" stations={")
for row in oracle.execute("""
SELECT 
	s.id, 
	p.lat * 100000,
	p.lon * 100000,
	LOWER(r.name)
FROM 
	SX_STATIONS s, 
	SX_POINTS p, 
	SX_REPORTS r 
WHERE
	p.id = s.point_id AND
	r.id = s.report_id
ORDER BY
	s.id
"""):
	print("  [%d]={lat=%d,lon=%d,rep='%s'}," % (row[0],row[1],row[2],row[3]))
print(" },")

print(" variables={")
for row in oracle.execute("""
SELECT 
	v.id,
	u.abbreviazione,
	v.bcode,
	v.pind,
	v.p1,
	v.p2,
	NANVL(v.ltype1,NULL),
	NANVL(v.l1, NULL),
	NANVL(v.ltype2, NULL),
	NANVL(v.l2, NULL)
FROM 
	SX_VARIABLES v,
	SX_UNITS u
WHERE
	u.id = v.unit_id
ORDER BY
	v.id
"""):
	s = "  [%d]={unit='%s',bcode='%s',pind=%d,p1=%d,p2=%d," % (row[0],row[1],row[2],int(row[3]),int(row[4]),int(row[5]))
	if (not row[6] is None): s += "lt1=%d," % (int(row[6]))
	if (not row[7] is None): s += "l1=%d," % (int(row[7]))
	if (not row[8] is None): s += "lt2=%d." % (int(row[8]))
	if (not row[9] is None): s += "l2=%d," % (int(row[9]))
	s += "},"
	print(s)
print(" }")


print("}")
