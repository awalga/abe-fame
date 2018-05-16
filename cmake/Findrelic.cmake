################################
# Not a proper find build
# and export build tree
################################

################################
# Vendor Relic Build Options
################################
set(MULTI PTHREAD CACHE INTERNAL "thread-lib-relic")
set(CORES 4 CACHE INTERNAL "cores-build-relic")
set(CHECK off CACHE INTERNAL "check-off-relic")
set(ARITH gmp CACHE INTERNAL "arith-gmp-relic")
set(FP_PRIME 381 CACHE INTERNAL "fp-prime-size-relic")
set(FP_QNRES on CACHE INTERNAL "fp-qnres-relic")
set(FP_METHD "BASIC;COMBA;COMBA;MONTY;LOWER;SLIDE" CACHE INTERNAL "fp-method-relic")
set(FPX_METHD "INTEG;INTEG;LAZYR" CACHE INTERNAL "fpx-method-relic")
set(PP_METHD "LAZYR;OATEP" CACHE INTERNAL "pp-methode-relic")
set(WITH BN CACHE INTERNAL "bn-module-relic")
set(WITH DV CACHE INTERNAL "dv-module-relic")
set(WITH FP CACHE INTERNAL "fp-module-relic")
set(WITH FPX CACHE INTERNAL "fpx-module-relic")
set(WITH FB CACHE INTERNAL "fb-module-relic")
set(WITH EP CACHE INTERNAL "ep-module-relic")
set(WITH EPX CACHE INTERNAL "epx-module-relic")
set(WITH EB CACHE INTERNAL "eb-module-relic")
set(WITH ED CACHE INTERNAL "ed-module-relic")
set(WTTH EC CACHE INTERNAL "ec-module-relic")
set(WITH PB CACHE INTERNAL "pb-module-relic")
set(WITH PP CACHE INTERNAL "pp-module-relic")
set(WITH PC CACHE INTERNAL "pc-module-relic")
set(SHLIB off CACHE INTERNAL "shared-lib-relic")
set(STLIB on CACHE INTERNAL "static-lib-relic")

################################
# Import Relic and add project
################################
CPM_GIT ("relic" "https://github.com/awalga/relic.git" "master" ${PROJECT_SOURCE_DIR}/thirdparty)
install(EXPORT relic-config DESTINATION share/relic/cmake)
export(TARGETS relic_s FILE relic-config.cmake)
set_property(TARGET relic_s APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
        $<BUILD_INTERFACE:${relic_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/thirdparty/relic/include>
        )
