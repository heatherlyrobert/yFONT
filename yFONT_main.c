/*============================----beg-of-source---============================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include   <png.h>                     /* to handle avatars                   */


/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"




int     nfont     =  0;
GLuint  syms      =  0;



/*---(prototypes)-------------------------------*/
static char      yFONT__init        (tFONT *a_txf);
static tVERT*    yFONT__findvert    (tFONT *a_txf, uint   a_code);
static char      yFONT__glyph       (tFONT *a_txf, uint   a_code);
static GLuint    yFONT__texture     (tFONT *a_txf, GLuint a_texobj);
static int       yFONT__width       (tFONT *a_txf, char *a_str, int a_len);
static char      yFONT__free        (tFONT *a_txf);




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

static char        /* PURPOSE : clear a new font -----------------------------*/
yFONT__init        (tFONT *a_txf)
{
   a_txf->file         = NULL;
   a_txf->point        = 0;
   a_txf->tex_w        = 0;
   a_txf->tex_h        = 0;
   a_txf->tex_ref      = 0;
   a_txf->tex_map      = NULL;
   a_txf->max_ascent   = 0;
   a_txf->max_descent  = 0;
   a_txf->num_glyph    = 0;
   a_txf->min_glyph    = 0;
   a_txf->max_glyph    = 0;
   a_txf->range        = 0;
   a_txf->glyphs       = NULL;
   a_txf->verts        = NULL;
   a_txf->lookup       = NULL;
   return 0;
}

static char        /* PURPOSE : free a font from texture memory --------------*/
yFONT__free        (tFONT  *a_txf)
{
   if (a_txf == NULL)                     return YF_BAD_SLOT;
   /*---(free it up)----------------------------*/
   if (a_txf->tex_map  )  free (a_txf->tex_map );
   if (a_txf->glyphs   )  free (a_txf->glyphs  );
   if (a_txf->verts    )  free (a_txf->verts   );
   if (a_txf->lookup   )  free (a_txf->lookup  );
   free (a_txf);
   /*---(complete)------------------------------*/
   return 0;
}

static tVERT*      /* PURPOSE : find a particular letter in the texture atlas */
yFONT__findvert    (tFONT *a_font, uint a_code)
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

/*> static char        /+ PURPOSE : draw a single glyph at the current location   +/            <* 
 *> yFONT__glyph_ORIG (tFONT *a_txf, int a_ch)                                                  <* 
 *> {                                                                                           <* 
 *>    tVERT *tgvi;                                                                             <* 
 *>    tgvi = yFONT__findvert(a_txf, a_ch);                                                         <* 
 *>    if (tgvi == NULL) return -1;                                                             <* 
 *>    /+> if (tgvi == NULL)  printf("letter %c not found\n", a_ch);                      <*    <* 
 *>     *> else               printf("letter %c found\n", a_ch);                          <+/   <* 
 *>    glBegin(GL_QUADS);                                                                       <* 
 *>    glTexCoord2fv (tgvi->t0);                                                                <* 
 *>    glVertex2sv   (tgvi->v0);                                                                <* 
 *>    glTexCoord2fv (tgvi->t1);                                                                <* 
 *>    glVertex2sv   (tgvi->v1);                                                                <* 
 *>    glTexCoord2fv (tgvi->t2);                                                                <* 
 *>    glVertex2sv   (tgvi->v2);                                                                <* 
 *>    glTexCoord2fv (tgvi->t3);                                                                <* 
 *>    glVertex2sv   (tgvi->v3);                                                                <* 
 *>    glEnd();                                                                                 <* 
 *>    glTranslatef(tgvi->a, 0.0, 0.0);                                                         <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

static char        /* PURPOSE : draw a single glyph at the current location   */
yFONT__glyph       (tFONT *a_txf, uint a_code)
{
   /*---(locals)-----------+-----------+-*/
   tVERT      *x_verts     = NULL;
   /*---(get glyph)----------------------*/
   x_verts = yFONT__findvert (a_txf, a_code);
   if (x_verts == NULL) return -1;
   /*> printf ("yFONT__glyph : %2d %-15s %5d %c %-10p : ", a_txf->slot, a_txf->name, a_code, (a_code < 128) ? a_code : '-', x_verts);   <* 
    *> printf ("%7.3f %7.3f %7.3f %7.3f %7.5f %7.5f %7.5f %7.5f\n",                                                                     <* 
    *>       x_verts->TL.rel_x, x_verts->TR.rel_x, x_verts->TL.rel_y, x_verts->BL.rel_y,                                                <* 
    *>       x_verts->TL.tex_x, x_verts->TR.tex_x, x_verts->TL.tex_y, x_verts->BL.tex_y);                                               <*/
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
   glTranslatef(x_verts->a, 0.0, 0.0);
   /*---(complete)-----------------------*/
   return 0;
}

static int         /* PURPOSE : determine the width of a displayed string     */
yFONT__uwidth      (tFONT *a_txf, uint *a_array, int a_len)
{
   int         i           = 0;                 /* iterator -- character          */
   int         w           = 0;                 /* width                          */
   tVERT      *x_vert;
   for (i = 0; i < a_len; i++) {
      x_vert = yFONT__findvert (a_txf, a_array[i]);
      if (x_vert == NULL) continue;
      w += x_vert->a;
   }
   return w;
}

