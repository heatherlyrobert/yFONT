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
   tFONT      *x_font      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(allocate structure)-------------*/
   x_font = g_font [a_slot];
   DEBUG_YFONT_M  yLOG_note    ("allocating font data structure");
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
         DEBUG_YFONT_M  yLOG_warn    ("malloc"    , "could not allocate a new font in 10 tries");
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
   tFONT      *x_font      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(defense)------------------------*/
   DEBUG_YFONT_M  yLOG_point   ("g_font[.]" , g_font [a_slot]);
   --rce;  if (g_font [a_slot] == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font ptr"  , "already null pointer, nothing to do");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   x_font  = g_font [a_slot];
   /*---(free it up)----------------------------*/
   DEBUG_YFONT_M  yLOG_note    ("freeing glyph index");
   free (x_font->glyphs);
   DEBUG_YFONT_M  yLOG_note    ("nulling slot entry");
   x_font->glyphs = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yFONT__index_init    (char  a_slot,  int a_entry)
{
   tFONT      *x_font      = NULL;
   x_font = g_font [a_slot];
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
   tFONT      *x_font      = NULL;
   x_font = g_font [a_slot];
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

char
yFONT__index_code       (char  a_slot,  int a_entry, int a_code, char a_good)
{
   g_font[a_slot]->glyphs[a_entry].code       = a_code;
   g_font[a_slot]->glyphs[a_entry].good       = a_good;
   if (a_code > s_max_code   )  s_max_code    = a_code;
   if (a_code < s_min_code   )  s_min_code    = a_code;
   return 0;
}

char
yFONT__index_size       (char  a_slot,  int a_entry, char a_wide, char a_tall)
{
   g_font[a_slot]->glyphs[a_entry].wide       = a_wide;
   g_font[a_slot]->glyphs[a_entry].tall       = a_tall;
   if (a_wide > s_max_wide   )  s_max_wide    = a_wide;
   if (a_tall > s_max_tall   )  s_max_tall    = a_tall;
   return 0;
}

char
yFONT__index_offset     (char  a_slot,  int a_entry, char a_xoff, char a_yoff, char a_adv)
{
   char     x_descent   = 0;
   g_font[a_slot]->glyphs[a_entry].xoff       = a_xoff;
   g_font[a_slot]->glyphs[a_entry].yoff       = a_yoff;
   g_font[a_slot]->glyphs[a_entry].adv        = a_adv;
   if (a_yoff > s_max_ascent )  s_max_ascent  = a_yoff;
   x_descent = a_yoff - g_font[a_slot]->glyphs[a_entry].tall;
   if (x_descent < s_max_descent)  s_max_descent = x_descent;
   return 0;
}

char
yFONT__index_pos        (char  a_slot,  int a_entry, short a_xpos, short a_ypos)
{
   g_font[a_slot]->glyphs[a_entry].xpos    = a_xpos;
   g_font[a_slot]->glyphs[a_entry].ypos    = a_ypos;
   return 0;
}

char
yFONT__index_maxes      (char  a_slot)
{
   yFONT__head_place  (a_slot, s_max_ascent, s_max_descent);
   return  s_max_tall;
}

char
yFONT__index_wide       (char  a_slot, int  a_entry)
{
   return  g_font[a_slot]->glyphs[a_entry].wide;
}

char
yFONT__index_dump       (char  a_slot)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   tFONT      *x_font      = NULL;
   x_font = g_font [a_slot];
   for (i = 0; i < x_font->num_glyph; ++i) {
      if (i % 5 == 0)  printf ("\n   order  unicode  c  xpos  ypos  wide  tall  xoff  yoff  adv  good\n\n");
      printf ("   %5d  %7d  %c  %4d %4d %5d %5d %5d %5d %5d   %c\n", i,
            x_font->glyphs[i].code , 
            (x_font->glyphs[i].code < 128) ? x_font->glyphs[i].code : '-',
            x_font->glyphs[i].xpos , x_font->glyphs[i].ypos , 
            x_font->glyphs[i].wide , x_font->glyphs[i].tall , 
            x_font->glyphs[i].xoff , x_font->glyphs[i].yoff ,
            x_font->glyphs[i].adv  , x_font->glyphs[i].good );
   }
}

char
yFONT__index_read       (char  a_slot)
{
}

char
yFONT__index_write      (char  a_slot)
{
}
