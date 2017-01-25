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
yFONT_load         (char *a_name)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   int         rc          = 0;                 /* generic return code            */
   int         x_slot      = -1;
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
   int       i         = 0;                 /* iterator -- character          */
   tFONT    *x_font    = g_font [a_slot];
   int       x_len     = strlen(a_text);     /* string length                  */
   float     x_scale   = (float) a_size / (float) x_font->point;
   int       w         = 0;
   float     x         = 0;
   float     y         = 0;
   /*---(defense)------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   if (x_font == NULL)                     return YF_BAD_SLOT;
   /*---(place start)--------------------*/
   w = yFONT__index_width  (x_font, a_text, x_len);
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
   return w * x_scale;
}

int                /* PURPOSE : print a string with word wrapping             */
yFONT_printw         (char a_slot, char a_point, char a_align, char *a_text, int a_width, int a_height, float a_spacing)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tFONT  *x_font = g_font [a_slot];
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
   tFONT  *x_font = g_font [a_slot];
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

/*> int                /+ PURPOSE : make a png image into a texture --------------+/                                                               <* 
 *> yFONT_symload      (char *a_filename)                                                                                                          <* 
 *> {                                                                                                                                              <* 
 *>    /+> yLOG_enter  (__FUNCTION__);                                                    <+/                                                      <* 
 *>    /+---(locals)-----------+-----------+-+/                                                                                                    <* 
 *>    png_byte    header      [8];             /+ png header                     +/                                                               <* 
 *>    png_structp png_ptr;                                                                                                                        <* 
 *>    png_infop   info_ptr, end_info;                                                                                                             <* 
 *>    int         interlace_type;                                                                                                                 <* 
 *>    int         rc          = 0;             /+ simple return code             +/                                                               <* 
 *>    int         len         = 0;                                                                                                                <* 
 *>    FILE       *f           = NULL;          /+ file pointer                   +/                                                               <* 
 *>    ulong       twidth, theight;                                                                                                                <* 
 *>    int         width, height;                                                                                                                  <* 
 *>    int         bit_depth, color_type;                                                                                                          <* 
 *>    /+---(defense)------------------------+/                                                                                                    <* 
 *>    syms = 0;                                                                                                                                   <* 
 *>    if (a_filename == NULL) {                                                                                                                   <* 
 *>       printf("   - FATAL, filename is null\n");                                                                                                <* 
 *>       return  -1;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    len = strlen (a_filename);                                                                                                                  <* 
 *>    if (len < 5) {                                                                                                                              <* 
 *>       printf("   - FATAL, file name too short (%d < 5)\n", len);                                                                               <* 
 *>       return  -2;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    /+---(open png file)------------------+/                                                                                                    <* 
 *>    /+> yLOG_info   ("file"      , a_filename);                                        <+/                                                      <* 
 *>    /+> printf ("\npng_load (%s)\n", a_filename);                                      <+/                                                      <* 
 *>    f = fopen(a_filename, "rb");                                                                                                                <* 
 *>    if (f == NULL) {                                                                                                                            <* 
 *>       printf("   - FATAL, can not find file\n");                                                                                               <* 
 *>       return -3;                                                                                                                               <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("status"    , "file found");                                      <+/                                                      <* 
 *>    /+> printf("   - file found\n");                                                   <+/                                                      <* 
 *>    /+---(check the header)---------------+/                                                                                                    <* 
 *>    fread (header, 1, 8, f);                                                                                                                    <* 
 *>    rc = png_sig_cmp(header, 0, 8);                                                                                                             <* 
 *>    if (rc != 0) {                                                                                                                              <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       printf("   - FATAL, header not png\n");                                                                                                  <* 
 *>       return  -4;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("format"    , "png");                                             <+/                                                      <* 
 *>    /+> printf("   - header is png\n");                                                <+/                                                      <* 
 *>    /+---(create read struct)-------------+/                                                                                                    <* 
 *>    png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);                                                                 <* 
 *>    if (png_ptr == NULL) {                                                                                                                      <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       printf("   - FATAL, png_ptr error\n");                                                                                                   <* 
 *>       return  -5;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("png_ptr"   , "pointer is good");                                 <+/                                                      <* 
 *>    /+> printf("   - png pointer is good\n");                                          <+/                                                      <* 
 *>    /+---(create info struct)-------------+/                                                                                                    <* 
 *>    info_ptr = png_create_info_struct (png_ptr);                                                                                                <* 
 *>    if (info_ptr == NULL) {                                                                                                                     <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       png_destroy_read_struct (&png_ptr, (png_infopp) NULL, (png_infopp) NULL);                                                                <* 
 *>       printf("   - FATAL, info_ptr error\n");                                                                                                  <* 
 *>       return  -6;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("info_ptr"  , "pointer is good");                                 <+/                                                      <* 
 *>    /+> printf("   - info pointer is good\n");                                         <+/                                                      <* 
 *>    /+---(create info struct)-------------+/                                                                                                    <* 
 *>    end_info = png_create_info_struct (png_ptr);                                                                                                <* 
 *>    if (end_info == NULL) {                                                                                                                     <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       png_destroy_read_struct (&png_ptr, &info_ptr, (png_infopp) NULL);                                                                        <* 
 *>       printf("   - FATAL, end_ptr error\n");                                                                                                   <* 
 *>       return  -7;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("end_ptr"   , "pointer is good");                                 <+/                                                      <* 
 *>    /+> printf("   - end pointer is good\n");                                          <+/                                                      <* 
 *>    /+---(error stuff 1)------------------+/                                                                                                    <* 
 *>    if (setjmp (png_jmpbuf (png_ptr))) {                                                                                                        <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);                                                                                 <* 
 *>       printf("   - FATAL, error during init_io\n");                                                                                            <* 
 *>       return  -8;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("jump"      , "set jump is good");                                <+/                                                      <* 
 *>    /+> printf("   - setjmp is good\n");                                               <+/                                                      <* 
 *>    /+---(setup read)---------------------+/                                                                                                    <* 
 *>    png_init_io          (png_ptr, f);                                                                                                          <* 
 *>    png_set_sig_bytes    (png_ptr, 8);                                                                                                          <* 
 *>    png_read_info        (png_ptr, info_ptr);                                                                                                   <* 
 *>    /+---(get info)-----------------------+/                                                                                                    <* 
 *>    png_get_IHDR         (png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);                                       <* 
 *>    twidth  = width;                                                                                                                            <* 
 *>    theight = height;                                                                                                                           <* 
 *>    png_read_update_info (png_ptr, info_ptr);                                                                                                   <* 
 *>    if (color_type != PNG_COLOR_TYPE_RGBA) {                                                                                                    <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);                                                                                 <* 
 *>       printf("   - FATAL, color is NOT RGBA\n");                                                                                               <* 
 *>       return  -9;                                                                                                                              <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("oolor"     , "RGBA is excellent");                               <+/                                                      <* 
 *>    /+> printf("   - color is RGBA, excellent\n");                                     <+/                                                      <* 
 *>    unsigned int rowbytes = png_get_rowbytes (png_ptr, info_ptr);                                                                               <* 
 *>    int channels = png_get_channels (png_ptr, info_ptr);                                                                                        <* 
 *>    /+> yLOG_value  ("width"     , twidth);                                            <+/                                                      <* 
 *>    /+> yLOG_value  ("height"    , theight);                                           <+/                                                      <* 
 *>    /+> yLOG_value  ("row_bytes" , rowbytes);                                          <+/                                                      <* 
 *>    /+> yLOG_value  ("bits"      , bit_depth);                                         <+/                                                      <* 
 *>    /+> yLOG_value  ("channels"  , channels);                                          <+/                                                      <* 
 *>    /+> yLOG_value  ("type"      , color_type);                                        <+/                                                      <* 
 *>    /+> printf("   - size     w=%3d, h=%3d, r=%3d, d=%3d, c=%3d, t=%3d\n", twidth, theight, rowbytes, bit_depth, channels, color_type);   <+/   <* 
 *>    /+---(transform)----------------------+/                                                                                                    <* 
 *>    png_set_palette_to_rgb (png_ptr);        /+ makes rgb                      +/                                                               <* 
 *>    /+> yLOG_info   ("palette"   , "good");                                            <+/                                                      <* 
 *>    png_set_tRNS_to_alpha  (png_ptr);        /+ expands out alpha              +/                                                               <* 
 *>    /+> yLOG_info   ("alpha"     , "good");                                            <+/                                                      <* 
 *>    /+> printf("   - palette and alpha are good\n");                                   <+/                                                      <* 
 *>    /+---(error stuff 2)------------------+/                                                                                                    <* 
 *>    if (setjmp(png_jmpbuf(png_ptr))) {                                                                                                          <* 
 *>       printf("   - FATAL, error during read image\n");                                                                                         <* 
 *>       png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);                                                                                 <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       return 0;                                                                                                                                <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("jump"      , "set jump is good");                                <+/                                                      <* 
 *>    /+---(allocate image)-----------------+/                                                                                                    <* 
 *>    png_byte *image_data = (uchar*) malloc(sizeof(png_byte) * rowbytes * theight);                                                              <* 
 *>    if (!image_data) {                                                                                                                          <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);                                                                                 <* 
 *>       printf("image_data error\n");                                                                                                            <* 
 *>       return 0;                                                                                                                                <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("image"     , "data is good");                                    <+/                                                      <* 
 *>    /+> printf("   - image data is good\n");                                           <+/                                                      <* 
 *>    /+---(row pointers)-------------------+/                                                                                                    <* 
 *>    png_bytep *row_pointers = (png_bytepp)  malloc(sizeof(png_bytep) * theight);                                                                <* 
 *>    if (row_pointers == NULL) {                                                                                                                 <* 
 *>       printf("row_pointers error\n");                                                                                                          <* 
 *>       png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);                                                                                 <* 
 *>       free (image_data);                                                                                                                       <* 
 *>       fclose(f);                                                                                                                               <* 
 *>       return 0;                                                                                                                                <* 
 *>    }                                                                                                                                           <* 
 *>    /+> yLOG_info   ("row_ptr"   , "row pointers are good");                           <+/                                                      <* 
 *>    /+> printf("   - row pointers are good\n");                                        <+/                                                      <* 
 *>    int i;                                                                                                                                      <* 
 *>    for (i = 0; i < theight; ++i) {                                                                                                             <* 
 *>       row_pointers[theight - 1 - i] = image_data + (i * rowbytes);                                                                             <* 
 *>    }                                                                                                                                           <* 
 *>    /+---(read)---------------------------+/                                                                                                    <* 
 *>    /+> printf("   - pre read_image\n");                                               <+/                                                      <* 
 *>    png_read_image(png_ptr, row_pointers);                                                                                                      <* 
 *>    /+> yLOG_info   ("read"      , "image is good");                                   <+/                                                      <* 
 *>    /+> printf("   - read_image was good\n");                                          <+/                                                      <* 
 *>    /+> uchar *bytey;                                                                  <*                                                       <* 
 *>     *> for (i = 0; i < rowbytes; ++i) {                                               <*                                                       <* 
 *>     *>    bytey = image_data + (50 * rowbytes) + i;                                   <*                                                       <* 
 *>     *>    *bytey = 0  ;                                                               <*                                                       <* 
 *>     *> }                                                                              <+/                                                      <* 
 *>    /+---(make texture)-------------------+/                                                                                                    <* 
 *>    GLuint texture;                                                                                                                             <* 
 *>    glGenTextures   (1, &texture);                                                                                                              <* 
 *>    glBindTexture   (GL_TEXTURE_2D, texture);                                                                                                   <* 
 *>    glTexParameteri (GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);                                                           <* 
 *>    glTexParameteri (GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, GL_LINEAR);                                                                         <* 
 *>    glTexParameterf (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,     GL_REPEAT );                                                                        <* 
 *>    glTexParameterf (GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,     GL_REPEAT );                                                                        <* 
 *>    glTexParameteri (GL_TEXTURE_2D,  GL_GENERATE_MIPMAP,    GL_TRUE);                                                                           <* 
 *>    glTexEnvi       (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_REPLACE);                                                                        <* 
 *>    /+> glHint          (GL_TEXTURE_COMPRESSION_HINT,           GL_NICEST);            <+/                                                      <* 
 *>    /+> glTexImage2D    (GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);   <+/                <* 
 *>    glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGBA, twidth, theight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);                                     <* 
 *>    glBindTexture (GL_TEXTURE_2D, 0);                                                                                                           <* 
 *>    /+---(clean)--------------------------+/                                                                                                    <* 
 *>    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);                                                                                    <* 
 *>    free(image_data);                                                                                                                           <* 
 *>    free(row_pointers);                                                                                                                         <* 
 *>    fclose(f);                                                                                                                                  <* 
 *>    /+---(complete)-----------------------+/                                                                                                    <* 
 *>    syms  = texture;                                                                                                                            <* 
 *>    /+> yLOG_exit   (__FUNCTION__);                                                    <+/                                                      <* 
 *>    return texture;                                                                                                                             <* 
 *> }                                                                                                                                              <*/

