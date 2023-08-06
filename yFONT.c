/*============================----beg-of-source---============================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include   <png.h>                     /* to handle avatars                   */


/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"




static int     nfont     =   0;
static GLuint  syms      =   0;
static float   s_nrow    =   0;
static float   s_ncol    =   0;
static char    s_type    = '-';

static int     s_iconcol  =  45;
static int     s_iconrow  =  21;
static int     s_iconside =  20;



/*---(icons)------------------------------------*/
#define     MAX_CATS       50
struct cCAT {
   char          cat       [10];
   char          desc      [50];
   int           start;
   int           count;
} s_cats [MAX_CATS] = {
   { "align"   ,  ""                             ,   0,  0 },
   { "comm"    ,  ""                             ,   0,  0 },
   { "data"    ,  ""                             ,   0,  0 },
   { "draw"    ,  ""                             ,   0,  0 },
   { "games"   ,  ""                             ,   0,  0 },
   { "map"     ,  ""                             ,   0,  0 },
   { "money"   ,  ""                             ,   0,  0 },
   { "play"    ,  ""                             ,   0,  0 },
   { "power"   ,  ""                             ,   0,  0 },
   { "sci"     ,  ""                             ,   0,  0 },
   { "sec"     ,  ""                             ,   0,  0 },
   { "talk"    ,  ""                             ,   0,  0 },
   { "tech"    ,  ""                             ,   0,  0 },
   { "tools"   ,  ""                             ,   0,  0 },
   { "touch"   ,  ""                             ,   0,  0 },
   { ""        ,  ""                             ,   0,  0 },
};

