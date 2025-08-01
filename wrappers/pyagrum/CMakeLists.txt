############################################################################
#   This file is part of the aGrUM/pyAgrum library.                        #
#                                                                          #
#   Copyright (c) 2005-2025 by                                             #
#       - Pierre-Henri WUILLEMIN(_at_LIP6)                                 #
#       - Christophe GONZALES(_at_AMU)                                     #
#                                                                          #
#   The aGrUM/pyAgrum library is free software; you can redistribute it    #
#   and/or modify it under the terms of either :                           #
#                                                                          #
#    - the GNU Lesser General Public License as published by               #
#      the Free Software Foundation, either version 3 of the License,      #
#      or (at your option) any later version,                              #
#    - the MIT license (MIT),                                              #
#    - or both in dual license, as here.                                   #
#                                                                          #
#   (see https://agrum.gitlab.io/articles/dual-licenses-lgplv3mit.html)    #
#                                                                          #
#   This aGrUM/pyAgrum library is distributed in the hope that it will be  #
#   useful, but WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,          #
#   INCLUDING BUT NOT LIMITED TO THE WARRANTIES MERCHANTABILITY or FITNESS #
#   FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT. IN NO EVENT SHALL THE   #
#   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER #
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,        #
#   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR  #
#   OTHER DEALINGS IN THE SOFTWARE.                                        #
#                                                                          #
#   See the GNU Lesser General Public License (LICENSE.LGPL) and the MIT   #
#   licence (LICENSE.MIT) for more details.                                #
#                                                                          #
#   Contact  : info_at_agrum_dot_org                                       #
#   homepage : http://agrum.gitlab.io                                      #
#   gitlab   : https://gitlab.com/agrumery/agrum                           #
#                                                                          #
############################################################################

if(PYTHON_TARGET)
  set(Python_EXECUTABLE ${PYTHON_TARGET})
  unset(Python_INCLUDE_DIRS CACHE)
  unset(Python_LIBRARIES CACHE)
  unset(Python_VERSION CACHE)
  unset(Python_VERSION_MAJOR CACHE)
endif()
option(AGRUM_PYTHON_SABI "Use Python stable ABI" ON)
if(AGRUM_PYTHON_SABI AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.26)
  find_package(Python ${PYAGRUM_REQUIRED_PYTHON_VERSION} COMPONENTS Interpreter Development.SABIModule REQUIRED)
  add_library(Python::Module ALIAS Python::SABIModule)
else()
  find_package(Python ${PYAGRUM_REQUIRED_PYTHON_VERSION} COMPONENTS Interpreter Development.Module REQUIRED)
endif()
list(APPEND CMAKE_SWIG_FLAGS "-fastdispatch")
add_definitions("-DSWIG_TYPE_TABLE=pyproba")
if(AGRUM_PYTHON_SABI)
  set(SWIG_COMPILE_FLAGS "-DPy_LIMITED_API=0x030A0000" CACHE STRING "C++ compiler flags used for wrapper code")
else()
  set(SWIG_COMPILE_FLAGS "" CACHE STRING "C++ compiler flags used for wrapper code")
endif()

if(WIN32)
  set(INSTALL_PYTHONDIR Lib/site-packages CACHE STRING "python site-dir")
else()
  set(INSTALL_PYTHONDIR ${CMAKE_INSTALL_LIBDIR}/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}/site-packages CACHE STRING "python site-dir")
endif()

message(STATUS "================================")
message(STATUS "python version : ${Python_VERSION}")
message(STATUS "python installation : ${INSTALL_PYTHONDIR}")
message(STATUS "python include : ${Python_INCLUDE_DIRS}")
message(STATUS "python library : ${Python_LIBRARIES}")
message(STATUS "================================")

