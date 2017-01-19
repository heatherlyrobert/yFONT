/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"



tFONT       *g_font         [MAX_FONT] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  NULL};



char         /*--> setup a new slot/font entry -----------[ ------ [ ------ ]-*/
yFONT__slot_new    (void)
{
   /*---(locals)-----------+-----------+-*/
   char        x_slot      =  -1;           /* open font slot                 */
   char        rc          =   0;           /* generic return code            */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(next)---------------------------*/
   if (rc >= 0)  rc     = yFONT__slot_next    ();
   if (rc >= 0)  x_slot = rc;
   if (rc >= 0)  rc     = yFONT__slot_alloc   (x_slot);
   if (rc >= 0)  rc     = yFONT__slot_init    (x_slot);
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   if (rc >= 0)  return x_slot;
   return rc;
}

char         /*--> find an open slot ---------------------[ ------ [ ------ ]-*/
yFONT__slot_next   (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   int         i           =   0;           /* font slot iterator             */
   char        x_slot      =  -1;           /* open font slot                 */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(look for open font slot)--------*/
   DEBUG_YFONT_M  yLOG_note    ("searching font slots");
   for (i = 0; i < MAX_FONT; ++i) {
      if (g_font [i] != NULL) {
         DEBUG_YFONT_M  yLOG_bullet  (i           , "occupied (not NULL)");
         continue;
      }
      DEBUG_YFONT_M  yLOG_bullet  (i           , "slot available (NULL)");
      x_slot    = i;
      break;
   }
   if (x_slot <  0) {
      DEBUG_YFONT_M  yLOG_warn    ("font slot" , "no slots available");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return x_slot;
}

char         /*--> allocate a font to a slot -------------[ ------ [ ------ ]-*/
yFONT__slot_alloc  (char a_slot)
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
   --rce;  if (g_font [a_slot] != NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font ptr"  , "pointer must start as null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(allocate structure)-------------*/
   DEBUG_YFONT_M  yLOG_note    ("allocating font data structure");
   --rce;  while (x_font == NULL) {
      ++x_tries;
      x_font = (tFONT *) malloc (sizeof (tFONT));
      if (x_tries > 10) {
         DEBUG_YFONT_M  yLOG_warn    ("malloc"    , "could not allocate a new font in 10 tries");
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(assign to slot)-----------------*/
   DEBUG_YFONT_M  yLOG_point   ("*x_font"   , x_font);
   g_font [a_slot] = x_font;
   DEBUG_YFONT_M  yLOG_point   ("g_font[.]" , g_font [a_slot]);
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> initialize a new font entry -----------[ ------ [ ------ ]-*/
yFONT__slot_init   (char a_slot)
{
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(file)---------------------------*/
   DEBUG_YFONT_M  yLOG_note    ("clear file variables");
   g_font [a_slot]->slot         = a_slot;
   g_font [a_slot]->file         = NULL;
   strlcpy (g_font [a_slot]->version, "                    ", LEN_LABEL);
   strlcpy (g_font [a_slot]->version, ""                    , LEN_LABEL);
   strlcpy (g_font [a_slot]->name   , "                    ", LEN_LABEL);
   strlcpy (g_font [a_slot]->name   , ""                    , LEN_LABEL);
   /*---(characteristics)----------------*/
   DEBUG_YFONT_M  yLOG_note    ("clear characteristics");
   g_font [a_slot]->point        = 0;
   g_font [a_slot]->format       = '-';
   g_font [a_slot]->style        = '-';
   g_font [a_slot]->max_ascent   = 0;
   g_font [a_slot]->max_descent  = 0;
   g_font [a_slot]->margin       = 0;
   /*---(working)------------------------*/
   DEBUG_YFONT_M  yLOG_note    ("clear working variables");
   g_font [a_slot]->min_glyph    = 0;
   g_font [a_slot]->max_glyph    = 0;
   g_font [a_slot]->range        = 0;
   /*---(texture)------------------------*/
   DEBUG_YFONT_M  yLOG_note    ("clear texture variables");
   g_font [a_slot]->tex_w        = 0;
   g_font [a_slot]->tex_h        = 0;
   g_font [a_slot]->tex_ref      = 0;
   g_font [a_slot]->tex_bits     = NULL;
   /*---(glyphs)-------------------------*/
   DEBUG_YFONT_M  yLOG_note    ("clear glyph variables");
   g_font [a_slot]->num_glyph    = 0;
   g_font [a_slot]->glyphs       = NULL;
   g_font [a_slot]->verts        = NULL;
   g_font [a_slot]->lookup       = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> free up all fonts ---------------------[ ------ [ ------ ]-*/
yFONT__slot_purge  (void)
{
   char        i           = 0;
   for (i = 0; i < MAX_FONT; ++i) {
      yFONT__slot_free (i);
   }
   return 0;
}

char         /*--> free up a font entry ------------------[ ------ [ ------ ]-*/
yFONT__slot_free   (char a_slot)
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
   DEBUG_YFONT_M  yLOG_note    ("checking/clearing texture");
   if (x_font->tex_bits )   free (x_font->tex_bits);
   DEBUG_YFONT_M  yLOG_note    ("checking/clearing glyph table");
   if (x_font->glyphs   )   free (x_font->glyphs  );
   DEBUG_YFONT_M  yLOG_note    ("checking/clearing vert table");
   if (x_font->verts    )   free (x_font->verts   );
   DEBUG_YFONT_M  yLOG_note    ("checking/clearing lookup table");
   if (x_font->lookup   )   free (x_font->lookup  );
   DEBUG_YFONT_M  yLOG_note    ("freeing font");
   free (g_font [a_slot]);
   DEBUG_YFONT_M  yLOG_note    ("nulling slot entry");
   g_font [a_slot] = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

tFONT*       /*--> test a slot reference -----------------[ ------ [ ------ ]-*/
yFONT__slot_font   (char a_slot)
{
   /*---(locals)--------------------------------*/
   tFONT      *x_font      = NULL;          /* new font                       */
   /*---(defense : font)------------------------*/
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   if (a_slot < 0 || a_slot >= MAX_FONT) {
      DEBUG_YFONT_M  yLOG_warn    ("slot"      , "not in required range (0 - MAX_FONT)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   x_font = g_font [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("x_font"    , x_font);
   if (x_font == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("name"      , "can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return NULL;
   }
   return x_font;
}

/*============================----end-of-source---============================*/
