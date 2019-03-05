/*============================----beg-of-source---============================*/

/*===[[ START HDOC ]]=========================================================*/
/*===[[ HEADER ]]=============================================================*

 *   focus         : visualize
 *   niche         : font_text
 *   application   : txf_make
 *   purpose       : builds sweet texture-mapped fonts for opengl applications
 *   base_system   : gnu/linux
 *   lang_name     : gnu/ansi-c (because it's right, just, best, and universal)
 *   created       : 2010-08
 *   author        : the_heatherlys
 *   dependencies  : xlib, glx
 * 
 */
/*===[[ PURPOSE ]]============================================================*

 *   txf_make is a clean, simple tool to create standard texture-mapped font
 *   files for use in opengl applications to display fast, 3d manipulatable
 *   text.
 *
 *   the original txf font method was developed by mark kilgard in 1997 and
 *   remains a proven and effective technique.  but, the original programs...
 *      - are based on x-windows for font access (rough and hard)
 *      - produce non-antialiased images and so got chunky/broken up
 *      - are built for older, more memory limited graphics hardware
 *      - required textures to by in a power-of-2 format
 *      - use space saving algorithms that are quite complex
 *      - didn't like glyphs that have negative horizontal offsets
 *      - allowed quite a bit of user tweaking and options
 *
 *   so, we will be updating the code thirteen years later to...
 *      - use the freetype library to find glyphs and generate bitmaps
 *      - write glyph images in 256 shades of intensity with antialiasing
 *      - simplify the font texture to have all characters in horizontal lines
 *      - dynamically calculate the texture size (non power of two allowed)
 *      - takes a reasonable stance on some settings rather than user input
 *
 *   defaults will be...
 *      - 60pt fonts as that makes it appoximately 1" high (with normal spacing)
 *      - byte format (0-255) to get clean looking fonts
 *      - 5 pixel gaps between characters to keep textures from bleeding
 *      - fully calculated widths to avoid waste
 *      - a space will be the same size an a lower case 'a'
 *
 *   there are many, stable, accepted, existing programs and utilities have
 *   been built by better programmers and are likely superior in speed, size,
 *   capability, and reliability; BUT, i would not have learned nearly as much
 *   using them, so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the underlying idea is to use this build process to learn new programming
 *   capabilities and put additional pressure on me to improve my standards,
 *   maintenance, and coding environment.  just using programs and ideas does
 *   not lead to understanding -- so get in there and get messy ;)
 *
 *   the central enemy of reliability is complexity - daniel geer
 *   simplicity is prerequisite for reliability - edsger dijkstra
 */
/*===[[ REUSE ]]==============================================================*

 * this code is based on the following work...
 *
 * Copyright (c) Mark J. Kilgard, 1997.
 *
 * This program is freely distributable without licensing fees  and is
 * provided without guarantee or warrantee expressed or  implied. This
 * program is -not- in the public domain.
 *
 */
/*===((END DOC))==============================================================*/




/*
 *
 *  special chars mapped to match my shrike font assignments
 *
 *  return
 *  escape
 *  space dot
 *  tab
 *  backspace
 *  field
 *  broken bar
 *  placeholder
 *  null
 *  wait/pause
 *  halt
 *
 *
 *
 *
 *
 *
 */

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <math.h>

#include    "yFONT.h"
#include    "yFONT_priv.h"




#define     SRC_DIR     "/home/shared/fonts"
#define     SRC_FILE    "comfortaa_regular.ttf"

#define     DST_DIR     "/usr/local/share/fonts"
#define     DST_EXT     "txf"

char        FONT__init         (void);
char        FONT__choose       (char a_slot);