#define     MAX_ICONS    2000
struct cICON {
   char          cat       [10];
   int           catno;
   char          abbr      [20];
   char          desc      [50];
} s_icons [MAX_ICONS] = {
   /*---(align)--------------------------*/
   { "align"    , 0, "top"            , "" },
   { "align"    , 0, "bottom"         , "" },
   { "align"    , 0, "middle"         , "" },
   { "align"    , 0, "horz"           , "" },
   { "align"    , 0, "left"           , "" },
   { "align"    , 0, "right"          , "" },
   { "align"    , 0, "center"         , "" },
   { "align"    , 0, "vert"           , "" },
   { "align"    , 0, "front"          , "" },
   { "align"    , 0, "forward"        , "" },
   { "align"    , 0, "backward"       , "" },
   { "align"    , 0, "back"           , "" },
   { "align"    , 0, "fliphorz"       , "" },
   { "align"    , 0, "flipvert"       , "" },
   { "align"    , 0, "rotleft"        , "" },
   { "align"    , 0, "rotright"       , "" },
   { "align"    , 0, "rotleft2"       , "" },
   { "align"    , 0, "rotright2"      , "" },
   { "align"    , 0, "group"          , "" },
   { "align"    , 0, "ungroup"        , "" },
   { "align"    , 0, "select"         , "" },
   { "align"    , 0, "layers"         , "" },
   { "align"    , 0, "chevleft"       , "" },
   { "align"    , 0, "chevright"      , "" },
   { "align"    , 0, "chevup"         , "" },
   { "align"    , 0, "chevdown"       , "" },
   /*---(comm)---------------------------*/
   { "comm"     , 0, "wifi_on"        , "" },
   { "comm"     , 0, "wifi_lock"      , "" },
   { "comm"     , 0, "wifi_off"       , "" },
   { "comm"     , 0, "gps_on"         , "" },
   { "comm"     , 0, "gps_off"        , "" },
   { "comm"     , 0, "gps_talk"       , "" },
   { "comm"     , 0, "orbit"          , "" },
   { "comm"     , 0, "sat"            , "" },
   { "comm"     , 0, "sat_loc"        , "" },
   { "comm"     , 0, "sat_on"         , "" },
   { "comm"     , 0, "cable_on"       , "" },
   { "comm"     , 0, "cable_off"      , "" },
   { "comm"     , 0, "data_none"      , "" },
   { "comm"     , 0, "data_in"        , "" },
   { "comm"     , 0, "data_out"       , "" },
   { "comm"     , 0, "data_both"      , "" },
   { "comm"     , 0, "phone"          , "" },
   { "comm"     , 0, "phone_miss"     , "" },
   { "comm"     , 0, "phone_callback" , "" },
   { "comm"     , 0, "phone_transfer" , "" },
   { "comm"     , 0, "phone_msg"      , "" },
   { "comm"     , 0, "phone_on"       , "" },
   { "comm"     , 0, "phone_end"      , "" },
   { "comm"     , 0, "radio"          , "" },
   { "comm"     , 0, "cell"           , "" },
   { "comm"     , 0, "bluetooth"      , "" },
   { "comm"     , 0, "rfid"           , "" },
   { "comm"     , 0, "mesh"           , "" },
   { "comm"     , 0, "usb"            , "" },
   { "comm"     , 0, "rs232"          , "" },
   { "comm"     , 0, "eth"            , "" },
   { "comm"     , 0, "snail"          , "" },
   { "comm"     , 0, "sign"           , "" },
   { "comm"     , 0, "qr"             , "" },
   { "comm"     , 0, "block"          , "" },
   { "comm"     , 0, "matrix"         , "" },
   { "comm"     , 0, "chat"           , "" },
   { "comm"     , 0, "comment"        , "" },
   { "comm"     , 0, "speech"         , "" },
   { "comm"     , 0, "thought"        , "" },
   { "comm"     , 0, "dialog"         , "" },
   { "comm"     , 0, "download"       , "" },
   { "comm"     , 0, "upload"         , "" },
   { "comm"     , 0, "clouddown"      , "" },
   { "comm"     , 0, "cloudup"        , "" },
   { "comm"     , 0, "updates"        , "" },
   { "comm"     , 0, "install"        , "" },
   { "comm"     , 0, "sync"           , "" },
   { "comm"     , 0, "voice"          , "" },
   { "comm"     , 0, "mailbox"        , "" },
   { "comm"     , 0, "reload"         , "" },
   { "comm"     , 0, "rollback"       , "" },
   { "comm"     , 0, "restore"        , "" },
   { "comm"     , 0, "progress"       , "" },
   { "comm"     , 0, "indox"          , "" },
   { "comm"     , 0, "outbox"         , "" },
   /*---(data)--------------------------*/
   { "data"     , 0, "sort"           , "" },
   { "data"     , 0, "ascend"         , "" },
   { "data"     , 0, "descend"        , "" },
   { "data"     , 0, "shuffle"        , "" },
   { "data"     , 0, "sheet"          , "" },
   { "data"     , 0, "reorder"        , "" },
   { "data"     , 0, "checklist"      , "" },
   { "data"     , 0, "htree"          , "" },
   { "data"     , 0, "vtree"          , "" },
   { "data"     , 0, "task"           , "" },
   { "data"     , 0, "flow"           , "" },
   { "data"     , 0, "insight"        , "" },
   { "data"     , 0, "mindmap"        , "" },
   { "data"     , 0, "org"            , "" },
   { "data"     , 0, "workflow"       , "" },
   { "data"     , 0, "vertical"       , "" },
   { "data"     , 0, "network"        , "" },
   { "data"     , 0, "scatter"        , "" },
   { "data"     , 0, "multi"          , "" },
   { "data"     , 0, "pipeline"       , "" },
   { "data"     , 0, "cat"            , "" },
   { "data"     , 0, "audio"          , "" },
   { "data"     , 0, "audio2"         , "" },
   { "data"     , 0, "puzzle"         , "" },
   { "data"     , 0, "smooth"         , "" },
   { "data"     , 0, "barbedwire"     , "" },
   { "data"     , 0, "unit"           , "" },
   { "data"     , 0, "storage"        , "" },
   { "data"     , 0, "tourn"          , "" },
   { "data"     , 0, "connect"        , "" },
   { "data"     , 0, "defrag"         , "" },
   { "data"     , 0, "timeline"       , "" },
   { "data"     , 0, "tune"           , "" },
   { "data"     , 0, "stacked"        , "" },
   { "data"     , 0, "thumbnail"      , "" },
   { "data"     , 0, "filter"         , "" },
   { "data"     , 0, "divider"        , "" },
   { "data"     , 0, "split"          , "" },
   { "data"     , 0, "vmerge"         , "" },
   { "data"     , 0, "hmerge"         , "" },
   { "data"     , 0, "parallel"       , "" },
   { "data"     , 0, "point"          , "" },
   { "data"     , 0, "path"           , "" },
   { "data"     , 0, "merge"          , "" },
   { "data"     , 0, "fork"           , "" },
   { "data"     , 0, "find"           , "" },
   { "data"     , 0, "link"           , "" },
   { "data"     , 0, "bookmark"       , "" },
   { "data"     , 0, "star"           , "" },
   { "data"     , 0, "unstart"        , "" },
   { "data"     , 0, "toggle_off"     , "" },
   { "data"     , 0, "toggle_on"      , "" },
   { "data"     , 0, "versions"       , "" },
   { "data"     , 0, "save"           , "" },
   { "data"     , 0, "folder"         , "" },
   { "data"     , 0, "box"            , "" },
   { "data"     , 0, "openbox"        , "" },
   { "data"     , 0, "cabinet"        , "" },
   { "data"     , 0, "trash_empty"    , "" },
   { "data"     , 0, "trash_full"     , "" },
   { "data"     , 0, "infomatics"     , "" },
   { "data"     , 0, "database"       , "" },
   { "data"     , 0, "heatmap"        , "" },
   { "data"     , 0, "candlestick"    , "" },
   { "data"     , 0, "donut"          , "" },
   { "data"     , 0, "plot"           , "" },
   { "data"     , 0, "gantt"          , "" },
   { "data"     , 0, "scatter"        , "" },
   { "data"     , 0, "bar"            , "" },
   { "data"     , 0, "polyline"       , "" },
   { "data"     , 0, "line"           , "" },
   { "data"     , 0, "pie"            , "" },
   { "data"     , 0, "hist"           , "" },
   { "data"     , 0, "combo"          , "" },
   { "data"     , 0, "graph"          , "" },
   { "data"     , 0, "deviation"      , "" },
   { "data"     , 0, "diversity"      , "" },
   { "data"     , 0, "variation"      , "" },
   { "data"     , 0, "cols"           , "" },
   { "data"     , 0, "rows"           , "" },
   { "data"     , 0, "table"          , "" },
   { "data"     , 0, "bell"           , "" },
   { "data"     , 0, "normal"         , "" },
   { "data"     , 0, "sine"           , "" },
   { "data"     , 0, "swap"           , "" },
   { "data"     , 0, "algorithm"      , "" },
   { "data"     , 0, "analyses"       , "" },
   { "data"     , 0, "network"        , "" },
   { "data"     , 0, "structure"      , "" },
   { "data"     , 0, "accountant"     , "" },
   /*---(draw)--------------------------*/
   { "draw"     , 0, "pen"            , "" },
   { "draw"     , 0, "calligraphy"    , "" },
   { "draw"     , 0, "chisel"         , "" },
   { "draw"     , 0, "crayon"         , "" },
   { "draw"     , 0, "dropper"        , "" },
   { "draw"     , 0, "cosmetic"       , "" },
   { "draw"     , 0, "illustrator"    , "" },
   { "draw"     , 0, "eraser"         , "" },
   { "draw"     , 0, "brush"          , "" },
   { "draw"     , 0, "tube"           , "" },
   { "draw"     , 0, "roller"         , "" },
   { "draw"     , 0, "quill"          , "" },
   { "draw"     , 0, "pencil"         , "" },
   { "draw"     , 0, "fill"           , "" },
   { "draw"     , 0, "design"         , "" },
   { "draw"     , 0, "compass"        , "" },
   { "draw"     , 0, "pallette"       , "" },
   { "draw"     , 0, "clipboard"      , "" },
   { "draw"     , 0, "copy"           , "" },
   { "draw"     , 0, "paste"          , "" },
   { "draw"     , 0, "cut"            , "" },
   { "draw"     , 0, "crop"           , "" },
   { "draw"     , 0, "width"          , "" },
   { "draw"     , 0, "height"         , "" },
   { "draw"     , 0, "depth"          , "" },
   { "draw"     , 0, "triangle"       , "" },
   { "draw"     , 0, "square"         , "" },
   { "draw"     , 0, "pentagon"       , "" },
   { "draw"     , 0, "hexagon"        , "" },
   { "draw"     , 0, "octagon"        , "" },
   { "draw"     , 0, "polygon"        , "" },
   { "draw"     , 0, "oval"           , "" },
   { "draw"     , 0, "corner"         , "" },
   { "draw"     , 0, "full"           , "" },
   { "draw"     , 0, "drag"           , "" },
   { "draw"     , 0, "collapse"       , "" },
   { "draw"     , 0, "collage"        , "" },
   { "draw"     , 0, "image"          , "" },
   { "draw"     , 0, "invisible"      , "" },
   { "draw"     , 0, "invert"         , "" },
   { "draw"     , 0, "coordinate"     , "" },
   { "draw"     , 0, "cube"           , "" },
   { "draw"     , 0, "front"          , "" },
   { "draw"     , 0, "side"           , "" },
   { "draw"     , 0, "top"            , "" },
   { "draw"     , 0, "3d"             , "" },
   { "draw"     , 0, "rotate"         , "" },
   { "draw"     , 0, "globe"          , "" },
   { "draw"     , 0, "resize"         , "" },
   { "draw"     , 0, "replace"        , "" },
   { "draw"     , 0, "return"         , "" },
   { "draw"     , 0, "tangent"        , "" },
   { "draw"     , 0, "vector"         , "" },
   { "draw"     , 0, "opacity"        , "" },
   { "draw"     , 0, "undo"           , "" },
   { "draw"     , 0, "redo"           , "" },
   { "draw"     , 0, "sphere"         , "" },
   { "draw"     , 0, "refresh"        , "" },
   { "draw"     , 0, "reset"          , "" },
   { "draw"     , 0, "grid"           , "" },
   { "draw"     , 0, "resolution"     , "" },
   { "draw"     , 0, "full_screen"    , "" },
   { "draw"     , 0, "line_size"      , "" },
   { "draw"     , 0, "metamorphose"   , "" },
   { "draw"     , 0, "editor"         , "" },
   { "draw"     , 0, "select"         , "" },
   { "draw"     , 0, "invert-color"   , "" },
   { "draw"     , 0, "lighting"       , "" },
   { "draw"     , 0, "bucket"         , "" },
   { "draw"     , 0, "video-edit"     , "" },
   { "draw"     , 0, "video-trim"     , "" },
   { "draw"     , 0, "spray"          , "" },
   { "draw"     , 0, "type"           , "" },
   { "draw"     , 0, "pic_in_pic"     , "" },
   { "draw"     , 0, "blueprint"      , "" },
   { "draw"     , 0, "davinci"        , "" },
   { "draw"     , 0, "picasso"        , "" },
   { "draw"     , 0, "sketch"         , "" },
   { "draw"     , 0, "brushes"        , "" },
   { "draw"     , 0, "resize"         , "" },
   { "draw"     , 0, "axis"           , "" },
   { "draw"     , 0, "layout"         , "" },
   { "draw"     , 0, "texteditor"     , "" },
   { "draw"     , 0, "set_square"     , "" },
   { "draw"     , 0, "pencil_case"    , "" },
   { "draw"     , 0, "shapes"         , "" },
   /*---(games)-------------------------*/
   { "games"    , 0, "kingw"          , "" },
   { "games"    , 0, "queenw"         , "" },
   { "games"    , 0, "bishopw"        , "" },
   { "games"    , 0, "knightw"        , "" },
   { "games"    , 0, "rookw"          , "" },
   { "games"    , 0, "pawnw"          , "" },
   { "games"    , 0, "kingb"          , "" },
   { "games"    , 0, "queenb"         , "" },
   { "games"    , 0, "bishopb"        , "" },
   { "games"    , 0, "knightb"        , "" },
   { "games"    , 0, "rookb"          , "" },
   { "games"    , 0, "pawnb"          , "" },
   { "games"    , 0, "8die"           , "" },
   { "games"    , 0, "10die"          , "" },
   { "games"    , 0, "12die"          , "" },
   { "games"    , 0, "20die"          , "" },
   { "games"    , 0, "ball"           , "" },
   { "games"    , 0, "rubic"          , "" },
   { "games"    , 0, "strategy"       , "" },
   /*---(map)---------------------------*/
   { "map"      , 0, "map"            , "" },
   { "map"      , 0, "flag"           , "" },
   { "map"      , 0, "sm_flag"        , "" },
   { "map"      , 0, "finish"         , "" },
   { "map"      , 0, "gps"            , "" },
   { "map"      , 0, "waypoints"      , "" },
   { "map"      , 0, "marker"         , "" },
   { "map"      , 0, "region"         , "" },
   { "map"      , 0, "map_marker"     , "" },
   { "map"      , 0, "signpost"       , "" },
   { "map"      , 0, "obelisk"        , "" },
   { "map"      , 0, "cone"           , "" },
   { "map"      , 0, "lighthouse"     , "" },
   { "map"      , 0, "place"          , "" },
   { "map"      , 0, "adventure"      , "" },
   { "map"      , 0, "windrose"       , "" },
   { "map"      , 0, "lat"            , "" },
   { "map"      , 0, "long"           , "" },
   { "map"      , 0, "left"           , "" },
   { "map"      , 0, "right"          , "" },
   { "map"      , 0, "treasure"       , "" },
   { "map"      , 0, "scenic"         , "" },
   { "map"      , 0, "fireworks"      , "" },
   { "map"      , 0, "streetview"     , "" },
   { "map"      , 0, "enter"          , "" },
   { "map"      , 0, "target"         , "" },
   { "map"      , 0, "leave"          , "" },
   { "map"      , 0, "north"          , "" },
   { "map"      , 0, "south"          , "" },
   { "map"      , 0, "west"           , "" },
   { "map"      , 0, "east"           , "" },
   { "map"      , 0, "floorplan"      , "" },
   { "map"      , 0, "stop"           , "" },
   { "map"      , 0, "warning"        , "" },
   { "map"      , 0, "building"       , "" },
   { "map"      , 0, "cathedral"      , "" },
   { "map"      , 0, "factory"        , "" },
   { "map"      , 0, "govt"           , "" },
   { "map"      , 0, "home"           , "" },
   { "map"      , 0, "depot"          , "" },
   { "map"      , 0, "gas"            , "" },
   { "map"      , 0, "light"          , "" },
   { "map"      , 0, "cars"           , "" },
   { "map"      , 0, "tardis"         , "" },
   { "map"      , 0, "cloud"          , "" },
   { "map"      , 0, "clouds"         , "" },
   { "map"      , 0, "windy"          , "" },
   { "map"      , 0, "air"            , "" },
   { "map"      , 0, "rain"           , "" },
   { "map"      , 0, "partly"         , "" },
   { "map"      , 0, "wet"            , "" },
   { "map"      , 0, "water"          , "" },
   { "map"      , 0, "drop"           , "" },
   { "map"      , 0, "snow"           , "" },
   { "map"      , 0, "summer"         , "" },
   { "map"      , 0, "sun"            , "" },
   { "map"      , 0, "earth"          , "" },
   { "map"      , 0, "pyramids"       , "" },
   { "map"      , 0, "fire"           , "" },
   { "map"      , 0, "tree"           , "" },
   { "map"      , 0, "escalator"      , "" },
   { "map"      , 0, "up"             , "" },
   { "map"      , 0, "down"           , "" },
   { "map"      , 0, "visit"          , "" },
   { "map"      , 0, "tracking"       , "" },
   { "map"      , 0, "tri"            , "" },
   { "map"      , 0, "well"           , "" },
   { "map"      , 0, "oragami"        , "" },
   /*---(money)-------------------------*/
   { "money"    , 0, "chip"           , "" },
   { "money"    , 0, "dollars"        , "" },
   { "money"    , 0, "stack"          , "" },
   { "money"    , 0, "coins"          , "" },
   { "money"    , 0, "credit"         , "" },
   { "money"    , 0, "magstrip"       , "" },
   { "money"    , 0, "cards"          , "" },
   { "money"    , 0, "dimsum"         , "" },
   { "money"    , 0, "briefcase"      , "" },
   { "money"    , 0, "piggybank"      , "" },
   { "money"    , 0, "calendar"       , "" },
   { "money"    , 0, "sand_empty"     , "" },
   { "money"    , 0, "sand_bottom"    , "" },
   { "money"    , 0, "sand_top"       , "" },
   { "money"    , 0, "clock"          , "" },
   { "money"    , 0, "stopwatch"      , "" },
   { "money"    , 0, "timer"          , "" },
   { "money"    , 0, "past"           , "" },
   { "money"    , 0, "present"        , "" },
   { "money"    , 0, "future"         , "" },
   { "money"    , 0, "span"           , "" },
   { "money"    , 0, "transaction"    , "" },
   { "money"    , 0, "ticket"         , "" },
   { "money"    , 0, "dashboard"      , "" },
   { "money"    , 0, "scales"         , "" },
   { "money"    , 0, "exchange"       , "" },
   { "money"    , 0, "accounting"     , "" },
   { "money"    , 0, "ledger"         , "" },
   { "money"    , 0, "bill"           , "" },
   { "money"    , 0, "invoice"        , "" },
   { "money"    , 0, "paycheck"       , "" },
   { "money"    , 0, "list"           , "" },
   { "money"    , 0, "cashbook"       , "" },
   { "money"    , 0, "day"            , "" },
   { "money"    , 0, "week"           , "" },
   { "money"    , 0, "month"          , "" },
   { "money"    , 0, "timeslider"     , "" },
   { "money"    , 0, "cart"           , "" },
   { "money"    , 0, "cart_full"      , "" },
   { "money"    , 0, "buy"            , "" },
   { "money"    , 0, "checkout"       , "" },
   { "money"    , 0, "return"         , "" },
   { "money"    , 0, "reserve"        , "" },
   { "money"    , 0, "pricetag"       , "" },
   { "money"    , 0, "purse"          , "" },
   { "money"    , 0, "metronome"      , "" },
   { "money"    , 0, "organizer"      , "" },
   /*---(player)------------------------*/
   { "play"     , 0, "first"          , "" },
   { "play"     , 0, "prev"           , "" },
   { "play"     , 0, "next"           , "" },
   { "play"     , 0, "last"           , "" },
   { "play"     , 0, "rewind"         , "" },
   { "play"     , 0, "fastforward"    , "" },
   { "play"     , 0, "slower"         , "" },
   { "play"     , 0, "normal"         , "" },
   { "play"     , 0, "faster"         , "" },
   { "play"     , 0, "play"           , "" },
   { "play"     , 0, "pause"          , "" },
   { "play"     , 0, "stop"           , "" },
   { "play"     , 0, "record"         , "" },
   { "play"     , 0, "resume"         , "" },
   { "play"     , 0, "enter"          , "" },
   { "play"     , 0, "exit"           , "" },
   { "play"     , 0, "jump"           , "" },
   { "play"     , 0, "stepprev"       , "" },
   { "play"     , 0, "stepnext"       , "" },
   { "play"     , 0, "load"           , "" },
   { "play"     , 0, "eject"          , "" },
   { "play"     , 0, "skipback"       , "" },
   { "play"     , 0, "skipfore"       , "" },
   { "play"     , 0, "repeat"         , "" },
   { "play"     , 0, "begtrack"       , "" },
   { "play"     , 0, "playrev"        , "" },
   { "play"     , 0, "endtrack"       , "" },
   { "play"     , 0, "repeatinf"      , "" },
   { "play"     , 0, "dj"             , "" },
   { "play"     , 0, "electronic"     , "" },
   /*---(player)------------------------*/
   { "power"    , 0, "empty"          , "" },
   { "power"    , 0, "low"            , "" },
   { "power"    , 0, "half"           , "" },
   { "power"    , 0, "high"           , "" },
   { "power"    , 0, "full"           , "" },
   { "power"    , 0, "charge"         , "" },
   { "power"    , 0, "none"           , "" },
   { "power"    , 0, "lightning"      , "" },
   { "power"    , 0, "tornado"        , "" },
   { "power"    , 0, "volcano"        , "" },
   { "power"    , 0, "windmill"       , "" },
   { "power"    , 0, "dam"            , "" },
   { "power"    , 0, "plug"           , "" },
   { "power"    , 0, "socket"         , "" },
   { "power"    , 0, "tower"          , "" },
   { "power"    , 0, "meter"          , "" },
   { "power"    , 0, "geoplant"       , "" },
   { "power"    , 0, "hydro"          , "" },
   { "power"    , 0, "nuclear"        , "" },
   { "power"    , 0, "oil"            , "" },
   { "power"    , 0, "power"          , "" },
   { "power"    , 0, "solar"          , "" },
   { "power"    , 0, "storage"        , "" },
   { "power"    , 0, "watertower"     , "" },
   { "power"    , 0, "compressor"     , "" },
   { "power"    , 0, "pump"           , "" },
   { "power"    , 0, "vessel"         , "" },
   { "power"    , 0, "waterfall"      , "" },
   { "power"    , 0, "pipe"           , "" },
   { "power"    , 0, "ship"           , "" },
   { "power"    , 0, "bomb"           , "" },
   { "power"    , 0, "missile"        , "" },
   { "power"    , 0, "rocket"         , "" },
   { "power"    , 0, "truck"          , "" },
   { "power"    , 0, "train"          , "" },
   /*---(science)-----------------------*/
   { "sci"      , 0, "abacus"         , "" },
   { "sci"      , 0, "atmos_pres"     , "" },
   { "sci"      , 0, "brain"          , "" },
   { "sci"      , 0, "cooling"        , "" },
   { "sci"      , 0, "heating"        , "" },
   { "sci"      , 0, "dna"            , "" },
   { "sci"      , 0, "dose"           , "" },
   { "sci"      , 0, "ecg"            , "" },
   { "sci"      , 0, "humidity"       , "" },
   { "sci"      , 0, "hygrometer"     , "" },
   { "sci"      , 0, "pressure"       , "" },
   { "sci"      , 0, "pres_gauge"     , "" },
   { "sci"      , 0, "radar"          , "" },
   { "sci"      , 0, "scale"          , "" },
   { "sci"      , 0, "spyglass"       , "" },
   { "sci"      , 0, "syringe"        , "" },
   { "sci"      , 0, "telescope"      , "" },
   { "sci"      , 0, "temp"           , "" },
   { "sci"      , 0, "testtube"       , "" },
   { "sci"      , 0, "volume"         , "" },
   { "sci"      , 0, "weight"         , "" },
   { "sci"      , 0, "wind"           , "" },
   { "sci"      , 0, "ai"             , "" },
   { "sci"      , 0, "atom"           , "" },
   { "sci"      , 0, "barometer"      , "" },
   { "sci"      , 0, "biohazard"      , "" },
   { "sci"      , 0, "biomass"        , "" },
   { "sci"      , 0, "biotech"        , "" },
   { "sci"      , 0, "brain2"         , "" },
   { "sci"      , 0, "co2"            , "" },
   { "sci"      , 0, "dew"            , "" },
   { "sci"      , 0, "elec_thresh"    , "" },
   { "sci"      , 0, "empty_testtube" , "" },
   { "sci"      , 0, "coolant"        , "" },
   { "sci"      , 0, "experiment"     , "" },
   { "sci"      , 0, "trial"          , "" },
   { "sci"      , 0, "filtration"     , "" },
   { "sci"      , 0, "intelligence"   , "" },
   { "sci"      , 0, "lab_items"      , "" },
   { "sci"      , 0, "math"           , "" },
   { "sci"      , 0, "microscope"     , "" },
   { "sci"      , 0, "mortar"         , "" },
   { "sci"      , 0, "nuclear"        , "" },
   { "sci"      , 0, "opera"          , "" },
   { "sci"      , 0, "pacemaker"      , "" },
   { "sci"      , 0, "physics"        , "" },
   { "sci"      , 0, "pill"           , "" },
   { "sci"      , 0, "radioactive"    , "" },
   { "sci"      , 0, "rain"           , "" },
   { "sci"      , 0, "recycling"      , "" },
   { "sci"      , 0, "stethoscope"    , "" },
   { "sci"      , 0, "beaker"         , "" },
   { "sci"      , 0, "trig"           , "" },
   { "sci"      , 0, "ultrasound"     , "" },
   { "sci"      , 0, "venn"           , "" },
   { "sci"      , 0, "windsock"       , "" },
   { "sci"      , 0, "xray"           , "" },
   { "sci"      , 0, "distilation"    , "" },
   { "sci"      , 0, "laser"          , "" },
   { "sci"      , 0, "microorg"       , "" },
   { "sci"      , 0, "molecule"       , "" },
   { "sci"      , 0, "particle"       , "" },
   { "sci"      , 0, "virus"          , "" },
   { "sci"      , 0, "bacteria"       , "" },
   { "sci"      , 0, "benzine"        , "" },
   { "sci"      , 0, "eukary"         , "" },
   { "sci"      , 0, "plasmid"        , "" },
   { "sci"      , 0, "infinity"       , "" },
   { "sci"      , 0, "unknown"        , "" },
   { "sci"      , 0, "infusion"       , "" },
   { "sci"      , 0, "coronavirus"    , "" },
   { "sci"      , 0, "saucer"         , "" },
   { "sci"      , 0, "urine"          , "" },
   { "sci"      , 0, "spectography"   , "" },
   { "sci"      , 0, "application"    , "" },
   { "sci"      , 0, "three_tubes"    , "" },
   { "sci"      , 0, "sonogram"       , "" },
   { "sci"      , 0, "voltmeter"      , "" },
   { "sci"      , 0, "flasks"         , "" },
   { "sci"      , 0, "molecule"       , "" },
   /*---(security)----------------------*/
   { "sec"      , 0, "key"            , "" },
   { "sec"      , 0, "lock"           , "" },
   { "sec"      , 0, "unlock"         , "" },
   { "sec"      , 0, "password"       , "" },
   { "sec"      , 0, "pincode"        , "" },
   { "sec"      , 0, "passcode"       , "" },
   { "sec"      , 0, "safe"           , "" },
   { "sec"      , 0, "login"          , "" },
   { "sec"      , 0, "logout"         , "" },
   { "sec"      , 0, "fingerprint"    , "" },
   { "sec"      , 0, "fingerscan"     , "" },
   { "sec"      , 0, "palmscan"       , "" },
   { "sec"      , 0, "irisscan"       , "" },
   { "sec"      , 0, "facescan"       , "" },
   { "sec"      , 0, "voicescan"      , "" },
   { "sec"      , 0, "pinpad"         , "" },
   { "sec"      , 0, "bodyscan"       , "" },
   { "sec"      , 0, "metaldetector"  , "" },
   { "sec"      , 0, "siren"          , "" },
   { "sec"      , 0, "horn"           , "" },
   { "sec"      , 0, "bell"           , "" },
   { "sec"      , 0, "alarm"          , "" },
   { "sec"      , 0, "firealarm"      , "" },
   { "sec"      , 0, "alert"          , "" },
   { "sec"      , 0, "bomb"           , "" },
   { "sec"      , 0, "firewall"       , "" },
   { "sec"      , 0, "fire"           , "" },
   { "sec"      , 0, "burglary"       , "" },
   { "sec"      , 0, "poison"         , "" },
   { "sec"      , 0, "ant"            , "" },
   { "sec"      , 0, "bug"            , "" },
   { "sec"      , 0, "cockroach"      , "" },
   { "sec"      , 0, "insect"         , "" },
   { "sec"      , 0, "mite"           , "" },
   { "sec"      , 0, "octopus"        , "" },
   { "sec"      , 0, "spider"         , "" },
   { "sec"      , 0, "web"            , "" },
   { "sec"      , 0, "honeypot"       , "" },
   { "sec"      , 0, "officer"        , "" },
   { "sec"      , 0, "customs"        , "" },
   { "sec"      , 0, "bulletcam"      , "" },
   { "sec"      , 0, "domecam"        , "" },
   { "sec"      , 0, "wallcam"        , "" },
   { "sec"      , 0, "incam"          , "" },
   { "sec"      , 0, "outcam"         , "" },
   { "sec"      , 0, "daycam"         , "" },
   { "sec"      , 0, "nightcam"       , "" },
   { "sec"      , 0, "homealarm"      , "" },
   { "sec"      , 0, "shield"         , "" },
   { "sec"      , 0, "motion"         , "" },
   { "sec"      , 0, "door"           , "" },
   { "sec"      , 0, "proximity"      , "" },
   { "sec"      , 0, "motion2"        , "" },
   { "sec"      , 0, "smoke"          , "" },
   { "sec"      , 0, "light"          , "" },
   { "sec"      , 0, "coil"           , "" },
   { "sec"      , 0, "latch"          , "" },
   { "sec"      , 0, "roadblock"      , "" },
   { "sec"      , 0, "restrict"       , "" },
   { "sec"      , 0, "antivirus"      , "" },
   { "sec"      , 0, "sandbox"        , "" },
   { "sec"      , 0, "skimask"        , "" },
   { "sec"      , 0, "spy"            , "" },
   { "sec"      , 0, "anonymous"      , "" },
   { "sec"      , 0, "fsociety"       , "" },
   { "sec"      , 0, "search"         , "" },
   { "sec"      , 0, "spot"           , "" },
   { "sec"      , 0, "hanglight"      , "" },
   { "sec"      , 0, "stagelight"     , "" },
   { "sec"      , 0, "sheild"         , "" },
   { "sec"      , 0, "law"            , "" },
   { "sec"      , 0, "rules"          , "" },
   { "sec"      , 0, "referee"        , "" },
   { "sec"      , 0, "checkup"        , "" },
   { "sec"      , 0, "police"         , "" },
   { "sec"      , 0, "quiz"           , "" },
   { "sec"      , 0, "choice"         , "" },
   { "sec"      , 0, "passfail"       , "" },
   { "sec"      , 0, "theater"        , "" },
   { "sec"      , 0, "results"        , "" },
   { "sec"      , 0, "center"         , "" },
   { "sec"      , 0, "reflection"     , "" },
   { "sec"      , 0, "secpass"        , "" },
   { "sec"      , 0, "whistle"        , "" },
   { "sec"      , 0, "yellowcard"     , "" },
   { "sec"      , 0, "strongbox"      , "" },
   { "sec"      , 0, "door"           , "" },
   /*---(talk)--------------------------*/
   { "talk"     , 0, "mute"           , "" },
   { "talk"     , 0, "low"            , "" },
   { "talk"     , 0, "med"            , "" },
   { "talk"     , 0, "high"           , "" },
   { "talk"     , 0, "none"           , "" },
   { "talk"     , 0, "micro"          , "" },
   { "talk"     , 0, "microphone"     , "" },
   { "talk"     , 0, "voicemail"      , "" },
   { "talk"     , 0, "projector"      , "" },
   { "talk"     , 0, "movie"          , "" },
   { "talk"     , 0, "bookshelf"      , "" },
   { "talk"     , 0, "bookstrap"      , "" },
   { "talk"     , 0, "openbook"       , "" },
   { "talk"     , 0, "book"           , "" },
   { "talk"     , 0, "books"          , "" },
   { "talk"     , 0, "mag"            , "" },
   { "talk"     , 0, "moleskine"      , "" },
   { "talk"     , 0, "paper"          , "" },
   { "talk"     , 0, "scroll"         , "" },
   { "talk"     , 0, "flippad"        , "" },
   { "talk"     , 0, "compose"        , "" },
   { "talk"     , 0, "class"          , "" },
   { "talk"     , 0, "conf"           , "" },
   { "talk"     , 0, "crowd"          , "" },
   { "talk"     , 0, "racism"         , "" },
   { "talk"     , 0, "staff"          , "" },
   { "talk"     , 0, "users"          , "" },
   { "talk"     , 0, "engineer"       , "" },
   { "talk"     , 0, "family"         , "" },
   { "talk"     , 0, "reading"        , "" },
   { "talk"     , 0, "contacts"       , "" },
   { "talk"     , 0, "frontdesk"      , "" },
   { "talk"     , 0, "training"       , "" },
   { "talk"     , 0, "wrestle"        , "" },
   { "talk"     , 0, "fight"          , "" },
   { "talk"     , 0, "queue"          , "" },
   { "talk"     , 0, "member"         , "" },
   { "talk"     , 0, "group"          , "" },
   { "talk"     , 0, "swimming"       , "" },
   { "talk"     , 0, "treadmill"      , "" },
   { "talk"     , 0, "trekking"       , "" },
   { "talk"     , 0, "meditation"     , "" },
   { "talk"     , 0, "babyfeet"       , "" },
   { "talk"     , 0, "babyprints"     , "" },
   { "talk"     , 0, "bear"           , "" },
   { "talk"     , 0, "cat"            , "" },
   { "talk"     , 0, "dog"            , "" },
   { "talk"     , 0, "feet"           , "" },
   { "talk"     , 0, "leftfoot"       , "" },
   { "talk"     , 0, "rightfoot"      , "" },
   { "talk"     , 0, "shoes"          , "" },
   { "talk"     , 0, "ethics"         , "" },
   { "talk"     , 0, "crystalball"    , "" },
   { "talk"     , 0, "hand"           , "" },
   { "talk"     , 0, "hearing"        , "" },
   { "talk"     , 0, "listen"         , "" },
   { "talk"     , 0, "hearingaid"     , "" },
   { "talk"     , 0, "news"           , "" },
   { "talk"     , 0, "typewriter"     , "" },
   { "talk"     , 0, "report"         , "" },
   { "talk"     , 0, "video_call"     , "" },
   { "talk"     , 0, "presentation"   , "" },
   { "talk"     , 0, "hamradio"       , "" },
   { "talk"     , 0, "video_cam"      , "" },
   { "talk"     , 0, "balloon"        , "" },
   { "talk"     , 0, "ribbon"         , "" },
   { "talk"     , 0, "balloons"       , "" },
   { "talk"     , 0, "megaphone"      , "" },
   { "talk"     , 0, "gopro"          , "" },
   { "talk"     , 0, "clapboard"      , "" },
   { "talk"     , 0, "film"           , "" },
   { "talk"     , 0, "gift"           , "" },
   { "talk"     , 0, "doorhanger"     , "" },
   { "talk"     , 0, "discussion"     , "" },
   { "talk"     , 0, "wallpaper"      , "" },
   { "talk"     , 0, "postits"        , "" },
   { "talk"     , 0, "walker"         , "" },
   { "talk"     , 0, "doa"            , "" },
   { "talk"     , 0, "speaker"        , "" },
   { "talk"     , 0, "brainstorm"     , "" },
   { "talk"     , 0, "qna"            , "" },
   { "talk"     , 0, "user"           , "" },
   /*---(tech)--------------------------*/
   { "tech"     , 0, "desktop"        , "" },
   { "tech"     , 0, "workstation"    , "" },
   { "tech"     , 0, "monitor"        , "" },
   { "tech"     , 0, "server"         , "" },
   { "tech"     , 0, "nas"            , "" },
   { "tech"     , 0, "cd"             , "" },
   { "tech"     , 0, "cd_burn"        , "" },
   { "tech"     , 0, "projector"      , "" },
   { "tech"     , 0, "printer"        , "" },
   { "tech"     , 0, "scanner"        , "" },
   { "tech"     , 0, "panorama"       , "" },
   { "tech"     , 0, "hdd"            , "" },
   { "tech"     , 0, "prototype"      , "" },
   { "tech"     , 0, "form"           , "" },
   { "tech"     , 0, "ic"             , "" },
   { "tech"     , 0, "resistor"       , "" },
   { "tech"     , 0, "stepper"        , "" },
   { "tech"     , 0, "pot"            , "" },
   { "tech"     , 0, "memory"         , "" },
   { "tech"     , 0, "chip"           , "" },
   { "tech"     , 0, "lcd"            , "" },
   { "tech"     , 0, "circuit"        , "" },
   { "tech"     , 0, "microchip"      , "" },
   { "tech"     , 0, "sd"             , "" },
   { "tech"     , 0, "comparitor"     , "" },
   { "tech"     , 0, "sensor"         , "" },
   { "tech"     , 0, "camera"         , "" },
   { "tech"     , 0, "cnc"            , "" },
   { "tech"     , 0, "gyro"           , "" },
   { "tech"     , 0, "menu"           , "" },
   { "tech"     , 0, "more"           , "" },
   { "tech"     , 0, "skeleton"       , "" },
   { "tech"     , 0, "robot"          , "" },
   { "tech"     , 0, "robotic"        , "" },
   { "tech"     , 0, "sf"             , "" },
   { "tech"     , 0, "bot"            , "" },
   { "tech"     , 0, "astronaut"      , "" },
   { "tech"     , 0, "alien"          , "" },
   { "tech"     , 0, "monster"        , "" },
   { "tech"     , 0, "shootingstars"  , "" },
   { "tech"     , 0, "planet"         , "" },
   { "tech"     , 0, "galaxy"         , "" },
   { "tech"     , 0, "astroid"        , "" },
   { "tech"     , 0, "comet"          , "" },
   { "tech"     , 0, "initial_state"  , "" },
   { "tech"     , 0, "active_state"   , "" },
   { "tech"     , 0, "inactive_state" , "" },
   { "tech"     , 0, "states"         , "" },
   { "tech"     , 0, "final_state"    , "" },
   { "tech"     , 0, "connection"     , "" },
   { "tech"     , 0, "home_button"    , "" },
   { "tech"     , 0, "wifi_router"    , "" },
   { "tech"     , 0, "web_cam"        , "" },
   { "tech"     , 0, "hub"            , "" },
   { "tech"     , 0, "switch"         , "" },
   { "tech"     , 0, "router"         , "" },
   { "tech"     , 0, "joystick"       , "" },
   { "tech"     , 0, "stack"          , "" },
   { "tech"     , 0, "settings"       , "" },
   { "tech"     , 0, "drone"          , "" },
   { "tech"     , 0, "glove"          , "" },
   { "tech"     , 0, "ai"             , "" },
   { "tech"     , 0, "hardware"       , "" },
   { "tech"     , 0, "solar_system"   , "" },
   { "tech"     , 0, "windows"        , "" },
   { "tech"     , 0, "vision"         , "" },
   { "tech"     , 0, "menu"           , "" },
   { "tech"     , 0, "dial1"          , "" },
   { "tech"     , 0, "dial2"          , "" },
   /*---(tools)-------------------------*/
   { "tools"    , 0, "chainsaw"       , "" },
   { "tools"    , 0, "drill"          , "" },
   { "tools"    , 0, "mask"           , "" },
   { "tools"    , 0, "fireex"         , "" },
   { "tools"    , 0, "firehouse"      , "" },
   { "tools"    , 0, "flashlight"     , "" },
   { "tools"    , 0, "shears"         , "" },
   { "tools"    , 0, "gasmask"        , "" },
   { "tools"    , 0, "gatling"        , "" },
   { "tools"    , 0, "glue"           , "" },
   { "tools"    , 0, "hammer"         , "" },
   { "tools"    , 0, "maintenance"    , "" },
   { "tools"    , 0, "measure"        , "" },
   { "tools"    , 0, "anvil"          , "" },
   { "tools"    , 0, "nail"           , "" },
   { "tools"    , 0, "barcode_reader" , "" },
   { "tools"    , 0, "needle"         , "" },
   { "tools"    , 0, "pushpin"        , "" },
   { "tools"    , 0, "pipeline"       , "" },
   { "tools"    , 0, "boxcutter"      , "" },
   { "tools"    , 0, "broom"          , "" },
   { "tools"    , 0, "caliper"        , "" },
   { "tools"    , 0, "crowbar"        , "" },
   { "tools"    , 0, "lightsaber"     , "" },
   { "tools"    , 0, "micrometer"     , "" },
   { "tools"    , 0, "mop"            , "" },
   { "tools"    , 0, "rope"           , "" },
   { "tools"    , 0, "saw"            , "" },
   { "tools"    , 0, "serilize"       , "" },
   { "tools"    , 0, "thimble"        , "" },
   { "tools"    , 0, "tweezers"       , "" },
   { "tools"    , 0, "welder"         , "" },
   { "tools"    , 0, "wrench"         , "" },
   { "tools"    , 0, "tireiron"       , "" },
   { "tools"    , 0, "plumbing"       , "" },
   { "tools"    , 0, "pottery"        , "" },
   { "tools"    , 0, "pump"           , "" },
   { "tools"    , 0, "ruler"          , "" },
   { "tools"    , 0, "saw_blade"      , "" },
   { "tools"    , 0, "scalpel"        , "" },
   { "tools"    , 0, "sissorlift"     , "" },
   { "tools"    , 0, "knot"           , "" },
   { "tools"    , 0, "screw"          , "" },
   { "tools"    , 0, "magglass"       , "" },
   { "tools"    , 0, "sewingmachine"  , "" },
   { "tools"    , 0, "ax"             , "" },
   { "tools"    , 0, "soldering"      , "" },
   { "tools"    , 0, "sonic_screw"    , "" },
   { "tools"    , 0, "shovel"         , "" },
   { "tools"    , 0, "stapler"        , "" },
   { "tools"    , 0, "army_knife"     , "" },
   { "tools"    , 0, "sword"          , "" },
   { "tools"    , 0, "tape"           , "" },
   { "tools"    , 0, "tuningfork"     , "" },
   { "tools"    , 0, "waterhose"      , "" },
   { "tools"    , 0, "coil"           , "" },
   { "tools"    , 0, "measuretape"    , "" },
   { "tools"    , 0, "tapemeasure"    , "" },
   { "tools"    , 0, "ascender"       , "" },
   { "tools"    , 0, "carabiner"      , "" },
   { "tools"    , 0, "fence"          , "" },
   { "tools"    , 0, "glasses"        , "" },
   { "tools"    , 0, "ladder"         , "" },
   { "tools"    , 0, "law"            , "" },
   { "tools"    , 0, "lifebouy"       , "" },
   { "tools"    , 0, "rack"           , "" },
   { "tools"    , 0, "sparkplug"      , "" },
   { "tools"    , 0, "settings"       , "" },
   { "tools"    , 0, "engineering"    , "" },
   { "tools"    , 0, "automation"     , "" },
   { "tools"    , 0, "gears"          , "" },
   { "tools"    , 0, "light"          , "" },
   { "tools"    , 0, "lamp"           , "" },
   { "tools"    , 0, "fishing"        , "" },
   { "tools"    , 0, "pinwheel"       , "" },
   { "tools"    , 0, "periscope"      , "" },
   { "tools"    , 0, "eating"         , "" },
   { "tools"    , 0, "paperairplane"  , "" },
   { "tools"    , 0, "stamp"          , "" },
   { "tools"    , 0, "servicebell"    , "" },
   { "tools"    , 0, "sewer"          , "" },
   { "tools"    , 0, "shower"         , "" },
   { "tools"    , 0, "snorkel"        , "" },
   { "tools"    , 0, "towel"          , "" },
   { "tools"    , 0, "tracks"         , "" },
   { "tools"    , 0, "bottle"         , "" },
   { "tools"    , 0, "airgun"         , "" },
   { "tools"    , 0, "machinegun"     , "" },
   { "tools"    , 0, "piston"         , "" },
   { "tools"    , 0, "paintball"      , "" },
   { "tools"    , 0, "pallet"         , "" },
   { "tools"    , 0, "mortar"         , "" },
   { "tools"    , 0, "nightvision"    , "" },
   { "tools"    , 0, "seamine"        , "" },
   { "tools"    , 0, "paperplane"     , "" },
   { "tools"    , 0, "mirror"         , "" },
   { "tools"    , 0, "monocle"        , "" },
   { "tools"    , 0, "landmine"       , "" },
   { "tools"    , 0, "blender"        , "" },
   { "tools"    , 0, "clip"           , "" },
   { "tools"    , 0, "beer"           , "" },
   { "tools"    , 0, "bobbin"         , "" },
   { "tools"    , 0, "dozer"          , "" },
   { "tools"    , 0, "bulletvest"     , "" },
   { "tools"    , 0, "candle"         , "" },
   { "tools"    , 0, "cannon"         , "" },
   { "tools"    , 0, "cleaver"        , "" },
   { "tools"    , 0, "crane"          , "" },
   { "tools"    , 0, "digger"         , "" },
   { "tools"    , 0, "engine"         , "" },
   { "tools"    , 0, "elevator"       , "" },
   { "tools"    , 0, "handgrenade"    , "" },
   { "tools"    , 0, "handgun"        , "" },
   { "tools"    , 0, "hairdryer"      , "" },
   { "tools"    , 0, "shredder"       , "" },
   { "tools"    , 0, "tape"           , "" },
   { "tools"    , 0, "caulk"          , "" },
   { "tools"    , 0, "vise"           , "" },
   { "tools"    , 0, "magnet"         , "" },
   { "tools"    , 0, "washing_machine", "" },
   { "tools"    , 0, "plumber"        , "" },
   /*---(touch)-------------------------*/
   { "touch"    , 0, "left"           , "" },
   { "touch"    , 0, "right"          , "" },
   { "touch"    , 0, "up"             , "" },
   { "touch"    , 0, "down"           , "" },
   { "touch"    , 0, "tap"            , "" },
   { "touch"    , 0, "dtap"           , "" },
   { "touch"    , 0, "dleft"          , "" },
   { "touch"    , 0, "dright"         , "" },
   { "touch"    , 0, "dup"            , "" },
   { "touch"    , 0, "ddown"          , "" },
   { "touch"    , 0, "pinch"          , "" },
   { "touch"    , 0, "spread"         , "" },
   { "touch"    , 0, "twotap"         , "" },
   { "touch"    , 0, "twodtap"        , "" },
   { "touch"    , 0, "cursor"         , "" },
   { "touch"    , 0, "hand"           , "" },
   { "touch"    , 0, "mouse"          , "" },
   { "touch"    , 0, "lclick"         , "" },
   { "touch"    , 0, "rclick"         , "" },
   { "touch"    , 0, "scroll"         , "" },
   { "touch"    , 0, "wacom"          , "" },
   { "touch"    , 0, "keypad"         , "" },
   { "touch"    , 0, "keyboard"       , "" },
   { "touch"    , 0, "phonepad"       , "" },
   /*---(done)---------------------------*/
   { ""         , 0, ""               , "" },
};




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
   snprintf (yFONT_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return yFONT_ver;
}

