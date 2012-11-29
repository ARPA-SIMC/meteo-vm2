program source
  implicit none
  include "meteo-vm2-fortran.h"

  integer :: sourcehandle, errorcode

  integer :: ivalue
  character (len=255) :: svalue

  ! Opening default attribute file
  ! For a custom attribute file: 
  !   call meteovm2_source_open(sourcehandle, filename, errorcode)
  call meteovm2_source_open(sourcehandle, errorcode)
  if (errorcode .ne. 0) then
    PRINT *, "Error while opening default source"
    STOP 1
  endif

  ! Reading the "lon" attribute (integer) of the station #1
  call meteovm2_get_station_attr(sourcehandle, 1, "lon", ivalue, errorcode)

  if (errorcode .ne. 0) then
    if (errorcode .eq. METEOVM2_ERROR_KEY_NOT_FOUND) then
      PRINT *, "Key 'lon' not found"
    else
      PRINT *, "Error while reading station attribute 'lon'"
      STOP 1
    endif
  else
    PRINT *, "Longitude: ", real(ivalue) / 100000
  endif
  ! Reading the "rep" attribute (string) of the station #1
  call meteovm2_get_station_attr(sourcehandle, 1, "rep", svalue, errorcode)
  if (errorcode .ne. 0) then
    if (errorcode .eq. METEOVM2_ERROR_KEY_NOT_FOUND) then
      PRINT *, "Key 'rep' not found"
    else
      PRINT *, "Error while reading station attribute 'rep'"
      STOP 1
    endif
  else
    PRINT *, "Report: ", trim(svalue)
  endif

  ! Closing attribute file
  call meteovm2_source_close(sourcehandle, errorcode)

end program source
