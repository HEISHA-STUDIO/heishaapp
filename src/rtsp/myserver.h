/*
Copyright (c) 2010-2018 Roger Light <roger@atchoo.org>

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

#ifndef myserver_H
#define myserver_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32) && !defined(WITH_BROKER) && !defined(LIBmyserver_STATIC)
#	ifdef libmyserver_EXPORTS
#		define libms_EXPORT  __declspec(dllexport)
#	else
#		define libms_EXPORT  __declspec(dllimport)
#	endif
#else
#	define libms_EXPORT
#endif

#if defined(_MSC_VER) && _MSC_VER < 1900
#	ifndef __cplusplus
#		define bool char
#		define true 1
#		define false 0
#	endif
#else
#	ifndef __cplusplus
#		include <stdbool.h>
#	endif
#endif

#include <stddef.h>

#define LIBmyserver_MAJOR 1
#define LIBmyserver_MINOR 5
#define LIBmyserver_REVISION 0
/* LIBmyserver_VERSION_NUMBER looks like 1002001 for e.g. version 1.2.1. */
#define LIBmyserver_VERSION_NUMBER (LIBmyserver_MAJOR*1000000+LIBmyserver_MINOR*1000+LIBmyserver_REVISION)

/* Log types */
#define MS_LOG_NONE 0x00
#define MS_LOG_INFO 0x01
#define MS_LOG_NOTICE 0x02
#define MS_LOG_WARNING 0x04
#define MS_LOG_ERR 0x08
#define MS_LOG_DEBUG 0x10
#define MS_LOG_SUBSCRIBE 0x20
#define MS_LOG_UNSUBSCRIBE 0x40
#define MS_LOG_WEBSOCKETS 0x80
#define MS_LOG_ALL 0xFFFF

/* Error values */
enum ms_err_t {
	MS_ERR_CONN_PENDING = -1,
	MS_ERR_SUCCESS = 0,
	MS_ERR_NOMEM = 1,
	MS_ERR_PROTOCOL = 2,
	MS_ERR_INVAL = 3,
	MS_ERR_NO_CONN = 4,
	MS_ERR_CONN_REFUSED = 5,
	MS_ERR_NOT_FOUND = 6,
	MS_ERR_CONN_LOST = 7,
	MS_ERR_TLS = 8,
	MS_ERR_PAYLOAD_SIZE = 9,
	MS_ERR_NOT_SUPPORTED = 10,
	MS_ERR_AUTH = 11,
	MS_ERR_ACL_DENIED = 12,
	MS_ERR_UNKNOWN = 13,
	MS_ERR_ERRNO = 14,
	MS_ERR_EAI = 15,
	MS_ERR_PROXY = 16,
	MS_ERR_PLUGIN_DEFER = 17,
	MS_ERR_MALFORMED_UTF8 = 18
};

/* Error values */
enum ms_opt_t {
	MS_OPT_PROTOCOL_VERSION = 1,
	MS_OPT_SSL_CTX = 2,
	MS_OPT_SSL_CTX_WITH_DEFAULTS = 3,
};

/* MQTT specification restricts client ids to a maximum of 23 characters */
#define MS_MQTT_ID_MAX_LENGTH 23

#define MQTT_PROTOCOL_V31 3
#define MQTT_PROTOCOL_V311 4

struct myserver_message{
	int mid;
	char *topic;
	void *payload;
	int payloadlen;
	int qos;
	bool retain;
};

struct myserver;

/*
 * Topic: Threads
 *	libmyserver provides thread safe operation, with the exception of
 *	<myserver_lib_init> which is not thread safe.
 *
 *	If your application uses threads you must use <myserver_threaded_set> to
 *	tell the library this is the case, otherwise it makes some optimisations
 *	for the single threaded case that may result in unexpected behaviour for
 *	the multi threaded case.
 */
/***************************************************
 * Important note
 *
 * The following functions that deal with network operations will return
 * MS_ERR_SUCCESS on success, but this does not mean that the operation has
 * taken place. An attempt will be made to write the network data, but if the
 * socket is not available for writing at that time then the packet will not be
 * sent. To ensure the packet is sent, call myserver_loop() (which must also
 * be called to process incoming network data).
 * This is especially important when disconnecting a client that has a will. If
 * the broker does not receive the DISCONNECT command, it will assume that the
 * client has disconnected unexpectedly and send the will.
 *
 * myserver_connect()
 * myserver_disconnect()
 * myserver_subscribe()
 * myserver_unsubscribe()
 * myserver_publish()
 ***************************************************/

/*
 * Function: myserver_lib_version
 *
 * Can be used to obtain version information for the myserver library.
 * This allows the application to compare the library version against the
 * version it was compiled against by using the LIBmyserver_MAJOR,
 * LIBmyserver_MINOR and LIBmyserver_REVISION defines.
 *
 * Parameters:
 *  major -    an integer pointer. If not NULL, the major version of the
 *             library will be returned in this variable.
 *  minor -    an integer pointer. If not NULL, the minor version of the
 *             library will be returned in this variable.
 *  revision - an integer pointer. If not NULL, the revision of the library will
 *             be returned in this variable.
 *
 * Returns:
 *	LIBmyserver_VERSION_NUMBER, which is a unique number based on the major,
 *		minor and revision values.
 * See Also:
 * 	<myserver_lib_cleanup>, <myserver_lib_init>
 */
libms_EXPORT int myserver_lib_version(int *major, int *minor, int *revision);

/*
 * Function: myserver_lib_init
 *
 * Must be called before any other myserver functions.
 *
 * This function is *not* thread safe.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - always
 *
 * See Also:
 * 	<myserver_lib_cleanup>, <myserver_lib_version>
 */
libms_EXPORT int myserver_lib_init(void);

/*
 * Function: myserver_lib_cleanup
 *
 * Call to free resources associated with the library.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - always
 *
 * See Also:
 * 	<myserver_lib_init>, <myserver_lib_version>
 */
libms_EXPORT int myserver_lib_cleanup(void);

/*
 * Function: myserver_new
 *
 * Create a new myserver client instance.
 *
 * Parameters:
 * 	id -            String to use as the client id. If NULL, a random client id
 * 	                will be generated. If id is NULL, clean_session must be true.
 * 	clean_session - set to true to instruct the broker to clean all messages
 *                  and subscriptions on disconnect, false to instruct it to
 *                  keep them. See the man page mqtt(7) for more details.
 *                  Note that a client will never discard its own outgoing
 *                  messages on disconnect. Calling <myserver_connect> or
 *                  <myserver_reconnect> will cause the messages to be resent.
 *                  Use <myserver_reinitialise> to reset a client to its
 *                  original state.
 *                  Must be set to true if the id parameter is NULL.
 * 	obj -           A user pointer that will be passed as an argument to any
 *                  callbacks that are specified.
 *
 * Returns:
 * 	Pointer to a struct myserver on success.
 * 	NULL on failure. Interrogate errno to determine the cause for the failure:
 *      - ENOMEM on out of memory.
 *      - EINVAL on invalid input parameters.
 *
 * See Also:
 * 	<myserver_reinitialise>, <myserver_destroy>, <myserver_user_data_set>
 */
libms_EXPORT struct myserver *myserver_new(const char *id, bool clean_session, void *obj);

/*
 * Function: myserver_destroy
 *
 * Use to free memory associated with a myserver client instance.
 *
 * Parameters:
 * 	ms - a struct myserver pointer to free.
 *
 * See Also:
 * 	<myserver_new>, <myserver_reinitialise>
 */
libms_EXPORT void myserver_destroy(struct myserver *ms);

