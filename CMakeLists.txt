cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

include(CMakePackageConfigHelpers)

set(Lyra2FileEncryptor_VERSION 1.2.0)

project(libLyra2FileEncryptor
	DESCRIPTION "Lyra2FileEncryptor library"
	VERSION ${Lyra2FileEncryptor_VERSION}
)

find_package(OpenSSL REQUIRED)
find_package(jsoncpp REQUIRED)

set(Lyra2_include
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

list(TRANSFORM Lyra2_include PREPEND "include/Lyra2/")
list(TRANSFORM src_include PREPEND "include/")
list(TRANSFORM AES_src PREPEND "src/AES/")
list(TRANSFORM FileAux_src PREPEND "src/FileAux/")
list(TRANSFORM HashWrapper_src PREPEND "src/HashWrapper/")
list(TRANSFORM Lyra2_src PREPEND "src/Lyra2/")
list(TRANSFORM Lyra2FileEncryptor_src PREPEND "src/Lyra2FileEncryptor/")

if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
	add_compile_options(-openmp)
elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
	add_compile_options(-fopenmp)
elseif(MSVC)
	add_compile_options(/openmp)
endif()

include_directories("include/")
include_directories("include/Lyra2")

add_library(lyra2-file-encryptor
        ${Lyra2_include}
        ${src_include}
        ${AES_src}
        ${FileAux_src}
        ${HashWrapper_src}
        ${Lyra2_src}
        ${Lyra2FileEncryptor_src})

# Configure package cmake files
configure_package_config_file(cmake/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/lyra2-file-encryptor-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/lyra2-file-encryptor
        NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/lyra2-file-encryptor-config-version.cmake
        VERSION ${Lyra2FileEncryptor_VERSION}
        COMPATIBILITY SameMajorVersion
)

# Install package files
install(
        FILES
                ${CMAKE_CURRENT_BINARY_DIR}/lyra2-file-encryptor-config.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/lyra2-file-encryptor-config-version.cmake
        DESTINATION
                ${CMAKE_INSTALL_PREFIX}/cmake/lyra2-file-encryptor)

install(TARGETS lyra2-file-encryptor EXPORT lyra2-file-encryptor-targets)

# install compiled lib
install(TARGETS lyra2-file-encryptor
	DESTINATION
		${CMAKE_INSTALL_PREFIX}/lib/
)

# install Header file
install(FILES 
                "include/Lyra2FileEncryptor.h"
                "include/Lyra2/Lyra2.h"
	DESTINATION
		${CMAKE_INSTALL_PREFIX}/include/
)

install(EXPORT lyra2-file-encryptor-targets
	NAMESPACE Lyra2FileEncryptor::
	DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake/lyra2-file-encryptor)