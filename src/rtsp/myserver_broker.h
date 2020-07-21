/*
Copyright (c) 2009-2018 Roger Light <roger@atchoo.org>

All rights reserved. This program and the accompanying materials
are made available under the terms of the Eclipse Public License v1.0
and Eclipse Distribution License v1.0 which accompany this distribution.
 
The Eclipse Public License is available at
   http://www.eclipse.org/legal/epl-v10.html
and the Eclipse Distribution License is available at
  http://www.eclipse.org/org/documents/edl-v10.php.
 
Contributors:
   Roger Light - initial implementation and documentation.
*/

#ifndef myserver_BROKER_H
#define myserver_BROKER_H

#include <stdbool.h>

struct myserver;

enum myserver_protocol {
	mp_mqtt,
	mp_mqttsn,
	mp_websockets
};

/* =========================================================================
 *
 * Utility Functions
 *
 * Use these functions from within your plugin.
 *
 * There are also very useful functions in libmyserver.
 *
 * ========================================================================= */


/*
 * Function: myserver_log_printf
 *
 * Write a log message using the broker configured logging.
 *
 * Parameters:
 * 	level -    Log message priority. Can currently be one of:
 *
 *             MS_LOG_INFO
 *             MS_LOG_NOTICE
 *             MS_LOG_WARNING
 *             MS_LOG_ERR
 *             MS_LOG_DEBUG
 *             MS_LOG_SUBSCRIBE (not recommended for use by plugins)
 *             MS_LOG_UNSUBSCRIBE (not recommended for use by plugins)
 *
 *             These values are defined in myserver.h.
 *
 *	fmt, ... - printf style format and arguments.
 */
void myserver_log_printf(int level, const char *fmt, ...);


/* =========================================================================
 *
 * Client Functions
 *
 * Use these functions to access client information.
 *
 * ========================================================================= */

/*
 * Function: myserver_client_address
 *
 * Retrieve the IP address of the client as a string.
 */
const char *myserver_client_address(const struct myserver *client);


/*
 * Function: myserver_client_clean_session
 *
 * Retrieve the clean session flag value for a client.
 */
bool myserver_client_clean_session(const struct myserver *client);


/*
 * Function: myserver_client_id
 *
 * Retrieve the client id associated with a client.
 */
const char *myserver_client_id(const struct myserver *client);


/*
 * Function: myserver_client_keepalive
 *
 * Retrieve the keepalive value for a client.
 */
int myserver_client_keepalive(const struct myserver *client);


/*
 * Function: myserver_client_certificate
 *
 * If TLS support is enabled, return the certificate provided by a client as an
 * X509 pointer from openssl. If the client did not provide a certificate, then
 * NULL will be returned. This function will only ever return a non-NULL value
 * if the `require_certificate` option is set to true.
 *
 * If TLS is not supported, this function will always return NULL.
 */
void *myserver_client_certificate(const struct myserver *client);


/*
 * Function: myserver_client_protocol
 *
 * Retrieve the protocol with which the client has connected. Can be one of:
 *
 * mp_mqtt (MQTT over TCP)
 * mp_mqttsn (MQTT-SN)
 * mp_websockets (MQTT over Websockets)
 */
int myserver_client_protocol(const struct myserver *client);


/*
 * Function: myserver_client_sub_count
 *
 * Retrieve the number of subscriptions that have been made by a client.
 */
int myserver_client_sub_count(const struct myserver *client);


/*
 * Function: myserver_client_username
 *
 * Retrieve the username associated with a client.
 */
const char *myserver_client_username(const struct myserver *client);

#endif
