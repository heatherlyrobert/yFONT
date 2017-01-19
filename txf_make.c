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


#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <math.h>

#include    "yFONT.h"
#include    "yFONT_priv.h"

#include    <yLOG.h>



/*===[[ DEBUGGING SETUP ]]====================================================*/
/* this is my latest standard format, vars, and urgents                       */
/* v3.0b : added signal handling                                (2014-feb-01) */
struct cDEBUG
{
   /*---(handle)-------------------------*/
   int         logger;                 /* log file so that we don't close it  */
   /*---(overall)------------------------*/  /* abcdefghi_kl__opq_stu__x__    */
   /* f = full urgents turns on all standard urgents                          */
   /* k = kitchen sink and turns everything, i mean everything on             */
   /* q = quiet turns all urgents off including the log itself                */
   char        tops;                   /* t) broad structure and context      */
   char        summ;                   /* s) statistics and analytical output */
   /*---(startup/shutdown)---------------*/
   char        args;                   /* a) command line args and urgents    */
   char        conf;                   /* c) configuration handling           */
   char        prog;                   /* p) program setup and teardown       */
   /*---(file processing)----------------*/
   char        inpt;                   /* i) text/data file input             */
   char        inpt_mas;               /* i) text/data file input   (mas/more)*/
   char        outp;                   /* o) text/data file output            */
   char        outp_mas;               /* o) text/data file output  (mas/more)*/
   /*---(event handling)-----------------*/
   char        loop;                   /* l) main program event loop          */
   char        user;                   /* u) user input and handling          */
   char        apis;                   /* z) interprocess communication       */
   char        sign;                   /* x) os signal handling               */
   char        scrp;                   /* b) scripts and batch operations     */
   char        hist;                   /* h) history, undo, redo              */
   /*---(program)------------------------*/
   char        graf;                   /* g) grahpics, drawing, and display   */
   char        data;                   /* d) complex data structure handling  */
   char        envi;                   /* e) environment processing           */
   char        envi_mas;               /* E) environment processing (mas/more)*/
   /*---(specific)-----------------------*/
   /*---(done)---------------------------*/
};
typedef     struct      cDEBUG       tDEBUG;
tDEBUG      debug;

#define     DEBUG_TOPS          if (debug.tops      == 'y')
#define     DEBUG_SUMM          if (debug.summ      == 'y')
#define     DEBUG_ARGS          if (debug.args      == 'y')
#define     DEBUG_CONF          if (debug.conf      == 'y')
#define     DEBUG_PROG          if (debug.prog      == 'y')
#define     DEBUG_INPT          if (debug.inpt      == 'y')
#define     DEBUG_INPTM         if (debug.inpt_mas  == 'y')
#define     DEBUG_OUTP          if (debug.outp      == 'y')
#define     DEBUG_OUTPM         if (debug.outp_mas  == 'y')
#define     DEBUG_LOOP          if (debug.loop      == 'y')
#define     DEBUG_USER          if (debug.user      == 'y')
#define     DEBUG_APIS          if (debug.apis      == 'y')
#define     DEBUG_SIGN          if (debug.sign      == 'y')
#define     DEBUG_SCRP          if (debug.scrp      == 'y')
#define     DEBUG_HIST          if (debug.hist      == 'y')
#define     DEBUG_GRAF          if (debug.graf      == 'y')
#define     DEBUG_DATA          if (debug.data      == 'y')
#define     DEBUG_ENVI          if (debug.envi      == 'y')
#define     DEBUG_ENVIM         if (debug.envi_mas  == 'y')



/*===[[ FREETYPE2 ]]==========================================================*/
#include    <ft2build.h>
#include    FT_FREETYPE_H


/*---(freetype structures)----------------------*/


/*---(program globals)--------------------------*/
#define   MAX_GLYPHS   3000
#define   MAX_TLISTS     20
#define   MAX_TGLYPHS   300

char      verbose       = 'n';
#define   VERB          if (verbose  == 'y')

typedef  struct cGLIST tGLIST;
struct cGLIST {
   int         unicode;                /* unicode of character                */
   char        reason;                 /* reason for character inclusion      */
   int         final;                  /* unicode of final character entry    */
   int         coords [12];            /* coordinates if modified             */
};
tGLIST      glist       [MAX_GLYPHS];
int         nglist      = 0;

typedef struct cTXF  tTXF;
struct  cTXF {
   FT_Face   face;
   char      name       [25];          /* name of font face                   */
   char      fixed;                    /* fixed width/height font (chinese)   */
   char     *font;                     /* name of font file                   */
   char     *txf;                      /* output txf file                     */
   FILE     *file;                     /* pointer to txf file                 */
   FILE       *o;
   int       point;                    /* point size of font                  */
   int       adjust;                   /* actual point size (for adjustment)  */
   char      byte;                     /* texture format (bit or byte)        */
   int       sp_char;                  /* character to use for space size     */
   /*> uchar    *glist;                    /+ incomming glyph list                +/   <*/
   /*> int       glist      [MAX_GLYPHS];  /+ incomming glyph list                +/   <*/
   /*> int       nglist;                   /+ number of glyphs requested          +/   <*/
   int       nglyphs;                  /* number of glyphs included           */
   int       ch_min;                   /* smallest ascii code included        */
   int       ch_max;                   /* largest ascii code included         */
   int       ch_range;                 /* number of pos beween min and max    */
   int       margin;                   /* character margin added              */
   int       row;                      /* size of a row                       */
   ulong     texw;                     /* width of texture                    */
   ulong     texh;                     /* height of texture                   */
   int       ascent;                   /* maximum ascent                      */
   int       descent;                  /* maximum descent                     */
   tGLYPH    glyph;                    /* current glyph                       */
   uchar    *texture;                  /* font texture map                    */
   float     scalex;                   /* convert metrics to bitmap units     */
   float     scaley;                   /* convert metrics to bitmap units     */
   tGLYPH   *glyph_table;              /* all glyphs                          */
};
tTXF      my;

