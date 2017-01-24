#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <math.h>

#include    "yFONT.h"
#include    "yFONT_priv.h"

tFONT       s_font;

int
main               (int argc, char *argv[])
{
   int         rc        = 0;                 /* generic return code            */
   char        x_name      [100]    = "arial";
   char        x_slot    = 0;
   if (argc == 2) {
      strncpy (x_name, argv [1], 90);
   }
   /*---(read config)--------------------*/
   if (rc >= 0)  rc = yFONT__conf_parse  ();
   /*---(setup slot)---------------------*/
   if (rc >= 0)  rc = yFONT__slot_new    ();
   if (rc >= 0)  x_slot = rc;
   /*---(open the font)------------------*/
   if (rc >= 0)  rc = yFONT__head_name   (x_slot, x_name, 40);
   if (rc >= 0)  rc = yFONT__file_open   (x_slot, 'r');
   /*---(header)-------------------------*/
   if (rc >= 0)  rc = yFONT__head_read   (x_slot);
   if (rc >= 0)  rc = yFONT__head_dump   (x_slot);
   /*---(index)--------------------------*/
   if (rc >= 0)  rc = yFONT__index_alloc (x_slot);
   if (rc >= 0)  rc = yFONT__index_read  (x_slot);
   if (rc >= 0)  rc = yFONT__index_dump  (x_slot);
   /*---(texture)------------------------*/
   if (rc >= 0)  rc = yFONT__map_alloc   (x_slot);
   if (rc >= 0)  rc = yFONT__map_read    (x_slot);
   if (rc >= 0)  rc = yFONT__map_texart  (x_slot);
   /*---(close the font)-----------------*/
   if (rc >= 0)  rc = yFONT__map_free    (x_slot);
   if (rc >= 0)  rc = yFONT__index_free  (x_slot);
   if (rc >= 0)  rc = yFONT__file_close  (x_slot);
   if (rc >= 0)  rc = yFONT__slot_free   (x_slot);
   /*---(complete)-----------------------*/
   return 0;
}
