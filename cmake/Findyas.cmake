################################
# create an interface lib
################################
add_library(yas INTERFACE)
target_include_directories(yas INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/yas/include>)
install(TARGETS yas EXPORT yas-config
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows
install(EXPORT yas-config DESTINATION share/yas/cmake)
export(TARGETS yas FILE yas-config.cmake)