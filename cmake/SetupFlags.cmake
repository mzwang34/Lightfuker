include(CheckCXXCompilerFlag)

option(LW_ENABLE_FASTMATH "Enable unsafe math optimizations" OFF)

if(LW_ENABLE_FASTMATH)
	if((CMAKE_CXX_COMPILER_ID MATCHES "MSVC") OR (CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC"))
		#set(FF_FLAGS /fp:fast) #< Disables Inf which is needed!
		set(FF_FLAGS /fp:contract) #< Only enables contractions like fma
		check_cxx_compiler_flag(${FF_FLAGS} HAS_CONTRACT)
		if(NOT HAS_CONTRACT)
			set(FF_FLAGS )
		endif()
    elseif((CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
		set(FF_FLAGS -funsafe-math-optimizations -fno-rounding-math -fno-math-errno)
	endif()
endif()

if((CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT MATCHES "MSVC") OR (CMAKE_CXX_COMPILER_ID MATCHES "GNU"))
	set(CMAKE_CXX_FLAGS_DEBUG "-g -Og" CACHE STRING "" FORCE)
	set(CMAKE_CXX_FLAGS_RELEASE "-O3" CACHE STRING "" FORCE)
endif()

function(add_fastmath TARGET)
    target_compile_options(${TARGET} PRIVATE ${FF_FLAGS})
endfunction()
