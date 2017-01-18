/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"



char         /*--> open a font file for reading ----------[ ------ [ ------ ]-*/
yFONT__file_open   (char a_slot, char a_mode)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        x_name      [1000];
   FILE       *f           = NULL;
   tFONT      *x_font      = NULL;              /* new font                       */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"   , a_slot);
   DEBUG_YFONT_M  yLOG_char    ("a_mode"   , a_mode);
   /*---(defense)------------------------*/
   x_font = g_font [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("file"      , x_font->file);
   --rce;  if (x_font->file != NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("file"      , "pointer not null (already open)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_char    ("a_mode"    , a_mode);
   --rce;  if (strchr ("rw", a_mode) == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("file mode" , "must be either 'r' or 'w'");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(full name)----------------------*/
   DEBUG_YFONT_M  yLOG_point   ("name"     , x_font->name);
   snprintf (x_name, 900, "/usr/local/share/fonts/%s.txf", x_font->name);
   DEBUG_YFONT_M  yLOG_info    ("file"      , x_name);
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
   --rce;  if (f == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font"      , "could not be openned");
      yFONT__slot_free  (a_slot);
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->file = f;
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> close an existing font file -----------[ ------ [ ------ ]-*/
yFONT__file_close  (char  a_slot)
{
   tFONT      *x_font      = NULL;              /* new font                       */
   x_font = g_font [a_slot];
   if (x_font->file != NULL)  fclose (x_font->file);
   x_font->file == NULL;
   return 0;
}
