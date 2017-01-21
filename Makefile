#*============================----beg-of-source---============================*#



#===[[ VARABLES ]]======================================================================================================================================================#

#===(current variables)=================================================================================================================================================#
BASE    = yFONT
DEBUG   = ${BASE}_debug
UNIT    = ${BASE}_unit
#*---(dirs and docs)------------------*#
HDIR    = /home/system/yFONT.opengl_font_renderer
IDIR    = /usr/local/sbin
MNUM    = 3
MDIR    = /usr/share/man/man${MNUM}
MBASE   = ${BASE}.${MNUM}
FNUM    = 5
FDIR    = /usr/share/man/man${FNUM}
FBASE   = ${BASE}.${FNUM}

#===(compilier variables)===============================================================================================================================================#
# must have "-x c" on gcc line so stripped files work with alternate extensions
COMP    = gcc -c -std=gnu89 -x c -g -pg -Wall -Wextra
INCS    = -I/usr/local/include -I/usr/include/freetype2

#===(linker options)====================================================================================================================================================================================================================================================================================#
#------   (0)-------------- (1)-------------- (2)-------------- (3)-------------- (4)-------------- (5)-------------- (6)-------------- (7)-------------- (8)-------------- (9)-------------- (A)-------------- (B)-------------- (C)-------------- (D)-------------- (E)-------------- (F)--------------
LINK    = gcc
LIBDIR  = -L/usr/local/lib
LIBS    = ${LIBDIR}         -lX11             -lGL              -lm               -lpng             -lySTR            -lyURG
LIBD    = ${LIBDIR}         -lX11             -lGL              -lm               -lpng             -lySTR_debug      -lyURG_debug      -lyLOG
LIBU    = ${LIBDIR}         -lX11             -lGL              -lm               -lpng             -lySTR_debug      -lyURG_debug      -lyLOG            -lyUNIT           -lyVAR

#===(file lists)============================================================================================================================================================================#
#------   (0)-------------- (1)-------------- (2)-------------- (3)-------------- (4)-------------- (5)-------------- (6)-------------- (7)-------------- (8)-------------- (9)-------------- (A)-------------- (B)-------------- (C)-------------- (D)-------------- (5)--------------
HEADS   = ${BASE}.h         ${BASE}_priv.h
OBJS    = ${BASE}_main.os   ${BASE}_conf.os   ${BASE}_slot.os   ${BASE}_file.os   ${BASE}_head.os   ${BASE}_index.os  ${BASE}_glyph.os  
OBJD    = ${BASE}_main.o    ${BASE}_conf.o    ${BASE}_slot.o    ${BASE}_file.o    ${BASE}_head.o    ${BASE}_index.o   ${BASE}_glyph.o   
OBJU    = ${BASE}_unit.o    ${OBJD}

#===(make variables)====================================================================================================================================================#
COPY    = cp -f
CLEAN   = rm -f
PRINT   = @printf
STRIP   = @grep -v -e " DEBUG_" -e " yLOG_" 




#*---(executables)--------------------*#
#all                : ${DEBUG} ${BASE} ${UNIT} txf_make txf_show
all                : ${DEBUG} ${BASE} ${UNIT}  txf_make

${BASE}            : ${OBJS}
	${LINK}  -shared -Wl,-soname,lib${BASE}.so.1   ${LIBS}  -o lib${BASE}.so.1.0   ${OBJS}
	ar       rcs  lib${BASE}.a   ${OBJS}

${DEBUG}           : ${OBJD}
	${LINK}  -shared -Wl,-soname,lib${DEBUG}.so.1  ${LIBD}  -o lib${DEBUG}.so.1.0  ${OBJD}
	ar       rcs  lib${DEBUG}.a  ${OBJD}

txf_make           : txf_make.o   ${OBJD}
	${LINK}  -o txf_make        ${OBJS} txf_make.os  ${LIBS}  -lfreetype
	${LINK}  -o txf_make_debug  ${OBJD} txf_make.o   ${LIBD}  -lfreetype

${DEBUG}           : ${OBJD}

${UNIT}            : ${OBJU}
	${LINK}  -o ${UNIT}        ${OBJU}   ${LIBU}


#all                : gcc txf_make doc txf_show

#gcc                : ${BASE}.h ${BASE}.c
#	${COMP}   -fPIC ${INCS} ${BASE}.c
#	${LINK}   -shared -Wl,-soname,lib${BASE}.so.1  ${LIBS}  -o lib${BASE}.so.1.0  ${BASE}.o

#tcc                : ${BASE}.h ${BASE}.c
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "compile yFONT (tcc)\n"
#	tcc       -Wall -Wunsupported -g -bench -shared -soname lib${BASE}.so.1  ${LIBS_T}  -o lib${BASE}.so.1.0  ${BASE}.c

#txf_make           : txf_make.c
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "compile txf_make (tcc)\n"
#	gcc       -g -ansi -Wall -Wextra -o txf_make   txf_make.c ${INCS} -I/usr/include/freetype2 ${LIBS_T} -lfreetype
#	${PRINT}  "\n--------------------------------------\n"
#	${PRINT}  "install txf_make and txf_inst\n"
#	${COPY}   txf_make    /usr/local/sbin/
#	${COPY}   txf_inst.sh /usr/local/sbin/txf_inst
#	chmod     0755        /usr/local/sbin/txf_inst

#txf_show           : yFONT.c yFONT.h txf_show.c
#	${COMP}   txf_show.c
#	${LINK}   -o txf_show yFONT.c txf_show.o ${LIBS}
#	${COPY}   txf_show    /usr/local/sbin/

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

${BASE}_glyph.o    : ${HEADS}       ${BASE}_glyph.c
	${COMP}  -fPIC  ${BASE}_glyph.c                          ${INC}
	${STRIP}        ${BASE}_glyph.c     > ${BASE}_glyph.cs
	${COMP}  -fPIC  ${BASE}_glyph.cs   -o ${BASE}_glyph.os   ${INC}

txf_make.o         : ${HEADS}       txf_make.c        
	${COMP}  -fPIC  txf_make.c                               ${INC} -I/usr/include/freetype2
	${STRIP}        txf_make.c          > txf_make.cs       
	${COMP}  -fPIC  txf_make.cs        -o txf_make.os        ${INC} -I/usr/include/freetype2

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
	${COPY}   txf_make         ${IDIR}/
	chown     root:root        ${IDIR}/txf_make
	chmod     0755             ${IDIR}/txf_make
	@sha1sum  txf_make
	#---(debug version)-------------------#
	${COPY}   txf_make_debug   ${IDIR}/
	chown     root:root        ${IDIR}/txf_make_debug
	chmod     0755             ${IDIR}/txf_make_debug
	@sha1sum  txf_make_debug
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
	#---(done)----------------------------#


#*============================----end-of-source---============================*#
