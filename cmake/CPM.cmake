function(CPM_GIT SYMBOL GIT_REPO GIT_TAG PREFIX)

    if (${SYMBOL}_IMPORTED)
        message("-- Already exist " ${SYMBOL} " " ${${SYMBOL}_IMPORTED_GIT_REPO} " " ${${SYMBOL}_IMPORTED_GIT_TAG} " " ${${SYMBOL}_IMPORTED_PREFIX})
    else ()
        set(${SYMBOL}_IMPORTED ON CACHE INTERNAL "${SYMBOL}_IMPORTED")
        set(${SYMBOL}_IMPORTED_GIT_REPO ${GIT_REPO} CACHE INTERNAL "${SYMBOL}_GIT_REPO")
        set(${SYMBOL}_IMPORTED_GIT_TAG ${GIT_TAG} CACHE INTERNAL "${SYMBOL}_GIT_TAG")
        set(${SYMBOL}_IMPORTED_PREFIX ${PREFIX} CACHE INTERNAL "${SYMBOL}_PREFIX")

        message("-- Importing " ${SYMBOL} " " ${GIT_REPO} " " ${GIT_TAG} " " ${PREFIX})
        execute_process(COMMAND ls ${PREFIX} RESULT_VARIABLE result OUTPUT_QUIET ERROR_QUIET)
        if (result)
            message("${PREFIX} doesn't exist.")
            message("---- mkdir -p ${PREFIX}")
            execute_process(COMMAND mkdir -p ${PREFIX})
        endif()
        if (EXISTS ${PREFIX}/${SYMBOL}/)
            message("---- Reset Directory and Import" ${PREFIX}/${SYMBOL})
            message("----git submodule deinit --force ${SYMBOL}")
            execute_process(
                    COMMAND git submodule deinit --force ${SYMBOL}
                    WORKING_DIRECTORY ${PREFIX})
            message("----git rm -f ${SYMBOL}")
            execute_process(
                    COMMAND git rm -f ${SYMBOL}
                    WORKING_DIRECTORY ${PREFIX})
            message("----git submodule add --force ${GIT_REPO} ${SYMBOL}")
            execute_process(
                    COMMAND git submodule add --force ${GIT_REPO} ${SYMBOL}
                    WORKING_DIRECTORY ${PREFIX})
            message("----git submodule update --init --force ${SYMBOL}")
            execute_process(
                    COMMAND git submodule update --init --force ${SYMBOL}
                    WORKING_DIRECTORY ${PREFIX})
            message("----git checkout ${GIT_TAG}")
            execute_process(
                    COMMAND git checkout ${GIT_TAG}
                    WORKING_DIRECTORY ${PREFIX}/${SYMBOL})
        else ()
            # Create dir prefix/symbol -> clone in prefix/symbol -> update --init prefix/symbol
            message("---- Import" ${PREFIX}/${SYMBOL})
            message("----git submodule add ${GIT_REPO} ${SYMBOL}")
            execute_process(
                    COMMAND git submodule add ${GIT_REPO} ${SYMBOL}
                    WORKING_DIRECTORY ${PREFIX})
            message("----git submodule update --init ${SYMBOL}")
            execute_process(
                    COMMAND git submodule update --init ${SYMBOL}
                    WORKING_DIRECTORY ${PREFIX})
            message("----git checkout ${GIT_TAG}")
            execute_process(
                    COMMAND git checkout ${GIT_TAG}
                    WORKING_DIRECTORY ${PREFIX}/${SYMBOL})
        endif ()
        add_subdirectory(${PREFIX}/${SYMBOL})
        set(${SYMBOL}_DIR ${PREFIX}/${SYMBOL} CACHE INTERNAL "DIR_IMPORTED")
    endif ()
endfunction(CPM_GIT)