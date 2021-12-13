# -helper macro to add a "doc" target with CMake build system. 
# and configure doxy.config.in to doxy.config
#
# target "doc" allows building the documentation with doxygen/dot on WIN32 and Linux
# Creates .chm windows help file if MS HTML help workshop 
# (available from http://msdn.microsoft.com/workshop/author/htmlhelp)
# is installed with its DLLs in PATH.
#
#
# Please note, that the tools, e.g.:
# doxygen, dot, latex, dvips, makeindex, gswin32, etc.
# must be in path.
#
# Note about Visual Studio Projects: 
# MSVS hast its own path environment which may differ from the shell.
# See "Menu Tools/Options/Projects/VC++ Directories" in VS 7.1
#
# author Jan Woetzel 2004-2006
# www.mip.informatik.uni-kiel.de/~jw


find_package(Doxygen)


if (DOXYGEN_FOUND)

  # click+jump in Emacs and Visual Studio (for doxy.config) (jw)
  IF    (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
    SET(DOXY_WARN_FORMAT "\"$file($line) : $text \"")
  ELSE  (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
    SET(DOXY_WARN_FORMAT "\"$file:$line: $text \"")
  ENDIF (CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
  
  # we need latex for doxygen because of the formulas
  find_package(LATEX)
  if    (NOT LATEX_COMPILER)
    message(STATUS "latex command LATEX_COMPILER not found but usually required. You will probably get warnings and user inetraction on doxy run.")
  endif (NOT LATEX_COMPILER)
  if    (NOT MAKEINDEX_COMPILER)
    message(STATUS "makeindex command MAKEINDEX_COMPILER not found but usually required.")
  endif (NOT MAKEINDEX_COMPILER)
  if    (NOT DVIPS_CONVERTER)
    message(STATUS "dvips command DVIPS_CONVERTER not found but usually required.")
  endif (NOT DVIPS_CONVERTER)
  
  if   (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in")
    message(STATUS "configured ${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in --> ${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in 
      ${CMAKE_CURRENT_BINARY_DIR}/doxy.config
      @ONLY )
    # use (configured) doxy.config from (out of place) BUILD tree:
    set(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
  else (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in")
    # use static hand-edited doxy.config from SOURCE tree:
    set(DOXY_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config")
    if   (EXISTS ${DOXY_CONFIG})
      message(STATUS "WARNING: using existing ${CMAKE_CURRENT_SOURCE_DIR}/doxy.config instead of configuring from doxy.config.in file.")
    else (EXISTS ${DOXY_CONFIG})
      if   (EXISTS "${CMAKE_MODULE_PATH}/doxy.config.in")
        # using template doxy.config.in
        message(STATUS "configured ${CMAKE_CMAKE_MODULE_PATH}/doxy.config.in --> ${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
        configure_file(${CMAKE_MODULE_PATH}/doxy.config.in 
          ${CMAKE_CURRENT_BINARY_DIR}/doxy.config
          @ONLY )
        set(DOXY_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/doxy.config")
      else (EXISTS "${CMAKE_MODULE_PATH}/doxy.config.in")
        # failed completely...
        message(SEND_ERROR "Please create ${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in (or doxy.config as fallback)")
      endif(EXISTS "${CMAKE_MODULE_PATH}/doxy.config.in")

    endif(EXISTS ${DOXY_CONFIG})
  endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doxy.config.in")
  
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${DOXY_CONFIG})
  
  # create a windows help .chm file using hhc.exe
  # HTMLHelp DLL must be in path!
  # fallback: use hhw.exe interactively
  IF    (WIN32)
    FIND_PACKAGE(HTMLHelp)
    IF   (HTML_HELP_COMPILER)      
      SET (TMP "${CMAKE_CURRENT_BINARY_DIR}\\Doc\\html\\index.hhp")
      STRING(REGEX REPLACE "[/]" "\\\\" HHP_FILE ${TMP} )
      # MESSAGE(SEND_ERROR "DBG  HHP_FILE=${HHP_FILE}")
      ADD_CUSTOM_TARGET(winhelp ${HTML_HELP_COMPILER} ${HHP_FILE})
      ADD_DEPENDENCIES (winhelp doc)
     
      IF (NOT TARGET_DOC_SKIP_INSTALL)
      # install windows help?
      # determine useful name for output file 
      # should be project and version unique to allow installing 
      # multiple projects into one global directory      
      IF   (EXISTS "${PROJECT_BINARY_DIR}/Doc/html/index.chm")
        IF   (PROJECT_NAME)
          SET(OUT "${PROJECT_NAME}")
        ELSE (PROJECT_NAME)
          SET(OUT "Documentation") # default
        ENDIF(PROJECT_NAME)
        IF   (${PROJECT_NAME}_VERSION_MAJOR)
          SET(OUT "${OUT}-${${PROJECT_NAME}_VERSION_MAJOR}")
          IF   (${PROJECT_NAME}_VERSION_MINOR)
            SET(OUT  "${OUT}.${${PROJECT_NAME}_VERSION_MINOR}")
            IF   (${PROJECT_NAME}_VERSION_PATCH)
              SET(OUT "${OUT}.${${PROJECT_NAME}_VERSION_PATCH}")      
            ENDIF(${PROJECT_NAME}_VERSION_PATCH)
          ENDIF(${PROJECT_NAME}_VERSION_MINOR)
        ENDIF(${PROJECT_NAME}_VERSION_MAJOR)
        # keep suffix
        SET(OUT  "${OUT}.chm")
        
        #MESSAGE("DBG ${PROJECT_BINARY_DIR}/Doc/html/index.chm \n${OUT}")
        # create target used by install and package commands 
        INSTALL(FILES "${PROJECT_BINARY_DIR}/Doc/html/index.chm"
          DESTINATION "doc"
          RENAME "${OUT}"
        )
      ENDIF(EXISTS "${PROJECT_BINARY_DIR}/Doc/html/index.chm")
      ENDIF(NOT TARGET_DOC_SKIP_INSTALL)

    ENDIF(HTML_HELP_COMPILER)
    # MESSAGE(SEND_ERROR "HTML_HELP_COMPILER=${HTML_HELP_COMPILER}")
  ENDIF (WIN32) 
else (DOXYGEN_FOUND)
  add_custom_target(doc echo "Doxygen was not found at configuration time, install it to be able to build doc!"
    COMMAND exit 1
  )
   
endif (DOXYGEN_FOUND)