static int         /* PURPOSE : determine the width of a displayed string     */
yFONT__width       (tFONT *a_txf, char *a_str, int a_len)
{
   int         i           = 0;                 /* iterator -- character          */
   int         w           = 0;                 /* width                          */
   tVERT      *x_vert;
   for (i = 0; i < a_len; i++) {
      x_vert = yFONT__findvert (a_txf, a_str[i]);
      if (x_vert == NULL) continue;
      w += x_vert->a;
   }
   return w;
}

static GLuint      /* PURPOSE : set up a font texture                         */
yFONT__texture     (tFONT *txf, GLuint a_texobj)
{
   if (txf->tex_ref == 0) {
      if (a_texobj == 0) {
         glGenTextures(1, &txf->tex_ref);
      } else {
         txf->tex_ref = a_texobj;
      }
   }
   glBindTexture   (GL_TEXTURE_2D, txf->tex_ref);
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
   glTexImage2D    (GL_TEXTURE_2D, 0, GL_ALPHA, txf->tex_w, txf->tex_h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, txf->tex_map );
   /*---(complete)------------------------------*/
   glBindTexture   (GL_TEXTURE_2D, 0);
   return txf->tex_ref;
}

static tFONT*             /* PURPOSE : read a font header                            */
yFONT__trhead (char *a_name)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;                 /* generic return code            */
   tFONT    *txf       = NULL;              /* new font                       */
   char      fileid[4];                     /* magic number of file           */
   int       type      = 0;                 /* data type                      */
   int       value     = 0;
   char      x_name [1000];
   /*---(allocate the font)---------------------*/
   txf = (tFONT *) malloc(sizeof(tFONT));
   if (txf == NULL) {
      fprintf(stderr, "yFONT_load() : can not allocate memory for font\n");
      free (txf);
      return NULL;
   }
   /*---(initialize)----------------------------*/
   yFONT__slot_init  (txf);
   /*---(open file)-----------------------------*/
   snprintf(x_name, 900, "/usr/local/share/fonts/%s.txf", a_name);
   txf->file = fopen(x_name, "rb");
   if (txf->file == NULL) {
      fprintf(stderr, "yFONT_load() : can not open font file\n");
      yFONT__free (txf);
      return NULL;
   }
   /*---(check file type)-----------------------*/
   rc = fread(fileid, 1, 4, txf->file);
   if (rc != 4) { fprintf(stderr, "yFONT_load() : premature end of file (magic#)\n"); yFONT__free (txf); return NULL; }
   if (strncmp(fileid, "\377txf", 4) != 0) {
      fprintf(stderr, "yFONT_load() : not a txf formatted file\n");
      yFONT__free (txf);
      return NULL;
   }
   /*---(name)----------------------------------*/
   rc = fread (&x_name,   sizeof (char), 20, txf->file);
   if (rc != 20) { fprintf(stderr, "yFONT_load() : premature end of file (name)\n"); yFONT__free (txf); return NULL; }
   strcpy (txf->name, x_name);
   /*---(point size)----------------------------*/
   rc = fread(&value,   sizeof(int), 1, txf->file);
   if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (point)\n"); yFONT__free (txf); return NULL; }
   txf->point           = value;
   if (txf->point < 6 || txf->point > 99) { fprintf(stderr, "yFONT_load() : point size out of range %d (6-99)\n", txf->point); yFONT__free (txf); return NULL; }
   /*> printf("points      = %8d\n", txf->point);                                         <*/
   /*---(check metrics)-------------------------*/
   rc = fread(&value,   sizeof(ulong), 1, txf->file);
   if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (width)\n"); yFONT__free (txf); return NULL; }
   txf->tex_w           = value;
   /*> printf("tex width   = %8d\n", txf->tex_w);                                         <*/
   /*---()---*/
   rc = fread(&value,   sizeof(int), 1, txf->file);
   if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (height)\n"); yFONT__free (txf); return NULL; }
   txf->tex_h           = value;
   /*> printf("tex height  = %8d\n", txf->tex_h);                                         <*/
   /*---()---*/
   rc = fread(&value,   sizeof(int), 1, txf->file);
   if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (ascent)\n"); yFONT__free (txf); return NULL; }
   txf->max_ascent  = value;
   /*> printf("max ascent  = %8d\n", txf->max_ascent);                                <*/
   /*---()---*/
   rc = fread(&value,   sizeof(int), 1, txf->file);
   if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (descent)\n"); yFONT__free (txf); return NULL; }
   txf->max_descent = value;
   /*> printf("max descent = %8d\n", txf->max_descent);                               <*/
   /*---()---*/
   rc = fread(&value,   sizeof(int), 1, txf->file);
   if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (margin)\n"); yFONT__free (txf); return NULL; }
   txf->margin      = value;
   /*---()---*/
   rc = fread(&value,   sizeof(int), 1, txf->file);
   if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (num glyphs)\n"); yFONT__free (txf); return NULL; }
   txf->num_glyph  = value;
   /*> printf("num glyphs  = %8d\n", txf->num_glyph);                                <*/
   /*---(complete)------------------------------*/
   return txf;
}

