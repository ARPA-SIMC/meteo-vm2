#!/bin/bash

EXIT_STATUS=0

# Print station id and unique keys
print_stations_keys()
{
	lua -e "source = require('default'); for k, v in pairs(source.stations) do print(k, v.B05001, v.B06001, v.B01194) end"
}
# Print variable id and unique keys
print_variables_keys()
{
	lua -e "source = require('default'); for k, v in pairs(source.variables) do print(k, v.bcode, v.tr, v.p1, v.p2, v.lt1, v.l1, v.lt2, v.l2) end"
}


# Test uniqueness of the stations
test_stations_uniqueness()
{
	n_stations=$(print_stations_keys|wc -l)
	n_stations_unique=$(print_stations_keys | awk '{ $1 = ""; print }' | sort -u | wc -l)
	[[ "$n_stations" -eq "$n_stations_unique" ]] || \
	{
		echo "Stations are not unique: $n_stations != $n_stations_unique"
		EXIT_STATUS=1
	}
	
}

# Test uniqueness of the variables
test_variables_uniqueness()
{
	n_variables=$(print_variables_keys|wc -l)
	n_variables_unique=$(print_variables_keys | awk '{ $1 = ""; print }' | sort -u | wc -l)
	[[ "$n_variables" -eq "$n_variables_unique" ]] || \
	{
		echo "Variables are not unique: $n_variables != $n_variables_unique"
		EXIT_STATUS=1
	}
	
}

test_stations_uniqueness
test_variables_uniqueness

exit $EXIT_STATUS
