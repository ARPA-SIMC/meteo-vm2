#!/usr/bin/env python
import bottle

@bottle.get("/source.json")
def get_source():
	"""
	Return source table
	"""
	return {
		'stations': [{ 'id': i, 'value': i } for i in range(1, 9000)],
		'variables': [{ 'id': i, 'value': i } for i in range(1, 500) ]
	}

# Run the web server as localhost:8080
bottle.run()