struct {
   char        name        [  10];
   char        code        [MAX_TGLYPHS];
} glists [MAX_TLISTS] = {
   { "--tsae"   , " DGKNQTcdghklmnpqstwxyzaefiouvrj0123456789!@#$%^&.,:\"\'()-"                                          },
   { "--norm"   , " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~"   },
   { "--lower"  , " abcdefghijklmnopqrstuvwxyz_"                                                                         },
   { "--min"    , " cdghklmnpqstwxyzaefiouv"                                                                             },
   { "--vowel"  , "aefiouv"                                                                                              },
   { "--single" , "a"                                                                                              },
};

int format = YFONT_BYTE;

/* #define REPORT_GLYPHS */
#ifdef REPORT_GLYPHS
#define DEBUG_GLYPH4(msg,a,b,c,d) printf(msg,a,b,c,d)
#define DEBUG_GLYPH(msg) printf(msg)
#else
#define DEBUG_GLYPH4(msg,a,b,c,d) { /* nothing */ }
#define DEBUG_GLYPH(msg) { /* nothing */ }
#endif

#define MAX_GLYPHS_PER_GRAB 512  /* this is big enough for 2^9 glyph
                                    character sets */

int         s_limit = 30 * 256;

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

char
MAKE__codelist     (char *a_list)
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
   /*---(null list)----------------------*/
   for (i = 0; i < MAX_GLYPHS; ++i) {
      glist [i].unicode = -1;
      glist [i].reason  = '-';
      glist [i].final   = -1;
      for (j = 0; j < 12; ++j)  glist [i].coords [j] = 0;
   }
   nglist = 0;
   /*---(mandarin)-----------------------*/
   if (strcmp ("--mandfreq"   , a_list) == 0) {
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
         glist [nglist].unicode = atoi (p);
         fprintf (my.o, "%d\n", glist [nglist].unicode);
         p = strtok (NULL  , q);
         if (p == NULL)  continue;
         str_trim (p);
         glist [nglist].reason  = p[0];
         fprintf (my.o, "   %c\n", glist [nglist].reason);
         /*---(handle mods)--------------*/
         if (strchr ("mM", glist[nglist].reason) != NULL) {
            p = strtok (NULL  , q);
            if (p == NULL)  continue;
            glist [nglist].final   = glist [nglist].unicode;
            glist [nglist].unicode = atoi (p);
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
               glist [nglist].coords [i] = atoi (p);
               fprintf (my.o, "   %d\n", glist [nglist].coords[i]);
               p = strtok (NULL  , ",");
            }
            if (i < 12)  continue;
         }
         /*---(done)---------------------*/
         ++nglist;
      }
      loaded = 'y';
      fclose (f);
      return 0;
   }
   /*---(others)-------------------------*/
   for (i = 0; i < MAX_TLISTS; ++i) {
      if (strcmp (glists [i].name, a_list) != 0)  continue;
      loaded = 'y';
      for (j = 0; j < MAX_TGLYPHS; ++j) {
         if (glists [i].code [j] == '\0')  break;
         glist [j].unicode = glists[i].code[j];
         glist [j].reason  = 'f';
         ++nglist;
      }
      break;
   }
   if (loaded != 'y')  return -1;
   /*---(complete)-----------------------*/
   return 0;
}



/*====================------------------------------------====================*/
/*===----                   standard program functions                 ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char      verstring    [500];

char*        /*--: return versioning information ---------[ ------ [ ------ ]-*/
PROG_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strcpy (t, "[tcc built]");
#elif  __GNUC__  > 0
   strcpy (t, "[gnu gcc  ]");
#elif  __CBANG__  > 0
   strcpy (t, "[cbang    ]");
#else
   strcpy (t, "[unknown  ]");
#endif
   snprintf (verstring, 100, "%s   %s : %s", t, YFONT_VER_NUM, YFONT_VER_TXT);
   return verstring;
}

char         /*--: wholesale urgents change --------------[ leaf   [ ------ ]-*/
PROG_urgsmass      (char a_set, char a_extra)
{
   /*---(overall)------------------------*/
   debug.tops     = a_set;
   debug.summ     = a_set;
   /*---(startup/shutdown)---------------*/
   debug.args     = a_set;
   debug.conf     = a_set;
   debug.prog     = a_set;
   /*---(file processing)----------------*/
   debug.inpt     = a_set;
   debug.inpt_mas = a_set;
   debug.outp     = a_set;
   debug.outp_mas = a_set;
   /*---(event handling)-----------------*/
   debug.loop     = a_set;
   debug.user     = a_set;
   debug.apis     = a_set;
   debug.sign     = a_set;
   debug.scrp     = a_set;
   debug.hist     = a_set;
   /*---(program)------------------------*/
   debug.graf     = a_set;
   debug.data     = a_set;
   debug.envi     = a_set;
   debug.envi_mas = a_set;
   /*---(specific)-----------------------*/
   if (a_extra == 'y') {
      ;;
   }
   /*---(complete)-----------------------*/
   return 0;
}

