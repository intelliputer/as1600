/*
HEADER:     ;
TITLE:      Frankenstein Cross Assemblers;
VERSION:    2.0;
DESCRIPTION: "  Reconfigurable Cross-assembler producing Intel (TM)
        Hex format object records.  ";
KEYWORDS:   cross-assemblers, 1805, 2650, 6301, 6502, 6805, 6809, 
        6811, tms7000, 8048, 8051, 8096, z8, z80;
SYSTEM:     UNIX, MS-Dos ;
FILENAME:   frasmain.c;
WARNINGS:   "This software is in the public domain.  
        Any prior copyright claims are relinquished.  

        This software is distributed with no warranty whatever.  
        The author takes no responsibility for the consequences 
        of its use.

        Yacc (or Bison) required to compile."  ;
SEE-ALSO:   base.doc, as*.doc (machine specific appendices) , 
        as*.1 (man pages);
AUTHORS:    Mark Zenier;
COMPILERS:  Microport Sys V/AT, ATT Yacc, Turbo C V1.5, Bison (CUG disk 285)
        (previous versions Xenix, Unisoft 68000 Version 7, Sun 3);
*/
/*
    description Main file
    usage       Unix, framework crossassembler
    history     September 25, 1987
            August 3, 1988    v 1.4
            September 14, 1990  v 1.5  Dosified
*/

#define Global

#include <stdio.h>
#include "frasmdat.h"

FILE * intermedf = (FILE *) NULL;
char interfn[32] = 
#ifdef DOSTMP
 "frtXXXXXX";
#else
 "/usr/tmp/frtXXXXXX";
#endif
char *cfgfn = NULL;
char *hexfn, *loutfn;
int errorcnt = 0, warncnt = 0;
int listflag = FALSE, hexflag = FALSE, hexvalid = FALSE;
static int debugmode = FALSE;
static FILE *symbf;
static char *symbfn;
static int  symbflag = FALSE;
char hexcva[17] = "0123456789abcdef";

#ifdef NOGETOPT
#include "getopt.h"
#endif

#ifdef macintosh
#include "getopt.h"
#include "console.h"
#endif

void parse_pathvar(char *str);
void append_path(char *str);

main(argc, argv)
    int argc;
    char *(argv[]);