/*
 * Function: myserver_reinitialise
 *
 * This function allows an existing myserver client to be reused. Call on a
 * myserver instance to close any open network connections, free memory
 * and reinitialise the client with the new parameters. The end result is the
 * same as the output of <myserver_new>.
 *
 * Parameters:
 * 	ms -          a valid myserver instance.
 * 	id -            string to use as the client id. If NULL, a random client id
 * 	                will be generated. If id is NULL, clean_session must be true.
 * 	clean_session - set to true to instruct the broker to clean all messages
 *                  and subscriptions on disconnect, false to instruct it to
 *                  keep them. See the man page mqtt(7) for more details.
 *                  Must be set to true if the id parameter is NULL.
 * 	obj -           A user pointer that will be passed as an argument to any
 *                  callbacks that are specified.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 *
 * See Also:
 * 	<myserver_new>, <myserver_destroy>
 */
libms_EXPORT int myserver_reinitialise(struct myserver *ms, const char *id, bool clean_session, void *obj);

/*
 * Function: myserver_will_set
 *
 * Configure will information for a myserver instance. By default, clients do
 * not have a will.  This must be called before calling <myserver_connect>.
 *
 * Parameters:
 * 	ms -       a valid myserver instance.
 * 	topic -      the topic on which to publish the will.
 * 	payloadlen - the size of the payload (bytes). Valid values are between 0 and
 *               268,435,455.
 * 	payload -    pointer to the data to send. If payloadlen > 0 this must be a
 *               valid memory location.
 * 	qos -        integer value 0, 1 or 2 indicating the Quality of Service to be
 *               used for the will.
 * 	retain -     set to true to make the will a retained message.
 *
 * Returns:
 * 	MS_ERR_SUCCESS -      on success.
 * 	MS_ERR_INVAL -          if the input parameters were invalid.
 * 	MS_ERR_NOMEM -          if an out of memory condition occurred.
 * 	MS_ERR_PAYLOAD_SIZE -   if payloadlen is too large.
 * 	MS_ERR_MALFORMED_UTF8 - if the topic is not valid UTF-8.
 */
libms_EXPORT int myserver_will_set(struct myserver *ms, const char *topic, int payloadlen, const void *payload, int qos, bool retain);

/*
 * Function: myserver_will_clear
 *
 * Remove a previously configured will. This must be called before calling
 * <myserver_connect>.
 *
 * Parameters:
 * 	ms - a valid myserver instance.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 */
libms_EXPORT int myserver_will_clear(struct myserver *ms);

/*
 * Function: myserver_username_pw_set
 *
 * Configure username and password for a myservern instance. This is only
 * supported by brokers that implement the MQTT spec v3.1. By default, no
 * username or password will be sent.
 * If username is NULL, the password argument is ignored.
 * This must be called before calling myserver_connect().
 *
 * This is must be called before calling <myserver_connect>.
 *
 * Parameters:
 * 	ms -     a valid myserver instance.
 * 	username - the username to send as a string, or NULL to disable
 *             authentication.
 * 	password - the password to send as a string. Set to NULL when username is
 * 	           valid in order to send just a username.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 */
libms_EXPORT int myserver_username_pw_set(struct myserver *ms, const char *username, const char *password);

/*
 * Function: myserver_connect
 *
 * Connect to an MQTT broker.
 *
 * Parameters:
 * 	ms -      a valid myserver instance.
 * 	host -      the hostname or ip address of the broker to connect to.
 * 	port -      the network port to connect to. Usually 1883.
 * 	keepalive - the number of seconds after which the broker should send a PING
 *              message to the client if no other messages have been exchanged
 *              in that time.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_ERRNO -   if a system call returned an error. The variable errno
 *                     contains the error code, even on Windows.
 *                     Use strerror_r() where available or FormatMessage() on
 *                     Windows.
 *
 * See Also:
 * 	<myserver_connect_bind>, <myserver_connect_async>, <myserver_reconnect>, <myserver_disconnect>, <myserver_tls_set>
 */
libms_EXPORT int myserver_connect(struct myserver *ms, const char *host, int port, int keepalive);

/*
 * Function: myserver_connect_bind
 *
 * Connect to an MQTT broker. This extends the functionality of
 * <myserver_connect> by adding the bind_address parameter. Use this function
 * if you need to restrict network communication over a particular interface.
 *
 * Parameters:
 * 	ms -         a valid myserver instance.
 * 	host -         the hostname or ip address of the broker to connect to.
 * 	port -         the network port to connect to. Usually 1883.
 * 	keepalive -    the number of seconds after which the broker should send a PING
 *                 message to the client if no other messages have been exchanged
 *                 in that time.
 *  bind_address - the hostname or ip address of the local network interface to
 *                 bind to.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_ERRNO -   if a system call returned an error. The variable errno
 *                     contains the error code, even on Windows.
 *                     Use strerror_r() where available or FormatMessage() on
 *                     Windows.
 *
 * See Also:
 * 	<myserver_connect>, <myserver_connect_async>, <myserver_connect_bind_async>
 */
libms_EXPORT int myserver_connect_bind(struct myserver *ms, const char *host, int port, int keepalive, const char *bind_address);

/*
 * Function: myserver_connect_async
 *
 * Connect to an MQTT broker. This is a non-blocking call. If you use
 * <myserver_connect_async> your client must use the threaded interface
 * <myserver_loop_start>. If you need to use <myserver_loop>, you must use
 * <myserver_connect> to connect the client.
 *
 * May be called before or after <myserver_loop_start>.
 *
 * Parameters:
 * 	ms -      a valid myserver instance.
 * 	host -      the hostname or ip address of the broker to connect to.
 * 	port -      the network port to connect to. Usually 1883.
 * 	keepalive - the number of seconds after which the broker should send a PING
 *              message to the client if no other messages have been exchanged
 *              in that time.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_ERRNO -   if a system call returned an error. The variable errno
 *                     contains the error code, even on Windows.
 *                     Use strerror_r() where available or FormatMessage() on
 *                     Windows.
 *
 * See Also:
 * 	<myserver_connect_bind_async>, <myserver_connect>, <myserver_reconnect>, <myserver_disconnect>, <myserver_tls_set>
 */
libms_EXPORT int myserver_connect_async(struct myserver *ms, const char *host, int port, int keepalive);

/*
 * Function: myserver_connect_bind_async
 *
 * Connect to an MQTT broker. This is a non-blocking call. If you use
 * <myserver_connect_bind_async> your client must use the threaded interface
 * <myserver_loop_start>. If you need to use <myserver_loop>, you must use
 * <myserver_connect> to connect the client.
 *
 * This extends the functionality of <myserver_connect_async> by adding the
 * bind_address parameter. Use this function if you need to restrict network
 * communication over a particular interface.
 *
 * May be called before or after <myserver_loop_start>.
 *
 * Parameters:
 * 	ms -         a valid myserver instance.
 * 	host -         the hostname or ip address of the broker to connect to.
 * 	port -         the network port to connect to. Usually 1883.
 * 	keepalive -    the number of seconds after which the broker should send a PING
 *                 message to the client if no other messages have been exchanged
 *                 in that time.
 *  bind_address - the hostname or ip address of the local network interface to
 *                 bind to.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_ERRNO -   if a system call returned an error. The variable errno
 *                     contains the error code, even on Windows.
 *                     Use strerror_r() where available or FormatMessage() on
 *                     Windows.
 *
 * See Also:
 * 	<myserver_connect_async>, <myserver_connect>, <myserver_connect_bind>
 */
libms_EXPORT int myserver_connect_bind_async(struct myserver *ms, const char *host, int port, int keepalive, const char *bind_address);

