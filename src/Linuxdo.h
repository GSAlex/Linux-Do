/*
 * Prtsc - screen cut program written for my wife
 *
 * Copyright (c) 2010-2010 microcai  <microcai@fedoraproject.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Prtsc.h - massive header file
 */

#ifndef PRTSC_H_
#define PRTSC_H_

#include "config.h"

#ifdef ENABLE_NLS
#include <libintl.h>
#include <locale.h>
#define _(String) gettext (String)
#define gettext_noop(String) String
#define N_(String) gettext_noop (String)
#else
#define _(String) (String)
#define N_(String) (String)
#define setlocale(x,y)
#define textdomain(x)
#define bindtextdomain(x,y)
#endif

#ifdef DEBUG
#define textdomain(domian) \
	do { textdomain(domian) ; \
		bindtextdomain(GETTEXT_PACKAGE, "/tmp/usr/share/locale"); \
	}while(0)

#endif

#endif /* PRTSC_H_ */
