function(AnubisHasSSE)

  # Check if g++ is used.
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    set(ANUBIS_SSE_CAN_COMPILE TRUE PARENT_SCOPE)

    # Try to compile and run an SSE application.
    try_run(SSE_RUN_RESULT SSE_COMPILE_RESULT
      "${CMAKE_BINARY_DIR}/HostTests" "${CMAKE_SOURCE_DIR}/HostTests/TestSSE.cpp"
      COMPILE_DEFINITIONS "${CMAKE_CXX_FLAGS} -msse4.1")

    if("${SSE_RUN_RESULT}" STREQUAL "0")
      set(ANUBIS_SSE_CAN_RUN TRUE PARENT_SCOPE)
    else()
      set(ANUBIS_SSE_CAN_RUN FALSE PARENT_SCOPE)
    endif()
  endif()
endfunction()