/*> /+---(read glyph verticies)------------------+/                                                                                                                                                                   <* 
 *> texw = txf->tex_w;                                                                                                                                                                                                    <* 
 *> texh = txf->tex_h;                                                                                                                                                                                                    <* 
 *> for (i = 0; i < txf->num_glyphs; i++) {                                                                                                                                                                           <* 
 *>    tGLYPH   *tgi;                                                                                                                                                                                                 <* 
 *>    tgi = &txf->tgi[i];                                                                                                                                                                                            <* 
 *>    float   t_top    = (texh - tgi->ypos)                             / texh;                                                                                                                                         <* 
 *>    float   t_bottom = (texh - (tgi->ypos + tgi->tall))                  / texh;                                                                                                                                         <* 
 *>    float   t_left   = (tgi->xpos)                                    / texw;                                                                                                                                         <* 
 *>    float   t_right  = (tgi->xpos + tgi->w)                           / texw;                                                                                                                                         <* 
 *>    /+> printf("%c   -fx- -fy-  -xo- -yo-  -fw- -fh-  ---tx--- ---ty---  -vx- -vy-\n", tgi->code);   <+/                                                                                                              <* 
 *>    /+---(top-left)---------------------------+/                                                                                                                                                                   <* 
 *>    txf->tgvi[i].t0[0] =  t_left;                                                                                                                                                                                  <* 
 *>    txf->tgvi[i].t0[1] =  t_top;                                                                                                                                                                                   <* 
 *>    /+> txf->tgvi[i].t0[1] =  1.0 - txf->tgvi[i].t0[1];                             <+/                                                                                                                            <* 
 *>    txf->tgvi[i].v0[0] =  tgi->xoff;                                                                                                                                                                                 <* 
 *>    txf->tgvi[i].v0[1] =  tgi->yoff;                                                                                                                                                                                 <* 
 *>    /+> printf("  0 %4d %4d  %4d %4d  %4d %4d  %8.6f %8.6f  %4d %4d\n", tgi->xpos, tgi->ypos, tgi->xoff, tgi->yoff, tgi->w, tgi->tall, txf->tgvi[i].t0[0], txf->tgvi[i].t0[1], txf->tgvi[i].v0[0], txf->tgvi[i].v0[1]);   <+/   <* 
 *>    /+---(top-right)--------------------------+/                                                                                                                                                                   <* 
 *>    txf->tgvi[i].t1[0] =  t_right;                                                                                                                                                                                 <* 
 *>    txf->tgvi[i].t1[1] =  t_top;                                                                                                                                                                                   <* 
 *>    /+> txf->tgvi[i].t1[1] =  1.0 - txf->tgvi[i].t1[1];                             <+/                                                                                                                            <* 
 *>    txf->tgvi[i].v1[0] =  tgi->xoff + tgi->w;                                                                                                                                                                        <* 
 *>    txf->tgvi[i].v1[1] =  tgi->yoff;                                                                                                                                                                                 <* 
 *>    /+> printf("  1 %4d %4d  %4d %4d  %4d %4d  %8.6f %8.6f  %4d %4d\n", tgi->xpos, tgi->ypos, tgi->xoff, tgi->yoff, tgi->w, tgi->tall, txf->tgvi[i].t1[0], txf->tgvi[i].t1[1], txf->tgvi[i].v1[0], txf->tgvi[i].v1[1]);   <+/   <* 
 *>    /+---(bottom-right)-----------------------+/                                                                                                                                                                   <* 
 *>    txf->tgvi[i].t2[0] =  t_right;                                                                                                                                                                                 <* 
 *>    txf->tgvi[i].t2[1] =  t_bottom;                                                                                                                                                                                <* 
 *>    /+> txf->tgvi[i].t2[1] =  1.0 - txf->tgvi[i].t2[1];                             <+/                                                                                                                            <* 
 *>    txf->tgvi[i].v2[0] =  tgi->xoff + tgi->w;                                                                                                                                                                        <* 
 *>    txf->tgvi[i].v2[1] =  tgi->yoff - tgi->tall;                                                                                                                                                                        <* 
 *>    /+> printf("  2 %4d %4d  %4d %4d  %4d %4d  %8.6f %8.6f  %4d %4d\n", tgi->xpos, tgi->ypos, tgi->xoff, tgi->yoff, tgi->w, tgi->tall, txf->tgvi[i].t2[0], txf->tgvi[i].t2[1], txf->tgvi[i].v2[0], txf->tgvi[i].v2[1]);   <+/   <* 
 *>    /+---(bottom-left)------------------------+/                                                                                                                                                                   <* 
 *>    txf->tgvi[i].t3[0] =  t_left;                                                                                                                                                                                  <* 
 *>    txf->tgvi[i].t3[1] =  t_bottom;                                                                                                                                                                                <* 
 *>    /+> txf->tgvi[i].t3[1] =  1.0 - txf->tgvi[i].t3[1];                             <+/                                                                                                                            <* 
 *>    txf->tgvi[i].v3[0] =  tgi->xoff;                                                                                                                                                                                 <* 
 *>    txf->tgvi[i].v3[1] =  tgi->yoff - tgi->tall;                                                                                                                                                                        <* 
 *>    /+> printf("  3 %4d %4d  %4d %4d  %4d %4d  %8.6f %8.6f  %4d %4d\n", tgi->xpos, tgi->ypos, tgi->xoff, tgi->yoff, tgi->w, tgi->tall, txf->tgvi[i].t3[0], txf->tgvi[i].t3[1], txf->tgvi[i].v3[0], txf->tgvi[i].v3[1]);   <+/   <* 
 *>    /+---(advance)----------------------------+/                                                                                                                                                                   <* 
 *>    txf->tgvi[i].a     = tgi->a;                                                                                                                                                                                   <* 
 *> }                                                                                                                                                                                                                 <*/

