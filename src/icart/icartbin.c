/* ======================================================================== */
/*  Routines for writing a .BIN and .CFG from an icartrom_t.                */
/* ------------------------------------------------------------------------ */
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
/* ------------------------------------------------------------------------ */
/*                 Copyright (c) 1998-2001, Joseph Zbiciak                  */
/* ======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "icart/icartrom.h"
#include "icart/icartbin.h"

/* ======================================================================== */
/*  These are errors that can be reported by the Intellicart routines.      */
/* ======================================================================== */
static char *errors[] =
{
    "No Error",
    "Bad Arguments",
    "Bad ROM Header",
    "CRC-16 Error in ROM Segments",
    "Bad ROM Segment Address Range",
    "Bad ROM Fine-Address Range",   
    "CRC-16 Error in Enable Tables",
    "Unknown Error"
};


/* ======================================================================== */
/*  ICB_SHOW_RANGES                                                         */
/*  Shows a list of ranges of addresses represented by a bit-vector.        */
/* ======================================================================== */
void icb_show_ranges(uint_32 *bv)
{
    int lo, hi, i;

    /* -------------------------------------------------------------------- */
    /*  Iterate over all 256 256-decle pages, with a little slop at both    */
    /*  ends of the spectrum.  Look for spans of set bits.                  */
    /* -------------------------------------------------------------------- */
    for (i = 0, lo = hi = -1; i <= 256; i++)
    {
        int idx, shf;

        idx = i >> 5;
        shf = i & 31;
        if (i < 256 && (1 & (bv[idx] >> shf)))
        {
            hi = i;
            if (lo == -1) { lo = i; }
        } else
        {
            if (lo != -1)
            {
                printf("    $%.4X - $%.4X (%d pages)\n",
                        lo << 8, (hi << 8) + 0xFF, (hi - lo + 1));
            }
            hi = lo = -1;
        }
    }
}

/* ======================================================================== */
/*  ICB_WRITE_MAPPINGS                                                      */
/*  Writes the [mappings] section of a .CFG file, based on the icartrom.    */
/* ======================================================================== */
int icb_write_mappings(FILE *fb, FILE *fc, icartrom_t *icart, int ofs)
{
    int lo, hi, i, j;

    /* -------------------------------------------------------------------- */
    /*  Make sure at least one page is both 'preload' and 'readable'.       */
    /*  If there are none, then don't output a [mapping] section.           */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < 8; i++)
        if (icart->preload[i] & icart->readable[i])
            break;
    if (i == 8)
        return 0;

    fprintf(fc, "[mapping]\r\n");

    /* -------------------------------------------------------------------- */
    /*  Iterate over all 256 256-decle pages, with a little slop at both    */
    /*  ends of the spectrum.  Look for spans of pages that are both        */
    /*  readable and preloaded.                                             */
    /* -------------------------------------------------------------------- */
    for (i = 0, lo = hi = -1; i <= 256; i++)
    {
        int idx, shf;

        idx = i >> 5;
        shf = i & 31;
        if (i < 256 && 
            (1 & ((icart->preload [idx] & 
                   icart->readable[idx]) >> shf)))
        {
            hi = i;
            if (lo == -1) { lo = i; }
        } else
        {
            if (lo != -1)
            {
                lo <<= 8;
                hi = (hi << 8) + 0x100;
                fprintf(fc, "$%.4X - $%.4X = $%.4X\r\n", 
                        ofs, ofs + hi - lo - 1, lo);
                for (j = lo; j < hi; j++)
                {
                    fputc(icart->image[j] >> 8,   fb);
                    fputc(icart->image[j] & 0xFF, fb);
                }
                ofs += hi - lo;
            }
            hi = lo = -1;
        }
    }

    return ofs;
}

/* ======================================================================== */
/*  ICB_WRITE_PRELOADS                                                      */
/*  Writes the [preload] section of a .CFG, which addresses ranges of       */
/*  address that are preloaded, but not readable.                           */
/* ======================================================================== */
int icb_write_preloads(FILE *fb, FILE *fc, icartrom_t *icart, int ofs)
{
    int lo, hi, i, j;

    /* -------------------------------------------------------------------- */
    /*  Make sure at least one page is both 'preload' and 'not readable'.   */
    /*  If there are none, then don't output a [preload] section.           */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < 8; i++)
        if (icart->preload[i] & ~icart->readable[i])
            break;
    if (i == 8)
        return 0;

    fprintf(fc, "[preload]\r\n");

    /* -------------------------------------------------------------------- */
    /*  Iterate over all 256 256-decle pages, with a little slop at both    */
    /*  ends of the spectrum.  Look for spans of pages that are both        */
    /*  preloaded and not-readable.                                         */
    /* -------------------------------------------------------------------- */
    for (i = 0, lo = hi = -1; i <= 256; i++)
    {
        int idx, shf;

        idx = i >> 5;
        shf = i & 31;
        if (i < 256 && 
            (1 & (( icart->preload [idx] & 
                   ~icart->readable[idx]) >> shf)))
        {
            hi = i;
            if (lo == -1) { lo = i; }
        } else
        {
            if (lo != -1)
            {
                lo <<= 8;
                hi = (hi << 8) + 0x100;
                fprintf(fc, "$%.4X - $%.4X = $%.4X\r\n", 
                        ofs, ofs + hi - lo - 1, lo);
                for (j = lo; j < hi; j++)
                {
                    fputc(icart->image[j] >> 8,   fb);
                    fputc(icart->image[j] & 0xFF, fb);
                }
                ofs += hi - lo;
            }
            hi = lo = -1;
        }
    }
    return ofs;
}

