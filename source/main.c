
#include <gba_console.h>
#include <gba_video.h>
#include <gba_interrupt.h>
#include <gba_systemcalls.h>
#include <gba_input.h>
#include <stdio.h>
#include <stdlib.h>

#include "test.h"

typedef int(*test_cb)(void);
static int curtest_n = 0;
static test_cb curtest = NULL;

static void switchtest()
{
    switch (curtest_n)
    {
        case 0:
            printf("Starting lowlevel test\n");
            curtest = do_test_lowlevel;
            break;
        case 1:
            printf("Starting highlevel test\n");
            curtest = do_test_highlevel;
            break;
        default:
            printf("All tests completed!\n");
            exit(0);
            break; /* should never reach */
    }

    /*printf("debug break! Use gdb now\n");
    while (1)
    {
        u16 bre = 0;
        if (bre == 1)
            break;
    }*/
}
//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
{
//---------------------------------------------------------------------------------
	// the vblank interrupt must be enabled for VBlankIntrWait() to work
	// since the default dispatcher handles the bios flags no vblank handler
	// is required
	irqInit();
	irqEnable(IRQ_VBLANK);

	consoleDemoInit();

	// ansi escape sequence to set print co-ordinates
	// /x1b[line;columnH
	iprintf("test app\n");

	switchtest();

	while (1) {
        if (curtest())
        {
            curtest_n++;
            switchtest();
        }

		VBlankIntrWait();
	}
}


