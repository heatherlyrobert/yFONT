/*============================----beg-of-source---============================*/

#include    "yFONT.h"
#include    "yFONT_priv.h"

static char  s_fontname [LEN_LABEL] = "comic";
static char  s_font;
char        s_type       [LEN_LABEL];   /* type of source (font vs grid)       */
char        s_point;                    /* point size of font                  */
char        s_adjust;                   /* actual point size (for adjustment)  */
int         s_spacer;                   /* unicode of space character          */
char        s_glist      [LEN_LABEL];   /* type of source (font vs grid)       */
char        s_text       [LEN_STR]   = "the quick brown fox jumped over the lazy dog.  THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.  abcdefghijklmnopqrstuvwxyz.  ABCDEFGHIJKLMNOPQRSTUVWXYZ.  ";
/*---(sizes)-----------------------------*/
int       win_w = 1600;                     /* window width                   */
int       win_h =  170;                     /* window height                  */
int       tex_w = 1600;                     /* texture width                  */
int       tex_h =  170;                     /* texture height                 */

char        s_pair      = 0;

#define    MAX_COLORS     10 
#define    MAX_PAIRS      400

struct cCOLORS {
   char     abbr;
   float    red;
   float    grn;
   float    blu;
} s_colors [MAX_COLORS] = {
   { 'w', 1.00, 1.00, 1.00 },
   { 'l', 0.66, 0.66, 0.66 },
   { 'd', 0.33, 0.33, 0.33 },
   { 'k', 0.00, 0.00, 0.00 },
   { 'r', 0.80, 0.00, 0.00 },
   { 'y', 0.60, 0.60, 0.00 },
   { 'g', 0.00, 0.80, 0.00 },
   { 'c', 0.00, 0.60, 0.60 },
   { 'b', 0.00, 0.00, 0.80 },
   { 'm', 0.60, 0.00, 0.60 },
};

struct cPAIRS {
   char     back;
   char     fore;
} s_pairs [MAX_PAIRS] = {
   { 'k', 'w' },   
   { 'k', 'l' },   
   { 'w', 'k' },   
   { 'w', 'd' },   
   /*-white----*/
   /*> { 'w', 'w' },                                                                  <*/
   { 'w', 'l' },
   { 'w', 'd' },
   { 'w', 'k' },
   { 'w', 'r' },
   { 'w', 'y' },
   { 'w', 'g' },
   { 'w', 'c' },
   { 'w', 'b' },
   { 'w', 'm' },
   /*--light---*/
   { 'l', 'w' },
   /*> { 'l', 'l' },                                                                  <*/
   { 'l', 'd' },
   { 'l', 'k' },
   { 'l', 'r' },
   { 'l', 'y' },
   { 'l', 'g' },
   { 'l', 'c' },
   { 'l', 'b' },
   { 'l', 'm' },
   /*--dark----*/
   { 'd', 'w' },
   { 'd', 'l' },
   /*> { 'd', 'd' },                                                                  <*/
   { 'd', 'k' },
   { 'd', 'r' },
   { 'd', 'y' },
   { 'd', 'g' },
   { 'd', 'c' },
   { 'd', 'b' },
   { 'd', 'm' },
   /*-black----*/
   { 'k', 'w' },
   { 'k', 'l' },
   { 'k', 'd' },
   /*> { 'd', 'k' },                                                                  <*/
   { 'k', 'r' },
   { 'k', 'y' },
   { 'k', 'g' },
   { 'k', 'c' },
   { 'k', 'b' },
   { 'k', 'm' },
   /*--red-----*/
   { 'r', 'w' },
   { 'r', 'l' },
   { 'r', 'd' },
   { 'r', 'k' },
   /*> { 'r', 'r' },                                                                  <*/
   { 'r', 'y' },
   { 'r', 'g' },
   { 'r', 'c' },
   { 'r', 'b' },
   { 'r', 'm' },
   /*--yellow--*/
   { 'y', 'w' },
   { 'y', 'l' },
   { 'y', 'd' },
   { 'y', 'k' },
   { 'y', 'r' },
   /*> { 'y', 'y' },                                                                  <*/
   { 'y', 'g' },
   { 'y', 'c' },
   { 'y', 'b' },
   { 'y', 'm' },
   /*--green---*/
   { 'g', 'w' },
   { 'g', 'l' },
   { 'g', 'd' },
   { 'g', 'k' },
   { 'g', 'r' },
   { 'g', 'y' },
   /*> { 'g', 'g' },                                                                  <*/
   { 'g', 'c' },
   { 'g', 'b' },
   { 'g', 'm' },
   /*--cyan----*/
   { 'c', 'w' },
   { 'c', 'l' },
   { 'c', 'd' },
   { 'c', 'k' },
   { 'c', 'r' },
   { 'c', 'y' },
   { 'c', 'g' },
   /*> { 'c', 'c' },                                                                  <*/
   { 'c', 'b' },
   { 'c', 'm' },
   /*--blue----*/
   { 'b', 'w' },
   { 'b', 'l' },
   { 'b', 'd' },
   { 'b', 'k' },
   { 'b', 'r' },
   { 'b', 'y' },
   { 'b', 'g' },
   { 'b', 'c' },
   /*> { 'b', 'b' },                                                                  <*/
   { 'b', 'm' },
   /*-magenta--*/
   { 'm', 'w' },
   { 'm', 'l' },
   { 'm', 'd' },
   { 'm', 'k' },
   { 'm', 'r' },
   { 'm', 'y' },
   { 'm', 'g' },
   { 'm', 'c' },
   { 'm', 'b' },
   /*> { 'm', 'm' },                                                                  <*/
   /*----------*/
   { '-', '-' },   
};

