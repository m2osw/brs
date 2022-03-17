# - Try to find LibMimeMail
#
# Once done this will define
#
# BRS_FOUND        - System has LibMimeMail
# BRS_INCLUDE_DIRS - The LibMimeMail include directories
# BRS_LIBRARIES    - The libraries needed to use LibMimeMail
# BRS_DEFINITIONS  - Compiler switches required for using LibMimeMail
#
# License:
#
# Copyright (c) 2022  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/brs
# contact@m2osw.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

find_path(
    BRS_INCLUDE_DIR
        brs/version.h

    PATHS
        $ENV{BRS_INCLUDE_DIR}
)

find_library(
    BRS_LIBRARY
        brs

    PATHS
        $ENV{BRS_LIBRARY}
)

mark_as_advanced(
    BRS_INCLUDE_DIR
    BRS_LIBRARY
)

set(BRS_INCLUDE_DIRS ${BRS_INCLUDE_DIR})
set(BRS_LIBRARIES    ${BRS_LIBRARY})

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set BRS_FOUND to
# TRUE if all listed variables are TRUE
find_package_handle_standard_args(
    BRS
    DEFAULT_MSG
    BRS_INCLUDE_DIR
    BRS_LIBRARY
)

# vim: ts=4 sw=4 et
