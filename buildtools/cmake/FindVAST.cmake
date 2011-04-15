# Locate VAST library 
# This module defines 
# VAST_LIBRARY, the name of the library to link against 
# VAST_FOUND, if false, do not try to link to VAST 
# VAST_INCLUDE_DIR, where to find VAST headers 
# 
# Created by Jonathan Marchand Based on the FindSDL_*.cmake modules, 
# created by Eric Wing, which were influenced by the FindSDL.cmake 
# module, but with modifications to recognize OS X frameworks and 
# additional Unix paths (FreeBSD, etc). 

SET(VAST_COMPONENTS
    common
    sim
    net
)

SET (VAST_INCLUDES
    Verse
    sim
)

SET(VAST_INCLUDE_SEARCH_DIR 
    ~/Library/Frameworks 
    /Library/Frameworks 
    /usr/local/include/VAST
    /usr/include/VAST
    /usr/local/include 
    /usr/include 
    /sw/include/VAST # Fink 
    /sw/include 
    /opt/local/include/VAST # DarwinPorts 
    /opt/local/include 
    /opt/csw/include/VAST # Blastwave 
    /opt/csw/include 
    /opt/include/VAST
    /opt/include 
) 

SET(VAST_LIBRARY_SEARCH_DIR 
    ~/Library/Frameworks 
    /Library/Frameworks 
    /usr/local 
    /usr 
    /sw 
    /opt/local 
    /opt/csw 
    /opt 
) 


FIND_LIBRARY(VAST_MAIN_LIBRARY
    NAMES vast
    HINTS
    $ENV{VASTDIR}
    PATH_SUFFIXES lib64 lib
    PATHS ${VAST_LIBRARY_SEARCH_DIR}
)
IF(VAST_MAIN_LIBRARY)
    LIST(APPEND VAST_LIBRARY ${VAST_MAIN_LIBRARY})
    LIST(REMOVE_DUPLICATES VAST_LIBRARY)
ENDIF(VAST_MAIN_LIBRARY)

FOREACH(COMPONENT ${VAST_COMPONENTS}) 
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT) 
    STRING(TOLOWER ${COMPONENT} LOWERCOMPONENT)
    FIND_LIBRARY(VAST_${UPPERCOMPONENT}_LIBRARY 
        NAMES vast${LOWERCOMPONENT} 
        HINTS 
        $ENV{VASTDIR} 
        PATH_SUFFIXES lib64 lib 
        PATHS ${VAST_LIBRARY_SEARCH_DIR} 
    ) 
    IF(VAST_${UPPERCOMPONENT}_LIBRARY) 
        LIST(APPEND VAST_LIBRARY ${VAST_${UPPERCOMPONENT}_LIBRARY}) 
        LIST(REMOVE_DUPLICATES VAST_LIBRARY) 
    ENDIF(VAST_${UPPERCOMPONENT}_LIBRARY) 
ENDFOREACH(COMPONENT) 

FOREACH(COMPONENT ${VAST_INCLUDES})
    STRING(TOUPPER ${COMPONENT} UPPERCOMPONENT)
    STRING(TOLOWER ${COMPONENT} LOWERCOMPONENT)
    FIND_PATH(VAST_${UPPERCOMPONENT}_INCLUDE_DIR VAST${COMPONENT}.h 
        HINTS
        $ENV{VASTDIR}
        PATH_SUFFIXES include
        PATHS ${VAST_INCLUDE_SEARCH_DIR}
    )
    IF(VAST_${UPPERCOMPONENT}_INCLUDE_DIR)
        LIST(APPEND VAST_INCLUDE_DIR ${VAST_${UPPERCOMPONENT}_INCLUDE_DIR})
        LIST(REMOVE_DUPLICATES VAST_INCLUDE_DIR)
    ENDIF(VAST_${UPPERCOMPONENT}_INCLUDE_DIR)
ENDFOREACH(COMPONENT)


SET(VAST_FOUND "NO") 
IF(VAST_SYSTEM_LIBRARY AND VAST_INCLUDE_DIR) 
    SET(VAST_FOUND "YES") 
ENDIF(VAST_SYSTEM_LIBRARY AND VAST_INCLUDE_DIR) 

INCLUDE(FindPackageHandleStandardArgs) 
FIND_PACKAGE_HANDLE_STANDARD_ARGS(VAST DEFAULT_MSG VAST_LIBRARY VAST_INCLUDE_DIR) 

