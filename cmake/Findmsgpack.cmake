################################
# create an interface lib
################################
add_library(msgpack INTERFACE)
target_include_directories(msgpack INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/msgpack-c/include>)
install(TARGETS msgpack EXPORT msgpack-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(EXPORT msgpack-config DESTINATION share/msgpack/cmake)
export(TARGETS msgpack FILE msgpack-config.cmake)