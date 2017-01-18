/*============================----beg-of-source---============================*/

#include "yFONT.h"
#include "yFONT_priv.h"


tFONT       *g_fonts        [MAX_FONT] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,  NULL};


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
      if (g_fonts [i] != NULL) {
         DEBUG_YFONT_M  yLOG_pair    (i           , "occupied (not NULL)");
         continue;
      }
      DEBUG_YFONT_M  yLOG_pair    (i           , "slot available (NULL)");
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
   DEBUG_YFONT_M  yLOG_info    ("allocating font data structure");
   while (x_txf == NULL) {
      ++x_tries;
      x_txf = (tFONT *) malloc (sizeof (tFONT));
      if (x_tries > 10) {
         DEBUG_YFONT_M  yLOG_warn    ("malloc"    , "could not allocate a new font in 10 tries");
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         return NULL;
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

char
yFONT__slot_valid  (char a_slot)
{

}

/*============================----end-of-source---============================*/
