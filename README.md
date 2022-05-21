
Introduction
============

The Binary Recursive Serialiation (BRS) project was a C++ library allowing you
to transform objects to binary and back.

The brs.h was moved to the [snapdev project](https://github.com/m2osw/snapdev) instead.


Implementation
==============

The implementation is a set of template functions allowing you to serialize and
unserialize binary data to a buffer. Each value is written with its name, type,
and size so you can read the data back in any order.

Bugs
====

Submit bug reports and patches on
[github](https://github.com/m2osw/brs/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
