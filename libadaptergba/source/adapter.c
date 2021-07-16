#include <gba_console.h>

#include <string.h>
#include <stdio.h>

#include "adapter.h"
#include "commands.h"

void adapter_init0(struct adapter* ad)
{
    ad->command_ack = 0;
    ad->status = STATUS_START;
    ad->ex.pos = 0;
    memset(ad->ex.buffer, 0, sizeof(ad->ex.buffer));

    exchange_init(&ad->ex, MODE_8BIT);

    ad->ex.buffer[0] = 0x4B;
    exchange_start(&ad->ex);
}

void adapter_init1(struct adapter* ad)
{
    ad->status = STATUS_HELLO;
    ad->ex.pos = 0;
    ad->command_ack = packet_create(ad->ex.buffer, 0x10, COMMAND_10_DATA, 8, 1);

    exchange_start(&ad->ex);
}

void adapter_init32(struct adapter* ad)
{
    exchange_init(&ad->ex, MODE_32BIT);
    adapter_init1(ad);
}

u8 adapter_loop(struct adapter* ad)
{
    u16 expected_len;

    if (exchange_step(&ad->ex) == 0)
        return ADAPTER_WAITING;

    if (ad->status == STATUS_START)
    {
        printf("DEBUG: start buf: %x\n", ad->ex.buffer[0]);

        if (ad->ex.buffer[0] == 0xD2)
        {
            adapter_init1(ad);
            return ADAPTER_OK;
        }
        else
            adapter_init0(ad);

        return ADAPTER_INVALID_START;
    }

    if (ad->ex.mode == MODE_32BIT)
        ad->ex.pos += 4;
    else
        ad->ex.pos++;

    expected_len = PACKET_HEADER_LENGTH + PACKET_FOOTER_LENGTH;
    if (ad->ex.pos >= expected_len)
    {
        expected_len += ad->ex.buffer[PACKET_DATA_LENGTH];
        if (ad->ex.pos >= expected_len)
        {
            u8 pkc = packet_check(ad->ex.buffer, ad->command_ack);
            ad->ex.pos = 0;

            if (pkc != 0)
                return pkc + 0x80; /* invalid packet */

            // Bake the packet
            if (ad->status == STATUS_HELLO && ad->command_ack)
            {
                /* We are now ready to receive the adapter data */
                ad->command_ack = 0;
                packet_create(ad->ex.buffer, 0x10, NULL, 8, 0);
            }
            else if (ad->status == STATUS_HELLO && !ad->command_ack)
            {
                if (memcmp(ad->ex.buffer + PACKET_HEADER_LENGTH, COMMAND_10_DATA, 8) != 0)
                    return ADAPTER_INVALID_MAGIC; /* Invalid magic */

                ad->status = STATUS_IDLE; /* yay! our adapter exists */
            }

            // Start the exchange
            if (ad->status != STATUS_WORK)
                exchange_start(&ad->ex);

            return ADAPTER_OK;
        }
    }

    return ADAPTER_PROCESSING;
}

void adapter_exchange(struct adapter* ad)
{
    ad->status = STATUS_WORK;
    ad->ex.pos = 0;
}
