#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <math.h>

#include    "yFONT.h"
#include    "yFONT_priv.h"

tFONT       s_font;

char
SHOW__header       (char *a_name)
{
   /*---(locals)--------------------------------*/
   int       rc        = 0;                 /* generic return code            */
   char      rce       = -10;
   char      fileid[4];                     /* magic number of file           */
   int       type      = 0;                 /* data type                      */
   int       value     = 0;
   char      x_name [1000];
   char      x_type    [5];
   int       x_value   = 0;
   /*---(open file)-----------------------------*/
   printf ("\nopen font...\n");
   printf ("   short name = %s\n", a_name);
   snprintf (x_name, 900, "/usr/local/share/fonts/%s.txf", a_name);
   printf ("   long name  = %s\n", x_name);
   printf ("   openning file as read binary...\n");
   s_font.f = fopen (x_name, "rb");
   printf ("   file ptr   = %p\n", s_font.f);
   --rce;  if (s_font.f == NULL) {
      printf ("   FATAL, can not open txf font file\n");
      return rce;
   }
   /*---(check file type)-----------------------*/
   printf ("\n");
   printf ("validate header...\n");
   rc = fread (x_type, 1, 4, s_font.f);
   printf ("   magic len  = %d\n", rc);
   --rce;  if (rc != 4) {
      printf ("   FATAL, can not read magic type\n");
      return rce;
   }
   printf ("   magic id   = %d\n", (uchar) x_type [0]);
   x_type [4] = '\0';
   printf ("   magic type = %s\n", x_type + 1);
   rc = strcmp ("txf", x_type + 1);
   printf ("   strcmp     = %d\n", rc);
   --rce;  if (rc != 0) {
      printf ("   FATAL, wrong magic string\n");
      return rce;
   }
   printf ("   correct magic string\n");
   /*---(point size)----------------------------*/
   rc = fread (&x_name,   sizeof (char), 20, s_font.f);
   strcpy (s_font.name, x_name);
   printf ("   short name = %s\n", x_name);
   /*---(point size)----------------------------*/
   rc = fread (&x_value,   sizeof (int), 1, s_font.f);
   printf ("   point size = %d\n", x_value);
   --rce;  if (x_value < 6) {
      printf ("   FATAL, point size less than six (6)\n");
      return rce;
   }
   --rce;  if (x_value > 99) {
      printf ("   FATAL, point size greater than ninety-nine (99)\n");
      return rce;
   }
   s_font.p = x_value;
   printf ("   point size in range (6 to 99)\n");
   /*---(check metrics)-------------------------*/
   rc = fread (&x_value, sizeof(ulong), 1, s_font.f);
   printf ("   width      = %6d   %10.3f (x16)\n", x_value, x_value / 16.0);
   s_font.w = x_value;
   rc = fread (&x_value, sizeof(uint), 1, s_font.f);
   printf ("   hieght     = %6d   %10.3f (x16)\n", x_value, x_value / 16.0);
   s_font.h = x_value;
   printf ("   (bytes)    = %d\n", s_font.w * s_font.h);
   rc = fread (&x_value, sizeof(int), 1, s_font.f);
   printf ("   max ascent = %d\n", x_value);
   s_font.max_ascent = x_value;
   rc = fread (&x_value, sizeof(int), 1, s_font.f);
   printf ("   max descent= %d\n", x_value);
   s_font.max_descent = x_value;
   rc = fread (&x_value, sizeof(int), 1, s_font.f);
   printf ("   margin     = %d\n", x_value);
   s_font.margin      = x_value;
   rc = fread (&x_value, sizeof(int), 1, s_font.f);
   printf ("   num glyphs = %d\n", x_value);
   s_font.n_glyph = x_value;
   /*---(complete)------------------------------*/
   return 0;
}

