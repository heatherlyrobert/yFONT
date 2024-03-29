/*============================----end-of-source---============================*/


/*===[[ DESCRIPTION ]]========================================================*/

/*   yFONT is a simple and clean set of custom library functions to provide
 *   fast, flexible, and aesthetic display of 3D manipulatible text in
 *   opengl applications which is both hard and rare.  its purpose is to
 *   transform a troublesome, involved, and painful task into a simple,
 *   repeatable, and predicable one.
 *
 *   the fonts used for yFONT are a special method particularly adapted to the
 *   strengths of opengl.  this library will use textures to store and display
 *   font glyphs which can then be rapidly manipulated in 2D and 3D to create
 *   stunning displays using hardware acceleration.  to aid performance, glyphs
 *   for a particular font are stored in a single texture as a texture atlas.
 *
 *   a key and valuable design decision is to decouple texture creation and
 *   texture-mapped font usage enabling the opengl application to be independent
 *   of any font rendering libraries or particular windowing system.
 *
 *   to accomplish these features, yFONT is actually a group of tools...
 *      - txf_make to create txf texture font files from ttf fonts
 *      - txf_show to inspect existing txf texture files
 *      - txf_inst to automate the creation of common "txf" files
 *      - yFONT to merge with c/opengl programs to display the actual text
 *
 *   yFONT will specifically handle the following...
 *      - load a prepared texture-mapped font
 *      - display a c-string of letters at varying font sizes
 *      - allow different alignments on the text (left, center, middle, ...)
 *      - provide simple word wrapping ability
 *      - unload a font
 *
 *   yFONT will NOT handle specialty needs like that are less common...
 *      - kerning specific letter pairs (its about speed, not accuracy)
 *      - curved and fitted text (better handled by programs)
 *      - alteration to bold, italic, underline, (separate files)
 *      - extended and wide-characters beyond the basic 256 (who cares)
 *
 *   yFONT has the following restrictions...
 *      - any one program can not use more than 10 fonts
 *
 *   AS ALWAYS, there are many stable, accepted, existing programs that have
 *   been built by better programmers and are likely superior in speed, size,
 *   capability, and reliability; BUT, i would not have learned nearly as much
 *   using them, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   THE UNDERLYING IDEA is to use this build process to learn new programming
 *   capabilities and put additional pressure on me to improve my standards,
 *   maintenance, and coding environment.  just using programs and ideas does
 *   not lead to understanding -- so get in there and get messy ;)
 *
 */
/*===[[ DECISION ]]===========================================================*

 *   SITUATION (S)
 *      graphics programming, whether simple display or complex visualization,
 *      is typically dependent on presenting explanitory text in various
 *      two and three dimensional contexts and orientations.
 *
 *   COMPLICATION (C)
 *      while critical, font and text handling is very dependent on the
 *      underlying windowing system and therefore in not included in the opengl
 *      specification and left to other outside libraries.  this makes programs
 *      very dependent on font libraries and rendering software which can
 *      change, break, or disappear.
 *
 *   QUESTION (Q)
 *      so, how are we going to fulfill this critical need in a way that is
 *      repeatable, sustainable, and accessible.
 *
 *   ANSWER (A)
 *      we will unhook real-time opengl font rendering from font applications
 *      and libraries.  this means creating an intermediate font version in
 *      batch and then using our own library to accomplish real-time display.
 *
 *
 *   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 *   PROBLEM (pr)
 *      how will we extend opengl to handle the presentation of text in both
 *      two and three dimensional contexts and orientations.
 *
 *   OBJECTIVES (o)
 *      - simple programming interface for ease of use
 *      - highly and quickly maintainable (even if out of general support)
 *      - quick processing to support complex applications (allows pre-work)
 *      - very high learning component and environment
 *      - non-commercial or proprietary so that we don't lose it over time
 *      - very specific to fonts and text rather than huge framework
 *      - covers a majority of end-uses without bloat and cruft
 *
 *   ALTERNATIVES (a) and CONSEQUENCES (c)
 *
 *      1) full-featured framework, such as, QT or GTK
 *         - enables and supports the programming, sparing the pain
 *         - already cross platform available and consistent
 *         - requires a massive framework, difficult to just use fonts
 *         - will always need the libraries, near zero learning
 *
 *      2) font-specific library, such as FTGL, GLTT, GLUT, or other
 *         - ready to go out of the box with lower learning curve
 *         - dependent on continuing support, many have discontinued
 *         - very little learning involved
 *         - will always need the libraries on each machine/platform
 *
 *      3) custom-build library for us
 *         - a great deal of up front work and programming
 *         - will have to make trade-offs and compromises to keep simple
 *         - will always be able to use and maintain
 *         - huge learning potential environment
 *
 *   DECISION
 *      while its nuts, i have to go with creating a custom library as i can
 *      always fall back on FTGL and QT when the situation demands it.  i have
 *      no idea how i can ever match the quality of existing libraries, but i
 *      gotta try
 *
 */
