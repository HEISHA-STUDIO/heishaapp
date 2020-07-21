/*
Copyright (c) 2012-2018 Roger Light <roger@atchoo.org>

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

#ifndef myserver_PLUGIN_H
#define myserver_PLUGIN_H

#define MS_AUTH_PLUGIN_VERSION 3

#define MS_ACL_NONE 0x00
#define MS_ACL_READ 0x01
#define MS_ACL_WRITE 0x02
#define MS_ACL_SUBSCRIBE 0x04

#include <stdbool.h>

struct myserver;

struct myserver_opt {
	char *key;
	char *value;
};

struct myserver_auth_opt {
	char *key;
	char *value;
};

struct myserver_acl_msg {
	const char *topic;
	const void *payload;
	long payloadlen;
	int qos;
	bool retain;
};

/*
 * To create an authentication plugin you must include this file then implement
 * the functions listed in the "Plugin Functions" section below. The resulting
 * code should then be compiled as a shared library. Using gcc this can be
 * achieved as follows:
 *
 * gcc -I<path to myserver_plugin.h> -fPIC -shared plugin.c -o plugin.so
 *
 * On Mac OS X:
 *
 * gcc -I<path to myserver_plugin.h> -fPIC -shared plugin.c -undefined dynamic_lookup -o plugin.so
 *
 * Authentication plugins can implement one or both of authentication and
 * access control. If your plugin does not wish to handle either of
 * authentication or access control it should return MS_ERR_PLUGIN_DEFER. In
 * this case, the next plugin will handle it. If all plugins return
 * MS_ERR_PLUGIN_DEFER, the request will be denied.
 *
 * For each check, the following flow happens:
 *
 * * The default password file and/or acl file checks are made. If either one
 *   of these is not defined, then they are considered to be deferred. If either
 *   one accepts the check, no further checks are made. If an error occurs, the
 *   check is denied
 * * The first plugin does the check, if it returns anything other than
 *   MS_ERR_PLUGIN_DEFER, then the check returns immediately. If the plugin
 *   returns MS_ERR_PLUGIN_DEFER then the next plugin runs its check.
 * * If the final plugin returns MS_ERR_PLUGIN_DEFER, then access will be
 *   denied.
 */


/* =========================================================================
 *
 * Plugin Functions
 *
 * You must implement these functions in your plugin.
 *
 * ========================================================================= */

/*
 * Function: myserver_auth_plugin_version
 *
 * The broker will call this function immediately after loading the plugin to
 * check it is a supported plugin version. Your code must simply return
 * MS_AUTH_PLUGIN_VERSION.
 */
int myserver_auth_plugin_version(void);


/*
 * Function: myserver_auth_plugin_init
 *
 * Called after the plugin has been loaded and <myserver_auth_plugin_version>
 * has been called. This will only ever be called once and can be used to
 * initialise the plugin.
 *
 * Parameters:
 *
 *	user_data :      The pointer set here will be passed to the other plugin
 *	                 functions. Use to hold connection information for example.
 *	opts :           Pointer to an array of struct myserver_opt, which
 *	                 provides the plugin options defined in the configuration file.
 *	opt_count :      The number of elements in the opts array.
 *
 * Return value:
 *	Return 0 on success
 *	Return >0 on failure.
 */
int myserver_auth_plugin_init(void **user_data, struct myserver_opt *opts, int opt_count);


/*
 * Function: myserver_auth_plugin_cleanup
 *
 * Called when the broker is shutting down. This will only ever be called once
 * per plugin.
 * Note that <myserver_auth_security_cleanup> will be called directly before
 * this function.
 *
 * Parameters:
 *
 *	user_data :      The pointer provided in <myserver_auth_plugin_init>.
 *	opts :           Pointer to an array of struct myserver_opt, which
 *	                 provides the plugin options defined in the configuration file.
 *	opt_count :      The number of elements in the opts array.
 *
 * Return value:
 *	Return 0 on success
 *	Return >0 on failure.
 */
int myserver_auth_plugin_cleanup(void *user_data, struct myserver_opt *opts, int opt_count);


