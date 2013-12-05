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
                          LOWER(r.name),
                          i.NOME
                          FROM 
                              SX_STATIONS s,
                              SX_POINTS p,
                              SX_REPORTS r,
                              SX_STATS_INFO i
                          WHERE
                              p.id = s.point_id AND
                              r.id = s.report_id AND
                              s.id = i.station_id AND
                              LOWER(r.name) NOT IN ('synop', 'temp', 'metar')
                          ORDER BY
                              s.id
                          """):
    s = "  [%d]={lat=%d,lon=%d,rep='%s'," % (row[0],row[1],row[2],row[3])

    if row[4]: s += "B01019='%s'," % (row[4].replace("'", "\\'"))
    s += "},"
    print(s)
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
                          v.ltype1,
                          v.l1,
                          v.ltype2,
                          v.l2
                          FROM 
                          SX_VARIABLES v,
                          SX_UNITS u
                          WHERE
                          u.id = v.unit_id
                          ORDER BY
                          v.id
                          """):
    s = "  [%d]={unit='%s',bcode='%s',tr=%d,p1=%d,p2=%d," % (row[0],row[1],row[2],int(row[3]),int(row[4]),int(row[5]))
    import math
    if (not math.isnan(row[6])): s += "lt1=%d," % (int(row[6]),)
    if (not math.isnan(row[7])): s += "l1=%d," % (int(row[7]),)
    if (not math.isnan(row[8])): s += "lt2=%d," % (int(row[8]),)
    if (not math.isnan(row[9])): s += "l2=%d," % (int(row[9]),)
    s += "},"
    print(s)
print(" }")


print("}")