/*
 * Function: myserver_connect_srv
 *
 * Connect to an MQTT broker. This is a non-blocking call. If you use
 * <myserver_connect_async> your client must use the threaded interface
 * <myserver_loop_start>. If you need to use <myserver_loop>, you must use
 * <myserver_connect> to connect the client.
 *
 * This extends the functionality of <myserver_connect_async> by adding the
 * bind_address parameter. Use this function if you need to restrict network
 * communication over a particular interface.
 *
 * May be called before or after <myserver_loop_start>.
 *
 * Parameters:
 * 	ms -         a valid myserver instance.
 * 	host -         the hostname or ip address of the broker to connect to.
 * 	keepalive -    the number of seconds after which the broker should send a PING
 *                 message to the client if no other messages have been exchanged
 *                 in that time.
 *  bind_address - the hostname or ip address of the local network interface to
 *                 bind to.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_ERRNO -   if a system call returned an error. The variable errno
 *                     contains the error code, even on Windows.
 *                     Use strerror_r() where available or FormatMessage() on
 *                     Windows.
 *
 * See Also:
 * 	<myserver_connect_async>, <myserver_connect>, <myserver_connect_bind>
 */
libms_EXPORT int myserver_connect_srv(struct myserver *ms, const char *host, int keepalive, const char *bind_address);

/*
 * Function: myserver_reconnect
 *
 * Reconnect to a broker.
 *
 * This function provides an easy way of reconnecting to a broker after a
 * connection has been lost. It uses the values that were provided in the
 * <myserver_connect> call. It must not be called before
 * <myserver_connect>.
 *
 * Parameters:
 * 	ms - a valid myserver instance.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_ERRNO -   if a system call returned an error. The variable errno
 *                     contains the error code, even on Windows.
 *                     Use strerror_r() where available or FormatMessage() on
 *                     Windows.
 *
 * See Also:
 * 	<myserver_connect>, <myserver_disconnect>, <myserver_reconnect_async>
 */
libms_EXPORT int myserver_reconnect(struct myserver *ms);

/*
 * Function: myserver_reconnect_async
 *
 * Reconnect to a broker. Non blocking version of <myserver_reconnect>.
 *
 * This function provides an easy way of reconnecting to a broker after a
 * connection has been lost. It uses the values that were provided in the
 * <myserver_connect> or <myserver_connect_async> calls. It must not be
 * called before <myserver_connect>.
 *
 * Parameters:
 * 	ms - a valid myserver instance.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 *
 * Returns:
 * 	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_ERRNO -   if a system call returned an error. The variable errno
 *                     contains the error code, even on Windows.
 *                     Use strerror_r() where available or FormatMessage() on
 *                     Windows.
 *
 * See Also:
 * 	<myserver_connect>, <myserver_disconnect>
 */
libms_EXPORT int myserver_reconnect_async(struct myserver *ms);

/*
 * Function: myserver_disconnect
 *
 * Disconnect from the broker.
 *
 * Parameters:
 *	ms - a valid myserver instance.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NO_CONN -  if the client isn't connected to a broker.
 */
libms_EXPORT int myserver_disconnect(struct myserver *ms);

/*
 * Function: myserver_publish
 *
 * Publish a message on a given topic.
 *
 * Parameters:
 * 	ms -       a valid myserver instance.
 * 	mid -        pointer to an int. If not NULL, the function will set this
 *               to the message id of this particular message. This can be then
 *               used with the publish callback to determine when the message
 *               has been sent.
 *               Note that although the MQTT protocol doesn't use message ids
 *               for messages with QoS=0, libmyserver assigns them message ids
 *               so they can be tracked with this parameter.
 *  topic -      null terminated string of the topic to publish to.
 * 	payloadlen - the size of the payload (bytes). Valid values are between 0 and
 *               268,435,455.
 * 	payload -    pointer to the data to send. If payloadlen > 0 this must be a
 *               valid memory location.
 * 	qos -        integer value 0, 1 or 2 indicating the Quality of Service to be
 *               used for the message.
 * 	retain -     set to true to make the message retained.
 *
 * Returns:
 * 	MS_ERR_SUCCESS -        on success.
 * 	MS_ERR_INVAL -          if the input parameters were invalid.
 * 	MS_ERR_NOMEM -          if an out of memory condition occurred.
 * 	MS_ERR_NO_CONN -        if the client isn't connected to a broker.
 *	MS_ERR_PROTOCOL -       if there is a protocol error communicating with the
 *                            broker.
 * 	MS_ERR_PAYLOAD_SIZE -   if payloadlen is too large.
 * 	MS_ERR_MALFORMED_UTF8 - if the topic is not valid UTF-8
 *
 * See Also:
 *	<myserver_max_inflight_messages_set>
 */
libms_EXPORT int myserver_publish(struct myserver *ms, int *mid, const char *topic, int payloadlen, const void *payload, int qos, bool retain);

/*
 * Function: myserver_subscribe
 *
 * Subscribe to a topic.
 *
 * Parameters:
 *	ms - a valid myserver instance.
 *	mid -  a pointer to an int. If not NULL, the function will set this to
 *	       the message id of this particular message. This can be then used
 *	       with the subscribe callback to determine when the message has been
 *	       sent.
 *	sub -  the subscription pattern.
 *	qos -  the requested Quality of Service for this subscription.
 *
 * Returns:
 *	MS_ERR_SUCCESS -        on success.
 * 	MS_ERR_INVAL -          if the input parameters were invalid.
 * 	MS_ERR_NOMEM -          if an out of memory condition occurred.
 * 	MS_ERR_NO_CONN -        if the client isn't connected to a broker.
 * 	MS_ERR_MALFORMED_UTF8 - if the topic is not valid UTF-8
 */
libms_EXPORT int myserver_subscribe(struct myserver *ms, int *mid, const char *sub, int qos);

/*
 * Function: myserver_unsubscribe
 *
 * Unsubscribe from a topic.
 *
 * Parameters:
 *	ms - a valid myserver instance.
 *	mid -  a pointer to an int. If not NULL, the function will set this to
 *	       the message id of this particular message. This can be then used
 *	       with the unsubscribe callback to determine when the message has been
 *	       sent.
 *	sub -  the unsubscription pattern.
 *
 * Returns:
 *	MS_ERR_SUCCESS -        on success.
 * 	MS_ERR_INVAL -          if the input parameters were invalid.
 * 	MS_ERR_NOMEM -          if an out of memory condition occurred.
 * 	MS_ERR_NO_CONN -        if the client isn't connected to a broker.
 * 	MS_ERR_MALFORMED_UTF8 - if the topic is not valid UTF-8
 */
libms_EXPORT int myserver_unsubscribe(struct myserver *ms, int *mid, const char *sub);

/*
 * Function: myserver_message_copy
 *
 * Copy the contents of a myserver message to another message.
 * Useful for preserving a message received in the on_message() callback.
 *
 * Parameters:
 *	dst - a pointer to a valid myserver_message struct to copy to.
 *	src - a pointer to a valid myserver_message struct to copy from.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 *
 * See Also:
 * 	<myserver_message_free>
 */
libms_EXPORT int myserver_message_copy(struct myserver_message *dst, const struct myserver_message *src);

/*
 * Function: myserver_message_free
 *
 * Completely free a myserver_message struct.
 *
 * Parameters:
 *	message - pointer to a myserver_message pointer to free.
 *
 * See Also:
 * 	<myserver_message_copy>, <myserver_message_free_contents>
 */
libms_EXPORT void myserver_message_free(struct myserver_message **message);