/*
    description top driver routine for framework cross assembler
                set the cpu type if implemented in parser
                process the command line parameters
                setup the tables
                call the first pass parser
                print the symbol table
                call the second pass
                close down and delete the outputs if any errors
    return      exit(2) for error, exit(0) for OK
*/
{
    extern char *optarg;
    extern int optind;
    int grv;

#ifdef macintosh
    argc = ccommand( &argv );
#endif

    grv = cpumatch(argv[0]);

    while( (grv = getopt(argc, argv, "dh:o:l:s:p:i:")) != EOF)
    {
        switch(grv)
        {
        case 'i':
            append_path(optarg);
            break;
        case 'o':
        case 'h':
            hexfn = optarg;
            hexflag = hexvalid = TRUE;
            break;
        
        case 'l':
            loutfn = optarg;
            listflag = TRUE;
            break;

        case 'd':
            debugmode = TRUE;
            break;

        case 's':
            symbflag = TRUE;
            symbfn = optarg;
            break;

        case 'p':
            if( ! cpumatch(optarg) )
            {
                fprintf(stderr, "%s: no match on CPU type %s, default used\n", 
                    argv[0], optarg);
            }
            break;

        case '?':
            break;
        }
    }

    if(optind < argc)
    {
        if(strcmp(argv[optind], "-") == 0)
        {
            yyin = stdin;
        }
        else
        {
            if( (yyin = fopen(argv[optind], "r")) == (FILE *)NULL)
            {
                fprintf(stderr, 
                    "%s: cannot open input file %s\n",
                    argv[0], argv[optind]);
                exit(1);
            }
        }
    }
    else
    {
        fprintf(stderr, "%s: no input file\n", argv[0]);
        exit(1);
    }

    parse_pathvar(getenv("AS1600_PATH"));

    if(listflag)
    {
        if(strcmp(argv[optind], loutfn) == 0) 
        {
            fprintf(stderr, "%s: list file overwrites input %s\n",
                argv[0], loutfn);
            listflag = FALSE;
        }
        else if( (loutf = fopen(loutfn, "w")) == (FILE *) NULL)
        {
            fprintf(stderr, "%s: cannot open list file %s\n",
                argv[0], loutfn);
            listflag = FALSE;
        }
    }

    if( ! listflag)
    {
        loutf = stdout;
    }
#ifndef macintosh
    mktemp(interfn);
#endif

    if( (intermedf = fopen(interfn, "w")) == (FILE *) NULL)
    {
        fprintf(stderr, "%s: cannot open temp file %s\n",
            argv[0], interfn);
        exit(1);
    }

    setophash();
    setreserved();
    elseifstk[0] = endifstk[0] = If_Err;
    fprintf(intermedf, "F:%s\n", argv[optind]);
    infilestk[0].fpt = yyin;
    infilestk[0].fnm = argv[optind];
    currfstk = 0;
    currseg = 0;
    
    yyparse();
    
    if(ifstkpt != 0)
        fraerror("active IF at end of file");

    buildsymbolindex();
    if(listflag)
        printsymbols();

    if(symbflag)
    {
        if(strcmp(argv[optind], symbfn) == 0) 
        {
            fprintf(stderr, "%s: symbol file overwrites input %s\n",
                argv[0], symbfn);
        }
        else if( (symbf = fopen(symbfn, "w")) == (FILE *) NULL)
        {
            fprintf(stderr, "%s: cannot open symbol file %s\n",
                argv[0], symbfn);
        }
        else
        {
            filesymbols();
            fclose(symbf);
        }
    }

    
    fclose(intermedf);
    if( (intermedf = fopen(interfn, "r")) == (FILE *) NULL)
    {
        fprintf(stderr, "%s: cannot open temp file %s\n",
            argv[0], interfn);
        exit(1);
    }

    if(errorcnt > 0)
        hexflag = FALSE;

    if(hexflag)
    {
        int hexfn_len = strlen(hexfn);
        char *s = hexfn + hexfn_len;

        if (hexfn_len < 5 || (strcmp(s - 4, ".rom") && strcmp(s - 4, ".bin")))
        {
            fprintf(stderr, 
                   "%s: Output file must have .bin or .rom extension\n",
                   argv[0]);
            hexflag = FALSE;
        } else if (strcmp(argv[optind], hexfn) == 0) 
        {
            fprintf(stderr, "%s: binary output overwrites input %s\n",
                argv[0], hexfn);
            hexflag = FALSE;
        }
        else 
        {
            if( (hexoutf = fopen(hexfn, "wb")) == (FILE *) NULL)
            {
                fprintf(stderr, "%s: cannot open binary output %s\n",
                    argv[0], hexfn);
                hexflag = FALSE;
            }
            if (strcmp(s - 4, ".bin") == 0)
            {
                cfgfn = strdup(hexfn);
                if (!cfgfn) 
                { 
                    fprintf(stderr, "%s: Out of memory\n", argv[0]);
                    exit(1);
                }

                s = cfgfn + hexfn_len - 4;
                strcpy(s, ".cfg");
                if ( (cfgoutf = fopen(cfgfn, "wb")) == (FILE *)NULL)
                {
                    fprintf(stderr, "%s: cannot open config file %s\n",
                            argv[0], cfgfn);
                    fclose(hexoutf);
                    hexflag = FALSE;
                }
            }
        }
    }

    currfstk = 0;
    outphase();

    if(errorcnt > 0)
        hexvalid = FALSE;

    fprintf(loutf, " ERROR SUMMARY - ERRORS DETECTED %d\n", errorcnt);
    fprintf(loutf, "               -  WARNINGS       %d\n", warncnt);

    if(listflag)
    {
        fprintf(stderr, " ERROR SUMMARY - ERRORS DETECTED %d\n", 
            errorcnt);
        fprintf(stderr, "               -  WARNINGS       %d\n", 
            warncnt);
    }

    if(listflag)
        fclose(loutf);
    
    if(hexflag)
    {
        fclose(hexoutf);
        if( ! hexvalid)
            unlink(hexfn);
        if (cfgoutf)
        {
            fclose(cfgoutf);
            if (! hexvalid)
                unlink(cfgfn);
        }
    }
    
    fclose(intermedf);
    if( ! debugmode)
        unlink(interfn);
    else
        abort();
    
    exit(errorcnt > 0 ? 2 : 0);
}
        

frafatal(str)
    char * str;
/*
    description Fatal error subroutine, shutdown and quit right now!
    parameters  message
    globals     if debug mode is true, save intermediate file
    return      exit(2)
*/
{
    fprintf(stderr, "Fatal error - %s\n",str);

    if( intermedf != (FILE *) NULL)
    {
        fclose(intermedf);
        if( ! debugmode)
            unlink(interfn);
    }
        
    exit(2);
}

frawarn(str)
    char * str;
/*
    description first pass - generate warning message by writing line
            to intermediate file
    parameters  message
    globals     the count of warnings
*/
{
    fprintf(intermedf, "E: WARNING - %s\n",str);
    warncnt++;
}

fraerror(str)
    char * str;
