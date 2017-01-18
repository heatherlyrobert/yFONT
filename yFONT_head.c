/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"



/*> typedef struct  cHEAD  tHEAD;                                                     <* 
 *> struct cHEAD {                                                                    <* 
 *>    char        magic_id;                                                          <* 
 *>    char        magic_num   [ 4];                                                  <* 
 *>    char        version     [ 5];                                                  <* 
 *>    char        name        [20];                                                  <* 
 *>    char        format;                                                            <* 
 *>    char        style;                                                             <* 
 *>    char        point;                                                             <* 
 *>    short       tex_width;                                                         <* 
 *>    short       tex_height;                                                        <* 
 *>    char        max_ascent;                                                        <* 
 *>    char        max_descent;                                                       <* 
 *>    char        margin;                                                            <* 
 *>    short       nglyphs;                                                           <* 
 *> };                                                                                <*/

/*> static tFONT*             /+ PURPOSE : read a font header                            +/                                                                <* 
 *> yFONT_head_read    (char *a_name)                                                                                                                      <* 
 *> {                                                                                                                                                      <* 
 *>    /+---(locals)--------------------------------+/                                                                                                     <* 
 *>    int       rc        = 0;                 /+ generic return code            +/                                                                       <* 
 *>    tFONT    *txf       = NULL;              /+ new font                       +/                                                                       <* 
 *>    char      fileid[4];                     /+ magic number of file           +/                                                                       <* 
 *>    int       type      = 0;                 /+ data type                      +/                                                                       <* 
 *>    int       value     = 0;                                                                                                                            <* 
 *>    char      x_name [1000];                                                                                                                            <* 
 *>    /+---(allocate the font)---------------------+/                                                                                                     <* 
 *>    txf = (tFONT *) malloc(sizeof(tFONT));                                                                                                              <* 
 *>    if (txf == NULL) {                                                                                                                                  <* 
 *>       fprintf(stderr, "yFONT_load() : can not allocate memory for font\n");                                                                            <* 
 *>       free (txf);                                                                                                                                      <* 
 *>       return NULL;                                                                                                                                     <* 
 *>    }                                                                                                                                                   <* 
 *>    /+---(initialize)----------------------------+/                                                                                                     <* 
 *>    yFONT__init(txf);                                                                                                                                   <* 
 *>    /+---(open file)-----------------------------+/                                                                                                     <* 
 *>    snprintf(x_name, 900, "/usr/local/share/fonts/%s.txf", a_name);                                                                                     <* 
 *>    txf->f = fopen(x_name, "rb");                                                                                                                       <* 
 *>    if (txf->f == NULL) {                                                                                                                               <* 
 *>       fprintf(stderr, "yFONT_load() : can not open font file\n");                                                                                      <* 
 *>       yFONT__free (txf);                                                                                                                               <* 
 *>       return NULL;                                                                                                                                     <* 
 *>    }                                                                                                                                                   <* 
 *>    /+---(check file type)-----------------------+/                                                                                                     <* 
 *>    rc = fread(fileid, 1, 4, txf->f);                                                                                                                   <* 
 *>    if (rc != 4) { fprintf(stderr, "yFONT_load() : premature end of file (magic#)\n"); yFONT__free (txf); return NULL; }                                <* 
 *>    if (strncmp(fileid, "\377txf", 4) != 0) {                                                                                                           <* 
 *>       fprintf(stderr, "yFONT_load() : not a txf formatted file\n");                                                                                    <* 
 *>       yFONT__free (txf);                                                                                                                               <* 
 *>       return NULL;                                                                                                                                     <* 
 *>    }                                                                                                                                                   <* 
 *>    /+---(name)----------------------------------+/                                                                                                     <* 
 *>    rc = fread (&x_name,   sizeof (char), 20, txf->f);                                                                                                  <* 
 *>    if (rc != 20) { fprintf(stderr, "yFONT_load() : premature end of file (name)\n"); yFONT__free (txf); return NULL; }                                 <* 
 *>    strcpy (txf->name, x_name);                                                                                                                         <* 
 *>    /+---(point size)----------------------------+/                                                                                                     <* 
 *>    rc = fread(&value,   sizeof(int), 1, txf->f);                                                                                                       <* 
 *>    if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (point)\n"); yFONT__free (txf); return NULL; }                                 <* 
 *>    txf->p           = value;                                                                                                                           <* 
 *>    if (txf->p < 6 || txf->p > 99) { fprintf(stderr, "yFONT_load() : point size out of range %d (6-99)\n", txf->p); yFONT__free (txf); return NULL; }   <* 
 *>    /+> printf("points      = %8d\n", txf->p);                                         <+/                                                              <* 
 *>    /+---(check metrics)-------------------------+/                                                                                                     <* 
 *>    rc = fread(&value,   sizeof(ulong), 1, txf->f);                                                                                                     <* 
 *>    if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (width)\n"); yFONT__free (txf); return NULL; }                                 <* 
 *>    txf->w           = value;                                                                                                                           <* 
 *>    /+> printf("tex width   = %8d\n", txf->w);                                         <+/                                                              <* 
 *>    /+---()---+/                                                                                                                                        <* 
 *>    rc = fread(&value,   sizeof(int), 1, txf->f);                                                                                                       <* 
 *>    if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (height)\n"); yFONT__free (txf); return NULL; }                                <* 
 *>    txf->h           = value;                                                                                                                           <* 
 *>    /+> printf("tex height  = %8d\n", txf->h);                                         <+/                                                              <* 
 *>    /+---()---+/                                                                                                                                        <* 
 *>    rc = fread(&value,   sizeof(int), 1, txf->f);                                                                                                       <* 
 *>    if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (ascent)\n"); yFONT__free (txf); return NULL; }                                <* 
 *>    txf->max_ascent  = value;                                                                                                                           <* 
 *>    /+> printf("max ascent  = %8d\n", txf->max_ascent);                                <+/                                                              <* 
 *>    /+---()---+/                                                                                                                                        <* 
 *>    rc = fread(&value,   sizeof(int), 1, txf->f);                                                                                                       <* 
 *>    if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (descent)\n"); yFONT__free (txf); return NULL; }                               <* 
 *>    txf->max_descent = value;                                                                                                                           <* 
 *>    /+> printf("max descent = %8d\n", txf->max_descent);                               <+/                                                              <* 
 *>    /+---()---+/                                                                                                                                        <* 
 *>    rc = fread(&value,   sizeof(int), 1, txf->f);                                                                                                       <* 
 *>    if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (margin)\n"); yFONT__free (txf); return NULL; }                                <* 
 *>    txf->margin      = value;                                                                                                                           <* 
 *>    /+---()---+/                                                                                                                                        <* 
 *>    rc = fread(&value,   sizeof(int), 1, txf->f);                                                                                                       <* 
 *>    if (rc != 1) { fprintf(stderr, "yFONT_load() : premature end of file (num glyphs)\n"); yFONT__free (txf); return NULL; }                            <* 
 *>    txf->n_glyph  = value;                                                                                                                              <* 
 *>    /+> printf("num glyphs  = %8d\n", txf->n_glyph);                                <+/                                                                 <* 
 *>    /+---(complete)------------------------------+/                                                                                                     <* 
 *>    return txf;                                                                                                                                         <* 
 *> }                                                                                                                                                      <*/

