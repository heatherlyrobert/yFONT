/*============================----beg-of-source---============================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include   <png.h>                     /* to handle avatars                   */


/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"




int     nfont     =   0;
GLuint  syms      =   0;
float   s_rows    =   0;
float   s_cols    =   0;
char    s_type    = '-';



/*---(prototypes)-------------------------------*/




/*====================------------------------------------====================*/
/*===----                           utility                            ----===*/
/*====================------------------------------------====================*/
static void      o___UTILITY_________________o (void) {;}

static char      yFONT_ver [200] = "";

char*      /* ---- : return library versioning information -------------------*/
yFONT_version      (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#elif  __HEPH__  > 0
   strncpy (t, "[hephaestus ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (yFONT_ver, 100, "%s   %s : %s", t, YFONT_VER_NUM, YFONT_VER_TXT);
   return yFONT_ver;
}

char               /* PURPOSE : load a font from a file ----------------------*/
yFONT__list        (void)
{
   int i = 0;
   for (i = 0; i < MAX_FONT; ++i) {
      if (g_yfont [i] == NULL)  printf ("%02d  NULL        empty\n", i);
      else                     printf ("%02d  %10p  %s\n", i, g_yfont [i], g_yfont[i]->name);
   }
}

char               /* PURPOSE : load a font from a file ----------------------*/
yFONT_load         (char *a_name)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   int         rc          = 0;                 /* generic return code            */
   char        x_slot      = -1;
   /*---(header)-------------------------*/
   DEBUG_YFONT    yLOG_enter   (__FUNCTION__);
   /*---(get font slot)------------------*/
   DEBUG_YFONT    yLOG_note    ("assign the font slot");
   if (rc >= 0)  rc = x_slot = yFONT__slot_new      ();
   /*---(open the font)------------------*/
   DEBUG_YFONT    yLOG_note    ("open the file");
   if (rc >= 0)  rc = yFONT__head_name     (x_slot, a_name, 40);
   if (rc >= 0)  rc = yFONT__file_open     (x_slot, 'r');
   /*---(header)-------------------------*/
   DEBUG_YFONT    yLOG_note    ("read the header");
   if (rc >= 0)  rc = yFONT__head_read     (x_slot);
   /*---(index)--------------------------*/
   DEBUG_YFONT    yLOG_note    ("read the font index");
   if (rc >= 0)  rc = yFONT__index_alloc   (x_slot);
   if (rc >= 0)  rc = yFONT__index_read    (x_slot);
   /*---(texture)------------------------*/
   DEBUG_YFONT    yLOG_note    ("read the font map and set texture");
   if (rc >= 0)  rc = yFONT__map_alloc     (x_slot);
   if (rc >= 0)  rc = yFONT__map_read      (x_slot);
   if (rc >= 0)  rc = yFONT__map_texture   (x_slot);
   /*---(vertices)-----------------------*/
   DEBUG_YFONT    yLOG_note    ("calc vertices and lookup");
   if (rc >= 0)  rc = yFONT__verts_alloc   (x_slot);
   if (rc >= 0)  rc = yFONT__verts_load    (x_slot);
   if (rc >= 0)  rc = yFONT__index_lookup  (x_slot);
   /*---(handle failure)-----------------*/
   if (rc <  0)  return rc;
   /*---(complete)-----------------------*/
   DEBUG_YFONT    yLOG_exit    (__FUNCTION__);
   return x_slot;
}


char               /* PURPOSE : free a font from texture memory --------------*/
yFONT_free           (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   int         rc          = 0;                 /* generic return code            */
   /*---(header)-------------------------*/
   DEBUG_YFONT    yLOG_enter   (__FUNCTION__);
   /*---(free it up)----------------------------*/
   if (rc >= 0)  rc = yFONT__slot_free (a_slot);
   /*---(complete)-----------------------*/
   DEBUG_YFONT    yLOG_exit    (__FUNCTION__);
   /*---(complete)------------------------------*/
   return rc;
}