/*
 * Function: myserver_message_free_contents
 *
 * Free a myserver_message struct contents, leaving the struct unaffected.
 *
 * Parameters:
 *	message - pointer to a myserver_message struct to free its contents.
 *
 * See Also:
 * 	<myserver_message_copy>, <myserver_message_free>
 */
libms_EXPORT void myserver_message_free_contents(struct myserver_message *message);

/*
 * Function: myserver_loop
 *
 * The main network loop for the client. You must call this frequently in order
 * to keep communications between the client and broker working. If incoming
 * data is present it will then be processed. Outgoing commands, from e.g.
 * <myserver_publish>, are normally sent immediately that their function is
 * called, but this is not always possible. <myserver_loop> will also attempt
 * to send any remaining outgoing messages, which also includes commands that
 * are part of the flow for messages with QoS>0.
 *
 * An alternative approach is to use <myserver_loop_start> to run the client
 * loop in its own thread.
 *
 * This calls select() to monitor the client network socket. If you want to
 * integrate myserver client operation with your own select() call, use
 * <myserver_socket>, <myserver_loop_read>, <myserver_loop_write> and
 * <myserver_loop_misc>.
 *
 * Threads:
 *
 * Parameters:
 *	ms -        a valid myserver instance.
 *	timeout -     Maximum number of milliseconds to wait for network activity
 *	              in the select() call before timing out. Set to 0 for instant
 *	              return.  Set negative to use the default of 1000ms.
 *	max_packets - this parameter is currently unused and should be set to 1 for
 *	              future compatibility.
 *
 * Returns:
 *	MS_ERR_SUCCESS -   on success.
 * 	MS_ERR_INVAL -     if the input parameters were invalid.
 * 	MS_ERR_NOMEM -     if an out of memory condition occurred.
 * 	MS_ERR_NO_CONN -   if the client isn't connected to a broker.
 *  MS_ERR_CONN_LOST - if the connection to the broker was lost.
 *	MS_ERR_PROTOCOL -  if there is a protocol error communicating with the
 *                       broker.
 * 	MS_ERR_ERRNO -     if a system call returned an error. The variable errno
 *                       contains the error code, even on Windows.
 *                       Use strerror_r() where available or FormatMessage() on
 *                       Windows.
 * See Also:
 *	<myserver_loop_forever>, <myserver_loop_start>, <myserver_loop_stop>
 */
libms_EXPORT int myserver_loop(struct myserver *ms, int timeout, int max_packets);

/*
 * Function: myserver_loop_forever
 *
 * This function call loop() for you in an infinite blocking loop. It is useful
 * for the case where you only want to run the MQTT client loop in your
 * program.
 *
 * It handles reconnecting in case server connection is lost. If you call
 * myserver_disconnect() in a callback it will return.
 *
 * Parameters:
 *  ms - a valid myserver instance.
 *	timeout -     Maximum number of milliseconds to wait for network activity
 *	              in the select() call before timing out. Set to 0 for instant
 *	              return.  Set negative to use the default of 1000ms.
 *	max_packets - this parameter is currently unused and should be set to 1 for
 *	              future compatibility.
 *
 * Returns:
 *	MS_ERR_SUCCESS -   on success.
 * 	MS_ERR_INVAL -     if the input parameters were invalid.
 * 	MS_ERR_NOMEM -     if an out of memory condition occurred.
 * 	MS_ERR_NO_CONN -   if the client isn't connected to a broker.
 *  MS_ERR_CONN_LOST - if the connection to the broker was lost.
 *	MS_ERR_PROTOCOL -  if there is a protocol error communicating with the
 *                       broker.
 * 	MS_ERR_ERRNO -     if a system call returned an error. The variable errno
 *                       contains the error code, even on Windows.
 *                       Use strerror_r() where available or FormatMessage() on
 *                       Windows.
 *
 * See Also:
 *	<myserver_loop>, <myserver_loop_start>
 */
libms_EXPORT int myserver_loop_forever(struct myserver *ms, int timeout, int max_packets);

/*
 * Function: myserver_loop_start
 *
 * This is part of the threaded client interface. Call this once to start a new
 * thread to process network traffic. This provides an alternative to
 * repeatedly calling <myserver_loop> yourself.
 *
 * Parameters:
 *  ms - a valid myserver instance.
 *
 * Returns:
 *	MS_ERR_SUCCESS -       on success.
 * 	MS_ERR_INVAL -         if the input parameters were invalid.
 *	MS_ERR_NOT_SUPPORTED - if thread support is not available.
 *
 * See Also:
 *	<myserver_connect_async>, <myserver_loop>, <myserver_loop_forever>, <myserver_loop_stop>
 */
libms_EXPORT int myserver_loop_start(struct myserver *ms);

/*
 * Function: myserver_loop_stop
 *
 * This is part of the threaded client interface. Call this once to stop the
 * network thread previously created with <myserver_loop_start>. This call
 * will block until the network thread finishes. For the network thread to end,
 * you must have previously called <myserver_disconnect> or have set the force
 * parameter to true.
 *
 * Parameters:
 *  ms - a valid myserver instance.
 *	force - set to true to force thread cancellation. If false,
 *	        <myserver_disconnect> must have already been called.
 *
 * Returns:
 *	MS_ERR_SUCCESS -       on success.
 * 	MS_ERR_INVAL -         if the input parameters were invalid.
 *	MS_ERR_NOT_SUPPORTED - if thread support is not available.
 *
 * See Also:
 *	<myserver_loop>, <myserver_loop_start>
 */
libms_EXPORT int myserver_loop_stop(struct myserver *ms, bool force);

/*
 * Function: myserver_socket
 *
 * Return the socket handle for a myserver instance. Useful if you want to
 * include a myserver client in your own select() calls.
 *
 * Parameters:
 *	ms - a valid myserver instance.
 *
 * Returns:
 *	The socket for the myserver client or -1 on failure.
 */
libms_EXPORT int myserver_socket(struct myserver *ms);

/*
 * Function: myserver_loop_read
 *
 * Carry out network read operations.
 * This should only be used if you are not using myserver_loop() and are
 * monitoring the client network socket for activity yourself.
 *
 * Parameters:
 *	ms -        a valid myserver instance.
 *	max_packets - this parameter is currently unused and should be set to 1 for
 *	              future compatibility.
 *
 * Returns:
 *	MS_ERR_SUCCESS -   on success.
 * 	MS_ERR_INVAL -     if the input parameters were invalid.
 * 	MS_ERR_NOMEM -     if an out of memory condition occurred.
 * 	MS_ERR_NO_CONN -   if the client isn't connected to a broker.
 *  MS_ERR_CONN_LOST - if the connection to the broker was lost.
 *	MS_ERR_PROTOCOL -  if there is a protocol error communicating with the
 *                       broker.
 * 	MS_ERR_ERRNO -     if a system call returned an error. The variable errno
 *                       contains the error code, even on Windows.
 *                       Use strerror_r() where available or FormatMessage() on
 *                       Windows.
 *
 * See Also:
 *	<myserver_socket>, <myserver_loop_write>, <myserver_loop_misc>
 */
libms_EXPORT int myserver_loop_read(struct myserver *ms, int max_packets);