# == Initializing aGrUM
set(AGRUM_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
set(AGRUM_FILE_VERSION "${CMAKE_CURRENT_SOURCE_DIR}/../../VERSION.txt")
include(${AGRUM_FILE_VERSION})
include("${AGRUM_SOURCE_DIR}/cmake/Config.agrum.cmake")
configure_file("${AGRUM_SOURCE_DIR}/cmake/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/agrum/config.h")

# ==
# == Adding files in build tree
# ==
set(SWIG_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../swig")
file(GLOB PYAGRUM_EXTENSIONS ${CMAKE_CURRENT_SOURCE_DIR}/extensions/*.h)
file(GLOB PYAGRUMFILES swigsrc/*.i)

# generate swig into /raw then post-process into /generated-files
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/raw)
set(GENERATED_PYTHON "${CMAKE_CURRENT_SOURCE_DIR}/generated-files")
file(MAKE_DIRECTORY ${CMAKE_SWIG_OUTDIR})

macro(gum_add_python_module MODULENAME)
  string(TOLOWER "${MODULENAME}" MODULENAME_LOWER)

  if(SWIG_FOUND)
    add_custom_command(OUTPUT ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}.py
      ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}PYTHON_wrap.cxx
      ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}PYTHON_wrap.h
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MODULENAME_LOWER}.i
      ${SWIG_SOURCE_DIR}/aGrUM_wrap_${MODULENAME}.i
      ${PYAGRUMFILES}
      COMMAND ${SWIG_EXECUTABLE} -c++ -python ${CMAKE_SWIG_FLAGS}
      -outdir ${CMAKE_SWIG_OUTDIR}
      -I${AGRUM_SOURCE_DIR} -I${SWIG_SOURCE_DIR} -I${CMAKE_CURRENT_SOURCE_DIR}/swigsrc/ -I${CMAKE_CURRENT_SOURCE_DIR}/doc
      -o ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}PYTHON_wrap.cxx
      ${CMAKE_CURRENT_SOURCE_DIR}/${MODULENAME_LOWER}.i
    )
    add_custom_command(OUTPUT ${GENERATED_PYTHON}/${MODULENAME_LOWER}.py
      ${GENERATED_PYTHON}/${MODULENAME_LOWER}PYTHON_wrap.cxx
      ${GENERATED_PYTHON}/${MODULENAME_LOWER}PYTHON_wrap.h
      DEPENDS ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}.py
      ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}PYTHON_wrap.cxx
      ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}PYTHON_wrap.h
      COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/passForPyType.py ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}.py ${GENERATED_PYTHON}/${MODULENAME_LOWER}.py
      COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/passForPyType.py ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}PYTHON_wrap.cxx ${GENERATED_PYTHON}/${MODULENAME_LOWER}PYTHON_wrap.cxx
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SWIG_OUTDIR}/${MODULENAME_LOWER}PYTHON_wrap.h ${GENERATED_PYTHON})
  endif()

  set(_PYAGRUMLIB _${MODULENAME_LOWER})
  add_library(${_PYAGRUMLIB} MODULE ${GENERATED_PYTHON}/${MODULENAME_LOWER}PYTHON_wrap.cxx)
  set_target_properties(${_PYAGRUMLIB} PROPERTIES PREFIX "")

  if(WIN32)
    set_target_properties(${_PYAGRUMLIB} PROPERTIES SUFFIX ".pyd")
  else()
    set_target_properties(${_PYAGRUMLIB} PROPERTIES SUFFIX ".so")
  endif()

  set_target_properties(${_PYAGRUMLIB} PROPERTIES NO_SONAME ON)

  target_link_libraries(${_PYAGRUMLIB} PRIVATE ${LIBAGRUM})
  target_include_directories(${_PYAGRUMLIB} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${_PYAGRUMLIB} PRIVATE Python::Module)

  set_target_properties(${_PYAGRUMLIB} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

  set_target_properties(${_PYAGRUMLIB} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR})

  set_target_properties(${_PYAGRUMLIB} PROPERTIES COMPILE_FLAGS "${SWIG_COMPILE_FLAGS}")
  set_target_properties(${_PYAGRUMLIB} PROPERTIES UNITY_BUILD OFF)

  install(FILES ${GENERATED_PYTHON}/${MODULENAME_LOWER}.py DESTINATION ${INSTALL_PYTHONDIR}/pyagrum)
  install(TARGETS ${_PYAGRUMLIB} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum)

  # for additionnal rule such test pyAgrum
  add_custom_command(TARGET ${_PYAGRUMLIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy "${GENERATED_PYTHON}/${MODULENAME_LOWER}.py" "${CMAKE_CURRENT_BINARY_DIR}/.")

  add_custom_command(TARGET ${_PYAGRUMLIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs" "${CMAKE_CURRENT_BINARY_DIR}")
  add_custom_command(TARGET ${_PYAGRUMLIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/causal" "${CMAKE_CURRENT_BINARY_DIR}/causal")
  add_custom_command(TARGET ${_PYAGRUMLIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/causal/causalEffectEstimation" "${CMAKE_CURRENT_BINARY_DIR}/causal/causalEffectEstimation")
  add_custom_command(TARGET ${_PYAGRUMLIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/skbn" "${CMAKE_CURRENT_BINARY_DIR}/skbn")
  add_custom_command(TARGET ${_PYAGRUMLIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/clg" "${CMAKE_CURRENT_BINARY_DIR}/clg")
  add_custom_command(TARGET ${_PYAGRUMLIB} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/ctbn" "${CMAKE_CURRENT_BINARY_DIR}/ctbn")

endmacro()

#foreach(MODULE ${LIST_OF_MODULES})
#  if(MODULE MATCHES "FMDP")
#    continue()
#
#  gum_add_python_module(${MODULE})
#endforeach()
gum_add_python_module(pyagrum)

# updating generated cat requirements.txt from pyagrum/requirements.txt and pyAgrum/optional_requirements.txt
set(PYAGRUM_REQUIREMENTS_LIST
  ${CMAKE_CURRENT_SOURCE_DIR}/../../binder
  ${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx
  ${CMAKE_CURRENT_SOURCE_DIR}/doc/sphinx/notebooks
  ${CMAKE_CURRENT_SOURCE_DIR}/testunits
)

FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt PYAGRUM_REQ_CONTENT)
FILE(READ ${CMAKE_CURRENT_SOURCE_DIR}/optional_requirements.txt PYAGRUM_OPT_REQ_CONTENT)

FOREACH(reqdir ${PYAGRUM_REQUIREMENTS_LIST})
  message(STATUS "** act Notification: updating requirements.txt in ${reqdir}")
  FILE(READ ${reqdir}/specific_requirements.txt PYAGRUM_SPECIFIC_REQ_CONTENT)
  FILE(WRITE ${reqdir}/requirements.txt "################################\n")
  FILE(APPEND ${reqdir}/requirements.txt "# generated : do not change it !\n")
  FILE(APPEND ${reqdir}/requirements.txt "################################\n")
  FILE(APPEND ${reqdir}/requirements.txt "${PYAGRUM_REQ_CONTENT}\n")
  FILE(APPEND ${reqdir}/requirements.txt "#### optional requirements #####\n")
  FILE(APPEND ${reqdir}/requirements.txt "${PYAGRUM_OPT_REQ_CONTENT}\n")
  FILE(APPEND ${reqdir}/requirements.txt "################################\n")
  FILE(APPEND ${reqdir}/requirements.txt "${PYAGRUM_SPECIFIC_REQ_CONTENT}\n")
ENDFOREACH()

# REQUIREMENTS files updated
file(GLOB GUMPY_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/*.py")
file(GLOB GUMPY_CONFIG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/*.ini")
file(GLOB GUMLIB_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/lib/*.py")
file(GLOB GUMCAUSAL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/causal/*.py")
file(GLOB GUMCAUSALEFFECT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/causal/causalEffectEstimation/*.py")
file(GLOB GUMSKBN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/skbn/*.py")
file(GLOB GUMCLG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/clg/*.py")
file(GLOB GUMCTBN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/ctbn/*.py")
file(GLOB GUMBNMIXTURE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/pyLibs/bnmixture/*.py")

# include("${CMAKE_CURRENT_SOURCE_DIR}/../../VERSION.txt")
set(PYAGRUM_VERSION "${AGRUM_VERSION_MAJOR}.${AGRUM_VERSION_MINOR}.${AGRUM_VERSION_PATCH}")

# temporary deactivation
# if (DEFINED AGRUM_VERSION_TWEAK)
# set(PYAGRUM_VERSION "${PYAGRUM_VERSION}.{$AGRUM_VERSION_TWEAK}")
# endif ()
set(PYAGRUM_DISTDIR "pyagrum-${PYAGRUM_VERSION}.dist-info")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/__init__.in.py" "${CMAKE_CURRENT_BINARY_DIR}/__init__.py")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/deprecated.in.py" "${CMAKE_CURRENT_BINARY_DIR}/deprecated.py")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/common.in.py" "${CMAKE_CURRENT_BINARY_DIR}/common.py")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/METADATA.in" "${CMAKE_CURRENT_BINARY_DIR}/METADATA")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/WHEEL.in" "${CMAKE_CURRENT_BINARY_DIR}/WHEEL")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE" "${CMAKE_CURRENT_BINARY_DIR}/LICENSE")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.LGPL" "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.LGPL")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/../../LICENSE.MIT" "${CMAKE_CURRENT_BINARY_DIR}/LICENSE.MIT")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py DESTINATION ${INSTALL_PYTHONDIR}/pyagrum)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/deprecated.py DESTINATION ${INSTALL_PYTHONDIR}/pyagrum)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/common.py DESTINATION ${INSTALL_PYTHONDIR}/pyagrum)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/METADATA DESTINATION ${INSTALL_PYTHONDIR}/${PYAGRUM_DISTDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/WHEEL DESTINATION ${INSTALL_PYTHONDIR}/${PYAGRUM_DISTDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LICENSE DESTINATION ${INSTALL_PYTHONDIR}/${PYAGRUM_DISTDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.LGPL DESTINATION ${INSTALL_PYTHONDIR}/${PYAGRUM_DISTDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.MIT DESTINATION ${INSTALL_PYTHONDIR}/${PYAGRUM_DISTDIR})

install(FILES ${GUMPY_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum)
install(FILES ${GUMPY_CONFIG_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum)
install(FILES ${GUMLIB_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum/lib)
install(FILES ${GUMCAUSAL_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum/causal)
install(FILES ${GUMCAUSALEFFECT_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum/causal/causalEffectEstimation)
install(FILES ${GUMCLG_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum/clg)
install(FILES ${GUMCTBN_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum/ctbn)
install(FILES ${GUMSKBN_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum/skbn)
install(FILES ${GUMBNMIXTURE_FILES} DESTINATION ${INSTALL_PYTHONDIR}/pyagrum/bnmixture)

add_test(NAME python_quick COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/testunits/gumTest.py quick)
set_tests_properties(python_quick PROPERTIES ENVIRONMENT "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}/..")

# # show all cmake variables
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
# message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
