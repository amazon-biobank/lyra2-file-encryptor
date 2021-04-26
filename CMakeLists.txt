cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

project(libLyra2FileEncryptor
	DESCRIPTION "Lyra2FileEncryptor library"
	VERSION 1.0.0
)

find_package(OpenSSL REQUIRED)
find_package(jsoncpp REQUIRED)

set(sse_include
	    blake2b-load-sse2
        blake2b-load-sse41
        blake2b-round
        Lyra2
        Sponge
)

set(src_include
        aesFile
        base64Util
        FileAux
        JsonEncryptionFormatter
        Lyra2FileEncryptor
        LyraHash
)

set(AES_src
        aesFile
)

set(FileAux_src
        base64Util
        FileAux
        JsonEncryptionFormatter
)

set(HashWrapper_src
        LyraHash
)

set(Lyra2_src
        Lyra2
        Sponge
)

set(Lyra2FileEncryptor_src
        Lyra2FileEncryptor
)

list(TRANSFORM sse_include PREPEND "include/sse/")
list(TRANSFORM src_include PREPEND "include/")
list(TRANSFORM AES_src PREPEND "src/AES/")
list(TRANSFORM FileAux_src PREPEND "src/FileAux/")
list(TRANSFORM HashWrapper_src PREPEND "src/HashWrapper/")
list(TRANSFORM Lyra2_src PREPEND "src/Lyra2/")
list(TRANSFORM Lyra2FileEncryptor_src PREPEND "src/Lyra2FileEncryptor/")

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
	add_compile_options(
		-openmp)
elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
	add_compile_options(
		-fopenmp)
elseif(MSVC)
	add_compile_options(
		/openmp)
endif()

include_directories("include/")
include_directories("include/sse")

add_library(Lyra2FileEncryptor
        ${sse_include}
        ${src_include}
        ${AES_src}
        ${FileAux_src}
        ${HashWrapper_src}
        ${Lyra2_src}
        ${Lyra2FileEncryptor_src}
)

# install compiled lib
install(TARGETS Lyra2FileEncryptor
	DESTINATION
		${CMAKE_INSTALL_PREFIX}/lib/
)

# install Header file
install(FILES "include/Lyra2FileEncryptor.h"
	DESTINATION
		${CMAKE_INSTALL_PREFIX}/include/
)