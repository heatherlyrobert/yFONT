/*============================----beg-of-source---============================*/

#include "yFONT.h"

/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YFONT_private
#define YFONT_private loaded


/* rapidly evolving version number to aid with visual change confirmation     */
#define     YFONT_VER_NUM   "2.1j"
#define     YFONT_VER_TXT   "added a yFONT_width to generate sizing without printing"




/*===[[ HEADERS ]]========================================*/
/*---(ansi-c standard)-------------------*/
#include    <stdio.h>        /* CLIBC   standard input/output                 */
#include    <stdlib.h>       /* CLIBC   standard general purpose              */
#include    <string.h>       /* CLIBC   standard string handling              */
#include    <stdint.h>       /* CLIBC   standard integer sizes                */

/*---(X11 standard)----------------------*/
#include    <X11/X.h>        /* X11     standard overall file                 */
#include    <X11/Xlib.h>     /* X11     standard C API                        */


/*---(opengl standard)-------------------*/
#include    <GL/gl.h>        /* OPENGL  standard primary header               */
#include    <GL/glx.h>       /* OPENGL  standard X11 integration              */

/*---(heatherly made)--------------------*/
#include    <yURG.h>         /* CUSTOM  heatherly urgent processing           */
#include    <yLOG.h>         /* CUSTOM  heatherly program logging             */
#include    <ySTR.h>         /* CUSTOM  heatherly string handling             */
#include    <yVAR.h>         /* CUSTOM  heatherly variable testing            */
#include    <yX11.h>         /* CUSTOM  heatherly xlib/glx setup/teardown     */
#include    <yGLTEX.h>       /* CUSTOM  heatherly texture handling            */



#define     ICON_SET  "/usr/local/share/fonts/outline_icons.png"





/*===[[ RATIONAL LIMITS ]]====================================================*/
/*   LEN_ is a length or size of something
 *   MIN_ is a minimum count
 *   DEF_ is a default count
 *   MAX_ is a maximum count
 *
 */
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012*/
/*---(string length)------------------*/
#define     LEN_HUGE     10000
#define     LEN_RECD     2000
#define     LEN_STR      500
#define     LEN_LABEL    20
/*---(fonts)--------------------------*/
#define     MAX_FONT     10


/*===[[ CONSTANTS ]]==========================================================*/
/*---(major modes)-----------------------*/

#define     YFONT_BYTE   0
#define     YFONT_BIT    1
#define     LEN_LINE      2000     /* max input line   */



/*===[[ TYPEDEFS ]]===========================================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/
/*---(basics)--------------------------*/
typedef     signed      char        schar;
typedef     unsigned    char        uchar;
typedef     const       char        cchar;
typedef     unsigned    short       ushort;
typedef     signed      long        slong;
typedef     unsigned    long        ulong;
typedef     signed      long long   llong;
typedef     unsigned    long long   ullong;
/*---(data structures)-----------------*/
typedef     struct      cYFONT      tYFONT;
typedef     struct      cGLYPH      tGLYPH;
typedef     struct      cPOINT      tPOINT;
typedef     struct      cVERT       tVERT;



/*===[[ FONT DATA STRUCTURE ]]================================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/

extern      tYFONT       *g_yfont       [MAX_FONT];
struct      cYFONT {
   /*---(tie to fonts)------*/
   char     slot;                      /* font array slot    (debugging)      */
   /*---(file)--------------*/
   FILE    *file;                      /* file (temp) for reading and writing */
   char     version      [LEN_LABEL];  /* yFONT version that created          */
   char     name         [LEN_LABEL];  /* short name of font (debugging)      */
   /*---(characteristics)---*/
   char     point;                     /* base font point                     */
   char     max_ascent;                /* greatest rise above baseline        */
   char     max_descent;               /* greatest fall below baseline        */
   char     margin;                    /* glyph margin allowed                */
   /*---(working)-----------*/
   int      spacer;                    /* unicode for size of space           */
   int      min_glyph;                 /* smallest unicode num in font        */
   int      max_glyph;                 /* smallest unicode num in font        */
   int      range;                     /* diff in min/max unicode numbers     */
   /*---(glyphs)------------*/
   int16_t  num_glyph;                 /* number of glyphs included           */
   tGLYPH  *glyphs;                    /* allocated glyph table               */
   /*---(texture)-----------*/
   int16_t  tex_w;                     /* texture width                       */
   int16_t  tex_h;                     /* texture height                      */
   GLuint   tex_ref;                   /* opengl texture reference            */
   uchar   *tex_map;                   /* actual texture                      */
   /*---(working)-----------*/
   tVERT   *verts;                     /* glyph vertices                      */
   short   *lookup;                    /* vertex lookup table                 */
   /*---(done)--------------*/
};



