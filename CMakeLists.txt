cmake_minimum_required(VERSION 3.10)

project(RealTimeVidEd)

# Defina o caminho para o OpenCV (certifique-se de que este caminho esteja correto)
set(OpenCV_DIR "C:/Users/tvpre/opencv/build")

# Encontre o pacote OpenCV
find_package(OpenCV REQUIRED)

# Mostra a versão do OpenCV e o caminho dos arquivos de cabeçalho
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dir: ${OpenCV_INCLUDE_DIRS}")

# Adiciona o executável
add_executable(${PROJECT_NAME} src/main.cpp)

# Inclui os diretórios do OpenCV
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

# Linka as bibliotecas do OpenCV ao projeto
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
