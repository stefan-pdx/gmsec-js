
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/* This include file fixes the HP-UX gcc bug that makes gcc think it has
   stdint.h when it doesn't.  See
   http://gcc.gnu.org/ml/libstdc++/2003-10/msg00319.html
*/
#ifndef _GMSEC_HP_BUG
#define _GMSEC_HP_BUG
#include <bits/c++config.h>
#undef _GLIBCXX_HAVE_STDINT_H
#endif
