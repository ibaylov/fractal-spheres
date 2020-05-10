#ifndef _ASSERT_HH_
#define _ASSERT_HH_

/**
@file assert.hh
@brief We want a debug supprt that vanishes completely in release
The graphisc is not a place for lingering debug code or unwize use of c++ exceptions, so we redefine
_ASSERT to be a macro over assert just to be sure it is excluded regardles of the actual assert implementation
*/

#ifdef _DEBUG_DUMP_
#include <assert.h>
#define _ASSERT( e ) assert( e )
#else
#define _ASSERT( e )
#endif
#endif
