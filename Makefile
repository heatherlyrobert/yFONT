#*============================----beg-of-source---============================*#



#===[[ VARABLES ]]======================================================================================================================================================#

#===(current variables)=================================================================================================================================================#
BASE    = yFONT
DEBUG   = ${BASE}_debug
UNIT    = ${BASE}_unit
#*---(dirs and docs)------------------*#
HDIR    = /home/system/yFONT.opengl_font_renderer
IDIR    = /usr/local/sbin
#*---(library documentation)----------*#
MNUM    = 3
MDIR    = /usr/share/man/man${MNUM}
MBASE   = ${BASE}.${MNUM}
#*---(file format)--------------------*#
FNUM    = 5
FDIR    = /usr/share/man/man${FNUM}
FBASE   = ${BASE}.${FNUM}
#*---(overview)-----------------------*#
ONUM    = 0
ODIR    = /usr/share/man/man${ONUM}
OBASE   = ${BASE}.${ONUM}

#===(compilier variables)===============================================================================================================================================#
# must have "-x c" on gcc line so stripped files work with alternate extensions
COMP    = gcc -c -std=gnu89 -x c -g -pg -Wall -Wextra
INCS    = -I/usr/local/include -I/usr/include/freetype2

#===(linker options)====================================================================================================================================================================================================================================================================================#
#------   (0)-------------- (1)-------------- (2)-------------- (3)-------------- (4)-------------- (5)-------------- (6)-------------- (7)-------------- (8)-------------- (9)-------------- (A)-------------- (B)-------------- (C)-------------- (D)-------------- (E)-------------- (F)--------------
LINK    = gcc
LIBDIR  = -L/usr/local/lib
LIBS    = ${LIBDIR}         -lX11             -lGL              -lm               -lpng             -lySTR            -lyURG            -lyGLTEX
LIBD    = ${LIBDIR}         -lX11             -lGL              -lm               -lpng             -lySTR_debug      -lyURG_debug      -lyGLTEX_debug    -lyLOG
LIBU    = ${LIBDIR}         -lX11             -lGL              -lm               -lpng             -lySTR_debug      -lyURG_debug      -lyGLTEX_debug    -lyLOG            -lyUNIT           -lyVAR

#===(file lists)============================================================================================================================================================================#
#------   (0)-------------- (1)-------------- (2)-------------- (3)-------------- (4)-------------- (5)-------------- (6)-------------- (7)-------------- (8)-------------- (9)-------------- (A)-------------- (B)-------------- (C)-------------- (D)-------------- (5)--------------
HEADS   = ${BASE}.h         ${BASE}_priv.h
OBJS    = ${BASE}_main.os   ${BASE}_conf.os   ${BASE}_slot.os   ${BASE}_head.os   ${BASE}_index.os  ${BASE}_map.os  
OBJD    = ${BASE}_main.o    ${BASE}_conf.o    ${BASE}_slot.o    ${BASE}_head.o    ${BASE}_index.o   ${BASE}_map.o   
OBJU    = ${BASE}_unit.o    ${OBJD}

#===(make variables)====================================================================================================================================================#
COPY    = cp -f
CLEAN   = rm -f
PRINT   = @printf
STRIP   = @grep -v -e " DEBUG_" -e " yLOG_" 



#*---(executables)--------------------*#
all                : ${DEBUG}  ${BASE}  ${UNIT}  ${BASE}_make  ${BASE}_show  ${BASE}_tick

${BASE}            : ${OBJS}
	${LINK}  -shared -Wl,-soname,lib${BASE}.so.1   ${LIBS}  -o lib${BASE}.so.1.0   ${OBJS}
	ar       rcs  lib${BASE}.a   ${OBJS}

${DEBUG}           : ${OBJD}
	${LINK}  -shared -Wl,-soname,lib${DEBUG}.so.1  ${LIBD}  -o lib${DEBUG}.so.1.0  ${OBJD}
	ar       rcs  lib${DEBUG}.a  ${OBJD}

${BASE}_make       : ${BASE}_make.o   ${OBJD}
	${LINK}  -o ${BASE}_make        ${OBJS} ${BASE}_make.os  ${LIBS}  -lfreetype
	${LINK}  -o ${BASE}_make_debug  ${OBJD} ${BASE}_make.o   ${LIBD}  -lfreetype

