program test
  implicit none
  include "meteo-vm2-fortran.h"

  call test_source()

  call test_station_attributes()

  call test_parser()
end program test

subroutine test_source()
  include "meteo-vm2-fortran.h"
  integer :: sourcehandle, errorcode
  character (len=255) :: filename;

  call meteovm2_source_open(sourcehandle, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while opening default source"
    STOP 1
  endif

  call meteovm2_source_close(sourcehandle, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while closing default source"
    STOP 1
  endif

  call getenv("METEO_VM2_SOURCE", filename)

  filename = trim(filename)

  call meteovm2_source_open(sourcehandle, filename, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while opening source '", trim(filename), "'"
    STOP 1
  endif

  call meteovm2_source_close(sourcehandle, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while closing source '", trim(filename), "'"
    STOP 1
  endif

  PRINT *, "test_source: OK"
end subroutine

subroutine test_station_attributes()
  include "meteo-vm2-fortran.h"
  integer :: sourcehandle, errorcode
  integer :: ivalue
  character (len=80) :: svalue

  call meteovm2_source_open(sourcehandle, errorcode)

  call meteovm2_get_station_attr(sourcehandle, 1, "lon", ivalue, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while reading station attribute 'lon'"
    STOP 1
  endif

  if (ivalue .ne. 1207738) then
    PRINT *, "Error: expected 1207738, was: ", ivalue
    STOP 1
  endif

  call meteovm2_get_station_attr(sourcehandle, 1, "rep", svalue, errorcode)
  if (errorcode .ne. 0) then
    PRINT *, "Error while reading station attribute 'rep'"
    STOP 1
  endif

  if (trim(svalue) .ne. "locali") then
    PRINT *, "Error: expected 'locali', was: ", trim(svalue)
  endif

  call meteovm2_source_close(sourcehandle, errorcode)

  PRINT *, "test_station_attributes: OK"
end subroutine

subroutine test_parser()
  include "meteo-vm2-fortran.h"
  character (len=255) :: filename
  
  integer :: parserhandle, errorcode
  logical :: havenext
  integer :: year, month, day, hour, mins, sec
  integer :: station, variable
  real :: value1, value2
  character (len=30) :: value3, flags
  
  call getenv("METEO_VM2_INPUT_TEST", filename)
  filename = trim(filename)

  call meteovm2_parser_open(parserhandle, filename, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while opening parser for file ", filename
    STOP 1
  endif

  call meteovm2_parser_next(parserhandle, havenext, &
    year, month, day, hour, mins, sec, &
    station, variable, &
    value1, value2, value3, flags, errorcode)
  if (errorcode .ne. 0) then
          PRINT *, "Error while reading file"
          STOP 1
  endif

  if (year .ne. 1987) then
    PRINT *, "year - expected 1987, was: ", year
    STOP 1
  endif

  if (value1 .ne. 1.2) then
    PRINT *, "value1 - expected 1.2, was: ", value1
    STOP 1
  endif

  if (value2 * 0 .eq. 0) then
     PRINT *, "value2 - expected Infinity, was: ", value2
     STOP 1
  endif

  call meteovm2_parser_close(parserhandle, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while closing parser"
    STOP 1
  endif

  PRINT *, "test_parser: OK"
end subroutine test_parser
