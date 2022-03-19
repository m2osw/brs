// Copyright (c) 2022  Made to Order Software Corp.  All Rights Reserved.
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
// Snap Websites Server -- snap watchdog library
// Snap Websites Servers -- create a feed where you can write an email
#pragma once

// libexcept
//
#include    <libexcept/exception.h>


// snapdev
//
#include    <snapdev/callback_manager.h>


// C++
//
#include    <map>
#include    <memory>
#include    <vector>



namespace brs
{



DECLARE_LOGIC_ERROR(brs_logic_error);

DECLARE_OUT_OF_RANGE(brs_out_of_range);


typedef std::uint32_t               magic_t;
typedef std::uint8_t                version_t;
typedef std::string                 name_t;
typedef std::vector<std::uint8_t>   buffer_t;

struct hunk_sizes_t
{
    std::uint32_t   f_name : 8;
    std::uint32_t   f_hunk : 24;
};


constexpr version_t const       BRS_ROOT = 0;       // indicate root buffer
constexpr version_t const       BRS_VERSION = 1;    // version of the format

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
constexpr magic_t const         BRS_MAGIC = (('B' << 24) | ('R' << 16) | ('S' << 8) | (BRS_VERSION << 0));
#else
constexpr magic_t const         BRS_MAGIC = (('B' << 0) | ('R' << 8) | ('S' << 16) | (BRS_VERSION << 24));
#endif



template<class T>
void add_value(buffer_t & buffer, name_t name, T const * ptr, std::size_t size)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
    hunk_sizes_t hunk_sizes = {
        .f_name = static_cast<std::uint8_t>(name.length()),
        .f_hunk = static_cast<std::uint32_t>(size & 0x00FFFFFF),
    };
#pragma GCC diagnostic pop

    if(hunk_sizes.f_name != name.length()
    || hunk_sizes.f_hunk != size)
    {
        throw brs_out_of_range("name or hunk too large");
    }

    // save the sizes
    //
    buffer.insert(
              buffer.end()
            , reinterpret_cast<buffer_t::value_type const *>(&hunk_sizes)
            , reinterpret_cast<buffer_t::value_type const *>(&hunk_sizes + 1));

    buffer.insert(
              buffer.end()
            , reinterpret_cast<buffer_t::value_type const *>(name.c_str())
            , reinterpret_cast<buffer_t::value_type const *>(name.c_str()) + hunk_sizes.f_name);

    buffer.insert(
              buffer.end()
            , reinterpret_cast<buffer_t::value_type const *>(ptr)
            , reinterpret_cast<buffer_t::value_type const *>(ptr) + size);
}


/** \brief Save a basic type or struct of basic types.
 *
 * This one function saves the specified value as is. It is expected to be
 * a basic type such as an int or a double. It also supports structures
 * that are only composed of basic types. Structures and classes with
 * complex types such as a string need to be handled manually.
 *
 * \tparam T  The type of value,
 * \param[out] buffer  The buffer where the value gets saved.
 * \param[in] name  The name of the field to be saved.
 * \param[in] value  The value to be saved with that name.
 */
template<typename T>
void add_value(buffer_t & buffer, name_t name, T const & value)
{
    add_value(buffer, name, &value, sizeof(value));
}


template<>
void add_value<std::string>(buffer_t & buffer, name_t name, std::string const & value)
{
    add_value(buffer, name, value.c_str(), value.length());
}


template<>
void add_value<buffer_t>(buffer_t & buffer, name_t name, buffer_t const & value)
{
    add_value(buffer, name, value.data(), value.size());
}


/** \brief Add the magic code at the end of the buffer.
 *
 * When creating a new buffer, you should first call this one function
 * to add the magic at the start of the buffer.
 *
 * Although you can call the function at any time, it is expected you
 * only call it at the very beginning.
 *
 * The add_magic() is separate because we want to make it easy to handle
 * sub-buffers without having to add a magic at each level.
 *
 * \param[out] buffer  The buffer where the magic is added.
 */
void add_magic(buffer_t & buffer)
{
    magic_t const magic(BRS_MAGIC);
    buffer.insert(
              buffer.end()
            , reinterpret_cast<buffer_t::value_type const *>(&magic)
            , reinterpret_cast<buffer_t::value_type const *>(&magic + 1));
}


class brs_object
{
public:
    typedef std::shared_ptr<brs_object>     pointer_t;

    virtual ~brs_object() {}

