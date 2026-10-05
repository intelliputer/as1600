typedef union {
    int intv;
    long    longv;
    char    *strng;
    struct symel *symb;
} YYSTYPE;
#define	REGISTER	257
#define	KOC_BDEF	258
#define	KOC_ELSE	259
#define	KOC_END	260
#define	KOC_ENDI	261
#define	KOC_EQU	262
#define	KOC_IF	263
#define	KOC_INCLUDE	264
#define	KOC_ORG	265
#define	KOC_RESM	266
#define	KOC_SDEF	267
#define	KOC_SET	268
#define	KOC_WDEF	269
#define	KOC_CHSET	270
#define	KOC_CHDEF	271
#define	KOC_CHUSE	272
#define	KOC_opcode	273
#define	KOC_opcode_i	274
#define	KOC_relbr	275
#define	KOC_relbr_x	276
#define	KOC_SDBD	277
#define	KOC_ROMW	278
#define	KOC_PROC	279
#define	KOC_ENDP	280
#define	KOC_STRUCT	281
#define	KOC_ENDS	282
#define	KOC_MEMATTR	283
#define	KOC_DDEF	284
#define	CONSTANT	285
#define	EOL	286
#define	KEOP_AND	287
#define	KEOP_DEFINED	288
#define	KEOP_EQ	289
#define	KEOP_GE	290
#define	KEOP_GT	291
#define	KEOP_HIGH	292
#define	KEOP_LE	293
#define	KEOP_LOW	294
#define	KEOP_LT	295
#define	KEOP_MOD	296
#define	KEOP_MUN	297
#define	KEOP_NE	298
#define	KEOP_NOT	299
#define	KEOP_OR	300
#define	KEOP_SHL	301
#define	KEOP_SHR	302
#define	KEOP_XOR	303
#define	KEOP_locctr	304
#define	LABEL	305
#define	STRING	306
#define	SYMBOL	307
#define	KTK_invalid	308


extern YYSTYPE yylval;