char         /*--: process command line urgents ----------[ ------ [ ------ ]-*/
PROG_urgs          (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;        /* loop iterator -- arguments          */
   char       *a           = NULL;     /* current argument                    */
   int         x_len       = 0;        /* argument length                     */
   int         x_total     = 0;
   int         x_urgs      = 0;
   char        x_logger    = '-';
   /*---(default urgents)----------------*/
   PROG_urgsmass ('-', 'y');
   /*---(logger preprocessing)-----------*/
   debug.logger = -1;
   for (i = 1; i < argc; ++i) {
      a = argv[i];
      if (a[0] != '@')  continue;
      x_logger   = 'y';
      if       (strcmp ("@a"        , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@@args"    , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@f"        , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@@full"    , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@k"        , a) == 0)  debug.args = 'y';
      else if  (strcmp ("@@kitchen" , a) == 0)  debug.args = 'y';
      else if ((strcmp ("@q", a) == 0) || (strcmp ("@@quiet", a) == 0)) {
         x_logger = 'n';
         PROG_urgsmass ('-', 'y');
         break;
      }
   }
   /*---(startup logging)----------------*/
   if (x_logger == 'y') {
      debug.tops = 'y';
      debug.logger = yLOG_begin ("txf_make", yLOG_SYSTEM, yLOG_NOISE);
      DEBUG_TOPS   yLOG_info     ("purpose" , "create independent, beautiful texture fonts");
      DEBUG_TOPS   yLOG_info     ("khronos" , PROG_version   ());
      DEBUG_TOPS   yLOG_info     ("yLOG"    , yLOG_version   ());
   } else {
      debug.logger = yLOG_begin ("khronos", yLOG_SYSTEM, yLOG_QUIET);
   }
   /*---(walk through urgents)-----------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   for (i = 1; i < argc; ++i) {
      /*---(prepare)---------------------*/
      a        = argv[i];
      x_len    = strlen (a);
      ++x_total;
      /*---(filter)----------------------*/
      if (a[0] != '@')  continue;
      ++x_urgs;
      DEBUG_ARGS  yLOG_info  ("urgent"    , a);
      /*---(overall)---------------------*/
      if      (strncmp(a, "@t"           ,10) == 0)  debug.tops = 'y';
      else if (strncmp(a, "@@top"        ,10) == 0)  debug.tops = 'y';
      else if (strncmp(a, "@s"           ,10) == 0)  debug.tops = debug.summ  = 'y';
      else if (strncmp(a, "@@summ"       ,10) == 0)  debug.tops = debug.summ  = 'y';
      /*---(startup/shutdown)------------*/
      else if (strncmp(a, "@a"           ,10) == 0)  debug.tops = debug.args  = 'y';
      else if (strncmp(a, "@@args"       ,10) == 0)  debug.tops = debug.args  = 'y';
      else if (strncmp(a, "@c"           ,10) == 0)  debug.tops = debug.conf  = 'y';
      else if (strncmp(a, "@@conf"       ,10) == 0)  debug.tops = debug.conf  = 'y';
      else if (strncmp(a, "@p"           ,10) == 0)  debug.tops = debug.prog  = 'y';
      else if (strncmp(a, "@@prog"       ,10) == 0)  debug.tops = debug.prog  = 'y';
      /*---(file processing)-------------*/
      else if (strncmp(a, "@i"           ,10) == 0)  debug.tops = debug.inpt  = 'y';
      else if (strncmp(a, "@@inpt"       ,10) == 0)  debug.tops = debug.inpt  = 'y';
      else if (strncmp(a, "@I"           ,10) == 0)  debug.tops = debug.inpt  = debug.inpt_mas = 'y';
      else if (strncmp(a, "@@INPT"       ,10) == 0)  debug.tops = debug.inpt  = debug.inpt_mas = 'y';
      else if (strncmp(a, "@o"           ,10) == 0)  debug.tops = debug.outp  = 'y';
      else if (strncmp(a, "@@outp"       ,10) == 0)  debug.tops = debug.outp  = 'y';
      else if (strncmp(a, "@O"           ,10) == 0)  debug.tops = debug.outp  = debug.outp_mas = 'y';
      else if (strncmp(a, "@@OUTP"       ,10) == 0)  debug.tops = debug.outp  = debug.outp_mas = 'y';
      /*---(processing)------------------*/
      else if (strncmp(a, "@l"           ,10) == 0)  debug.tops = debug.loop  = 'y';
      else if (strncmp(a, "@@loop"       ,10) == 0)  debug.tops = debug.loop  = 'y';
      else if (strncmp(a, "@u"           ,10) == 0)  debug.tops = debug.user  = 'y';
      else if (strncmp(a, "@@user"       ,10) == 0)  debug.tops = debug.user  = 'y';
      else if (strncmp(a, "@z"           ,10) == 0)  debug.tops = debug.apis  = 'y';
      else if (strncmp(a, "@@apis"       ,10) == 0)  debug.tops = debug.apis  = 'y';
      else if (strncmp(a, "@x"           ,10) == 0)  debug.tops = debug.sign  = 'y';
      else if (strncmp(a, "@@sign"       ,10) == 0)  debug.tops = debug.sign  = 'y';
      else if (strncmp(a, "@b"           ,10) == 0)  debug.tops = debug.scrp  = 'y';
      else if (strncmp(a, "@@scrp"       ,10) == 0)  debug.tops = debug.scrp  = 'y';
      else if (strncmp(a, "@h"           ,10) == 0)  debug.tops = debug.hist  = 'y';
      else if (strncmp(a, "@@hist"       ,10) == 0)  debug.tops = debug.hist  = 'y';
      /*---(program)---------------------*/
      else if (strncmp(a, "@g"           ,10) == 0)  debug.tops = debug.graf  = 'y';
      else if (strncmp(a, "@@graf"       ,10) == 0)  debug.tops = debug.graf  = 'y';
      else if (strncmp(a, "@d"           ,10) == 0)  debug.tops = debug.data  = 'y';
      else if (strncmp(a, "@@data"       ,10) == 0)  debug.tops = debug.data  = 'y';
      else if (strncmp(a, "@e"           ,10) == 0)  debug.tops = debug.envi  = 'y';
      else if (strncmp(a, "@@envi"       ,10) == 0)  debug.tops = debug.envi  = 'y';
      else if (strncmp(a, "@E"           ,10) == 0)  debug.tops = debug.envi  = debug.envi_mas = 'y';
      else if (strncmp(a, "@@ENVI"       ,10) == 0)  debug.tops = debug.envi  = debug.envi_mas = 'y';
      /*---(complex)---------------------*/
      else if (strncmp(a, "@f"           ,10) == 0)  PROG_urgsmass ('y', '-');
      else if (strncmp(a, "@@full"       ,10) == 0)  PROG_urgsmass ('y', '-');
      else if (strncmp(a, "@k"           ,10) == 0)  PROG_urgsmass ('y', 'y');
      else if (strncmp(a, "@@kitchen"    ,10) == 0)  PROG_urgsmass ('y', 'y');
      /*---(specific)--------------------*/
      /*---(unknown)--------------------*/
      else {
         DEBUG_ARGS  yLOG_note    ("urgent not understood");
      }
      /*---(done)-----------------------*/
   }
   /*---(display urgents)----------------*/
   DEBUG_ARGS   yLOG_note    ("summarization of urgent processing");
   DEBUG_ARGS   yLOG_value   ("entries"   , x_total);
   DEBUG_ARGS   yLOG_value   ("urgents"   , x_urgs);
   DEBUG_ARGS   yLOG_note    ("standard urgents");
   DEBUG_ARGS   yLOG_char    ("tops"      , debug.tops);
   DEBUG_ARGS   yLOG_char    ("summ"      , debug.summ);
   DEBUG_ARGS   yLOG_char    ("args"      , debug.args);
   DEBUG_ARGS   yLOG_char    ("conf"      , debug.conf);
   DEBUG_ARGS   yLOG_char    ("prog"      , debug.prog);
   DEBUG_ARGS   yLOG_char    ("inpt"      , debug.inpt);
   DEBUG_ARGS   yLOG_char    ("INPT"      , debug.inpt_mas);
   DEBUG_ARGS   yLOG_char    ("outp"      , debug.outp);
   DEBUG_ARGS   yLOG_char    ("OUTP"      , debug.outp_mas);
   DEBUG_ARGS   yLOG_char    ("loop"      , debug.loop);
   DEBUG_ARGS   yLOG_char    ("user"      , debug.user);
   DEBUG_ARGS   yLOG_char    ("apis"      , debug.apis);
   DEBUG_ARGS   yLOG_char    ("sign"      , debug.sign);
   DEBUG_ARGS   yLOG_char    ("scrp"      , debug.scrp);
   DEBUG_ARGS   yLOG_char    ("hist"      , debug.hist);
   DEBUG_ARGS   yLOG_char    ("graf"      , debug.graf);
   DEBUG_ARGS   yLOG_char    ("data"      , debug.data);
   DEBUG_ARGS   yLOG_char    ("envi"      , debug.envi);
   DEBUG_ARGS   yLOG_char    ("ENVI"      , debug.envi_mas);
   DEBUG_ARGS   yLOG_note    ("specialty urgents");
   DEBUG_ARGS   yLOG_note    ("none yet");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: pre-argument program initialization ---[ leaf   [ ------ ]-*/
PROG_init          (void)
{
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   my.fixed = '-';
   /*> my.font   = "/usr/local/t_hyasdo/fonts/tsa_klinzhai.v74.ttf";                <*/
   my.font   = "/home/shared/fonts/comfortaa_regular.ttf";
   my.txf    = "default.txf";
   MAKE__codelist ("--norm");
   my.byte   = YFONT_BYTE;  /* always byte version */
   my.point  = 60;
   my.adjust = 0;
   my.margin = 2;
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: argument processing -------------------[ ------ [ ------ ]-*/
PROG_args     (int argc, char *argv[])
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
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
      if      (!strcmp(a, "--tsae"))     rc = MAKE__codelist (a);
      else if (!strcmp(a, "--norm"))     rc = MAKE__codelist (a);
      else if (!strcmp(a, "--lower"))    rc = MAKE__codelist (a);
      else if (!strcmp(a, "--min"))      rc = MAKE__codelist (a);
      else if (!strcmp(a, "--vowel"))    rc = MAKE__codelist (a);
      else if (!strcmp(a, "--single"))   rc = MAKE__codelist (a);
      else if (!strcmp(a, "--mandfreq")) rc = MAKE__codelist (a);
      else if (!strcmp(a, "--font"))     my.font    = argv[++i];
      else if (!strcmp(a, "--point"))    my.point   = atoi(argv[++i]);
      else if (!strcmp(a, "--adjust"))   my.adjust  = atoi(argv[++i]);
      else if (!strcmp(a, "--margin"))   my.margin  = atoi(argv[++i]);
      else if (!strcmp(a, "--file"))     my.txf     = argv[++i];
      else if (!strcmp(a, "--space"))    my.sp_char = argv[++i];
      else if (!strcmp(a, "--verbose"))  verbose    = 'y';
      else if (!strcmp(a, "-v"))         verbose    = 'y';
   }
   /*---(updates)-------------------------------*/
   if (my.point  == 0) my.point  = 60;
   my.adjust += my.point;
   if (my.margin == 0) my.margin = 2;
   /*---(header)--------------------------------*/
   /*> printf("#---------------------------------------------------------------------(begin)---\n");   <* 
    *> printf("   focus         : visualization\n");                                                   <* 
    *> printf("   niche         : fonts and text\n");                                                  <* 
    *> printf("   application   : txf_make\n");                                                        <* 
    *> printf("   purpose       : builds sweet texture-mapped fonts for opengl applications\n");       <* 
    *> printf("   base_system   : gnu/linux\n");                                                       <* 
    *> printf("   lang_name     : gnu/ansi-c (because it's right, just, best, and universal)\n");      <* 
    *> printf("   created       : 2010-08\n");                                                         <* 
    *> printf("   author        : the_heatherlys\n");                                                  <* 
    *> printf("   dependencies  : freetype library\n");                                                <* 
    *> printf("#---------------------------------------------------------------------(usage)---\n");   <* 
    *> printf("   --glist  ABC  : specific list of glyphs\n");                                         <* 
    *> printf("   --norm        : normal range of glyphs\n");                                          <* 
    *> printf("   --tsae        : tsae set of glyphs\n");                                              <* 
    *> printf("   --min         : only miniumu tsae glyphs (testing)\n");                              <* 
    *> printf("   --lower       : only lower case gyphs (testing)\n");                                 <* 
    *> printf("   --vowel       : only vowel gyphs (testing)\n");                                      <* 
    *> printf("   --point  NUM  : point size (default=60)\n");                                         <* 
    *> printf("   --adjust NUM  : adjusted size (default=60)\n");                                      <* 
    *> printf("   --margin NUM  : margin size (default=2)\n");                                            <* 
    *> printf("   --font   NAME : ttf font file\n");                                                   <* 
    *> printf("   --file   NAME : txf output file\n");                                                 <* 
    *> printf("#-------------------------------------------------------------------(urgents)---\n");   <* 
    *> printf("   verbose       : %c\n",     verbose);                                                 <* 
    *> printf("#----------------------------------------------------------------------(args)---\n");   <* 
    *> printf("   font source   : %s\n",     my.font);                                                 <* 
    *> printf("   font size     : %d pts\n", my.point);                                                <* 
    *> printf("   font adjusted : %d pts\n", my.adjust);                                               <* 
    *> printf("   char margin   : %d px\n",  my.margin);                                                  <* 
    *> printf("   glyph count   : %d\n",     my.nglist);                                               <* 
    *> printf("   txf output    : %s\n",     my.txf);                                                  <* 
    *> printf("   txf format    : %s\n",     (my.byte == 0) ? "byte" : "bit");                         <* 
    *> printf("#----------------------------------------------------------------------(code)---\n");   <*/
   /*> if (useError) {                                                                                    <* 
    *>    printf("   BAD ARGUMENT, FATAL\n");                                                             <* 
    *>    printf("#-----------------------------------------------------------------------(end)---\n");   <* 
    *>    exit(1);                                                                                        <* 
    *> }                                                                                                  <*/
   /*---(cut out name)-------------------*/
   /*> printf ("   name    = %s\n", my.txf);                                          <*/
   /*> printf ("   ptr     = %-10p\n", my.txf);                                       <*/
   p = strrchr (my.txf , '/');
   /*> printf ("   last /  = %-10p\n", p);                                            <*/
   if (p == NULL)  p = my.txf;
   else            ++p;
   /*> printf ("   file    = %s\n", p);                                               <*/
   strcpy (my.name, p);
   p = strrchr (my.name, '.');
   if (p != NULL)  p[0] = '\0';
   /*> printf ("   file    = %s\n", my.name);                                         <*/
   l = strlen (my.name);
   /*> printf ("   len     = %d\n", l);                                               <*/
   if  (l > 19)  l = 19;
   for (i = l; i < 25; ++i)  my.name [i] = '\0';
   /*> printf ("   file    = %s\n", my.name);                                         <*/
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: setup font for processing -------------[ ------ [ ------ ]-*/
MAKE__freetype     (char *a_font, int a_point)
{
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         rc          =   0;      /* simple return code                  */
   char        rce         = -10;      /* generic return code for errors      */
   FT_Library  x_lib;
   /*---(initialize freetype)------------*/
   DEBUG_INPT   yLOG_note    ("initialize the freetype library");
   rc = FT_Init_FreeType (&x_lib);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("freetype2"      , "library could not be openned");
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("initialization successful");
   /*---(set up font)---------------------------*/
   DEBUG_INPT   yLOG_note    ("load the source font");
   --rce;  if (a_font == NULL) {
      DEBUG_INPT   yLOG_fatal   ("font"           , "font name string argument is null");
      return rce;
   }
   DEBUG_INPT   yLOG_info    ("a_font"    , a_font);
   rc = FT_New_Face (x_lib, a_font, 0, &my.face);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("font"           , "freetype2 library could not find font");
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("font load successful");
   /*---(set up standard font size)-------------*/
   DEBUG_INPT   yLOG_note    ("setup the font size/point");
   DEBUG_INPT   yLOG_value   ("a_point"   , a_point);
   rc = FT_Set_Char_Size (my.face, 0, a_point * 64, 96, 96); /* in 1/64pt units    */
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("font"           , "freetype2 library could not set pixel size");
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("point size setting successful");
   /*---(complete)------------------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: find a glyph in the font --------------[ ------ [ ------ ]-*/
MAKE__find         (uint a_char)
{
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
      DEBUG_INPT   yLOG_warn    ("glyph"          , "freetype2 could not find the glyph");
      return rce;
   }
   /*---(pull the glyph)-----------------*/
   DEBUG_INPT   yLOG_snote   ("load");
   rc = FT_Load_Glyph (my.face, rc, FT_LOAD_DEFAULT);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_warn    ("glyph"          , "freetype2 could not load the glyph");
      return rce;
   }
   /*---(draw the glyph)-----------------*/
   DEBUG_INPT   yLOG_snote   ("render");
   rc = FT_Render_Glyph (my.face->glyph, FT_RENDER_MODE_NORMAL);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_warn    ("glyph"          , "freetype2 could not render the glyph");
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*--: gather glyph statistics ---------------[ ------ [ ------ ]-*/
MAKE__metrics      (void)
{
   /*---(begin)--------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(basics)-------------------------*/
   my.glyph.w   = my.face->glyph->bitmap.width; /* same as scaled metrics */
   DEBUG_INPT   yLOG_sint    (my.glyph.w);
   my.glyph.h   = my.face->glyph->bitmap.rows;  /*           "            */
   DEBUG_INPT   yLOG_sint    (my.glyph.h);
   /*---(convertions)--------------------*/
   my.scalex    = (float) my.glyph.w / (float) my.face->glyph->metrics.width;
   my.scaley    = (float) my.glyph.h / (float) my.face->glyph->metrics.height;
   /*---(usage)--------------------------*/
   my.glyph.xo  = my.face->glyph->metrics.horiBearingX * my.scalex;
   DEBUG_INPT   yLOG_sint    (my.glyph.xo);
   my.glyph.yo  = my.face->glyph->metrics.horiBearingY * my.scaley;
   DEBUG_INPT   yLOG_sint    (my.glyph.yo);
   my.glyph.a   = my.face->glyph->metrics.horiAdvance  * my.scalex;
   DEBUG_INPT   yLOG_sint    (my.glyph.a);
   /*---(complete)--------------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char         /*--: show ascii version -----------s--------[ ------ [ ------ ]-*/
MAKE__asciiart     (void)
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

char         /*--: load the character information --------[ ------ [ ------ ]-*/
MAKE__load         (void)
{
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   /*---(working)---------*/
   int         rc          =      0;   /* generic return code                 */
   int         k           =      0;   /* iterator -- gylphs                  */
   int         c           =      0;   /* glyph count                         */
   /*---(maximums)--------*/
   int         max_w       =   -100;   /* greatest character width            */
   int         max_h       =      0;   /* greatest character height           */
   int         max_a       =   -100;   /* greatest rise above baseline        */
   int         max_d       =    100;   /* greatest fall below baseline        */
   /*---(characters)------*/
   uint        cur_c       =      0;   /* current character code              */
   uint        min_c       = 100000;   /* minimum character code              */
   uint        max_c       =      0;   /* minimum character code              */
   /*---(counts)----------*/
   int         cum_l       =      0;   /* current width                       */
   int         cum_t       =      0;   /* current height                      */
   /*---(other)-----------*/
   uint        ch_min      = 100000;
   uint        ch_max      =    0;
   int         x_ascent    = 0;
   int         x_descent   = 0;
   /*---(initialize)---------------------*/
   my.texh = 0;
   my.texw = 0;
   cum_l   = my.margin + 1;
   cum_t   = my.margin + 1;
   /*---(allocate)-----------------------*/
   my.glyph_table = (tGLYPH *) malloc (nglist * sizeof(tGLYPH));
   if (my.glyph_table == NULL) {
      return YF_MEM_FULL;
   }
   /*---(measure all glyphs)-------------*/
   for (k = 0; k < nglist; ++k) {
      /*---(find glyph)------------------*/
      DEBUG_INPT   yLOG_value   ("character" , k);
      cur_c = glist [k].unicode;
      DEBUG_INPT   yLOG_value   ("unicode"   , cur_c);
      rc = MAKE__find    (cur_c);
      if (rc < 0) {
         cur_c = ' ';
         /*> return rc;                                                               <*/
      }
      if (rc > 0) continue;
      /*---(get basics)------------------*/
      my.glyph.c = cur_c;
      MAKE__metrics ();
      /*---(watch for space)-------------*/
      if (my.glyph.c == ' ' && my.glyph.a == 0) {
         DEBUG_INPT   yLOG_note    ("special handling for space character");
         my.glyph.c = 'a';
         MAKE__find    (my.glyph.c);
         MAKE__metrics ();
         my.glyph.c = ' ';
      }
      if (glist [k].unicode != cur_c && cur_c == ' ') {
         my.glyph.c = cur_c = glist [k].unicode;
      }
      /*---(watch width limit)-----------*/
      if ((cum_l + my.glyph.w + (my.margin * 2) + 2) >= s_limit) {
         my.texw = s_limit;
         cum_l   = my.margin + 1;
         cum_t  += (max_h + my.margin + 1);
         max_h   = 0;
      }
      /*---(calculate)-------------------*/
      x_ascent     = my.glyph.yo;
      x_descent    = x_ascent - my.glyph.h;
      if (cur_c       < ch_min)  ch_min  = cur_c;
      if (cur_c       > ch_max)  ch_max  = cur_c;
      if (my.glyph.w  > max_w)   max_w   = my.glyph.w;
      if (my.glyph.h  > max_h)   max_h   = my.glyph.h;
      if (x_ascent    > max_a)   max_a   = x_ascent;
      if (x_descent   < max_d)   max_d   = x_descent;
      /*---(totalize)---------------------------*/
      my.glyph.y   = cum_t;
      my.glyph.x   = cum_l;
      cum_l       += my.glyph.w + (my.margin + 1);
      if (my.texw < s_limit)  my.texw      = cum_l;
      my.texh      = cum_t + max_h + (my.margin + 1);
      /*---(output)----------------------*/
      printf ("--------------------------------------------------------\n");
      printf ("count numbr x_pos y_pos width heigh ascen desce xoffs yoffs advan    cum_t cum_l tex_w tex_h\n");
      printf ("%5d %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d    %5d %5d %5d\n",
            c, glist[k].unicode, my.glyph.x, my.glyph.y, my.glyph.w, my.glyph.h, 
            x_ascent, x_descent,
            my.glyph.xo, my.glyph.yo, my.glyph.a,
            cum_t, cum_l, my.texw, my.texh);
      MAKE__asciiart ();
      /*---(adjust widths)---------------*/
      /*> my.glyph.xo -= my.margin;                                                   <* 
       *> my.glyph.x  -= my.margin;                                                   <* 
       *> my.glyph.w  += (my.margin + my.margin);                                     <*/
      /*---(load glyph)------------------*/
      my.glyph_table [k].c        = my.glyph.c;
      my.glyph_table [k].x        = my.glyph.x;
      my.glyph_table [k].y        = my.glyph.y;
      my.glyph_table [k].w        = my.glyph.w;
      my.glyph_table [k].h        = my.glyph.h;
      my.glyph_table [k].xo       = my.glyph.xo;
      my.glyph_table [k].yo       = my.glyph.yo;
      my.glyph_table [k].a        = my.glyph.a;
      if (strchr ("mM", glist [k].reason) != NULL)  my.glyph_table [k].c = glist [k].final;
      /*---(done)------------------------*/
      ++c;
   }
   /*---(save statistics)----------------*/
   my.texw      +=  16 - (my.texw %  16);   /* get a clean mulpilier  */
   my.texh       =  cum_t + max_h + (my.margin + 1);
   my.texh      +=  16 - (my.texh %  16);   /* get a clean mulpilier  */
   my.ascent     =  max_a;
   my.descent    =  max_d;
   my.nglyphs    =  c;
   my.ch_min     =  ch_min;
   my.ch_max     =  ch_max;
   /*---(output summary)-----------------*/
   printf("\n");
   printf("exp glyphs  =  %7d\n", nglist);
   printf("act glyphs  =  %7d\n", c);
   printf("max width   =  %7d\n", max_w);
   printf("max height  =  %7d\n", max_h);
   printf("max ascent  =  %7d\n", my.ascent);
   printf("max descent =  %7d\n", my.descent);
   printf("min char    =  %7d\n", my.ch_min);
   printf("max char    =  %7d\n", my.ch_max);
   printf("tex width   =  %7d\n", my.texw);
   printf("tex height  =  %7d\n", my.texh);
   printf("tex bytes   =  %7d\n", my.texh * my.texw);
   printf("\n");
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
MAKE__image        (int a_index)
{
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   ulong     texloc, bitloc;
   int       i, j;
   int       k;
   int       width, offset;
   uchar     value         = 0;
   int       x_top         = 0;
   int       x_left        = 0;
   int       x_width       = 0;
   int       x_height      = 0;
   int       x_ctop        = 3000;
   int       x_clef        = 3000;
   int       x_cbot        = -1;
   int       x_crig        = -1;
   DEBUG_OUTP   yLOG_value   ("texh"      , my.texh);
   DEBUG_OUTP   yLOG_value   ("texw"      , my.texw);
   DEBUG_OUTP   yLOG_value   ("size"      , my.texh * my.texw);
   DEBUG_OUTP   yLOG_value   ("height"    , my.glyph.h);
   DEBUG_OUTP   yLOG_value   ("width"     , my.glyph.w);
   DEBUG_OUTP   yLOG_value   ("x"         , my.glyph.x);
   DEBUG_OUTP   yLOG_value   ("y"         , my.glyph.y);
   for (i = 0; i < my.glyph.h; ++i) {
      DEBUG_OUTP   yLOG_value   ("row (i)"   , i);
      /*> offset = (my.texh - my.glyph.y - i) * (my.texw);                            <*/
      x_top    = my.glyph.y;
      /*> x_left   = my.glyph.x + my.margin;                                          <* 
       *> x_width  = my.glyph.w - (my.margin * 2);                                    <*/
      x_left   = my.glyph.x;
      x_width  = my.glyph.w;
      x_height = my.glyph.h;
      offset   = (my.glyph.y + i) * my.texw;
      /*> offset   = (my.glyph.y + my.glyph.h - i) * my.texw;                         <*/
      DEBUG_OUTP   yLOG_value   ("offset"    , offset);
      for (j = 0; j < x_width; ++j) {
         DEBUG_OUTP   yLOG_value   ("col (j)"   , j);
         bitloc   =  (i * x_width) + j;
         DEBUG_OUTP   yLOG_value   ("source"    , bitloc);
         value    = (uchar) my.face->glyph->bitmap.buffer[bitloc];
         DEBUG_OUTP   yLOG_value   ("value"     , value);
         width    = x_left + j;
         DEBUG_OUTP   yLOG_value   ("width"     , width);
         texloc   =  offset + width;
         DEBUG_OUTP   yLOG_value   ("destin"    , texloc);
         /*> printf ("%3d,%3d) %3d   from  %6d to %6d\n", i, j, value, bitloc, texloc);   <*/
         /*> if (value == 0) printf("  ");                                         <* 
          *> else            printf ("%02x", value);                               <*/
         my.texture [texloc] = value;
      }
      /*> printf("\n");                                                            <*/
   }
   if (strchr ("mM", glist [a_index].reason) != NULL) {
      fprintf (my.o, "%-4d : u=%5d, h=%3d, w=%3d\n", a_index, glist [a_index].unicode, my.glyph.h, my.glyph.w);
      for (k = 0; k <= 8; k += 4) {
         fprintf (my.o, "%-4d : u=%5d, k=%d, t=%3d, l=%3d, b=%3d, r=%3d\n", a_index, glist [a_index].unicode, k, glist [a_index].coords [0 + k], glist [a_index].coords [1 + k], glist [a_index].coords [2 + k], glist [a_index].coords [3 + k]);
         x_ctop = my.glyph.h * ((100 - glist [a_index].coords [0 + k]) / 100.0);
         x_clef = my.glyph.w * (glist [a_index].coords [1 + k] / 100.0);
         x_cbot = my.glyph.h * ((100 - glist [a_index].coords [2 + k]) / 100.0);
         x_crig = my.glyph.w * (glist [a_index].coords [3 + k] / 100.0);
         fprintf (my.o, "%-4d : u=%5d, k=%d, t=%3d, l=%3d, b=%3d, r=%3d\n", a_index, glist [a_index].unicode, k, x_ctop, x_clef, x_cbot, x_crig);
         if (x_ctop == x_cbot && x_clef == x_crig) {
            fprintf (my.o, "   skipping\n");
            continue;
         }
         for (i = x_ctop; i <= x_cbot; ++i) {
            x_left   = my.glyph.x;
            offset   = (my.glyph.y + i) * my.texw;
            for (j = x_clef; j < x_crig; ++j) {
               width    = x_left + j;
               texloc   =  offset + width;
               my.texture [texloc] = 0;
            }
         }
      }
   }
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> write txf file header -----------------[ ------ [ ------ ]-*/
MAKE__header       (void)
{
   /*---(begin)--------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   /*---(open file)----------------------*/
   my.file = fopen(my.txf , "wb");
   if (my.file == NULL) {
      return -1;
   }
   /*---(write magic number)-------------*/
   fwrite ("\377txf"   ,  1,  4, my.file);
   /*---(write header)-------------------*/
   fwrite (&my.name    ,  1, 20, my.file);
   fwrite (&my.point   ,  4,  1, my.file);
   fwrite (&my.texw    ,  8,  1, my.file);
   fwrite (&my.texh    ,  4,  1, my.file);
   fwrite (&my.ascent  ,  4,  1, my.file);
   fwrite (&my.descent ,  4,  1, my.file);
   fwrite (&my.margin  ,  4,  1, my.file);
   fwrite (&my.nglyphs ,  4,  1, my.file);
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> write txf glyph table -----------------[ ------ [ ------ ]-*/
MAKE__table        (void)
{
   /*---(begin)--------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   int k = 0;
   for (k = 0; k < nglist; ++k) {
      fwrite (&my.glyph_table [k], sizeof (tGLYPH), 1, my.file);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> write txf glyph table -----------------[ ------ [ ------ ]-*/
MAKE__draw         (void)
{
   char        rc          = 0;
   int         save_c      = 0;
   int k = 0;
   /*---(begin)--------------------------*/
   DEBUG_OUTP   yLOG_enter   (__FUNCTION__);
   for (k = 0; k < nglist; ++k) {
      DEBUG_OUTP   yLOG_value   ("character" , k);
      DEBUG_OUTP   yLOG_value   ("unicode"   , glist[k].unicode);
      rc = MAKE__find    (glist[k].unicode);
      DEBUG_OUTP   yLOG_value   ("find_rc"   , rc);
      my.glyph.c    = glist[k].unicode;
      if (rc < 0) {
         my.glyph.c = ' ';
         /*> return rc;                                                               <*/
      }
      rc = MAKE__metrics ();
      DEBUG_OUTP   yLOG_value   ("metrics_rc", rc);
      /*---(watch for space)--------------------*/
      if (my.glyph.c == ' ') {
         my.glyph.c = 'a';
         MAKE__find    (my.glyph.c);
         MAKE__metrics ();
         my.glyph.c = ' ';
      }
      if (my.glyph.c != save_c) {
         my.glyph.c = save_c;
      }
      /*---(load glyph)------------------*/
      my.glyph.c   = my.glyph_table[k].c;
      my.glyph.x   = my.glyph_table[k].x;
      my.glyph.y   = my.glyph_table[k].y;
      my.glyph.w   = my.glyph_table[k].w;
      my.glyph.h   = my.glyph_table[k].h;
      my.glyph.xo  = my.glyph_table[k].xo;
      my.glyph.yo  = my.glyph_table[k].yo;
      my.glyph.a   = my.glyph_table[k].a;
      /*---(copy image)-------------------------*/
      if (my.glyph.c != ' ')  MAKE__image(k);
   }
   /*---(complete)-----------------------*/
   DEBUG_OUTP   yLOG_exit    (__FUNCTION__);
   return 0;
}

int
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int       rc        = 0;                   /* generic reture code          */
   int       i         = 0;
   uint      x_max     = 0;
   /*---(setup)--------------------------*/
   if (rc == 0)  rc = PROG_urgs (argc, argv);
   if (rc == 0)  rc = PROG_init ();
   if (rc == 0)  rc = PROG_args (argc, argv);
   if (rc <  0) {
      return rc;
   }
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(setup freetype)-----------------*/
   if (rc == 0)  rc = MAKE__freetype (my.font, my.adjust);
   if (rc == 0)  rc = MAKE__load     ();
   if (rc <  0) {
      return rc;
   }
   /*---(write txf header)----------------------*/
   if (rc >= 0)  rc = yFONT__slot_new   ();
   if (rc >= 0)  x_slot = rc;
   if (rc >= 0)  rc = yFONT__file_open  (x_slot);
   if (rc >= 0)  rc = yFONT__head_write (x_slot);
   if (rc <  0) {
      return rc;
   }
   MAKE__table ();
   /*---(allocate the texture)------------------*/
   DEBUG_OUTP   yLOG_note    ("allocating texture");
   my.texture = (uchar *) malloc (my.texw * my.texh * sizeof (uchar));
   DEBUG_OUTP   yLOG_point   ("texture"   , my.texture);
   if (my.texture == NULL) {
      printf("FATAL: calloc could not allocate memory for texture\n");
      return -10;
   }
   x_max = my.texw * my.texh;
   for (i = 0; i < x_max; ++i)  my.texture [i] = 0;
   /*---(write the per glyph info)--------------*/
   MAKE__draw ();
   fwrite (my.texture, my.texh * my.texw, 1, my.file);
   fclose (my.file);
   printf ("header      =  %7d\n",  8 * 4);
   printf ("glyphs      =  %7d\n",  my.nglyphs * sizeof(tGLYPH));
   printf ("texture     =  %7d\n",  my.texh * my.texw);
   printf ("TOTAL       =  %7d\n",  (8 * 4) + (my.nglyphs * sizeof(tGLYPH)) + (my.texh * my.texw));
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================----end-of-source---============================*/
