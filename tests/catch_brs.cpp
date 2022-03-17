// Copyright (c) 2011-2022  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/edhttp
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

/** \file
 * \brief Verify the BRS functions.
 *
 * This file implements tests to verify that the BRS functions do what
 * they are expected to do.
 */

// self
//
#include    "catch_main.h"


// brs
//
#include    <brs/brs.h>



CATCH_TEST_CASE("basic_types", "[basic]")
{
    CATCH_SECTION("push/restore char")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        char value = 33;
        brs::add_value(buffer, "orange", value);

        // make sure it did not get smashed
        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(buffer[4] == 6);  // hunk_sizes_t
        CATCH_REQUIRE(buffer[5] == 1);
        CATCH_REQUIRE(buffer[6] == 0);
        CATCH_REQUIRE(buffer[7] == 0);

        CATCH_REQUIRE(buffer[8] == 'o');    // name
        CATCH_REQUIRE(buffer[9] == 'r');
        CATCH_REQUIRE(buffer[10] == 'a');
        CATCH_REQUIRE(buffer[11] == 'n');
        CATCH_REQUIRE(buffer[12] == 'g');
        CATCH_REQUIRE(buffer[13] == 'e');

        CATCH_REQUIRE(buffer[14] == 33);   // value

        struct obj
            : public brs::brs_object
        {
            virtual bool process_chunk(
                          brs::name_t const & name
                        , std::uint8_t const * data
                        , std::size_t size) override
            {
                CATCH_REQUIRE(name == "orange");
                CATCH_REQUIRE(size == 1);
                CATCH_REQUIRE(data[0] == 33);
                return true;
            }
        };
        obj::pointer_t o(std::make_shared<obj>());

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool r(brs::unserialize_buffer(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore short")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        std::int16_t purple = 3003;
        brs::add_value(buffer, "purple", purple);

        std::uint16_t black = 65001;
        brs::add_value(buffer, "black", black);

        // make sure it did not get smashed
        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(buffer[4] == 6);  // hunk_sizes_t
        CATCH_REQUIRE(buffer[5] == 2);
        CATCH_REQUIRE(buffer[6] == 0);
        CATCH_REQUIRE(buffer[7] == 0);

        CATCH_REQUIRE(buffer[8] == 'p');    // name
        CATCH_REQUIRE(buffer[9] == 'u');
        CATCH_REQUIRE(buffer[10] == 'r');
        CATCH_REQUIRE(buffer[11] == 'p');
        CATCH_REQUIRE(buffer[12] == 'l');
        CATCH_REQUIRE(buffer[13] == 'e');

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        CATCH_REQUIRE(buffer[14] * 256 + buffer[15] == 3003);   // value
#else
        CATCH_REQUIRE(buffer[14] + buffer[15] * 256 == 3003);   // value
#endif

        struct obj
            : public brs::brs_object
        {
            virtual bool process_chunk(
                          brs::name_t const & name
                        , std::uint8_t const * data
                        , std::size_t size) override
            {
                if(name == "purple")
                {
                    CATCH_REQUIRE(size == 2);
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                    CATCH_REQUIRE(data[0] * 256 + data[1] == 3003);
#else
                    CATCH_REQUIRE(data[0] + data[1] * 256 == 3003);
#endif
                }
                else if(name == "black")
                {
                    CATCH_REQUIRE(size == 2);
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                    CATCH_REQUIRE(data[0] * 256 + data[1] == 65001);
#else
                    CATCH_REQUIRE(data[0] + data[1] * 256 == 65001);
#endif
                }
                else
                {
                    CATCH_REQUIRE(name == "?unknown?");
                }
                return true;
            }
        };
        obj::pointer_t o(std::make_shared<obj>());

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool r(brs::unserialize_buffer(buffer, cb, true));
        CATCH_REQUIRE(r);
    }
}


// vim: ts=4 sw=4 et