char         /*--> create vertex table -------------------[ ------ [ ------ ]-*/
yFONT__verts       (tFONT *a_txf)
{
   /*---(locals)-----------+-----------+-*/
   int         i           = 0;
   tGLYPH     *x_glyph     = NULL;
   long        x_texw      = 0;
   long        x_texh      = 0;
   double      x_top       = 0;
   double      x_bottom    = 0;
   double      x_left      = 0;
   double      x_right     = 0;
   /*---(allocate the glyph verticies)----------*/
   a_txf->verts = (tVERT *) malloc (a_txf->num_glyph * sizeof(tVERT));
   if (a_txf->verts == NULL) {
      fprintf(stderr, "yFONT_load() : could not allocate glyph verticies\n");
      yFONT__free (a_txf);
      return YF_MEM_FULL;
   }
   /*---(read glyph verticies)-----------*/
   x_texw = a_txf->tex_w;
   x_texh = a_txf->tex_h;
   for (i = 0; i < a_txf->num_glyph; i++) {
      /*---(point to glyph)--------------*/
      x_glyph = &a_txf->glyphs[i];
      /*> printf ("verts : %2d %-15s %5d %c : ", a_txf->slot, a_txf->name, x_glyph->code, (x_glyph->code < 128) ? x_glyph->code : '-');   <* 
       *> printf ("x=%5d y=%3d w=%3d h=%3d xo=%3d yo=%3d : ",                                                                    <* 
       *>       x_glyph->xpos, x_glyph->ypos, x_glyph->w, x_glyph->tall, x_glyph->xoff, x_glyph->yoff);                                       <*/
      /*---(relative coordinates)--------*/
      /* flip top and bottom for opengl */
      x_top    = (double) (x_glyph->ypos - 2                   )  / (double) x_texh;
      x_bottom = (double) (x_glyph->ypos + x_glyph->tall + 2)        / (double) x_texh;
      x_left   = (double) (          x_glyph->xpos - 2         )  / (double) x_texw;
      x_right  = (double) (      2 + x_glyph->xpos + x_glyph->wide)  / (double) x_texw;
      /*> printf ("t=%7.5f b=%7.5f l=%7.5f r=%7.5f : ",                               <* 
       *>       x_top, x_bottom, x_left, x_right);                                    <*/
      /*---(top-left)--------------------*/
      a_txf->verts[i].TL.rel_x  = x_glyph->xoff - 2;
      a_txf->verts[i].TL.rel_y  = x_glyph->yoff + 2;
      a_txf->verts[i].TL.tex_x  = x_left;
      a_txf->verts[i].TL.tex_y  = x_top;
      /*> printf ("rx=%7d ry=%7d tx=%7.5f ty=%7.5f\n",                                                                     <* 
       *>       a_txf->verts[i].TL.rel_x, a_txf->verts[i].TL.rel_y, a_txf->verts[i].TL.tex_x, a_txf->verts[i].TL.tex_y);   <*/
      /*---(top-right)-------------------*/
      a_txf->verts[i].TR.rel_x  = x_glyph->xoff + x_glyph->wide + 2;
      a_txf->verts[i].TR.rel_y  = x_glyph->yoff + 2;
      a_txf->verts[i].TR.tex_x  = x_right;
      a_txf->verts[i].TR.tex_y  = x_top;
      /*---(bottom-right)----------------*/
      a_txf->verts[i].BR.rel_x  = x_glyph->xoff + x_glyph->wide + 2;
      a_txf->verts[i].BR.rel_y  = x_glyph->yoff - x_glyph->tall - 2;
      a_txf->verts[i].BR.tex_x  = x_right;
      a_txf->verts[i].BR.tex_y  = x_bottom;
      /*---(bottom-left)-----------------*/
      a_txf->verts[i].BL.rel_x  = x_glyph->xoff - 2;
      a_txf->verts[i].BL.rel_y  = x_glyph->yoff - x_glyph->tall - 2;
      a_txf->verts[i].BL.tex_x  = x_left;
      a_txf->verts[i].BL.tex_y  = x_bottom;
      /*---(advance)---------------------*/
      a_txf->verts[i].a         = x_glyph->adv;
      /*---(done)------------------------*/
   }
   /*---(complete)------------------------------*/
   return 0;
}

char         /*--> create lookup table -------------------[ ------ [ ------ ]-*/
yFONT__index       (tFONT *a_txf)
{
   /*---(locals)-----------+-----------+-*/
   uint        min_glyph   = 0;
   uint        max_glyph   = 0;
   int         i           = 0;
   /*---(identify range)-----------------*/
   min_glyph = a_txf->glyphs[0].code;
   max_glyph = a_txf->glyphs[0].code;
   for (i = 1; i < a_txf->num_glyph; i++) {
      if (a_txf->glyphs[i].code < min_glyph) {
         min_glyph = a_txf->glyphs[i].code;
      }
      if (a_txf->glyphs[i].code > max_glyph) {
         max_glyph = a_txf->glyphs[i].code;
      }
   }
   a_txf->min_glyph = min_glyph;
   a_txf->range     = max_glyph - min_glyph + 1;
   /*---(create lookup table)------------*/
   a_txf->lookup = (short *) malloc (a_txf->range * sizeof (short));
   if (a_txf->lookup == NULL) {
      fprintf(stderr, "yFONT_load() : could not allocate lookup\n");
      yFONT__free (a_txf);
      return YF_MEM_FULL;
   }
   /*---(initialize lookup table)--------*/
   for (i = 0; i < a_txf->range; i++)  a_txf->lookup [i] = -1;
   /*---(load lookup table)--------------*/
   for (i = 0; i < a_txf->num_glyph; i++) {
      a_txf->lookup [a_txf->glyphs[i].code - a_txf->min_glyph] = i;
   }
   /*---(complete)-----------------------*/
   return 0;
}