/* ======================================================================== */
/*  ICB_WRITE_BANKSW                                                        */
/*  Writes the [bankswitch] section.  These are sections marked for         */
/*  Intellicart-style bankswitching.                                        */
/* ======================================================================== */
void icb_write_banksw(FILE *fc, icartrom_t *icart)
{
    int lo, hi, i;

    /* -------------------------------------------------------------------- */
    /*  Make sure at least one page is 'bankswitched'.                      */
    /*  If there are none, then don't output a [bankswitch] section.        */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < 8; i++)
        if (icart->dobanksw[i])
            break;
    if (i == 8)
        return;

    /* -------------------------------------------------------------------- */
    /*  Iterate over all 256 256-decle pages, with a little slop at both    */
    /*  ends of the spectrum.  Look for spans of pages that are banksw.     */
    /* -------------------------------------------------------------------- */
    fprintf(fc, "[bankswitch]\r\n");
    for (i = 0, lo = hi = -1; i <= 256; i++)
    {
        int idx, shf;

        idx = i >> 5;
        shf = i & 31;
        if (i < 256 && (1 & (icart->dobanksw[idx] >> shf)))
        {
            hi = i;
            if (lo == -1) { lo = i; }
        } else
        {
            if (lo != -1)
            {
                fprintf(fc, "$%.4X - $%.4X\r\n",
                        lo << 8, (hi << 8) + 0xFF);
            }
            hi = lo = -1;
        }
    }
}

/* ======================================================================== */
/*  ICB_WRITE_MEMATTR                                                       */
/*  Writes the [memattr] section.  These are sections marked as RAM.        */
/* ======================================================================== */
void icb_write_memattr(FILE *fc, icartrom_t *icart)
{
    int lo, hi, i;

    /* -------------------------------------------------------------------- */
    /*  Make sure at least one page is 'writable.'                          */
    /*  If there are none, then don't output a [memattr] section.           */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < 8; i++)
        if (icart->writable[i])
            break;
    if (i == 8)
        return;

    fprintf(fc, "[memattr]\r\n");
    /* -------------------------------------------------------------------- */
    /*  Iterate over all 256 256-decle pages, with a little slop at both    */
    /*  ends of the spectrum.  Look for spans of pages that are writable    */
    /*  but not narrow.  These are RAM 16 spans.                            */
    /* -------------------------------------------------------------------- */
    for (i = 0, lo = hi = -1; i <= 256; i++)
    {
        int idx, shf;

        idx = i >> 5;
        shf = i & 31;
        if (i < 256 && 
            (1 & (( icart->writable [idx] & 
                   ~icart->narrow   [idx]) >> shf)))
        {
            hi = i;
            if (lo == -1) { lo = i; }
        } else
        {
            if (lo != -1)
            {
                fprintf(fc, "$%.4X - $%.4X = RAM 16\r\n",
                        lo << 8, (hi << 8) + 0xFF);
            }
            hi = lo = -1;
        }
    }
    /* -------------------------------------------------------------------- */
    /*  Iterate over all 256 256-decle pages, with a little slop at both    */
    /*  ends of the spectrum.  Look for spans of pages that are writable    */
    /*  and also narrow.  These are RAM 8 spans.                            */
    /* -------------------------------------------------------------------- */
    for (i = 0, lo = hi = -1; i <= 256; i++)
    {
        int idx, shf;

        idx = i >> 5;
        shf = i & 31;
        if (i < 256 && 
            (1 & (( icart->writable [idx] & 
                    icart->narrow   [idx]) >> shf)))
        {
            hi = i;
            if (lo == -1) { lo = i; }
        } else
        {
            if (lo != -1)
            {
                fprintf(fc, "$%.4X - $%.4X = RAM 8\r\n",
                        lo << 8, (hi << 8) + 0xFF);
            }
            hi = lo = -1;
        }
    }
}

/* ======================================================================== */
/*  ICB_WRITE_BINCFG                                                        */
/*  Write out an entire BIN+CFG.                                            */
/* ======================================================================== */
int icb_write_bincfg(FILE *fb, FILE *fc, icartrom_t *icart, int ofs)
{
    ofs = icb_write_mappings(fb, fc, icart, ofs);
    ofs = icb_write_preloads(fb, fc, icart, ofs);
    icb_write_memattr(fc, icart);
    icb_write_banksw (fc, icart);

    return ofs;
}

/* ======================================================================== */
/*  ICB_SHOW_SUMMARY                                                        */
/*  Show a bunch of human-readable info about an icartrom.                  */
/* ======================================================================== */
void icb_show_summary(icartrom_t *icart)
{
    printf("Preloaded memory ranges:\n");
    icb_show_ranges(icart->preload);

    printf("Readable memory ranges:\n");
    icb_show_ranges(icart->readable);

    printf("Writeable memory ranges:\n");
    icb_show_ranges(icart->writable);

    printf("Narrow (8-bit wide) memory ranges:\n");
    icb_show_ranges(icart->narrow);

    printf("Bank-switched memory ranges:\n");
    icb_show_ranges(icart->dobanksw);
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
/*                 Copyright (c) 1998-2001, Joseph Zbiciak                  */
/* ======================================================================== */

