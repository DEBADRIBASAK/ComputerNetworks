
// -*- C++ -*-
// Definition for Win32 Export directives.
// This file is generated automatically by generate_export_file.pl SYNCH
// ------------------------------
#ifndef RTSCHEDTESTLIB_EXPORT_H
#define RTSCHEDTESTLIB_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (RTSCHEDTESTLIB_HAS_DLL)
#  define RTSCHEDTESTLIB_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && RTSCHEDTESTLIB_HAS_DLL */

#if !defined (RTSCHEDTESTLIB_HAS_DLL)
#  define RTSCHEDTESTLIB_HAS_DLL 1
#endif /* ! RTSCHEDTESTLIB_HAS_DLL */

#if defined (RTSCHEDTESTLIB_HAS_DLL) && (RTSCHEDTESTLIB_HAS_DLL == 1)
#  if defined (RTSCHEDTESTLIB_BUILD_DLL)
#    define RTSCHEDTESTLIB_Export ACE_Proper_Export_Flag
#    define RTSCHEDTESTLIB_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#    define RTSCHEDTESTLIB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  else /* RTSCHEDTESTLIB_BUILD_DLL */
#    define RTSCHEDTESTLIB_Export ACE_Proper_Import_Flag
#    define RTSCHEDTESTLIB_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#    define RTSCHEDTESTLIB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif /* RTSCHEDTESTLIB_BUILD_DLL */
#else /* RTSCHEDTESTLIB_HAS_DLL == 1 */
#  define RTSCHEDTESTLIB_Export
#  define RTSCHEDTESTLIB_SINGLETON_DECLARATION(T)
#  define RTSCHEDTESTLIB_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif /* RTSCHEDTESTLIB_HAS_DLL == 1 */

// Set RTSCHEDTESTLIB_NTRACE = 0 to turn on library specific tracing even if
// tracing is turned off for ACE.
#if !defined (RTSCHEDTESTLIB_NTRACE)
#  if (ACE_NTRACE == 1)
#    define RTSCHEDTESTLIB_NTRACE 1
#  else /* (ACE_NTRACE == 1) */
#    define RTSCHEDTESTLIB_NTRACE 0
#  endif /* (ACE_NTRACE == 1) */
#endif /* !RTSCHEDTESTLIB_NTRACE */

#if (RTSCHEDTESTLIB_NTRACE == 1)
#  define RTSCHEDTESTLIB_TRACE(X)
#else /* (RTSCHEDTESTLIB_NTRACE == 1) */
#  if !defined (ACE_HAS_TRACE)
#    define ACE_HAS_TRACE
#  endif /* ACE_HAS_TRACE */
#  define RTSCHEDTESTLIB_TRACE(X) ACE_TRACE_IMPL(X)
#  include "ace/Trace.h"
#endif /* (RTSCHEDTESTLIB_NTRACE == 1) */

#endif /* RTSCHEDTESTLIB_EXPORT_H */

// End of auto generated file.
