
# Menentukan versi minimum Cmake yang dibutuhkan 
cmake_minimum_required(VERSION 3.8)

# MEndeklarasikan nama proyek dan menggunakna bahasa C++
project(dummy_ip_project CXX)

# Menentukan standar bahasa C++ yang digunakan, yaitu C++17
set(CMAKE_CXX_STANDARD 17)

# Mewajibkan penggunaan standar C++17
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Menentukan lojasi folder header atau library include GVSoC
set(GVSOC_ENGINE_INCLUDE "/home/oknielsilitonga/gvsoc/gvsoc/engine/engine/include")

# Membuat shared library bernama dummy_ip dari file dummy_ip.cpp
add_library(dummy_ip SHARED
    dummy_ip.cpp
)

# Menambahkan direktori pencarian file header untuk proses kompilasi
target_include_directories(dummy_ip PUBLIC
    
    # Menggunakan direktori tempat CMakeLists.txt berada
    ${CMAKE_CURRENT_SOURCE_DIR}
    
    # Menggunakan direktori header GVSoC yang telah ditentukan
    ${GVSOC_ENGINE_INCLUDE}
)
