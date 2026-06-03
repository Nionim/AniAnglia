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

set(SRC_DIR       "src")
set(LIB_DIR       "lib")
set(RES_DIR       "res")
set(LANG_DIR      "lang")
set(ENTITLE_DIR   "ent")

file(GLOB_RECURSE SOURCES
    "${SRC_DIR}/Auth/*.mm"
    "${SRC_DIR}/Auth/*.m"
    "${SRC_DIR}/Common/**/*.mm"
    "${SRC_DIR}/Common/**/*.m"
    "${SRC_DIR}/Common/**/*.c"
    "${SRC_DIR}/Common/**/*.cpp"
    "${SRC_DIR}/Core/*.mm"
    "${SRC_DIR}/Main/**/*.mm"
    "${SRC_DIR}/Main/**/*.m"
    "${SRC_DIR}/Misc/**/*.mm"
    "${SRC_DIR}/Misc/**/*.m"
    "${SRC_DIR}/Profiles/**/*.mm"
    "${SRC_DIR}/Profiles/**/*.m"
    "${SRC_DIR}/Releases/**/*.mm"
    "${SRC_DIR}/Releases/**/*.m"
)

file(GLOB_RECURSE HEADERS
    "${SRC_DIR}/**/*.h"
    "${SRC_DIR}/**/*.hpp"
)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${SOURCES}
    ${HEADERS}
)

file(GLOB_RECURSE SRC_DIRS LIST_DIRECTORIES true "${SRC_DIR}/*")
set(INCLUDE_DIRS "${SRC_DIR}")
foreach(DIR ${SRC_DIRS})
    if(IS_DIRECTORY ${DIR})
        list(APPEND INCLUDE_DIRS ${DIR})
    endif()
endforeach()

file(GLOB_RECURSE XCFRAMEWORKS "${LIB_DIR}/*.xcframework")
set(FRAMEWORK_INCLUDE_DIRS "")
foreach(XCFW ${XCFRAMEWORKS})
    set(ARCH_DIR "${XCFW}/ios-arm64")
    if(EXISTS ${ARCH_DIR})
        file(GLOB INCLUDE_CANDIDATES "${ARCH_DIR}/*.framework/Headers" "${ARCH_DIR}/include" "${ARCH_DIR}/Headers")
        foreach(INC ${INCLUDE_CANDIDATES})
            if(IS_DIRECTORY ${INC})
                list(APPEND FRAMEWORK_INCLUDE_DIRS ${INC})
            endif()
        endforeach()
    endif()
endforeach()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${INCLUDE_DIRS}
    ${FRAMEWORK_INCLUDE_DIRS}
    "${LIB_DIR}/include"
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

if(EXISTS "${LIB_DIR}/curlpp/curl.xcframework/ios-arm64/curl.framework/curl")
    target_link_libraries(${PROJECT_NAME}
        "${LIB_DIR}/curlpp/curl.xcframework/ios-arm64/curl.framework/curl"
    )
elseif(EXISTS "${LIB_DIR}/curlpp/curl.xcframework/ios-arm64/libcurl.a")
    target_link_libraries(${PROJECT_NAME}
        "${LIB_DIR}/curlpp/curl.xcframework/ios-arm64/libcurl.a"
    )
endif()

if(EXISTS "${LIB_DIR}/curlpp/curlpp.xcframework/ios-arm64/libcurlpp.a")
    target_link_libraries(${PROJECT_NAME}
        "${LIB_DIR}/curlpp/curlpp.xcframework/ios-arm64/libcurlpp.a"
    )
endif()

foreach(boost_lib container json regex system)
    if(EXISTS "${LIB_DIR}/boost/boost_${boost_lib}.xcframework/ios-arm64/libboost_${boost_lib}.a")
        target_link_libraries(${PROJECT_NAME}
            "${LIB_DIR}/boost/boost_${boost_lib}.xcframework/ios-arm64/libboost_${boost_lib}.a"
        )
    endif()
endforeach()

if(EXISTS "${LIB_DIR}/aateam/libnetsess.xcframework/ios-arm64/libnetsess.a")
    target_link_libraries(${PROJECT_NAME}
        "${LIB_DIR}/aateam/libnetsess.xcframework/ios-arm64/libnetsess.a"
    )
endif()

if(EXISTS "${LIB_DIR}/aateam/libanixart.xcframework/ios-arm64/libanixart.a")
    target_link_libraries(${PROJECT_NAME}
        "${LIB_DIR}/aateam/libanixart.xcframework/ios-arm64/libanixart.a"
    )
endif()

target_compile_options(${PROJECT_NAME} PRIVATE
    -fobjc-arc
    -Wall
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST "${RES_DIR}/Info.plist"
)

set(ASSET_CATALOG "${RES_DIR}/Assets.xcassets")
if(EXISTS ${ASSET_CATALOG})
    set_source_files_properties(${ASSET_CATALOG} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    target_sources(${PROJECT_NAME} PRIVATE ${ASSET_CATALOG})
endif()

file(GLOB_RECURSE STRING_FILES "${LANG_DIR}/*.lproj/Localizable.strings")
foreach(str_file ${STRING_FILES})
    string(REPLACE "${LANG_DIR}/" "" rel_path ${str_file})
    get_filename_component(lproj_dir ${rel_path} DIRECTORY)
    set_source_files_properties(${str_file} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/${lproj_dir}"
    )
    target_sources(${PROJECT_NAME} PRIVATE ${str_file})
endforeach()

set(ENTITLEMENTS_FILE "${ENTITLE_DIR}/AniSaturn.entitlements")
if(EXISTS ${ENTITLEMENTS_FILE})
    set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${ENTITLEMENTS_FILE}"
    )
endif()

set(VER_SHORT "" CACHE STRING "Marketing version")
set(VER_BUILD "" CACHE STRING "Build number")
if(VER_SHORT)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_MARKETING_VERSION "${VER_SHORT}"
    )
endif()
if(VER_BUILD)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        XCODE_ATTRIBUTE_CURRENT_PROJECT_VERSION "${VER_BUILD}"
    )
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
)