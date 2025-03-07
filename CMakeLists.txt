cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

#CMake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#if(EMSCRIPTEN)
#    #add_compile_options(
#    #   -target wasm64
#    #)
#else()
#    add_compile_options(
#        -Werror
#        -Wall
#    )
#endif()

project(wordle-client-qt)
message(STATUS "EMSCRIPTEN = ${EMSCRIPTEN}")
set(PROTOS_NAMES_
    uuid.proto
    enum_the_char_color.proto
    enum_check_the_row_result.proto
    request_check_the_row_body.proto
    request_new_game_body.proto
    response_check_the_row_body.proto
    response_new_game_body.proto
)

set(PROTO_FILES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/wordle-data/proto/")
set(PROTOS_PATHS_ ${PROTOS_NAMES_})
list(TRANSFORM PROTOS_PATHS_ PREPEND ${PROTO_FILES_PATH})

set(PROTO_GENERATED_PATH "${CMAKE_CURRENT_BINARY_DIR}/src/generated/")
file(MAKE_DIRECTORY ${PROTO_GENERATED_PATH})

if(EMSCRIPTEN)
    # Protobuf lib
    find_package(Protobuf REQUIRED)
    
    if (NOT Protobuf_FOUND)
        message(FATAL_ERROR "Protobuf lib not found!")
    endif()
    if(Protobuf_VERBOSE)
      message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")
    endif()
    # Protobuf lib

    # Abseil lib
    find_package(absl REQUIRED)
    if (NOT absl_FOUND)
        message(FATAL_ERROR "absl lib not found!")
    endif()

    set(protobuf_ABSL_USED_TARGETS
    absl::absl_check
    absl::absl_log
    absl::algorithm
    absl::base
    absl::bind_front
    absl::bits
    absl::btree
    absl::cleanup
    absl::cord
    absl::core_headers
    absl::debugging
    absl::die_if_null
    absl::dynamic_annotations
    absl::flags
    absl::flat_hash_map
    absl::flat_hash_set
    absl::function_ref
    absl::hash
    absl::layout
    absl::log_initialize
    absl::log_globals
    absl::log_severity
    absl::memory
    absl::node_hash_map
    absl::node_hash_set
    absl::optional
    absl::random_distributions
    absl::random_random
    absl::span
    absl::status
    absl::statusor
    absl::strings
    absl::synchronization
    absl::time
    absl::type_traits
    absl::utility
    absl::variant
    )
    # Abseil lib

    # utf8_range lib
    find_package(utf8_range REQUIRED)
    if (NOT utf8_range_FOUND)
        message(FATAL_ERROR "utf8_range lib not found!")
    endif()
    # utf8_range lib

    # protoc
    find_program(PROTOC
                 protoc
                 REQUIRED
                 )
    if (NOT PROTOC)
        message(FATAL_ERROR "protoc binary not found!")
    endif()
    message(STATUS call "${PROTOC} --proto_path=${PROTO_FILES_PATH} --cpp_out=${PROTO_GENERATED_PATH} ${PROTOS_PATHS_}")
    execute_process(
        COMMAND ${PROTOC} --proto_path=${PROTO_FILES_PATH} --cpp_out=${PROTO_GENERATED_PATH} ${PROTOS_PATHS_}
    )
    foreach(PROTO_NAME ${PROTOS_NAMES_})
        string(REPLACE ".proto" ".pb.h" PROTO_GENERATED_H ${PROTO_NAME})
        string(REPLACE ".proto" ".pb.cc" PROTO_GENERATED_CPP ${PROTO_NAME})
        list(APPEND GENERATED_H_CPP_FILES ${PROTO_GENERATED_H})
        list(APPEND GENERATED_H_CPP_FILES ${PROTO_GENERATED_CPP})
    endforeach()
    list(TRANSFORM GENERATED_H_CPP_FILES PREPEND ${PROTO_GENERATED_PATH})
    message(STATUS "GENERATED_H_CPP_FILES = ${GENERATED_H_CPP_FILES}")
    add_library(${CMAKE_PROJECT_NAME}-generated STATIC ${GENERATED_H_CPP_FILES})
    # protoc

    target_link_libraries(${CMAKE_PROJECT_NAME}-generated
        ${protobuf_ABSL_USED_TARGETS}
        protobuf::libprotobuf
        /emsdk/upstream/emscripten/cache/sysroot/lib/libutf8_range.a
        /emsdk/upstream/emscripten/cache/sysroot/lib/libutf8_validity.a)
    

    # protobuf_generate
    #TODO: instead of execute_process here use protobuf_genereate+PROTOC_EXE when cmake 4.0.0++ will be in kitware repo
    #add_library(${CMAKE_PROJECT_NAME}-generated STATIC ${PROTOS_NAMES_})
    #protobuf_generate (
    #    TARGET ${PROJECT_NAME}-generated
    #   LANGUAGE cpp
    #    PROTOC_OUT_DIR  ${PROTO_GENERATED_PATH}
    #    OUT_VAR GENERATED_H_CPP_FILES
    #    PROTOS ${PROTOS_}
    #    IMPORT_DIRS ${PROTO_FILES_PATH}
    #    PROTOC_EXE ${PROTOC}
    #)
    # protobuf_generate
else()
    # Protobuf lib and protoc
    find_package(Protobuf REQUIRED)
    if (NOT Protobuf_FOUND)
        message(FATAL_ERROR "Protobuf lib not found!")
    endif()
    if(Protobuf_VERBOSE)
      message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")
    endif()
    add_library(${PROJECT_NAME}-generated STATIC ${PROTOS_NAMES_})
    protobuf_generate (
        TARGET ${PROJECT_NAME}-generated
        LANGUAGE cpp
        PROTOC_OUT_DIR  ${PROTO_GENERATED_PATH}
        OUT_VAR GENERATED_H_CPP_FILES
        PROTOS ${PROTOS_NAMES_}
        IMPORT_DIRS ${PROTO_FILES_PATH}
    )
    target_link_libraries(${PROJECT_NAME}-generated PUBLIC protobuf::libprotobuf)
    # Protobuf
endif(EMSCRIPTEN)

# Qt
set(CMAKE_AUTOMOC ON)
message(STATUS "CMAKE_CXX_COMPILE_FEATURES = ${CMAKE_CXX_COMPILE_FEATURES}")
message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Network)

