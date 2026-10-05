/*
 * ============================================================================
 *  Title:    Platform Portability "Library"
 *  Author:   J. Zbiciak, T. Lindner
 *  $Id: plat_lib.c,v 1.2 2002/04/17 18:32:53 im14u2c Exp $
 * ============================================================================
 *  This module fills in missing features on various platforms.
 * ============================================================================
 *  GETTIMEOFDAY     -- Return current time in seconds/microseconds.
 *  STRDUP           -- Copy a string into freshly malloc'd storage.
 *  SNPRINTF         -- Like sprintf(), only with bounds checking.
 *  PLAT_DELAY       -- Sleep w/ millisecond precision.
 * ============================================================================
 */

static const char rcs_id[]="$Id: plat_lib.c,v 1.2 2002/04/17 18:32:53 im14u2c Exp $";
 
#include "../config.h"

/* ======================================================================== */
/*  GENERIC PORTABLE VERSIONS...                                            */
/* ======================================================================== */

#if 0
#ifdef NO_GETTIMEOFDAY

static clock_t last_clock = -1;
static struct timeval last_tv = { 0, 0 };

/* ------------------------------------------------------------------------ */
/*  GETTIMEOFDAY     -- Just call clock and convert to seconds/microsec.    */
/*                                                                          */
/*  Note:  This code attempts to detect overflows in the clock() return     */
/*  values, but this only works if there is at most one overflow between    */
/*  calls to gettimeofday().                                                */
/* ------------------------------------------------------------------------ */
void gettimeofday(struct timeval *tp, void *unused)
{
    clock_t now;
    unsigned long diff;
    double usec;

    (void)unused;

    now = clock();

    if (now < last_clock)
    {
        diff = now - last_clock + INT_MAX;
    } else
    {
        diff = now - last_clock;
    }

    last_clock = now;

    last_tv.tv_sec += diff / CLOCKS_PER_SEC;

    usec = (double)(diff % CLOCKS_PER_SEC)*1000000.0 / (double)CLOCKS_PER_SEC;

    last_tv.tv_usec += (long)usec;

    while (last_tv.tv_usec > 1000000)
    {
        last_tv.tv_sec++;
        last_tv.tv_usec -= 1000000;
    }

    *tp = last_tv;
}
   
#endif
#endif


/* ------------------------------------------------------------------------ */
/*  STRDUP           -- Copy a string into freshly malloc'd storage.        */
/*                                                                          */
/*  Unfortunately, strdup() is not specified by ANSI.  *sigh*               */
/* ------------------------------------------------------------------------ */
#ifdef NO_STRDUP

char * strdup(const char *s)
{
    int len = strlen(s) + 1;
    char *new_str = malloc(len);

    if (new_str) strcpy(new_str, s);

    return new_str;
}

#endif /* NO_STRDUP */

/* ------------------------------------------------------------------------ */
/*  SNPRINTF         -- Like sprintf(), only with bounds checking.          */
/* ------------------------------------------------------------------------ */
/*  WARNING:  THIS COULD CAUSE BUFFER OVERFLOW PROBLEMS AND IS MERELY       */
/*            A SHIM WHICH IS BEING USED TO GET jzIntv TO COMPILE.          */
/* ------------------------------------------------------------------------ */
#ifdef NO_SNPRINTF
# include <stdarg.h>

/* ------------------------------------------------------------------------ */
/*  WARNING:  THIS COULD CAUSE BUFFER OVERFLOW PROBLEMS AND IS MERELY       */
/*            A SHIM WHICH IS BEING USED TO GET jzIntv TO COMPILE.          */
/* ------------------------------------------------------------------------ */
void snprintf(char * buf, int len, const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    (void)len;
}
#endif /* NO_SNPRINTF */

/* ------------------------------------------------------------------------ */
/*  PLAT_DELAY       -- Sleep w/ millisecond precision.                     */
/* ------------------------------------------------------------------------ */
#if defined(NO_SDL) && !defined(macintosh)
void plat_delay(unsigned delay)
{
    struct timeval now, soon;
    unsigned over;

    gettimeofday(&soon, NULL);

    soon.tv_usec += delay * 1000;
    over          = soon.tv_usec / 1000000;
    soon.tv_sec  += over;
    soon.tv_usec -= over * 1000000;
    
    /* -------------------------------------------------------------------- */
    /*  BAD BAD BAD: Sit in a busy loop until time expires.                 */
    /* -------------------------------------------------------------------- */
    do
    {
        gettimeofday(&now, NULL);
    } while (now.tv_sec < soon.tv_sec || 
             (now.tv_sec == soon.tv.sec && now.tv_usec < soon.tv_usec));
    
    return;
}
#endif /* NO_SDL */


