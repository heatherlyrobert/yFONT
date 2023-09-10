/*============================----beg-of-source---============================*/

/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"

/*2345678912+12345678912+12345678912+12345678912+12345678912+12345678912+12345*/
#define     FILE_CONF   "/etc/yFONT.conf"
#define     MAX_CONF    200

char        s_confname  [LEN_STR]        = FILE_CONF;
char        s_fontname  [LEN_LABEL]      = "";

/*2345678912+12345678912+12345678912+12345678912+12345678912+12345678912+12345*/
typedef     struct      cFONT_INFO  tFONT_INFO;
struct  cFONT_INFO {
   char        cat         [LEN_LABEL];
   char        name        [LEN_LABEL];
   char        type        [LEN_LABEL];
   char        point;                  /* base font point                     */
   char        adjust;                 /* point adjust to make "seem" right   */
   int         spacer;
   char        glist       [LEN_LABEL];
   char        source      [LEN_STR  ];
};
tFONT_INFO  s_font_info [MAX_CONF];
int         s_nentry     = 0;



/*====================------------------------------------====================*/
/*===----                     getting header values                    ----===*/
/*====================------------------------------------====================*/
static void      o___GETTERS_________________o (void) {;}

/*2345678912+12345678912+12345678912+12345678912+12345678912+12345678912+12345*/
static      FILE       *s_conf      = NULL;
static      int         s_lines     = 0;
static      int         s_curr      = 0;

char         /*--> load the first config font ------------[ ------ [ ------ ]-*/
yFONT__conf_head     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_spacer, char *a_glist, char *a_source)
{
   s_curr = 0;
   if (a_name   != NULL)  ystrlcpy (a_name  , s_font_info [s_curr].name  , LEN_LABEL);
   if (a_type   != NULL)  ystrlcpy (a_type  , s_font_info [s_curr].type  , LEN_LABEL);
   if (a_point  != NULL)  *a_point  = s_font_info [s_curr].point;
   if (a_adjust != NULL)  *a_adjust = s_font_info [s_curr].adjust;
   if (a_spacer != NULL)  *a_spacer = s_font_info [s_curr].spacer;
   if (a_glist  != NULL)  ystrlcpy (a_glist , s_font_info [s_curr].glist , LEN_LABEL);
   if (a_source != NULL)  ystrlcpy (a_source, s_font_info [s_curr].source, LEN_STR  );
   return 0;
}

char         /*--> load the next config font -------------[ ------ [ ------ ]-*/
yFONT__conf_next     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_spacer, char *a_glist, char *a_source)
{
   ++s_curr;
   if (s_curr >= s_nentry) return -1;
   if (a_name   != NULL)  ystrlcpy (a_name  , s_font_info [s_curr].name  , LEN_LABEL);
   if (a_type   != NULL)  ystrlcpy (a_type  , s_font_info [s_curr].type  , LEN_LABEL);
   if (a_point  != NULL)  *a_point  = s_font_info [s_curr].point;
   if (a_adjust != NULL)  *a_adjust = s_font_info [s_curr].adjust;
   if (a_spacer != NULL)  *a_spacer = s_font_info [s_curr].spacer;
   if (a_glist  != NULL)  ystrlcpy (a_glist , s_font_info [s_curr].glist , LEN_LABEL);
   if (a_source != NULL)  ystrlcpy (a_source, s_font_info [s_curr].source, LEN_STR  );
   return 0;
}

char         /*--> load the next config font -------------[ ------ [ ------ ]-*/
yFONT__conf_prev     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_spacer, char *a_glist, char *a_source)
{
   --s_curr;
   if (s_curr <  0) return -1;
   if (a_name   != NULL)  ystrlcpy (a_name  , s_font_info [s_curr].name  , LEN_LABEL);
   if (a_type   != NULL)  ystrlcpy (a_type  , s_font_info [s_curr].type  , LEN_LABEL);
   if (a_point  != NULL)  *a_point  = s_font_info [s_curr].point;
   if (a_adjust != NULL)  *a_adjust = s_font_info [s_curr].adjust;
   if (a_spacer != NULL)  *a_spacer = s_font_info [s_curr].spacer;
   if (a_glist  != NULL)  ystrlcpy (a_glist , s_font_info [s_curr].glist , LEN_LABEL);
   if (a_source != NULL)  ystrlcpy (a_source, s_font_info [s_curr].source, LEN_STR  );
   return 0;
}