char               /* PURPOSE : load a font from a file ----------------------*/
yFONT__list        (void)
{
   int i = 0;
   for (i = 0; i < MAX_FONT; ++i) {
      if (g_yfont [i] == NULL) printf ("%02d  NULL        empty\n", i);
      else                     printf ("%02d  %10p  %s\n", i, g_yfont [i], g_yfont[i]->name);
   }
}

char               /* PURPOSE : load a font from a file ----------------------*/
yFONT_load         (char *a_name)
{
   /*---(locals)-----------+-----------+-*/
   char        rce         = -10;           /* return code for errors         */
   int         rc          = 0;                 /* generic return code            */
   char        x_slot      = -1;
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
yFONT_print          (char a_slot, char a_size, char a_align, uchar *a_text)
{
   /*---(locals)-------------------------*/
   char      rce       = -10;
   int       i         = 0;                 /* iterator -- character          */
   tYFONT    *x_font    = NULL;
   int       x_len     = 0;
   float     x_scale   = 0;
   int       w         = 0;
   float     x         = 0;
   float     y         = 0;
   /*---(header)-------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_YFONT_M  yLOG_value   ("a_slot"    , a_slot);
   --rce;  if (a_slot < 0 || a_slot >= MAX_FONT) {
      DEBUG_YFONT_M  yLOG_warn    ("a_slot out of accepable range (0 to MAX_FONT)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font    = g_yfont [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("x_font"    , x_font);
   --rce;  if (x_font == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("x_font slot can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_point   ("*a_text"   , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("text can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_info    ("a_text"    , a_text);
   x_len     = strllen(a_text, LEN_HUGE);
   DEBUG_YFONT_M  yLOG_value   ("x_len"     , x_len);
   /*---(place start)--------------------*/
   w = yFONT__index_width  (x_font, a_text, x_len);
   DEBUG_YFONT_M  yLOG_value   ("w"         , w);
   switch (a_align) {
   /* top */ case  7 : case  8 : case  9 :  y = -x_font->max_ascent;      break;
   /* mid */ case  4 : case  5 : case  6 :  y = -x_font->max_ascent / 2;  break;
   /* bot */ case  1 : case  2 : case  3 :  y =  0;                       break;
   /* bas */ case 10 : case 11 : case 12 :  y = -x_font->max_descent;     break;
   }
   switch (a_align) {
   /* lef */ case  1 : case  4 : case  7 : case 10 :  x =  0;             break;
   /* cen */ case  2 : case  5 : case  8 : case 11 :  x = -w / 2;         break;
   /* cen */ case  3 : case  6 : case  9 : case 12 :  x = -w;             break;
   }
   /*---(draw text)----------------------*/
   x_scale   = (float) a_size / (float) x_font->point;
   DEBUG_YFONT_M  yLOG_double  ("x_scale"   , x_scale);
   glPushMatrix(); {
      glScalef      (x_scale, x_scale, x_scale);
      /*---(draw characters)-------------*/
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBindTexture (GL_TEXTURE_2D, x_font->tex_ref);
      glTranslatef  (x, y, 0);
      for (i = 0; i < x_len; ++i) {
         /*> printf ("a_text [%02d] = %d\n", i, (uchar) a_text [i]);                  <*/
         yFONT__map_glyph  (x_font, (uchar) a_text[i]);
      }
      glBindTexture (GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(complete)-----------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return w * x_scale;
}

float
yFONT_width          (char a_slot, char a_point)
{
   /*---(locals)-----------+-----+-----+-*/
   char       *x_test      = " ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz?.;,!*:\"\'/@#$%^&()_<>{}[]+-=\\|`~";
   tYFONT     *x_font      = NULL;
   tVERT      *x_vert      = NULL;
   float       x_scale     =  0.0;
   int         x_len       =    0;
   int         i           =    0;                 /* iterator -- character          */
   float       cw          =    0;                 /* cum width                      */
   float       rc          =    0;
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   x_font = g_yfont [a_slot];
   if (x_font == NULL)                     return YF_BAD_SLOT;
   x_scale   = (float) a_point / (float) x_font->point;
   x_len     = strllen (x_test, 1000);
   /*> printf ("%2d %2d   %-10p %2d   %5.2f  %3d\n", a_slot, a_point, x_font, x_font->point, x_scale, x_len);   <*/
   for (i = 0; i < x_len; i++) {
      if (x_test [i] == '\0') break;
      x_vert = yFONT__verts_find  (x_font, x_test [i]);
      if (x_vert == NULL) continue;
      cw += x_vert->a * x_scale;
      /*> printf ("%2d   %3d %c   %-10p %3d   %5.2f %5.2f %7.2f\n", i, x_test [i], x_test [i], x_vert, x_vert->a, x_scale, x_vert->a * x_scale, cw);   <*/
   }
   rc = cw / (float) x_len;
   /*> printf ("%7.2f  %3d  %8.2f\n", cw, x_len, rc);                                 <*/
   return rc;
}

int                /* PURPOSE : print a string with word wrapping             */
yFONT_printw         (char a_slot, char a_point, char a_align, uchar *a_text, int a_width, int a_height, float a_spacing)
{
   /*---(locals)--------------------------------*/
   char        rce         =  -10;
   int         i           =    0;                 /* iterator -- character          */
   int         x_len       =    0;               /* string length                  */
   int         s           =    0;                 /* start of next print            */
   int         w           =    0;                 /* current width                  */
   int         l           =    1;                 /* number of lines                */
   int         space       =    0;                 /* last breakable place in text   */
   uchar      *x_str       = NULL;               /* alterable string               */
   float       x_scale     =  0.0;
   int         v           =    0;                 /* vertical spacing               */
   tYFONT     *x_font      = NULL;
   tVERT      *x_vert      = NULL;
   /*---(header)--------------------------------*/
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_complex ("args"      , "%d, %d, %d, %p, %d, %d, %f", a_slot, a_point, a_align, a_text, a_width, a_height, a_spacing);
   /*---(defense)-------------------------------*/
   --rce;  if (a_slot < 0 || a_slot >= MAX_FONT) {
      DEBUG_YFONT_M  yLOG_exitr   (__FUNCTION__, rce);
      return YF_BAD_SLOT;
   }
   x_font = g_yfont [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("x_font"    , x_font);
   --rce;  if (x_font == NULL) {
      DEBUG_YFONT_M  yLOG_exitr   (__FUNCTION__, rce);
      return YF_BAD_SLOT;
   }
   --rce;  if (a_text == NULL) {
      DEBUG_YFONT_M  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_info    ("a_text"    , a_text);
   /*---(prepare)-------------------------------*/
   x_len       = strlen (a_text);
   DEBUG_YFONT_M  yLOG_value   ("x_len"     , x_len);
   x_str     = strdup (a_text);
   x_scale   = (float) a_point / (float) x_font->point;
   v = (x_font->max_ascent - x_font->max_descent) * a_spacing * x_scale;
   --rce;  if (l * v > a_height) {
      DEBUG_YFONT_M  yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  for (i = 0; i < x_len; i++) {
      DEBUG_YFONT_M  yLOG_complex ("char"      , "%3d, %3d, %c", i, x_str [i], x_str [i]);
      if (x_str[i] == '\0') {
         DEBUG_YFONT_M  yLOG_note    ("hit the null/end, break");
         break;
      }
      if (strchr("-/ ", x_str[i]) != NULL) {
         DEBUG_YFONT_M  yLOG_note    ("found a delimiter/space");
         space = i;
      }
      if (x_str [i] == (uchar) '¦') {
         DEBUG_YFONT_M  yLOG_note    ("found new line");
         space = i;
      } else {
         x_vert = yFONT__verts_find  (x_font, x_str[i]);
         DEBUG_YFONT_M  yLOG_point   ("x_vert"    , x_vert);
         if (x_vert == NULL) {
            DEBUG_YFONT_M  yLOG_note    ("null vert, continue");
            continue;
         }
         w += x_vert->a * x_scale;
      }
      DEBUG_YFONT_M  yLOG_complex ("width"     , "%3d adv, %f scale, %3d width, %3d cum, %3d limit", x_vert->a, x_scale, x_vert->a * x_scale, w, a_width);
      if (w > a_width || x_str [i] == (uchar) '¦') {
         DEBUG_YFONT_M  yLOG_note    ("passed width or newline, display");
         x_str [space] = '\0';
         yFONT_print (a_slot, a_point, a_align, &x_str [s]);
         glTranslatef(0.0, -v, 0.0);
         w = 0;
         i = space + 1;
         s = i;
         ++l;
         if (l * v > a_height) {
            DEBUG_YFONT_M  yLOG_exitr   (__FUNCTION__, rce);
            return rce;
         }
      } else {
         DEBUG_YFONT_M  yLOG_note    ("under width limit, get next char");
      }
   }
   DEBUG_YFONT_M  yLOG_complex ("leftover"  , "%3d w, %3d s, %3d len", w, s, x_len);
   /*> if (s != x_len - 1) {                                                          <*/
   if (w > 0) {
      DEBUG_YFONT_M  yLOG_note    ("print final bits");
      yFONT_print (a_slot, a_point, a_align, &x_str[s]);
   } else {
      DEBUG_YFONT_M  yLOG_note    ("nothing left at end to print");
   }
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return a_width;
}

int                /* PURPOSE : print a string -------------------------------*/
yFONT_printu       (char a_slot, char a_point, char a_align, int  *a_array, int a_max)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tYFONT  *x_font = g_yfont [a_slot];
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
   switch (a_align) {
   /* top */ case  7 : case  8 : case  9 :  y = -x_font->max_ascent;      break;
   /* mid */ case  4 : case  5 : case  6 :  y = -x_font->max_ascent / 2;  break;
   /* bot */ case  1 : case  2 : case  3 :  y =  0;                       break;
   /* bas */ case 10 : case 11 : case 12 :  y = -x_font->max_descent;     break;
   }
   switch (a_align) {
   /* lef */ case  1 : case  4 : case  7 : case 10 :  x =  0;             break;
   /* cen */ case  2 : case  5 : case  8 : case 11 :  x = -w / 2;         break;
   /* cen */ case  3 : case  6 : case  9 : case 12 :  x = -w;             break;
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

int
yFONT_iconload     (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         x_cat       =    0;
   int         x_icon      =    0;
   /*---(header)-------------------------*/
   DEBUG_YFONT    yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT    yLOG_value   ("MAX_CATS"  , MAX_CATS);
   /*---(initialize values)--------------*/
   for (x_cat = 0; x_cat < MAX_CATS; ++x_cat) {
      DEBUG_YFONT    yLOG_value   ("x_cat"     , x_cat);
      if (s_cats[x_cat].cat[0]   == 0)                                break;
      DEBUG_YFONT    yLOG_info    ("cat"       , s_cats[x_cat].cat);
      for (x_icon = 0; x_icon < MAX_ICONS; ++x_icon) {
         if (s_icons[x_icon].cat[0] == 0)                             break;
         if (s_icons[x_icon].cat[0] != s_cats[x_cat].cat[0])          continue;
         if (strcmp (s_icons[x_icon].cat, s_cats[x_cat].cat) != 0)    continue;
         DEBUG_YFONT    yLOG_info    ("icon"      , s_icons[x_icon].abbr);
         if (s_cats[x_cat].count == 0)  s_cats[x_cat].start = x_icon;
         ++(s_cats[x_cat].count);
         s_icons[x_icon].catno = x_cat;
      }
      DEBUG_YFONT    yLOG_value   ("start"     , s_cats[x_cat].start);
      DEBUG_YFONT    yLOG_value   ("count"     , s_cats[x_cat].count);
   }
   /*---(load texture)-------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return yFONT_symload (ICON_SET, s_iconcol, s_iconrow, 'i');
}

int
yFONT_icon         (char *a_cat, char *a_name, int a_side)
{
   /*---(design notes)-------------------*/
   /*
    *  calling with a negative side size will act as a find/lookup only to
    *  verify the existance of the icon and return its number.
    *
    */
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   int         x_cat       =   -1;
   int         x_off       =   -1;
   int         x_icon      =   -1;
   float       x_scale     =  1.0;
   int         x_col       =    0;
   int         x_row       =    0;
   /*---(defence)------------------------*/
   --rce;  if (a_cat  == NULL)  return rce;
   --rce;  if (a_name == NULL)  return rce;
   --rce;  if (a_side >= 0 && a_side <  10  )  return rce;
   --rce;  if (a_side >  50  )  return rce;
   /*---(find cat)-----------------------*/
   for (i = 0; i < MAX_CATS; ++i) {
      if (s_cats[i].cat    == NULL)             break;
      if (s_cats[i].cat[0] == NULL)             break;
      if (strcmp (s_cats[i].cat, a_cat) != 0)   continue;
      x_cat = i;
      break;
   }
   --rce;  if (x_cat < 0)       return rce;
   x_off = s_cats[x_cat].start;
   /*---(find icon)----------------------*/
   for (i = x_off; i < MAX_ICONS; ++i) {
      if (s_icons[i].cat     == NULL)              break;
      if (s_icons[i].catno   != x_cat)             break;
      if (s_icons[i].abbr[0] != a_name[0])         continue;
      if (strcmp (s_icons[i].abbr, a_name) != 0)   continue;
      x_icon = i;
      break;
   }
   --rce;  if (x_icon < 0)       return -6;
   /*---(check for find-only)------------*/
   if (a_side < 0)  return x_icon;
   /*---(request)------------------------*/
   x_scale = a_side / (float) s_iconside;
   x_row   = x_icon / s_iconcol;
   x_col   = x_icon - (x_row * s_iconcol);
   /*---(complete)-----------------------*/
   return yFONT_symbol (x_scale, x_col, x_row, 0);
}

int
yFONT_iconno       (int a_icon, int a_side)
{
   /*---(locals)-----------+-----+-----+-*/
   float       x_scale     =  1.0;
   int         x_col       =    0;
   int         x_row       =    0;
   /*---(calc)---------------------------*/
   x_scale = a_side / (float) s_iconside;
   x_row   = a_icon / s_iconcol;
   x_col   = a_icon - (x_row * s_iconcol);
   /*---(complete)-----------------------*/
   return yFONT_symbol (x_scale, x_col, x_row, 0);
}

int                /* PURPOSE : make a png image into a texture --------------*/
yFONT_symload      (char *a_filename, int a_ncol, int a_nrow, char a_type)
{
   /*---(defense)------------------------*/
   syms   = 0;
   syms   = yGLTEX_png2tex (a_filename);
   s_ncol = a_ncol;
   s_nrow = a_nrow;
   s_type = a_type;
   /*---(complete)-----------------------*/
   return syms;
}

char
yFONT__color       (float a_red, float a_grn, float a_blu)
{
    glColor4f (a_red / 0xff, a_grn / 0xff, a_blu / 0xff,  0.3f);
    return 0;
}

int
yFONT_symbol       (float a_scale, int a_col, int a_row, int a_mod)
{
   DEBUG_YFONT_M  yLOG_enter   (__FUNCTION__);
   DEBUG_YFONT_M  yLOG_value   ("a_scale"   , a_scale);
   DEBUG_YFONT_M  yLOG_value   ("a_col"     , a_col);
   DEBUG_YFONT_M  yLOG_value   ("a_row"     , a_row);
   DEBUG_YFONT_M  yLOG_value   ("a_mod"     , a_mod);
   if (syms == 0)  return -1;
   float    x_row [25] = {
      0.9354, 0.8740, 0.8115, 0.7500,
      0.6875, 0.6265, 0.5640, 0.5030,
      0.4410, 0.3790, 0.3180, 0.2555,
      0.1935, 0.1322, 0.0700, 0.0080,
   };
   float    x_col [25] = {
      0.0032, 0.0508, 0.0982, 0.1450,
      0.1920, 0.2390, 0.2870, 0.3340,
      0.3810, 0.4280, 0.4760, 0.5225,
      0.5702, 0.6170, 0.6645, 0.7110,
      0.7585, 0.8060, 0.8530, 0.9000,
      0.9475,
   };
   float    x_width    = 0;
   float    x_height   = 0;
   float    x          = 0;
   float    y          = 0;
   if (s_type == 't') {
      x_width    = 0.0460 - 0.0040;
      x_height   = 0.0547 - 0.0040;
      x          = x_col [a_col] + 0.0020;
      y          = x_row [a_row] + 0.0020;
   } else {
      x_width    = 1.00 / s_ncol;
      x_height   = 1.00 / s_nrow;
      x          = a_col * x_width + (x_width * 0.05);
      y          = ((s_nrow - a_row - 1) * x_height) + (x_height * 0.05);
      x_width   *= 0.9000;
      x_height  *= 0.9000;
   }
   DEBUG_YFONT_M  yLOG_double  ("x_width"   , x_width);
   DEBUG_YFONT_M  yLOG_double  ("x_height"  , x_height);
   DEBUG_YFONT_M  yLOG_double  ("x"         , x);
   DEBUG_YFONT_M  yLOG_double  ("y"         , y);
   /*> printf ("r=%2d, c=%2d, x=%8.3f, y=%8.3f, w=%8.3f, h=%8.3f\n", a_row, a_col, x, y, x_width, x_height);   <*/
   /*---(draw text)-----------------------------*/
   glPushMatrix(); {
      glScalef (a_scale, a_scale, a_scale);
      if (a_mod != 0) {
         switch (a_mod) {
         case  0  : yFONT__color (0xaa, 0x66, 0x44);   break;  /* brown/base  */
         case  1  : yFONT__color (0x00, 0xcc, 0x00);   break;  /* green/mate  */
         case  2  : yFONT__color (0xff, 0x00, 0x00);   break;  /* red/twist   */
         case  3  : yFONT__color (0xaa, 0xaa, 0x00);   break;  /* yellow/kenn */
         case  4  : yFONT__color (0x44, 0xbb, 0xdd);   break;  /* blue/major  */
         case  5  : yFONT__color (0x99, 0x30, 0xee);   break;  /* purple/part */
         case  6  : yFONT__color (0xff, 0x77, 0x00);   break;  /* gold/embody */
         case  7  : yFONT__color (0xee, 0x22, 0x99);   break;  /* crimson/fut */
         default  : yFONT__color (0xff, 0xff, 0xff);   break;  /* white/error */
         }
         glBegin  (GL_POLYGON); {
            glVertex3f   (    0.0, -20.0,   0.0);
            glVertex3f   (   20.0, -20.0,   0.0);
            glVertex3f   (   20.0,   0.0,   0.0);
            glVertex3f   (    0.0,   0.0,   0.0);
         } glEnd();
      }
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glBindTexture(GL_TEXTURE_2D, syms);
      glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
      glBegin  (GL_POLYGON); {
         glTexCoord2f ( x          , y);
         glVertex3f   (    0.0, -20.0,   0.0);
         glTexCoord2f ( x + x_width, y);
         glVertex3f   (   20.0, -20.0,   0.0);
         glTexCoord2f ( x + x_width, y + x_height);
         glVertex3f   (   20.0,   0.0,   0.0);
         glTexCoord2f ( x          , y + x_height);
         glVertex3f   (    0.0,   0.0,   0.0);
      } glEnd();
      glBindTexture(GL_TEXTURE_2D, 0);
   } glPopMatrix();
   /*---(complete)------------------------------*/
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return 0;
}

/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

#define       LEN_TEXT  2000
char          unit_answer [ LEN_TEXT ];

char*            /* [------] unit test accessor ------------------------------*/
yFONT__unit        (char *a_question, int a_num)
{
   tYFONT      *x_font      = NULL;              /* new font                       */
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "yFONT_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if        (strncmp(a_question, "font_name"   , 20)  == 0) {
      x_font = g_yfont [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT font name  : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT font name  : %2d:%-20.20s     point=%2d", strllen (x_font->name, LEN_LABEL), x_font->name, x_font->point);
   } else if (strncmp(a_question, "tex_size"    , 20)  == 0) {
      x_font = g_yfont [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT tex size   : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT tex size   : width =%4d, height=%4d" , x_font->tex_w, x_font->tex_h);
   } else if (strncmp(a_question, "char_limits" , 20)  == 0) {
      x_font = g_yfont [a_num];
      if (x_font == NULL) snprintf (unit_answer, LEN_TEXT, "yFONT char limit : no font assigned");
      else                snprintf (unit_answer, LEN_TEXT, "yFONT char limit : mascent =%3d, mdescent=%3d, margin  =%3d" , x_font->max_ascent, x_font->max_descent, x_font->margin);
   } else if (strncmp(a_question, "num_glyph"   , 20)  == 0) {
      x_font = g_yfont [a_num];
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
   DEBUG_YFONT   yLOGS_end     ();
   return 0;
}

/*============================----end-of-source---============================*/
