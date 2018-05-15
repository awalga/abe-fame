################################
# Not a proper find build
# and export build tree
################################

################################
# Vendor Relic Build Options
################################
set(MULTI PTHREAD)
set(CORES 4)
set(CHECK off)
set(ARITH gmp)
set(FP_PRIME 381)
set(FP_QNRES on)
set(FP_METHD "BASIC;COMBA;COMBA;MONTY;LOWER;SLIDE")
set(FPX_METHD "INTEG;INTEG;LAZYR")
set(PP_METHD "LAZYR;OATEP")
set(WITH BN)
set(WITH DV)
set(WITH FP)
set(WITH FPX)
set(WITH FB)
set(WITH EP)
set(WITH EPX)
set(WITH EB)
set(WITH ED)
set(WTTH EC)
set(WITH PB)
set(WITH PP)
set(WITH PC)
set(SHLIB off)
set(STLIB on)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/relic)
install(EXPORT relic-config DESTINATION share/relic/cmake)
export(TARGETS relic_s FILE relic-config.cmake)
set_property(TARGET relic_s APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/relic/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/thirdparty/relic/include>
        )
