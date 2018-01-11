cmake_minimum_required(VERSION 3.7)
project(HetSNets)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp MySQLManager.cpp MySQLManager.h Configuration.h Configuration.cpp ImportExport.h ImportExport.cpp SystemDriveBus.h Device.h Device.cpp Interface.h Mymath.h Mymath.cpp Global.h channel.h channel.cpp HardwareEntity.h HardwareEntity.cpp SoftwareEntity.h SoftwareEntity.cpp ImtaChannelModel.h ImtaChannelModel.cpp dBLibrary.h dBLibrary.cpp Traffic.h Traffic.cpp HARQACKTreatment.h HARQACKTreatment.cpp Sense.h Sense.cpp InterferenceIndex.h InterferenceIndex.cpp Computing.h Computing.cpp Polarization.h Polarization.cpp  ControlCenter.h  ControlCenter.cpp graphColoring.h graphColoring.cpp ResourceAllocation.h ResourceAllocation.cpp)
add_executable(HetSNets ${SOURCE_FILES})

include_directories(/opt/lampp/include)
include_directories(/usr/include/eigen)

target_link_libraries(HetSNets /opt/lampp/lib/libmysqlclient.so)