char
SHOW__table        (void)
{
   /*---(locals)--------------------------------*/
   int       rc        =   0;               /* generic return code            */
   char      rce       = -10;               /* return code for errors         */
   int       i         =   0;
   int       c         =   0;
   /*---(begin)--------------------------*/
   printf ("\nglyph table...\n");
   /*---(allocate the glyph info)--------*/
   printf ("   num glyphs = %d\n", s_font.n_glyph);
   printf ("   allocating glyph space\n");
   s_font.a_glyphs = (tGLYPH *) malloc (s_font.n_glyph * sizeof (tGLYPH));
   printf ("   glyph ptr  = %p\n", s_font.a_glyphs);
   --rce;  if (s_font.a_glyphs == NULL) {
      printf ("   FATAL, can not allocate glyph table\n");
      return rce;
   }
   printf ("   successfully allocated glyph table\n");
   /*---(read glyphs one-by-one)---------*/
   --rce;
   printf ("   display all fields\n");
   /*> printf ("   count ucode tex_x tex_y width heigh x-off y-off advan\n");         <*/
   for (i = 0; i < s_font.n_glyph; ++i) {
      rc = fread (s_font.a_glyphs + i, sizeof (tGLYPH), 1, s_font.f);
      if (rc != 1) {
         printf ("   glyph rc   = %d\n", rc);
         printf ("   FATAL, could not read table entry\n");
         return rce;
      }
      if ((c %  5) == 0) printf ("   count ucode chr tex_x tex_y width heigh x-off y-off advan\n");
      ++c;
      printf ("   %5d %5d  %c  %5d %5d %5d %5d %5d %5d %5d\n",
            c                    , s_font.a_glyphs[i].c , 
            (s_font.a_glyphs[i].c < 128) ? s_font.a_glyphs[i].c : '-',
            s_font.a_glyphs[i].x , s_font.a_glyphs[i].y , 
            s_font.a_glyphs[i].w , s_font.a_glyphs[i].h , 
            s_font.a_glyphs[i].xo, s_font.a_glyphs[i].yo,
            s_font.a_glyphs[i].a );
   }
   printf ("   done printing\n");
   /*---(complete)-----------------------*/
   return 0;
}

char
SHOW__verts        (void)
{
   int       c         =   0;
   int       i         =   0;
   printf ("\nshow verticies...\n");
   yFONT__verts (&s_font);
   for (i = 0; i < s_font.n_glyph; ++i) {
      if ((c %  5) == 0) printf ("   count ucode chr abs_x abs_y width heigh side --rel_x --rel_y --tex_x --tex_y\n");
      ++c;
      printf ("   %5d %5d  %c  %5d %5d %5d %5d  TL  %7d %7d %7.5f %7.5f\n",
            c                    , s_font.a_glyphs[i].c , 
            (s_font.a_glyphs[i].c < 128) ? s_font.a_glyphs[i].c : '-',
            s_font.a_glyphs[i].x , s_font.a_glyphs[i].y ,
            s_font.a_glyphs[i].w , s_font.a_glyphs[i].h ,
            s_font.verts[i].TL.rel_x, s_font.verts[i].TL.rel_y,
            s_font.verts[i].TL.tex_x, s_font.verts[i].TL.tex_y);
      printf ("                                            TR  %7d %7d %7.5f %7.5f\n",
            s_font.verts[i].TR.rel_x, s_font.verts[i].TR.rel_y,
            s_font.verts[i].TR.tex_x, s_font.verts[i].TR.tex_y);
      printf ("                                            BR  %7d %7d %7.5f %7.5f\n",
            s_font.verts[i].BR.rel_x, s_font.verts[i].BR.rel_y,
            s_font.verts[i].BR.tex_x, s_font.verts[i].BR.tex_y);
      printf ("                                            BL  %7d %7d %7.5f %7.5f\n",
            s_font.verts[i].BL.rel_x, s_font.verts[i].BL.rel_y,
            s_font.verts[i].BL.tex_x, s_font.verts[i].BL.tex_y);
   }
   /*---(complete)-----------------------*/
   return 0;
}

char
SHOW__index        (void)
{
   int       c         =   0;
   int       i         =   0;
   printf ("\nshow index...\n");
   yFONT__index (&s_font);
   for (i = 0; i < s_font.range; ++i) {
      if (s_font.lookup [i] < 0) continue;
      printf ("   %5d %5d %5d\n", i, i + s_font.min_glyph, s_font.lookup [i]);
      ++c;
   }
   printf ("   done\n");
   printf ("   %d entries\n", c);
   if (c == s_font.n_glyph)   printf ("   SUCCESS, matches n_glyph\n");
   else                       printf ("   FAILED, does not match n_glyph\n");
   return 0;
}

