cmake_minimum_required(VERSION 3.26)
project(veriaktar)
set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    set(SELECTDIR windows)
 
       
        set(SELECTRESOURCE  WIN32 resource.rc veriaktar.exe.manifest)


    set(LIBRARIES ws2_32 Bcrypt Urlmon   )
elseif (APPLE)
    #set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE INTERNAL "" FORCE)
    #set(SELECTDIR macos/arm64)
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE INTERNAL "" FORCE)
    set(SELECTDIR macos)
    set(LIBRARIES iconv "-framework SystemConfiguration -framework Carbon -framework IOKit -framework CoreVideo -framework Cocoa")
elseif (UNIX AND NOT APPLE)
    set(SELECTDIR linux)
    set(LIBRARIES dl pthread GL X11)
endif ()


#for compat, osx mingw linux
include_directories(../third_party/include)
link_directories(../third_party/lib/${SELECTDIR} ../bin)


add_executable(veriaktar ${SELECTRESOURCE} main.cpp yahoo.cpp metastock.cpp)

#set_target_properties(Musavir PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
target_link_libraries(veriaktar    curl  ssl crypto ${LIBRARIES})


