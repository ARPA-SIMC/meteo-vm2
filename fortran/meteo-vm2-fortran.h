!
! meteo-vm2f - Fortran interface for meteo-vm2
!
! Copyright (C) 2012  ARPA-SIM <urpsim@smr.arpa.emr.it>
!
! This program is free software; you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation; either version 2 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License along
! with this program; if not, write to the Free Software Foundation, Inc.,
! 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
!
! Author: Emanuele Di Giacomo <edigiacomo@arpa.emr.it>

integer, parameter :: METEOVM2_ERROR_GENERIC = 1
integer, parameter :: METEOVM2_ERROR_ID_NOT_FOUND = 2
integer, parameter :: METEOVM2_ERROR_KEY_NOT_FOUND = 3
integer, parameter :: METEOVM2_ERROR_TYPE_CONVERSION = 4

! Open a new meteo-vm2 source
interface meteovm2_source_open
  ! Open the default meteo-vm2 source
  subroutine meteovm2_source_open_default(sourcehandle, errorcode)
    integer, intent(out) :: sourcehandle 
    integer, intent(out) :: errorcode
  end subroutine meteovm2_source_open_default
  ! Open a meteo-vm2 source by filename
  subroutine meteovm2_source_open_filename(sourcehandle, filename, errorcode)
    integer, intent(out) :: sourcehandle
    character (len=*), intent(in) :: filename
    integer, intent(out) :: errorcode
  end subroutine meteovm2_source_open_filename
end interface meteovm2_source_open

! Close a meteo-vm2
interface meteovm2_source_close
  subroutine meteovm2_source_close_handle(sourcehandle, errorcode)
    integer, intent(in) :: sourcehandle
    integer, intent(out) :: errorcode
  end subroutine meteovm2_source_close_handle
end interface meteovm2_source_close

! Get station attribute
interface meteovm2_get_station_attr
  ! Integer attribute
  subroutine meteovm2_get_station_attr_i(sourcehandle, id, key, value, errorcode)
    integer, intent(in) :: sourcehandle
    integer, intent(in) :: id
    character (len=*), intent(in) :: key
    integer, intent(out) :: value
    integer, intent(out) :: errorcode
  end subroutine meteovm2_get_station_attr_i
  ! String attribute
  subroutine meteovm2_get_station_attr_s(sourcehandle, id, key, value, errorcode)
    integer, intent(in) :: sourcehandle
    integer, intent(in) :: id
    character (len=*), intent(in) :: key
    character (len=*), intent(out) :: value
    integer, intent(out) :: errorcode
  end subroutine meteovm2_get_station_attr_s
end interface meteovm2_get_station_attr

! Get variable attribute
interface meteovm2_get_variable_attr
  ! Integer attribute
  subroutine meteovm2_get_variable_attr_i(sourcehandle, id, key, value, errorcode)
    integer, intent(in) :: sourcehandle
    integer, intent(in) :: id
    character (len=*), intent(in) :: key
    integer, intent(out) :: value
    integer, intent(out) :: errorcode
  end subroutine meteovm2_get_variable_attr_i
  ! String attribute
  subroutine meteovm2_get_variable_attr_s(sourcehandle, id, key, value, errorcode)
    integer, intent(in) :: sourcehandle
    integer, intent(in) :: id
    character (len=*), intent(in) :: key
    character (len=*), intent(out) :: value
    integer, intent(out) :: errorcode
  end subroutine meteovm2_get_variable_attr_s
end interface meteovm2_get_variable_attr

interface meteovm2_parser_open
  subroutine meteovm2_parser_open_filename(parserhandle, filename, errorcode)
    integer, intent(out) :: parserhandle
    character (len=*), intent(in) :: filename
    integer, intent(out) :: errorcode
  end subroutine meteovm2_parser_open_filename
end interface meteovm2_parser_open

interface meteovm2_parser_close
  subroutine meteovm2_parser_close_handle(parserhandle, errorcode)
    integer, intent(in) :: parserhandle
    integer, intent(out) :: errorcode 
  end subroutine meteovm2_parser_close_handle
end interface meteovm2_parser_close

interface meteovm2_parser_next
  subroutine meteovm2_parser_next_1(parserhandle, next, y, mo, d, h, mi, s, station, variable, v1, v2, v3, flags, errorcode)
    integer, intent(in) :: parserhandle
    logical, intent(out) :: next
    integer, intent(out) :: y, mo, d, h, mi, s
    integer, intent(out) :: station
    integer, intent(out) :: variable
    real, intent(out) :: v1
    real, intent(out) :: v2
    character, intent(out) :: v3
    character, intent(out) :: flags
    integer, intent(out) :: errorcode
  end subroutine meteovm2_parser_next_1
end interface meteovm2_parser_next