/*
 * Function: myserver_loop_write
 *
 * Carry out network write operations.
 * This should only be used if you are not using myserver_loop() and are
 * monitoring the client network socket for activity yourself.
 *
 * Parameters:
 *	ms -        a valid myserver instance.
 *	max_packets - this parameter is currently unused and should be set to 1 for
 *	              future compatibility.
 *
 * Returns:
 *	MS_ERR_SUCCESS -   on success.
 * 	MS_ERR_INVAL -     if the input parameters were invalid.
 * 	MS_ERR_NOMEM -     if an out of memory condition occurred.
 * 	MS_ERR_NO_CONN -   if the client isn't connected to a broker.
 *  MS_ERR_CONN_LOST - if the connection to the broker was lost.
 *	MS_ERR_PROTOCOL -  if there is a protocol error communicating with the
 *                       broker.
 * 	MS_ERR_ERRNO -     if a system call returned an error. The variable errno
 *                       contains the error code, even on Windows.
 *                       Use strerror_r() where available or FormatMessage() on
 *                       Windows.
 *
 * See Also:
 *	<myserver_socket>, <myserver_loop_read>, <myserver_loop_misc>, <myserver_want_write>
 */
libms_EXPORT int myserver_loop_write(struct myserver *ms, int max_packets);

/*
 * Function: myserver_loop_misc
 *
 * Carry out miscellaneous operations required as part of the network loop.
 * This should only be used if you are not using myserver_loop() and are
 * monitoring the client network socket for activity yourself.
 *
 * This function deals with handling PINGs and checking whether messages need
 * to be retried, so should be called fairly frequently.
 *
 * Parameters:
 *	ms - a valid myserver instance.
 *
 * Returns:
 *	MS_ERR_SUCCESS -   on success.
 * 	MS_ERR_INVAL -     if the input parameters were invalid.
 * 	MS_ERR_NO_CONN -   if the client isn't connected to a broker.
 *
 * See Also:
 *	<myserver_socket>, <myserver_loop_read>, <myserver_loop_write>
 */
libms_EXPORT int myserver_loop_misc(struct myserver *ms);

/*
 * Function: myserver_want_write
 *
 * Returns true if there is data ready to be written on the socket.
 *
 * Parameters:
 *	ms - a valid myserver instance.
 *
 * See Also:
 *	<myserver_socket>, <myserver_loop_read>, <myserver_loop_write>
 */
libms_EXPORT bool myserver_want_write(struct myserver *ms);

/*
 * Function: myserver_threaded_set
 *
 * Used to tell the library that your application is using threads, but not
 * using <myserver_loop_start>. The library operates slightly differently when
 * not in threaded mode in order to simplify its operation. If you are managing
 * your own threads and do not use this function you will experience crashes
 * due to race conditions.
 *
 * When using <myserver_loop_start>, this is set automatically.
 *
 * Parameters:
 *  ms -     a valid myserver instance.
 *  threaded - true if your application is using threads, false otherwise.
 */
libms_EXPORT int myserver_threaded_set(struct myserver *ms, bool threaded);

/*
 * Function: myserver_opts_set
 *
 * Used to set options for the client.
 *
 * Parameters:
 *	ms -   a valid myserver instance.
 *	option - the option to set.
 *	value -  the option specific value.
 *
 * Options:
 *	MS_OPT_PROTOCOL_VERSION
 *	          Value must be an int, set to either MQTT_PROTOCOL_V31 or
 *	          MQTT_PROTOCOL_V311. Must be set before the client connects.
 *	          Defaults to MQTT_PROTOCOL_V31.
 *
 *	MS_OPT_SSL_CTX
 *	          Pass an openssl SSL_CTX to be used when creating TLS connections
 *	          rather than libmyserver creating its own.  This must be called
 *	          before connecting to have any effect. If you use this option, the
 *	          onus is on you to ensure that you are using secure settings.
 *	          Setting to NULL means that libmyserver will use its own SSL_CTX
 *	          if TLS is to be used.
 *	          This option is only available for openssl 1.1.0 and higher.
 *
 *	MS_OPT_SSL_CTX_WITH_DEFAULTS
 *	          Value must be an int set to 1 or 0. If set to 1, then the user
 *	          specified SSL_CTX passed in using MS_OPT_SSL_CTX will have the
 *	          default options applied to it. This means that you only need to
 *	          change the values that are relevant to you. If you use this
 *	          option then you must configure the TLS options as normal, i.e.
 *	          you should use <myserver_tls_set> to configure the cafile/capath
 *	          as a minimum.
 *	          This option is only available for openssl 1.1.0 and higher.
 */
libms_EXPORT int myserver_opts_set(struct myserver *ms, enum ms_opt_t option, void *value);


/*
 * Function: myserver_tls_set
 *
 * Configure the client for certificate based SSL/TLS support. Must be called
 * before <myserver_connect>.
 *
 * Cannot be used in conjunction with <myserver_tls_psk_set>.
 *
 * Define the Certificate Authority certificates to be trusted (ie. the server
 * certificate must be signed with one of these certificates) using cafile.
 *
 * If the server you are connecting to requires clients to provide a
 * certificate, define certfile and keyfile with your client certificate and
 * private key. If your private key is encrypted, provide a password callback
 * function or you will have to enter the password at the command line.
 *
 * Parameters:
 *  ms -        a valid myserver instance.
 *  cafile -      path to a file containing the PEM encoded trusted CA
 *                certificate files. Either cafile or capath must not be NULL.
 *  capath -      path to a directory containing the PEM encoded trusted CA
 *                certificate files. See myserver.conf for more details on
 *                configuring this directory. Either cafile or capath must not
 *                be NULL.
 *  certfile -    path to a file containing the PEM encoded certificate file
 *                for this client. If NULL, keyfile must also be NULL and no
 *                client certificate will be used.
 *  keyfile -     path to a file containing the PEM encoded private key for
 *                this client. If NULL, certfile must also be NULL and no
 *                client certificate will be used.
 *  pw_callback - if keyfile is encrypted, set pw_callback to allow your client
 *                to pass the correct password for decryption. If set to NULL,
 *                the password must be entered on the command line.
 *                Your callback must write the password into "buf", which is
 *                "size" bytes long. The return value must be the length of the
 *                password. "userdata" will be set to the calling myserver
 *                instance. The myserver userdata member variable can be
 *                retrieved using <myserver_userdata>.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 *
 * See Also:
 *	<myserver_tls_opts_set>, <myserver_tls_psk_set>,
 *	<myserver_tls_insecure_set>, <myserver_userdata>
 */
libms_EXPORT int myserver_tls_set(struct myserver *ms,
		const char *cafile, const char *capath,
		const char *certfile, const char *keyfile,
		int (*pw_callback)(char *buf, int size, int rwflag, void *userdata));

/*
 * Function: myserver_tls_insecure_set
 *
 * Configure verification of the server hostname in the server certificate. If
 * value is set to true, it is impossible to guarantee that the host you are
 * connecting to is not impersonating your server. This can be useful in
 * initial server testing, but makes it possible for a malicious third party to
 * impersonate your server through DNS spoofing, for example.
 * Do not use this function in a real system. Setting value to true makes the
 * connection encryption pointless.
 * Must be called before <myserver_connect>.
 *
 * Parameters:
 *  ms -  a valid myserver instance.
 *  value - if set to false, the default, certificate hostname checking is
 *          performed. If set to true, no hostname checking is performed and
 *          the connection is insecure.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 *
 * See Also:
 *	<myserver_tls_set>
 */
libms_EXPORT int myserver_tls_insecure_set(struct myserver *ms, bool value);

