/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"


char         /*--> clear a font map ----------------------[ ------ [ ------ ]-*/
yFONT__map_clear     (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   tFONT      *x_font      = NULL;
   int         x_max       =   0;           /* max byte in font map           */
   int         i           =   0;
   x_font = g_font [a_slot];
   x_max  = x_font->tex_w * x_font->tex_h;
   for (i = 0; i < x_max; ++i) {
      x_font->tex_map [i] = 0;
   }
   return 0;
}

char         /*--> allocate a font map -------------------[ ------ [ ------ ]-*/
yFONT__map_alloc     (char a_slot)
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
   DEBUG_YFONT_M  yLOG_note    ("allocating font map texture");
   DEBUG_YFONT_M  yLOG_value   ("width"     , x_font->tex_w);
   DEBUG_YFONT_M  yLOG_value   ("height"    , x_font->tex_h);
   DEBUG_YFONT_M  yLOG_value   ("bytes"     , x_font->tex_w * x_font->tex_h);
   --rce;  while (x_font->tex_map == NULL) {
      ++x_tries;
      DEBUG_YFONT_M  yLOG_value   ("x_tries"   , x_tries);
      x_font->tex_map = (uchar  *) malloc (x_font->tex_w * x_font->tex_h);
      DEBUG_YFONT_M  yLOG_point   ("*tex_map"  , x_font->tex_map);
      if (x_font->tex_map != NULL) {
         DEBUG_YFONT_M  yLOG_note    ("got it, done");
         break;
      }
      if (x_tries > 10) {
         DEBUG_YFONT_M  yLOG_warn    ("malloc"    , "could not allocate a new font map in 10 tries");
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> free up a font map --------------------[ ------ [ ------ ]-*/
yFONT__map_free      (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        x_tries     =   0;
   tFONT      *x_font      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(free it up)----------------------------*/
   x_font  = g_font [a_slot];
   DEBUG_YFONT_M  yLOG_note    ("freeing font map");
   free (x_font->tex_map);
   DEBUG_YFONT_M  yLOG_note    ("nulling slot entry");
   x_font->tex_map = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}



