#ifndef __rmvm_syscalls_h__
#define __rmvm_syscalls_h__

#include <ddcKeyboard.h>

#include "./cpu.h"

// print int
void sc0(long f[10])
{
	ddPrint_int(f[1]);
}
// print char
void sc1(long f[10])
{
	ddPrint_char(f[1]);
}
// getch
void sc2(long f[10])
{
	f[1] = ddKey_getch_noesc();
}

const void (*cpu_syscalls[])(long f[10]) = {
	sc0,
	sc1,
	sc2,
};

#endif