/*
 * Function: myserver_tls_opts_set
 *
 * Set advanced SSL/TLS options. Must be called before <myserver_connect>.
 *
 * Parameters:
 *  ms -        a valid myserver instance.
 *	cert_reqs -   an integer defining the verification requirements the client
 *	              will impose on the server. This can be one of:
 *	              * SSL_VERIFY_NONE (0): the server will not be verified in any way.
 *	              * SSL_VERIFY_PEER (1): the server certificate will be verified
 *	                and the connection aborted if the verification fails.
 *	              The default and recommended value is SSL_VERIFY_PEER. Using
 *	              SSL_VERIFY_NONE provides no security.
 *	tls_version - the version of the SSL/TLS protocol to use as a string. If NULL,
 *	              the default value is used. The default value and the
 *	              available values depend on the version of openssl that the
 *	              library was compiled against. For openssl >= 1.0.1, the
 *	              available options are tlsv1.2, tlsv1.1 and tlsv1, with tlv1.2
 *	              as the default. For openssl < 1.0.1, only tlsv1 is available.
 *	ciphers -     a string describing the ciphers available for use. See the
 *	              "openssl ciphers" tool for more information. If NULL, the
 *	              default ciphers will be used.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 *
 * See Also:
 *	<myserver_tls_set>
 */
libms_EXPORT int myserver_tls_opts_set(struct myserver *ms, int cert_reqs, const char *tls_version, const char *ciphers);

/*
 * Function: myserver_tls_psk_set
 *
 * Configure the client for pre-shared-key based TLS support. Must be called
 * before <myserver_connect>.
 *
 * Cannot be used in conjunction with <myserver_tls_set>.
 *
 * Parameters:
 *  ms -     a valid myserver instance.
 *  psk -      the pre-shared-key in hex format with no leading "0x".
 *  identity - the identity of this client. May be used as the username
 *             depending on the server settings.
 *	ciphers -  a string describing the PSK ciphers available for use. See the
 *	           "openssl ciphers" tool for more information. If NULL, the
 *	           default ciphers will be used.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 *
 * See Also:
 *	<myserver_tls_set>
 */
libms_EXPORT int myserver_tls_psk_set(struct myserver *ms, const char *psk, const char *identity, const char *ciphers);

/*
 * Function: myserver_connect_callback_set
 *
 * Set the connect callback. This is called when the broker sends a CONNACK
 * message in response to a connection.
 *
 * Parameters:
 *  ms -       a valid myserver instance.
 *  on_connect - a callback function in the following form:
 *               void callback(struct myserver *ms, void *obj, int rc)
 *
 * Callback Parameters:
 *  ms - the myserver instance making the callback.
 *  obj - the user data provided in <myserver_new>
 *  rc -  the return code of the connection response, one of:
 *
 * * 0 - success
 * * 1 - connection refused (unacceptable protocol version)
 * * 2 - connection refused (identifier rejected)
 * * 3 - connection refused (broker unavailable)
 * * 4-255 - reserved for future use
 */
libms_EXPORT void myserver_connect_callback_set(struct myserver *ms, void (*on_connect)(struct myserver *, void *, int));

/*
 * Function: myserver_connect_with_flags_callback_set
 *
 * Set the connect callback. This is called when the broker sends a CONNACK
 * message in response to a connection.
 *
 * Parameters:
 *  ms -       a valid myserver instance.
 *  on_connect - a callback function in the following form:
 *               void callback(struct myserver *ms, void *obj, int rc)
 *
 * Callback Parameters:
 *  ms - the myserver instance making the callback.
 *  obj - the user data provided in <myserver_new>
 *  rc -  the return code of the connection response, one of:
 *  flags - the connect flags.
 *
 * * 0 - success
 * * 1 - connection refused (unacceptable protocol version)
 * * 2 - connection refused (identifier rejected)
 * * 3 - connection refused (broker unavailable)
 * * 4-255 - reserved for future use
 */
libms_EXPORT void myserver_connect_with_flags_callback_set(struct myserver *ms, void (*on_connect)(struct myserver *, void *, int, int));

/*
 * Function: myserver_disconnect_callback_set
 *
 * Set the disconnect callback. This is called when the broker has received the
 * DISCONNECT command and has disconnected the client.
 *
 * Parameters:
 *  ms -          a valid myserver instance.
 *  on_disconnect - a callback function in the following form:
 *                  void callback(struct myserver *ms, void *obj)
 *
 * Callback Parameters:
 *  ms - the myserver instance making the callback.
 *  obj -  the user data provided in <myserver_new>
 *  rc -   integer value indicating the reason for the disconnect. A value of 0
 *         means the client has called <myserver_disconnect>. Any other value
 *         indicates that the disconnect is unexpected.
 */
libms_EXPORT void myserver_disconnect_callback_set(struct myserver *ms, void (*on_disconnect)(struct myserver *, void *, int));

/*
 * Function: myserver_publish_callback_set
 *
 * Set the publish callback. This is called when a message initiated with
 * <myserver_publish> has been sent to the broker successfully.
 *
 * Parameters:
 *  ms -       a valid myserver instance.
 *  on_publish - a callback function in the following form:
 *               void callback(struct myserver *ms, void *obj, int mid)
 *
 * Callback Parameters:
 *  ms - the myserver instance making the callback.
 *  obj -  the user data provided in <myserver_new>
 *  mid -  the message id of the sent message.
 */
libms_EXPORT void myserver_publish_callback_set(struct myserver *ms, void (*on_publish)(struct myserver *, void *, int));

/*
 * Function: myserver_message_callback_set
 *
 * Set the message callback. This is called when a message is received from the
 * broker.
 *
 * Parameters:
 *  ms -       a valid myserver instance.
 *  on_message - a callback function in the following form:
 *               void callback(struct myserver *ms, void *obj, const struct myserver_message *message)
 *
 * Callback Parameters:
 *  ms -    the myserver instance making the callback.
 *  obj -     the user data provided in <myserver_new>
 *  message - the message data. This variable and associated memory will be
 *            freed by the library after the callback completes. The client
 *            should make copies of any of the data it requires.
 *
 * See Also:
 * 	<myserver_message_copy>
 */
libms_EXPORT void myserver_message_callback_set(struct myserver *ms, void (*on_message)(struct myserver *, void *, const struct myserver_message *));

/*
 * Function: myserver_subscribe_callback_set
 *
 * Set the subscribe callback. This is called when the broker responds to a
 * subscription request.
 *
 * Parameters:
 *  ms -         a valid myserver instance.
 *  on_subscribe - a callback function in the following form:
 *                 void callback(struct myserver *ms, void *obj, int mid, int qos_count, const int *granted_qos)
 *
 * Callback Parameters:
 *  ms -        the myserver instance making the callback.
 *  obj -         the user data provided in <myserver_new>
 *  mid -         the message id of the subscribe message.
 *  qos_count -   the number of granted subscriptions (size of granted_qos).
 *  granted_qos - an array of integers indicating the granted QoS for each of
 *                the subscriptions.
 */
libms_EXPORT void myserver_subscribe_callback_set(struct myserver *ms, void (*on_subscribe)(struct myserver *, void *, int, int, const int *));

/*
 * Function: myserver_unsubscribe_callback_set
 *
 * Set the unsubscribe callback. This is called when the broker responds to a
 * unsubscription request.
 *
 * Parameters:
 *  ms -           a valid myserver instance.
 *  on_unsubscribe - a callback function in the following form:
 *                   void callback(struct myserver *ms, void *obj, int mid)
 *
 * Callback Parameters:
 *  ms - the myserver instance making the callback.
 *  obj -  the user data provided in <myserver_new>
 *  mid -  the message id of the unsubscribe message.
 */
libms_EXPORT void myserver_unsubscribe_callback_set(struct myserver *ms, void (*on_unsubscribe)(struct myserver *, void *, int));

