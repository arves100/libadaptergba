#ifndef LIBADAPTERGBA_EXCHANGE_H
#define LIBADAPTERGBA_EXCHANGE_H

#include "packet.h"

enum
{
    /* The communication is done in 8-bit mode */
    MODE_8BIT,

    /* The communication is done in 32-bit mode */
    MODE_32BIT,
};

/*
 * Structure that holds the data status that will be exchanges
 */
struct exchange
{
    /*
     * The complete packet buffer
     * The extra 3 bytes is to keep alignment with 4 bytes in 32-bit mode
     */
    u8 buffer[PACKET_DATA_MAX_LEN + PACKET_HEADER_LENGTH + PACKET_FOOTER_LENGTH + 3];

    /* current position of the buffer */
    u8 pos;

    /* communication mode */
    u8 mode;
};

/* Initialize and starts normal mode data exchange
 * if mode32 if 1 or higher the game will setup 32-bit mode
 */
void exchange_init(struct exchange* ex, u8 mode);

/* Async data exchange
 * The function will return 0 when there is no data available/is transferring
 * The function will return 1 when the data is being read
 * The data will be stored inside its relative buffer at the current position
 */
u8 exchange_step(struct exchange* ex);

/* Starts a communication with the adapter */
void exchange_start(struct exchange* ex);

#endif /* LIBADAPTERGBA_EXCHANGE_H */