/*> int                                                                                         <* 
 *> yFONT_symbol       (float a_scale, int  a_row, int a_col, int a_mod)                        <* 
 *> {                                                                                           <* 
 *>    if (syms == 0)  return -1;                                                               <* 
 *>    float    x_row [25] = {                                                                  <* 
 *>       0.9354, 0.8740, 0.8115, 0.7500,                                                       <* 
 *>       0.6875, 0.6265, 0.5640, 0.5030,                                                       <* 
 *>       0.4410, 0.3790, 0.3180, 0.2555,                                                       <* 
 *>       0.1935, 0.1322, 0.0700, 0.0080,                                                       <* 
 *>    };                                                                                       <* 
 *>    float    x_col [25] = {                                                                  <* 
 *>       0.0032, 0.0508, 0.0982,                                                               <* 
 *>       0.1450, 0.1920, 0.2390,                                                               <* 
 *>       0.2870, 0.3340, 0.3810,                                                               <* 
 *>       0.4280, 0.4760, 0.5225,                                                               <* 
 *>       0.5702, 0.6170, 0.6645,                                                               <* 
 *>       0.7110, 0.7585, 0.8060,                                                               <* 
 *>       0.8530, 0.9000, 0.9475,                                                               <* 
 *>    };                                                                                       <* 
 *>    float    x_width    = 0.0460 - 0.0040;                                                   <* 
 *>    float    x_height   = 0.0547 - 0.0040;                                                   <* 
 *>    float    x          = x_col [a_col] + 0.0020;                                            <* 
 *>    float    y          = x_row [a_row] + 0.0020;                                            <* 
 *>    /+> printf ("r=%8.3f, c=%8.3f\n", y, x);                                           <+/   <* 
 *>    /+---(draw text)-----------------------------+/                                          <* 
 *>    glPushMatrix(); {                                                                        <* 
 *>       glScalef(a_scale, a_scale, a_scale);                                                  <* 
 *>       switch (a_mod) {                                                                      <* 
 *>       case  1  : glColor4f (1.0f, 0.0f, 0.0f, 0.2f); break;                                 <* 
 *>       case  2  : glColor4f (0.7f, 0.3f, 0.0f, 0.2f); break;                                 <* 
 *>       case  3  : glColor4f (0.3f, 0.7f, 0.0f, 0.2f); break;                                 <* 
 *>       case  4  : glColor4f (0.0f, 0.1f, 0.0f, 0.2f); break;                                 <* 
 *>       case  5  : glColor4f (0.0f, 0.7f, 0.3f, 0.2f); break;                                 <* 
 *>       case  6  : glColor4f (0.0f, 0.3f, 0.7f, 0.2f); break;                                 <* 
 *>       case  7  : glColor4f (0.0f, 0.0f, 1.0f, 0.2f); break;                                 <* 
 *>       case  8  : glColor4f (0.3f, 0.0f, 0.7f, 0.2f); break;                                 <* 
 *>       case  9  : glColor4f (0.7f, 0.0f, 0.3f, 0.2f); break;                                 <* 
 *>       default  : glColor4f (0.5f, 0.5f, 0.5f, 0.2f); break;                                 <* 
 *>       }                                                                                     <* 
 *>       glBegin  (GL_POLYGON); {                                                              <* 
 *>          glVertex3f   (    0.0, -20.0,   0.0);                                              <* 
 *>          glVertex3f   (   20.0, -20.0,   0.0);                                              <* 
 *>          glVertex3f   (   20.0,   0.0,   0.0);                                              <* 
 *>          glVertex3f   (    0.0,   0.0,   0.0);                                              <* 
 *>       } glEnd();                                                                            <* 
 *>       /+> glColor4f (0.0f, 0.0f, 0.0f, 0.2f);                                         <*    <* 
 *>        *> glLineWidth (1.5);                                                          <*    <* 
 *>        *> glBegin  (GL_LINE_STRIP); {                                                 <*    <* 
 *>        *>    glVertex3f   (   -1.0, -21.0,   5.0);                                    <*    <* 
 *>        *>    glVertex3f   (   21.0, -21.0,   5.0);                                    <*    <* 
 *>        *>    glVertex3f   (   21.0,   1.0,   5.0);                                    <*    <* 
 *>        *>    glVertex3f   (   -1.0,   1.0,   5.0);                                    <*    <* 
 *>        *>    glVertex3f   (   -1.0, -21.0,   5.0);                                    <*    <* 
 *>        *> } glEnd();                                                                  <+/   <* 
 *>       glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);                                           <* 
 *>       glBindTexture(GL_TEXTURE_2D, syms);                                                   <* 
 *>       glColor4f (1.0f, 1.0f, 1.0f, 1.0f);                                                   <* 
 *>       glBegin  (GL_POLYGON); {                                                              <* 
 *>                                                                                             <* 
 *>          glTexCoord2f ( x          , y);                                                    <* 
 *>          glVertex3f   (    0.0, -20.0,   0.0);                                              <* 
 *>                                                                                             <* 
 *>          glTexCoord2f ( x + x_width, y);                                                    <* 
 *>          glVertex3f   (   20.0, -20.0,   0.0);                                              <* 
 *>                                                                                             <* 
 *>          glTexCoord2f ( x + x_width, y + x_height);                                         <* 
 *>          glVertex3f   (   20.0,   0.0,   0.0);                                              <* 
 *>                                                                                             <* 
 *>          glTexCoord2f ( x          , y + x_height);                                         <* 
 *>          glVertex3f   (    0.0,   0.0,   0.0);                                              <* 
 *>                                                                                             <* 
 *>       } glEnd();                                                                            <* 
 *>       glBindTexture(GL_TEXTURE_2D, 0);                                                      <* 
 *>    } glPopMatrix();                                                                         <* 
 *>    /+---(complete)------------------------------+/                                          <* 
 *>    return 0;                                                                                <* 
 *> }                                                                                           <*/

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