${BASE}_show       : ${BASE}_show.o   ${OBJD}
	${LINK}  -o ${BASE}_show        ${OBJS} ${BASE}_show.os  ${LIBS}
	${LINK}  -o ${BASE}_show_debug  ${OBJD} ${BASE}_show.o   ${LIBD}

${BASE}_tick       : ${BASE}_tick.o   ${OBJD}
	${LINK}  -o ${BASE}_tick        ${OBJS} ${BASE}_tick.os  ${LIBS}  -lyX11
	${LINK}  -o ${BASE}_tick_debug  ${OBJD} ${BASE}_tick.o   ${LIBD}  -lyX11

${DEBUG}           : ${OBJD}

${UNIT}            : ${OBJU}
	${LINK}  -o ${UNIT}        ${OBJU}   ${LIBU}



#*---(components)---------------------*#

${BASE}_main.o     : ${HEADS}       ${BASE}_main.c
	${COMP}  -fPIC  ${BASE}_main.c                           ${INC}
	${STRIP}        ${BASE}_main.c      > ${BASE}_main.cs
	${COMP}  -fPIC  ${BASE}_main.cs    -o ${BASE}_main.os    ${INC}

${BASE}_conf.o     : ${HEADS}       ${BASE}_conf.c
	${COMP}  -fPIC  ${BASE}_conf.c                           ${INC}
	${STRIP}        ${BASE}_conf.c      > ${BASE}_conf.cs
	${COMP}  -fPIC  ${BASE}_conf.cs    -o ${BASE}_conf.os    ${INC}

${BASE}_slot.o     : ${HEADS}       ${BASE}_slot.c
	${COMP}  -fPIC  ${BASE}_slot.c                           ${INC}
	${STRIP}        ${BASE}_slot.c      > ${BASE}_slot.cs
	${COMP}  -fPIC  ${BASE}_slot.cs    -o ${BASE}_slot.os    ${INC}

${BASE}_file.o     : ${HEADS}       ${BASE}_file.c
	${COMP}  -fPIC  ${BASE}_file.c                           ${INC}
	${STRIP}        ${BASE}_file.c      > ${BASE}_file.cs
	${COMP}  -fPIC  ${BASE}_file.cs    -o ${BASE}_file.os    ${INC}

${BASE}_head.o     : ${HEADS}       ${BASE}_head.c
	${COMP}  -fPIC  ${BASE}_head.c                           ${INC}
	${STRIP}        ${BASE}_head.c      > ${BASE}_head.cs
	${COMP}  -fPIC  ${BASE}_head.cs    -o ${BASE}_head.os    ${INC}

${BASE}_index.o    : ${HEADS}       ${BASE}_index.c
	${COMP}  -fPIC  ${BASE}_index.c                          ${INC}
	${STRIP}        ${BASE}_index.c     > ${BASE}_index.cs
	${COMP}  -fPIC  ${BASE}_index.cs   -o ${BASE}_index.os   ${INC}

${BASE}_map.o      : ${HEADS}       ${BASE}_map.c
	${COMP}  -fPIC  ${BASE}_map.c                            ${INC}
	${STRIP}        ${BASE}_map.c       > ${BASE}_map.cs
	${COMP}  -fPIC  ${BASE}_map.cs     -o ${BASE}_map.os     ${INC}

${BASE}_make.o     : ${HEADS}       ${BASE}_make.c        
	${COMP}  -fPIC  ${BASE}_make.c                           ${INC} -I/usr/include/freetype2
	${STRIP}        ${BASE}_make.c      > ${BASE}_make.cs
	${COMP}  -fPIC  ${BASE}_make.cs    -o ${BASE}_make.os    ${INC} -I/usr/include/freetype2

${BASE}_show.o     : ${HEADS}       ${BASE}_show.c        
	${COMP}  -fPIC  ${BASE}_show.c                           ${INC}
	${STRIP}        ${BASE}_show.c      > ${BASE}_show.cs
	${COMP}  -fPIC  ${BASE}_show.cs    -o ${BASE}_show.os    ${INC}

${BASE}_tick.o     : ${HEADS}       ${BASE}_tick.c        
	${COMP}  -fPIC  ${BASE}_tick.c                           ${INC}
	${STRIP}        ${BASE}_tick.c      > ${BASE}_tick.cs
	${COMP}  -fPIC  ${BASE}_tick.cs    -o ${BASE}_tick.os    ${INC}