char
color_pairs          (char a_dir)
{
   char      x_fore      = -1;
   char      x_back      = -1;
   int       i           = -1;
   /*> printf ("color_pairs (%c)\n", a_dir);                                          <*/
   if (a_dir == '0')                  s_pair = 0;
   if (a_dir == '+')                  ++s_pair;
   if (s_pair >= MAX_PAIRS)           s_pair = MAX_PAIRS - 1;
   if (s_pairs [s_pair].back == '-')  --s_pair;;
   if (a_dir == '-')                  --s_pair;
   if (s_pair < 0)                    s_pair = 0;
   for (i = 0; i < MAX_COLORS; ++i) {
      if (s_colors [i].abbr != s_pairs [s_pair].fore)  continue;
      x_fore = i;
   }
   if (x_fore < 0)  return -1;
   /*> printf ("fore color = %c @ %d\n", s_pairs [s_pair].fore, x_fore);              <*/
   for (i = 0; i < MAX_COLORS; ++i) {
      if (s_colors [i].abbr != s_pairs [s_pair].back)  continue;
      x_back = i;
   }
   if (x_back < 0)  return -1;
   /*> printf ("back color = %c @ %d\n", s_pairs [s_pair].back, x_back);              <*/
   glColor4f    ( s_colors [x_fore].red, s_colors [x_fore].grn, s_colors [x_fore].blu, 1.00);
   glClearColor ( s_colors [x_back].red, s_colors [x_back].grn, s_colors [x_back].blu, 1.00);
   return 0;
}



/*============================--------------------============================*/
/*===----                             draw                             ----===*/
/*============================--------------------============================*/
static void      o___OPENGL__________________o (void) {;}

/*---(opengl objects)--------------------*/
uint      tex       =     0;                /* texture for image              */
uint      fbo       =     0;                /* framebuffer                    */
uint      depth     =     0;                /* depth buffer                   */

