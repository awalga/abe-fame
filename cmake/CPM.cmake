function(CPM_GIT SYMBOL GIT_REPO GIT_TAG PREFIX)

    if (${SYMBOL}_IMPORTED)
        message("-- Already exist " ${SYMBOL} " " ${GIT_REPO} " " ${GIT_TAG} " " ${PREFIX})
    else ()
        set(${SYMBOL}_IMPORTED ON CACHE INTERNAL "SYMBOL_IMPORTED")
        set(${SYMBOL}_IMPORTED_GIT_REPO ${GIT_REPO})
        set(${SYMBOL}_IMPORTED_GIT_TAG ${GIT_TAG})
        set(${SYMBOL}_IMPORTED_PREFIX ${PREFIX})

        message("-- Importing " ${SYMBOL} " " ${GIT_REPO} " " ${GIT_TAG} " " ${PREFIX})
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
        endif ()
        add_subdirectory(${PREFIX}/${SYMBOL})
        set(${SYMBOL}_DIR ${PREFIX}/${SYMBOL} CACHE INTERNAL "DIR_IMPORTED")
    endif ()
endfunction(CPM_GIT)