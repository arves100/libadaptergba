#ifndef LIBADAPTERGBA_ADAPTER_H
#define LIBADAPTERGBA_ADAPTER_H

#include "exchange.h"

enum
{
    STATUS_START,
    STATUS_HELLO,
    STATUS_IDLE,
    STATUS_WORK,
};

struct adapter
{
    struct exchange ex;

    u8 status;

    /* this is used for checking the ack last byte when receiving data
     * from the adapter. This also acts as a check to know when the game
     * is sending or receiving
     *
     * If it's 0 then the game is receiving */
    u8 command_ack;
};

enum AdapterErrorCodes
{
    ADAPTER_OK,
    ADAPTER_WAITING,
    ADAPTER_PROCESSING,
    ADAPTER_INVALID_MAGIC,
    ADAPTER_INVALID_PACKET,
    ADAPTER_INVALID_START,
};

/*
 * First stage of adapter connection, usually run once to generate a consistent flow
 */
void adapter_init0(struct adapter* ad);

/*
 * Second stage of adapter connection, this sends the init command (0x10) and process the response
 */
void adapter_init1(struct adapter* ad);

/*
 * Switch to 32-bit mode.
 * NOTE: A normal adapter initialization must have been executed before switching to 32-bit mode
 */
void adapter_init32(struct adapter* ad);


/*
 * Step one handling of the adapter, done in async way.
 * Returns error codes from AdapterErrorCodes.
 */
u8 adapter_loop(struct adapter* ad);

/*
 * Starts an exchange with the adapter
 */
void adapter_exchange(struct adapter* ad);

#endif /* LIBADAPTERGBA_ADAPTER_H */