/*===[[ GLYPH DATA STRUCTURE ]]===============================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/

struct cGLYPH {
   /*---(lookup)--------------*/
   int32_t     code;          /* unicode number                               */
   char        good;          /* good (y) or bad (-)                          */
   /*---(texture)-------------*/
   int16_t     xpos;          /* glyph x position in texture                  */
   int16_t     ypos;          /* glyph y position in texture                  */
   char        wide;          /* glyph width                                  */
   char        tall;          /* glyph height                                 */
   /*---(placement)-----------*/
   char        xoff;          /* glyph placement offset -- horizontal         */
   char        yoff;          /* glyph placement offset -- vertical           */
   char        adv;           /* position advancement for next glyph start    */
   /*---(done)----------------*/
};



/*===[[ VERTEX DATA STRUCTURE ]]==============================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/

struct      cPOINT {
   schar       rel_x;
   schar       rel_y;
   double      tex_x;
   double      tex_y;
};

struct      cVERT {
   tPOINT      TL;
   tPOINT      TR;
   tPOINT      BR;
   tPOINT      BL;
   char        a;
};



/*===[[ PROTOTYPES ]]=========================================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/

char        yFONT__list          (void);

char        yFONT__conf_head     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_space, char *a_glist, char *a_source);
char        yFONT__conf_next     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_space, char *a_glist, char *a_source);
char        yFONT__conf_prev     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_space, char *a_glist, char *a_source);
char        yFONT__conf_info     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_space, char *a_glist, char *a_source);
char       *yFONT__conf_font     (char a_entry);
char        yFONT__conf_name     (char *a_name);
char        yFONT__conf_list     (void);
char        yFONT__conf_open     (void);
char        yFONT__conf_close    (void);
char        yFONT__conf_parse    (void);

char        yFONT__slot_new      (void);
char        yFONT__slot_next     (void);
char        yFONT__slot_alloc    (char  a_slot);
char        yFONT__slot_init     (char  a_slot);
char        yFONT__slot_purge    (void);
char        yFONT__slot_free     (char  a_slot);
tYFONT*     yFONT__slot_font     (char  a_slot);

char        yFONT__file_open     (char  a_slot, char a_mode);
char        yFONT__file_close    (char  a_slot);

/*---(read/write)-----------*/
char        yFONT__head_read     (char  a_slot);
char        yFONT__head_write    (char  a_slot);
/*---(add data)-------------*/
char        yFONT__head_name     (char  a_slot, char *a_name, char a_point);
char        yFONT__head_tex      (char  a_slot, short a_texw, short a_texh);
char        yFONT__head_place    (char  a_slot, char a_mascent, char a_mdescent);
char        yFONT__head_margin   (char  a_slot, char a_margin);
char        yFONT__head_nglyph   (char  a_slot, short a_nglyph);
/*---(reporting)------------*/
char        yFONT__head_title    (void);
char        yFONT__head_line     (char  a_slot);
char        yFONT__head_dump     (char  a_slot);

char        yFONT__index_alloc   (char  a_slot);
char        yFONT__index_init    (char  a_slot,  int a_entry);
char        yFONT__index_wipe    (char  a_slot);
char        yFONT__index_free    (char  a_slot);
char        yFONT__index_code    (char  a_slot,  int a_order, int   a_code, char  a_good);
char        yFONT__index_size    (char  a_slot,  int a_order, char  a_wide, char  a_tall);
char        yFONT__index_offset  (char  a_slot,  int a_order, char  a_xoff, char  a_yoff, char a_adv);
char        yFONT__index_pos     (char  a_slot,  int a_order, short a_xpos, short a_ypos);
char        yFONT__index_maxes   (char  a_slot);
char        yFONT__index_wide    (char  a_slot, int  a_entry);
char        yFONT__index_dump    (char  a_slot);
char        yFONT__index_who     (char  a_slot, int  a_entry, int *a_code, char *a_good);
char        yFONT__index_coords  (char  a_slot, int  a_entry, short *a_xpos, short *a_ypos, char *a_wide, char *a_tall);
char        yFONT__index_write   (char  a_slot);
char        yFONT__index_lookup  (char  a_slot);
int         yFONT__index_widthu  (tYFONT *a_font, int  *a_array, int a_len);
int         yFONT__index_width   (tYFONT *a_font, char *a_text , int a_len);

char        yFONT__map_alloc     (char  a_slot);
char        yFONT__map_write     (char  a_slot);
char        yFONT__map_read      (char  a_slot);
char        yFONT__map_art       (char  a_slot, int  a_entry);
char        yFONT__map_texart    (char  a_slot);
char        yFONT__map_texture   (char  a_slot);
char        yFONT__verts_alloc   (char  a_slot);
char        yFONT__verts_load    (char  a_slot);
tVERT*      yFONT__verts_find    (tYFONT *a_font, int a_code);
char        yFONT__map_glyph     (tYFONT *a_font, int a_code);

char        yFONT__verts         (tYFONT *a_txf);
char        yFONT__index         (tYFONT *a_txf);

char*       yFONT__unit          (char *a_question, int a_num);
char        yFONT__testquiet     (void);
char        yFONT__testloud      (void);
char        yFONT__testend       (void);


#endif


/*============================----end-of-source---============================*/
