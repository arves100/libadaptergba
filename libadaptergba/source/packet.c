#include <gba_console.h>

#include <stdio.h>

#include "packet.h"

#define MAGIC1 0x99
#define MAGIC2 0x66

u8 packet_check(u8* buffer, u8 command_ack)
{
    u8 i = 0;
    u16 checksum = 0;

    printf("DEBUG: magic1 is %x, magic2 is %x\n", buffer[PACKET_MAGIC_1], buffer[PACKET_MAGIC_2]);

    if (buffer[PACKET_MAGIC_1] != MAGIC1 || buffer[PACKET_MAGIC_2] != MAGIC2)
        return PACKET_CHECK_INVALID_MAGIC; /* Invalid magic */

    for (; i < buffer[PACKET_DATA_LENGTH]; i++)
        checksum += buffer[PACKET_HEADER_LENGTH + i];

    printf("DEBUG: checksum is %x (hi %x lo %x), len %x\n", checksum, buffer[i + PACKET_CHECKSUM_HIGH], buffer[i + PACKET_CHECKSUM_LOW], buffer[PACKET_DATA_LENGTH]);

    if ((checksum & 0xFF) != buffer[i + PACKET_CHECKSUM_LOW] || (checksum >> 8) != buffer[i + PACKET_CHECKSUM_HIGH])
        return PACKET_CHECK_INVALID_CHECKSUM; /* Invalid checksum */

    if (command_ack > 0 && (command_ack ^ 0x80) != buffer[i + PACKET_COMMAND_ACK])
        return PACKET_CHECK_INVALID_ACK; /* Invalid command ack */

    return PACKET_CHECK_OK; /* Everything ok */
}

u8 packet_create(u8* buffer, u8 command, const u8* data, u8 len, u8 send)
{
    u8 i = 0;
    u16 checksum = 0;

    buffer[PACKET_MAGIC_1] = MAGIC1;
    buffer[PACKET_MAGIC_2] = MAGIC2;
    buffer[PACKET_UNKNOWN_1] = 0;
    buffer[PACKET_UNKNOWN_2] = 0;
    buffer[PACKET_COMMAND_ID] = command;
    buffer[PACKET_DATA_LENGTH] = len;

    for (; i < len; i++)
    {
        if (data)
        {
            buffer[PACKET_HEADER_LENGTH + i] = data[i];
            checksum += data[i];
        }
        else
        {
            buffer[PACKET_HEADER_LENGTH + i] = 0x4B;
        }
    }

    buffer[i + PACKET_CHECKSUM_HIGH] = checksum >> 8;
    buffer[i + PACKET_CHECKSUM_LOW] = checksum & 0xFF;
    buffer[i + PACKET_DEVICE_ID] = 0x81; /* GameBoy Advance (ID 1) */

    if (send)
    {
        buffer[i + PACKET_COMMAND_ACK] = 0; /* sending */
        return command;
    }

    buffer[i + PACKET_COMMAND_ACK] = buffer[PACKET_COMMAND_ID] ^ 0x80; /* receiving */
    return 0;
}
