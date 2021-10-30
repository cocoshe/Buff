cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(RM_CV)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_module)

#只需要修改路径
set(INC_DIRS buff/Inc
             camera/cam_inc
             camera/Inc
             armor/Inc
             main/Inc
             serials/Inc
             general/Inc
             pnpSolve/Inc
             offsetSolve/Inc
             utilities/Inc
             kalman/Inc
             spin/inc
             state_tab
             #tracker/Inc
             coordinate/inc
             )

set(LIB_DIRS ${PROJECT_SOURCE_DIR}/camera/lib)



#链接库和.h
include_directories(${INC_DIRS})
link_libraries(${LIB_DIRS})

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)
#find_package(Glog REQUIRED)

add_executable(RM_CV main.cpp armor/Src/ArmorDector.cpp
                            camera/Src/camera_wrapper.cpp
                            pnpSolve/Src/PNPSolver.cpp
                            main/Src/img_buffer.cpp
                            main/Src/ImgProdCons.cpp
                            serials/Src/serial.cpp
                            utilities/Src/opencv_extend.cpp
                            camera/Src/camera.cpp
                            buff/Src/Buff.cpp
                            offsetSolve/Src/offsetSolve.cpp
                            kalman/Src/kalmanKF.cpp
                            spin/src/spin.cpp
                            #tracker/Src/track.cpp
                            state_tab/state_tab.cpp

                            coordinate/src/coordinate.cpp
                            )

#链接
target_link_libraries(RM_CV ${LIB_DIRS}   libGCBase_gcc421_v3_0.so
                                        libGenApi_gcc421_v3_0.so  
                                        libLog_gcc421_v3_0.so  
                                        liblog4cpp_gcc421_v3_0.so
                                        libMathParser_gcc421_v3_0.so  
                                        libMediaProcess.so  
                                        libMvCameraControl.so
                                        libMVGigEVisionSDK.so  
                                        libMVRender.so  
                                        libMvUsb3vTL.so 
                                        libNodeMapData_gcc421_v3_0.so  
                                        libXmlParser_gcc421_v3_0.so
                                        libgxiapi.so)


#target_link_libraries(RM_CV ${OpenCV_LIBS} ${GLOG_LIBRARY})
target_link_libraries(RM_CV ${OpenCV_LIBS})
target_link_libraries(RM_CV Threads::Threads)