#define      MAX_CHARS    500
typedef struct cCHARS  tCHARS;
struct cCHARS {
   int         mycode;
   int         unicode;
   char        name      [20];
   char        use       [20];   /* t=tsae, n=norm, c=code, s=serious   */
};
tCHARS   s_chars   [MAX_CHARS] = {
   /*---(upper latin)--------------------*/
   { 'A' , 'A'   , "A"          , "-ncs------"},
   { 'B' , 'B'   , "B"          , "-ncs------"},
   { 'C' , 'C'   , "C"          , "-ncs------"},
   { 'D' , 'D'   , "D"          , "tncs------"},
   { 'E' , 'E'   , "E"          , "-ncs------"},
   { 'F' , 'F'   , "F"          , "-ncs------"},
   { 'G' , 'G'   , "G"          , "tncs------"},
   { 'H' , 'H'   , "H"          , "-ncs------"},
   { 'I' , 'I'   , "I"          , "-ncs------"},
   { 'J' , 'J'   , "J"          , "-ncs------"},
   { 'K' , 'K'   , "K"          , "tncs------"},
   { 'L' , 'L'   , "L"          , "-ncs------"},
   { 'M' , 'M'   , "M"          , "-ncs------"},
   { 'N' , 'N'   , "N"          , "tncs------"},
   { 'O' , 'O'   , "O"          , "-ncs------"},
   { 'P' , 'P'   , "P"          , "-ncs------"},
   { 'Q' , 'Q'   , "Q"          , "tncs------"},
   { 'R' , 'R'   , "R"          , "-ncs------"},
   { 'S' , 'S'   , "S"          , "-ncs------"},
   { 'T' , 'T'   , "T"          , "tncs------"},
   { 'U' , 'U'   , "U"          , "-ncs------"},
   { 'V' , 'V'   , "V"          , "-ncs------"},
   { 'W' , 'W'   , "W"          , "-ncs------"},
   { 'X' , 'X'   , "X"          , "-ncs------"},
   { 'Y' , 'Y'   , "Y"          , "-ncs------"},
   { 'Z' , 'Z'   , "Z"          , "-ncs------"},
   /*---(lower latin)--------------------*/
   { 'a' , 'a'   , "a"          , "tncs------"},
   { 'b' , 'b'   , "b"          , "-ncs------"},
   { 'c' , 'c'   , "c"          , "tncs------"},
   { 'd' , 'd'   , "d"          , "tncs------"},
   { 'e' , 'e'   , "e"          , "tncs------"},
   { 'f' , 'f'   , "f"          , "tncs------"},
   { 'g' , 'g'   , "g"          , "tncs------"},
   { 'h' , 'h'   , "h"          , "tncs------"},
   { 'i' , 'i'   , "i"          , "tncs------"},
   { 'j' , 'j'   , "j"          , "tncs------"},
   { 'k' , 'k'   , "k"          , "tncs------"},
   { 'l' , 'l'   , "l"          , "tncs------"},
   { 'm' , 'm'   , "m"          , "tncs------"},
   { 'n' , 'n'   , "n"          , "tncs------"},
   { 'o' , 'o'   , "o"          , "tncs------"},
   { 'p' , 'p'   , "p"          , "tncs------"},
   { 'q' , 'q'   , "q"          , "tncs------"},
   { 'r' , 'r'   , "r"          , "tncs------"},
   { 's' , 's'   , "s"          , "tncs------"},
   { 't' , 't'   , "t"          , "tncs------"},
   { 'u' , 'u'   , "u"          , "tncs------"},
   { 'v' , 'v'   , "v"          , "tncs------"},
   { 'w' , 'w'   , "w"          , "tncs------"},
   { 'x' , 'x'   , "x"          , "tncs------"},
   { 'y' , 'y'   , "y"          , "tncs------"},
   { 'z' , 'z'   , "z"          , "tncs------"},
   /*---(punctuation)--------------------*/
   { ' ' , ' '   , " "          , "tncs------"},
   { '!' , '!'   , "!"          , "tncs------"},
   { '"' , '"'   , "\""         , "tncs------"},
   { '#' , '#'   , "#"          , "tncs------"},
   { '$' , '$'   , "$"          , "tncs------"},
   { '%' , '%'   , "%"          , "tncs------"},
   { '&' , '&'   , "&"          , "tncs------"},
   { '\'', '\''  , "'"          , "tncs------"},
   { '(' , '('   , "("          , "tncs------"},
   { ')' , ')'   , ")"          , "tncs------"},
   { '*' , '*'   , "*"          , "-ncs------"},
   { '+' , '+'   , "+"          , "-ncs------"},
   { ',' , ','   , ","          , "tncs------"},
   { '-' , '-'   , "-"          , "tncs------"},
   { '.' , '.'   , "."          , "tncs------"},
   { '/' , '/'   , "/"          , "-ncs------"},
   /*---(numbers)------------------------*/
   { '0' , '0'   , "0"          , "tncs------"},
   { '1' , '1'   , "1"          , "tncs------"},
   { '2' , '2'   , "2"          , "tncs------"},
   { '3' , '3'   , "3"          , "tncs------"},
   { '4' , '4'   , "4"          , "tncs------"},
   { '5' , '5'   , "5"          , "tncs------"},
   { '6' , '6'   , "6"          , "tncs------"},
   { '7' , '7'   , "7"          , "tncs------"},
   { '8' , '8'   , "8"          , "tncs------"},
   { '9' , '9'   , "9"          , "tncs------"},
   /*---(punctuation)--------------------*/
   { ':' , ':'   , ":"          , "-ncs------"},
   { ';' , ';'   , ";"          , "-ncs------"},
   { '<' , '<'   , "<"          , "-ncs------"},
   { '=' , '='   , "="          , "-ncs------"},
   { '>' , '>'   , ">"          , "-ncs------"},
   { '?' , '?'   , "?"          , "-ncs------"},
   { '@' , '@'   , "@"          , "-ncs------"},
   { '[' , '['   , "["          , "-ncs------"},
   { '\\', '\\'  , "\\"         , "-ncs------"},
   { ']' , ']'   , "]"          , "-ncs------"},
   { '^' , '^'   , "^"          , "tncs------"},
   { '_' , '_'   , "_"          , "-ncs------"},
   { '`' , '`'   , "`"          , "-ncs------"},
   { '{' , '{'   , "{"          , "-ncs------"},
   { '|' , '|'   , "|"          , "-ncs------"},
   { '}' , '}'   , "}"          , "-ncs------"},
   { '~' , '~'   , "~"          , "-ncs------"},
   /*---(upper greek)--------------------*/
   {  128, 0x0391, "ALPHA"      , "---s------"},
   {  129, 0x0392, "BETA"       , "---s------"},
   {  130, 0x0393, "GAMMA"      , "---s------"},
   {  131, 0x0394, "DELTA"      , "---s------"},
   {  132, 0x0395, "EPSILON"    , "---s------"},
   {  133, 0x0396, "ZETA"       , "---s------"},
   {  134, 0x0397, "ETA"        , "---s------"},
   {  135, 0x0398, "THETA"      , "---s------"},
   {  136, 0x0399, "IOTA"       , "---s------"},
   {  137, 0x039a, "KAPPA"      , "---s------"},
   {  138, 0x039b, "LAMBDA"     , "---s------"},
   {  139, 0x039c, "MU"         , "---s------"},
   {  140, 0x039d, "NU"         , "---s------"},
   {  141, 0x039e, "XI"         , "---s------"},
   {  142, 0x039f, "OMICRON"    , "---s------"},
   {  143, 0x03a0, "PI"         , "---s------"},
   {  144, 0x03a1, "RHO"        , "---s------"},
   {  145, 0x03a3, "SIGMA"      , "---s------"},
   {  146, 0x03a4, "TAU"        , "---s------"},
   {  147, 0x03a5, "UPSILON"    , "---s------"},
   {  148, 0x03a6, "PHI"        , "---s------"},
   {  149, 0x03a7, "CHI"        , "---s------"},
   {  150, 0x03a8, "PSI"        , "---s------"},
   {  151, 0x03a9, "OMEGA"      , "---s------"},
   /*---(typical latin)------------------*/
   {  163,    216, "null"       , "--cs------"},
   {  164,    164, "placeholder", "--cs------"},
   {  165,    165, "escape"     , "--cs------"},
   {  166,    182, "return"     , "--cs------"},
   {  167,    167, "field"      , "--cs------"},
   {  168,    208, "display"    , "--cs------"},
   {  169,    177, "wait"       , "--cs------"},
   {  170,    171, "bs"         , "--cs------"},
   {  171,    187, "tab"        , "--cs------"},
   {  185,    183, "dot"        , "--cs------"},
   /*---(lower greek)--------------------*/
   {  232, 0x03b1, "alpha"      , "---s------"},
   {  233, 0x03b2, "beta"       , "---s------"},
   {  234, 0x03b3, "gamma"      , "---s------"},
   {  235, 0x03b4, "delta"      , "---s------"},
   {  236, 0x03b5, "epsilon"    , "---s------"},
   {  237, 0x03b6, "zeta"       , "---s------"},
   {  238, 0x03b7, "eta"        , "---s------"},
   {  239, 0x03b8, "theta"      , "---s------"},
   {  240, 0x03b9, "iota"       , "---s------"},
   {  241, 0x03ba, "kappa"      , "---s------"},
   {  242, 0x03bb, "lambda"     , "---s------"},
   {  243, 0x03bc, "mu"         , "---s------"},
   {  244, 0x03bd, "nu"         , "---s------"},
   {  245, 0x03be, "xi"         , "---s------"},
   {  246, 0x03bf, "omicron"    , "---s------"},
   {  247, 0x03c0, "pi"         , "---s------"},
   {  248, 0x03c1, "rho"        , "---s------"},
   {  249, 0x03c3, "sigma"      , "---s------"},
   {  250, 0x03c4, "tau"        , "---s------"},
   {  251, 0x03c5, "upsilon"    , "---s------"},
   {  252, 0x03c6, "phi"        , "---s------"},
   {  253, 0x03c7, "chi"        , "---s------"},
   {  254, 0x03c8, "psi"        , "---s------"},
   {  255, 0x03c9, "omega"      , "---s------"},
   /*---(done)---------------------------*/
   {    0, 0x0000, ""           , "----------"},
};