/*
 * Function: myserver_log_callback_set
 *
 * Set the logging callback. This should be used if you want event logging
 * information from the client library.
 *
 *  ms -   a valid myserver instance.
 *  on_log - a callback function in the following form:
 *           void callback(struct myserver *ms, void *obj, int level, const char *str)
 *
 * Callback Parameters:
 *  ms -  the myserver instance making the callback.
 *  obj -   the user data provided in <myserver_new>
 *  level - the log message level from the values:
 *	        MS_LOG_INFO
 *	        MS_LOG_NOTICE
 *	        MS_LOG_WARNING
 *	        MS_LOG_ERR
 *	        MS_LOG_DEBUG
 *	str -   the message string.
 */
libms_EXPORT void myserver_log_callback_set(struct myserver *ms, void (*on_log)(struct myserver *, void *, int, const char *));

/*
 * Function: myserver_reconnect_delay_set
 *
 * Control the behaviour of the client when it has unexpectedly disconnected in
 * <myserver_loop_forever> or after <myserver_loop_start>. The default
 * behaviour if this function is not used is to repeatedly attempt to reconnect
 * with a delay of 1 second until the connection succeeds.
 *
 * Use reconnect_delay parameter to change the delay between successive
 * reconnection attempts. You may also enable exponential backoff of the time
 * between reconnections by setting reconnect_exponential_backoff to true and
 * set an upper bound on the delay with reconnect_delay_max.
 *
 * Example 1:
 *	delay=2, delay_max=10, exponential_backoff=False
 *	Delays would be: 2, 4, 6, 8, 10, 10, ...
 *
 * Example 2:
 *	delay=3, delay_max=30, exponential_backoff=True
 *	Delays would be: 3, 6, 12, 24, 30, 30, ...
 *
 * Parameters:
 *  ms -                          a valid myserver instance.
 *  reconnect_delay -               the number of seconds to wait between
 *                                  reconnects.
 *  reconnect_delay_max -           the maximum number of seconds to wait
 *                                  between reconnects.
 *  reconnect_exponential_backoff - use exponential backoff between
 *                                  reconnect attempts. Set to true to enable
 *                                  exponential backoff.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 */
libms_EXPORT int myserver_reconnect_delay_set(struct myserver *ms, unsigned int reconnect_delay, unsigned int reconnect_delay_max, bool reconnect_exponential_backoff);

/*
 * Function: myserver_max_inflight_messages_set
 *
 * Set the number of QoS 1 and 2 messages that can be "in flight" at one time.
 * An in flight message is part way through its delivery flow. Attempts to send
 * further messages with <myserver_publish> will result in the messages being
 * queued until the number of in flight messages reduces.
 *
 * A higher number here results in greater message throughput, but if set
 * higher than the maximum in flight messages on the broker may lead to
 * delays in the messages being acknowledged.
 *
 * Set to 0 for no maximum.
 *
 * Parameters:
 *  ms -                  a valid myserver instance.
 *  max_inflight_messages - the maximum number of inflight messages. Defaults
 *                          to 20.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success.
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 */
libms_EXPORT int myserver_max_inflight_messages_set(struct myserver *ms, unsigned int max_inflight_messages);

/*
 * Function: myserver_message_retry_set
 *
 * This function now has no effect.
 */
libms_EXPORT void myserver_message_retry_set(struct myserver *ms, unsigned int message_retry);

/*
 * Function: myserver_user_data_set
 *
 * When <myserver_new> is called, the pointer given as the "obj" parameter
 * will be passed to the callbacks as user data. The <myserver_user_data_set>
 * function allows this obj parameter to be updated at any time. This function
 * will not modify the memory pointed to by the current user data pointer. If
 * it is dynamically allocated memory you must free it yourself.
 *
 * Parameters:
 *  ms - a valid myserver instance.
 * 	obj -  A user pointer that will be passed as an argument to any callbacks
 * 	       that are specified.
 */
libms_EXPORT void myserver_user_data_set(struct myserver *ms, void *obj);

/* =============================================================================
 *
 * Section: SOCKS5 proxy functions
 *
 * =============================================================================
 */

/*
 * Function: myserver_socks5_set
 *
 * Configure the client to use a SOCKS5 proxy when connecting. Must be called
 * before connecting. "None" and "username/password" authentication is
 * supported.
 *
 * Parameters:
 *   ms - a valid myserver instance.
 *   host - the SOCKS5 proxy host to connect to.
 *   port - the SOCKS5 proxy port to use.
 *   username - if not NULL, use this username when authenticating with the proxy.
 *   password - if not NULL and username is not NULL, use this password when
 *              authenticating with the proxy.
 */
libms_EXPORT int myserver_socks5_set(struct myserver *ms, const char *host, int port, const char *username, const char *password);

/* =============================================================================
 *
 * Section: Utility functions
 *
 * =============================================================================
 */

/*
 * Function: myserver_strerror
 *
 * Call to obtain a const string description of a myserver error number.
 *
 * Parameters:
 *	ms_errno - a myserver error number.
 *
 * Returns:
 *	A constant string describing the error.
 */
libms_EXPORT const char *myserver_strerror(int ms_errno);

/*
 * Function: myserver_connack_string
 *
 * Call to obtain a const string description of an MQTT connection result.
 *
 * Parameters:
 *	connack_code - an MQTT connection result.
 *
 * Returns:
 *	A constant string describing the result.
 */
libms_EXPORT const char *myserver_connack_string(int connack_code);

/*
 * Function: myserver_sub_topic_tokenise
 *
 * Tokenise a topic or subscription string into an array of strings
 * representing the topic hierarchy.
 *
 * For example:
 *
 * subtopic: "a/deep/topic/hierarchy"
 *
 * Would result in:
 *
 * topics[0] = "a"
 * topics[1] = "deep"
 * topics[2] = "topic"
 * topics[3] = "hierarchy"
 *
 * and:
 *
 * subtopic: "/a/deep/topic/hierarchy/"
 *
 * Would result in:
 *
 * topics[0] = NULL
 * topics[1] = "a"
 * topics[2] = "deep"
 * topics[3] = "topic"
 * topics[4] = "hierarchy"
 *
 * Parameters:
 *	subtopic - the subscription/topic to tokenise
 *	topics -   a pointer to store the array of strings
 *	count -    an int pointer to store the number of items in the topics array.
 *
 * Returns:
 *	MS_ERR_SUCCESS -        on success
 * 	MS_ERR_NOMEM -          if an out of memory condition occurred.
 * 	MS_ERR_MALFORMED_UTF8 - if the topic is not valid UTF-8
 *
 * Example:
 *
 * > char **topics;
 * > int topic_count;
 * > int i;
 * >
 * > myserver_sub_topic_tokenise("$SYS/broker/uptime", &topics, &topic_count);
 * >
 * > for(i=0; i<token_count; i++){
 * >     printf("%d: %s\n", i, topics[i]);
 * > }
 *
 * See Also:
 *	<myserver_sub_topic_tokens_free>
 */
libms_EXPORT int myserver_sub_topic_tokenise(const char *subtopic, char ***topics, int *count);

/*
 * Function: myserver_sub_topic_tokens_free
 *
 * Free memory that was allocated in <myserver_sub_topic_tokenise>.
 *
 * Parameters:
 *	topics - pointer to string array.
 *	count - count of items in string array.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 *
 * See Also:
 *	<myserver_sub_topic_tokenise>
 */
libms_EXPORT int myserver_sub_topic_tokens_free(char ***topics, int count);

/*
 * Function: myserver_topic_matches_sub
 * Function: myserver_topic_matches_sub2
 *
 * Check whether a topic matches a subscription.
 *
 * For example:
 *
 * foo/bar would match the subscription foo/# or +/bar
 * non/matching would not match the subscription non/+/+
 *
 * Parameters:
 *	sub - subscription string to check topic against.
 *	sublen - length in bytes of sub string
 *	topic - topic to check.
 *	topiclen - length in bytes of topic string
 *	result - bool pointer to hold result. Will be set to true if the topic
 *	         matches the subscription.
 *
 * Returns:
 *	MS_ERR_SUCCESS - on success
 * 	MS_ERR_INVAL -   if the input parameters were invalid.
 * 	MS_ERR_NOMEM -   if an out of memory condition occurred.
 */