int                /* PURPOSE : load a font from a file ----------------------*/
yFONT_load         (char *a_name)
{
   /*---(locals)--------------------------------*/
   char        rce         = -10;           /* return code for errors         */
   int       rc        = 0;                 /* generic return code            */
   FILE     *file      = NULL;
   tFONT    *x_txf       = NULL;
   GLfloat texw, texh;
   unsigned char *texbitmap;
   uint        min_glyph   = 0;
   uint        max_glyph   = 0;
   int stride, width, height;
   int i, j;
   int         x_slot      = -1;
   /*---(header)-------------------------*/
   DEBUG_YFONT    yLOG_enter   (__FUNCTION__);
   /*---(get font slot)------------------*/
   DEBUG_YFONT    yLOG_note    ("assign the font slot");
   x_slot = yFONT__slot_new   ();
   --rce;  if (x_slot <  0) {
      DEBUG_YFONT    yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(open file)-----------------------------*/
   DEBUG_YFONT    yLOG_point   ("*a_name"   , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YFONT    yLOG_warn    ("font name" , "can not be null");
      DEBUG_YFONT    yLOG_exit    (__FUNCTION__);
      return x_slot;
   }
   DEBUG_YFONT    yLOG_info    ("a_name"    , a_name);


   x_txf  = yFONT__trhead (a_name);
   if (x_txf == NULL) return YF_BAD_HEAD;
   x_txf->slot = x_slot;
   file = x_txf->file;
   /*---(allocate the glyph info)---------------*/
   x_txf->glyphs = (tGLYPH *) malloc(x_txf->num_glyph * sizeof(tGLYPH));
   if (x_txf->glyphs == NULL) {
      fprintf(stderr, "yFONT_load() : could not allocate glyph info\n");
      yFONT__free (x_txf);
      return YF_MEM_FULL;
   }
   /*---(read all information)------------------*/
   rc = fread(x_txf->glyphs, sizeof(tGLYPH), x_txf->num_glyph, file);
   if (rc != x_txf->num_glyph) { fprintf(stderr, "yFONT_load() : premature end of file (reading glyphs)\n"); yFONT__free (x_txf); return YF_PREMATURE_END; }
   /*---(allocate the glyph verticies)----------*/
   yFONT__verts (x_txf);
   yFONT__index (x_txf);
   /*---(type)---------------------------*/
   x_txf->tex_map  = (uchar *) malloc (x_txf->tex_w * x_txf->tex_h);
   if (x_txf->tex_map  == NULL) {
      fprintf(stderr, "yFONT_load() : could not allocate glyph\n");
      yFONT__free (x_txf);
      return YF_MEM_FULL;
   }
   rc = fread (x_txf->tex_map , 1, x_txf->tex_w * x_txf->tex_h, file);
   if (rc != x_txf->tex_w * x_txf->tex_h) { fprintf(stderr, "yFONT_load() : premature end of file (byte)\n"); yFONT__free (x_txf); return YF_MEM_FULL; }

   fclose(file);
   yFONT__texture (x_txf, x_txf->tex_ref);
   /*> yFONT_list (x_txf);                                                              <*/
   /*---(complete)-----------------------*/
   g_font [x_slot] = x_txf;
   DEBUG_YFONT    yLOG_exit    (__FUNCTION__);
   return x_slot;
}


char               /* PURPOSE : free a font from texture memory --------------*/
yFONT_free         (int  a_slot)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tFONT  *x_txf = g_font [a_slot];
   if (x_txf == NULL)                     return YF_BAD_SLOT;
   /*---(free it up)----------------------------*/
   yFONT__free(x_txf);
   g_font [a_slot] = NULL;
   /*---(complete)------------------------------*/
   return 0;
}

