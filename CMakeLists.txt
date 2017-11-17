cmake_minimum_required(VERSION 3.7)
project(HetSNets)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp MySQLManager.cpp MySQLManager.h Configuration.h Configuration.cpp ImportExport.h ImportExport.cpp SystemDriveBus.h Device.h Device.cpp Interface.h Mymath.h Mymath.cpp Global.h channel.h channel.cpp HardwareEntity.h HardwareEntity.cpp SoftwareEntity.h SoftwareEntity.cpp ImtaChannelModel.h ImtaChannelModel.cpp dBLibrary.h dBLibrary.cpp Traffic.h Traffic.cpp HARQACKTreatment.h HARQACKTreatment.cpp Sense.h Sense.cpp InterferenceIndex.h InterferenceIndex.cpp Computing.h Computing.cpp Polarization.h Polarization.cpp  ControlCenter.h  ControlCenter.cpp
        psshn_platform/eml_rand_mt19937ar_stateful.h
        psshn_platform/inv.h
        psshn_platform/polarizationChannel.h
        psshn_platform/psshn_platform.h
        psshn_platform/psshn_platform_data.h
        psshn_platform/psshn_platform_initialize.h
        psshn_platform/psshn_platform_rtwutil.h
        psshn_platform/psshn_platform_terminate.h
        psshn_platform/psshn_platform_types.h
        psshn_platform/rand.h
        psshn_platform/relop.h
        psshn_platform/rt_defines.h
        psshn_platform/rt_nonfinite.h
        psshn_platform/rtGetInf.h
        psshn_platform/rtGetNaN.h
        psshn_platform/rtwtypes.h
        psshn_platform/svd.h
        psshn_platform/sqrt.h
        psshn_platform/xaxpy.h
        psshn_platform/xdotc.h
        psshn_platform/xnrm2.h
        psshn_platform/xrot.h
        psshn_platform/xrotg.h
        psshn_platform/xscal.h
        psshn_platform/xswap.h
        psshn_platform/xzggbal.h
        psshn_platform/xzggev.h
        psshn_platform/xzhgeqz.h
        psshn_platform/xztgevc.h

        psshn_platform/eml_rand_mt19937ar_stateful.cpp
        psshn_platform/inv.cpp
        psshn_platform/polarizationChannel.cpp
        psshn_platform/psshn_platform.cpp
        psshn_platform/psshn_platform_data.cpp
        psshn_platform/psshn_platform_initialize.cpp
        psshn_platform/psshn_platform_rtwutil.cpp
        psshn_platform/psshn_platform_terminate.cpp
        psshn_platform/rand.cpp
        psshn_platform/relop.cpp
        psshn_platform/rt_nonfinite.cpp
        psshn_platform/rtGetInf.cpp
        psshn_platform/rtGetNaN.cpp
        psshn_platform/svd.cpp
        psshn_platform/sqrt.cpp
        psshn_platform/xaxpy.cpp
        psshn_platform/xdotc.cpp
        psshn_platform/xnrm2.cpp
        psshn_platform/xrot.cpp
        psshn_platform/xrotg.cpp
        psshn_platform/xscal.cpp
        psshn_platform/xswap.cpp
        psshn_platform/xzggbal.cpp
        psshn_platform/xzggev.cpp
        psshn_platform/xzhgeqz.cpp
        psshn_platform/xztgevc.cpp)
add_executable(HetSNets ${SOURCE_FILES})

include_directories(/opt/lampp/include)
include_directories(/usr/include/eigen)

target_link_libraries(HetSNets /opt/lampp/lib/libmysqlclient.so)