    virtual bool process_chunk(
                  name_t const & name
                , std::uint8_t const * data
                , std::size_t size) = 0;

    char to_char(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(char), size);
        return *reinterpret_cast<std::int16_t const *>(data);
    }

    signed char to_schar(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(signed char), size);
        return *reinterpret_cast<signed char const *>(data);
    }

    unsigned char to_uchar(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(unsigned char), size);
        return *reinterpret_cast<unsigned char const *>(data);
    }

    std::int16_t to_int16(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(std::int16_t), size);
        return *reinterpret_cast<std::int16_t const *>(data);
    }

    std::uint16_t to_uint16(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(std::uint16_t), size);
        return *reinterpret_cast<std::uint16_t const *>(data);
    }

    std::int32_t to_int32(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(std::int32_t), size);
        return *reinterpret_cast<std::int32_t const *>(data);
    }

    std::uint32_t to_uint32(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(std::uint32_t), size);
        return *reinterpret_cast<std::uint32_t const *>(data);
    }

    std::int64_t to_int64(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(std::int64_t), size);
        return *reinterpret_cast<std::int64_t const *>(data);
    }

    std::uint64_t to_uint64(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(std::uint64_t), size);
        return *reinterpret_cast<std::uint64_t const *>(data);
    }

    float to_float(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(float), size);
        return *reinterpret_cast<float const *>(data);
    }

    double to_double(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(double), size);
        return *reinterpret_cast<double const *>(data);
    }

    long double to_long_double(std::uint8_t const * data, std::size_t size)
    {
        verify_size(sizeof(long double), size);
        return *reinterpret_cast<long double const *>(data);
    }

    std::string to_string(std::uint8_t const * data, std::size_t size)
    {
        return std::string(reinterpret_cast<char const *>(data), size);
    }

    buffer_t to_buffer(std::uint8_t const * data, std::size_t size)
    {
        return buffer_t(data, data + size);
    }

private:
    void verify_size(std::size_t expected_size, std::size_t buffer_size)
    {
        if(expected_size != buffer_size)
        {
            throw brs_logic_error("unexpected size, wrong type?");
        }
    }
};


/** \brief Unserialize the specified buffer.
 *
 * This function reads each hunk and calls the specified \p callback
 * with them. You can then save the data in your object fields.
 *
 * The root buffer is expected to include the magic code at the start.
 * Set the \p include_magic to true in that case to verify that it
 * is indeed set and valid. If you do not do that, the unserialization
 * will fail since everything will be off by sizeof(magic_t).
 *
 * \param[in] buffer  The buffer to unserialize.
 * \param[in] callback  The callback manager used to store the callbacks to
 * call on each hunk.
 * \param[in] includes_magic  Whether \p buffer includes the a magic code
 * at the start or not. The top buffer is expected to include a magic
 * code. Sub-buffers should not include the magic code.
 *
 * \return true if the unserialization succeeded, false otherwise.
 */
template<typename T>
bool unserialize_buffer(
          buffer_t const & buffer
        , snapdev::callback_manager<T> & callback
        , bool includes_magic)
{
    std::size_t pos(0);

    if(includes_magic)
    {
        if(sizeof(magic_t) > buffer.size())
        {
            return false;
        }

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        magic_t const magic((buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3] << 0));
#else
        magic_t const magic((buffer[0] << 0) | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24));
#endif
        pos += sizeof(magic_t);

        if(magic != BRS_MAGIC)
        {
            return false;
        }
    }

    for(;;)
    {
        if(pos + sizeof(hunk_sizes_t) > buffer.size())
        {
            return pos == buffer.size();
        }

        hunk_sizes_t const * hunk_sizes(reinterpret_cast<hunk_sizes_t const *>(buffer.data() + pos));
        pos += sizeof(hunk_sizes_t);

        if(pos + hunk_sizes->f_name + hunk_sizes->f_hunk > buffer.size())
        {
            return false;
        }

        std::string const name(std::string(
                      reinterpret_cast<char const *>(buffer.data() + pos)
                    , hunk_sizes->f_name));
        pos += hunk_sizes->f_name;

        std::uint8_t const * ptr(reinterpret_cast<std::uint8_t const *>(buffer.data() + pos));
        pos += hunk_sizes->f_hunk;

        callback.call(&brs_object::process_chunk, name, ptr, hunk_sizes->f_hunk);
    }
}



} // namespace brs
// vim: ts=4 sw=4 et