char
SHOW__texture      (void)
{
   int         rc        = 0;                 /* generic return code            */
   s_font.teximage = (uchar *) malloc (s_font.w * s_font.h * sizeof (uchar));
   if (s_font.teximage == NULL) {
      printf ("yFONT_load() : could not allocate glyph\n");
      return YF_MEM_FULL;
   }
   rc = fread (s_font.teximage, 1, s_font.w * s_font.h, s_font.f);
   if (rc != s_font.w * s_font.h) {
      printf("yFONT_load() : premature end of file (byte)\n");
      return YF_MEM_FULL;
   }
   return 0;
}

char         /*--: show ascii version -----------s--------[ ------ [ ------ ]-*/
SHOW__asciiart     (void)
{
   int   i = 0;
   int   j = 0;
   int   k = 0;
   int   x_ascii;
   char  x_chars [20] = " -+:*172%98#@";   /* use symmetrical charaters    */
   uchar x_value;
   tGLYPH     *x_glyph     = NULL;
   int         x_pos       = 0;
   int         y_pos       = 0;
   printf("\nshow the whole image in arcii...\n");
   for (k = 0; k < (s_font.w / 100) + 1; ++k) {
      printf ("---(%3d)---------------------------------------\n", k);
      for (i = 0; i < s_font.h; ++i) {
         printf ("%02d : ", i);
         for (j = k * 100; j < (k + 1) * 100; ++j) {
            if (j > s_font.w) break;
            x_value = s_font.teximage [(i * s_font.w) + j];
            x_ascii = x_value / (256 / 12);
            printf ("%c", x_chars [x_ascii]);
         }
         printf ("\n");
      }
   }
   printf ("\n");



   printf("\nshow the characters in arcii...\n");
   for (k = 0; k < s_font.n_glyph; ++k) {
      x_glyph  = &(s_font.a_glyphs [k]);
      printf ("   count %5d, unicode %5d, x_pos %5d, y_pos %5d, height %5d, width %5d\n",
            k, x_glyph->c, x_glyph->x, x_glyph->y, x_glyph->h, x_glyph->w);
      /*> for (i = 0; i < x_glyph->h; ++i) {                                                    <* 
       *>    y_pos   = x_glyph->y + i;                                                          <* 
       *>    printf ("   %03d : ", y_pos);                                                      <* 
       *>    for (j = 0; j < x_glyph->w; ++j) {                                                 <* 
       *>       x_pos   = x_glyph->x + j;                                                       <* 
       *>       printf ("%04d ", x_pos);                                                        <* 
       *>       x_value = s_font.teximage [(y_pos * s_font.w) + x_pos];                         <* 
       *>       x_ascii = x_value / (256 / 12);                                                 <* 
       *>       /+> if (i % 2) printf("%c", x_chars[x_ascii]);                            <+/   <* 
       *>    }                                                                                  <* 
       *>    printf (" |\n");                                                                   <* 
       *> }                                                                                     <*/
      /*> if (i % 2) printf("\n");                                                 <*/
      for (i = 0; i < x_glyph->h; ++i) {
         y_pos   = x_glyph->y + i;
         printf ("   %02d : ", i);
         for (j = 0; j < x_glyph->w; ++j) {
            x_pos   = x_glyph->x + j;
            x_value = s_font.teximage [(y_pos * s_font.w) + x_pos];
            x_ascii = x_value / (256 / 12);
            /*> if (i % 2) printf("%c", x_chars[x_ascii]);                            <*/
            printf ("%c", x_chars [x_ascii]);
         }
         printf (" |\n");
         /*> if (i % 2) printf("\n");                                                 <*/
      }
   }
   printf("\n");
   return 0;
}

int
main               (int argc, char *argv[])
{
   int         rc        = 0;                 /* generic return code            */
   char        x_font      [100]    = "arial";
   if (argc == 2) {
      strncpy (x_font, argv [1], 90);
   }
   if (rc == 0)  rc = SHOW__header   (x_font);
   if (rc == 0)  rc = SHOW__table    ();
   if (rc == 0)  rc = SHOW__verts    ();
   if (rc == 0)  rc = SHOW__index    ();
   if (rc == 0)  rc = SHOW__texture  ();
   if (rc == 0)  rc = SHOW__asciiart ();
   return 0;
}
