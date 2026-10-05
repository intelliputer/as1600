/* ======================================================================== */
/*  SYMTAB -- Symbol table routines for DASM0256.                           */
/* ======================================================================== */

#define ADDR(x)  (x) >> 3, (x) & 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "symtab.h"

/* ------------------------------------------------------------------------ */
/*  Internal static comparison functions for AVL trees.                     */
/* ------------------------------------------------------------------------ */
static int compare_symbol(symtab_ent_t *a, symtab_ent_t *b)
{
    return -strcmp(a->symbol, b->symbol);
}

static int compare_address(symtab_ent_t *a, symtab_ent_t *b)
{
    return  (a->address > b->address) ? -1 : 
            (a->address < b->address) ?  1 : 
            (a->addrseq > b->addrseq) ? -1 : 
            (a->addrseq < b->addrseq) ?  1 : 0;
}

/* ------------------------------------------------------------------------ */
/*  SYMTAB_CREATE           -- Allocate and return a new symbol table.      */
/* ------------------------------------------------------------------------ */
symtab_t* symtab_create(void)
{
    symtab_t *new_symtab;

    new_symtab = calloc(sizeof(symtab_t), 1);

    if (!new_symtab) { fprintf(stderr, "symtab: Out of memory\n"); exit(1); }

    AVL_InitTree(&new_symtab->by_symbol );
    AVL_InitTree(&new_symtab->by_address);

    AVL_SetTreeComp(&new_symtab->by_symbol,  (void*)compare_symbol );
    AVL_SetTreeComp(&new_symtab->by_address, (void*)compare_address);

    return new_symtab;
}

/* ------------------------------------------------------------------------ */
/*  SYMTAB_DEFSYM           -- Associate address with symbol.               */
/* ------------------------------------------------------------------------ */
char *   symtab_defsym      (symtab_t *symtab, char *symbol, uint_32 addr)
{   
    symtab_ent_t *entry;
    char *sym_copy;
    int err;

    /* -------------------------------------------------------------------- */
    /*  Allocate an entry and set it up.                                    */
    /* -------------------------------------------------------------------- */
    entry    = calloc(sizeof(symtab_ent_t), 1);
    sym_copy = strdup(symbol);
    if (!entry || !sym_copy) 
    { 
        fprintf(stderr, "symtab_defsym: Out of memory\n"); 
        exit(1); 
    }

    entry->symbol  = sym_copy;
    entry->address = addr;

    /* -------------------------------------------------------------------- */
    /*  Add the address to the "by symbol" tree.  If we find a duplicate,   */
    /*  generate an error if the addresses differ.  If the dup has the      */
    /*  same address, drop this addition.                                   */
    /* -------------------------------------------------------------------- */
    err = AVL_AddNode(&symtab->by_symbol, entry);

    if (err != EAVL_NOERR)
    {
        if (err == EAVL_DUPREC)
        {
            symtab_ent_t *dup = NULL;

            if (AVL_SearchTree(&symtab->by_symbol, 
                               (void*)entry, (void*)&dup) != EAVL_NOERR)
            {
                fprintf(stderr, "symtab_defsym: Internal error\n");
                exit(1);
            }
            if (dup->address == entry->address)
            {
                /* This is a harmless duplicate.  Just return. */
                free(entry->symbol);
                free(entry);
                return dup->symbol;
            } else
            {
                fprintf(stderr, "ERROR:  Symbol redefined with different "
                                "addresses!\n %s at %.4X.%.1X vs. %.4X.%.1X\n",
                                entry->symbol,
                                ADDR(entry->address), ADDR(dup->address));
                exit(1);
            }
        } else
        {
            fprintf(stderr, "symtab_defsym: AVL error %d\n", err);
            exit(1);
        }
    }

    /* -------------------------------------------------------------------- */
    /*  Increment our symbol counter.                                       */
    /* -------------------------------------------------------------------- */
    symtab->num_symbols++;

    /* -------------------------------------------------------------------- */
    /*  Add the address to the "by address" tree.  Here, if we find a dup,  */
    /*  just keep increasing our sequence number until we aren't a dup      */
    /*  any more.  This resolves collisions, in a somewhat clunky way.      */
    /* -------------------------------------------------------------------- */
    while (AVL_AddNode(&symtab->by_address, entry) == EAVL_DUPREC)
        entry->addrseq++;

    return sym_copy;
}


