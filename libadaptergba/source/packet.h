#ifndef LIBADAPTERGBA_PACKET_H
#define LIBADAPTERGBA_PACKET_H

enum PacketInfo
{
    /* Header */

    PACKET_MAGIC_1,
    PACKET_MAGIC_2,
    PACKET_COMMAND_ID,
    PACKET_UNKNOWN_1,
    PACKET_UNKNOWN_2,
    PACKET_DATA_LENGTH,

    /* Complete length of the first bytes */
    PACKET_HEADER_LENGTH,

    /* Footer */

    PACKET_CHECKSUM_HIGH = 0,
    PACKET_CHECKSUM_LOW,
    PACKET_DEVICE_ID,
    PACKET_COMMAND_ACK,

    /* Complete length of the last bytes */
    PACKET_FOOTER_LENGTH,

    /* Maximum data length of a packet */
    PACKET_DATA_MAX_LEN = 0xFF,
};

enum PacketCheckError
{
    PACKET_CHECK_OK,
    PACKET_CHECK_INVALID_MAGIC,
    PACKET_CHECK_INVALID_CHECKSUM,
    PACKET_CHECK_INVALID_ACK,
};

/* Creates a new packet
 * Send is 1 (or greater) if the GBA is sending
 * Send is 0 if the GBA is receiving
 * Returns the ack command
 */
u8 packet_create(u8* buffer, u8 command, const u8* data, u8 len, u8 send);

/* Check if a received packet is valid
 * Returns 0 if everything is ok
 * Returns 1 if the magic is invalid
 * Returns 2 if the checksum is invalid
 * Returns 3 if the ack command id is invalid (sender only)
 */
u8 packet_check(u8* buffer, u8 command_ack);

#endif /* LIBADAPTERGBA_PACKET_H */
