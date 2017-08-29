#ifndef __GC_H__
#define __GC_H__

#pragma once
/*---------------------------------------------------------------------------*/

#define GC_OPERATOR_NEW_ARRAY
// #define FIND_LEAK	1
#include <gc_cpp.h>
#include <gc_allocator.h>

/*---------------------------------------------------------------------------*/

#ifdef FIND_LEAK
#	define CHECK_LEAKS()      GC_gcollect ()
#else
#	define CHECK_LEAKS()
#endif

/*---------------------------------------------------------------------------*/
#endif /* __GC_H__ */
