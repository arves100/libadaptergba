#include <gba_console.h>

#include <stdio.h>

#include "../libadaptergba/source/adapter.h"
#include "../libadaptergba/source/packet.h"

#include "test.h"

static struct adapter g_ad;
static int sts = 0;

static void printret(int ret)
{
    if (ret > 0x80)
    {
        switch (ret - 0x80)
        {
            case PACKET_CHECK_INVALID_MAGIC:
                printf("Invalid check magic\n");
                break;
            case PACKET_CHECK_INVALID_CHECKSUM:
                printf("Invalid check checksum\n");
                break;
            case PACKET_CHECK_INVALID_ACK:
                printf("Invalid check ack\n");
                break;
        }

        return;
    }
    switch (ret)
    {
        case ADAPTER_OK:
            printf("Ok\n");
            break;
        case ADAPTER_WAITING:
            printf("Waiting...\n");
            break;
        case ADAPTER_PROCESSING:
            printf("Processing...\n");
            break;
        case ADAPTER_INVALID_MAGIC:
            printf("Invalid magic!\n");
            break;
        case ADAPTER_INVALID_PACKET:
            printf("Invalid packet!\n");
            break;
        case ADAPTER_INVALID_START:
            printf("Invalid start!\n");
            break;
        default:
            printf("Unknown %x\n", ret);
            break;
    }
}

int do_test_lowlevel()
{
    int ret;

    if (sts == 0) {
        adapter_init0(&g_ad);
        sts = 1;
    }

    ret = adapter_loop(&g_ad);

    if (ret != ADAPTER_PROCESSING && ret != ADAPTER_WAITING && ret != ADAPTER_OK) {
        printret(ret);
        return 1;
    }
    else if (ret == ADAPTER_OK)
    {
        sts++;

        switch (sts) {
            case 2:
            adapter_init1(&g_ad);
            break;

            default:
                return 1;
        }
    }


    return 0;
}