libms_EXPORT int myserver_topic_matches_sub(const char *sub, const char *topic, bool *result);
libms_EXPORT int myserver_topic_matches_sub2(const char *sub, size_t sublen, const char *topic, size_t topiclen, bool *result);

/*
 * Function: myserver_pub_topic_check
 *
 * Check whether a topic to be used for publishing is valid.
 *
 * This searches for + or # in a topic and checks its length.
 *
 * This check is already carried out in <myserver_publish> and
 * <myserver_will_set>, there is no need to call it directly before them. It
 * may be useful if you wish to check the validity of a topic in advance of
 * making a connection for example.
 *
 * Parameters:
 *   topic - the topic to check
 *   topiclen - length of the topic in bytes
 *
 * Returns:
 *   MS_ERR_SUCCESS -        for a valid topic
 *   MS_ERR_INVAL -          if the topic contains a + or a #, or if it is too long.
 * 	 MS_ERR_MALFORMED_UTF8 - if sub or topic is not valid UTF-8
 *
 * See Also:
 *   <myserver_sub_topic_check>
 */
libms_EXPORT int myserver_pub_topic_check(const char *topic);
libms_EXPORT int myserver_pub_topic_check2(const char *topic, size_t topiclen);

/*
 * Function: myserver_sub_topic_check
 *
 * Check whether a topic to be used for subscribing is valid.
 *
 * This searches for + or # in a topic and checks that they aren't in invalid
 * positions, such as with foo/#/bar, foo/+bar or foo/bar#, and checks its
 * length.
 *
 * This check is already carried out in <myserver_subscribe> and
 * <myserver_unsubscribe>, there is no need to call it directly before them.
 * It may be useful if you wish to check the validity of a topic in advance of
 * making a connection for example.
 *
 * Parameters:
 *   topic - the topic to check
 *   topiclen - the length in bytes of the topic
 *
 * Returns:
 *   MS_ERR_SUCCESS -        for a valid topic
 *   MS_ERR_INVAL -          if the topic contains a + or a # that is in an
 *                             invalid position, or if it is too long.
 * 	 MS_ERR_MALFORMED_UTF8 - if topic is not valid UTF-8
 *
 * See Also:
 *   <myserver_sub_topic_check>
 */
libms_EXPORT int myserver_sub_topic_check(const char *topic);
libms_EXPORT int myserver_sub_topic_check2(const char *topic, size_t topiclen);


struct libmyserver_will {
	char *topic;
	void *payload;
	int payloadlen;
	int qos;
	bool retain;
};

struct libmyserver_auth {
	char *username;
	char *password;
};

struct libmyserver_tls {
	char *cafile;
	char *capath;
	char *certfile;
	char *keyfile;
	char *ciphers;
	char *tls_version;
	int (*pw_callback)(char *buf, int size, int rwflag, void *userdata);
	int cert_reqs;
};

/*
 * Function: myserver_subscribe_simple
 *
 * Helper function to make subscribing to a topic and retrieving some messages
 * very straightforward.
 *
 * This connects to a broker, subscribes to a topic, waits for msg_count
 * messages to be received, then returns after disconnecting cleanly.
 *
 * Parameters:
 *   messages - pointer to a "struct myserver_message *". The received
 *              messages will be returned here. On error, this will be set to
 *              NULL.
 *   msg_count - the number of messages to retrieve.
 *   want_retained - if set to true, stale retained messages will be treated as
 *                   normal messages with regards to msg_count. If set to
 *                   false, they will be ignored.
 *   topic - the subscription topic to use (wildcards are allowed).
 *   qos - the qos to use for the subscription.
 *   host - the broker to connect to.
 *   port - the network port the broker is listening on.
 *   client_id - the client id to use, or NULL if a random client id should be
 *               generated.
 *   keepalive - the MQTT keepalive value.
 *   clean_session - the MQTT clean session flag.
 *   username - the username string, or NULL for no username authentication.
 *   password - the password string, or NULL for an empty password.
 *   will - a libmyserver_will struct containing will information, or NULL for
 *          no will.
 *   tls - a libmyserver_tls struct containing TLS related parameters, or NULL
 *         for no use of TLS.
 *
 *
 * Returns:
 *   MS_ERR_SUCCESS - on success
 *   >0 - on error.
 */
libms_EXPORT int myserver_subscribe_simple(
		struct myserver_message **messages,
		int msg_count,
		bool want_retained,
		const char *topic,
		int qos,
		const char *host,
		int port,
		const char *client_id,
		int keepalive,
		bool clean_session,
		const char *username,
		const char *password,
		const struct libmyserver_will *will,
		const struct libmyserver_tls *tls);


/*
 * Function: myserver_subscribe_callback
 *
 * Helper function to make subscribing to a topic and processing some messages
 * very straightforward.
 *
 * This connects to a broker, subscribes to a topic, then passes received
 * messages to a user provided callback. If the callback returns a 1, it then
 * disconnects cleanly and returns.
 *
 * Parameters:
 *   callback - a callback function in the following form:
 *              int callback(struct myserver *ms, void *obj, const struct myserver_message *message)
 *              Note that this is the same as the normal on_message callback,
 *              except that it returns an int.
 *   userdata - user provided pointer that will be passed to the callback.
 *   topic - the subscription topic to use (wildcards are allowed).
 *   qos - the qos to use for the subscription.
 *   host - the broker to connect to.
 *   port - the network port the broker is listening on.
 *   client_id - the client id to use, or NULL if a random client id should be
 *               generated.
 *   keepalive - the MQTT keepalive value.
 *   clean_session - the MQTT clean session flag.
 *   username - the username string, or NULL for no username authentication.
 *   password - the password string, or NULL for an empty password.
 *   will - a libmyserver_will struct containing will information, or NULL for
 *          no will.
 *   tls - a libmyserver_tls struct containing TLS related parameters, or NULL
 *         for no use of TLS.
 *
 *
 * Returns:
 *   MS_ERR_SUCCESS - on success
 *   >0 - on error.
 */
libms_EXPORT int myserver_subscribe_callback(
		int (*callback)(struct myserver *, void *, const struct myserver_message *),
		void *userdata,
		const char *topic,
		int qos,
		const char *host,
		int port,
		const char *client_id,
		int keepalive,
		bool clean_session,
		const char *username,
		const char *password,
		const struct libmyserver_will *will,
		const struct libmyserver_tls *tls);


/*
 * Function: myserver_validate_utf8
 *
 * Helper function to validate whether a UTF-8 string is valid, according to
 * the UTF-8 spec and the MQTT additions.
 *
 * Parameters:
 *   str - a string to check
 *   len - the length of the string in bytes
 *
 * Returns:
 *   MS_ERR_SUCCESS -        on success
 *   MS_ERR_INVAL -          if str is NULL or len<0 or len>65536
 *   MS_ERR_MALFORMED_UTF8 - if str is not valid UTF-8
 */
libms_EXPORT int myserver_validate_utf8(const char *str, int len);


/* Function: myserver_userdata
 *
 * Retrieve the "userdata" variable for a myserver client.
 *
 * Parameters:
 * 	ms - a valid myserver instance.
 *
 * Returns:
 *	A pointer to the userdata member variable.
 */
libms_EXPORT void *myserver_userdata(struct myserver *ms);

#ifdef __cplusplus
}
#endif

#endif