/* ------------------------------------------------------------------------ */
/*  SYMTAB_GETSYM           -- Get symbol associated with address.          */
/* ------------------------------------------------------------------------ */
char    *symtab_getsym(symtab_t *symtab, uint_32 addr, int attrib, int which)
{
    symtab_ent_t key, *find = NULL;
    int err;
    char buf[64], *symbol;

    /* -------------------------------------------------------------------- */
    /*  Set up our key structure, and then search the AVL tree for it.      */
    /* -------------------------------------------------------------------- */
    key.address = addr;
    key.addrseq = which;

    err = AVL_SearchTree(&symtab->by_address, (void*)&key, (void*)&find);

    if (err == EAVL_NULLTREE) err = EAVL_NOTFOUND;

    if (err != EAVL_NOERR && err != EAVL_NOTFOUND)
    {
        fprintf(stderr, "symtab_getsym: AVL error %d\n", err);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  If we found it, return it.                                          */
    /* -------------------------------------------------------------------- */
    if (err != EAVL_NOTFOUND)
        return find->symbol;

    /* -------------------------------------------------------------------- */
    /*  If we didn't find it, generate a generic label for this address     */
    /*  if we're allowed to, otherwise just return NULL.                    */
    /* -------------------------------------------------------------------- */
    if (!attrib || which)
        return NULL;

    sprintf(buf, "%c_%.4X", attrib, addr >> 3);

    symbol = symtab_defsym(symtab, buf, addr);

    return symbol;
}

/* ------------------------------------------------------------------------ */
/*  SYMTAB_GETADDR          -- Get address associated with symbol.          */
/* ------------------------------------------------------------------------ */
int       symtab_getaddr     (symtab_t *symtab, char *symbol, uint_32 *addr)
{
    symtab_ent_t key, *find = NULL;
    int err;

    /* -------------------------------------------------------------------- */
    /*  Set up our key structure, and then search the AVL tree for it.      */
    /* -------------------------------------------------------------------- */
    key.symbol = symbol;

    err = AVL_SearchTree(&symtab->by_symbol, (void*)&key, (void*)&find);

    if (err == EAVL_NULLTREE) err = EAVL_NOTFOUND;
    if (err != EAVL_NOERR && err != EAVL_NOTFOUND)
    {
        fprintf(stderr, "symtab_getsym: AVL error %d\n", err);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  If we didn't find it, return an error.                              */
    /* -------------------------------------------------------------------- */
    if (err == EAVL_NOTFOUND)
        return -1;

    /* -------------------------------------------------------------------- */
    /*  Otherwise, return the address.                                      */
    /* -------------------------------------------------------------------- */
    *addr = find->address;
    return 0;
}


/* ------------------------------------------------------------------------ */
/*  SYMTAB_XREF_ADDR        -- Say 'addr' is referenced from 'xref'.        */
/* ------------------------------------------------------------------------ */
void     symtab_xref_addr   (symtab_t *symtab, uint_32 addr, uint_32 xref)
{
    symtab_ent_t key, *find = NULL;
    int err, i;

    /* -------------------------------------------------------------------- */
    /*  Set up our key structure, and then search the AVL tree for it.      */
    /* -------------------------------------------------------------------- */
    key.address = addr;
    key.addrseq = 0;

    err = AVL_SearchTree(&symtab->by_address, (void*)&key, (void*)&find);

    if (err == EAVL_NULLTREE) err = EAVL_NOTFOUND;
    if (err != EAVL_NOERR && err != EAVL_NOTFOUND)
    {
        fprintf(stderr, "symtab_getsym: AVL error %d\n", err);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  If we found it, add the cross-reference and return.                 */
    /* -------------------------------------------------------------------- */
    if (err != EAVL_NOTFOUND)
    {
        /* ---------------------------------------------------------------- */
        /*  Avoid adding redundant cross-references.                        */
        /* ---------------------------------------------------------------- */
        for (i = 0; i < find->xrefs; i++)
            if (find->xref[i] == xref)
                return;

        /* ---------------------------------------------------------------- */
        /*  Allocate memory for the cross-reference if needed.              */
        /* ---------------------------------------------------------------- */
        if (find->xrsize <= find->xrefs)
        {
            if (!find->xrsize)  find->xrsize = 4;
            else                find->xrsize <<= 1;

            find->xref = realloc(find->xref, find->xrsize * sizeof(uint_32));
        }

        /* ---------------------------------------------------------------- */
        /*  Add the cross-reference.                                        */
        /* ---------------------------------------------------------------- */
        find->xref[find->xrefs++] = xref;
    }
} 
    

/* ------------------------------------------------------------------------ */
/*  SYMTAB_DREF_ADDR        -- Say that we've directly referenced 'addr'.   */
/* ------------------------------------------------------------------------ */
void     symtab_dref_addr   (symtab_t *symtab, uint_32 addr)
{
    symtab_ent_t key, *find = NULL;
    int err;

    /* -------------------------------------------------------------------- */
    /*  Set up our key structure, and then search the AVL tree for it.      */
    /* -------------------------------------------------------------------- */
    key.address = addr;
    key.addrseq = 0;

    err = AVL_SearchTree(&symtab->by_address, (void*)&key, (void*)&find);

    if (err == EAVL_NULLTREE) err = EAVL_NOTFOUND;
    if (err != EAVL_NOERR && err != EAVL_NOTFOUND)
    {
        fprintf(stderr, "symtab_getsym: AVL error %d\n", err);
        exit(1);
    }

    /* -------------------------------------------------------------------- */
    /*  If we found it, increment the direct-reference counter and return.  */
    /* -------------------------------------------------------------------- */
    if (err != EAVL_NOTFOUND)
    {
        find->drefs++;
    }
} 
    
static int ord_num = 0;
static symtab_ent_t **ord_list = NULL;

/* ------------------------------------------------------------------------ */
/*  PUT_IN_LIST             -- Internal: builds linear list from tree.      */
/* ------------------------------------------------------------------------ */
static int put_in_list(void *sym)
{
    ord_list[ord_num++] = (symtab_ent_t*)sym;   
    return 0;
}

/* ------------------------------------------------------------------------ */
/*  DISP_SYM                -- Internal helper:  displays a symbol.         */
/* ------------------------------------------------------------------------ */
static void  disp_sym(FILE *f, symtab_ent_t *sym, int col)
{
    fprintf(f, "%s%14s:%c %.4X.%.1X%s",
            col == 0 ? "## " : "  ",
            sym->symbol,
            sym->addrseq == 0 && sym->drefs == 0 ? '!' :
            sym->addrseq != 0                    ? 'a' : ' ',
            ADDR(sym->address),
            col == 2 ? "\n" : "");
}

/* ------------------------------------------------------------------------ */
/*  DUMP_XREF               -- Internal helper:  displays cross-refs.       */
/* ------------------------------------------------------------------------ */
static FILE *xref_file = NULL;
static int   disp_xref(symtab_ent_t *sym)
{
    int i;
    FILE *f = xref_file;

    if (!f) return -1;

    if (sym->xrefs == 0) return 0;

    for (i = 0; i < sym->xrefs; i++)
    {
        if ((i & 7) == 0)
        {
            if (i == 0) 
                fprintf(f, "## %14s:%c", sym->symbol,
                            sym->addrseq == 0 && sym->drefs == 0 ? '!' :
                            sym->addrseq != 0                    ? 'a' : ' ');
            else        
                fprintf(f, "\n##                 ");
        }
        fprintf(f, " %.4X.%.1X", ADDR(sym->xref[i]));
    }

    fputc('\n', f);

    return 0;
}

/* ------------------------------------------------------------------------ */
/*  DUMP_GENERIC            -- Internal helper used to dump symbol tbls.    */
/* ------------------------------------------------------------------------ */
static void dump_generic(symtab_t *symtab, PAVLTree tree, char * sort, FILE *f)
{
    int i, j, k, third;

    fprintf(f, "\n##-------------------------------------"
               "---------------------------------------\n");
    fprintf(f, "## Symbol Table, sorted by %s", sort);
    fprintf(f, "\n##-------------------------------------"
               "---------------------------------------\n");

    ord_num  = 0;
    ord_list = calloc(symtab->num_symbols + 4, sizeof(symtab_ent_t*));
    if (!ord_list)
    {
        fprintf(f, "## --> warning, not enough memory for operation\n");
        return;
    }

    AVL_Traverse(tree, InOrder, (void*) put_in_list);

    third = (symtab->num_symbols + 2) / 3;

    for (i = j = 0; i < third; i++)
    {
        for (j = 0, k = i; j < 3; j++, k += third)
        {
            if (ord_list[k]) disp_sym(f, ord_list[k], j);
            else
                break;
        }

        if (j != 3) break;
    }

    if (j != 3) fputc('\n', f);

    fprintf(f, "##-------------------------------------"
               "---------------------------------------\n");

    free(ord_list);
}

/* ------------------------------------------------------------------------ */
/*  SYMTAB_DUMP_BY_SYMS     -- Write symbol table dump, sorted by symbol.   */
/* ------------------------------------------------------------------------ */
void     symtab_dump_by_syms(symtab_t *symtab, FILE *f)
{
    dump_generic(symtab, &symtab->by_symbol, "Symbol", f);
}

/* ------------------------------------------------------------------------ */
/*  SYMTAB_DUMP_BY_ADDR     -- Write symbol table dump, sorted by address.  */
/* ------------------------------------------------------------------------ */
void     symtab_dump_by_addr(symtab_t *symtab, FILE *f)
{
    dump_generic(symtab, &symtab->by_address, "Address", f);
}

/* ------------------------------------------------------------------------ */
/*  SYMTAB_DUMP_XREFS       -- Write cross-reference table.                 */
/* ------------------------------------------------------------------------ */
void     symtab_dump_xrefs  (symtab_t *symtab, FILE *f)
{
    fprintf(f, "\n##-------------------------------------"
               "---------------------------------------\n");
    fprintf(f, "## Cross Reference Table");
    fprintf(f, "\n##-------------------------------------"
               "---------------------------------------\n");

    xref_file = f;
    AVL_Traverse(&symtab->by_address, InOrder, (void*) disp_xref);

    fprintf(f, "##-------------------------------------"
               "---------------------------------------\n");
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