/*
 * Function: myserver_auth_security_init
 *
 * This function is called in two scenarios:
 *
 * 1. When the broker starts up.
 * 2. If the broker is requested to reload its configuration whilst running. In
 *    this case, <myserver_auth_security_cleanup> will be called first, then
 *    this function will be called.  In this situation, the reload parameter
 *    will be true.
 *
 * Parameters:
 *
 *	user_data :      The pointer provided in <myserver_auth_plugin_init>.
 *	opts :           Pointer to an array of struct myserver_opt, which
 *	                 provides the plugin options defined in the configuration file.
 *	opt_count :      The number of elements in the opts array.
 *	reload :         If set to false, this is the first time the function has
 *	                 been called. If true, the broker has received a signal
 *	                 asking to reload its configuration.
 *
 * Return value:
 *	Return 0 on success
 *	Return >0 on failure.
 */
int myserver_auth_security_init(void *user_data, struct myserver_opt *opts, int opt_count, bool reload);


/* 
 * Function: myserver_auth_security_cleanup
 *
 * This function is called in two scenarios:
 *
 * 1. When the broker is shutting down.
 * 2. If the broker is requested to reload its configuration whilst running. In
 *    this case, this function will be called, followed by
 *    <myserver_auth_security_init>. In this situation, the reload parameter
 *    will be true.
 *
 * Parameters:
 *
 *	user_data :      The pointer provided in <myserver_auth_plugin_init>.
 *	opts :           Pointer to an array of struct myserver_opt, which
 *	                 provides the plugin options defined in the configuration file.
 *	opt_count :      The number of elements in the opts array.
 *	reload :         If set to false, this is the first time the function has
 *	                 been called. If true, the broker has received a signal
 *	                 asking to reload its configuration.
 *
 * Return value:
 *	Return 0 on success
 *	Return >0 on failure.
 */
int myserver_auth_security_cleanup(void *user_data, struct myserver_opt *opts, int opt_count, bool reload);


/*
 * Function: myserver_auth_acl_check
 *
 * Called by the broker when topic access must be checked. access will be one
 * of:
 *  MS_ACL_SUBSCRIBE when a client is asking to subscribe to a topic string.
 *                     This differs from MS_ACL_READ in that it allows you to
 *                     deny access to topic strings rather than by pattern. For
 *                     example, you may use MS_ACL_SUBSCRIBE to deny
 *                     subscriptions to '#', but allow all topics in
 *                     MS_ACL_READ. This allows clients to subscribe to any
 *                     topic they want, but not discover what topics are in use
 *                     on the server.
 *  MS_ACL_READ      when a message is about to be sent to a client (i.e. whether
 *                     it can read that topic or not).
 *  MS_ACL_WRITE     when a message has been received from a client (i.e. whether
 *                     it can write to that topic or not).
 *
 * Return:
 *	MS_ERR_SUCCESS if access was granted.
 *	MS_ERR_ACL_DENIED if access was not granted.
 *	MS_ERR_UNKNOWN for an application specific error.
 *	MS_ERR_PLUGIN_DEFER if your plugin does not wish to handle this check.
 */
int myserver_auth_acl_check(void *user_data, int access, const struct myserver *client, const struct myserver_acl_msg *msg);


/*
 * Function: myserver_auth_unpwd_check
 *
 * Called by the broker when a username/password must be checked.
 *
 * Return:
 *	MS_ERR_SUCCESS if the user is authenticated.
 *	MS_ERR_AUTH if authentication failed.
 *	MS_ERR_UNKNOWN for an application specific error.
 *	MS_ERR_PLUGIN_DEFER if your plugin does not wish to handle this check.
 */
int myserver_auth_unpwd_check(void *user_data, const struct myserver *client, const char *username, const char *password);


/*
 * Function: myserver_psk_key_get
 *
 * Called by the broker when a client connects to a listener using TLS/PSK.
 * This is used to retrieve the pre-shared-key associated with a client
 * identity.
 *
 * Examine hint and identity to determine the required PSK (which must be a
 * hexadecimal string with no leading "0x") and copy this string into key.
 *
 * Parameters:
 *	user_data :   the pointer provided in <myserver_auth_plugin_init>.
 *	hint :        the psk_hint for the listener the client is connecting to.
 *	identity :    the identity string provided by the client
 *	key :         a string where the hex PSK should be copied
 *	max_key_len : the size of key
 *
 * Return value:
 *	Return 0 on success.
 *	Return >0 on failure.
 *	Return MS_ERR_PLUGIN_DEFER if your plugin does not wish to handle this check.
 */
int myserver_auth_psk_key_get(void *user_data, const struct myserver *client, const char *hint, const char *identity, char *key, int max_key_len);

#endif
