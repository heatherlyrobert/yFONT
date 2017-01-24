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
   if (x_font->tex_map != NULL)  free (x_font->tex_map);
   DEBUG_YFONT_M  yLOG_note    ("nulling slot entry");
   x_font->tex_map = NULL;
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> write the font map --------------------[ ------ [ ------ ]-*/
yFONT__map_write     (char a_slot)
{
   /*---(locals)--------------------------------*/
   char      rce       = -10;               /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   tFONT    *x_font    = NULL;              /* new font                       */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(check file type)-----------------------*/
   x_font = g_font [a_slot];
   /*---(write map info)-----------------*/
   fwrite (x_font->tex_map, x_font->tex_h * x_font->tex_w, 1, x_font->file);
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> write the font map --------------------[ ------ [ ------ ]-*/
yFONT__map_read      (char a_slot)
{
   /*---(locals)--------------------------------*/
   char      rce       = -10;               /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   tFONT    *x_font    = NULL;              /* new font                       */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(check file type)-----------------------*/
   x_font = g_font [a_slot];
   /*---(write map info)-----------------*/
   fread  (x_font->tex_map, x_font->tex_h * x_font->tex_w, 1, x_font->file);
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--: show ascii version --------------------[ ------ [ ------ ]-*/
yFONT__map_art     (char a_slot, int  a_entry)
{
   /*---(locals)-----------+-----------+-*/
   tFONT    *x_font    = NULL;              /* new font                       */
   char        rc          =  0;
   int         x_code      =  0;
   char        x_good      =  0;
   int         x_row       =  0;   /* vertical iterator                   */
   int         x_col       =  0;   /* horizontal iterator                 */
   short       x_xpos      =  0;
   short       x_ypos      =  0;
   char        x_tall      =  0;   /* greatest character height           */
   char        x_wide      =  0;
   uchar       x_value     =  0;       /* byte value in source glyph          */
   int         x_ascii     =  0;       /* byte value in source glyph          */
   short       x_curx      =  0;
   short       x_cury      =  0;
   char        x_chars [20] = " -+:*172%98#@";   /* use symmetrical charaters    */
   /*---(coordinates)--------------------*/
   x_font = g_font [a_slot];
   rc = yFONT__index_who    (a_slot, a_entry, &x_code, &x_good);
   rc = yFONT__index_coords (a_slot, a_entry, &x_xpos, &x_ypos, &x_wide, &x_tall);
   if (rc < 0)  return rc;
   printf("\n");
   printf ("%4d, code %7d, good %c x_pos %5d, y_pos %5d, wide %5d, tall %5d\n",
         a_entry, x_code, x_good, x_xpos, x_ypos, x_wide, x_tall);
   /*---(draw)---------------------------*/
   x_xpos -= 2;;
   x_wide += 4;
   x_ypos -= 2;;
   x_tall += 4;
   for (x_row = 0; x_row < x_tall; ++x_row) {
      x_cury   = x_ypos + x_row;
      printf ("   |");
      for (x_col = 0; x_col < x_wide; ++x_col) {
         x_curx  = x_xpos + x_col;
         x_value = x_font->tex_map [(x_cury * x_font->tex_w) + x_curx];
         x_ascii = x_value / (256 / 12);
         printf ("%c", x_chars [x_ascii]);
      }
      printf ("|\n");
   }
   printf("\n");
   return 0;
}

char         /*--: show ascii version --------------------[ ------ [ ------ ]-*/
yFONT__map_texart  (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   tFONT    *x_font    = NULL;              /* new font                       */
   int       i         = 0;
   x_font = g_font [a_slot];
   for (i = 0; i < x_font->num_glyph; ++i) {
      yFONT__map_art (a_slot, i);
   }
   return 0;
}

char         /*--> allocate a vertex table ---------------[ ------ [ ------ ]-*/
yFONT__verts_alloc   (char a_slot)
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
   DEBUG_YFONT_M  yLOG_note    ("allocating font vertices");
   DEBUG_YFONT_M  yLOG_value   ("num_glyph" , x_font->num_glyph);
   --rce;  while (x_font->verts   == NULL) {
      ++x_tries;
      DEBUG_YFONT_M  yLOG_value   ("x_tries"   , x_tries);
      x_font->verts = (tVERT  *) malloc (x_font->num_glyph * sizeof (tVERT));
      DEBUG_YFONT_M  yLOG_point   ("*verts"    , x_font->verts);
      if (x_font->verts != NULL) {
         DEBUG_YFONT_M  yLOG_note    ("got it, done");
         break;
      }
      if (x_tries > 10) {
         DEBUG_YFONT_M  yLOG_warn    ("malloc"    , "could not allocate a new font vertices in 10 tries");
         DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
         return rce;
      }
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> write a report line -------------------[ ------ [ ------ ]-*/
yFONT__verts_line    (char a_corner, short a_val1, double a_val2, char a_val3, short a_val4, double a_val5, char a_val6)
{
   printf ("        ");
   switch (a_corner) {
   case 1 :  printf ("TL  ");  break;
   case 2 :  printf ("TR  ");  break;
   case 3 :  printf ("BR  ");  break;
   case 4 :  printf ("BL  ");  break;
   }
   switch (a_corner) {
   case 1 : case 2 :
      printf ("%4d %7.5f %3d  ", a_val1, a_val2, a_val3);
      printf ("                  ");
      break;
   case 3 : case 4 :
      printf ("                  ");
      printf ("%4d %7.5f %3d  ", a_val1, a_val2, a_val3);
      break;
   }
   switch (a_corner) {
   case 1 : case 4 :
      printf ("%4d %7.5f %3d  ", a_val4, a_val5, a_val6);
      printf ("                  ");
      break;
   case 2 : case 3 :
      printf ("                  ");
      printf ("%4d %7.5f %3d  ", a_val4, a_val5, a_val6);
      break;
   }
   printf ("\n");
   return 0;
}

char         /*--> create vertex table -------------------[ ------ [ ------ ]-*/
yFONT__verts_load    (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   tGLYPH     *x_glyph     = NULL;
   long        x_texw      = 0;
   long        x_texh      = 0;
   short       x_topa      = 0;
   double      x_topr      = 0;
   short       x_bota      = 0;
   double      x_botr      = 0;
   short       x_lefa      = 0;
   double      x_lefr      = 0;
   short       x_riga      = 0;
   double      x_rigr      = 0;
   tFONT      *x_font      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   /*---(allocate structure)-------------*/
   x_font = g_font [a_slot];
   /*---(read glyph verticies)-----------*/
   x_texw = x_font->tex_w;
   x_texh = x_font->tex_h;
   URG_VERB   printf ("\n");
   for (i = 0; i < x_font->num_glyph; i++) {
      /*---(point to glyph)--------------*/
      x_glyph = &x_font->glyphs[i];
      /*> printf ("%7d %c ", x_glyph->code, (x_glyph->code < 128) ? x_glyph->code : '-');   <*/
      URG_VERB   printf ("%6d  ", x_glyph->code);
      URG_VERB   printf ("--  top- --pct-- rel  bot- --pct-- rel  lef- --pct-- rel  rig- --pct-- rel\n");
      /*---(absolute coordinates)--------*/
      /* flip top and bottom for opengl */
      x_topa   = x_glyph->ypos - 1;
      x_bota   = x_glyph->ypos + x_glyph->tall + 1;
      x_lefa   = x_glyph->xpos - 1;
      x_riga   = x_glyph->xpos + x_glyph->wide + 1;
      /*---(relative coordinates)--------*/
      /* flip top and bottom for opengl */
      x_topr   = (double) (x_topa) / (double) x_texh;
      x_botr   = (double) (x_bota) / (double) x_texh;
      x_lefr   = (double) (x_lefa) / (double) x_texw;
      x_rigr   = (double) (x_riga) / (double) x_texw;
      /*---(top-left)--------------------*/
      x_font->verts[i].TL.rel_x  = x_glyph->xoff - 2;
      x_font->verts[i].TL.rel_y  = x_glyph->yoff + 2;
      x_font->verts[i].TL.tex_x  = x_lefr;
      x_font->verts[i].TL.tex_y  = x_topr;
      URG_VERB   yFONT__verts_line (1, x_topa, x_topr, x_font->verts[i].TL.rel_y, x_lefa, x_lefr, x_font->verts[i].TL.rel_x);
      /*---(top-right)-------------------*/
      x_font->verts[i].TR.rel_x  = x_glyph->xoff + x_glyph->wide + 2;
      x_font->verts[i].TR.rel_y  = x_glyph->yoff + 2;
      x_font->verts[i].TR.tex_x  = x_rigr;
      x_font->verts[i].TR.tex_y  = x_topr;
      URG_VERB   yFONT__verts_line (2, x_topa, x_topr, x_font->verts[i].TR.rel_y, x_riga, x_rigr, x_font->verts[i].TR.rel_x);
      /*---(bottom-right)----------------*/
      x_font->verts[i].BR.rel_x  = x_glyph->xoff + x_glyph->wide + 2;
      x_font->verts[i].BR.rel_y  = x_glyph->yoff - x_glyph->tall - 2;
      x_font->verts[i].BR.tex_x  = x_rigr;
      x_font->verts[i].BR.tex_y  = x_botr;
      URG_VERB   yFONT__verts_line (3, x_bota, x_botr, x_font->verts[i].BR.rel_y, x_riga, x_rigr, x_font->verts[i].BR.rel_x);
      /*---(bottom-left)-----------------*/
      x_font->verts[i].BL.rel_x  = x_glyph->xoff - 2;
      x_font->verts[i].BL.rel_y  = x_glyph->yoff - x_glyph->tall - 2;
      x_font->verts[i].BL.tex_x  = x_lefr;
      x_font->verts[i].BL.tex_y  = x_botr;
      URG_VERB   yFONT__verts_line (4, x_bota, x_botr, x_font->verts[i].BL.rel_y, x_lefa, x_lefr, x_font->verts[i].BL.rel_x);
      /*---(advance)---------------------*/
      x_font->verts[i].a         = x_glyph->adv;
      /*---(done)------------------------*/
   }
   /*---(complete)------------------------------*/
   return 0;
}

char               /* PURPOSE : set up a font texture                         */
yFONT__map_texture   (char a_slot)
{
   /*---(locals)-----------+-----------+-*/
   tFONT      *x_font      = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT   yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT   yLOG_value   ("a_slot"    , a_slot);
   /*---(allocate structure)-------------*/
   x_font = g_font [a_slot];
   DEBUG_YFONT   yLOG_point   ("x_font"    , x_font);
   if (x_font->tex_ref == 0) {
      glGenTextures(1, &x_font->tex_ref);
   }
   glBindTexture   (GL_TEXTURE_2D, x_font->tex_ref);
   /*---(select quality)------------------------*/
   /* texture filtering is a performance vs quality tradeoff...
    *  GL_NEAREST                 -- fastest, no mipmappping
    *  GL_LINEAR                  -- better looking, no mipmapping
    *  GL_NEAREST_MIPMAP_NEAREST  -- mipmapped, fast, but lowest clarity
    *  GL_LINEAR_MIPMAP_NEAREST   -- better filter and still fast (game use)
    *  GL_NEAREST_MIPMAP_LINEAR   -- much more processing, but better yet
    *  GL_LINEAR_MIPMAP_LINEAR    -- trilinear, best quality short of anisotropic
    */
   /*> glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);   <*/
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   /*---(select wrapping to not fuzz edges)-----*/
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   /*---(must be modulate for intensity texture)*/
   glTexEnvi       (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   /*---(copy into a texture)-------------------*/
   /* use the GL_ALPHA format to drive antialiased and clear text             */
   glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
   glTexImage2D    (GL_TEXTURE_2D, 0, GL_ALPHA, x_font->tex_w, x_font->tex_h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, x_font->tex_map );
   /*---(complete)------------------------------*/
   glBindTexture   (GL_TEXTURE_2D, 0);
   DEBUG_YFONT   yLOG_exit    (__FUNCTION__);
   return 0;
}

tVERT*             /* PURPOSE : find a particular letter in the texture atlas */
yFONT__verts_find    (tFONT *a_font, int a_code)
{
   /*---(locals)-----------+-----------+-*/
   long        x_index     = -1;       /* found glyph index                   */
   tVERT      *x_vert      = NULL;     /* found glyph vertex pointer          */
   uint        x_min       = 0;
   uint        x_max       = 0;
   /*---(prepare)------------------------*/
   x_min   = a_font->min_glyph;
   x_max   = a_font->min_glyph + a_font->range;
   /*---(check range)--------------------*/
   if (a_code <  x_min) {
      return NULL;
   }
   if (a_code >= x_max) {
      return NULL;
   }
   /*---(find in index table)------------*/
   x_index = a_font->lookup [a_code - a_font->min_glyph];
   if (x_index < 0) {
      return NULL;
   }
   /*---(translate to glyph)-------------*/
   x_vert = a_font->verts + x_index;
   /*---(complete)-----------------------*/
   return x_vert;
}

char               /* PURPOSE : draw a single glyph at the current location   */
yFONT__map_glyph   (tFONT *a_font, int a_code)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   tVERT      *x_verts     = NULL;
   /*---(header)-------------------------*/
   DEBUG_YFONT_MAP    yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_MAP    yLOG_point   ("a_font"    , a_font);
   DEBUG_YFONT_MAP    yLOG_value   ("a_code"    , a_code);
   /*---(get glyph)----------------------*/
   --rce;  if (a_font == NULL) {
      DEBUG_YFONT_MAP    yLOG_warn    ("a_font"   , "font has not been assigned to this slot");
      DEBUG_YFONT_MAP    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_verts = yFONT__verts_find  (a_font, a_code);
   DEBUG_YFONT_MAP    yLOG_point   ("x_verts"   , x_verts);
   --rce;  if (x_verts == NULL) {
      DEBUG_YFONT_MAP    yLOG_warn    ("x_verts"  , "verticies not found for code");
      DEBUG_YFONT_MAP    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(bounds)-------------------------*/
   DEBUG_YFONT_MAP    yLOG_double  ("top"       , x_verts->TL.rel_y);
   DEBUG_YFONT_MAP    yLOG_double  ("bottom"    , x_verts->BR.rel_y);
   DEBUG_YFONT_MAP    yLOG_double  ("left"      , x_verts->TL.rel_x);
   DEBUG_YFONT_MAP    yLOG_double  ("right"     , x_verts->BR.rel_x);
   /*---(draw texture)-------------------*/
   glPushMatrix(); {
      glBegin(GL_QUADS); {
         glTexCoord2d  (x_verts->TL.tex_x, x_verts->TL.tex_y);
         glVertex2f    (x_verts->TL.rel_x, x_verts->TL.rel_y);
         glTexCoord2d  (x_verts->TR.tex_x, x_verts->TR.tex_y);
         glVertex2f    (x_verts->TR.rel_x, x_verts->TR.rel_y);
         glTexCoord2d  (x_verts->BR.tex_x, x_verts->BR.tex_y);
         glVertex2f    (x_verts->BR.rel_x, x_verts->BR.rel_y);
         glTexCoord2d  (x_verts->BL.tex_x, x_verts->BL.tex_y);
         glVertex2f    (x_verts->BL.rel_x, x_verts->BL.rel_y);
      } glEnd();
   } glPopMatrix();
   /*---(advance)------------------------*/
   DEBUG_YFONT_MAP    yLOG_value   ("advance"   , x_verts->a);
   glTranslatef (x_verts->a, 0.0, 0.0);
   /*---(complete)-----------------------*/
   DEBUG_YFONT_MAP    yLOG_exit    (__FUNCTION__);
   return 0;
}


