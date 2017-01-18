/*============================----end-of-source---============================*/

/*===[[ HEADER ]]=============================================================*/

/*   category      : (l)  library
 *   focus         : (--) visualization
 *   application   : yFONT
 *   purpose       : fast, clean, and aesthetic texture-mapped fonts for opengl
 *
 *   base_system   : gnu/linux   (powerful, ubiquitous, technical, and hackable)
 *   lang_name     : ansi-c      (wicked, limitless, universal, and everlasting)
 *   dependencies  : none
 *   size          : small       (less than 2,000 slocL)
 * 
 *   author        : rsheatherly
 *   created       : 2010-08
 *   priorities    : direct, simple, brief, vigorous, and lucid (h.w. fowler)
 *   end goal      : loosely coupled, strict interface, maintainable, portable
 * 
 */

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
 */

#define    YF_BASLEF         0
#define    YF_BASCEN         1
#define    YF_BASRIG         2
#define    YF_MIDLEF         3
#define    YF_MIDCEN         4
#define    YF_MIDRIG         5
#define    YF_TOPLEF         6
#define    YF_TOPCEN         7
#define    YF_TOPRIG         8
#define    YF_BOTLEF         9
#define    YF_BOTCEN        10
#define    YF_BOTRIG        11


/*===[[ PROTOTYPES ]]=========================================================*/

char     *yFONT_version      (void);

int       yFONT_load         (char *a_name);
char      yFONT_free         (int   a_slot);

int       yFONT_uprint       (int  a_slot, int a_point, char a_align, unsigned int *a_array, int a_max);
int       yFONT_print        (int   a_slot, int a_point, char a_align, char *a_str);
int       yFONT_printw       (int   a_slot, int a_point, char a_align, char *a_str, int a_width, int a_height, float a_wrap);

int       yFONT_symload      (char *a_name);
int       yFONT_symbol       (float a_scale, int  a_row, int a_col, int a_mod);


#define    YF_NO_SLOTS         -1
#define    YF_BAD_HEAD         -2
#define    YF_MEM_FULL         -3
#define    YF_PREMATURE_END    -4
#define    YF_BAD_SLOT         -5

#endif

/*============================----end-of-source---============================*/