/*===[[ STATISTICS ]]=========================================================*

 *   i also like to look at how our code compares to other options...
 *
 *   yFONT         :  15k (including all source code)
 *
 *   FTGL library  : 691k (lot more functionality, but ...)
 *
 */
/*===[[ REUSE ]]==============================================================*

 * concept for this code is based on the following work...
 *
 * Copyright (c) Mark J. Kilgard, 1997.
 *
 * This program is freely distributable without licensing fees  and is
 * provided without guarantee or warrantee expressed or  implied. This
 * program is -not- in the public domain.
 *
 */
/*============================================================================*/



/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YFONT
#define YFONT loaded


typedef unsigned char uchar;
/*
 * alignment macros... which are all relative to the current position...
 *
 * vertical
 *    BAS : baseline
 *    MID : midline of ascent
 *    TOP : top of highest letter
 *    BOT : bottom of lowest descender
 *
 * horizontal
 *    LEF : left
 *    CEN : centered
 *    RIG : right
 *
 *  left  cen  right
 *
 *     1   2   3    -- top
 *     4   5   6    -- middle
 *     7   8   9    -- baselinne
 *    10  11  12    -- full bottom
 */

#define    YF_ORIGIN         0

#define    YF_TOPLEF         1
#define    YF_TOPCEN         2
#define    YF_TOPRIG         3
#define    YF_MIDLEF         4
#define    YF_MIDCEN         5
#define    YF_MIDRIG         6
#define    YF_BASLEF         7
#define    YF_BASCEN         8
#define    YF_BASRIG         9
#define    YF_BOTLEF        10
#define    YF_BOTCEN        11
#define    YF_BOTRIG        12

#define    YF_ORILEF        13
#define    YF_ORICEN        14
#define    YF_ORIRIG        15
#define    YF_TOPORI        16
#define    YF_MIDORI        17
#define    YF_BASORI        18
#define    YF_BOTORI        19

#define    YF_GREGG         20


#define    YF_BRN_TINT      10
#define    YF_RED_TINT      11
#define    YF_ORA_TINT      12
#define    YF_YEL_TINT      13
#define    YF_GRN_TINT      14
#define    YF_BLU_TINT      15
#define    YF_PUR_TINT      16
#define    YF_CRI_TINT      17
#define    YF_GRY_TINT      18
#define    YF_WHI_TINT      19

#define    YF_BRN_FULL      20
#define    YF_RED_FULL      21
#define    YF_ORA_FULL      22
#define    YF_YEL_FULL      23
#define    YF_GRN_FULL      24
#define    YF_BLU_FULL      25
#define    YF_PUR_FULL      26
#define    YF_CRI_FULL      27
#define    YF_GRY_FULL      28
#define    YF_WHI_FULL      29


/*===[[ PROTOTYPES ]]=========================================================*/

char     *yFONT_version      (void);

char      yFONT_load         (char *a_name);
char      yFONT_free         (char  a_slot);

int       yFONT_print        (char  a_slot, char  a_point, char  a_align, uchar *a_text);
int       yFONT_printw       (char  a_slot, char  a_point, char  a_align, uchar *a_text , int  a_wide, int  a_tall, float  a_spacing);
int       yFONT_printu       (char  a_slot, char  a_point, char  a_align, int   *a_array, int  a_max);
float     yFONT_width        (char  a_slot, char  a_point);

int       yFONT_symload      (char *a_filename, int a_ncol, int a_nrow, char a_type);
int       yFONT_symbol       (float a_scale   , int a_col , int a_row , int a_mod);

int       yFONT_iconload     (void);
int       yFONT_icon         (char *a_cat, char *a_name, int a_side, char a_mod);
int       yFONT_iconno       (int a_icon, int a_side, char a_mod);

/*---(external building)-----------------*/
char      yFONT_head_force        (char *a_slot, char *a_name, char a_point, short a_twide, short a_ttall, short a_ascent, short a_descent, char a_margin, short a_glyphs);
char      yFONT_index_force       (char a_slot, char a_gwide, char a_gtall);
uchar*    yFONT_map_force         (char a_slot);
char      yFONT_write_force       (char a_slot);


#define    YF_NO_SLOTS         -1
#define    YF_BAD_HEAD         -2
#define    YF_MEM_FULL         -3
#define    YF_PREMATURE_END    -4
#define    YF_BAD_SLOT         -5

#endif

/*============================----end-of-source---============================*/
