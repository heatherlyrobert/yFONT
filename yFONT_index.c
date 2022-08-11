/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"


static      int         s_max_code       = -100;
static      int         s_min_code       = -100;
static      char        s_max_wide       = -100;
static      char        s_max_tall       = -100;
static      char        s_max_ascent     = -100;
static      char        s_max_descent    =  100;


char         /*--> allocate a glyph index ----------------[ ------ [ ------ ]-*/
yFONT__index_alloc   (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        x_tries     =   0;
   tYFONT      *x_font      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(allocate structure)-------------*/
   x_font = g_yfont [a_slot];
   DEBUG_YFONT_M  yLOG_note    ("allocating font table data structure");
   DEBUG_YFONT_M  yLOG_value   ("nglyph"    , x_font->num_glyph);
   --rce;  while (x_font->glyphs == NULL) {
      ++x_tries;
      DEBUG_YFONT_M  yLOG_value   ("x_tries"   , x_tries);
      x_font->glyphs = (tGLYPH *) malloc (x_font->num_glyph * sizeof (tGLYPH));
      DEBUG_YFONT_M  yLOG_point   ("*glyphs"   , x_font->glyphs);
      if (x_font->glyphs != NULL) {
         DEBUG_YFONT_M  yLOG_note    ("got it, done");
         break;
      }
      if (x_tries > 10) {
         DEBUG_YFONT_M  yLOG_warn    ("malloc could not allocate a new font in 10 tries");
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> free up a font entry ------------------[ ------ [ ------ ]-*/
yFONT__index_free    (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        x_tries     =   0;
   tYFONT      *x_font      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(defense)------------------------*/
   DEBUG_YFONT_M  yLOG_point   ("g_yfont[.]" , g_yfont [a_slot]);
   --rce;  if (g_yfont [a_slot] == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font ptr already null pointer, nothing to do");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   x_font  = g_yfont [a_slot];
   /*---(free it up)----------------------------*/
   DEBUG_YFONT_M  yLOG_note    ("freeing glyph index");
   if (x_font->glyphs  != NULL)  free (x_font->glyphs);
   DEBUG_YFONT_M  yLOG_note    ("nulling slot entry");
   x_font->glyphs = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yFONT__index_init    (char  a_slot,  int a_entry)
{
   tYFONT      *x_font      = NULL;
   x_font = g_yfont [a_slot];
   if (a_entry >= x_font->num_glyph)  return -1;
   x_font->glyphs[a_entry].code   =  -1;
   x_font->glyphs[a_entry].good   = '-';
   x_font->glyphs[a_entry].xpos   =   0;
   x_font->glyphs[a_entry].ypos   =   0;
   x_font->glyphs[a_entry].wide   =   0;
   x_font->glyphs[a_entry].tall   =   0;
   x_font->glyphs[a_entry].xoff   =   0;
   x_font->glyphs[a_entry].yoff   =   0;
   x_font->glyphs[a_entry].adv    =   0;
   return 0;
}

char
yFONT__index_wipe    (char  a_slot)
{
   tYFONT      *x_font      = NULL;
   x_font = g_yfont [a_slot];
   if (x_font->glyphs == NULL)  return -1;
   int i = 0;
   for (i = 0; i < x_font->num_glyph; ++i) {
      yFONT__index_init (a_slot, i);
   }
   s_max_code    =   -100;
   s_min_code    =    100;
   s_max_wide    =   -100;   /* greatest character width            */
   s_max_tall    =   -100;   /* greatest character height           */
   s_max_ascent  =   -100;   /* greatest rise above baseline        */
   s_max_descent =    100;   /* greatest fall below baseline        */
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     setting header values                    ----===*/
/*====================------------------------------------====================*/
static void      o___SETTERS_________________o (void) {;}

char
yFONT__index_code       (char  a_slot,  int a_entry, int a_code, char a_good)
{
   g_yfont[a_slot]->glyphs[a_entry].code       = a_code;
   g_yfont[a_slot]->glyphs[a_entry].good       = a_good;
   if (a_code > s_max_code   )  s_max_code     = a_code;
   if (a_code < s_min_code   )  s_min_code     = a_code;
   return 0;
}

char
yFONT__index_size       (char  a_slot,  int a_entry, char a_wide, char a_tall)
{
   g_yfont[a_slot]->glyphs[a_entry].wide       = a_wide;
   g_yfont[a_slot]->glyphs[a_entry].tall       = a_tall;
   if (a_wide > s_max_wide   )  s_max_wide     = a_wide;
   if (a_tall > s_max_tall   )  s_max_tall     = a_tall;
   return 0;
}

char
yFONT__index_offset     (char  a_slot,  int a_entry, char a_xoff, char a_yoff, char a_adv)
{
   char     x_descent   = 0;
   g_yfont[a_slot]->glyphs[a_entry].xoff       = a_xoff;
   g_yfont[a_slot]->glyphs[a_entry].yoff       = a_yoff;
   g_yfont[a_slot]->glyphs[a_entry].adv        = a_adv;
   if (a_yoff > s_max_ascent )  s_max_ascent   = a_yoff;
   x_descent = a_yoff - g_yfont[a_slot]->glyphs[a_entry].tall;
   if (x_descent < s_max_descent)  s_max_descent = x_descent;
   return 0;
}

char
yFONT__index_pos        (char  a_slot,  int a_entry, short a_xpos, short a_ypos)
{
   g_yfont[a_slot]->glyphs[a_entry].xpos       = a_xpos;
   g_yfont[a_slot]->glyphs[a_entry].ypos       = a_ypos;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     Getting header values                    ----===*/
/*====================------------------------------------====================*/
static void      o___GETTERS_________________o (void) {;}

char
yFONT__index_maxes      (char  a_slot)
{
   yFONT__head_place  (a_slot, s_max_ascent, s_max_descent);
   return  s_max_tall;
}

char
yFONT__index_wide       (char  a_slot, int  a_entry)
{
   return  g_yfont[a_slot]->glyphs[a_entry].wide;
}

char
yFONT__index_who        (char  a_slot, int  a_entry, int *a_code, char *a_good)
{
   if (a_code   != NULL)  *a_code   = g_yfont[a_slot]->glyphs[a_entry].code;
   if (a_good   != NULL)  *a_good   = g_yfont[a_slot]->glyphs[a_entry].good;
   return  0;
}

char
yFONT__index_coords     (char  a_slot, int  a_entry, short *a_xpos, short *a_ypos, char *a_wide, char *a_tall)
{
   if (a_xpos   != NULL)  *a_xpos   = g_yfont[a_slot]->glyphs[a_entry].xpos;
   if (a_ypos   != NULL)  *a_ypos   = g_yfont[a_slot]->glyphs[a_entry].ypos;
   if (a_wide   != NULL)  *a_wide   = g_yfont[a_slot]->glyphs[a_entry].wide;
   if (a_tall   != NULL)  *a_tall   = g_yfont[a_slot]->glyphs[a_entry].tall;
   return  0;
}



/*====================------------------------------------====================*/
/*===----                           reporting                          ----===*/
/*====================------------------------------------====================*/
static void      o___REPORTING_______________o (void) {;}

char
yFONT__index_dump       (char  a_slot)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   tYFONT     *x_font      = NULL;
   uchar       x_ch        =  '-';
   x_font = g_yfont [a_slot];
   printf ("\n");
   for (i = 0; i < x_font->num_glyph; ++i) {
      if (i % 5 == 0)  printf ("   order  unicode  c  xpos  ypos  wide  tall  xoff  yoff  adv  good\n");
      if      (x_font->glyphs[i].code <   32)  x_ch = '´';
      else if (x_font->glyphs[i].code == 127)  x_ch = '´';
      else                                     x_ch = x_font->glyphs [i].code;
      printf ("   %5d  %7d  %c  %4d %4d %5d %5d %5d %5d %5d   %c\n", i,
            x_font->glyphs[i].code , x_ch,
            x_font->glyphs[i].xpos , x_font->glyphs[i].ypos , 
            x_font->glyphs[i].wide , x_font->glyphs[i].tall , 
            x_font->glyphs[i].xoff , x_font->glyphs[i].yoff ,
            x_font->glyphs[i].adv  , x_font->glyphs[i].good );
   }
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      reading and writing                     ----===*/
/*====================------------------------------------====================*/
static void      o___FILES___________________o (void) {;}

char         /*--> write the font index ------------------[ ------ [ ------ ]-*/
yFONT__index_write   (char a_slot)
{
   /*---(locals)--------------------------------*/
   char      rce       = -10;               /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   tYFONT    *x_font    = NULL;              /* new font                       */
   int         i           = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(check file type)-----------------------*/
   x_font = g_yfont [a_slot];
   /*---(write map info)-----------------*/
   for (i = 0; i < x_font->num_glyph; ++i) {
      fwrite (&x_font->glyphs [i], sizeof (tGLYPH), 1, x_font->file);
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yFONT__index_read       (char  a_slot)
{
   /*---(locals)--------------------------------*/
   char      rce       = -10;               /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   tYFONT    *x_font    = NULL;              /* new font                       */
   int         i           = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(check file type)-----------------------*/
   x_font = g_yfont [a_slot];
   /*---(write map info)-----------------*/
   for (i = 0; i < x_font->num_glyph; ++i) {
      fread  (&x_font->glyphs [i], sizeof (tGLYPH), 1, x_font->file);
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: layout the characters for texture -----[ ------ [ ------ ]-*/
yFONT_layout_force      (char a_slot, char a_margin, short a_gwide, short a_gtall)
{
   /*---(locals)-----------+-----------+-*/
   int         n           =      0;   /* iterator -- gylphs                  */
   int         x, y;                   /* iterator -- gylphs                  */
   int         x_pos       =      0;   /* current width                       */
   int         y_pos       =      0;   /* current height                      */
   /*---(header)-------------------------*/
   DEBUG_TOPS   yLOG_enter   (__FUNCTION__);
   DEBUG_VIEW     printf       ("%s\n", __FUNCTION__);
   /*---(initialize)---------------------*/
   DEBUG_INPT   yLOG_value   ("a_gwide"   , a_gwide);
   DEBUG_INPT   yLOG_value   ("a_gtall"   , a_gtall);
   /*---(measure all glyphs)-------------*/
   for (y = 0; y < 16; ++y) {
      for (x = 0; x < 16; ++x) {
         y_pos  = a_margin + 1 + (y * (a_gtall + a_margin));
         x_pos  = a_margin + 1 + (x * (a_gwide + a_margin));
         DEBUG_INPT   yLOG_complex ("char"      , "%3d#, %4dx, %4dy", n, x_pos, y_pos);
         yFONT__index_pos   (a_slot, n, x_pos, y_pos);
         ++n;
         /*---(done)------------------------*/
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_TOPS   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yFONT_index_force       (char a_slot, char a_gwide, char a_gtall)
{
   char        rc          =    0;
   int         i           = 0;
   tYFONT    *x_font    = NULL;              /* new font                       */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(allocate)------------------------------*/
   rc = yFONT__index_alloc   (a_slot);
   /*---(check file type)-----------------------*/
   x_font = g_yfont [a_slot];
   /*---(fill out index)-----------------*/
   for (i = 0; i < x_font->num_glyph; ++i) {
      /*---(lookup)--------------*/
      x_font->glyphs [i].code   = i;
      x_font->glyphs [i].good   = 'y';
      /*---(texture)-------------*/
      x_font->glyphs [i].xpos   = 0;
      x_font->glyphs [i].ypos   = 0;
      x_font->glyphs [i].wide   = a_gwide;
      x_font->glyphs [i].tall   = a_gtall;
      /*---(texture)-------------*/
      x_font->glyphs [i].xoff   = 0;
      x_font->glyphs [i].yoff   = x_font->max_ascent;
      x_font->glyphs [i].adv    = a_gwide + x_font->margin;
   }
   /*---(layout)-------------------------*/
   rc = yFONT_layout_force  (a_slot, x_font->margin, a_gwide, a_gtall);
   /*---(write to file)------------------*/
   rc = yFONT__index_write  (a_slot);
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      creating the lookup                     ----===*/
/*====================------------------------------------====================*/
static void      o___LOOKUP__________________o (void) {;}

char              /*--> create lookup table -------------------[ ------ [ ------ ]-*/
yFONT__index_lookup  (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;               /* return code for errors         */
   tYFONT      *x_font      = NULL;              /* new font                       */
   int         i           = 0;
   int         x_min       = 0;
   int         x_max       = 0;
   int         x_range     = 0;
   char        x_tries     =   0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(set font pointer)---------------*/
   x_font = g_yfont [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("x_font"    , x_font);
   /*---(initialize)---------------------*/
   x_min = x_font->glyphs[0].code;
   x_max = x_font->glyphs[0].code;
   /*---(identify range)-----------------*/
   for (i = 0; i < x_font->num_glyph; i++) {
      if (x_font->glyphs[i].code < x_min) {
         x_min = x_font->glyphs[i].code;
      }
      if (x_font->glyphs[i].code > x_max) {
         x_max = x_font->glyphs[i].code;
      }
   }
   x_range           = x_max - x_min + 1;
   /*---(save values)--------------------*/
   x_font->min_glyph = x_min;
   x_font->max_glyph = x_max;
   x_font->range     = x_range;
   DEBUG_YFONT_M  yLOG_value   ("min_glyph" , x_min);
   DEBUG_YFONT_M  yLOG_value   ("max_glyph" , x_max);
   DEBUG_YFONT_M  yLOG_value   ("range"     , x_range);
   /*---(create lookup table)------------*/
   DEBUG_YFONT_M  yLOG_note    ("allocate lookup table");
   --rce;  while (x_font->lookup == NULL) {
      ++x_tries;
      x_font->lookup = (short  *) malloc (x_font->range * sizeof (short));
      if (x_font->lookup != NULL) {
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         break;
      }
      if (x_tries > 10) {
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(initialize lookup table)--------*/
   DEBUG_YFONT_M  yLOG_note    ("initialize table");
   for (i = 0; i < x_font->range; i++)  x_font->lookup [i] = -1;
   /*---(load lookup table)--------------*/
   DEBUG_YFONT_M  yLOG_note    ("load font values into table");
   for (i = 0; i < x_font->num_glyph; i++) {
      x_font->lookup [x_font->glyphs[i].code - x_font->min_glyph] = i;
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

int         /* PURPOSE : determine the width of a displayed string     */
yFONT__index_widthu  (tYFONT *a_font, int *a_array, int a_len)
{
   int         i           = 0;                 /* iterator -- character          */
   int         w           = 0;                 /* width                          */
   tVERT      *x_vert;
   for (i = 0; i < a_len; i++) {
      x_vert = yFONT__verts_find  (a_font, a_array[i]);
      if (x_vert == NULL) continue;
      w += x_vert->a;
   }
   return w;
}

int         /* PURPOSE : determine the width of a displayed string     */
yFONT__index_width   (tYFONT *a_font, char *a_text, int a_len)
{
   int         i           = 0;                 /* iterator -- character          */
   int         w           = 0;                 /* width                          */
   tVERT      *x_vert;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_point   ("a_font"    , a_font);
   DEBUG_YFONT_M  yLOG_point   ("a_text"    , a_text);
   DEBUG_YFONT_M  yLOG_info    ("a_text"    , a_text);
   DEBUG_YFONT_M  yLOG_value   ("a_len"     , a_len);
   for (i = 0; i < a_len; i++) {
      DEBUG_YFONT_M  yLOG_value   ("a_text[i]" , a_text[i]);
      x_vert = yFONT__verts_find  (a_font, a_text[i]);
      DEBUG_YFONT_M  yLOG_point   ("x_ver"     , x_vert);
      if (x_vert == NULL)  continue;
      DEBUG_YFONT_M  yLOG_value   ("x_vert->a" , x_vert->a);
      w += x_vert->a;
      DEBUG_YFONT_M  yLOG_value   ("w"         , w);
   }
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return w;
}
