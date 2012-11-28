program parser
  include "meteo-vm2-fortran.h"

  character (len=255) :: filename
  integer :: parserhandle, errorcode
  logical :: havenext
  integer :: year, month, day, hour, mins, sec
  integer :: station, variable
  real :: value1, value2
  character (len=30) :: value3, flags

  call getarg(1, filename)
  filename = trim(filename)

  ! Create a VM2 parser
  call meteovm2_parser_open(parserhandle, filename, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while opening parser for file ", filename
    STOP 1
  endif

  do 
    ! Read the next VM2 message.
    call meteovm2_parser_next(parserhandle, havenext, &
      year, month, day, hour, mins, sec, &
      station, variable, &
      value1, value2, value3, flags, errorcode)

    if (errorcode .ne. 0) then
      PRINT *, "Error while reading file"
      STOP 1
    endif

    if (.not. havenext) then
      PRINT *, "end of VM2 file reached"
      EXIT
    endif

    PRINT *, "values: ", value1, ", ", value2, ", ", value3

  enddo

  ! Close the parser
  call meteovm2_parser_close(parserhandle, errorcode)

  if (errorcode .ne. 0) then
    PRINT *, "Error while closing parser"
    STOP 1
  endif

end subroutine parser