set(SOURCE_FILES__
    main.cxx
    GameState.cxx GameState.h
    WidgetButtons.cxx WidgetButtons.h
    WidgetKeyboard.cxx WidgetButtons.h
    WidgetGame.h WidgetGame.cxx
    WidgetApplication.h WidgetApplication.cxx
    WidgetApplicationLogic.h WidgetApplicationLogic.cxx
    APIApplicationLogic.h APIApplicationLogic.cxx
    WidgetAuthorization.h WidgetAuthorization.cxx
    WidgetMenu.h WidgetMenu.cxx
    WidgetTopMenu.h WidgetTopMenu.cxx
)
list(TRANSFORM SOURCE_FILES__ PREPEND "src/")
if(EMSCRIPTEN)
    qt_add_executable(${CMAKE_PROJECT_NAME}
                      ${SOURCE_FILES__}
                      )
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "--preload-file ../deploy/wordle-client-qt/words/words.txt@/opt/wordle-client-qt/words/words.txt")
    target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -sASYNCIFY -Os)
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
                          Qt${QT_VERSION_MAJOR}::Gui
                          Qt${QT_VERSION_MAJOR}::Widgets
                          ${CMAKE_PROJECT_NAME}-generated
                          )
else()
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES__})
    target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets ${PROJECT_NAME}-generated)
endif()

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/src
                           ${CMAKE_CURRENT_SOURCE_DIR}/wordle-data/src
                           ${CMAKE_CURRENT_BINARY_DIR}/src/generated/
                           )
# Qt

# CPack
if(EMSCRIPTEN)
    #NOT DEFINED FOR WASM
else()
    set(CMAKE_INSTALL_PREFIX /opt/${CMAKE_PROJECT_NAME})

    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}
            DESTINATION bin
            PERMISSIONS WORLD_EXECUTE OWNER_WRITE OWNER_READ)

    execute_process(COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/symlink)
    execute_process(COMMAND ln -s /opt/${CMAKE_PROJECT_NAME}/bin/${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/symlink/${CMAKE_PROJECT_NAME})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/symlink/wordle-client-qt
            DESTINATION /usr/bin/
            PERMISSIONS WORLD_EXECUTE WORLD_READ)

    install(DIRECTORY deploy/wordle-client-qt/words
            DESTINATION ${CMAKE_INSTALL_PREFIX}
            FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ WORLD_READ
            DIRECTORY_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ WORLD_READ WORLD_WRITE WORLD_EXECUTE)

    install(FILES wordle-media/icons/wordle.svg
            DESTINATION /usr/share/icons/hicolor/scalable/apps/)

    install(FILES deploy/wordle-client-qt.desktop
            DESTINATION /usr/share/applications/)

    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
    set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
    set(CPACK_PACKAGE_CONTACT "chicherin.alexey@ya.ru")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/alec-chicherini/wordle-client-qt")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE amd64)
    set(CPACK_DEBIAN_PACKAGE_VERSION 25.02)
    set(CPACK_DEBIAN_FILE_NAME ${CPACK_DEBIAN_PACKAGE_NAME}_${CPACK_DEBIAN_PACKAGE_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}.deb)
    if(${QT_VERSION_MAJOR} EQUAL 5)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS libqt5widgets5|libqt5widgets5t64 libqt5gui5|libqt5gui5t64 libqt5core5|libqt5core5a|libqt5core5t64 )
    elseif(${QT_VERSION_MAJOR} EQUAL 6)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS libqt6widgets6|libqt6widgets6t64 libqt6gui6|libqt6gui6t64 libqt6core6|libqt6core6t64 )
    else()
        message(FATAL_ERROR "Qt${QT_VERSION_MAJOR} not supported")
    endif()

    include(CPack)
endif()
# CPack