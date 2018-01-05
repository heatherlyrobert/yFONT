/*============================----beg-of-source---============================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include   <png.h>                     /* to handle avatars                   */


/*---(headers)---------------------------*/
#include    "yFONT.h"
#include    "yFONT_priv.h"




int     nfont     =   0;
GLuint  syms      =   0;
float   s_nrow    =   0;
float   s_ncol    =   0;
char    s_type    = '-';



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

#define     MAX_ICONS    1000
struct cICON {
   char          cat       [10];
   int           catno;
   char          abbr      [10];
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
   { "play"     , 0, "stepnext"       , "" },
   { "play"     , 0, "stepprev"       , "" },
   { "play"     , 0, "load"           , "" },
   { "play"     , 0, "eject"          , "" },
   { "play"     , 0, "skipback"       , "" },
   { "play"     , 0, "skipfore"       , "" },
   { "play"     , 0, "repeat"         , "" },
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
   /*---(player)------------------------*/
   { "power"    , 0, "empty"          , "" },







   { "sound"    , 0, "mute"           , "" },
   { "sound"    , 0, "low"            , "" },
   { "sound"    , 0, "med"            , "" },
   { "sound"    , 0, "high"           , "" },
   { "sound"    , 0, "none"           , "" },

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
   snprintf (yFONT_ver, 100, "%s   %s : %s", t, YFONT_VER_NUM, YFONT_VER_TXT);
   return yFONT_ver;
}

char               /* PURPOSE : load a font from a file ----------------------*/
yFONT__list        (void)
{
   int i = 0;
   for (i = 0; i < MAX_FONT; ++i) {
      if (g_yfont [i] == NULL)  printf ("%02d  NULL        empty\n", i);
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
yFONT_print          (char a_slot, char a_size, char a_align, char *a_text)
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
      DEBUG_YFONT_M  yLOG_warn    ("a_slot"    , "out of accepable range (0 to MAX_FONT)");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   x_font    = g_yfont [a_slot];
   DEBUG_YFONT_M  yLOG_point   ("x_font"    , x_font);
   --rce;  if (x_font == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("x_font"    , "slot can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_point   ("*a_text"   , a_text);
   --rce;  if (a_text == NULL) {
      DEBUG_YFONT_M  yLOG_warn    ("text"      , "can not be null");
      DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
      return rce;
   }
   DEBUG_YFONT_M  yLOG_info    ("a_text"    , a_text);
   x_len     = strllen(a_text, LEN_STR);
   DEBUG_YFONT_M  yLOG_value   ("x_len"     , x_len);
   /*---(place start)--------------------*/
   w = yFONT__index_width  (x_font, a_text, x_len);
   DEBUG_YFONT_M  yLOG_value   ("w"         , w);
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
   x_scale   = (float) a_size / (float) x_font->point;
   DEBUG_YFONT_M  yLOG_double  ("x_scale"   , x_scale);
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
   DEBUG_YFONT_M  yLOG_exit    (__FUNCTION__);
   return w * x_scale;
}

int                /* PURPOSE : print a string with word wrapping             */
yFONT_printw         (char a_slot, char a_point, char a_align, char *a_text, int a_width, int a_height, float a_spacing)
{
   /*---(defense)-------------------------------*/
   if (a_slot < 0 || a_slot >= MAX_FONT)  return YF_BAD_SLOT;
   tYFONT  *x_font = g_yfont [a_slot];
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
   return yFONT_symload (ICON_SET, 20, 38, 'i');
}

int
yFONT_icon         (char *a_cat, char *a_name, int a_side)
{
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
   --rce;  if (a_side <  10  )  return rce;
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
   /*---(request)------------------------*/
   x_scale = a_side / 20.0;
   x_row   = x_icon / 20;
   x_col   = x_icon - (x_row * 20);
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
         case  1  : glColor4f (1.0f, 0.0f, 0.0f, 0.2f); break; /* a */
         case  2  : glColor4f (0.8f, 0.8f, 0.0f, 0.2f); break; /* e */
         case  3  : glColor4f (0.0f, 1.0f, 0.0f, 0.2f); break; /* f */
         case  4  : glColor4f (0.0f, 0.8f, 0.8f, 0.2f); break; /* i */
         case  5  : glColor4f (0.0f, 0.0f, 1.0f, 0.2f); break; /* o */
         case  6  : glColor4f (0.8f, 0.0f, 0.8f, 0.2f); break; /* u */
         default  : glColor4f (0.8f, 0.8f, 0.8f, 0.2f); break;
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
   DEBUG_TOPS   yLOG_end     ();
   return 0;
}

/*============================----end-of-source---============================*/
