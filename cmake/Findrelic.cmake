################################
# Not a proper find build
# and export build tree
################################

################################
# Vendor Relic Build Options
################################
set ( MULTI PTHREAD )
set ( CORES 4 )
set ( CHECK off )
# set ( ARITH gmp )
set ( FP_PRIME 381 )
set ( FP_QNRES on )
set ( FP_METHD "BASIC;COMBA;COMBA;MONTY;LOWER;SLIDE" )
set ( FPX_METHD "INTEG;INTEG;LAZYR" )
set ( PP_METHD "LAZYR;OATEP" )

################################
# Import Relic and add project
################################
CPM_GIT ("relic" "https://github.com/awalga/relic.git" "master" "${PROJECT_SOURCE_DIR}/thirdparty")
