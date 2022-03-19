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
                CATCH_REQUIRE(to_char(data, size) == 33);
                return true;
            }
        };
        obj::pointer_t o(std::make_shared<obj>());

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool const r(brs::unserialize_buffer(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore signed char")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        signed char value = -43;
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

        CATCH_REQUIRE(static_cast<signed char>(buffer[14]) == -43);   // value

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
                CATCH_REQUIRE(static_cast<signed char>(data[0]) == -43);
                CATCH_REQUIRE(to_schar(data, size) == -43);
                return true;
            }
        };
        obj::pointer_t o(std::make_shared<obj>());

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool const r(brs::unserialize_buffer(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore unsigned char")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        unsigned char value = 200;
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

        CATCH_REQUIRE(static_cast<unsigned char>(buffer[14]) == 200);   // value

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
                CATCH_REQUIRE(static_cast<unsigned char>(data[0]) == 200);
                CATCH_REQUIRE(to_uchar(data, size) == 200);
                return true;
            }
        };
        obj::pointer_t o(std::make_shared<obj>());

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool const r(brs::unserialize_buffer(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore shorts (16 bits)")
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
                    std::int16_t value(to_int16(data, size));
                    CATCH_REQUIRE(value == 3003);
                }
                else if(name == "black")
                {
                    CATCH_REQUIRE(size == 2);
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                    CATCH_REQUIRE(data[0] * 256 + data[1] == 65001);
#else
                    CATCH_REQUIRE(data[0] + data[1] * 256 == 65001);
#endif
                    std::uint16_t value(to_uint16(data, size));
                    CATCH_REQUIRE(value == 65001);
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
        bool const r(brs::unserialize_buffer(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore ints (32 bits)")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        std::int32_t red = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        brs::add_value(buffer, "red", red);

        std::uint32_t blue = static_cast<std::int32_t>(SNAP_CATCH2_NAMESPACE::rand_int64());
        brs::add_value(buffer, "blue", blue);

        // make sure it did not get smashed
        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(buffer[4] == 3);  // hunk_sizes_t
        CATCH_REQUIRE(buffer[5] == 4);
        CATCH_REQUIRE(buffer[6] == 0);
        CATCH_REQUIRE(buffer[7] == 0);

        CATCH_REQUIRE(buffer[8] == 'r');    // name
        CATCH_REQUIRE(buffer[9] == 'e');
        CATCH_REQUIRE(buffer[10] == 'd');

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        CATCH_REQUIRE(
                  buffer[11] * 0x1000000
                + buffer[12] * 0x10000
                + buffer[13] * 0x100
                + buffer[14] * 0x1 == red);   // value
#else
        CATCH_REQUIRE(
                  buffer[14] * 0x1000000
                + buffer[13] * 0x10000
                + buffer[12] * 0x100
                + buffer[11] * 0x1 == red);   // value
#endif

        struct obj
            : public brs::brs_object
        {
            typedef std::shared_ptr<obj>        pointer_t;

            virtual bool process_chunk(
                          brs::name_t const & name
                        , std::uint8_t const * data
                        , std::size_t size) override
            {
                if(name == "red")
                {
                    CATCH_REQUIRE(size == 4);
                    std::int32_t const value(to_int32(data, size));
                    CATCH_REQUIRE(value == f_red);
                }
                else if(name == "blue")
                {
                    CATCH_REQUIRE(size == 4);
                    std::uint32_t const value(to_uint32(data, size));
                    CATCH_REQUIRE(value == f_blue);
                }
                else
                {
                    CATCH_REQUIRE(name == "?unknown?");
                }
                return true;
            }

            void set_red(std::int32_t value)
            {
                f_red = value;
            }

            void set_blue(std::uint32_t value)
            {
                f_blue = value;
            }

        private:
            std::int32_t    f_red = 0;
            std::uint32_t   f_blue = 0;
        };
        obj::pointer_t o(std::make_shared<obj>());
        o->set_red(red);
        o->set_blue(blue);


        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool const r(brs::unserialize_buffer(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore ints (64 bits)")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        std::int64_t white = SNAP_CATCH2_NAMESPACE::rand_int64();
        brs::add_value(buffer, "white", white);

        std::uint64_t gray = SNAP_CATCH2_NAMESPACE::rand_int64();
        brs::add_value(buffer, "gray", gray);

        // make sure it did not get smashed
        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(buffer[4] == 5);  // hunk_sizes_t
        CATCH_REQUIRE(buffer[5] == 8);
        CATCH_REQUIRE(buffer[6] == 0);
        CATCH_REQUIRE(buffer[7] == 0);

        CATCH_REQUIRE(buffer[8] == 'w');    // name
        CATCH_REQUIRE(buffer[9] == 'h');
        CATCH_REQUIRE(buffer[10] == 'i');
        CATCH_REQUIRE(buffer[11] == 't');
        CATCH_REQUIRE(buffer[12] == 'e');

        struct obj
            : public brs::brs_object
        {
            typedef std::shared_ptr<obj>        pointer_t;

            virtual bool process_chunk(
                          brs::name_t const & name
                        , std::uint8_t const * data
                        , std::size_t size) override
            {
                if(name == "white")
                {
                    CATCH_REQUIRE(size == 8);
                    std::int64_t const value(to_int64(data, size));
                    CATCH_REQUIRE(value == f_white);
                }
                else if(name == "gray")
                {
                    CATCH_REQUIRE(size == 8);
                    std::uint64_t const value(to_uint64(data, size));
                    CATCH_REQUIRE(value == f_gray);
                }
                else
                {
                    CATCH_REQUIRE(name == "?unknown?");
                }
                return true;
            }

            void set_white(std::int64_t value)
            {
                f_white = value;
            }

            void set_gray(std::uint64_t value)
            {
                f_gray = value;
            }

        private:
            std::int64_t    f_white = 0;
            std::uint64_t   f_gray = 0;
        };
        obj::pointer_t o(std::make_shared<obj>());
        o->set_white(white);
        o->set_gray(gray);

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool const r(brs::unserialize_buffer<obj::pointer_t>(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore floats")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        float green = static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64())
                    / static_cast<float>(SNAP_CATCH2_NAMESPACE::rand_int64());
        brs::add_value(buffer, "green", green);

        double yellow = static_cast<double>(SNAP_CATCH2_NAMESPACE::rand_int64())
                      / static_cast<double>(SNAP_CATCH2_NAMESPACE::rand_int64());
        brs::add_value(buffer, "yellow", yellow);

        long double fushia = static_cast<long double>(SNAP_CATCH2_NAMESPACE::rand_int64())
                           / static_cast<long double>(SNAP_CATCH2_NAMESPACE::rand_int64());
        brs::add_value(buffer, "fushia", fushia);

        // make sure it did not get smashed
        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(buffer[4] == 5);  // hunk_sizes_t
        CATCH_REQUIRE(buffer[5] == 4);
        CATCH_REQUIRE(buffer[6] == 0);
        CATCH_REQUIRE(buffer[7] == 0);

        CATCH_REQUIRE(buffer[8] == 'g');    // name
        CATCH_REQUIRE(buffer[9] == 'r');
        CATCH_REQUIRE(buffer[10] == 'e');
        CATCH_REQUIRE(buffer[11] == 'e');
        CATCH_REQUIRE(buffer[12] == 'n');

        struct obj
            : public brs::brs_object
        {
            typedef std::shared_ptr<obj>        pointer_t;

            virtual bool process_chunk(
                          brs::name_t const & name
                        , std::uint8_t const * data
                        , std::size_t size) override
            {
                if(name == "green")
                {
                    CATCH_REQUIRE(size == 4);
                    std::int64_t const value(to_float(data, size));
                    CATCH_REQUIRE(value == f_green);
                }
                else if(name == "yellow")
                {
                    CATCH_REQUIRE(size == 8);
                    std::uint64_t const value(to_double(data, size));
                    CATCH_REQUIRE(value == f_yellow);
                }
                else if(name == "fushia")
                {
                    CATCH_REQUIRE(size == 16);
                    std::uint64_t const value(to_long_double(data, size));
                    CATCH_REQUIRE(value == f_fushia);
                }
                else
                {
                    CATCH_REQUIRE(name == "?unknown?");
                }
                return true;
            }

            void set_green(std::int64_t value)
            {
                f_green = value;
            }

            void set_yellow(std::uint64_t value)
            {
                f_yellow = value;
            }

            void set_fushia(std::uint64_t value)
            {
                f_fushia = value;
            }

        private:
            std::int64_t    f_green = 0;
            std::uint64_t   f_yellow = 0;
            std::uint64_t   f_fushia = 0;
        };
        obj::pointer_t o(std::make_shared<obj>());
        o->set_green(green);
        o->set_yellow(yellow);
        o->set_fushia(fushia);

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool const r(brs::unserialize_buffer<obj::pointer_t>(buffer, cb, true));
        CATCH_REQUIRE(r);
    }

    CATCH_SECTION("push/restore string")
    {
        brs::buffer_t buffer;

        brs::add_magic(buffer);

        CATCH_REQUIRE(buffer.size() == sizeof(brs::magic_t));

        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        std::string message("this is the message we are going to serialize");
        brs::add_value(buffer, "message",  message);

        // make sure it did not get smashed
        CATCH_REQUIRE(buffer[0] == 'B');
        CATCH_REQUIRE(buffer[1] == 'R');
        CATCH_REQUIRE(buffer[2] == 'S');
        CATCH_REQUIRE(buffer[3] == brs::BRS_VERSION);

        CATCH_REQUIRE(buffer[4] == 7);  // hunk_sizes_t
        CATCH_REQUIRE(buffer[5] == 45);
        CATCH_REQUIRE(buffer[6] == 0);
        CATCH_REQUIRE(buffer[7] == 0);

        CATCH_REQUIRE(buffer[8] == 'm');    // name
        CATCH_REQUIRE(buffer[9] == 'e');
        CATCH_REQUIRE(buffer[10] == 's');
        CATCH_REQUIRE(buffer[11] == 's');
        CATCH_REQUIRE(buffer[12] == 'a');
        CATCH_REQUIRE(buffer[13] == 'g');
        CATCH_REQUIRE(buffer[14] == 'e');

        struct obj
            : public brs::brs_object
        {
            typedef std::shared_ptr<obj>        pointer_t;

            virtual bool process_chunk(
                          brs::name_t const & name
                        , std::uint8_t const * data
                        , std::size_t size) override
            {
                CATCH_REQUIRE(name == "message");
                CATCH_REQUIRE(size == 45);
                std::string const value(to_string(data, size));
                CATCH_REQUIRE(value == f_message);
                return true;
            }

            void set_message(std::string value)
            {
                f_message = value;
            }

        private:
            std::string     f_message = std::string();
        };
        obj::pointer_t o(std::make_shared<obj>());
        o->set_message(message);

        snapdev::callback_manager<obj::pointer_t> cb;
        CATCH_REQUIRE(cb.add_callback(o) != snapdev::callback_manager<obj::pointer_t>::NULL_CALLBACK_ID);

        // WARNING: we want to use CATCH_...() macros inside the callback
        //          so make sure not to use one around unserialize_buffer().
        //
        bool const r(brs::unserialize_buffer<obj::pointer_t>(buffer, cb, true));
        CATCH_REQUIRE(r);
    }
}


// vim: ts=4 sw=4 et