char         /*--> load the header from configuration ----[ ------ [ ------ ]-*/
yFONT__conf_info     (char *a_name, char *a_type, char *a_point, char *a_adjust, int *a_spacer, char *a_glist, char *a_source)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   int         i           =   0;
   int         x_len       =   0;
   int         x_entry     =  -1;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense : content)--------------*/
   DEBUG_YFONT_M  yLOG_point   ("*a_name"   , a_name);
   --rce;  if (a_name == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("name can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_info    ("a_name"    , a_name);
   x_len = ystrllen (a_name, LEN_LABEL);
   DEBUG_YFONT_M  yLOG_value   ("x_len"     , x_len);
   --rce;  if (x_len <= 0 || x_len >= LEN_LABEL) {
      DEBUG_YFONT_M  yLOG_warn    ("name length not in required range (0 - LEN_LABEL)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(find the font)------------------*/
   for (i = 0; i < s_nentry; ++i) {
      if (a_name [0] != s_font_info [i].name [0])          continue;
      if (strcmp (a_name, s_font_info [i].name) != 0)      continue;
      if (a_type   != NULL)  ystrlcpy (a_type  , s_font_info [s_curr].type  , LEN_LABEL);
      if (a_point  != NULL)  *a_point  = s_font_info [i].point;
      if (a_adjust != NULL)  *a_adjust = s_font_info [i].adjust;
      if (a_spacer != NULL)  *a_spacer = s_font_info [i].spacer;
      if (a_glist  != NULL)  ystrlcpy (a_glist , s_font_info [i].glist , LEN_LABEL);
      if (a_source != NULL)  ystrlcpy (a_source, s_font_info [i].source, LEN_STR  );
      x_entry = i;
      s_curr  = i;
      break;
   }
   --rce;  if (x_entry < 0) {
      DEBUG_YFONT_M  yLOG_warn    ("font name not found");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char*        /*--> return the font name ------------------[ leaf   [ ------ ]-*/
yFONT__conf_font     (char a_entry)
{
   if      (a_entry < 0        )  ystrlcpy (s_fontname, "not legal"               , LEN_LABEL);
   else if (a_entry >= s_nentry)  ystrlcpy (s_fontname, "not loaded"              , LEN_LABEL);
   else                           ystrlcpy (s_fontname, s_font_info [a_entry].name, LEN_LABEL);
   return s_fontname;
}



/*====================------------------------------------====================*/
/*===----                     setting header values                    ----===*/
/*====================------------------------------------====================*/
static void      o___SETTERS_________________o (void) {;}

char         /*--> open the configuration file -----------[ leaf   [ ------ ]-*/
yFONT__conf_name     (char *a_name)
{
   if (a_name == NULL)  ystrlcpy (s_confname, FILE_CONF, LEN_STR);
   else                 ystrlcpy (s_confname, a_name   , LEN_STR);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                           reporting                          ----===*/
/*====================------------------------------------====================*/
static void      o___REPORTING_______________o (void) {;}

char         /*--> open the configuration file -----------[ leaf   [ ------ ]-*/
yFONT__conf_list     (void)
{
   int i = 0;
   printf ("\nconfiguration file font list\n");
   for (i = 0; i < s_nentry; ++i) {
      if ((i % 5) == 0)  printf ("-#-  ---category---------  ---name-------------  type  pts  adj  spacer  letter  ---source-------------------------------\n");
      printf ("%3d  %-20.20s  %-20.20s  %-4.4s  %3d  %3d  %6d  %-6.6s  %s\n", i,
            s_font_info [i].cat     , s_font_info [i].name    ,
            s_font_info [i].type    ,
            s_font_info [i].point   , s_font_info [i].adjust  ,
            s_font_info [i].spacer  ,
            s_font_info [i].glist   , s_font_info [i].source  );
   }
   printf ("\n");
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     reading and parsing                      ----===*/
/*====================------------------------------------====================*/
static void      o___PARSING_________________o (void) {;}

char         /*--> open the configuration file -----------[ leaf   [ ------ ]-*/
yFONT__conf_open     (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_info    ("s_confname", s_confname);
   s_conf = fopen (s_confname, "r");
   DEBUG_YFONT_M  yLOG_point   ("s_conf"    , s_conf);
   --rce;  if (s_conf == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("conf file could not be openned");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

char         /*--> close the configuration file ----------[ leaf   [ ------ ]-*/
yFONT__conf_close    (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   char        rc          =  -1;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   if (s_conf == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("conf file already closed");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return 0;
   }
   rc = fclose (s_conf);
   DEBUG_YFONT_M  yLOG_point   ("rc"        , rc);
   --rce;  if (rc != 0) {
      DEBUG_YFONT_M  yLOG_warn    ("conf file could not be closed");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

#define     FIELD_CAT      0
#define     FIELD_NAME     1
#define     FIELD_TYPE     2
#define     FIELD_POINT    3
#define     FIELD_ADJUST   4
#define     FIELD_SPACE    5
#define     FIELD_GLIST    6
#define     FIELD_SOURCE   7

char         /*--> parse the configuration file ----------[ ------ [ ------ ]-*/
yFONT__conf_parse    (void)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;
   char        rc          = 0;
   char        x_recd      [LEN_RECD]  = "";
   int         x_len       =   0;
   int         i           = 0;
   char       *p           = NULL;               /* strtok result             */
   char       *q           = "";               /* strtok delimeters         */
   char       *s           = NULL;               /* strtok context variable   */
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   rc = yFONT__conf_open   ();
   if (rc < 0 ) {
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(read lines)---------------------*/
   while (1) {
      /*---(read and clean)--------------*/
      ++s_lines;
      DEBUG_YFONT_M  yLOG_value   ("line"      , s_lines);
      fgets (x_recd, LEN_RECD, s_conf);
      if (feof (s_conf))  {
         DEBUG_YFONT_M  yLOG_note    ("end of file reached");
         break;
      }
      x_len = strlen (x_recd);
      if (x_len <= 50)  {
         DEBUG_YFONT_M  yLOG_note    ("record empty or incomplete");
         continue;
      }
      x_recd [--x_len] = '\0';
      DEBUG_YFONT_M  yLOG_value   ("length"    , x_len);
      DEBUG_YFONT_M  yLOG_info    ("fixed"     , x_recd);
      if (x_recd [0] == '#') {
         DEBUG_YFONT_M  yLOG_note    ("comment line, skipping");
         continue;
      }
      /*---(read fields)--------------------*/
      p = strtok_r (x_recd, q, &s);
      for (i = FIELD_CAT   ; i <= FIELD_SOURCE ; ++i) {
         /*---(parse field)-----------------*/
         DEBUG_YFONT_M   yLOG_note    ("read field");
         --rce;  if (p == NULL) {
            DEBUG_YFONT_M  yLOG_note    ("strtok_r came up empty");
            DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
            break;
         }
         ystrltrim (p, ySTR_BOTH, LEN_RECD);
         x_len = strlen (p);
         DEBUG_YFONT_M     yLOG_info    ("field"     , p);
         /*---(handle)----------------------*/
         switch (i) {
         case  FIELD_CAT     :
            ystrlcpy (s_font_info [s_nentry].cat   , p, LEN_LABEL);
            DEBUG_YFONT_M  yLOG_info    ("category"  , s_font_info [s_nentry].cat     );
            break;
         case  FIELD_NAME    :
            ystrlcpy (s_font_info [s_nentry].name  , p, LEN_LABEL);
            DEBUG_YFONT_M  yLOG_info    ("name"      , s_font_info [s_nentry].name    );
            break;
         case  FIELD_TYPE    :
            ystrlcpy (s_font_info [s_nentry].type  , p, LEN_LABEL);
            DEBUG_YFONT_M  yLOG_info    ("type"      , s_font_info [s_nentry].type    );
            break;
         case  FIELD_POINT   :
            s_font_info [s_nentry].point   = atoi (p);
            DEBUG_YFONT_M  yLOG_value   ("point"     , s_font_info [s_nentry].point  );
            break;
         case  FIELD_ADJUST  :
            s_font_info [s_nentry].adjust  = atoi (p);
            DEBUG_YFONT_M  yLOG_value   ("adjust"    , s_font_info [s_nentry].adjust );
            break;
         case  FIELD_SPACE   :
            s_font_info [s_nentry].spacer = atoi  (p);
            DEBUG_YFONT_M  yLOG_value   ("spacer"    , s_font_info [s_nentry].spacer  );
            break;
         case  FIELD_GLIST   :
            ystrlcpy (s_font_info [s_nentry].glist , p, LEN_STR  );
            DEBUG_YFONT_M  yLOG_info    ("glist"     , s_font_info [s_nentry].glist   );
            break;
         case  FIELD_SOURCE  :
            ystrlcpy (s_font_info [s_nentry].source, p, LEN_STR  );
            DEBUG_YFONT_M  yLOG_info    ("source"    , s_font_info [s_nentry].source  );
            break;
         }
         DEBUG_YFONT_M  yLOG_note    ("done with loop");
         p = strtok_r (NULL  , q, &s);
      } 
      DEBUG_YFONT_M  yLOG_note    ("done parsing fields");
      ++s_nentry;
   }
   /*---(prepare)------------------------*/
   rc = yFONT__conf_close  ();
   if (rc < 0 ) {
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rc;
   }
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M yLOG_exit    (__FUNCTION__);
   return 0;
}

/*============================----end-of-source---============================*/
