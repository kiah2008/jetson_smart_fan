cmake_minimum_required(VERSION 3.10.0)
project(jetson_smart_fan VERSION 0.1.0)

add_executable(jetson_smart_fan  main.cpp
                                debug.cpp
                                PwmHandler.cpp
                                ThermalHandler.cpp
                                PwmFanMgr.cpp)
#target_compile_definitions(jetson_smart_fan PRIVATE -DENABLE_DEBUG=1 -DUSING_THREAD=1 -DHAVE_VSYSLOG=1)
target_compile_definitions(jetson_smart_fan PRIVATE -DUSING_THREAD=1 -DHAVE_VSYSLOG=1)
target_link_libraries(jetson_smart_fan static_cjason pthread)

message(warning "===== ${CMAKE_INSTALL_PREFIX}")
# set_target_properties(jetson_smart_fan
#   PROPERTIES
#   ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin"
#   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/lib"
#   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_INSTALL_PREFIX}/bin"
# )
install(TARGETS jetson_smart_fan DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT binaries)
install(FILES ${CMAKE_SOURCE_DIR}/config/pwm_fan.json DESTINATION ${CMAKE_INSTALL_PREFIX} COMPONENT config)

# install(TARGETS jetson_smart_fan RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)
