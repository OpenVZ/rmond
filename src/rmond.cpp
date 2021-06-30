/*
 * Copyright (c) 2016-2017, Parallels International GmbH
 * Copyright (c) 2017-2019 Virtuozzo International GmbH. All rights reserved.
 *
 * This file is part of OpenVZ. OpenVZ is free software; you can redistribute
 * it and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Our contact details: Virtuozzo International GmbH, Vordergasse 59, 8200
 * Schaffhausen, Switzerland.
 */

#include "mib.h"
#include <signal.h>

extern "C"
{

void init_RmondMIB(void)
{
        snmp_log(LOG_WARNING, LOG_PREFIX "Initializing the " TOKEN" module\n");
	// NB. the main thread is the dedicated one for the
	// signal handling. all the rest should block all
	// the signals. this call is in context of the main
	// thread thus there is no race with signal handling
	// of the snmpd.
	sigset_t a, o;
	sigfillset(&a);
	pthread_sigmask(SIG_BLOCK, &a, &o);
	Rmond::Central::init();
	pthread_sigmask(SIG_SETMASK, &o, NULL);
	snmp_log(LOG_WARNING, LOG_PREFIX "Done initalizing " TOKEN" module\n");
}

void deinit_RmondMIB(void)
{
        snmp_log(LOG_WARNING, LOG_PREFIX "Finalizing the " TOKEN" module\n");
	Rmond::Central::fini();
        snmp_log(LOG_WARNING, LOG_PREFIX "Done finalizing " TOKEN" module\n");
}

}

