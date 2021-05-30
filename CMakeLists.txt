cmake_minimum_required(VERSION 3.10.0)
project(jetson_smart_fan VERSION 0.1.0 LANGUAGES C)
include(GNUInstallDirs)

set(BUILD_DIRECTORY ${CMAKE_BINARY_DIR})

include(ExternalProject)
ExternalProject_Add(cJSON
        PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cjson"
        INSTALL_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/cjson/out"
        GIT_REPOSITORY "https://github.com.cnpmjs.org/DaveGamble/cJSON.git"
        GIT_TAG  "v1.7.14"
        CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DCJSON_BUILD_SHARED_LIBS=OFF -DENABLE_CJSON_UTILS=ON -DENABLE_CJSON_TEST=OFF
        #CONFIGURE_COMMAND ""
        INSTALL_COMMAND ""
)

ExternalProject_Get_Property(cJSON BINARY_DIR)

add_library(static_cjason STATIC IMPORTED)
set_target_properties(static_cjason PROPERTIES IMPORTED_LOCATION ${BINARY_DIR}/libcjson.a)
add_dependencies(static_cjason cJSON)

ExternalProject_Get_Property(cJSON source_dir)
include_directories(${source_dir}/..)

add_subdirectory(src)

macro(install_systemd_service component file_path binary_path config_path)
    get_filename_component(file_name ${file_path} NAME)
    get_filename_component(binary_directory ${binary_path} DIRECTORY)

    file(READ ${file_path} FILE_CONTENT)
    string(REPLACE "[CONFIG_PATH]" ${config_path} FILE_CONTENT ${FILE_CONTENT})
    string(REPLACE "[BINARY]" ${binary_path} FILE_CONTENT ${FILE_CONTENT})
    file(WRITE ${CMAKE_SOURCE_DIR}/tmp/systemd/${file_name} ${FILE_CONTENT})

  if(EXISTS "/lib/systemd/system")
    install(FILES ${CMAKE_SOURCE_DIR}/tmp/systemd/${file_name}
      COMPONENT ${component}
      DESTINATION "/lib/systemd/system"
      PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    )
  endif()
endmacro(install_systemd_service)

# get_target_property(CFAN_RUNTIME_OUTPUT_DIRECTORY jetson_smart_fan RUNTIME_OUTPUT_DIRECTORY)
# message(warning "====== build output ${CFAN_RUNTIME_OUTPUT_DIRECTORY}")

install_systemd_service("jetson_cfan" "${CMAKE_SOURCE_DIR}/services/jetson_smart_fan.service" "${CMAKE_INSTALL_PREFIX}/jetson_smart_fan" "${CMAKE_INSTALL_PREFIX}/pwm_fan.json")
