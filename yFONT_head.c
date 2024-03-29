/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"



/*====================------------------------------------====================*/
/*===----                     setting header values                    ----===*/
/*====================------------------------------------====================*/
static void      o___SETTERS_________________o (void) {;}

char         /*--> read a font header --------------------[ ------ [ ------ ]-*/
yFONT__head_name   (char a_slot, char *a_name, char a_point)
{
   /*---(locals)--------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   tYFONT      *x_font      = NULL;          /* new font                       */
   int         x_len       = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense : font)------------------------*/
   --rce;  if ((x_font = yFONT__slot_font  (a_slot)) == NULL) {
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;;
   }
   /*---(defense : content)---------------------*/
   DEBUG_YFONT_M  yLOG_point   ("*a_name"   , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("name can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_info    ("a_name"    , a_name);
   x_len = ystrllen (a_name, LEN_LABEL);
   DEBUG_YFONT_M  yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 0 || x_len >= LEN_LABEL) {
      DEBUG_YFONT_M  yLOG_warn    ("name length not in required range (0 - LEN_LABEL)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("a_point"   , a_point);
   --rce;  if (a_point <  0 || a_point >  99) {
      DEBUG_YFONT_M  yLOG_warn    ("point not in required range (4 - 99)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(update)--------------------------------*/
   ystrlcpy (x_font->name, a_name, LEN_LABEL);
   x_font->point   = a_point;
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read a font header --------------------[ ------ [ ------ ]-*/
yFONT__head_tex    (char a_slot, short a_texw, short a_texh)
{
   /*---(locals)--------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   tYFONT      *x_font      = NULL;          /* new font                       */
   int         x_len       = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense : font)------------------------*/
   --rce;  if ((x_font = yFONT__slot_font  (a_slot)) == NULL) {
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;;
   }
   /*---(defense : content)---------------------*/
   DEBUG_YFONT_M  yLOG_value   ("a_texw"    , a_texw);
   --rce;  if (a_texw  <= 0 || a_texw  >  9999) {
      DEBUG_YFONT_M  yLOG_warn    ("texw not in required range (0 - 9999)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("a_texh"    , a_texh);
   --rce;  if (a_texh  <= 0 || a_texh  >  9999) {
      DEBUG_YFONT_M  yLOG_warn    ("texh not in required range (0 - 9999)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(update)--------------------------------*/
   x_font->tex_w   = a_texw;
   x_font->tex_h   = a_texh;
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read a font header --------------------[ ------ [ ------ ]-*/
yFONT__head_place  (char a_slot, char a_mascent, char a_mdescent)
{
   /*---(locals)--------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   tYFONT      *x_font      = NULL;          /* new font                       */
   int         x_len       = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense : font)------------------------*/
   --rce;  if ((x_font = yFONT__slot_font  (a_slot)) == NULL) {
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;;
   }
   /*---(defense : content)---------------------*/
   DEBUG_YFONT_M  yLOG_value   ("a_mascent" , a_mascent);
   --rce;  if (a_mascent  <  0 || a_mascent  >  99) {
      DEBUG_YFONT_M  yLOG_warn    ("mascent not in required range (0 to 99)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("a_mdescent", a_mdescent);
   --rce;  if (a_mdescent  >  0 || a_mdescent  < -99) {
      DEBUG_YFONT_M  yLOG_warn    ("mdescent not in required range (-0 to -99)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(update)--------------------------------*/
   x_font->max_ascent  = a_mascent;
   x_font->max_descent = a_mdescent;
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read a font header --------------------[ ------ [ ------ ]-*/
yFONT__head_margin   (char a_slot, char a_margin)
{
   /*---(locals)--------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   tYFONT      *x_font      = NULL;          /* new font                       */
   int         x_len       = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense : font)------------------------*/
   --rce;  if ((x_font = yFONT__slot_font  (a_slot)) == NULL) {
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;;
   }
   /*---(defense : content)---------------------*/
   DEBUG_YFONT_M  yLOG_value   ("a_margin"  , a_margin);
   --rce;  if (a_margin   <  0 || a_margin   >  99) {
      DEBUG_YFONT_M  yLOG_warn    ("margin not in required range (0 to 99)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(update)--------------------------------*/
   x_font->margin      = a_margin;
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read a font header --------------------[ ------ [ ------ ]-*/
yFONT__head_nglyph   (char a_slot, short a_nglyph)
{
   /*---(locals)--------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   tYFONT      *x_font      = NULL;          /* new font                       */
   int         x_len       = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense : font)------------------------*/
   --rce;  if ((x_font = yFONT__slot_font  (a_slot)) == NULL) {
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;;
   }
   /*---(defense : content)---------------------*/
   DEBUG_YFONT_M  yLOG_value   ("a_nglyph"  , a_nglyph);
   --rce;  if (a_nglyph  <= 0 || a_nglyph  >  9999) {
      DEBUG_YFONT_M  yLOG_warn    ("nglyph not in required range (0 - 9999)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(update)--------------------------------*/
   x_font->num_glyph = a_nglyph;
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      reading and writing                     ----===*/
/*====================------------------------------------====================*/
static void      o___FILE____________________o (void) {;}

char         /*--> open a font file for reading ----------[ ------ [ ------ ]-*/
yFONT__file_open   (char a_slot, char a_mode)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        x_name      [1000];
   FILE       *f           = NULL;
   tYFONT      *x_font      = NULL;              /* new font                       */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"   , a_slot);
   DEBUG_YFONT_M  yLOG_char    ("a_mode"   , a_mode);
   /*---(defense)------------------------*/
   x_font = g_yfont [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("file"      , x_font->file);
   --rce;  if (x_font->file != NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("file pointer not null (already open)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_char    ("a_mode"    , a_mode);
   --rce;  if (strchr ("rw", a_mode) == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("file mode must be either 'r' or 'w'");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(full name)----------------------*/
   DEBUG_YFONT_M  yLOG_point   ("name"     , x_font->name);
   snprintf (x_name, 900, "/usr/local/share/yfonts/%s.txf", x_font->name);
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
      DEBUG_YFONT_M  yLOG_warn    ("file mode must be either 'r' or 'w'");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
      break;
   }
   DEBUG_YFONT_M  yLOG_point   ("f"         , f);
   --rce;  if (f == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("font could not be openned");
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
   tYFONT      *x_font      = NULL;              /* new font                       */
   x_font = g_yfont [a_slot];
   if (x_font->file != NULL)  fclose (x_font->file);
   x_font->file == NULL;
   return 0;
}

char         /*--> read a font header --------------------[ ------ [ ------ ]-*/
yFONT__head_write  (char a_slot)
{
   /*---(locals)--------------------------------*/
   char      rce       = -10;               /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   tYFONT    *x_font    = NULL;              /* new font                       */
   char      x_text    [LEN_STR];
   int8_t    x_char    = 0;
   int16_t   x_short   = 0;
   int32_t   x_int     = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(check file type)-----------------------*/
   x_font = g_yfont [a_slot];
   /*---(write file info)----------------*/
   --rce;  if ( 1 != fwrite ("\377"                 , sizeof (char),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("magic ID could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if ( 4 != fwrite ("txf"                  , sizeof (char),  4, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("magic num could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   --rce;  if ( 5 != fwrite (P_VERNUM          , sizeof (char),  5, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("version could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(write header)-------------------*/
   --rce;  if (20 != fwrite (&(x_font->name       ) , sizeof (char), 20, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("name could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(point)--------------------------*/
   --rce;  if ( 1 != fwrite (&(x_font->point      ) , sizeof (char),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("point could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(tex width)----------------------*/
   --rce;  if ( 1 != fwrite (&(x_font->tex_w      ) , sizeof (int16_t),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("text_w could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(tex height)---------------------*/
   --rce;  if ( 1 != fwrite (&(x_font->tex_h      ) , sizeof (int16_t),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("text_h could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(max ascent)---------------------*/
   --rce;  if ( 1 != fwrite (&(x_font->max_ascent ) , sizeof (char),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("max_ascent could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(max descent)--------------------*/
   --rce;  if ( 1 != fwrite (&(x_font->max_descent) , sizeof (char),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("max_descen could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(margin)-------------------------*/
   --rce;  if ( 1 != fwrite (&(x_font->margin     ) , sizeof (char),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("margin could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(margin)-------------------------*/
   --rce;  if ( 1 != fwrite (&(x_font->num_glyph  ) , sizeof (int16_t),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("num_glyph could not be written");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> read a font header --------------------[ ------ [ ------ ]-*/
yFONT__head_read   (char a_slot)
{
   /*---(locals)--------------------------------*/
   char      rce       = -10;               /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   tYFONT    *x_font    = NULL;              /* new font                       */
   char      x_text    [LEN_STR];
   uchar     x_uchar   = 0;
   int8_t    x_char    = 0;
   int16_t   x_short   = 0;
   int32_t   x_int     = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(check file type)-----------------------*/
   x_font = g_yfont [a_slot];
   /*---(check initial char)--------------------*/
   --rce;  if ( 1 != fread (&x_uchar, sizeof (char),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("magic ID could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("magic ID"  , x_uchar);
   --rce;  if (x_uchar != 0xFF) {
      DEBUG_YFONT_M  yLOG_warn    ("magic ID does not match expected (0xFF)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(check magic number)--------------------*/
   --rce;  if ( 4 != fread (&x_text, sizeof (char),  4, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("magic num could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_text [3] = '\0';
   DEBUG_YFONT_M  yLOG_info    ("magic num" , x_text);
   --rce;  if (strcmp ("txf", x_text) != 0) {
      DEBUG_YFONT_M  yLOG_warn    ("magic num does not match expected (txf)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(read version)--------------------------*/
   --rce;  if ( 5 != fread (&x_text, sizeof (char),  5, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("version could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_text [4] = '\0';
   DEBUG_YFONT_M  yLOG_info    ("version"   , x_text);
   /*---(read name)-----------------------------*/
   --rce;  if (20 != fread (&x_text, sizeof (char), 20, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("name could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_text [19] = '\0';
   ystrltrim (x_text, ySTR_BOTH, LEN_LABEL);
   ystrlcpy (x_font->name, x_text, LEN_LABEL);
   DEBUG_YFONT_M  yLOG_info    ("name"      , x_text);
   /*---(point)---------------------------------*/
   --rce;  if ( 1 != fread (&x_char, sizeof (char),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("point could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("point"     , x_char);
   --rce;  if (x_char < 0 || x_char > 99) {
      DEBUG_YFONT_M  yLOG_warn    ("point does not match range (1 to 99)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->point  = x_char;
   /*---(texture width)-------------------------*/
   --rce;  if ( 1 != fread (&x_short, sizeof (int16_t),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("tex_w could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("tex_w"     , x_short);
   --rce;  if (x_short < 1 || x_short > 9999) {
      DEBUG_YFONT_M  yLOG_warn    ("tex_w does not fit in range (1 to 9999)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->tex_w  = x_short;
   /*---(texture height)------------------------*/
   --rce;  if ( 1 != fread (&x_short, sizeof (int16_t),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("tex_h could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("tex_h"     , x_short);
   --rce;  if (x_short < 1 || x_short > 9999) {
      DEBUG_YFONT_M  yLOG_warn    ("tex_h does not fit in range (1 to 9999)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->tex_h  = x_short;
   /*---(max ascent)----------------------------*/
   --rce;  if ( 1 != fread (&x_char , sizeof (char   ),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("max_ascent could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("max_ascent", x_char);
   --rce;  if (x_char  <  0 || x_char >  100) {
      DEBUG_YFONT_M  yLOG_warn    ("max_ascent does not fit in range (0 to 100)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->max_ascent  = x_char;
   /*---(max descent)---------------------------*/
   --rce;  if ( 1 != fread (&x_char , sizeof (char   ),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("max_descen could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("max_descen", x_char);
   --rce;  if (x_char  >  0 || x_char <  -100) {
      DEBUG_YFONT_M  yLOG_warn    ("max_descen does not fit in range (-0 to -100)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->max_descent = x_char;
   /*---(margin)--------------------------------*/
   --rce;  if ( 1 != fread (&x_char , sizeof (char   ),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("margin could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("margin"    , x_char);
   --rce;  if (x_char  <  0 || x_char >    10) {
      DEBUG_YFONT_M  yLOG_warn    ("margin does not fit in range (0 to 10)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->margin      = x_char;
   /*---(margin)--------------------------------*/
   --rce;  if ( 1 != fread (&x_short, sizeof (int16_t),  1, x_font->file)) {
      DEBUG_YFONT_M  yLOG_warn    ("num_glyph could not be read");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_value   ("num_glyph" , x_short);
   --rce;  if (x_short <= 0 || x_char >= 9999) {
      DEBUG_YFONT_M  yLOG_warn    ("num_glyph does not fit in range (1 to 9999)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font->num_glyph   = x_short;
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yFONT_head_force        (char *a_slot, char *a_name, char a_point, short a_wide, short a_tall, short a_ascent, short a_descent, char a_margin, short a_glyphs)
{
   /*---(locals)--------------------------------*/
   char      rce       = -10;               /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   char      x_slot    =  0;
   tYFONT    *x_font    = NULL;              /* new font                       */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(check file type)-----------------------*/
   x_slot = yFONT__slot_new      ();
   DEBUG_YFONT_M  yLOG_value   ("x_slot"    , x_slot);
   if (a_slot != NULL)  *a_slot = x_slot;
   x_font = g_yfont [x_slot];
   /*---(read name)-----------------------------*/
   ystrlcpy (x_font->name, a_name, LEN_LABEL);
   DEBUG_YFONT_M  yLOG_info    ("name"      , x_font->name);
   /*---(point)---------------------------------*/
   x_font->point  = a_point;
   DEBUG_YFONT_M  yLOG_value   ("point"     , x_font->point);
   /*---(texture width)-------------------------*/
   x_font->tex_w  = a_wide;
   DEBUG_YFONT_M  yLOG_value   ("tex_w"     , x_font->tex_w);
   /*---(texture height)------------------------*/
   x_font->tex_h  = a_tall;
   DEBUG_YFONT_M  yLOG_value   ("tex_h"     , x_font->tex_h);
   /*---(max ascent)----------------------------*/
   x_font->max_ascent  = a_ascent;
   DEBUG_YFONT_M  yLOG_value   ("ascent"    , x_font->max_ascent);
   /*---(max descent)---------------------------*/
   x_font->max_descent = a_descent;
   DEBUG_YFONT_M  yLOG_value   ("descent"   , x_font->max_descent);
   /*---(margin)--------------------------------*/
   x_font->margin      = a_margin;
   DEBUG_YFONT_M  yLOG_value   ("margin"    , x_font->margin);
   /*---(margin)--------------------------------*/
   x_font->num_glyph   = a_glyphs;;
   DEBUG_YFONT_M  yLOG_value   ("glyphs"    , x_font->num_glyph);
   /*---(open the file)-------------------------*/
   rc = yFONT__file_open   (x_slot, 'w');
   /*---(write to file)-------------------------*/
   rc = yFONT__head_write  (x_slot);
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           reporting                          ----===*/
/*====================------------------------------------====================*/
static void      o___REPORTING_______________o (void) {;}

char
yFONT__head_title    (void)
{
   printf ("---name-------------  -pt-  f  s  -tw-  -th-  -ma-  -md-  -ma-  -ng-\n");
   /*> printf ("--------------------  ----  -  -  ----  ----  ----  ----  ----  ----\n");   <*/
   return 0;
}

char
yFONT__head_line     (char a_slot)
{
   tYFONT    *x_font    = NULL;              /* new font                       */
   x_font = g_yfont [a_slot];
   printf ("%-20.20s  %4d  %4d  %4d  %4d  %4d  %4d  %4d\n",
         x_font->name       , x_font->point      ,
         x_font->tex_w      , x_font->tex_h      ,
         x_font->max_ascent , x_font->max_descent,
         x_font->margin     , x_font->num_glyph  );
   return 0;
}

char
yFONT__head_dump     (char a_slot)
{
   tYFONT    *x_font    = NULL;              /* new font                       */
   x_font = g_yfont [a_slot];
   printf ("\n");
   printf ("font header\n");
   printf ("   name         : %s\n" , x_font->name       );
   printf ("   point        : %4d\n", x_font->point      );
   printf ("   max ascent   : %4d\n", x_font->max_ascent );
   printf ("   max descent  : %4d\n", x_font->max_descent);
   printf ("   margin       : %4d\n", x_font->margin     );
   printf ("   tex width    : %4d (%6.2f x 16)\n", x_font->tex_w,  (float) x_font->tex_w / 16.0);
   printf ("   tex height   : %4d (%6.2f x 16)\n", x_font->tex_h,  (float) x_font->tex_h / 16.0);
   printf ("   num glyph    : %4d\n", x_font->num_glyph  );
   return 0;
}

