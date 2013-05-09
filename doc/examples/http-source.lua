--[[
Attributes from CGI script, assuming that the response is a JSON object with
the following form:

  {
    "stations": [ { "id": ID, ... }, { "id": ID, ... } ],
    "variables": [ { "id": ID, ... }, { "id": ID, ... } ],
  }

]]--
-- Libraries
local http = require('socket.http')
local json = require('json')
-- URL of the CGI script
local url = "http://localhost:8080/source.json"
-- Retrieving data
local resp = http.request(url)
-- JSON decoding
local jsonresp = json.decode(resp)
-- Attributes table
local attrs = { stations={} , variables={} }
-- Iterating over the JSON response
for _, v in ipairs(jsonresp.stations) do
	attrs.stations[v.id] = v
end
for _, v in ipairs(jsonresp.variables) do
	attrs.variables[v.id] = v
end
-- Returning attributes
return attrs
