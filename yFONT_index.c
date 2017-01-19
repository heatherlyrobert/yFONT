/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"


char
yFONT__index_alloc      (char  a_slot)
{
   /*---(allocate the glyph info)---------------*/
   /*> x_txf->glyphs = (tGLYPH *) malloc(x_txf->num_glyph * sizeof(tGLYPH));          <* 
    *> if (x_txf->glyphs == NULL) {                                                   <* 
    *>    fprintf(stderr, "yFONT_load() : could not allocate glyph info\n");          <* 
    *>    yFONT__free (x_txf);                                                        <* 
    *>    return YF_MEM_FULL;                                                         <* 
    *> }                                                                              <*/
}

char
yFONT__index_unicode    (char  a_slot,  int a_order, int a_unicode)
{
}

char
yFONT__index_size       (char  a_slot,  int a_order, short a_xpos, short a_ypos, char a_width, char a_height)
{
}

char
yFONT__index_offset     (char  a_slot,  int a_order, char a_xoff, char a_yoff, char a_advance)
{
}

char
yFONT__index_read       (char  a_slot)
{
}

char
yFONT__index_write      (char  a_slot)
{
}