/*===[[ FREETYPE2 ]]==========================================================*/
#include    <ft2build.h>
#include    FT_FREETYPE_H


/*---(freetype structures)----------------------*/


/*---(program globals)--------------------------*/
#define   MAX_GLYPHS   3000
#define   MAX_TLISTS     20
#define   MAX_TGLYPHS   300


typedef  struct cGLIST tGLIST;
struct cGLIST {
   int         mycode;                 /* my code based on shrike font        */
   int         unicode;                /* unicode of character                */
   char        reason;                 /* reason for character inclusion      */
   int         final;                  /* unicode of final character entry    */
   int         coords [12];            /* coordinates if modified             */
};
static tGLIST  s_glist     [MAX_GLYPHS];
static int     s_nglist    = 0;

typedef struct cACCESSOR  tACCESSOR;
struct  cACCESSOR {
   /*---(files)--------------------------*/
   char        font_name  [LEN_STR];     /* font name from command line         */
   char        out_name   [LEN_STR];     /* name of output txf font name        */
   char        out_file   [LEN_STR];     /* name of output txf font file        */
   char        src_file   [LEN_STR];     /* name of source ttf font file        */
   /*---(characteristics)----------------*/
   char        type       [LEN_LABEL];   /* type of source (font vs grid)       */
   char        point;                    /* point size of font                  */
   char        adjust;                   /* actual point size (for adjustment)  */
   int         spacer;                   /* unicode of space character          */
   int         margin;                   /* character margin added              */
   int         texw;                     /* width of texture                    */
   /*---(freetype)-----------------------*/
   FT_Library  lib;
   FT_Face     face;                     /* freetype interface variable         */
   tGLYPH      glyph;                    /* current glyph                       */
   /*---(texture)------------------------*/
   FILE     *file;                     /* pointer to txf file                 */
   FILE       *o;
   char      glist       [LEN_LABEL];
   char      byte;                     /* texture format (bit or byte)        */
   int       nglyphs;                  /* number of glyphs included           */
   int       row;                      /* size of a row                       */
   uchar    *texture;                  /* font texture map                    */
   tGLYPH   *glyph_table;              /* all glyphs                          */
};
tACCESSOR      my;


/*> " !\"#0%&'0)*+0-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~ "   <*/

struct {
   char        name        [  10];
   char        abbr;
   char        col;
   char        code        [MAX_TGLYPHS];
} s_glist_info [MAX_TLISTS] = {
   { "tsae"    , 't', 0, " DGKNQTcdghklmnpqstwxyzaefiouvrj0123456789!@#$%^&.,:\"\'()-"                                          },
   { "norm"    , 'n', 1, " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~"   },
   { "code"    , 'c', 2, " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~"   },
   { "serious" , 's', 3, " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~"   },
};

/*> int format = YFONT_BYTE;                                                          <*/


/*> int         s_limit = 30 * 256;                                                   <*/
int         s_limit =  (12 * 256) - 1;

void       /* PURPOSE : trim junk from the front and back of a string --------*/
str_trim           (char *a_cstring)
{
   int i;
   if (a_cstring == NULL) return;
   while (1) {
      if (a_cstring[0] == ' ') {
         for (i = 0; i <= (int) strlen(a_cstring); ++i)
            a_cstring[i] = a_cstring[i + 1];
      }
      else                     break;
   }
   for (i = strlen(a_cstring) - 1; i >= 0; --i) {
      if (a_cstring[i] == ' ') a_cstring[i] = '\0';
      else                     break;
   }
   return;
}



