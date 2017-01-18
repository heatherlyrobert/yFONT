/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"



char         /*--> open a font file for reading ----------[ ------ [ ------ ]-*/
yFONT__file_open   (char a_slot, char *a_name, char a_mode)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        x_name      [1000];
   FILE       *f           = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YFONT_M  yLOG_point   ("*a_name"   , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font name" , "can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_info    ("a_name"    , a_name);
   DEBUG_YFONT_M  yLOG_char    ("a_mode"    , a_mode);
   --rce;  if (strchr ("rw", a_mode) == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("file mode" , "must be either 'r' or 'w'");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(full name)----------------------*/
   snprintf (x_name, 900, "/usr/local/share/fonts/%s.txf", a_name);
   DEBUG_YFONT_M  yLOG_info    ("full name" , x_name);
   /*---(open)---------------------------*/
   --rce;  switch (a_mode) {
   case 'r' :
      f = fopen (x_name, "rb");
      break;
   case 'w' :
      f = fopen (x_name, "wb");
      break;
   default  :
      DEBUG_YFONT_M  yLOG_warn    ("file mode" , "must be either 'r' or 'w'");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
      break;
   }
   DEBUG_YFONT_M  yLOG_point   ("f"         , f);
   if (f == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font"      , "could not be openned");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      yFONT__slot_free  (a_slot);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> close an existing font file -----------[ ------ [ ------ ]-*/
yFONT__file_close  (char  a_slot)
{
}
