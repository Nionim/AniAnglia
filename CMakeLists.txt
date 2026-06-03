cmake_minimum_required(VERSION 3.20)
project(AniSaturn VERSION 1.0.0 LANGUAGES OBJCXX)

set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_OSX_SYSROOT iphoneos)
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")
set(CMAKE_OSX_ARCHITECTURES "arm64")

set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
set(CMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH "NO")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(
    -DBOOST_HANA_CONFIG_ENABLE_STRING_UDL=0
    -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
    -DBOOST_VARIANT_USE_RELAXED_GET
)

set(SRC_DIR       "src")
set(LIB_DIR       "lib")
set(RES_DIR       "res")
set(LANG_DIR      "lang")
set(ENTITLE_DIR   "ent")

file(GLOB_RECURSE SOURCES
    "${SRC_DIR}/**/*.mm"
    "${SRC_DIR}/**/*.m"
    "${SRC_DIR}/**/*.c"
    "${SRC_DIR}/**/*.cpp"
)

file(GLOB_RECURSE HEADERS "${SRC_DIR}/**/*.h" "${SRC_DIR}/**/*.hpp")

set(INCLUDE_DIRS "${SRC_DIR}")
file(GLOB_RECURSE SRC_SUBDIRS LIST_DIRECTORIES true "${SRC_DIR}/*")
foreach(DIR ${SRC_SUBDIRS})
    if(IS_DIRECTORY ${DIR})
        list(APPEND INCLUDE_DIRS ${DIR})
    endif()
endforeach()

list(APPEND INCLUDE_DIRS
    "${LIB_DIR}/aateam/libanixart/include"
    "${LIB_DIR}/aateam/libnetsess/include"
    "${LIB_DIR}/aateam/libtorrentrepo/include"
)

set(CURL_TEMP_DIR "${CMAKE_BINARY_DIR}/curl_headers")
file(MAKE_DIRECTORY ${CURL_TEMP_DIR}/curl)
set(CURL_HEADERS_SRC "${LIB_DIR}/curlpp/curl.xcframework/ios-arm64/curl.framework/Headers")
if(EXISTS ${CURL_HEADERS_SRC})
    file(COPY ${CURL_HEADERS_SRC}/ DESTINATION "${CURL_TEMP_DIR}/curl")
endif()
list(APPEND INCLUDE_DIRS ${CURL_TEMP_DIR})
list(APPEND INCLUDE_DIRS "${LIB_DIR}/include")

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_compile_options(${PROJECT_NAME} PRIVATE
    -fobjc-arc
    -Wall
    -frtti
    -fno-objc-arc-exceptions
    -Wno-unknown-pragmas
    -Wno-explicit-specialization-storage-class
    -Wno-deprecated-builtins
    -Wno-deprecated-declarations
    -Wno-unused-function
    -Wno-unused-variable
    -Wno-unused-private-field
)

target_link_libraries(${PROJECT_NAME}
    "-framework UIKit"
    "-framework Foundation"
    "-framework CoreGraphics"
    "-framework AVFoundation"
    "-framework AVKit"
    "-framework MediaPlayer"
    "-framework AVFAudio"
    "-framework SystemConfiguration"
    "-framework Security"
    "-lz"
)

set(CURL_FRAMEWORK "${CMAKE_SOURCE_DIR}/lib/curlpp/curl.xcframework/ios-arm64/curl.framework/curl")
if(EXISTS "${CURL_FRAMEWORK}")
    target_link_libraries(${PROJECT_NAME} "${CURL_FRAMEWORK}")
else()
    set(CURL_STATIC "${CMAKE_SOURCE_DIR}/lib/curlpp/curl.xcframework/ios-arm64/libcurl.a")
    if(EXISTS "${CURL_STATIC}")
        target_link_libraries(${PROJECT_NAME} "${CURL_STATIC}")
    endif()
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/lib/curlpp/curlpp.xcframework/ios-arm64/libcurlpp.a")
    target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/lib/curlpp/curlpp.xcframework/ios-arm64/libcurlpp.a")
endif()

foreach(boost_lib container json regex system)
    set(BOOST_LIB "${CMAKE_SOURCE_DIR}/lib/boost/boost_${boost_lib}.xcframework/ios-arm64/libboost_${boost_lib}.a")
    if(EXISTS "${BOOST_LIB}")
        target_link_libraries(${PROJECT_NAME} "${BOOST_LIB}")
    endif()
endforeach()

set(LIBNETSESS "${CMAKE_SOURCE_DIR}/lib/aateam/libnetsess.xcframework/ios-arm64/libnetsess.a")
set(LIBANIXART "${CMAKE_SOURCE_DIR}/lib/aateam/libanixart.xcframework/ios-arm64/libanixart.a")

if(EXISTS "${LIBNETSESS}")
    target_link_libraries(${PROJECT_NAME} -force_load "${LIBNETSESS}")
endif()

if(EXISTS "${LIBANIXART}")
    target_link_libraries(${PROJECT_NAME} -force_load "${LIBANIXART}")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${RES_DIR}/Info.plist"
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
)

set(ASSET_CATALOG "${RES_DIR}/Assets.xcassets")
if(EXISTS ${ASSET_CATALOG})
    set_source_files_properties(${ASSET_CATALOG} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    target_sources(${PROJECT_NAME} PRIVATE ${ASSET_CATALOG})
endif()

file(GLOB_RECURSE STRING_FILES "${LANG_DIR}/*.lproj/Localizable.strings")
foreach(str_file ${STRING_FILES})
    string(REPLACE "${LANG_DIR}/" "" rel_path ${str_file})
    get_filename_component(lproj_dir ${rel_path} DIRECTORY)
    set_source_files_properties(${str_file} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${lproj_dir}")
    target_sources(${PROJECT_NAME} PRIVATE ${str_file})
endforeach()

set(ENTITLEMENTS_FILE "${ENTITLE_DIR}/AniSaturn.entitlements")
if(EXISTS ${ENTITLEMENTS_FILE})
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${ENTITLEMENTS_FILE}")
endif()

set(VER_SHORT "" CACHE STRING "Marketing version")
set(VER_BUILD "" CACHE STRING "Build number")
if(VER_SHORT)
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_MARKETING_VERSION "${VER_SHORT}")
endif()
if(VER_BUILD)
    set_target_properties(${PROJECT_NAME} PROPERTIES XCODE_ATTRIBUTE_CURRENT_PROJECT_VERSION "${VER_BUILD}")
endif()