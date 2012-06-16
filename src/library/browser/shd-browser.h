/*
 * The Shadow Simulator
 *
 * Copyright (c) 2010-2012 Rob Jansen <jansen@cs.umn.edu>
 *
 * This file is part of Shadow.
 *
 * Shadow is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shadow is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Shadow.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SHD_BROWSER_H_
#define SHD_BROWSER_H_

#include <unistd.h> /* close */
#include <string.h> /* memset */
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <netdb.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <assert.h>

#include <shd-library.h>

#include "shd-html.h"
#include "shd-url.h"
#include "shd-filetransfer-defs.h"
#include "shd-filegetter.h"

enum browser_state {
	SB_DOCUMENT, SB_EMBEDDED_OBJECTS, SB_DONE
};

typedef struct browser_connection_s {
	filegetter_t fg;
	filegetter_filespec_t fspec;
	filegetter_serverspec_t sspec;	
} browser_connection_t, *browser_connection_tp;

typedef struct browser_download_tasks_s {
	/* set that contains the paths that were already downloaded */
	GHashTable* blocked;
	/* contains paths to download */
	GQueue* unfinished;
	/* contains all open connections (browser_connection_t) */
	GSList* running;
} browser_download_tasks_t, *browser_download_tasks_tp;

typedef struct browser_server_args_s {
	gchar* host;
	gchar* port;
} browser_server_args_t, *browser_server_args_tp;

typedef struct browser_s { 
	ShadowlibFunctionTable* shadowlib;
	enum browser_state state;
	gint epolld;
	gchar* first_hostname;
	/* We never change them during simumlation */
	browser_server_args_tp socks_proxy;
	/* hostname (gchar*) -> download tasks (browser_connection_tp) */
	GHashTable* download_tasks;
	gint max_concurrent_downloads;
} browser_t, *browser_tp;

typedef struct browser_args_s {
	browser_server_args_t http_server;
	browser_server_args_t socks_proxy;
	gchar* max_concurrent_downloads;
	gchar* document_path;
} browser_args_t, *browser_args_tp;

typedef struct browser_activate_result_s {
	browser_connection_tp connection;
	enum filegetter_code code;
	gchar* hostname;
} browser_activate_result_t, *browser_activate_result_tp;

void browser_start(browser_tp b, browser_args_t args);
void browser_activate(browser_tp b, gint sockd);

#endif /* SHD_BROWSER_H_ */
