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
   tFONT      *x_txf       = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YFONT_M  yLOG_point   ("g_font[.]" , g_font [a_slot]);
   --rce;  if (g_font [a_slot] != NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font ptr"  , "pointer must start as null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(allocate structure)-------------*/
   DEBUG_YFONT_M  yLOG_note    ("allocating font data structure");
   --rce;  while (x_txf == NULL) {
      ++x_tries;
      x_txf = (tFONT *) malloc (sizeof (tFONT));
      if (x_tries > 10) {
         DEBUG_YFONT_M  yLOG_warn    ("malloc"    , "could not allocate a new font in 10 tries");
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(assign to slot)-----------------*/
   DEBUG_YFONT_M  yLOG_point   ("*x_txf"    , x_txf);
   g_font [a_slot] = x_txf;
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
   /*---(file)---------------------------*/
   DEBUG_YFONT_M  yLOG_note    ("clear file variables");
   g_font [a_slot]->file         = NULL;
   strlcpy (g_font [a_slot]->name, "", LEN_LABEL);
   g_font [a_slot]->slot         = a_slot;
   /*---(characteristics)----------------*/
   DEBUG_YFONT_M  yLOG_note    ("clear characteristics");
   g_font [a_slot]->point        = 0;
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
   g_font [a_slot]->width        = 0;
   g_font [a_slot]->height       = 0;
   g_font [a_slot]->tex_ref      = 0;
   g_font [a_slot]->texture      = NULL;
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

char         /*--> free up a font entry ------------------[ ------ [ ------ ]-*/
yFONT__slot_free   (char a_slot)
{
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}


/*============================----end-of-source---============================*/