/*
    description first pass - generate error message by writing line to
            intermediate file
    parameters  message
    globals     count of errors
*/
{
    fprintf(intermedf, "E: ERROR - %s\n",str);
    errorcnt++;
}

fracherror(str, start, beyond)
    char * str, *start, *beyond;
/*
    description first pass - generate error message by writing line to
            intermediate file
    parameters  message
            pointer to bad character definition
            pointer after bad definition
    globals     count of errors
*/
{
    char bcbuff[8];
    int cnt;

    for(cnt=0; start < beyond && *start != '\0' && cnt < 7; cnt++)
    {
        bcbuff[cnt] = *start++;
    }
    bcbuff[cnt] = '\0';

    fprintf(intermedf, "E: ERROR - %s \'%s\'\n",str, bcbuff);
    errorcnt++;
}


prtequvalue(fstr, lv)
    char * fstr;
    long lv;
/*
    description first pass - generate comment lines in intermediate file
            for the value in a set, equate, or org statement, etc...
    parameters  format string and a long integer value
*/
{
    fprintf(intermedf, fstr, lv);
}

#define SYMPERLINE 3

printsymbols()
/*
    description print the symbols on the listing file, 3 symbols
            across.  Only the first 15 characters are printed
            though all are significant.  Reserved symbols are
            not assigned symbol numbers and thus are not printed.
    globals     the symbol index array and the symbol table elements.
*/
{
    int syn, npl = 0;
    struct symel *syp;

    for(syn = 1; syn <nextsymnum; syn++)
    {
        if(npl >= SYMPERLINE)
        {
            fputc('\n', loutf);
            npl = 0;
        }

        syp = symbindex[syn];

        if(syp -> seg != SSG_UNDEF)
            fprintf(loutf, "%8.8lx %-15.15s  ",syp -> value,
                syp -> symstr);
        else
            fprintf(loutf, "???????? %-15.15s  ", syp -> symstr);
        npl++;
    }

    if(npl > 0)
        fputc('\n', loutf);

    fputc('\f', loutf);
}


filesymbols()
/*
    description print the symbols to the symbol table file
    globals     the symbol index array and the symbol table elements.
*/
{
    int syn;
    struct symel *syp;

    for(syn = 1; syn <nextsymnum; syn++)
    {
        syp = symbindex[syn];

        if(syp -> seg != SSG_UNDEF)
            fprintf(symbf, "%8.8lx %s\n",syp -> value,
                syp -> symstr);
        else
            fprintf(symbf, "???????? %s\n", syp -> symstr);
    }
}

FILE *path_fopen(const char *fname, const char *mode)
{
    FILE *f;
    pathlist *cur_path;
    static char *path_fname = NULL;
    static int   path_fname_len = 0;
    int          fname_len = 0;
 
    
    if ((f = fopen(fname, mode)) != NULL)
        return f;

    cur_path = pathlist_head;

    while (cur_path)
    {
        while (fname_len + cur_path->plen + 2 > path_fname_len)
        {
            if (!path_fname_len) path_fname_len = 64;
            path_fname = realloc(path_fname, path_fname_len <<= 2);
            if (!path_fname)
            {
                fprintf(stderr, "Out of memory in path_fopen\n");
                exit(1);
            }
        }
        sprintf(path_fname, "%s/%s", cur_path->path, fname);
        if ((f = fopen(path_fname, "r")) != NULL)
            return f;

        cur_path = cur_path->next;
    }

    return NULL;
}

pathlist *get_new_path(char *pathname)
{
    pathlist *new_path;
    new_path = calloc(sizeof(pathlist), 1);
    if (!new_path)
    {
        fprintf(stderr, "Out of memory in insert_path\n");
        exit(1);
    }

    new_path->path = strdup(pathname);
    new_path->plen = strlen(pathname);

    if (!new_path->path)
    {
        fprintf(stderr, "Out of memory in get_new_path\n");
        exit(1);
    }

    return new_path;
}

#if 0
void insert_path(char *pathname)
{
    pathlist *new_path = get_new_path(pathname);
    
    new_path->next = pathlist_head;
    pathlist_head  = new_path;
    if (!pathlist_tail)
        pathlist_tail = pathlist_head;
}
#endif

void append_path(char *pathname)
{
    pathlist *new_path = get_new_path(pathname);
    
    if (pathlist_tail) pathlist_tail->next = new_path;
    else               pathlist_head       = new_path;
    
    pathlist_tail = new_path;
}

void parse_pathvar(char *str)
{
    char *tmp, *p;

    if (!str || strlen(str) < 1)
        return;

    tmp = strdup(str);  /* we don't know if str is writeable */

    p = strtok(tmp, ";");
    while (p)
    {
        append_path(p);
        p = strtok(NULL, ";");
    }

    free(tmp);
}
 