${UNIT}.o          : ${HEADS} ${BASE}.unit
	koios    ${BASE}
	${COMP}  ${UNIT}.c  ${INCS}



#*---(housecleaning)------------------*#
clean              :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "cleaning out local object, backup, and temp files\n"
	${CLEAN} lib${BASE}.so.1
	${CLEAN} lib${BASE}.so.1.0
	${CLEAN} lib${DEBUG}.so.1
	${CLEAN} lib${DEBUG}.so.1.0
	${CLEAN} *.o
	${CLEAN} *.cs
	${CLEAN} *.os
	${CLEAN} *~
	${CLEAN} temp*
	${CLEAN} ${BASE}_unit.c
	${CLEAN}  txf_make
	${CLEAN}  txf_show

bigclean           :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "clean out local swap files\n"
	${CLEAN}  '.'*.sw?

remove             :
	${PRINT}  "\n--------------------------------------\n"
	${PRINT}  "remove ${BASE} from production\n"
	_lib      -d  ${BASE}
	_lib      -d  ${DEBUG}
	ldconfig

install            :
	#---(production version)--------------#
	sha1sum   lib${BASE}.so.1.0
	_lib      -s ${BASE}
	ldconfig
	sha1sum   lib${BASE}.a
	_lib      -a ${BASE}
	ldconfig
	#---(debug version)-------------------#
	sha1sum   lib${DEBUG}.so.1.0
	_lib      -S ${DEBUG}
	ldconfig
	sha1sum   lib${DEBUG}.a
	_lib      -A ${DEBUG}
	ldconfig
	#---(production version)--------------#
	${COPY}   ${BASE}_make         ${IDIR}/
	chown     root:root            ${IDIR}/${BASE}_make
	chmod     0755                 ${IDIR}/${BASE}_make
	@sha1sum  ${BASE}_make
	#---(debug version)-------------------#
	${COPY}   ${BASE}_make_debug   ${IDIR}/
	chown     root:root            ${IDIR}/${BASE}_make_debug
	chmod     0755                 ${IDIR}/${BASE}_make_debug
	@sha1sum  ${BASE}_make_debug
	#---(production version)--------------#
	${COPY}   ${BASE}_show         ${IDIR}/
	chown     root:root            ${IDIR}/${BASE}_show
	chmod     0755                 ${IDIR}/${BASE}_show
	@sha1sum  ${BASE}_show
	#---(debug version)-------------------#
	${COPY}   ${BASE}_show_debug   ${IDIR}/
	chown     root:root            ${IDIR}/${BASE}_show_debug
	chmod     0755                 ${IDIR}/${BASE}_show_debug
	@sha1sum  ${BASE}_show_debug
	#---(production version)--------------#
	${COPY}   ${BASE}_tick         ${IDIR}/
	chown     root:root            ${IDIR}/${BASE}_tick
	chmod     0755                 ${IDIR}/${BASE}_tick
	@sha1sum  ${BASE}_tick
	#---(debug version)-------------------#
	${COPY}   ${BASE}_tick_debug   ${IDIR}/
	chown     root:root            ${IDIR}/${BASE}_tick_debug
	chmod     0755                 ${IDIR}/${BASE}_tick_debug
	@sha1sum  ${BASE}_tick_debug
	#---(overview)------------------------#
	rm -f     ${MDIR}/${MBASE}.bz2
	cp -f     ${MBASE}    ${MDIR}
	bzip2     ${MDIR}/${MBASE}
	chmod     0644  ${MDIR}/${MBASE}.bz2
	#---(documentation)-------------------#
	rm -f     ${MDIR}/${MBASE}.bz2
	cp -f     ${MBASE}    ${MDIR}
	bzip2     ${MDIR}/${MBASE}
	chmod     0644  ${MDIR}/${MBASE}.bz2
	#---(file format)---------------------#
	rm -f     ${FDIR}/${FBASE}.bz2
	cp -f     ${FBASE}    ${FDIR}
	bzip2     ${FDIR}/${FBASE}
	chmod     0644  ${FDIR}/${FBASE}.bz2
	#---(overview)------------------------#
	rm -f     ${ODIR}/${OBASE}.bz2
	cp -f     ${OBASE}    ${ODIR}
	bzip2     ${ODIR}/${OBASE}
	chmod     0644  ${ODIR}/${OBASE}.bz2
	#---(done)----------------------------#


#*============================----end-of-source---============================*#
