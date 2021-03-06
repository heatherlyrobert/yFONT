#================================[[ beg-code ]]================================#



#===(manditory)============================================#
# basename of executable, header, and all code files
NAME_BASE  = yFONT
# additional standard and outside libraries
LIB_STD    = -lX11 -lGL -lm -lpng
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lyX11 -lySTR_debug -lyGLTEX_debug
# directory for production code, no trailing slash
INST_DIR   = /usr/local/sbin



#===(optional)=============================================#
# extra include directories required
INC_MINE   = -I/usr/include/freetype2
# utilities generated, separate from main program
NAME_UTIL  = yFONT_make yFONT_show yFONT_tick
# libraries only for the utilities
LIB_UTIL   = -lfreetype



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...
#install_post       :
#remove_post        :



#================================[[ end-code ]]================================#