int                /* PURPOSE : print a string -------------------------------*/
yFONT_print          (char a_slot, char a_size, char a_align, char *a_text)
{
   /*---(locals)-------------------------*/
   char      rce       = -10;
   int       i         = 0;                 /* iterator -- character          */
   tYFONT    *x_font    = NULL;
   int       x_len     = 0;
   float     x_scale   = 0;
   int       w         = 0;
   float     x         = 0;
   float     y         = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   --rce;  if (a_slot < 0 || a_slot >= MAX_FONT) {
      DEBUG_YFONT_M  yLOG_warn    ("a_slot"    , "out of accepable range (0 to MAX_FONT)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font    = g_yfont [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("x_font"    , x_font);
   --rce;  if (x_font == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("x_font"    , "slot can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_point   ("*a_text"   , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("text"      , "can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_info    ("a_text"    , a_text);
   x_len     = strllen(a_text, LEN_STR);
   DEBUG_YFONT_M  yLOG_value   ("x_len"     , x_len);
   /*---(place start)--------------------*/
   w = yFONT__index_width  (x_font, a_text, x_len);
   DEBUG_YFONT_M  yLOG_value   ("w"         , w);
   switch (a_align / 3) {
   case  0:  y =  0;                      break;
   case  1:  y = -x_font->max_ascent / 2;  break;
   case  2:  y = -x_font->max_ascent;      break;
   case  3:  y = -x_font->max_descent;     break;
   }
   switch (a_align % 3) {
   case  0:  x =  0;                      break;
   case  1:  x = -w / 2;                  break;
   case  2:  x = -w;                      break;
   }
   /*---(draw text)----------------------*/
   x_scale   = (float) a_size / (float) x_font->point;
   DEBUG_YFONT_M  yLOG_double  ("x_scale"   , x_scale);
   glPushMatrix(); {
      glScalef      (x_scale, x_scale, x_scale);
      /*---(draw characters)-------------*/
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBindTexture (GL_TEXTURE_2D, x_font->tex_ref);
      glTranslatef  (x, y, 0);
      for (i = 0; i < x_len; ++i) {
         yFONT__map_glyph  (x_font, a_text[i]);
      }
      glBindTexture (GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return w * x_scale;
}

int                /* PURPOSE : print a string with word wrapping             */
yFONT_printw         (char a_slot, char a_point, char a_align, char *a_text, int a_width, int a_height, float a_spacing)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tYFONT  *x_font = g_yfont [a_slot];
   if (x_font == NULL)                     return YF_BAD_SLOT;
   /*---(locals)--------------------------------*/
   int       i         = 0;                 /* iterator -- character          */
   int       len       = strlen(a_text);     /* string length                  */
   int       s         = 0;                 /* start of next print            */
   int       w         = 0;                 /* current width                  */
   int       l         = 1;                 /* number of lines                */
   int       space     = 0;                 /* last breakable place in text   */
   char     *x_str     = strdup(a_text);     /* alterable string               */
   float     x_scale   = (float) a_point / (float) x_font->point;
   int       v         = 0;                 /* vertical spacing               */
   v = (x_font->max_ascent - x_font->max_descent) * a_spacing * x_scale;
   if (l * v > a_height) return -1;
   tVERT      *x_vert;
   for (i = 0; i < len; i++) {
      if (x_str[i] == '\0') break;
      if (strchr("-/ ", x_str[i]) != 0) {
         space = i;
      }
      x_vert = yFONT__verts_find  (x_font, x_str[i]);
      if (x_vert == NULL) continue;
      w += x_vert->a * x_scale;
      if (w > a_width) {
         x_str[space] = '\0';
         yFONT_print (a_slot, a_point, a_align, &x_str[s]);
         glTranslatef(0.0, -v, 0.0);
         w = 0;
         i = space + 1;
         s = i;
         ++l;
         if (l * v > a_height) return  2;
      }
   }
   if (s != len - 1) {
      yFONT_print (a_slot, a_point, a_align, &x_str[s]);
   }
   return a_width;
}

int                /* PURPOSE : print a string -------------------------------*/
yFONT_printu       (char a_slot, char a_point, char a_align, int  *a_array, int a_max)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tYFONT  *x_font = g_yfont [a_slot];
   if (x_font == NULL)                     return YF_BAD_SLOT;
   /*---(locals)--------------------------------*/
   int       i         = 0;                 /* iterator -- character          */
   int       x_len     = 0;                  /* string length                  */
   float     x_scale   = (float) a_point / (float) x_font->point;
   int       w         = 0;
   float     x         = 0;
   float     y         = 0;
   /*---(get length)----------------------------*/
   for (i = 0; i < a_max; ++i) {
      if (a_array [i] == '\0') break;
      ++x_len;
   }
   /*---(place start)---------------------------*/
   w = yFONT__index_widthu (x_font, a_array, x_len);
   switch (a_align / 3) {
   case  0:  y =  0;                      break;
   case  1:  y = -x_font->max_ascent / 2;  break;
   case  2:  y = -x_font->max_ascent;      break;
   case  3:  y = -x_font->max_descent;     break;
   }
   switch (a_align % 3) {
   case  0:  x =  0;                      break;
   case  1:  x = -w / 2;                  break;
   case  2:  x = -w;                      break;
   }
   /*---(draw text)-----------------------------*/
   glPushMatrix(); {
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBindTexture (GL_TEXTURE_2D, x_font->tex_ref);
      glScalef      (x_scale, x_scale, x_scale);
      glTranslatef  (x, y, 0);
      for (i = 0; i < x_len; ++i) {
         yFONT__map_glyph  (x_font, a_array [i]);
      }
      glBindTexture(GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(complete)------------------------------*/
   return w * x_scale;
}

int                /* PURPOSE : make a png image into a texture --------------*/
yFONT_symload      (char *a_filename, int a_col, int a_row, char a_type)
{
   /*---(defense)------------------------*/
   syms   = 0;
   syms   = yGLTEX_png2tex (a_filename);
   s_cols = a_col;
   s_rows = a_row;
   s_type = a_type;
   /*---(complete)-----------------------*/
   return syms;
}

int
yFONT_symbol       (float a_scale, int a_row, int a_col, int a_mod)
{
   if (syms == 0)  return -1;
   float    x_row [25] = {
      0.9354, 0.8740, 0.8115, 0.7500,
      0.6875, 0.6265, 0.5640, 0.5030,
      0.4410, 0.3790, 0.3180, 0.2555,
      0.1935, 0.1322, 0.0700, 0.0080,
   };
   float    x_col [25] = {
      0.0032, 0.0508, 0.0982, 0.1450,
      0.1920, 0.2390, 0.2870, 0.3340,
      0.3810, 0.4280, 0.4760, 0.5225,
      0.5702, 0.6170, 0.6645, 0.7110,
      0.7585, 0.8060, 0.8530, 0.9000,
      0.9475,
   };
   float    x_width    = 0;
   float    x_height   = 0;
   float    x          = 0;
   float    y          = 0;
   if (s_type == 't') {
      x_width    = 0.0460 - 0.0040;
      x_height   = 0.0547 - 0.0040;
      x          = x_col [a_col] + 0.0020;
      y          = x_row [a_row] + 0.0020;
   } else {
      x_width    = 1.00 / s_cols;
      x_height   = 1.00 / s_rows;
      x          = a_col * x_width;
      y          = 1.00 - (a_row * x_height);
   }
   /*> printf ("r=%2d, c=%2d, x=%8.3f, y=%8.3f, w=%8.3f, h=%8.3f\n", a_row, a_col, x, y, x_width, x_height);   <*/
   /*---(draw text)-----------------------------*/
   glPushMatrix(); {
      glScalef (a_scale, a_scale, a_scale);
      if (a_mod != 0) {
         switch (a_mod) {
         case  1  : glColor4f (1.0f, 0.0f, 0.0f, 0.2f); break; /* a */
         case  2  : glColor4f (0.8f, 0.8f, 0.0f, 0.2f); break; /* e */
         case  3  : glColor4f (0.0f, 1.0f, 0.0f, 0.2f); break; /* f */
         case  4  : glColor4f (0.0f, 0.8f, 0.8f, 0.2f); break; /* i */
         case  5  : glColor4f (0.0f, 0.0f, 1.0f, 0.2f); break; /* o */
         case  6  : glColor4f (0.8f, 0.0f, 0.8f, 0.2f); break; /* u */
         default  : glColor4f (0.8f, 0.8f, 0.8f, 0.2f); break;
         }
         glBegin  (GL_POLYGON); {
            glVertex3f   (    0.0, -20.0,   0.0);
            glVertex3f   (   20.0, -20.0,   0.0);
            glVertex3f   (   20.0,   0.0,   0.0);
            glVertex3f   (    0.0,   0.0,   0.0);
         } glEnd();
      }
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBindTexture(GL_TEXTURE_2D, syms);
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
      glBegin  (GL_POLYGON); {
         glTexCoord2f ( x          , y);
         glVertex3f   (    0.0, -20.0,   0.0);
         glTexCoord2f ( x + x_width, y);
         glVertex3f   (   20.0, -20.0,   0.0);
         glTexCoord2f ( x + x_width, y + x_height);
         glVertex3f   (   20.0,   0.0,   0.0);
         glTexCoord2f ( x          , y + x_height);
         glVertex3f   (    0.0,   0.0,   0.0);
      } glEnd();
      glBindTexture(GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(complete)------------------------------*/
   return 0;
}

/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*            /* [------] unit test accessor ------------------------------*/
yFONT__unit        (char *a_question, int a_num)
{
   tYFONT      *x_font      = NULL;              /* new font                       */
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "yFONT_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if        (strncmp(a_question, "font_name"   , 20)  == 0) {
      x_font = g_yfont [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT font name  : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT font name  : %2d:%-20.20s     point=%2d", strllen (x_font->name, LEN_LABEL), x_font->name, x_font->point);
   } else if (strncmp(a_question, "tex_size"    , 20)  == 0) {
      x_font = g_yfont [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT tex size   : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT tex size   : width =%4d, height=%4d" , x_font->tex_w, x_font->tex_h);
   } else if (strncmp(a_question, "char_limits" , 20)  == 0) {
      x_font = g_yfont [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT char limit : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT char limit : mascent =%3d, mdescent=%3d, margin  =%3d" , x_font->max_ascent, x_font->max_descent, x_font->margin);
   } else if (strncmp(a_question, "num_glyph"   , 20)  == 0) {
      x_font = g_yfont [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT nglyph     : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT nglyph     : %4d" , x_font->num_glyph);
   } else if (strncmp(a_question, "conf_font"   , 20)  == 0) {
      snprintf (unit_answer, LEN_TEXT, "yFONT conf font  : %3d %s" , a_num, yFONT__conf_font (a_num));
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


char       /*----: set up programgents/debugging -----------------------------*/
yFONT__testquiet    (void)
{
   char       *x_args [2]  = { "yFONT_debug","@@quiet" };
   yURG_logger (2, x_args);
   yURG_urgs   (2, x_args);
   return 0;
}

char       /*----: set up programgents/debugging -----------------------------*/
yFONT__testloud     (void)
{
   char       *x_args [3]  = { "yFONT_debug","@@kitchen","@@YFONT" };
   yURG_logger (3, x_args);
   yURG_urgs   (3, x_args);
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yFONT__testend      (void)
{
   DEBUG_TOPS   yLOG_end     ();
   return 0;
}

/*============================----end-of-source---============================*/
