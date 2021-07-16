/* This file handles normal mode data exchange
 * TODO: Add IRQ support
 */
#include <gba_console.h>
#include <gba_sio.h>

#include "exchange.h"

void exchange_init(struct exchange* ex, u8 mode)
{
    ex->mode = mode;
    ex->pos = 0;

    REG_RCNT = 0;
    if (ex->mode == MODE_32BIT)
        REG_SIOCNT = SIO_SO_HIGH | SIO_32BIT;
    else
        REG_SIOCNT = SIO_SO_HIGH | SIO_8BIT;
}

void exchange_start(struct exchange* ex)
{
    // Setup 32-bit mode buffer
    if (ex->mode == MODE_32BIT) {
        REG_SIODATA32 = *(u32*)(ex->buffer + ex->pos); // Setup data buffer
        REG_SIOCNT = SIO_SO_HIGH | SIO_32BIT | SIO_START; // Start
    } else {
        REG_SIODATA8 = ex->buffer[ex->pos]; // Setup data buffer
        REG_SIOCNT = SIO_SO_HIGH | SIO_8BIT | SIO_START; // Start
    }
}

u8 exchange_step(struct exchange* ex)
{
    if (REG_SIOCNT & SIO_START)
        return 0;

    if (ex->mode == MODE_32BIT)
        *(u32*)(ex->buffer + ex->pos) = REG_SIODATA32;
    else
        ex->buffer[ex->pos] = REG_SIODATA8;

    return 1;
}
