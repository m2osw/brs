
<p align="center">
<img alt="brs" title="Binary Recursive Serialization -- a library to serialize data to a binary format."
src="https://snapwebsites.org/sites/snapwebsites.org/files/images/brs-logo.png" width="300" height="150"/>
</p>

Introduction
============

The Binary Recursive Serialiation (BRS) project is a C++ library allowing you
to transform objects to binary and back.


TODO
====

The current implementation requires us to create a `buffer_t` to pass
around the `serialize()` and `unserialize()` functions.

* Serialize

  I think we could use an `std::stringstream` instead. That class allocate
  lose buffers instead of one vector, which is way more costly. At the end,
  there is still one extra copy when we call the `str()` function, but that
  copy only happens if the output buffer is larger than the first allocated
  buffer.

* Unserialize

  When a sub-buffer is required (recusive unserialization), we make a copy
  of the data in a new buffer. We should look into having a buffer view
  instead. Also, it may be possible to reimplement using the
  `std::stringstream` too (in which case an `std::istream` and possibly
  an `std::ostream` for the serialization).


Bugs
====

Submit bug reports and patches on
[github](https://github.com/m2osw/brs/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
