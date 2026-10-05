/* ======================================================================== */
/*  Takes a BIN (and optional CFG) and generates a .ROM from it.            */
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
#include <ctype.h>
#include "config.h"
#include "icart/icartrom.h"
#include "icart/icartbin.h"

/* ======================================================================== */
/*  These are errors that can be reported by the Intellicart routines.      */
/* ======================================================================== */
char *errors[] =
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

icartrom_t the_icart;

typedef struct binseg_t
{
    uint_16     ofs;
    uint_16     addr;
    uint_16     len;
    uint_16     flags;
} binseg_t;

#define MAXSEG  (256)

binseg_t  seglist[MAXSEG];
binseg_t  defsegs[4] =
{
    {   0x0000, 0x5000, 0x2000, ICARTROM_READ|ICARTROM_PRELOAD   },
    {   0x2000, 0xD000, 0x1000, ICARTROM_READ|ICARTROM_PRELOAD   },
    {   0x3000, 0xF000, 0x1000, ICARTROM_READ|ICARTROM_PRELOAD   },
    {   0x0000, 0x0000, 0x0000, 0                                }
};

const char *secname[13] =
{
    "MACRO",
    "DISASM",
    "VARS",
    "KEYS",
    "NUMLOCK",
    "CAPSLOCK",
    "SCROLLLOCK",
    "VOICES",
    "JOYSTICK",

    "BANKSWITCH",
    "MEMATTR",
    "PRELOAD",
    "MAPPING" 
};

/* ======================================================================== */
/*  ADJ_DEFSEGS -- Adjust the default configuration for a file based on     */
/*                 its filesize.                                            */
/* ======================================================================== */
void adj_defsegs(int len)
{
    if (len >= 0x4000)
        return;

    if (len >= 0x3000 && len < 0x4000)
    {
        defsegs[2].len = 0x4000 - len;
        return;
    }

    if (len >= 0x2000 && len < 0x3000)
    {
        defsegs[1].len = 0x3000 - len;
        defsegs[2] = defsegs[3];
        return;
    }

    if (len >= 0x0000 && len < 0x2000)
    {
        defsegs[0].len = 0x2000 - len;
        defsegs[1] = defsegs[3];
        defsegs[2] = defsegs[3];
        return;
    }

    return;
}

