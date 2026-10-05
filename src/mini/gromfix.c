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
/*                  Copyright (c) 1999-2000, Joe Zbiciak                    */
/* ======================================================================== */

/*
; Note:  The assembler will generate a 16-bit GROM image with this, and most
;        emulators expect an 8-bit GROM image.  Therefore, you will need to 
;        post-process the binary created by this source with a short program 
;        such as this one which works under Unix:
*/

#include <stdio.h>

int main(void)
{ 
    int c, x = 0; 
    while ((c = getchar()) != EOF)
    {
        if (x) putchar(c);
        x = !x;
    }
}
 