/* ======================================================================== */
/*  Portable random number generator, from Knuth vol 2.                     */
/*  J. Zbiciak, 1998                                                        */
/*                                                                          */
/*  This code is provided without any waranty of fitness for any            */
/*  purpose.  Caveat emptor.  Your mileage may vary.  Void where            */
/*  prohibited or taxed by law.  Not part of this nutritious breakfast.     */
/* ======================================================================== */

static unsigned __rand_buf[128], __rand_ptr = 0;

/* ======================================================================== */
/*  RAND_JZ      -- Return a random integer in the range  [0, 2^32)         */
/* ======================================================================== */
uint_32 rand_jz(void)
{
    uint_32 p, p1, p2;

    /* -------------------------------------------------------------------- */
    /*  Lagged Fibonacci Sequence Random Number Generator.                  */
    /*                                                                      */
    /*  This random number generator comes from Knuth vol 2., 3rd Ed,       */
    /*  p27-29.  The algorithm should produce a sequence whose most         */
    /*  significant bits have a period of 2^31 * (2^127 - 1) and whose      */
    /*  least significant bits have a period of 2^127 - 1.  Not bad.        */
    /*  The lags of 30 and 127 come from Table 1 on p29.                    */
    /*                                                                      */
    /*  The final XOR that this function performs is my own invention.      */
    /*  XOR'ing with a constant should not negatively impact the random     */
    /*  sequence, but it may slightly obscure a poor initialization         */
    /*  sequence.                                                           */
    /* -------------------------------------------------------------------- */

    p  = __rand_ptr++ & 127;
    p1 = (p -  30) & 127;
    p2 = (p - 127) & 127;

    return 0x5A4A3A2A ^ (__rand_buf[p] = __rand_buf[p1] + __rand_buf[p2]);
}

/* ======================================================================== */
/*  DRAND_JZ     -- Return a random double in the range [0.0, 1.0).         */
/* ======================================================================== */
double drand_jz(void)
{
    return rand_jz() / (((double)(~0U)) + 1.0);
}

/* ======================================================================== */
/*  SRAND_JZ     -- Seed the random number generator, setting it to a       */
/*                  known initial state.                                    */
/* ======================================================================== */
void srand_jz(uint_32 seed)
{
    uint_32 s = seed ^ 0x2A3A4A5A;
    int i, j;

    /* -------------------------------------------------------------------- */
    /*  This initializer uses the user-provided seed to drive a linear-     */
    /*  feedback-shift-register (LFSR) random number generator to produce   */
    /*  the initial random number buffer for the lagged-Fibonacci           */
    /*  generator that rand_jz() uses.  The LFSR uses the equation          */
    /*  x = x^-29 + x^-31, which gives a maximal LFSR sequence of 2^32 - 1  */
    /*  values.                                                             */
    /*                                                                      */
    /*  The user-provided seed is salted with my favorite magic constant,   */
    /*  0x2A3A4A5A, to provide the initial LSFR setting.  If that comes     */
    /*  up zero, then the salt is removed, giving an initial LSFR value of  */
    /*  0x2A3A4A5A.                                                         */
    /*                                                                      */
    /*  The LFSR is run for 43 iterations between buffer writes.  43 is     */
    /*  relatively prime to 2^32 - 1, and so all 2^32 - 1 unique seeds      */
    /*  will produce unique LFSR sequences to be written to the lagged-     */
    /*  Fibonacci generator buffer.                                         */
    /*                                                                      */
    /*  The output of the LFSR is XORed with the initial seed for a         */
    /*  touch of randomness, but I doubt that it significantly impacts      */
    /*  its randomness.  LFSR's are already pretty good random number       */
    /*  generators.  :-)                                                    */
    /* -------------------------------------------------------------------- */

    if (!s) 
        s = 0x2A3A4A5A;

    for (i = 0; i < 127; i++)
    {
        for (j = 0; j <= 42; j++)
            s = (((s ^ (s >> 2)) >> 29) & 1) | (s << 1);

        __rand_buf[i] = seed ^ s;
    }
}
/* ======================================================================== */
/*  This program is free software; you can redistribute it and/or modify    */
/*  it under the terms of the GNU General Public License as published by    */
/*  the Free Software Foundation; either version 2 of the License, or       */
/*  (at your option) any later version.                                     */
/*                                                                          */
/*  This program is distributed in the hope that it will be useful,         */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       */
/*  General Public License for more details.                                */
/*                                                                          */
/*  You should have received a copy of the GNU General Public License       */
/*  along with this program; if not, write to the Free Software             */
/*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               */
/* ======================================================================== */
/*           Copyright (c) 1998-1999, Joseph Zbiciak, Tim Lindner           */
/* ======================================================================== */