/* ======================================================================== */
/*  PARSE_RANGE -- Parses the first part of the line where range is         */
/*                 expected.  Accepts buffer and line number.               */
/* ======================================================================== */
char *parse_range(int lineno, char *buf, uint_32 *lo, uint_32 *hi, int eqok)
{
    int tmp;
    int eq = eqok ? '=' : 0;
    char *s1, *s2;
    
    /* -------------------------------------------------------------------- */
    /*  Look for end of first address.                                      */
    /* -------------------------------------------------------------------- */
    s1 = buf + 1;
    while (*s1 && ((*s1 >= '0' && *s1 <= '9') ||
                   (*s1 >= 'A' && *s1 <= 'F')))    s1++;
    
    /* -------------------------------------------------------------------- */
    /*  Make sure first addr is terminated only by space or dash and that   */
    /*  there was even an address to begin with.                            */
    /* -------------------------------------------------------------------- */
    if ((*s1 != ' ' && *s1 != '-') || s1 == buf + 1)
    {
        if (!*s1)
        {
            fprintf(stderr, "ERROR [%3d]: Premature end of line "
                    "in address range\n>> '%s'\n",
                    lineno, buf);
        } else if (*s1 != ' ' && *s1 != '-')
        {
            fprintf(stderr, "ERROR [%3d]: Unexpected character "
                    "'%c' in address range\n>> '%s'\n",
                    lineno, *s1, buf);
        } else
        {
            fprintf(stderr, "ERROR [%3d]: Missing address "
                    "in address range\n>> '%s'\n",
                    lineno, buf);
        }
        exit(1);
    }



    /* -------------------------------------------------------------------- */
    /*  Scan out the first address.  We NUL-terminate it so as to ensure    */
    /*  sscanf() only sees the address.                                     */
    /* -------------------------------------------------------------------- */
    s2 = s1;
    tmp = *s1;
    *s1 = 0;

    if (sscanf(buf + 1, "%x", lo) != 1)
    {
        fprintf(stderr, "ERROR [%3d]: Could not interpret "
                "address\n>> '%s'\n", lineno, buf);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  Restore the original character where we had NUL-terminated.  Now    */
    /*  skip whitespace and look for the '-' separator.                     */
    /* -------------------------------------------------------------------- */
    *s1 = tmp;
    while (*s2 && isspace(*s2)) s2++;

    if (*s2 != '-')
    {
        fprintf(stderr, "ERROR [%3d]: Missing '-' separator\n"
                ">> '%s'\n", lineno, buf);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  Move past the '-' and look for the '$' that starts the second addr. */
    /* -------------------------------------------------------------------- */
    s2++;
    while (*s2 && isspace(*s2)) s2++;
                
    if (*s2 != '$')
    {
        fprintf(stderr, "ERROR [%3d]: Missing '$' on second "
                "first address\n>> '%s'\n", lineno, buf);
        exit(1);
    }
    s2++;
    
    /* -------------------------------------------------------------------- */
    /*  Now scope out the second address and see where it ends.             */
    /* -------------------------------------------------------------------- */
    s1 = s2; 
    while (*s2 && ((*s2 >= '0' && *s2 <= '9') ||
                   (*s2 >= 'A' && *s2 <= 'F')))    s2++;
    
    
    /* -------------------------------------------------------------------- */
    /*  Make sure it ended with whitespace or end-of-line, or if equals     */
    /*  are allowed, an equals sign.  Also make sure there was something    */
    /*  at all!                                                             */
    /* -------------------------------------------------------------------- */
    if ((*s2 != 0 && *s2 != ' ' && *s2 != eq) || s2 == s1)
    {
        if (*s2 != 0 && *s2 != ' ' && *s2 != eq)
        {
            fprintf(stderr, "ERROR [%3d]: Unexpected character "
                    "'%c'\n>> '%s'\n",
                    lineno, *s1, buf);
        } else
        {
            fprintf(stderr, "ERROR [%3d]: Missing second address\n>> '%s'\n",
                    lineno, buf);
        }
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  Scan out the second address.  We NUL-terminate it so as to ensure   */
    /*  sscanf() only sees the address.                                     */
    /* -------------------------------------------------------------------- */
    tmp = *s2;
    *s2 = 0;

    if (sscanf(s1, "%x", hi) != 1)
    {
        fprintf(stderr, "ERROR [%3d]: Could not interpret "
                "second address\n>> '%s'\n", lineno, buf);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  Restore the value under the NUL, and return our parse location.     */
    /* -------------------------------------------------------------------- */
    *s2 = tmp;

    return s2;
}

/* ======================================================================== */
/*  PARSE_CFG -- This parses a configuration file and returns a list of     */
/*               sections to load from a file.                              */
/*                                                                          */
/*  Sections of interest in a CFG:                                          */
/*                                                                          */
/*      [mapping]     PRELOAD | READ                                        */
/*      [preload]     PRELOAD                                               */
/*      [bankswitch]  READ | BANKSW                                         */
/*      [memattr]     READ                                                  */
/*      [memattr]     READ | WRITE          (if RAM 16 qualifier)           */
/*      [memattr]     READ | WRITE | NARROW (if RAM 8 qualifier)            */
/*      [memattr]     WRITE                 (if WOM 16 qualifier)           */
/*      [memattr]     WRITE | NARROW        (if WOM 8 qualifier)            */
/*                                                                          */
/*                                                                          */
/*  In the [bankswitch] section, the lines are of the following form:       */
/*                                                                          */
/*      $xxxx - $yyyy                                                       */
/*                                                                          */
/*  Here, $xxxx - $yyyy describes a range of addresses in the INTV          */
/*  address map.  There is no preload data associated with the range.       */
/*                                                                          */
/*                                                                          */
/*  In the [memattr] section, the lines are of the form:                    */
/*                                                                          */
/*      $xxxx - $yyyy = QUAL n                                              */
/*                                                                          */
/*  Here, "QUAL n" is allowed to be one of "RAM 8", "RAM 16", "WOM 8" or    */
/*  "WOM 16".  (WOM 8 and WOM 16 are new, and are being introduced with     */
/*  this tool.)  RAM is used for read/write memory, and WOM is used for     */
/*  write-only memory.                                                      */
/*                                                                          */
/*                                                                          */
/*  In [mapping] and [preload] sections, the lines are of the following     */
/*  format:                                                                 */
/*                                                                          */
/*      $xxxx - $yyyy = $zzzz                                               */
/*                                                                          */
/*  Here, $xxxx - $yyyy describes a range in the BIN file, and $zzzz        */
/*  is the address this place is inserted in the Intellicart memory map.    */
/*                                                                          */
/*                                                                          */
/*  This parser skips other sections, and skips lines starting with any     */
/*  character other than '[' or '$'.  It also ignores anything after a      */
/*  ';' on a line.  The parser is case insensitive, and is not white-space  */
/*  sensitive around '-' and '=' or at the start/end of the lines.  It is   */
/*  sensitive to whitespace inside '[' and ']'.                             */
/* ======================================================================== */
binseg_t *parse_cfg(FILE *f)
{
    int section = -1;
    int numseg = 0;
    char buf[1024], *s1, *s2;
    int lineno = 0;
    int num_preload = 0;
    int i;

    /* -------------------------------------------------------------------- */
    /*  Parse the config in a line-oriented manner.                         */
    /* -------------------------------------------------------------------- */
    while (fgets(buf, sizeof(buf) - 10, f))
    {
        lineno++;

        /* ---------------------------------------------------------------- */
        /*  Kill comments, carriage-returns and newlines.                   */
        /* ---------------------------------------------------------------- */
        if ((s1 = strchr(buf, ';' )) != NULL) *s1 = 0;
        if ((s1 = strchr(buf, '\r')) != NULL) *s1 = 0;
        if ((s1 = strchr(buf, '\n')) != NULL) *s1 = 0;

        /* ---------------------------------------------------------------- */
        /*  Kill leading whitespace.  Skip blank lines.                     */
        /* ---------------------------------------------------------------- */
        s1 = buf;
        while (*s1 && isspace(*s1)) s1++;
        if (!*s1) 
            continue;

        /* ---------------------------------------------------------------- */
        /*  Kill extra internal whitespace.  Force all whitespace to be     */
        /*  space characters, not tabs or anything like that.               */
        /* ---------------------------------------------------------------- */
        s2 = buf;
        while (*s1)
        {
            int c = *s1;

            c = *s1++;

            if (isspace(c))                   c = ' ';
            if (isalpha(c))                   c = toupper(c);
            if (!isspace(c) || !isspace(*s2)) *s2++ = c;
        }
        *s2 = 0;

        /* ---------------------------------------------------------------- */
        /*  Look for '[' as leading character.  If we see it, this is a     */
        /*  section header.                                                 */
        /* ---------------------------------------------------------------- */
        if (buf[0] == '[')
        {
            /* ------------------------------------------------------------ */
            /*  Nuke the right bracket if it exists.                        */
            /* ------------------------------------------------------------ */
            if ((s1 = strchr(buf + 1, ']')) != NULL) *s1 = 0;

            /* ------------------------------------------------------------ */
            /*  Search for the section name.                                */
            /* ------------------------------------------------------------ */
            for (i = 12; i >= 0; i--)
                if (!strcmp(secname[i], &buf[1]))
                    break;

            /* ------------------------------------------------------------ */
            /*  Warn the user if we didn't recognize the section ID.        */
            /* ------------------------------------------------------------ */
            if (i < 0)
            {
                fprintf(stderr, "Warning [%3d]:  Unrecognized CFG section: "
                        "%s\n", lineno, buf + 1);
            }

            /* ------------------------------------------------------------ */
            /*  We don't handle section numbers 0 through 8.  Map those to  */
            /*  -1.  Map 9 thru 12 to 0 thru 3.                             */
            /* ------------------------------------------------------------ */
            section = i > 7 ? i - 9 : -1;

            continue;
        }

        /* ---------------------------------------------------------------- */
        /*  If we're in a section we don't handle, skip further processing  */
        /*  of this configuration line.                                     */
        /* ---------------------------------------------------------------- */
        if (section < 0)
            continue;

        /* ---------------------------------------------------------------- */
        /*  Look for '$' as leading character.  If it's not there, skip to  */
        /*  the next line -- we don't handle this one.                      */
        /* ---------------------------------------------------------------- */
        if (buf[0] != '$')
        {
            fprintf(stderr, "Warning [%3d]:  Unhandled config line: '%s'\n", 
                    lineno, buf);
            continue;
        }

        /* ---------------------------------------------------------------- */
        /*  Parse the line according to the section.                        */
        /* ---------------------------------------------------------------- */
        switch (section)
        {
            uint_32 addr_lo, addr_hi;

            case 0:  /* [bankswitch] */
            {
                /* -------------------------------------------------------- */
                /*  Get $xxxx - $yyyy part of line.                         */
                /* -------------------------------------------------------- */
                s2 = parse_range(lineno, buf, &addr_lo, &addr_hi, 0);

                while (*s2)
                {
                    if (!isspace(*s2))
                    {
                        fprintf(stderr, "ERROR [%3d]: Trailing junk on "
                                "line.\n>> '%s'\n", lineno, buf);
                        exit(1);
                    }
                    s2++;
                }

                /* -------------------------------------------------------- */
                /*  Add the segment to the segment list.                    */
                /* -------------------------------------------------------- */
                seglist[numseg].ofs   = 0;
                seglist[numseg].addr  = addr_lo;
                seglist[numseg].len   = addr_hi - addr_lo + 1;
                seglist[numseg].flags = ICARTROM_READ | ICARTROM_BANKSW;
                numseg++;
                
                break;
            }

            case 1: /* [memattr] */
            {
                int ram_wom = -1, width = -1, tmp;

                /* -------------------------------------------------------- */
                /*  Get $xxxx - $yyyy part of line.                         */
                /* -------------------------------------------------------- */
                s2 = parse_range(lineno, buf, &addr_lo, &addr_hi, 1);

                /* -------------------------------------------------------- */
                /*  Look for '=' divider.                                   */
                /* -------------------------------------------------------- */
                while (*s2 && isspace(*s2))
                    s2++;
                
                if (*s2 != '=')
                {
                    fprintf(stderr, "ERROR [%3d]: Expected '=', found '%c'\n"
                            ">> '%s'\n", lineno, *s2, buf);
                    exit(1);
                }


                /* -------------------------------------------------------- */
                /*  Move past '=' and whitespace.                           */
                /* -------------------------------------------------------- */
                s2++;
                while (*s2 && isspace(*s2))
                    s2++;

                /* -------------------------------------------------------- */
                /*  Look for 'RAM' or 'WOM' qualifer.  We can blindly do a  */
                /*  NUL-terminate here because we under-read buf[] up top.  */
                /* -------------------------------------------------------- */
                tmp = s2[3];
                s2[3] = 0;
                if      (!strcmp("RAM", s2)) ram_wom = 1;
                else if (!strcmp("WOM", s2)) ram_wom = 0;
                else
                {
                    s2[3] = tmp;
                    fprintf(stderr, "ERROR [%3d]: Expected RAM or WOM, found "
                            "'%s' instead\n>> '%s'\n", lineno, s2, buf);
                    exit(1);
                }
                s2[3] = tmp;

                /* -------------------------------------------------------- */
                /*  Move past qualifier and whitespace.                     */
                /* -------------------------------------------------------- */
                s2 += 3;
                while (*s2 && isspace(*s2))
                    s2++;

                /* -------------------------------------------------------- */
                /*  Now we should have an 8 or 16.  Let's scan it out as    */
                /*  a decimal number and atoi it.                           */
                /* -------------------------------------------------------- */
                s1 = s2; 
                while (*s2 && ((*s2 >= '0' && *s2 <= '9'))) s2++;

                /* -------------------------------------------------------- */
                /*  If it doesn't end with EOL or whitespace, or we didn't  */
                /*  find anything, complain loudly.                         */
                /* -------------------------------------------------------- */
                if ((*s2 != 0 && *s2 != ' ') || s2 == s1)
                {
                    if (*s2 != 0 && *s2 != ' ')
                    {
                        fprintf(stderr, "ERROR [%3d]: Unexpected character "
                                "'%c' in [memattr] section\n>> '%s'\n",
                                lineno, *s1, buf);
                    } else
                    {
                        fprintf(stderr, "ERROR [%3d]: Missing memory width "
                                "in [memattr] section\n>> '%s'\n",
                                lineno, buf);
                    }
                    exit(1);
                }

                /* -------------------------------------------------------- */
                /*  Get the width and make sure it's 8 or 16.               */
                /* -------------------------------------------------------- */
                width = atoi(s1);

                if (width != 8 && width != 16)
                {
                    fprintf(stderr, "ERROR [%3d]:  Unsupported memory "
                            "width %d\n>> '%s'", lineno, width, buf);
                    exit(1);
                }

                /* -------------------------------------------------------- */
                /*  Add the segment to the segment list.                    */
                /* -------------------------------------------------------- */
                seglist[numseg].ofs   = 0;
                seglist[numseg].addr  = addr_lo;
                seglist[numseg].len   = addr_hi - addr_lo + 1;
                seglist[numseg].flags = ICARTROM_WRITE |
                                        (ram_wom    ? ICARTROM_READ   : 0) |
                                        (width == 8 ? ICARTROM_NARROW : 0);
                numseg++;
                
                break;
            }

            case 2: /* [preload] */
            case 3: /* [mapping] */
            {
                uint_32 cart_addr;

                /* -------------------------------------------------------- */
                /*  Get $xxxx - $yyyy part of line.                         */
                /* -------------------------------------------------------- */
                s2 = parse_range(lineno, buf, &addr_lo, &addr_hi, 1);

                /* -------------------------------------------------------- */
                /*  Look for '=' divider.                                   */
                /* -------------------------------------------------------- */
                while (*s2 && isspace(*s2))
                    s2++;
                
                if (*s2 != '=')
                {
                    fprintf(stderr, "ERROR [%3d]: Expected '=', found '%c'\n"
                            ">> '%s'\n", lineno, *s2, buf);
                    exit(1);
                }



                /* -------------------------------------------------------- */
                /*  Move past the '-' and look for the '$' that starts the  */
                /*  Intellicart address.                                    */
                /* -------------------------------------------------------- */
                s2++;
                while (*s2 && isspace(*s2)) s2++;
                            
                if (*s2 != '$')
                {
                    fprintf(stderr, "ERROR [%3d]: Missing '$' on second "
                            "first address\n>> '%s'\n", lineno, buf);
                    exit(1);
                }
                s2++;
                
                /* -------------------------------------------------------- */
                /*  Now scope out the icart address and see where it ends.  */
                /* -------------------------------------------------------- */
                s1 = s2; 
                while (*s2 && ((*s2 >= '0' && *s2 <= '9') ||
                               (*s2 >= 'A' && *s2 <= 'F')))    s2++;
                
                
                /* -------------------------------------------------------- */
                /*  Make sure it ended with whitespace or end-of-line.      */
                /*  Also make sure there was something at all!              */
                /* -------------------------------------------------------- */
                if ((*s2 != 0 && *s2 != ' ') || s2 == s1)
                {
                    if (*s2 != 0 && *s2 != ' ')
                    {
                        fprintf(stderr, "ERROR [%3d]: Unexpected character "
                                "'%c'\n>> '%s'\n",
                                lineno, *s1, buf);
                    } else
                    {
                        fprintf(stderr, "ERROR [%3d]: Missing cartridge "
                                "address\n>> '%s'\n", lineno, buf);
                    }
                    exit(1);
                }

                /* -------------------------------------------------------- */
                /*  Scan out the cartridge address.                         */
                /* -------------------------------------------------------- */
                sscanf(s1, "%x", &cart_addr);

                /* -------------------------------------------------------- */
                /*  Add the segment to the segment list.                    */
                /* -------------------------------------------------------- */
                seglist[numseg].ofs   = addr_lo;
                seglist[numseg].addr  = cart_addr;
                seglist[numseg].len   = addr_hi - addr_lo + 1;
                seglist[numseg].flags = ICARTROM_PRELOAD |
                                        (section == 3 ? ICARTROM_READ : 0);
                numseg++;
                num_preload++;
                
                break;
            }

            default:
            {
                fprintf(stderr, "ERROR [%3d]: Internal error.  Unknown "
                        "section #%d\n", lineno, section);
                exit(1);
            }
        }
                
        /* ---------------------------------------------------------------- */
        /*  Make sure we don't add too many segments.                       */
        /* ---------------------------------------------------------------- */
        if (numseg >= MAXSEG)
        {
            fprintf(stderr, "ERROR [%3d]: Too many ROM segments specified.\n",
                    lineno);
            
            exit(1);
        }
    }

    /* -------------------------------------------------------------------- */
    /*  If we didn't find any segments, just return the default list.       */
    /* -------------------------------------------------------------------- */
    if (numseg == 0)
        return defsegs;

    /* -------------------------------------------------------------------- */
    /*  If there were NO preloading sections (eg. [mapping] or [preload])   */
    /*  then tack on the default configuration.  (Grrrr....)                */
    /* -------------------------------------------------------------------- */
    if (num_preload < 1)
        for (i = 0; i < 3; i++)
        {
            seglist[numseg] = defsegs[i];

            /* ------------------------------------------------------------ */
            /*  Make sure we don't add too many segments.                   */
            /* ------------------------------------------------------------ */
            if (++numseg >= MAXSEG)
            {
                fprintf(stderr, "ERROR [EOF]: Too many ROM segments "
                        "specified.\n");
            
                exit(1);
            }
        }

    /* -------------------------------------------------------------------- */
    /*  Otherwise, terminate our list with a null record and return that.   */
    /* -------------------------------------------------------------------- */
    seglist[numseg].ofs   = 0;
    seglist[numseg].addr  = 0;
    seglist[numseg].len   = 0;
    seglist[numseg].flags = 0;

    return seglist;
}

/* ======================================================================== */
/*  APPLY_CFG -- Applies segment descriptions to a file image, generating   */
/*               an appropriate icartrom_t.                                 */
/*                                                                          */
/*  Note:  apply_cfg expects that file_img is 64K long even if the file     */
/*  isn't.  It also expects the file_img to be in the proper endian, etc.   */
/* ======================================================================== */
void apply_cfg(binseg_t *segs, uint_16 *file_img, uint_32 img_len, 
               icartrom_t *icart)
{
    int i;
    uint_32 new_len;

    /* -------------------------------------------------------------------- */
    /*  Traverse the segment list, calling icartrom_addseg on each.         */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < MAXSEG; i++)
    {
        /* ---------------------------------------------------------------- */
        /*  The list is terminated by a zero-length segment with no flags.  */
        /* ---------------------------------------------------------------- */
        if (segs[i].len == 0 && segs[i].flags == 0)
            break;
        
        /* ---------------------------------------------------------------- */
        /*  Add the segment once w/out the PRELOAD bits.                    */
        /* ---------------------------------------------------------------- */
        icartrom_addseg(icart, NULL, 
                        segs[i].addr, 
                        segs[i].len,
                        segs[i].flags & ~ICARTROM_PRELOAD, 0);

        printf("SEGMENT ofs %.4X  len %.4X  addr %.4X  FLAGS: %c%c%c%c\n", 
               segs[i].ofs, segs[i].len, segs[i].addr, 
               segs[i].flags & ICARTROM_READ   ? 'R' : '-',
               segs[i].flags & ICARTROM_WRITE  ? 'W' : '-',
               segs[i].flags & ICARTROM_BANKSW ? 'B' : '-',
               segs[i].flags & ICARTROM_NARROW ? 'N' : '-');

        /* ---------------------------------------------------------------- */
        /*  Figure out where in the file image we're getting data.          */
        /*  If ICARTROM_PRELOAD is unset, then we're not getting any.       */
        /* ---------------------------------------------------------------- */
        if ((segs[i].flags & ICARTROM_PRELOAD) == 0)
            continue;

        /* ---------------------------------------------------------------- */
        /*  Skip preload segments that lie entirely outside the file_img.   */
        /*  Trim back those that reach beyond the end of the file_img.      */
        /* ---------------------------------------------------------------- */
        if (segs[i].ofs > img_len)
             continue;

        new_len = segs[i].len;
        if (segs[i].ofs + new_len > img_len)
            new_len = img_len - segs[i].ofs;

        printf("PRELOAD ofs %.4X  len %.4X  addr %.4X\n", 
               segs[i].ofs, new_len, segs[i].addr);
        
        /* ---------------------------------------------------------------- */
        /*  Re-add the segment again with the PRELOAD bits.  We do this     */
        /*  in two steps since the second addseg may add a shorter segment. */
        /* ---------------------------------------------------------------- */
        icartrom_addseg(icart, 
                        segs[i].ofs + file_img,
                        segs[i].addr, 
                        new_len,
                        segs[i].flags, 0);
    }
}

uint_16 binfile[65536];

/* ======================================================================== */
/*  MAIN                                                                    */
/*  This is the main program.  The action happens here.                     */
/* ======================================================================== */
int main(int argc, char *argv[])
{
    int i, len;
    char bin_fn[1024], cfg_fn[1024], rom_fn[1024];
    int fn_len;
    FILE *fb, *fc, *fr;
    binseg_t *segs;
    uint_8 *rom_img;
    uint_32 size;

    if (argc != 2)
    {
        fprintf(stderr, "usage: bin2rom foo[.bin]\n");
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  Generate .BIN, .CFG, and .ROM filenames from argument filename.     */
    /*  If the argument lacks a .BIN extension, add one.                    */
    /* -------------------------------------------------------------------- */
    strncpy(bin_fn, argv[1], 1019);
    bin_fn[1019] = 0;

    fn_len = strlen(bin_fn);
    if (strcmp(bin_fn + fn_len - 4, ".bin"))
    {
        strcpy(bin_fn + fn_len, ".bin");
        fn_len += 4;
    }

    strcpy(cfg_fn, bin_fn);
    strcpy(rom_fn, bin_fn);

    strcpy(cfg_fn + fn_len - 4, ".cfg");
    strcpy(rom_fn + fn_len - 4, ".rom");

    /* -------------------------------------------------------------------- */
    /*  Open the BIN file.                                                  */
    /* -------------------------------------------------------------------- */
    fb = fopen(bin_fn, "rb");
    
    if (!fb)
    {
        perror("fopen()");
        fprintf(stderr, "Couldn't open '%s' for reading\n", bin_fn);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  Determine filesize and read it in.                                  */
    /* -------------------------------------------------------------------- */
    fseek(fb, 0, SEEK_END);
    if ((len = ftell(fb)) < 0)
    {
        fprintf(stderr, "Error seeking\n");
        exit(1);
    }
    rewind(fb);

    fread(binfile, 1, len, fb);
    fclose(fb);

    len >>= 1;

#ifdef _LITTLE_ENDIAN
    /* -------------------------------------------------------------------- */
    /*  Correct for endianness.                                             */
    /* -------------------------------------------------------------------- */
    for (i = 0; i < len; i++)
        binfile[i] = (binfile[i] >> 8) | (binfile[i] << 8);
#endif

    /* -------------------------------------------------------------------- */
    /*  Initialize the icartrom.                                            */
    /* -------------------------------------------------------------------- */
    icartrom_init(&the_icart);

    /* -------------------------------------------------------------------- */
    /*  Adjust the default config, in case we end up using it.              */
    /* -------------------------------------------------------------------- */
    adj_defsegs(len);

    /* -------------------------------------------------------------------- */
    /*  Now open the .CFG file.  If we can open it, then parse it.          */
    /*  Otherwise, we skip it -- lack of .CFG file is non-fatal.            */
    /* -------------------------------------------------------------------- */
    if ((fc = fopen(cfg_fn, "r")) != NULL)
    {
        segs = parse_cfg(fc);
        fclose(fc);
    } else
    {
        segs = defsegs;
    }

    /* -------------------------------------------------------------------- */
    /*  Apply the configuration.  This generates the icartrom.              */
    /* -------------------------------------------------------------------- */
    apply_cfg(segs, binfile, len, &the_icart);

    /* -------------------------------------------------------------------- */
    /*  Finally, generate the ROM file and write it out.                    */
    /* -------------------------------------------------------------------- */
    fr = fopen(rom_fn, "wb");
    if (!fr)
    {
        fprintf(stderr, "ERROR:  Could not open '%s' for writing\n", rom_fn);
        exit(1);
    }

    rom_img = icartrom_genrom(&the_icart, &size);

    if (!rom_img)
    {
        fprintf(stderr, "ERROR:  No ROM image generated?\n");
        exit(1);
    }

    fwrite(rom_img, 1, size, fr);
    fclose(fr);

    return 0;
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
