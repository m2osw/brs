// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/brs
// contact@m2osw.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// Tell catch we want it to add the runner code in this file.
#pragma once

// catch2 lib
//
#include    <catch2/snapcatch2.hpp>


// C++ lib
//
#include    <string>
#include    <cstring>
#include    <cstdlib>
#include    <iostream>



namespace SNAP_CATCH2_NAMESPACE
{



extern char ** g_argv;



inline std::int64_t rand_int64()
{
    return (static_cast<std::int64_t>(rand()) << 48)
         | (static_cast<std::int64_t>(rand()) << 32)
         | (static_cast<std::int64_t>(rand()) << 16)
         | (static_cast<std::int64_t>(rand()) <<  0);
}



}
// namespace SNAP_CATCH2_NAMESPACE
// vim: ts=4 sw=4 et
