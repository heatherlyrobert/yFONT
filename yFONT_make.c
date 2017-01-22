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




#define     SRC_DIR     "/home/shared/fonts"
#define     SRC_FILE    "comfortaa_regular.ttf"

#define     DST_DIR     "/usr/local/share/fonts"
#define     DST_EXT     "txf"

char        FONT__init         (void);
char        FONT__choose       (char a_slot);


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
   int         unicode;                /* unicode of character                */
   char        reason;                 /* reason for character inclusion      */
   int         final;                  /* unicode of final character entry    */
   int         coords [12];            /* coordinates if modified             */
};
tGLIST      glist       [MAX_GLYPHS];
int         nglist      = 0;

typedef struct cACCESSOR  tACCESSOR;
struct  cACCESSOR {
   /*---(files)--------------------------*/
   char        font_name  [LEN_STR];     /* font name from command line         */
   char        out_name   [LEN_STR];     /* name of output txf font name        */
   char        out_file   [LEN_STR];     /* name of output txf font file        */
   char        src_file   [LEN_STR];     /* name of source ttf font file        */
   /*---(characteristics)----------------*/
   char        point;                    /* point size of font                  */
   char        adjust;                   /* actual point size (for adjustment)  */
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
   /*> uchar    *glist;                    /+ incomming glyph list                +/   <*/
   /*> int       glist      [MAX_GLYPHS];  /+ incomming glyph list                +/   <*/
   /*> int       nglist;                   /+ number of glyphs requested          +/   <*/
   int       nglyphs;                  /* number of glyphs included           */

   int       row;                      /* size of a row                       */

   uchar    *texture;                  /* font texture map                    */
   tGLYPH   *glyph_table;              /* all glyphs                          */
};
tACCESSOR      my;


