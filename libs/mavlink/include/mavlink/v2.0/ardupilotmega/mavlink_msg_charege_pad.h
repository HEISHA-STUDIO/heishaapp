#pragma once
// MESSAGE CHAREGE_PAD PACKING

#define MAVLINK_MSG_ID_CHAREGE_PAD 227

MAVPACKED(
typedef struct __mavlink_charege_pad_t {
 uint8_t charge; /*<  CHAREGEPAD control*/
}) mavlink_charege_pad_t;

#define MAVLINK_MSG_ID_CHAREGE_PAD_LEN 1
#define MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN 1
#define MAVLINK_MSG_ID_227_LEN 1
#define MAVLINK_MSG_ID_227_MIN_LEN 1

#define MAVLINK_MSG_ID_CHAREGE_PAD_CRC 13
#define MAVLINK_MSG_ID_227_CRC 13



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CHAREGE_PAD { \
    227, \
    "CHAREGE_PAD", \
    1, \
    {  { "charge", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_charege_pad_t, charge) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CHAREGE_PAD { \
    "CHAREGE_PAD", \
    1, \
    {  { "charge", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_charege_pad_t, charge) }, \
         } \
}
#endif

/**
 * @brief Pack a charege_pad message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param charge  CHAREGEPAD control
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_charege_pad_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t charge)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CHAREGE_PAD_LEN];
    _mav_put_uint8_t(buf, 0, charge);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CHAREGE_PAD_LEN);
#else
    mavlink_charege_pad_t packet;
    packet.charge = charge;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CHAREGE_PAD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CHAREGE_PAD;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_CRC);
}

/**
 * @brief Pack a charege_pad message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param charge  CHAREGEPAD control
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_charege_pad_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t charge)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CHAREGE_PAD_LEN];
    _mav_put_uint8_t(buf, 0, charge);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CHAREGE_PAD_LEN);
#else
    mavlink_charege_pad_t packet;
    packet.charge = charge;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CHAREGE_PAD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CHAREGE_PAD;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_CRC);
}

/**
 * @brief Encode a charege_pad struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param charege_pad C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_charege_pad_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_charege_pad_t* charege_pad)
{
    return mavlink_msg_charege_pad_pack(system_id, component_id, msg, charege_pad->charge);
}

/**
 * @brief Encode a charege_pad struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param charege_pad C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_charege_pad_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_charege_pad_t* charege_pad)
{
    return mavlink_msg_charege_pad_pack_chan(system_id, component_id, chan, msg, charege_pad->charge);
}

/**
 * @brief Send a charege_pad message
 * @param chan MAVLink channel to send the message
 *
 * @param charge  CHAREGEPAD control
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_charege_pad_send(mavlink_channel_t chan, uint8_t charge)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CHAREGE_PAD_LEN];
    _mav_put_uint8_t(buf, 0, charge);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHAREGE_PAD, buf, MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_CRC);
#else
    mavlink_charege_pad_t packet;
    packet.charge = charge;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHAREGE_PAD, (const char *)&packet, MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_CRC);
#endif
}

/**
 * @brief Send a charege_pad message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_charege_pad_send_struct(mavlink_channel_t chan, const mavlink_charege_pad_t* charege_pad)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_charege_pad_send(chan, charege_pad->charge);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHAREGE_PAD, (const char *)charege_pad, MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_CRC);
#endif
}

#if MAVLINK_MSG_ID_CHAREGE_PAD_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_charege_pad_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t charge)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint8_t(buf, 0, charge);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHAREGE_PAD, buf, MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_CRC);
#else
    mavlink_charege_pad_t *packet = (mavlink_charege_pad_t *)msgbuf;
    packet->charge = charge;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CHAREGE_PAD, (const char *)packet, MAVLINK_MSG_ID_CHAREGE_PAD_MIN_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_LEN, MAVLINK_MSG_ID_CHAREGE_PAD_CRC);
#endif
}
#endif

#endif

// MESSAGE CHAREGE_PAD UNPACKING


/**
 * @brief Get field charge from charege_pad message
 *
 * @return  CHAREGEPAD control
 */
static inline uint8_t mavlink_msg_charege_pad_get_charge(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a charege_pad message into a struct
 *
 * @param msg The message to decode
 * @param charege_pad C-struct to decode the message contents into
 */
static inline void mavlink_msg_charege_pad_decode(const mavlink_message_t* msg, mavlink_charege_pad_t* charege_pad)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    charege_pad->charge = mavlink_msg_charege_pad_get_charge(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CHAREGE_PAD_LEN? msg->len : MAVLINK_MSG_ID_CHAREGE_PAD_LEN;
        memset(charege_pad, 0, MAVLINK_MSG_ID_CHAREGE_PAD_LEN);
    memcpy(charege_pad, _MAV_PAYLOAD(msg), len);
#endif
}
