/*---------------------------------------------------------------------------*
 * str.h -
 * Written By: Nicholas DiPasquale
 * Copyright (c) 2005-2009 N.I.E.Software
 *---------------------------------------------------------------------------*/

#pragma once
#ifndef __STR_H__
#define __STR_H__

/*---------------------------------------------------------------------------*/

#include <string>
#include <sstream>
#include <iomanip>

namespace niesoft {

/*---------------------------------------------------------------------------*/

#ifdef USE_WCHAR_STR
typedef wchar_t char_t;
#else
typedef char char_t;
#endif


typedef std::basic_string<char_t, std::char_traits<char_t>, gc_allocator<char_t> > String;
typedef std::basic_ostringstream<char_t, std::char_traits<char_t>, gc_allocator<char_t> > StringOutputStream;

/*---------------------------------------------------------------------------*/

}; // namespace niesoft;

/*---------------------------------------------------------------------------*/
#endif // __STR_H__