struct {
   char        name        [  10];
   char        code        [MAX_TGLYPHS];
} s_glist_info [MAX_TLISTS] = {
   { "tsae"   , " DGKNQTcdghklmnpqstwxyzaefiouvrj0123456789!@#$%^&.,:\"\'()-"                                          },
   { "norm"   , " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~"   },
   { "lower"  , " abcdefghijklmnopqrstuvwxyz_"                                                                         },
   { "min"    , " cdghklmnpqstwxyzaefiouv"                                                                             },
   { "vowel"  , "aefiouv"                                                                                              },
   { "single" , "a"                                                                                              },
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   DEBUG_YFONT  yLOG_value   ("font_name" , my.font_name);
   /*---(check for all fonts)------------*/
   if      (strcmp (my.font_name, "all") == 0) {
      DEBUG_YFONT  yLOG_note    ("program in ALL font mode");
      /*---(check for first)-------------*/
      if (strcmp (my.out_name, "") == 0) {
         DEBUG_YFONT  yLOG_note    ("first font to be chosen");
         rc = yFONT__conf_head (my.out_name, &my.point, &my.adjust, my.glist, my.src_file);
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
         rc = yFONT__conf_next (my.out_name, &my.point, &my.adjust, my.glist, my.src_file);
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
      rc = yFONT__conf_info (my.font_name, &my.point, &my.adjust, my.glist, my.src_file);
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   for (i = 0; i < MAX_GLYPHS; ++i) {
      glist [i].unicode = -1;
      glist [i].reason  = '-';
      glist [i].final   = -1;
      for (j = 0; j < 12; ++j)  glist [i].coords [j] = 0;
   }
   nglist = 0;
   return 0;
}

char
GLIST__normal        (char *a_list)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   int         i           =   0;
   int         j           =   0;
   /*---(defense)------------------------*/
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   if (a_list == NULL)  return -1;
   /*---(load)---------------------------*/
   for (i = 0; i < MAX_TLISTS; ++i) {
      /*---(filter)----------------------*/
      if (s_glist_info [i].name [0] != a_list [0])  continue;
      if (strcmp (s_glist_info [i].name, a_list) != 0)  continue;
      /*---(load)------------------------*/
      for (j = 0; j < MAX_TGLYPHS; ++j) {
         if (s_glist_info [i].code [j] == '\0')  break;
         glist [j].unicode = s_glist_info[i].code[j];
         glist [j].reason  = 'f';
         ++nglist;
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   if (rc == 0)  rc = GLIST__clear       ();
   if (rc == 0)  rc = GLIST__normal      (a_list);
   if (rc <  0)  rc = GLIST__mandarin    (a_list);
   yFONT__head_nglyph  (a_slot, nglist);
   yFONT__index_alloc  (a_slot);
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         rc          =   0;      /* simple return code                  */
   char        rce         = -10;      /* generic return code for errors      */
   /*---(initialize freetype)------------*/
   DEBUG_INPT   yLOG_note    ("initialize the freetype library");
   rc = FT_Init_FreeType (&my.lib);
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
   rc = FT_New_Face (my.lib, a_font, 0, &my.face);
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

char         /*--: shutdown a font after processing ------[ ------ [ ------ ]-*/
FREETYPE__shutdown   (void)
{  /*---(design notes)-------------------*/
   /*
    *  only effects the my.lib, my.face variables
    */
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   /*---(locals)-----------+-----------+-*/
   int         rc          =   0;      /* simple return code                  */
   char        rce         = -10;      /* generic return code for errors      */
   /*---(close the font)-----------------*/
   DEBUG_INPT   yLOG_note    ("close the source font");
   rc = FT_Done_Face  (my.face);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("font"           , "freetype2 library could not close font");
      return rce;
   }
   DEBUG_INPT   yLOG_note    ("font unloaded successful");
   /*---(shutdown freetype)--------------*/
   DEBUG_INPT   yLOG_note    ("shutdown the freetype library");
   rc = FT_Done_FreeType (my.lib);
   DEBUG_INPT   yLOG_value   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_INPT   yLOG_fatal   ("freetype2"      , "library could not be closed");
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
   int         x_code      =      0;   /* glyph count                         */
   char        x_good      =    '-';   /* good vs bad                         */
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   /*---(measure all glyphs)-------------*/
   for (i = 0; i < nglist; ++i) {
      x_good = '-';
      /*---(find glyph)------------------*/
      DEBUG_INPT   yLOG_value   ("CHAR"      , i);
      x_code = my.glyph.code = glist [i].unicode;
      DEBUG_INPT   yLOG_value   ("unicode"   , x_code);
      rc = FREE__find    (x_code);
      /*---(handle misses)---------------*/
      DEBUG_INPT   yLOG_value   ("rc"        , rc);
      if (rc <  0) {
         x_good = '?';
         yFONT__index_code   (a_slot, i, x_code, x_good);
         continue;
      }
      /*---(handle success)--------------*/
      if (rc == 0)  x_good = 'y';
      GLYPH__metrics ();
      /*---(watch for bad spaces)--------*/
      if (my.glyph.code == ' ' && my.glyph.adv == 0) {
         DEBUG_INPT   yLOG_note    ("special handling for null space character");
         my.glyph.code = 'a';
         FREE__find     (my.glyph.code);
         GLYPH__metrics ();
         my.glyph.code = ' ';
         x_good = 'r';
      }
      /*---(load index)------------------*/
      yFONT__index_code   (a_slot, i, x_code       , x_good);
      yFONT__index_size   (a_slot, i, my.glyph.wide, my.glyph.tall);
      yFONT__index_offset (a_slot, i, my.glyph.xoff, my.glyph.yoff, my.glyph.adv);
      /*---(handle special)--------------*/
      if (strchr ("mM", glist [i].reason) != NULL) {
         x_good = 'm';
         yFONT__index_code   (a_slot, i, glist [i].final, x_good);
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
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   /*---(initialize)---------------------*/
   x_tall  = yFONT__index_maxes  (a_slot);
   x_cumw  = my.margin + 1;
   x_cumh  = my.margin + 1;
   /*---(measure all glyphs)-------------*/
   for (i = 0; i < nglist; ++i) {
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
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(row-by-row)---------------------*/
   for (x_row = 0; x_row < x_tall; ++x_row) {
      DEBUG_INPT   yLOG_value   ("row"       , x_row);
      /*---(col-by-col)------------------*/
      for (x_col = 0; x_col < x_wide; ++x_col) {
         /*---(transfer value)-----------*/
         x_bitloc   = (x_row * x_wide) + x_col;
         x_value    = (uchar) my.face->glyph->bitmap.buffer[x_bitloc];
         x_texloc   = ((x_ypos + x_row) * a_texw) + (x_xpos + x_col);
         g_font [a_slot]->tex_map [x_texloc] = x_value;
         /*---(done with value)----------*/
      }
      /*---(done with col)---------------*/
   }
   /*---(done with row)------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
TEX__crop            (char x_slot)
{
}

char
TEX__draw            (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   char        rc          =   0;
   int         i           =   0;   /* iterator -- gylphs                  */
   int         x_code      =   0;
   char        x_good      = '-';
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   URG_VERB     printf       ("%s\n", __FUNCTION__);
   /*---(allocate texture)---------------*/
   rc = yFONT__map_alloc (a_slot);
   DEBUG_TOPS   yLOG_value   ("rc"        , rc);
   if (rc < 0) {
      DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(transfer all glyphs)------------*/
   for (i = 0; i < nglist; ++i) {
      rc = yFONT__index_who  (a_slot, i, &x_code, &x_good);
      if (rc < 0)                         continue;
      if (strchr ("?r", x_good) != NULL)  continue;
      rc = FREE__find (x_code);
      rc = TEX__glyph (a_slot, i, my.texw);
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                         old stuff                            ----===*/
/*====================------------------------------------====================*/
static void      o___OLD_STUFF_______________o (void) {;}

/*> char         /+--: load the character information --------[ ------ [ ------ ]-+/                                   <* 
 *> MAKE__load         (void)                                                                                          <* 
 *> {                                                                                                                  <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                        <* 
 *>    /+---(working)---------+/                                                                                       <* 
 *>    int         rc          =      0;   /+ generic return code                 +/                                   <* 
 *>    int         k           =      0;   /+ iterator -- gylphs                  +/                                   <* 
 *>    int         c           =      0;   /+ glyph count                         +/                                   <* 
 *>    /+---(maximums)--------+/                                                                                       <* 
 *>    int         max_w       =   -100;   /+ greatest character width            +/                                   <* 
 *>    int         max_h       =      0;   /+ greatest character height           +/                                   <* 
 *>    int         max_a       =   -100;   /+ greatest rise above baseline        +/                                   <* 
 *>    int         max_d       =    100;   /+ greatest fall below baseline        +/                                   <* 
 *>    /+---(characters)------+/                                                                                       <* 
 *>    uint        cur_c       =      0;   /+ current character code              +/                                   <* 
 *>    uint        min_c       = 100000;   /+ minimum character code              +/                                   <* 
 *>    uint        max_c       =      0;   /+ minimum character code              +/                                   <* 
 *>    /+---(counts)----------+/                                                                                       <* 
 *>    int         cum_l       =      0;   /+ current width                       +/                                   <* 
 *>    int         cum_t       =      0;   /+ current height                      +/                                   <* 
 *>    /+---(other)-----------+/                                                                                       <* 
 *>    uint        ch_min      = 100000;                                                                               <* 
 *>    uint        ch_max      =    0;                                                                                 <* 
 *>    int         x_ascent    = 0;                                                                                    <* 
 *>    int         x_descent   = 0;                                                                                    <* 
 *>    /+---(header)-------------------------+/                                                                        <* 
 *>    DEBUG_TOPS   yLOG_enter   (__FUNCTION__);                                                                       <* 
 *>    /+---(initialize)---------------------+/                                                                        <* 
 *>    my.texh = 0;                                                                                                    <* 
 *>    my.texw = 0;                                                                                                    <* 
 *>    cum_l   = my.margin + 1;                                                                                        <* 
 *>    cum_t   = my.margin + 1;                                                                                        <* 
 *>    /+---(allocate)-----------------------+/                                                                        <* 
 *>    my.glyph_table = (tGLYPH *) malloc (nglist * sizeof(tGLYPH));                                                   <* 
 *>    if (my.glyph_table == NULL) {                                                                                   <* 
 *>       return YF_MEM_FULL;                                                                                          <* 
 *>    }                                                                                                               <* 
 *>    /+---(measure all glyphs)-------------+/                                                                        <* 
 *>    for (k = 0; k < nglist; ++k) {                                                                                  <* 
 *>       /+---(find glyph)------------------+/                                                                        <* 
 *>       DEBUG_INPT   yLOG_value   ("character" , k);                                                                 <* 
 *>       cur_c = glist [k].unicode;                                                                                   <* 
 *>       DEBUG_INPT   yLOG_value   ("unicode"   , cur_c);                                                             <* 
 *>       rc = FREE__find    (cur_c);                                                                                  <* 
 *>       if (rc < 0) {                                                                                                <* 
 *>          cur_c = ' ';                                                                                              <* 
 *>          /+> return rc;                                                               <+/                          <* 
 *>       }                                                                                                            <* 
 *>       if (rc > 0) continue;                                                                                        <* 
 *>       /+---(get basics)------------------+/                                                                        <* 
 *>       my.glyph.code = cur_c;                                                                                       <* 
 *>       GLYPH__metrics ();                                                                                           <* 
 *>       /+---(watch for space)-------------+/                                                                        <* 
 *>       if (my.glyph.code == ' ' && my.glyph.adv == 0) {                                                             <* 
 *>          DEBUG_INPT   yLOG_note    ("special handling for space character");                                       <* 
 *>          my.glyph.code = 'a';                                                                                      <* 
 *>          FREE__find    (my.glyph.code);                                                                            <* 
 *>          GLYPH__metrics ();                                                                                        <* 
 *>          my.glyph.code = ' ';                                                                                      <* 
 *>       }                                                                                                            <* 
 *>       if (glist [k].unicode != cur_c && cur_c == ' ') {                                                            <* 
 *>          my.glyph.code = cur_c = glist [k].unicode;                                                                <* 
 *>       }                                                                                                            <* 
 *>       /+---(watch width limit)-----------+/                                                                        <* 
 *>       if ((cum_l + my.glyph.wide + (my.margin * 2) + 2) >= s_limit) {                                              <* 
 *>          my.texw = s_limit;                                                                                        <* 
 *>          cum_l   = my.margin + 1;                                                                                  <* 
 *>          cum_t  += (max_h + my.margin + 1);                                                                        <* 
 *>          max_h   = 0;                                                                                              <* 
 *>       }                                                                                                            <* 
 *>       /+---(calculate)-------------------+/                                                                        <* 
*>       x_ascent     = my.glyph.yoff;                                                                                <* 
*>       x_descent    = x_ascent - my.glyph.tall;                                                                     <* 
*>       if (cur_c       < ch_min)  ch_min  = cur_c;                                                                  <* 
*>       if (cur_c       > ch_max)  ch_max  = cur_c;                                                                  <* 
*>       if (my.glyph.wide  > max_w)   max_w   = my.glyph.wide;                                                       <* 
*>       if (my.glyph.tall  > max_h)   max_h   = my.glyph.tall;                                                       <* 
*>       if (x_ascent    > max_a)   max_a   = x_ascent;                                                               <* 
*>       if (x_descent   < max_d)   max_d   = x_descent;                                                              <* 
*>       /+---(totalize)---------------------------+/                                                                 <* 
*>       my.glyph.ypos   = cum_t;                                                                                     <* 
*>       my.glyph.xpos   = cum_l;                                                                                     <* 
*>       cum_l       += my.glyph.wide + (my.margin + 1);                                                              <* 
*>       if (my.texw < s_limit)  my.texw      = cum_l;                                                                <* 
*>       my.texh      = cum_t + max_h + (my.margin + 1);                                                              <* 
*>       /+---(output)----------------------+/                                                                        <* 
*>       printf ("--------------------------------------------------------\n");                                       <* 
*>       printf ("count numbr x_pos y_pos width heigh ascen desce xoffs yoffs advan    cum_t cum_l tex_w tex_h\n");   <* 
*>       printf ("%5d %5d %5d %5d %5d %5d %5d %5d %5d %5d %5d    %5d %5d %5d\n",                                      <* 
      *>             c, glist[k].unicode, my.glyph.xpos, my.glyph.ypos, my.glyph.wide, my.glyph.tall,                       <* 
      *>             x_ascent, x_descent,                                                                                   <* 
      *>             my.glyph.xoff, my.glyph.yoff, my.glyph.adv,                                                            <* 
      *>             cum_t, cum_l, my.texw, my.texh);                                                                       <* 
*>       GLYPH__art     ();                                                                                           <* 
*>       /+---(adjust widths)---------------+/                                                                        <* 
*>       /+> my.glyph.xoff -= my.margin;                                                   <*                         <* 
*>        *> my.glyph.xpos  -= my.margin;                                                   <*                        <* 
*>        *> my.glyph.wide  += (my.margin + my.margin);                                     <+/                       <* 
*>       /+---(load glyph)------------------+/                                                                        <* 
*>       my.glyph_table [k].code        = my.glyph.code;                                                              <* 
*>       my.glyph_table [k].xpos        = my.glyph.xpos;                                                              <* 
*>       my.glyph_table [k].ypos        = my.glyph.ypos;                                                              <* 
*>       my.glyph_table [k].wide        = my.glyph.wide;                                                              <* 
*>       my.glyph_table [k].tall        = my.glyph.tall;                                                              <* 
*>       my.glyph_table [k].xoff       = my.glyph.xoff;                                                               <* 
*>       my.glyph_table [k].yoff       = my.glyph.yoff;                                                               <* 
*>       my.glyph_table [k].adv        = my.glyph.adv;                                                                <* 
*>       if (strchr ("mM", glist [k].reason) != NULL)  my.glyph_table [k].code = glist [k].final;                     <* 
*>       /+---(done)------------------------+/                                                                        <* 
*>       ++c;                                                                                                         <* 
*>    }                                                                                                               <* 
*>    /+---(save statistics)----------------+/                                                                        <* 
*>    my.texw      +=  16 - (my.texw %  16);   /+ get a clean mulpilier  +/                                           <* 
*>    my.texh       =  cum_t + max_h + (my.margin + 1);                                                               <* 
*>    my.texh      +=  16 - (my.texh %  16);   /+ get a clean mulpilier  +/                                           <* 
*>    my.ascent     =  max_a;                                                                                         <* 
*>    my.descent    =  max_d;                                                                                         <* 
*>    my.nglyphs    =  c;                                                                                             <* 
*>    my.ch_min     =  ch_min;                                                                                        <* 
*>    my.ch_max     =  ch_max;                                                                                        <* 
*>    /+---(output summary)-----------------+/                                                                        <* 
*>    printf("\n");                                                                                                   <* 
*>    printf("exp glyphs  =  %7d\n", nglist);                                                                         <* 
*>    printf("act glyphs  =  %7d\n", c);                                                                              <* 
*>    printf("max width   =  %7d\n", max_w);                                                                          <* 
*>    printf("max height  =  %7d\n", max_h);                                                                          <* 
*>    printf("max ascent  =  %7d\n", my.ascent);                                                                      <* 
*>    printf("max descent =  %7d\n", my.descent);                                                                     <* 
*>    printf("min char    =  %7d\n", my.ch_min);                                                                      <* 
*>    printf("max char    =  %7d\n", my.ch_max);                                                                      <* 
*>    printf("tex width   =  %7d\n", my.texw);                                                                        <* 
*>    printf("tex height  =  %7d\n", my.texh);                                                                        <* 
*>    printf("tex bytes   =  %7d\n", my.texh * my.texw);                                                              <* 
*>    printf("\n");                                                                                                   <* 
*>    /+---(complete)-----------------------+/                                                                        <* 
*>    DEBUG_TOPS   yLOG_exit    (__FUNCTION__);                                                                       <* 
*>    return 0;                                                                                                       <* 
*> }                                                                                                                  <*/

/*> char                                                                                                                                                                                                                                                <* 
 *> MAKE__image        (int a_index)                                                                                                                                                                                                                    <* 
 *> {                                                                                                                                                                                                                                                   <* 
 *>    DEBUG_TOPS   yLOG_enter   (__FUNCTION__);                                                                                                                                                                                                        <* 
 *>    ulong     texloc, bitloc;                                                                                                                                                                                                                        <* 
 *>    int       i, j;                                                                                                                                                                                                                                  <* 
 *>    int       k;                                                                                                                                                                                                                                     <* 
 *>    int       width, offset;                                                                                                                                                                                                                         <* 
 *>    uchar     value         = 0;                                                                                                                                                                                                                     <* 
 *>    int       x_top         = 0;                                                                                                                                                                                                                     <* 
 *>    int       x_left        = 0;                                                                                                                                                                                                                     <* 
 *>    int       x_width       = 0;                                                                                                                                                                                                                     <* 
 *>    int       x_height      = 0;                                                                                                                                                                                                                     <* 
 *>    int       x_ctop        = 3000;                                                                                                                                                                                                                  <* 
 *>    int       x_clef        = 3000;                                                                                                                                                                                                                  <* 
 *>    int       x_cbot        = -1;                                                                                                                                                                                                                    <* 
 *>    int       x_crig        = -1;                                                                                                                                                                                                                    <* 
 *>    DEBUG_OUTP   yLOG_value   ("texh"      , my.texh);                                                                                                                                                                                               <* 
 *>    DEBUG_OUTP   yLOG_value   ("texw"      , my.texw);                                                                                                                                                                                               <* 
 *>    DEBUG_OUTP   yLOG_value   ("size"      , my.texh * my.texw);                                                                                                                                                                                     <* 
 *>    DEBUG_OUTP   yLOG_value   ("height"    , my.glyph.tall);                                                                                                                                                                                         <* 
 *>    DEBUG_OUTP   yLOG_value   ("width"     , my.glyph.wide);                                                                                                                                                                                         <* 
 *>    DEBUG_OUTP   yLOG_value   ("x"         , my.glyph.xpos);                                                                                                                                                                                         <* 
 *>    DEBUG_OUTP   yLOG_value   ("y"         , my.glyph.ypos);                                                                                                                                                                                         <* 
 *>    for (i = 0; i < my.glyph.tall; ++i) {                                                                                                                                                                                                            <* 
 *>       DEBUG_OUTP   yLOG_value   ("row (i)"   , i);                                                                                                                                                                                                  <* 
 *>       /+> offset = (my.texh - my.glyph.ypos - i) * (my.texw);                            <+/                                                                                                                                                        <* 
 *>       x_top    = my.glyph.ypos;                                                                                                                                                                                                                     <* 
 *>       /+> x_left   = my.glyph.xpos + my.margin;                                          <*                                                                                                                                                         <* 
 *>        *> x_width  = my.glyph.wide - (my.margin * 2);                                    <+/                                                                                                                                                        <* 
 *>       x_left   = my.glyph.xpos;                                                                                                                                                                                                                     <* 
 *>       x_width  = my.glyph.wide;                                                                                                                                                                                                                     <* 
 *>       x_height = my.glyph.tall;                                                                                                                                                                                                                     <* 
 *>       offset   = (my.glyph.ypos + i) * my.texw;                                                                                                                                                                                                     <* 
 *>       /+> offset   = (my.glyph.ypos + my.glyph.tall - i) * my.texw;                         <+/                                                                                                                                                     <* 
 *>       DEBUG_OUTP   yLOG_value   ("offset"    , offset);                                                                                                                                                                                             <* 
 *>       for (j = 0; j < x_width; ++j) {                                                                                                                                                                                                               <* 
 *>          DEBUG_OUTP   yLOG_value   ("col (j)"   , j);                                                                                                                                                                                               <* 
 *>          bitloc   =  (i * x_width) + j;                                                                                                                                                                                                             <* 
 *>          DEBUG_OUTP   yLOG_value   ("source"    , bitloc);                                                                                                                                                                                          <* 
 *>          value    = (uchar) my.face->glyph->bitmap.buffer[bitloc];                                                                                                                                                                                  <* 
 *>          DEBUG_OUTP   yLOG_value   ("value"     , value);                                                                                                                                                                                           <* 
 *>          width    = x_left + j;                                                                                                                                                                                                                     <* 
 *>          DEBUG_OUTP   yLOG_value   ("width"     , width);                                                                                                                                                                                           <* 
 *>          texloc   =  offset + width;                                                                                                                                                                                                                <* 
 *>          DEBUG_OUTP   yLOG_value   ("destin"    , texloc);                                                                                                                                                                                          <* 
 *>          /+> printf ("%3d,%3d) %3d   from  %6d to %6d\n", i, j, value, bitloc, texloc);   <+/                                                                                                                                                       <* 
 *>          /+> if (value == 0) printf("  ");                                         <*                                                                                                                                                               <* 
 *>           *> else            printf ("%02x", value);                               <+/                                                                                                                                                              <* 
 *>          my.texture [texloc] = value;                                                                                                                                                                                                               <* 
 *>       }                                                                                                                                                                                                                                             <* 
 *>       /+> printf("\n");                                                            <+/                                                                                                                                                              <* 
 *>    }                                                                                                                                                                                                                                                <* 
 *>    if (strchr ("mM", glist [a_index].reason) != NULL) {                                                                                                                                                                                             <* 
 *>       fprintf (my.o, "%-4d : u=%5d, h=%3d, w=%3d\n", a_index, glist [a_index].unicode, my.glyph.tall, my.glyph.wide);                                                                                                                               <* 
 *>       for (k = 0; k <= 8; k += 4) {                                                                                                                                                                                                                 <* 
 *>          fprintf (my.o, "%-4d : u=%5d, k=%d, t=%3d, l=%3d, b=%3d, r=%3d\n", a_index, glist [a_index].unicode, k, glist [a_index].coords [0 + k], glist [a_index].coords [1 + k], glist [a_index].coords [2 + k], glist [a_index].coords [3 + k]);   <* 
 *>          x_ctop = my.glyph.tall * ((100 - glist [a_index].coords [0 + k]) / 100.0);                                                                                                                                                                 <* 
 *>          x_clef = my.glyph.wide * (glist [a_index].coords [1 + k] / 100.0);                                                                                                                                                                         <* 
 *>          x_cbot = my.glyph.tall * ((100 - glist [a_index].coords [2 + k]) / 100.0);                                                                                                                                                                 <* 
 *>          x_crig = my.glyph.wide * (glist [a_index].coords [3 + k] / 100.0);                                                                                                                                                                         <* 
 *>          fprintf (my.o, "%-4d : u=%5d, k=%d, t=%3d, l=%3d, b=%3d, r=%3d\n", a_index, glist [a_index].unicode, k, x_ctop, x_clef, x_cbot, x_crig);                                                                                                   <* 
 *>          if (x_ctop == x_cbot && x_clef == x_crig) {                                                                                                                                                                                                <* 
 *>             fprintf (my.o, "   skipping\n");                                                                                                                                                                                                        <* 
 *>             continue;                                                                                                                                                                                                                               <* 
 *>          }                                                                                                                                                                                                                                          <* 
 *>          for (i = x_ctop; i <= x_cbot; ++i) {                                                                                                                                                                                                       <* 
 *>             x_left   = my.glyph.xpos;                                                                                                                                                                                                               <* 
 *>             offset   = (my.glyph.ypos + i) * my.texw;                                                                                                                                                                                               <* 
 *>             for (j = x_clef; j < x_crig; ++j) {                                                                                                                                                                                                     <* 
 *>                width    = x_left + j;                                                                                                                                                                                                               <* 
 *>                texloc   =  offset + width;                                                                                                                                                                                                          <* 
 *>                my.texture [texloc] = 0;                                                                                                                                                                                                             <* 
 *>             }                                                                                                                                                                                                                                       <* 
 *>          }                                                                                                                                                                                                                                          <* 
 *>       }                                                                                                                                                                                                                                             <* 
 *>    }                                                                                                                                                                                                                                                <* 
 *>    DEBUG_TOPS   yLOG_exit    (__FUNCTION__);                                                                                                                                                                                                        <* 
 *>    return 0;                                                                                                                                                                                                                                        <* 
 *> }                                                                                                                                                                                                                                                   <*/

/*> char         /+--> write txf glyph table -----------------[ ------ [ ------ ]-+/   <* 
 *> MAKE__table        (void)                                                          <* 
 *> {                                                                                  <* 
 *>    /+---(begin)--------------------------+/                                        <* 
 *>    DEBUG_OUTP   yLOG_enter   (__FUNCTION__);                                       <* 
 *>    int k = 0;                                                                      <* 
 *>    for (k = 0; k < nglist; ++k) {                                                  <* 
 *>       fwrite (&my.glyph_table [k], sizeof (tGLYPH), 1, my.file);                   <* 
 *>    }                                                                               <* 
 *>    /+---(complete)-----------------------+/                                        <* 
 *>    DEBUG_OUTP   yLOG_exit    (__FUNCTION__);                                       <* 
 *>    return 0;                                                                       <* 
 *> }                                                                                  <*/

/*> char         /+--> write txf glyph table -----------------[ ------ [ ------ ]-+/            <* 
 *> MAKE__draw         (void)                                                                   <* 
 *> {                                                                                           <* 
 *>    char        rc          = 0;                                                             <* 
 *>    int         save_c      = 0;                                                             <* 
 *>    int k = 0;                                                                               <* 
 *>    /+---(begin)--------------------------+/                                                 <* 
 *>    DEBUG_OUTP   yLOG_enter   (__FUNCTION__);                                                <* 
 *>    for (k = 0; k < nglist; ++k) {                                                           <* 
 *>       DEBUG_OUTP   yLOG_value   ("character" , k);                                          <* 
 *>       DEBUG_OUTP   yLOG_value   ("unicode"   , glist[k].unicode);                           <* 
 *>       rc = FREE__find    (glist[k].unicode);                                                <* 
 *>       DEBUG_OUTP   yLOG_value   ("find_rc"   , rc);                                         <* 
 *>       my.glyph.code    = glist[k].unicode;                                                  <* 
 *>       if (rc < 0) {                                                                         <* 
 *>          my.glyph.code = ' ';                                                               <* 
 *>          /+> return rc;                                                               <+/   <* 
 *>       }                                                                                     <* 
 *>       rc = GLYPH__metrics ();                                                               <* 
 *>       DEBUG_OUTP   yLOG_value   ("metrics_rc", rc);                                         <* 
 *>       /+---(watch for space)--------------------+/                                          <* 
 *>       if (my.glyph.code == ' ') {                                                           <* 
 *>          my.glyph.code = 'a';                                                               <* 
 *>          FREE__find    (my.glyph.code);                                                     <* 
 *>          GLYPH__metrics ();                                                                 <* 
 *>          my.glyph.code = ' ';                                                               <* 
 *>       }                                                                                     <* 
 *>       if (my.glyph.code != save_c) {                                                        <* 
 *>          my.glyph.code = save_c;                                                            <* 
 *>       }                                                                                     <* 
 *>       /+---(load glyph)------------------+/                                                 <* 
 *>       my.glyph.code   = my.glyph_table[k].code;                                             <* 
 *>       my.glyph.xpos   = my.glyph_table[k].xpos;                                             <* 
 *>       my.glyph.ypos   = my.glyph_table[k].ypos;                                             <* 
 *>       my.glyph.wide   = my.glyph_table[k].wide;                                             <* 
 *>       my.glyph.tall   = my.glyph_table[k].tall;                                             <* 
 *>       my.glyph.xoff  = my.glyph_table[k].xoff;                                              <* 
 *>       my.glyph.yoff  = my.glyph_table[k].yoff;                                              <* 
 *>       my.glyph.adv   = my.glyph_table[k].adv;                                               <* 
 *>       /+---(copy image)-------------------------+/                                          <* 
 *>       if (my.glyph.code != ' ')  MAKE__image(k);                                            <* 
 *>    }                                                                                        <* 
 *>    /+---(complete)-----------------------+/                                                 <* 
 *>    DEBUG_OUTP   yLOG_exit    (__FUNCTION__);                                                <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/



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
   if (rc >= 0)  rc = yURG_logger (argc, argv);
   if (rc >= 0)  rc = PROG_init   ();
   if (rc >= 0)  rc = yURG_urgs   (argc, argv);
   if (rc >= 0)  rc = PROG_args   (argc, argv);
   if (rc <  0) {
      return rc;
   }
   /*---(begin)--------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   /*---(set up font)--------------------*/
   if (rc >= 0)  rc = yFONT__conf_parse    ();
   if (rc >= 0)  rc = yFONT__slot_new      ();
   if (rc >= 0)  x_slot = rc;
   if (rc >= 0)  rc = FONT__choose         (x_slot);
   if (rc >= 0)  rc = FREETYPE__setup      (my.src_file, my.point + my.adjust);
   if (rc >= 0)  rc = GLIST__load          (x_slot, my.glist);
   if (rc >= 0)  rc = INDEX__load          (x_slot);
   if (rc >= 0)  rc = INDEX__layout        (x_slot);
   if (rc >= 0)  rc = TEX__draw            (x_slot);
   if (rc <  0) {
      return rc;
   }
   yFONT__head_dump  (x_slot);
   yFONT__index_dump (x_slot);


   FREETYPE__shutdown   ();
   yFONT__slot_free     (x_slot);

   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   exit (0);

   /*---(write txf header)----------------------*/
   if (rc >= 0)  rc = yFONT__file_open  (x_slot, 'w');
   if (rc >= 0)  rc = yFONT__head_write (x_slot);
   if (rc <  0) {
      return rc;
   }
   /*> MAKE__table ();                                                                <*/
   /*> /+---(allocate the texture)------------------+/                                                    <* 
    *> DEBUG_OUTP   yLOG_note    ("allocating texture");                                                  <* 
    *> my.texture = (uchar *) malloc (my.texw * my.texh * sizeof (uchar));                                <* 
    *> DEBUG_OUTP   yLOG_point   ("texture"   , my.texture);                                              <* 
    *> if (my.texture == NULL) {                                                                          <* 
    *>    printf("FATAL: calloc could not allocate memory for texture\n");                                <* 
    *>    return -10;                                                                                     <* 
    *> }                                                                                                  <* 
    *> x_max = my.texw * my.texh;                                                                         <* 
    *> for (i = 0; i < x_max; ++i)  my.texture [i] = 0;                                                   <* 
    *> /+---(write the per glyph info)--------------+/                                                    <* 
    *> MAKE__draw ();                                                                                     <* 
    *> fwrite (my.texture, my.texh * my.texw, 1, my.file);                                                <* 
    *> fclose (my.file);                                                                                  <* 
    *> printf ("header      =  %7d\n",  8 * 4);                                                           <* 
    *> printf ("glyphs      =  %7d\n",  my.nglyphs * sizeof(tGLYPH));                                     <* 
    *> printf ("texture     =  %7d\n",  my.texh * my.texw);                                               <* 
    *> printf ("TOTAL       =  %7d\n",  (8 * 4) + (my.nglyphs * sizeof(tGLYPH)) + (my.texh * my.texw));   <*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}



/*============================----end-of-source---============================*/
