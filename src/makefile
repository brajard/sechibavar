

#INCDIR="/usr/neuro/local/yao/yao9/yao/include -I/usr/include"
#INCDIR= ${PWD}/include/yao -I/usr/include
INCDIR=${PWD}/include
INCYAO=INCDIR/yao

#Variable pour NetCDF
LIBNC=/usr/lib/libnetcdf_c++.so.5.0.0
INCNC=/usr/include/netcdf-3

SRC=${PWD}/src
OBJ=${PWD}/obj

NCUTIL=ncutil
Ympp= Ymain_sechiba
Ycpp= Ysechiba


CXX= g++ -c
GXX= g++
CXXFLAGS= -Wall -DLINUXPC -Wno-deprecated -Wno-write-strings
INCFLAGS= -I${INCDIR} -I${INCYAO} -I${INCNC}



EXECNAME=sechiba
prj=sechiba

BINMQN= ${PWD}/bin
LDMQN= -L${BINMQN} -lmqn


LDYEXTOBJ= ${PWD}/include/ncutil.o
LDFLAGS= -lm -lgfortran -L/usr/lib64 -lnetcdf


objs = Ymain_sechiba.o Ysechiba.o




.cpp.o :
	${CXX} ${CXXFLAGS} ${PARALLEL} $<

${EXECNAME}: ${objs}
	${CXX} ${CXXFLAGS} ${PARALLEL} -o ${EXECNAME} Ymain_${prj}.o Y${prj}.o ${LDMQN} ${LDYEXTOBJ} ${LDFLAGS}


${OBJ}/${NCUTIL}.o : ${INCDIR}/${NCUTIL}.h ${SRC}/${NCUTIL}.cpp
	${CXX} ${INCFLAGS} ${SRC}/${NCUTIL}.cpp ${CXXFLAGS} -o ${OBJ}/${NCUTIL}.o

${OBJ}/${Ympp}.o : ${SRC}/${Ympp}.cpp
	${CXX} ${INCFLAGS} ${SRC}/${Ympp}.cpp ${CXXFLAGS} -o ${OBJ}/${NCUTIL}.o

${OBJ}/${Ycpp}.o : ${SRC}/${Ycpp}.cpp
	${CXX} ${INCFLAGS} ${SRC}/${Ycpp}.cpp ${CXXFLAGS} -o ${OBJ}/${NCUTIL}.o
#${sources} : ${Ympp} ${Ycpp}
#	${CXX} ${CXXFLAGS} ${PARALLEL} -c $Ympp $Ycpp







clean:
	rm -rf *o ${EXECNAME}