char       /* PURPOSE : place the texture in the window ----------------------*/
draw_texture       (void)
{
   DEBUG_GRAF  printf("draw_texture () beg\n");
   /*---(locals)-------------------------*/
   float     offset, ratio;
   float     tx1, ty1, tx2, ty2;
   float     wx1, wy1, wx2, wy2;
   float     z;
   int       i;
   /*---(horizontal views)---------------*/
   /*> offset    = (my.ccol * 320.0) / tex_w;                                         <*/
   offset    = 0;
   /*> if (my.format == 't') offset    = (my.ccol * 300.0) / tex_w;                   <* 
    *> if (my.format == 'b') offset    = (my.ccol * 325.0) / tex_w;                   <*/
   /*> offset += my.move;                                                             <*/
   ratio   = (float) win_w / tex_w;
   ty1     = 0.0;
   ty2     = 1.0;
   tx1     = 0.0 + offset;
   tx2     = 0.0 + offset + ratio;
   /*---(window coordinates)-------------*/
   wx1     = 0.0;
   wx2     = win_w;
   wy1     = 0.0;
   wy2     = -win_h;
   z       = -10.0;
   /*> printf("win_h = %d, tex_h = %d, ratio = %f, win_w = %d, tex_w = %d\n", win_h, tex_h, ratio, win_w, tex_w);   <*/
   /*> printf("wy1 = %f, wy2 = %f, ty1 = %f, ty2 = %f\n", wy1, wy2, ty1, ty2);        <*/
   /*---(prepare drawing)----------------*/
   glClear       (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   /*---(draw it)------------------------*/
   glBindTexture   (GL_TEXTURE_2D, tex);
   glBegin(GL_POLYGON); {
      glTexCoord2f ( tx1,  ty2);
      glVertex3f   ( wx1,  wy1,    z);
      glTexCoord2f ( tx2,  ty2);
      glVertex3f   ( wx2,  wy1,    z);
      glTexCoord2f ( tx2,  ty1);
      glVertex3f   ( wx2,  wy2,    z);
      glTexCoord2f ( tx1,  ty1);
      glVertex3f   ( wx1,  wy2,    z);
   } glEnd();
   glBindTexture   (GL_TEXTURE_2D, 0);
   /*---(force the redraw)---------------*/
   glXSwapBuffers(DISP, BASE);
   glFlush();
   DEBUG_GRAF  printf("draw_texture () end\n");
   /*---(complete)-------------------------*/
   return 0;
}

char               /* PURPOSE : create a texture -----------------------------*/
texture_create     (void)
{
   DEBUG_GRAF  printf("texture_create () beg\n");
   /*---(generate)-----------------------*/
   glGenFramebuffersEXT         (1, &fbo);
   glGenTextures                (1, &tex);
   glGenRenderbuffersEXT        (1, &depth);
   /*---(bind)---------------------------*/
   DEBUG_GRAF  printf("texture_create () bind\n");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  fbo);
   glBindTexture                (GL_TEXTURE_2D,       tex);
   /*---(settings)-----------------------*/
   DEBUG_GRAF  printf("texture_create () settings\n");
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri              (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvi                    (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri              (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP, GL_TRUE);
   /*---(copy)---------------------------*/
   DEBUG_GRAF  printf("texture_create () allocate\n");
   glTexImage2D                 (GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   DEBUG_GRAF  printf("texture_create () copy\n");
   glFramebufferTexture2DEXT    (GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex, 0);
   /*---(depth)--------------------------*/
   DEBUG_GRAF  printf("texture_create () depth\n");
   glBindRenderbufferEXT        (GL_RENDERBUFFER_EXT, depth);
   glRenderbufferStorageEXT     (GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, tex_w, tex_h);
   glFramebufferRenderbufferEXT (GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth);
   /*---(unbind)-------------------------*/
   DEBUG_GRAF  printf("texture_create () unbind\n");
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   /*---(complete)-----------------------*/
   DEBUG_GRAF  printf("texture_create () end\n");
   return 0;
}

char               /* PURPOSE : delete a framebuffer -------------------------*/
texture_free       (void)
{
   DEBUG_GRAF  printf("texture_free () beg\n");
   /*---(generate)-----------------------*/
   glDeleteTextures                (1, &tex);
   glDeleteRenderbuffersEXT        (1, &depth);
   glDeleteFramebuffersEXT         (1, &fbo);
   /*---(complete)-----------------------*/
   DEBUG_GRAF  printf("texture_free () end\n");
   return 0;
}

char
draw_init(void)
{
   glShadeModel(GL_SMOOTH);
   /*> glClearColor(0.0f, 0.0f, 0.0f, 1.0f);                                          <*/
   /*> color_pairs ('0');                                                             <*/
   glClearDepth(1.0f);
   glEnable(GL_TEXTURE_2D);    /* NEW */
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_ALPHA_TEST);
   glEnable(GL_BLEND);
   glAlphaFunc(GL_GEQUAL, 0.0125);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   /*---(special polygon antialiasing)----------*/
   /*> glEnable(GL_POLYGON_SMOOTH);                                                   <*/
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   glFlush();
   return 0;
}
char
draw_font          (char a_font, char *a_fontname)
{
   /*---(locals)-------------------------*/
   int       i;                             /* loop iterator -- word          */
   int       w       = 0;
   char      x_text  [LEN_STR];
   glPushMatrix(); {
      glTranslatef( 20.0, -75.0,   0.0);
      glRotatef  ( 90.0, 0.0f, 0.0f, 1.0f);
      yFONT_print (a_font, 16, YF_BASCEN, a_fontname);
   } glPopMatrix();
   glPushMatrix(); {
      glTranslatef( 40.0, -45.0,   0.0);
      glPushMatrix(); {
         w = yFONT_print (a_font, 40, YF_BASLEF, "40");
         glTranslatef( w + 30.0,   0.0,   0.0);
         for (i = 0; i < 10; ++i) {
            w = yFONT_print (a_font, 40, YF_BASLEF, s_text);
            if (w >= tex_w) break;
            glTranslatef( w, 0.0,   0.0);
         }
      } glPopMatrix();
      glTranslatef(  0.0, -35.0,   0.0);
      glPushMatrix(); {
         w = yFONT_print (a_font, 24, YF_BASLEF, "24");
         glTranslatef( w + 30.0,   0.0,   0.0);
         for (i = 0; i < 10; ++i) {
            w = yFONT_print (a_font, 24, YF_BASLEF, s_text);
            if (w >= tex_w) break;
            glTranslatef( w, 0.0,   0.0);
         }
      } glPopMatrix();
      glTranslatef(  0.0, -20.0,   0.0);
      glPushMatrix(); {
         w = yFONT_print (a_font, 12, YF_BASLEF, "12");
         glTranslatef( w + 30.0,   0.0,   0.0);
         for (i = 0; i < 10; ++i) {
            w = yFONT_print (a_font, 12, YF_BASLEF, s_text);
            if (w >= tex_w) break;
            glTranslatef( w, 0.0,   0.0);
         }
      } glPopMatrix();
      glTranslatef(  0.0, -15.0,   0.0);
      glPushMatrix(); {
         w = yFONT_print (a_font, 10, YF_BASLEF, "10");
         glTranslatef( w + 30.0,   0.0,   0.0);
         for (i = 0; i < 10; ++i) {
            w = yFONT_print (a_font, 10, YF_BASLEF, s_text);
            if (w >= tex_w) break;
            glTranslatef( w, 0.0,   0.0);
         }
      } glPopMatrix();
      glTranslatef(  0.0, -12.0,   0.0);
      glPushMatrix(); {
         w = yFONT_print (a_font,  8, YF_BASLEF, "8");
         glTranslatef( w + 30.0,   0.0,   0.0);
         for (i = 0; i < 10; ++i) {
            w = yFONT_print (a_font,  8, YF_BASLEF, s_text);
            if (w >= tex_w) break;
            glTranslatef( w, 0.0,   0.0);
         }
      } glPopMatrix();
      glTranslatef(  0.0, -10.0,   0.0);
      glPushMatrix(); {
         w = yFONT_print (a_font,  7, YF_BASLEF, "7");
         glTranslatef( w + 30.0,   0.0,   0.0);
         for (i = 0; i < 10; ++i) {
            w = yFONT_print (a_font,  7, YF_BASLEF, s_text);
            if (w >= tex_w) break;
            glTranslatef( w, 0.0,   0.0);
         }
      } glPopMatrix();
      glTranslatef(  0.0, -10.0,   0.0);
      glPushMatrix(); {
         w = yFONT_print (a_font,  6, YF_BASLEF, "6");
         glTranslatef( w + 30.0,   0.0,   0.0);
         for (i = 0; i < 10; ++i) {
            w = yFONT_print (a_font,  6, YF_BASLEF, s_text);
            if (w >= tex_w) break;
            glTranslatef( w, 0.0,   0.0);
         }
      } glPopMatrix();
   } glPopMatrix();
   return 0;
}

char
draw_main          (void)
{
   DEBUG_GRAF  printf("draw_main () beg\n");
   /*---(locals)-------------------------*/
   int       i;                             /* loop iterator -- word          */
   int       j;                             /* loop iterator -- word          */
   int       rc      = 0;                   /* simple return code             */
   int       w       = 0;
   int       pos     = 0;
   /*---(create objects)-----------------*/
   /*> printf("   entered\n");                                                        <*/
   texture_create();
   /*> printf("   texture create done\n");                                            <*/
   /*---(setup)--------------------------*/
   glViewport(0,  0, tex_w, tex_h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(        0,   tex_w, -tex_h,  0, -500.0,  500.0);
   glMatrixMode(GL_MODELVIEW);
   glBindTexture   (GL_TEXTURE_2D, 0);
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT,  fbo);
   /*> printf("   bound texture\n");                                                  <*/
   /*---(draw)---------------------------*/
   color_pairs  ('.');
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*---(font)---------------------------*/
   /*> glColor4f (1.0f, 1.0f, 1.0f, 1.0f);                                            <*/
   glTranslatef(  0.0, -10.0,   0.0);
   draw_font (s_font, s_fontname);
   /*---(mipmaps)------------------------*/
   glBindFramebufferEXT         (GL_FRAMEBUFFER_EXT, 0);
   glBindTexture   (GL_TEXTURE_2D, tex);
   glGenerateMipmapEXT(GL_TEXTURE_2D);
   glBindTexture   (GL_TEXTURE_2D, 0);
   /*> printf("   draw complete\n");                                                  <*/
   /*---(complete)-------------------------*/
   DEBUG_GRAF  printf("draw_main () end\n");
   return 0;
}

char
font_load            (void)
{
   s_font  = yFONT_load (s_fontname);
   if (s_font <  0) {
      fprintf (stderr, "Problem loading %s\n", s_fontname);
      exit    (1);
   }
   return 0;
}

char
font_delete          (void)
{
   yFONT_free (s_font);
   return 0;
}


char        /* PURPOSE : process the xwindows event stream                    */
prog_event    (void)
{
   /*---(locals)--------------------------------*/
   XKeyEvent *key_event;
   char       the_key[5];
   int        the_bytes;
   char       x_search[100];
   int        loop = 0;
   /*---(event loop)----------------------------*/
   while (1) {
      XNextEvent(DISP, &EVNT);
      /*> printf ("loop %d\n", loop++);                                               <*/
      /*---(start)----------------------------*/
      switch(EVNT.type) {
      case FocusIn:
         break;
      case FocusOut:
         break;
      case Expose:
         break;
      case ConfigureNotify:
         break;
      case KeyPress:
         /*> printf ("key pressed\n");                                                <*/
         key_event  = (XKeyEvent *) &EVNT;
         the_bytes = XLookupString((XKeyEvent *) &EVNT, the_key, 5, NULL, NULL);
         if (the_bytes < 1) break;
         /*> printf ("the key %d\n", the_key [0]);                                    <*/
         /*---(mode changes)-------*/
         switch (the_key[0]) {
         case 'Q' :
            return 1;
            break;
         case 'j' :
            yFONT__conf_next   (s_fontname, s_type, &s_point, &s_adjust, &s_spacer, s_glist, NULL);
            return 0;
            break;
         case 'k' :
            yFONT__conf_prev   (s_fontname, s_type, &s_point, &s_adjust, &s_spacer, s_glist, NULL);
            return 0;
            break;
         case '+' :
            color_pairs ('+');
            return 0;
            break;
         case '-' :
            color_pairs ('-');
            return 0;
            break;
         case '0' :
            color_pairs ('0');
            return 0;
            break;
         }
         break;

      default:
         break;
      }
      draw_texture();
      /*> sleep( 1);                                                                  <*/
   }
   /*---(complete)------------------------------*/
   return 0;
}


int
main               (int argc, char *argv[])
{
   /*---(locals)-----------+-----------+-*/
   int         rc          = 0;
   /*---(read config)--------------------*/
   strncpy (s_fontname, ""      , 90);
   if (rc >= 0)  rc = yFONT__conf_parse  ();
   if (argc >= 2) {
      if (strcmp ("--conf", argv [1]) == 0) {
         yFONT__conf_list ();
         return 0;
      } else {
         strncpy (s_fontname, argv [1], 90);
      }
   }
   /*---(preparation)--------------------*/
   yXINIT_start ("yFONT_tick", win_w, win_h, YX_FOCUSABLE, YX_FIXED, YX_SILENT);
   yFONT__conf_parse  ();
   if (strcmp (s_fontname, "") != 0) rc = yFONT__conf_info   (s_fontname, s_type, &s_point, &s_adjust, &s_spacer, s_glist, NULL);
   else                              rc = yFONT__conf_head   (s_fontname, s_type, &s_point, &s_adjust, &s_spacer, s_glist, NULL);
   if (rc < 0)                       rc = yFONT__conf_head   (s_fontname, s_type, &s_point, &s_adjust, &s_spacer, s_glist, NULL);
   if (strcmp (s_fontname, "") != 0) rc = yFONT__conf_head   (s_fontname, s_type, &s_point, &s_adjust, &s_spacer, s_glist, NULL);
   if (strcmp ("norm", s_glist) == 0)  strlcpy (s_text, "the quick brown fox jumped over the lazy dog.  THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.  abcdefghijklmnopqrstuvwxyz.  ABCDEFGHIJKLMNOPQRSTUVWXYZ.  ?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~.  ", LEN_STR);
   if (strcmp ("tsae", s_glist) == 0)  strlcpy (s_text, "siyowina qaze nihi gaya geyv zuda siyv gohv wenv saqu goya sida tanu wagv kuwa guqe nasu zuqa naqu disv  ", LEN_STR);
   if (strcmp ("mand", s_glist) == 0)  strlcpy (s_text, "siyo'wina.  DGKNQTcdghklmnpqstwxyzaefiouvrj0123456789!@#$%^&,:\"\'()-.  ", LEN_STR);
   if (strcmp ("test", s_glist) == 0)  strlcpy (s_text, "abcd   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   u   v   w   x   y   z   ", LEN_STR);
   font_load          ();
   color_pairs        ('0');
   draw_init          ();
   draw_main          ();
   draw_texture       ();
   /*---(main loop)----------------------*/
   while (rc == 0) {
      rc = prog_event ();
      /*> printf ("switching to %s, %s\n", s_fontname, s_glist);                      <*/
      if (strcmp ("norm", s_glist) == 0)  strlcpy (s_text, "the quick brown fox jumped over the lazy dog.  THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.  abcdefghijklmnopqrstuvwxyz.  ABCDEFGHIJKLMNOPQRSTUVWXYZ.  ?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~.  ", LEN_STR);
      if (strcmp ("tsae", s_glist) == 0)  strlcpy (s_text, "siyo wina qaze nihi gaya geyv zuda siyv gohv wenv saqu goya sida tanu wagv kuwa guqe nasu zuqa naqu disv  ", LEN_STR);
      if (strcmp ("mand", s_glist) == 0)  strlcpy (s_text, "siyo'wina.  DGKNQTcdghklmnpqstwxyzaefiouvrj0123456789!@#$%^&,:\"\'()-.  ", LEN_STR);
      if (strcmp ("test", s_glist) == 0)  strlcpy (s_text, "   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   p   q   r   s   t   u   v   w   x   y   z   ", LEN_STR);
      /*> printf ("test string is [%s]\n", s_text);                                   <*/
      texture_free ();
      font_delete  ();
      font_load    ();
      color_pairs  ('.');
      draw_init    ();
      draw_main    ();
   }
   /*---(wrap-up)------------------------*/
   font_delete;
   /*---(complete)-----------------------*/
   return rc;
}