int                /* PURPOSE : print a string with word wrapping             */
yFONT_printw (int  a_slot, int a_point, char a_align, char *a_str, int a_width, int a_height, float a_wrap)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tFONT  *x_txf = g_font [a_slot];
   if (x_txf == NULL)                     return YF_BAD_SLOT;
   /*---(locals)--------------------------------*/
   int       i         = 0;                 /* iterator -- character          */
   int       len       = strlen(a_str);     /* string length                  */
   int       s         = 0;                 /* start of next print            */
   int       w         = 0;                 /* current width                  */
   int       l         = 1;                 /* number of lines                */
   int       space     = 0;                 /* last breakable place in text   */
   char     *x_str     = strdup(a_str);     /* alterable string               */
   float     scale     = (float) a_point / (float) x_txf->point;
   int       v         = 0;                 /* vertical spacing               */
   v = (x_txf->max_ascent - x_txf->max_descent) * a_wrap * scale;
   if (l * v > a_height) return -1;
   tVERT      *x_vert;
   for (i = 0; i < len; i++) {
      if (x_str[i] == '\0') break;
      if (strchr("-/ ", x_str[i]) != 0) {
         space = i;
      }
      x_vert = yFONT__findvert (x_txf, x_str[i]);
      if (x_vert == NULL) continue;
      w += x_vert->a * scale;
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
yFONT_uprint       (int  a_slot, int a_point, char a_align, uint *a_array, int a_max)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tFONT  *x_txf = g_font [a_slot];
   if (x_txf == NULL)                     return YF_BAD_SLOT;
   /*---(locals)--------------------------------*/
   int       i         = 0;                 /* iterator -- character          */
   int       x_len     = 0;                  /* string length                  */
   float     scale     = (float) a_point / (float) x_txf->point;
   int       w         = 0;
   float     x         = 0;
   float     y         = 0;
   /*---(get length)----------------------------*/
   for (i = 0; i < a_max; ++i) {
      if (a_array [i] == '\0') break;
      ++x_len;
   }
   /*---(place start)---------------------------*/
   w = yFONT__uwidth (x_txf, a_array, x_len);
   switch (a_align / 3) {
   case  0:  y =  0;                      break;
   case  1:  y = -x_txf->max_ascent / 2;  break;
   case  2:  y = -x_txf->max_ascent;      break;
   case  3:  y = -x_txf->max_descent;     break;
   }
   switch (a_align % 3) {
   case  0:  x =  0;                      break;
   case  1:  x = -w / 2;                  break;
   case  2:  x = -w;                      break;
   }
   /*---(draw text)-----------------------------*/
   glPushMatrix(); {
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBindTexture (GL_TEXTURE_2D, x_txf->tex_ref);
      glScalef      (scale, scale, scale);
      glTranslatef  (x, y, 0);
      for (i = 0; i < x_len; ++i) {
         yFONT__glyph (x_txf, a_array [i]);
      }
      glBindTexture(GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(complete)------------------------------*/
   return w * scale;
}

int                /* PURPOSE : print a string -------------------------------*/
yFONT_print  (int  a_slot, int a_point, char a_align, char *a_str)
{
   /*---(defense)------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tFONT  *x_txf = g_font [a_slot];
   if (x_txf == NULL)                     return YF_BAD_SLOT;
   /*---(locals)-------------------------*/
   int       i         = 0;                 /* iterator -- character          */
   int       len       = strlen(a_str);     /* string length                  */
   float     scale     = (float) a_point / (float) x_txf->point;
   int       w         = 0;
   float     x         = 0;
   float     y         = 0;
   /*---(place start)--------------------*/
   w = yFONT__width (x_txf, a_str, len);
   switch (a_align / 3) {
   case  0:  y =  0;                      break;
   case  1:  y = -x_txf->max_ascent / 2;  break;
   case  2:  y = -x_txf->max_ascent;      break;
   case  3:  y = -x_txf->max_descent;     break;
   }
   switch (a_align % 3) {
   case  0:  x =  0;                      break;
   case  1:  x = -w / 2;                  break;
   case  2:  x = -w;                      break;
   }
   /*---(draw text)----------------------*/
   glPushMatrix(); {
      glScalef      (scale, scale, scale);
      /*---(draw baseline)---------------*/
      /*> glBegin(GL_POLYGON); {                                                      <* 
       *>    glVertex3f   ( 0.0,   0.0 , 20.0);                                       <* 
       *>    glVertex3f   (   w,   0.0 , 20.0);                                       <* 
       *>    glVertex3f   (   w,  -2.0 , 20.0);                                       <* 
       *>    glVertex3f   ( 0.0,  -2.0 , 20.0);                                       <* 
       *> } glEnd();                                                                  <*/
      /*> for (i = 0; i < w; i += 5) {                                                <* 
       *>    glBegin(GL_POLYGON); {                                                   <* 
       *>       glVertex3f   ( i + 0.0,  29.0 , 20.0);                                <* 
       *>       glVertex3f   ( i + 2.0,  29.0 , 20.0);                                <* 
       *>       glVertex3f   ( i + 2.0,  27.0 , 20.0);                                <* 
       *>       glVertex3f   ( i + 0.0,  27.0 , 20.0);                                <* 
       *>    } glEnd();                                                               <* 
       *> }                                                                           <*/
      /*> for (i = 0; i < w; i += 5) {                                                <* 
       *>    glBegin(GL_POLYGON); {                                                   <* 
       *>       glVertex3f   ( i + 0.0,  39.0 , 20.0);                                <* 
       *>       glVertex3f   ( i + 2.0,  39.0 , 20.0);                                <* 
       *>       glVertex3f   ( i + 2.0,  37.0 , 20.0);                                <* 
       *>       glVertex3f   ( i + 0.0,  37.0 , 20.0);                                <* 
       *>    } glEnd();                                                               <* 
       *> }                                                                           <*/
      /*---(draw characters)-------------*/
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBindTexture (GL_TEXTURE_2D, x_txf->tex_ref);
      /*> glScalef      (scale, scale, scale);                                        <*/
      glTranslatef  (x, y, 0);
      for (i = 0; i < len; ++i) {
         yFONT__glyph (x_txf, a_str[i]);
      }
      glBindTexture (GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(complete)-----------------------*/
   return w * scale;
}

int                /* PURPOSE : make a png image into a texture --------------*/
yFONT_symload      (char *a_filename)
{
   /*> yLOG_enter  (__FUNCTION__);                                                    <*/
   /*---(locals)-----------+-----------+-*/
   png_byte    header      [8];             /* png header                     */
   png_structp png_ptr;
   png_infop   info_ptr, end_info;
   int         interlace_type;
   int         rc          = 0;             /* simple return code             */
   int         len         = 0;
   FILE       *f           = NULL;          /* file pointer                   */
   ulong       twidth, theight;
   int         width, height;
   int         bit_depth, color_type;
   /*---(defense)------------------------*/
   syms = 0;
   if (a_filename == NULL) {
      printf("   - FATAL, filename is null\n");
      return  -1;
   }
   len = strlen (a_filename);
   if (len < 5) {
      printf("   - FATAL, file name too short (%d < 5)\n", len);
      return  -2;
   }
   /*---(open png file)------------------*/
   /*> yLOG_info   ("file"      , a_filename);                                        <*/
   /*> printf ("\npng_load (%s)\n", a_filename);                                      <*/
   f = fopen(a_filename, "rb");
   if (f == NULL) {
      printf("   - FATAL, can not find file\n");
      return -3;
   }
   /*> yLOG_info   ("status"    , "file found");                                      <*/
   /*> printf("   - file found\n");                                                   <*/
   /*---(check the header)---------------*/
   fread (header, 1, 8, f);
   rc = png_sig_cmp(header, 0, 8);
   if (rc != 0) {
      fclose(f);
      printf("   - FATAL, header not png\n");
      return  -4;
   }
   /*> yLOG_info   ("format"    , "png");                                             <*/
   /*> printf("   - header is png\n");                                                <*/
   /*---(create read struct)-------------*/
   png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL) {
      fclose(f);
      printf("   - FATAL, png_ptr error\n");
      return  -5;
   }
   /*> yLOG_info   ("png_ptr"   , "pointer is good");                                 <*/
   /*> printf("   - png pointer is good\n");                                          <*/
   /*---(create info struct)-------------*/
   info_ptr = png_create_info_struct (png_ptr);
   if (info_ptr == NULL) {
      fclose(f);
      png_destroy_read_struct (&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
      printf("   - FATAL, info_ptr error\n");
      return  -6;
   }
   /*> yLOG_info   ("info_ptr"  , "pointer is good");                                 <*/
   /*> printf("   - info pointer is good\n");                                         <*/
   /*---(create info struct)-------------*/
   end_info = png_create_info_struct (png_ptr);
   if (end_info == NULL) {
      fclose(f);
      png_destroy_read_struct (&png_ptr, &info_ptr, (png_infopp) NULL);
      printf("   - FATAL, end_ptr error\n");
      return  -7;
   }
   /*> yLOG_info   ("end_ptr"   , "pointer is good");                                 <*/
   /*> printf("   - end pointer is good\n");                                          <*/
   /*---(error stuff 1)------------------*/
   if (setjmp (png_jmpbuf (png_ptr))) {
      fclose(f);
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      printf("   - FATAL, error during init_io\n");
      return  -8;
   }
   /*> yLOG_info   ("jump"      , "set jump is good");                                <*/
   /*> printf("   - setjmp is good\n");                                               <*/
   /*---(setup read)---------------------*/
   png_init_io          (png_ptr, f);
   png_set_sig_bytes    (png_ptr, 8);
   png_read_info        (png_ptr, info_ptr);
   /*---(get info)-----------------------*/
   png_get_IHDR         (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
   twidth  = width;
   theight = height;
   png_read_update_info (png_ptr, info_ptr);
   if (color_type != PNG_COLOR_TYPE_RGBA) {
      fclose(f);
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      printf("   - FATAL, color is NOT RGBA\n");
      return  -9;
   }
   /*> yLOG_info   ("oolor"     , "RGBA is excellent");                               <*/
   /*> printf("   - color is RGBA, excellent\n");                                     <*/
   unsigned int rowbytes = png_get_rowbytes (png_ptr, info_ptr);
   int channels = png_get_channels (png_ptr, info_ptr);
   /*> yLOG_value  ("width"     , twidth);                                            <*/
   /*> yLOG_value  ("height"    , theight);                                           <*/
   /*> yLOG_value  ("row_bytes" , rowbytes);                                          <*/
   /*> yLOG_value  ("bits"      , bit_depth);                                         <*/
   /*> yLOG_value  ("channels"  , channels);                                          <*/
   /*> yLOG_value  ("type"      , color_type);                                        <*/
   /*> printf("   - size     w=%3d, h=%3d, r=%3d, d=%3d, c=%3d, t=%3d\n", twidth, theight, rowbytes, bit_depth, channels, color_type);   <*/
   /*---(transform)----------------------*/
   png_set_palette_to_rgb (png_ptr);        /* makes rgb                      */
   /*> yLOG_info   ("palette"   , "good");                                            <*/
   png_set_tRNS_to_alpha  (png_ptr);        /* expands out alpha              */
   /*> yLOG_info   ("alpha"     , "good");                                            <*/
   /*> printf("   - palette and alpha are good\n");                                   <*/
   /*---(error stuff 2)------------------*/
   if (setjmp(png_jmpbuf(png_ptr))) {
      printf("   - FATAL, error during read image\n");
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      fclose(f);
      return 0;
   }
   /*> yLOG_info   ("jump"      , "set jump is good");                                <*/
   /*---(allocate image)-----------------*/
   png_byte *image_data = (uchar*) malloc(sizeof(png_byte) * rowbytes * theight);
   if (!image_data) {
      fclose(f);
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      printf("image_data error\n");
      return 0;
   }
   /*> yLOG_info   ("image"     , "data is good");                                    <*/
   /*> printf("   - image data is good\n");                                           <*/
   /*---(row pointers)-------------------*/
   png_bytep *row_pointers = (png_bytepp)  malloc(sizeof(png_bytep) * theight);
   if (row_pointers == NULL) {
      printf("row_pointers error\n");
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      free (image_data);
      fclose(f);
      return 0;
   }
   /*> yLOG_info   ("row_ptr"   , "row pointers are good");                           <*/
   /*> printf("   - row pointers are good\n");                                        <*/
   int i;
   for (i = 0; i < theight; ++i) {
      row_pointers[theight - 1 - i] = image_data + (i * rowbytes);
   }
   /*---(read)---------------------------*/
   /*> printf("   - pre read_image\n");                                               <*/
   png_read_image(png_ptr, row_pointers);
   /*> yLOG_info   ("read"      , "image is good");                                   <*/
   /*> printf("   - read_image was good\n");                                          <*/
   /*> uchar *bytey;                                                                  <* 
    *> for (i = 0; i < rowbytes; ++i) {                                               <* 
    *>    bytey = image_data + (50 * rowbytes) + i;                                   <* 
    *>    *bytey = 0  ;                                                               <* 
    *> }                                                                              <*/
   /*---(make texture)-------------------*/
   GLuint texture;
   glGenTextures   (1, &texture);
   glBindTexture   (GL_TEXTURE_2D, texture);
   glTexParameteri (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,     GL_REPEAT );
   glTexParameterf (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,     GL_REPEAT );
   glTexParameteri (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP,    GL_TRUE);
   glTexEnvi       (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_REPLACE);
   /*> glHint          (GL_TEXTURE_COMPRESSION_HINT,           GL_NICEST);            <*/
   /*> glTexImage2D    (GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);   <*/
   glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
   glBindTexture (GL_TEXTURE_2D, 0);
   /*---(clean)--------------------------*/
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   free(image_data);
   free(row_pointers);
   fclose(f);
   /*---(complete)-----------------------*/
   syms  = texture;
   /*> yLOG_exit   (__FUNCTION__);                                                    <*/
   return texture;
}

int  
yFONT_symbol       (float a_scale, int  a_row, int a_col, int a_mod)
{
   if (syms == 0)  return -1;
   float    x_row [25] = {
      0.9354, 0.8740, 0.8115, 0.7500,
      0.6875, 0.6265, 0.5640, 0.5030,
      0.4410, 0.3790, 0.3180, 0.2555,
      0.1935, 0.1322, 0.0700, 0.0080,
   };
   float    x_col [25] = {
      0.0032, 0.0508, 0.0982,
      0.1450, 0.1920, 0.2390,
      0.2870, 0.3340, 0.3810,
      0.4280, 0.4760, 0.5225,
      0.5702, 0.6170, 0.6645,
      0.7110, 0.7585, 0.8060,
      0.8530, 0.9000, 0.9475,
   };
   float    x_width    = 0.0460 - 0.0040;
   float    x_height   = 0.0547 - 0.0040;
   float    x          = x_col [a_col] + 0.0020;
   float    y          = x_row [a_row] + 0.0020;
   /*> printf ("r=%8.3f, c=%8.3f\n", y, x);                                           <*/
   /*---(draw text)-----------------------------*/
   glPushMatrix(); {
      glScalef(a_scale, a_scale, a_scale);
      switch (a_mod) {
      case  1  : glColor4f (1.0f, 0.0f, 0.0f, 0.2f); break;
      case  2  : glColor4f (0.7f, 0.3f, 0.0f, 0.2f); break;
      case  3  : glColor4f (0.3f, 0.7f, 0.0f, 0.2f); break;
      case  4  : glColor4f (0.0f, 0.1f, 0.0f, 0.2f); break;
      case  5  : glColor4f (0.0f, 0.7f, 0.3f, 0.2f); break;
      case  6  : glColor4f (0.0f, 0.3f, 0.7f, 0.2f); break;
      case  7  : glColor4f (0.0f, 0.0f, 1.0f, 0.2f); break;
      case  8  : glColor4f (0.3f, 0.0f, 0.7f, 0.2f); break;
      case  9  : glColor4f (0.7f, 0.0f, 0.3f, 0.2f); break;
      default  : glColor4f (0.5f, 0.5f, 0.5f, 0.2f); break;
      }
      glBegin  (GL_POLYGON); {
         glVertex3f   (    0.0, -20.0,   0.0);
         glVertex3f   (   20.0, -20.0,   0.0);
         glVertex3f   (   20.0,   0.0,   0.0);
         glVertex3f   (    0.0,   0.0,   0.0);
      } glEnd();
      /*> glColor4f (0.0f, 0.0f, 0.0f, 0.2f);                                         <* 
       *> glLineWidth (1.5);                                                          <* 
       *> glBegin  (GL_LINE_STRIP); {                                                 <* 
       *>    glVertex3f   (   -1.0, -21.0,   5.0);                                    <* 
       *>    glVertex3f   (   21.0, -21.0,   5.0);                                    <* 
       *>    glVertex3f   (   21.0,   1.0,   5.0);                                    <* 
       *>    glVertex3f   (   -1.0,   1.0,   5.0);                                    <* 
       *>    glVertex3f   (   -1.0, -21.0,   5.0);                                    <* 
       *> } glEnd();                                                                  <*/
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
   tFONT      *x_font      = NULL;              /* new font                       */
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "yFONT_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if        (strncmp(a_question, "font_name"   , 20)  == 0) {
      x_font = g_font [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT font name  : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT font name  : %2d:%-20.20s     point=%2d", strllen (x_font->name, LEN_LABEL), x_font->name, x_font->point);
   } else if (strncmp(a_question, "tex_size"    , 20)  == 0) {
      x_font = g_font [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT tex size   : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT tex size   : width =%4d, height=%4d" , x_font->tex_w, x_font->tex_h);
   } else if (strncmp(a_question, "char_limits" , 20)  == 0) {
      x_font = g_font [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT char limit : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT char limit : mascent =%3d, mdescent=%3d, margin  =%3d" , x_font->max_ascent, x_font->max_descent, x_font->margin);
   } else if (strncmp(a_question, "num_glyph"   , 20)  == 0) {
      x_font = g_font [a_num];
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
