WORKDIR=


#INCDIR="/usr/neuro/local/yao/yao9/yao/include -I/usr/include"
#INCDIR= ${PWD}/include/yao -I/usr/include
INCDIR=${PWD}/include
INCYAO=${INCDIR}/yao

#Variable pour NetCDF
LIBNC=/usr/lib/libnetcdf_c++.so.5.0.0
INCNC=/usr/include/netcdf-3

SRC=${PWD}/src
OBJ=${PWD}/obj
BIN=${PWD}/bin

NCUTIL=ncutil
Ympp= Ymain_sechiba
Ycpp= Ysechiba


CXX= g++ 
CXXFLAGS= -g -c -Wall -DLINUXPC -Wno-deprecated -Wno-write-strings
INCFLAGS= -I${INCDIR} -I${INCYAO} -I${INCNC} -I. 



EXECNAME=sechiba
prj=sechiba

DIRMQN= ${PWD}/lib
LDMQN= -L${DIRMQN} -lmqn


LDFLAGS= -lm -lgfortran -L/usr/lib64 -lnetcdf


objs = ${OBJ}/${NCUTIL}.o ${OBJ}/${Ympp}.o ${OBJ}/${Ycpp}.o


 

#.cpp.o :
#	${CXX} ${CXXFLAGS} ${PARALLEL} $<

sechiba: ${BIN}/${EXECNAME}

${BIN}/${EXECNAME}: ${objs} 
	${CXX} -o ${BIN}/${EXECNAME} ${objs} ${LDMQN} ${LDFLAGS}


${OBJ}/${NCUTIL}.o : ${INCDIR}/${NCUTIL}.h ${SRC}/${NCUTIL}.cpp
	${CXX} ${INCFLAGS} ${SRC}/${NCUTIL}.cpp ${CXXFLAGS} -o ${OBJ}/${NCUTIL}.o

${OBJ}/${Ympp}.o : ${SRC}/${Ympp}.cpp
	${CXX} ${INCFLAGS} ${SRC}/${Ympp}.cpp ${CXXFLAGS} -o ${OBJ}/${Ympp}.o

${OBJ}/${Ycpp}.o : ${SRC}/${Ycpp}.cpp
	${CXX} ${INCFLAGS} ${SRC}/${Ycpp}.cpp ${CXXFLAGS} -o ${OBJ}/${Ycpp}.o
#${sources} : ${Ympp} ${Ycpp}
#	${CXX} ${CXXFLAGS} ${PARALLEL} -c $Ympp $Ycpp







clean:
	rm -rf ${objs} ${BIN}/${EXECNAME}



