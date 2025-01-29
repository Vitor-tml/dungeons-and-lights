# Versão mínima requerida do CMake
cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(dungeon)

# Definir a versão do C++
set(CMAKE_CXX_STANDARD 17)

# Localização do SFML
set(SFML_DIR "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/lib/cmake/SFML")

# Encontrar e carregar os pacotes SFML
find_package(SFML 2.6.1 REQUIRED COMPONENTS system window graphics network audio)

# Adicionar arquivos de origem e cabeçalhos
file(GLOB_RECURSE SOURCES "src/*.cpp")
# file(GLOB_RECURSE HEADERS "src/*.h")

# Criar o executável
add_executable(dungeon ${SOURCES} ${HEADERS})

set_target_properties(dungeon PROPERTIES 
    WIN32_EXECUTABLE OFF  # Garante que seja um aplicativo de console
)
# Vincular as bibliotecas SFML corretamente
target_link_libraries(dungeon 
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
)

# Copiar as DLLs para o diretório de saída
if (MSYS)
    message(STATUS "Detectado Sistema Windows. Copiando DLLs")
    add_custom_command(TARGET dungeon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/openal32.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-audio-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-audio-d-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-graphics-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-graphics-d-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-network-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-network-d-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-system-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-system-d-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-window-2.dll"
            "${CMAKE_SOURCE_DIR}/libs/SFML-2.6.1/bin/sfml-window-d-2.dll"
            $<TARGET_FILE_DIR:dungeon>
        COMMENT "Copying SFML DLLs to the output directory"
    )
endif()