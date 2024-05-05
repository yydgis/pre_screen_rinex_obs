cmake_minimum_required(VERSION 3.16)
project(pre_screen_rinex_obs)

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DTRACE -DENAGLO -DENAQZS -DENAGAL -DENACMP -DENAIRN -DNFREQ=7 -DNEXOBS=3)
include_directories(./)
include_directories(../rtklib_lte/src)
include_directories(../rtklib_lte/src/rcv)
add_executable(pre_screen_rinex_obs
    pre_screen_rinex_obs.cpp
    ../rtklib_lte/src/rtklib.h   
    ../rtklib_lte/src/rtkcmn.c
    ../rtklib_lte/src/rinex.c   
    ../rtklib_lte/src/rtkpos.c   
    ../rtklib_lte/src/postpos.c  
    ../rtklib_lte/src/solution.c 
    ../rtklib_lte/src/lambda.c   
    ../rtklib_lte/src/sbas.c     
    ../rtklib_lte/src/preceph.c  
    ../rtklib_lte/src/pntpos.c   
    ../rtklib_lte/src/geoid.c
    ../rtklib_lte/src/ephemeris.c
    ../rtklib_lte/src/options.c  
    ../rtklib_lte/src/ppp.c      
    ../rtklib_lte/src/ppp_ar.c   
    ../rtklib_lte/src/rtcm.c     
    ../rtklib_lte/src/rtcm2.c    
    ../rtklib_lte/src/rtcm3.c    
    ../rtklib_lte/src/rtcm3e.c   
    ../rtklib_lte/src/ionex.c    
    ../rtklib_lte/src/tides.c   
    )

target_link_libraries(pre_screen_rinex_obs m)