/*====================------------------------------------====================*/
/*===----                   standard program functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char         /*--: pre-argument program initialization ---[ leaf   [ ------ ]-*/
PROG_init          (void)
{
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(flags)--------------------------*/
   DEBUG_TOPS   yLOG_note    ("intialize program status flags");
   /*---(fonts and files)----------------*/
   DEBUG_TOPS   yLOG_note    ("intialize font and file names");
   FONT__init    ();
   /*---(defaults)-----------------------*/
   DEBUG_TOPS   yLOG_note    ("intialize global settings");
   my.byte   = YFONT_BYTE;  /* always byte version */
   my.point  = -1;
   my.adjust = -1;
   my.margin =  2;
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: argument processing -------------------[ ------ [ ------ ]-*/
PROG_args     (int argc, char *argv[])
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(locals)--------------------------------*/
   char     *a         = NULL;               /* current argument              */
   int       i         = 0;                  /* loop iterator                 */
   int       useError  = 0;
   char      rc        = 0;
   char     *p         = NULL;
   int       l         = 0;
   /*---(process arguments)---------------------*/
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (a[0] == '@')  continue;
      DEBUG_ARGS  yLOG_info  ("argument"  , a);
      strlcpy (my.font_name, argv[i], LEN_LABEL);
   }
   /*---(updates)-------------------------------*/
   DEBUG_PROG   yLOG_info    ("font_name" , my.font_name);
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        choosing fonts                        ----===*/
/*====================------------------------------------====================*/
static void      o___FONTS___________________o (void) {;}

char
FONT__init         (void)
{
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   strlcpy (my.font_name , "", LEN_LABEL);
   strlcpy (my.out_name  , "", LEN_LABEL);
   strlcpy (my.out_file  , "", LEN_LABEL);
   strlcpy (my.src_file  , "", LEN_LABEL);
   return 0;
}

char
FONT__choose       (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          =   0;
   char       *p           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT  yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   DEBUG_YFONT  yLOG_info    ("font_name" , my.font_name);
   /*---(check for all fonts)------------*/
   if      (strcmp (my.font_name, "all") == 0) {
      DEBUG_YFONT  yLOG_note    ("program in ALL font mode");
      /*---(check for first)-------------*/
      if (strcmp (my.out_name, "") == 0) {
         DEBUG_YFONT  yLOG_note    ("first font to be chosen");
         rc = yFONT__conf_head (my.out_name, my.type, &my.point, &my.adjust, &my.spacer, my.glist, my.src_file);
         DEBUG_YFONT  yLOG_value   ("rc"        , rc);
         if (rc < 0) {
            DEBUG_YFONT  yLOG_note    ("no fonts found in configuration");
            DEBUG_YFONT  yLOG_exit    (__FUNCTION__);
            return rc;
         }
      } 
      /*---(check for next)--------------*/
      else {
         DEBUG_YFONT  yLOG_note    ("next font to be chosen");
         rc = yFONT__conf_next (my.out_name, my.type, &my.point, &my.adjust, &my.spacer, my.glist, my.src_file);
         DEBUG_YFONT  yLOG_value   ("rc"        , rc);
         if (rc < 0) {
            DEBUG_YFONT  yLOG_note    ("no more fonts available");
            DEBUG_YFONT  yLOG_exit    (__FUNCTION__);
            return rc;
         }
      }
      /*---(done)------------------------*/
   }
   /*---(check for specific font)--------*/
   else {
      DEBUG_YFONT  yLOG_note    ("program in SPECIFIC font mode");
      if (strcmp (my.out_name, "") != 0) {
         DEBUG_YFONT  yLOG_note    ("already run on this font");
         DEBUG_YFONT  yLOG_exit    (__FUNCTION__);
         return rce;
      }
      rc = yFONT__conf_info (my.font_name, my.type, &my.point, &my.adjust, &my.spacer, my.glist, my.src_file);
      DEBUG_YFONT  yLOG_value   ("rc"        , rc);
      if (rc < 0) {
         DEBUG_YFONT  yLOG_note    ("font not found in configuration");
         DEBUG_YFONT  yLOG_exit    (__FUNCTION__);
         return rc;
      }
      strlcpy  (my.out_name, my.font_name, LEN_LABEL);
   }
   /*---(setup output file)--------------*/
   sprintf (my.out_file, "%s/%s.%s", DST_DIR, my.out_name, DST_EXT);
   DEBUG_PROG   yLOG_info    ("out_file"  , my.out_file);
   /*---(update header)------------------*/
   yFONT__head_name    (a_slot, my.out_name, my.point);
   yFONT__head_margin  (a_slot, my.margin);
   /*---(complete)-----------------------*/
   DEBUG_YFONT  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                        glyph lists                           ----===*/
/*====================------------------------------------====================*/
static void      o___GLISTS__________________o (void) {;}

char
GLIST__clear         (void)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =   0;
   int         j           =   0;
   /*---(null list)----------------------*/
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   for (i = 0; i < MAX_GLYPHS; ++i) {
      s_glist [i].mycode  = -1;
      s_glist [i].unicode = -1;
      s_glist [i].reason  = '-';
      s_glist [i].final   = -1;
      for (j = 0; j < 12; ++j)  s_glist [i].coords [j] = 0;
   }
   s_nglist = 0;
   return 0;
}

char
GLIST__normal        (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   int         j           =   0;
   char        x_col       =   0;
   /*---(defense)------------------------*/
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   if (a_list == NULL)  return -1;
   /*---(load)---------------------------*/
   for (i = 0; i < MAX_CHARS; ++i) {
      /*---(filter)----------------------*/
      if (s_glist_info [i].name [0] != a_list [0])  continue;
      if (strcmp (s_glist_info [i].name, a_list) != 0)  continue;
      printf ("found %s, abbr %c, col %d\n", s_glist_info [i].name,
           s_glist_info [i].abbr, s_glist_info [i].col);
      x_col = s_glist_info [i].col;
      /*---(load special)----------------*/
      for (j = 0; j < MAX_TGLYPHS; ++j) {
         if (s_chars [j].mycode   == 0  )        break;
         if (s_chars [j].use [x_col] == '-')     continue;
         s_glist [s_nglist].mycode  = s_chars [j].mycode;
         s_glist [s_nglist].unicode = s_chars [j].unicode;
         s_glist [s_nglist].reason  = 'f';
         ++s_nglist;
      }
      /*---(exit)------------------------*/
      return 0;
   }
   /*---(complete)-----------------------*/
   return -1;
}

char
GLIST__mandarin    (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   int         j           = 0;
   char        loaded      = '-';
   char        r           [1000] = "";      /* record from stdin                   */
   FILE       *f           = NULL;
   char       *p           = NULL;          /* strtok pointer                 */
   char       *x_coords    = NULL;          /* strtok pointer                 */
   char       *q           = "\x1F";        /* strtok delimiter               */
   /*---(mandarin)-----------------------*/
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   if (strcmp ("mand" , a_list) != 0) {
      return -1;
   }
   f = fopen ("/home/member/mand_new.txt" , "r");
   if (f == NULL)  return -1;
   my.o = fopen ("/home/member/mand_out.txt" , "w");
   while (1) {
      /*---(read basics)--------------*/
      fgets (r, 1000, f);
      if (feof (f))  break;
      if (r[0] == '#' ) continue;
      if (r[0] == ' ' ) continue;
      if (r[0] == '\0') continue;
      p = strtok (r, q);
      if (p == NULL)  continue;
      str_trim (p);
      s_glist [s_nglist].unicode = atoi (p);
      fprintf (my.o, "%d\n", s_glist [s_nglist].unicode);
      p = strtok (NULL  , q);
      if (p == NULL)  continue;
      str_trim (p);
      s_glist [s_nglist].reason  = p[0];
      fprintf (my.o, "   %c\n", s_glist [s_nglist].reason);
      /*---(handle mods)--------------*/
      if (strchr ("mM", s_glist[s_nglist].reason) != NULL) {
         p = strtok (NULL  , q);
         if (p == NULL)  continue;
         s_glist [s_nglist].final   = s_glist [s_nglist].unicode;
         s_glist [s_nglist].unicode = atoi (p);
         for (i = 4; i < 18; ++i) {
            p = strtok (NULL  , q);
            if (p == NULL)  continue;
         }
         if (i != 18) continue;
         p = strtok (NULL  , q);
         if (p == NULL)  continue;
         str_trim (p);
         fprintf (my.o, "   %s\n", p);
         x_coords = p;
         p = strtok (x_coords, ",");
         for (i = 0; i < 12; ++i) {
            if (p == NULL) break;
            str_trim (p);
            s_glist [s_nglist].coords [i] = atoi (p);
            fprintf (my.o, "   %d\n", s_glist [s_nglist].coords[i]);
            p = strtok (NULL  , ",");
         }
         if (i < 12)  continue;
      }
      /*---(done)---------------------*/
      ++s_nglist;
   }
   fclose (f);
   return 0;
   /*---(complete)-----------------------*/
   return 0;
}

char
GLIST__load        (char a_slot, char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          = 0;
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   if (rc == 0)  rc = GLIST__clear       ();
   if (rc == 0)  rc = GLIST__normal      (a_list);
   if (rc <  0)  rc = GLIST__mandarin    (a_list);
   return rc;
}




/*====================------------------------------------====================*/
/*===----                        freetype access                       ----===*/
/*====================------------------------------------====================*/
static void      o___FREETYPE________________o (void) {;}

char         /*--: setup font for processing -------------[ ------ [ ------ ]-*/
FREETYPE__setup      (char *a_font, int a_point)
{  /*---(design notes)-------------------*/
   /*
    *  only effects the my.lib, my.face variables
    */
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         rc          =   0;      /* simple return code                  */
   char        rce         = -10;      /* generic return code for errors      */
   /*---(initialize freetype)------------*/
   DEBUG_INPT   yLOG_note    ("initialize the freetype library");
   rc = FT_Init_FreeType (&my.lib);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("freetype2 library could not be openned");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("initialization successful");
   /*---(set up font)---------------------------*/
   DEBUG_INPT   yLOG_note    ("load the source font");
   --rce;  if (a_font == NULL) {
      DEBUG_INPT   yLOG_fatal   ("font font name string argument is null");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_font"    , a_font);
   rc = FT_New_Face (my.lib, a_font, 0, &my.face);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("font freetype2 library could not find font");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("font load successful");
   /*---(set up standard font size)-------------*/
   DEBUG_INPT   yLOG_note    ("setup the font size/point");
   DEBUG_INPT   yLOG_value   ("a_point"   , a_point);
   rc = FT_Set_Char_Size (my.face, 0, a_point * 64, 96, 96); /* in 1/64pt units    */
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("font freetype2 library could not set pixel size");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("point size setting successful");
   /*---(complete)------------------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: shutdown a font after processing ------[ ------ [ ------ ]-*/
FREETYPE__shutdown   (void)
{  /*---(design notes)-------------------*/
   /*
    *  only effects the my.lib, my.face variables
    */
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         rc          =   0;      /* simple return code                  */
   char        rce         = -10;      /* generic return code for errors      */
   /*---(close the font)-----------------*/
   DEBUG_INPT   yLOG_note    ("close the source font");
   rc = FT_Done_Face  (my.face);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("font freetype2 library could not close font");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("font unloaded successful");
   /*---(shutdown freetype)--------------*/
   DEBUG_INPT   yLOG_note    ("shutdown the freetype library");
   rc = FT_Done_FreeType (my.lib);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("freetype2 library could not be closed");
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("shutdown successful");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: find a glyph in the font --------------[ ------ [ ------ ]-*/
FREE__find         (uint a_char)
{  /*---(design notes)-------------------*/
   /*
    *  only effects the my.face variable
    */
   /*---(begin)--------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(locals)-----------+-------------*/
   int         rc          = 0;        /* generic return code                 */
   char        rce         = -10;      /* generic return code for errors      */
   /*---(find the glyph)-----------------*/
   DEBUG_INPT   yLOG_sint    (a_char);
   DEBUG_INPT   yLOG_snote   ("find");
   rc = FT_Get_Char_Index (my.face, a_char);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc == 0) {
      DEBUG_INPT   yLOG_warn    ("glyph freetype2 could not find the glyph");
      return rce;
   }
   /*---(pull the glyph)-----------------*/
   DEBUG_INPT   yLOG_snote   ("load");
   rc = FT_Load_Glyph (my.face, rc, FT_LOAD_DEFAULT);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_warn    ("glyph freetype2 could not load the glyph");
      return rce;
   }
   /*---(draw the glyph)-----------------*/
   DEBUG_INPT   yLOG_snote   ("render");
   rc = FT_Render_Glyph (my.face->glyph, FT_RENDER_MODE_NORMAL);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_warn    ("glyph freetype2 could not render the glyph");
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      glyph-focused functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___GLYPH___________________o (void) {;}

char         /*--: gather glyph statistics ---------------[ ------ [ ------ ]-*/
GLYPH__metrics     (void)
{
   float     x_scalex;                   /* convert metrics to bitmap units     */
   float     x_scaley;                   /* convert metrics to bitmap units     */
   /*---(begin)--------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(basics)-------------------------*/
   my.glyph.wide   = my.face->glyph->bitmap.width; /* same as scaled metrics */
   DEBUG_INPT   yLOG_sint    (my.glyph.wide);
   my.glyph.tall   = my.face->glyph->bitmap.rows;  /*           "            */
   DEBUG_INPT   yLOG_sint    (my.glyph.tall);
   /*---(convertions)--------------------*/
   x_scalex       = (float) my.glyph.wide / (float) my.face->glyph->metrics.width;
   x_scaley       = (float) my.glyph.tall / (float) my.face->glyph->metrics.height;
   /*---(usage)--------------------------*/
   my.glyph.xoff   = my.face->glyph->metrics.horiBearingX * x_scalex;
   DEBUG_INPT   yLOG_sint    (my.glyph.xoff);
   my.glyph.yoff   = my.face->glyph->metrics.horiBearingY * x_scaley;
   DEBUG_INPT   yLOG_sint    (my.glyph.yoff);
   my.glyph.adv    = my.face->glyph->metrics.horiAdvance  * x_scalex;
   DEBUG_INPT   yLOG_sint    (my.glyph.adv);
   /*---(complete)--------------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*--: show ascii version -----------s--------[ ------ [ ------ ]-*/
GLYPH__art         (void)
{
   int   i = 0;
   int   j = 0;
   int   x_ascii;
   char  x_chars [20] = " -+:*172%98#@";   /* use symmetrical charaters    */
   uchar x_value;
   printf("\n");
   for (i = 0; i < my.face->glyph->bitmap.rows; ++i) {
      for (j = 0; j < my.face->glyph->bitmap.width; ++j) {
         x_value = my.face->glyph->bitmap.buffer[i * my.face->glyph->bitmap.width + j];
         x_ascii = x_value / (256 / 12);
         if (i % 2) printf("%c", x_chars[x_ascii]);
      }
      if (i % 2) printf("\n");
   }
   printf("\n");
}



/*====================------------------------------------====================*/
/*===----                      index-focused functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___INDEX___________________o (void) {;}

char         /*--: load the character information --------[ ------ [ ------ ]-*/
INDEX__load        (char a_slot)
{
   /*---(working)---------*/
   int         rc          =      0;   /* generic return code                 */
   int         i           =      0;   /* iterator -- gylphs                  */
   int         x_unicode   =      0;   /* glyph unicode                       */
   int         x_mycode    =      0;   /* glyph my shrike code                */
   char        x_good      =    '-';   /* good vs bad                         */
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(measure all glyphs)-------------*/
   for (i = 0; i < s_nglist; ++i) {
      x_good = '-';
      /*---(find glyph)------------------*/
      DEBUG_INPT   yLOG_value   ("CHAR"      , i);
      x_unicode = my.glyph.code = s_glist [i].unicode;
      DEBUG_INPT   yLOG_value   ("unicode"   , x_unicode);
      x_mycode  = s_glist [i].mycode;
      DEBUG_INPT   yLOG_value   ("mycode"    , x_mycode);
      if (x_unicode == 32) {
         x_unicode   = my.spacer;
         x_mycode = my.spacer;
         x_good = 'r';
      }
      DEBUG_INPT   yLOG_value   ("unicode"   , x_unicode);
      rc = FREE__find    (x_unicode);
      /*---(handle misses)---------------*/
      DEBUG_INPT   yLOG_value   ("rc"        , rc);
      if (rc <  0) {
         x_good = '?';
         yFONT__index_code   (a_slot, i, x_mycode, x_good);
         continue;
      }
      /*---(handle success)--------------*/
      if (rc == 0 && x_good == '-')  x_good = 'y';
      GLYPH__metrics ();
      /*---(load index)------------------*/
      x_unicode   = s_glist [i].unicode;
      x_mycode = s_glist [i].mycode;
      yFONT__index_code   (a_slot, i, x_mycode         , x_good);
      yFONT__index_size   (a_slot, i, my.glyph.wide, my.glyph.tall);
      yFONT__index_offset (a_slot, i, my.glyph.xoff, my.glyph.yoff, my.glyph.adv);
      /*---(handle special)--------------*/
      if (strchr ("mM", s_glist [i].reason) != NULL) {
         x_good = 'm';
         yFONT__index_code   (a_slot, i, s_glist [i].final, x_good);
      }
      /*---(done)------------------------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: layout the characters for texture -----[ ------ [ ------ ]-*/
INDEX__layout      (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   int         i           =      0;   /* iterator -- gylphs                  */
   char        x_tall      =      0;   /* greatest character height           */
   char        x_wide      =      0;
   short       x_texw      =      0;
   short       x_texh      =      0;
   int         x_cumw      =      0;   /* current width                       */
   int         x_cumh      =      0;   /* current height                      */
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(initialize)---------------------*/
   x_tall  = yFONT__index_maxes  (a_slot);
   x_cumw  = my.margin + 1;
   x_cumh  = my.margin + 1;
   /*---(measure all glyphs)-------------*/
   for (i = 0; i < s_nglist; ++i) {
      /*---(find glyph)------------------*/
      DEBUG_INPT   yLOG_value   ("CHAR"      , i);
      x_wide  = yFONT__index_wide (a_slot, i);
      DEBUG_INPT   yLOG_value   ("width"     , x_wide);
      /*---(watch width limit)-----------*/
      if ((x_cumw + x_wide + (my.margin * 2) + 2) >= s_limit) {
         DEBUG_INPT   yLOG_note    ("HIT RIGHT side, start a new line");
         x_texw   = s_limit;
         x_cumw   = my.margin + 1;
         x_cumh  += (x_tall + my.margin + 1);
      }
      /*---(totalize)---------------------------*/
      DEBUG_INPT   yLOG_value   ("xpos"      , x_cumw);
      DEBUG_INPT   yLOG_value   ("ypos"      , x_cumh);
      yFONT__index_pos   (a_slot, i, x_cumw, x_cumh);
      x_cumw       += x_wide + (my.margin + 1);
      if (x_texw < s_limit)  x_texw      = x_cumw;
      x_texh      = x_cumh + x_tall + (my.margin + 1);
      /*---(done)------------------------*/
   }
   /*---(save statistics)----------------*/
   DEBUG_INPT   yLOG_value   ("x_texw"    , x_texw);
   DEBUG_INPT   yLOG_value   ("x_texh"    , x_texh);
   x_texw      +=  16 - (x_texw %  16);   /* get a clean mulpilier  */
   x_texh       =  x_cumh + x_tall + (my.margin + 1);
   x_texh      +=  16 - (x_texh %  16);   /* get a clean mulpilier  */
   DEBUG_INPT   yLOG_value   ("x_cumw"    , x_cumw);
   DEBUG_INPT   yLOG_value   ("x_cumh"    , x_cumh);
   DEBUG_INPT   yLOG_value   ("x_texw"    , x_texw);
   DEBUG_INPT   yLOG_value   ("x_texh"    , x_texh);
   yFONT__head_tex   (a_slot, x_texw, x_texh);
   my.texw = x_texw;
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    texture-focused functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___TEXTURE_________________o (void) {;}

char
TEX__glyph           (char a_slot, int a_entry, int a_texw)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =  0;
   int         x_row       =  0;   /* vertical iterator                   */
   int         x_col       =  0;   /* horizontal iterator                 */
   short       x_xpos      =  0;
   short       x_ypos      =  0;
   char        x_tall      =  0;   /* greatest character height           */
   char        x_wide      =  0;
   ulong       x_bitloc    =  0;       /* byte location in source glyph       */
   uchar       x_value     =  0;       /* byte value in source glyph          */
   ulong       x_texloc    =  0;       /* byte location in texture            */
   /*---(coordinates)--------------------*/
   rc = yFONT__index_coords (a_slot, a_entry, &x_xpos, &x_ypos, &x_wide, &x_tall);
   if (rc < 0)  return rc;
   /*> DEBUG_INPT   yLOG_enter   (__FUNCTION__);                                      <*/
   /*---(row-by-row)---------------------*/
   for (x_row = 0; x_row < x_tall; ++x_row) {
      /*---(col-by-col)------------------*/
      for (x_col = 0; x_col < x_wide; ++x_col) {
         /*---(transfer value)-----------*/
         x_bitloc   = (x_row * x_wide) + x_col;
         x_value    = (uchar) my.face->glyph->bitmap.buffer[x_bitloc];
         x_texloc   = ((x_ypos + x_row) * a_texw) + (x_xpos + x_col);
         g_yfont [a_slot]->tex_map [x_texloc] = x_value;
         /*---(done with value)----------*/
      }
      /*---(done with col)---------------*/
   }
   /*---(done with row)------------------*/
   /*> DEBUG_INPT   yLOG_exit    (__FUNCTION__);                                      <*/
   return 0;
}

char
TEX__crop            (char a_slot, int a_entry, int a_texw)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =  0;
   int         x_row       =  0;   /* vertical iterator                   */
   int         x_col       =  0;   /* horizontal iterator                 */
   short       x_xpos      =  0;
   short       x_ypos      =  0;
   char        x_tall      =  0;   /* greatest character height           */
   char        x_wide      =  0;
   char        x_ctop      =  0;
   char        x_clef      =  0;
   char        x_cbot      =  0;   /* greatest character height           */
   char        x_crig      =  0;
   ulong       x_bitloc    =  0;       /* byte location in source glyph       */
   uchar       x_value     =  0;       /* byte value in source glyph          */
   ulong       x_texloc    =  0;       /* byte location in texture            */
   int         x_run       =  0;
   /*---(coordinates)--------------------*/
   rc = yFONT__index_coords (a_slot, a_entry, &x_xpos, &x_ypos, &x_wide, &x_tall);
   if (rc < 0)  return rc;
   /*---(crop header)--------------------*/
   fprintf (my.o, "%-4d : u=%5d, h=%3d, w=%3d\n", a_entry, s_glist [a_entry].unicode, x_tall, x_wide);
   for (x_run = 0; x_run <= 8; x_run += 4) {
      /*---(set crop coords)-------------*/
      fprintf (my.o, "%-4d : u=%5d, x_run=%d, t=%3d, l=%3d, b=%3d, r=%3d\n", a_entry, s_glist [a_entry].unicode, x_run, s_glist [a_entry].coords [0 + x_run], s_glist [a_entry].coords [1 + x_run], s_glist [a_entry].coords [2 + x_run], s_glist [a_entry].coords [3 + x_run]);
      x_ctop = x_tall * ((100 - s_glist [a_entry].coords [0 + x_run]) / 100.0);
      x_clef = x_wide * (s_glist [a_entry].coords [1 + x_run] / 100.0);
      x_cbot = x_tall * ((100 - s_glist [a_entry].coords [2 + x_run]) / 100.0);
      x_crig = x_wide * (s_glist [a_entry].coords [3 + x_run] / 100.0);
      fprintf (my.o, "%-4d : u=%5d, x_run=%d, t=%3d, l=%3d, b=%3d, r=%3d\n", a_entry, s_glist [a_entry].unicode, x_run, x_ctop, x_clef, x_cbot, x_crig);
      if (x_ctop == x_cbot && x_clef == x_crig) {
         fprintf (my.o, "   skipping\n");
         continue;
      }
      for (x_row = x_ctop; x_row <= x_cbot; ++x_row) {
         for (x_col = x_clef; x_col < x_crig; ++x_col) {
            x_texloc   = ((x_ypos + x_row) * a_texw) + (x_xpos + x_col);
            g_yfont [a_slot]->tex_map [x_texloc] = 0;
         }
      }
   }
   return 0;
}

char
TEX__draw            (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =   0;
   int         i           =   0;   /* iterator -- gylphs                  */
   int         x_mycode    =   0;
   char        x_good      = '-';
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(allocate texture)---------------*/
   DEBUG_TOPS   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(transfer all glyphs)------------*/
   for (i = 0; i < s_nglist; ++i) {
      rc = yFONT__index_who  (a_slot, i, &x_mycode, &x_good);
      if (rc < 0)                         continue;
      if (strchr ("?r", x_good) != NULL)  continue;
      /*> rc = FREE__find (x_mycode);                                                   <*/
      rc = FREE__find (s_glist [i].unicode);
      rc = TEX__glyph (a_slot, i, my.texw);
      if (strchr ("mM", s_glist [i].reason) != NULL) {
         rc = TEX__crop  (a_slot, i, my.texw);
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         main driver                          ----===*/
/*====================------------------------------------====================*/
static void      o___MAIN____________________o (void) {;}

int
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int       rc        = 0;                   /* generic reture code          */
   int       i         = 0;
   uint      x_max     = 0;
   int       x_slot    = 0;
   /*---(setup)--------------------------*/
   if (rc >= 0)  rc = yURG_logger          (argc, argv);
   if (rc >= 0)  rc = PROG_init            ();
   if (rc >= 0)  rc = yURG_urgs            (argc, argv);
   if (rc >= 0)  rc = PROG_args            (argc, argv);
   if (rc >= 0)  rc = yFONT__conf_parse    ();
   if (rc <  0) {
      return rc;
   }
   /*> yFONT__conf_list ();                                                           <*/
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(set up font)--------------------*/
   while (1) {
      rc = 0;
      if (rc >= 0)  rc = yFONT__slot_new      ();
      if (rc >= 0)  x_slot = rc;
      DEBUG_VIEW   printf ("current slot : %d\n", x_slot);
      if (rc >= 0)  rc = FONT__choose         (x_slot);
      if (rc <  0)  break;
      if (rc >= 0)  rc = FREETYPE__setup      (my.src_file, my.point + my.adjust);
      if (rc <  0)  break;
      if (rc >= 0)  rc = GLIST__load          (x_slot, my.glist);
      if (rc >= 0)  rc = yFONT__head_nglyph   (x_slot, s_nglist);
      if (rc >= 0)  rc = yFONT__index_alloc   (x_slot);
      if (rc >= 0)  rc = INDEX__load          (x_slot);
      if (rc >= 0)  rc = INDEX__layout        (x_slot);
      if (rc >= 0)  rc = yFONT__map_alloc     (x_slot);
      if (rc >= 0)  rc = yFONT__map_clear     (x_slot);
      if (rc >= 0)  rc = TEX__draw            (x_slot);
      /*---(reporting)-----------------------------*/
      if (rc >= 0)  rc = yFONT__head_dump     (x_slot);
      if (rc >= 0)  rc = yFONT__index_dump    (x_slot);
      printf ("index dump rc = %d\n", rc);
      if (rc >= 0)  rc = yFONT__map_texart    (x_slot);
      /*---(calculations)--------------------------*/
      if (rc >= 0)  rc = yFONT__verts_alloc   (x_slot);
      if (rc >= 0)  rc = yFONT__verts_load    (x_slot);
      if (rc >= 0)  rc = yFONT__index_lookup  (x_slot);
      /*---(write txf)-----------------------------*/
      if (rc >= 0)  rc = yFONT__file_open     (x_slot, 'w');
      printf ("open  rc = %d\n", rc);
      if (rc >= 0)  rc = yFONT__head_write    (x_slot);
      printf ("head  rc = %d\n", rc);
      if (rc >= 0)  rc = yFONT__index_write   (x_slot);
      printf ("index rc = %d\n", rc);
      if (rc >= 0)  rc = yFONT__map_write     (x_slot);
      printf ("map   rc = %d\n", rc);
      if (rc >= 0)  rc = yFONT__file_close    (x_slot);
      printf ("close rc = %d\n", rc);
      /*---(shutdown)------------------------------*/
      rc = yFONT__map_free      (x_slot);
      rc = yFONT__index_free    (x_slot);
      rc = FREETYPE__shutdown   ();
      rc = yFONT__slot_free     (x_slot);
      /*---(done)----------------------------------*/
   }

   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================----end-of-source---============================*/
