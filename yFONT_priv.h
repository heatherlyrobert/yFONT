/*============================----beg-of-source---============================*/

#include "yFONT.h"

/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YFONT_private
#define YFONT_private loaded


/* rapidly evolving version number to aid with visual change confirmation     */
#define     YFONT_VER_NUM   "2.0h"
#define     YFONT_VER_TXT   "creates clean indexes now, ready for texture"




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




/*===[[ RATIONAL LIMITS ]]====================================================*/
/*   LEN_ is a length or size of something
 *   MIN_ is a minimum count
 *   DEF_ is a default count
 *   MAX_ is a maximum count
 *
 */
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012*/
/*---(string length)------------------*/
#define     LEN_RECD     2000
#define     LEN_STR      200
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
typedef     struct      cFONT       tFONT;
typedef     struct      cGLYPH      tGLYPH;
typedef     struct      cPOINT      tPOINT;
typedef     struct      cVERT       tVERT;



/*===[[ FONT DATA STRUCTURE ]]================================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/

struct      cFONT {
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
   int      min_glyph;                 /* smallest unicode num in font        */
   int      max_glyph;                 /* smallest unicode num in font        */
   int      range;                     /* diff in min/max unicode numbers     */
   /*---(texture)-----------*/
   int16_t  tex_w;                     /* texture width                       */
   int16_t  tex_h;                     /* texture height                      */
   GLuint   tex_ref;                   /* opengl texture reference            */
   uchar   *tex_bits;                  /* actual texture                      */
   /*---(glyphs)------------*/
   int16_t  num_glyph;                 /* number of glyphs included           */
   tGLYPH  *glyphs;                    /* allocated glyph table               */
   tVERT   *verts;                     /* glyph vertices                      */
   short   *lookup;                    /* vertex lookup table                 */
   /*---(done)--------------*/
};
extern      tFONT       *g_font       [MAX_FONT];



/*===[[ GLYPH DATA STRUCTURE ]]===============================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/

struct cGLYPH {
   /*---(lookup)--------------*/
   uint        code;          /* unicode number                               */
   char        good;          /* good (y) or bad (-)                          */
   /*---(texture)-------------*/
   uint        xpos;          /* glyph x position in texture                  */
   uint        ypos;          /* glyph y position in texture                  */
   uchar       wide;          /* glyph width                                  */
   uchar       tall;          /* glyph height                                 */
   /*---(placement)-----------*/
   schar       xoff;          /* glyph placement offset -- horizontal         */
   schar       yoff;          /* glyph placement offset -- vertical           */
   schar       adv;           /* position advancement for next glyph start    */
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

char        yFONT__conf_info     (char *a_name, char *a_point, char *a_adjust, char *a_glist, char *a_source);
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
tFONT*      yFONT__slot_font     (char  a_slot);

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

char        yFONT__verts       (tFONT *a_txf);
char        yFONT__index       (tFONT *a_txf);

char*       yFONT__unit        (char *a_question, int a_num);
char        yFONT__testquiet   (void);
char        yFONT__testloud    (void);
char        yFONT__testend     (void);


#endif


/*============================----end-of-source---============================*/
