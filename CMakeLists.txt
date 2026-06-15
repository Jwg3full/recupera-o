cmake_minimum_required(VERSION 3.9)
project("Grafico")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw-3.4)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glm-master)

file(GLOB SOURCES "src/*.cpp")

add_executable(meuAPP main.cpp ${SOURCES})

target_include_directories(meuAPP PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_definitions(meuAPP PRIVATE GLEW_STATIC)

target_include_directories(meuAPP PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.3.1/include")

target_link_libraries(meuAPP PRIVATE 
    glfw 
    glm
    "${CMAKE_CURRENT_SOURCE_DIR}/external/glew-2.3.1/lib/Release/x64/glew32s.lib"
    opengl32
)

add_custom_command(TARGET meuAPP POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:meuAPP>/assets"
)

set(SHADER_FILES
    "_camadas_vs.glsl"
    "_camadas_fs.glsl"
    "_sprites_fs.glsl"
    "_sprites_vs.glsl"
)

foreach(SHADER ${SHADER_FILES})
    add_custom_command(
        TARGET meuAPP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}"
            "$<TARGET_FILE_DIR:meuAPP>/${SHADER}"
        COMMENT "Copiando shader ${SHADER} para o diretorio de build..."
    )
endforeach()






