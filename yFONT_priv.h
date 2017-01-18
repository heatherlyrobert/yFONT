/*============================----beg-of-source---============================*/

#include "yFONT.h"

/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YFONT_private
#define YFONT_private loaded


/* rapidly evolving version number to aid with visual change confirmation     */
#define YFONT_VER_NUM   "2.0d"
#define YFONT_VER_TXT   "got it back to full compile before more changes"



/*===[[ HEADERS ]]========================================*/
/*---(ansi-c standard)-------------------*/
#include    <stdio.h>        /* CLIBC   standard input/output                 */
#include    <stdlib.h>       /* CLIBC   standard general purpose              */
#include    <string.h>       /* CLIBC   standard string handling              */

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
   /*---(file)--------------*/
   FILE    *file;                      /* file (temp) for reading and writing */
   char     name         [LEN_LABEL];  /* short name of font (debugging)      */
   char     slot;                      /* font array slot    (debugging)      */
   /*---(characteristics)---*/
   int      point;                     /* base font point                     */
   int      max_ascent;                /* greatest rise above baseline        */
   int      max_descent;               /* greatest fall below baseline        */
   int      margin;                    /* glyph margin allowed                */
   /*---(working)-----------*/
   int      min_glyph;                 /* smallest unicode num in font        */
   int      max_glyph;                 /* smallest unicode num in font        */
   int      range;                     /* diff in min/max unicode numbers     */
   /*---(texture)-----------*/
   ulong    tex_w;                     /* texture width                       */
   ulong    tex_h;                     /* texture height                      */
   GLuint   tex_ref;                   /* opengl texture reference            */
   uchar   *tex_bits;                  /* actual texture                      */
   /*---(glyphs)------------*/
   int      num_glyph;                 /* number of glyphs included           */
   tGLYPH  *glyphs;                    /* allocated glyph table               */
   tVERT   *verts;                     /* glyph vertices                      */
   short   *lookup;                    /* vertex lookup table                 */
   /*---(done)--------------*/
};
extern      tFONT       *g_fonts      [MAX_FONT];



/*===[[ GLYPH DATA STRUCTURE ]]===============================================*/
/*3456789012+123456789012+123456789012+123456789012+123456789012+123456789012-*/

struct cGLYPH {
   /*---(lookup)--------------*/
   uint        c;             /* unicode number                               */
   /*---(texture)-------------*/
   uint        x;             /* glyph x position in texture                  */
   uint        y;             /* glyph y position in texture                  */
   uchar       w;             /* glyph width                                  */
   uchar       h;             /* glyph height                                 */
   /*---(placement)-----------*/
   schar       xo;            /* glyph placement offset -- horizontal         */
   schar       yo;            /* glyph placement offset -- vertical           */
   schar       a;             /* position advancement for next glyph start    */
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

char        yFONT__slot_new    (void);
char        yFONT__slot_next   (void);
char        yFONT__slot_alloc  (char  a_slot);
char        yFONT__slot_init   (char  a_slot);
char        yFONT__slot_free   (char  a_slot);

char        yFONT__file_open   (char  a_slot, char *a_name, char a_mode);
char        yFONT__file_close  (char  a_slot);

char        yFONT__verts       (tFONT *a_txf);
char        yFONT__index       (tFONT *a_txf);



#endif


/*============================----end-of-source---============================*/
