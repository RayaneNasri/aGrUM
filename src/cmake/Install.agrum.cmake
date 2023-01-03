## installation module for recognize agrum (see documentation "How to use agrum")
install(FILES
        ${CMAKE_CURRENT_CMAKE_DIR}/aGrUMUse.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/aGrUMConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/aGrUMConfigVersion.cmake
        DESTINATION lib${LIB_SUFFIX}/cmake/aGrUM
        )
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/agrum.pc
        DESTINATION lib${LIB_SUFFIX}/pkgconfig
        )

set(CXX_FLAGS "${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")
configure_file(
        "${CMAKE_CURRENT_CMAKE_DIR}/agrum.pc.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/agrum.pc"
)

install(CODE "MESSAGE(\"\n\n************************\")")
install(CODE "MESSAGE(\"* Installing Agrum ... *\")")
install(CODE "MESSAGE(\"************************\n\n\")")
install(DIRECTORY ${AGRUM_SOURCE_DIR}/agrum DESTINATION include FILES_MATCHING PATTERN PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.hh")
install(DIRECTORY ${AGRUM_BINARY_DIR}/agrum DESTINATION include FILES_MATCHING PATTERN PATTERN "*.h")
install(TARGETS ${LIBAGRUM}
        EXPORT ${LIBAGRUM}-targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib${LIB_SUFFIX}
        ARCHIVE DESTINATION lib${LIB_SUFFIX})

install(EXPORT ${LIBAGRUM}-targets
        DESTINATION lib${LIB_SUFFIX}/cmake/aGrUM
        COMPONENT Development)
