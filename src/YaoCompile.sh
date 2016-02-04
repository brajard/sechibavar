#/bin/sh


#CXX            --> g++
#CXXFLAGS	--> flags variables globales
#PARALLEL	--> si se compila con la option -p, para codigo paralelo
#Ympp		--> ./Yworkdir/Ymain_sechiba.cpp
#Ycpp		--> ./Yworkdir/Ysechiba.cpp



OPTCPIL=""
DBGCPIL=""
#INCDIR="/usr/neuro/local/yao/yao9/yao/include -I/usr/include"
INCDIR="${PWD}/include/yao -I/usr/include"

CXX="g++"
CXXFLAGS=" ${OPTCPIL} ${DBGCPIL} -Wall -DLINUXPC -Wno-deprecated -Wno-write-strings -I${INCDIR}"
PARALLEL=""
Ympp="./Ymain_sechiba.cpp"
Ycpp="./Ysechiba.cpp"

${CXX} ${CXXFLAGS} ${PARALLEL} -c $Ympp $Ycpp





EXECNAME="sechiba"
prj="sechiba"

#YAODIR="/usr/neuro/local/yao/yao9"
#MQNDIR="${YAODIR}/yao/share/inria/"
#MACHINE="x86_64_SciLin64"
#BINMQN="${MQNDIR}/bin/${MACHINE}"
#LDMQN=" -L${BINMQN} -lmqn"


BINMQN="${PWD}/bin"
LDMQN=" -L${BINMQN} -lmqn"


LDYEXTOBJ="${PWD}/include/ncutil.o"
LDFLAGS=" -lm -lgfortran -L/usr/lib64 -lnetcdf"


${CXX} ${CXXFLAGS} ${PARALLEL} -o $EXECNAME Ymain_$prj.o Y$prj.o ${LDMQN} ${LDYEXTOBJ} ${LDFLAGS}





#g++    -Wall -DLINUXPC -Wno-deprecated -Wno-write-strings -I/usr/neuro/local/yao/yao9/yao/include -I/usr/include  -c ./Ymain_sechiba.cpp ./Ysechiba.cpp
#g++    -Wall -DLINUXPC -Wno-deprecated -Wno-write-strings -I/usr/neuro/local/yao/yao9/yao/include -I/usr/include  -o sechiba ./Ymain_sechiba.o ./Ysechiba.o  -L/usr/neuro/local/yao/yao9/yao/share/inria//bin/x86_64_SciLin64 -lmqn  ./include/ncutil.o   -lm -lgfortran -L/usr/lib64 -lnetcdf





