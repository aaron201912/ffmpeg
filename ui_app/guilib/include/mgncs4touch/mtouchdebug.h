/*
 * \file mtouchdebug.h
 * \author miniStudio team of FMSoft
 * \date 2010/09/09
 *
 \verbatim

    This file is part of mGNCS4Touch, one of MiniGUI components.

    Copyright (C) 2008-2018 FMSoft (http://www.fmsoft.cn).

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Or,

    As this program is a library, any link to this program must follow
    GNU General Public License version 3 (GPLv3). If you cannot accept
    GPLv3, you need to be licensed from FMSoft.

    If you have got a commercial license of this program, please use it
    under the terms and conditions of the commercial license.

    For more information about the commercial license, please refer to
    <http://www.minigui.com/en/about/licensing-policy/>.

 \endverbatim
*/

#ifndef	__MTOUCH_DEBUG_H__
#define	__MTOUCH_DEBUG_H__

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * Normally we strip LOGV (VERBOSE messages) from release builds.
 * You can modify this (for example with "#define NDEBUG 1"
 * at the top of your source file) to change that behavior.
 */
#ifndef	LOG_NDEBUG
#ifdef	NDEBUG
#define	LOG_NDEBUG	1
#else
#define	LOG_NDEBUG	0
#endif
#endif

#ifndef	STDIN
#define	STDIN	stdin
#endif

#ifndef	STDOUT
#define	STDOUT	stdout
#endif

#ifndef	STDERR
#define	STDERR	stderr
#endif

#ifndef	PP_STR
#define	PP_STR(str)	#str
#endif

#ifndef	LOG
#if	LOG_NDEBUG
#define	LOG		fprintf
#else
#define	LOG
#endif
#endif

// print file function and line info, macro difine
#ifndef	LOGFFL
#if	LOG_NDEBUG
#define	LOGFFL(STD)	LOG(STD, "%s  %s()  %d lines >>>> ", \
		__FILE__, __FUNCTION__, __LINE__)
#else
#define	LOGFFL(STD)	((void)0)
#endif
#endif

#ifndef	LOGFFL_ENDL
#if	LOG_NDEBUG
#define	LOGFFL_ENDL(STD)	LOG(STD, "\tFFL : %s  %s()  %d lines\n", \
		__FILE__, __FUNCTION__, __LINE__)
#else
#define	LOGFFL_ENDL(STD)	((void)0)
#endif
#endif

#ifndef	LOGELINE
#if	LOG_NDEBUG
#define	LOGELINE()	LOGFFL_ENDL(STDERR)
#else
#define	LOGELINE()
#endif
#endif

#ifndef	LOGILINE
#if	LOG_NDEBUG
#define	LOGILINE()	LOGFFL_ENDL(STDIN)
#else
#define	LOGILINE()
#endif
#endif

#ifndef	LOGOLINE
#if	LOG_NDEBUG
#define	LOGOLINE()	LOGFFL_ENDL(STDOUT)
#else
#define	LOGOLINE()
#endif
#endif

// stderr print info, macro difine
#ifndef	LOGE
#if	LOG_NDEBUG
#define	LOGE(...)	do { LOGFFL(STDERR); LOG(STDERR, __VA_ARGS__); } while (0)
#else
#define	LOGE(...)	((void)0)
#endif
#endif

// stdin print info, macro difine
#ifndef	LOGI
#if	LOG_NDEBUG
#define	LOGI(...)	do { LOGFFL(STDIN); LOG(STDIN,  __VA_ARGS__); } while (0)
#else
#define	LOGI(...)	((void)0)
#endif
#endif

// stdout print info, macro difine
#ifndef	LOGO
#if	LOG_NDEBUG
#define	LOGO(...)	do { LOGFFL(STDOUT); LOG(STDOUT, __VA_ARGS__); } while (0)
#else
#define	LOGO(...)	((void)0)
#endif
#endif

// decimal print a variable info, macro difine
#ifndef	LOGD
#if	LOG_NDEBUG
#define	LOGD(var)	do { LOGFFL(STDERR); \
	LOG(STDERR, "%s = %d\n", PP_STR(var), var); } while (0)
#else
#define	LOGD(var)	((void)0)
#endif
#endif

// real print a variable info, macro difine
#ifndef	LOGF
#if	LOG_NDEBUG
#define	LOGF(var)	do { LOGFFL(STDERR); \
	LOG(STDERR, "%s = %f\n", PP_STR(var), var); } while (0)
#else
#define	LOGF(var)	((void)0)
#endif
#endif

// hex print a variable info, macro difine
#ifndef	LOGH
#if	LOG_NDEBUG
#define	LOGH(var)	do { LOGFFL(STDERR); \
	LOG(STDERR, "%s = 0x%x\n", PP_STR(var), var); } while (0)
#else
#define	LOGH(var)	((void)0)
#endif
#endif

// character print a variable info, macro difine
#ifndef	LOGC
#if	LOG_NDEBUG
#define	LOGC(var)	do { LOGFFL(STDERR); \
	LOG(STDERR, "%s = %c\n", PP_STR(var), var); } while (0)
#else
#define	LOGC(var)	((void)0)
#endif
#endif

// strings print a variable info, macro difine
#ifndef	LOGS
#if	LOG_NDEBUG
#define	LOGS(var)	do { LOGFFL(STDERR); \
	LOG(STDERR, "%s = %s\n", PP_STR(var), var); } while (0)
#else
#define	LOGS(var)	((void)0)
#endif
#endif

#ifdef	__cplusplus
}
#endif

#endif	//__MTOUCH_DEBUG_H__

