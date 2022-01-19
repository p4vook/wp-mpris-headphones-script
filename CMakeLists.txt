cmake_minimum_required(VERSION 3.16)
set(project_name wp-mpris-headphones-script)

project(${project_name}
        LANGUAGES C
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(PIPEWIRE REQUIRED IMPORTED_TARGET libpipewire-0.3)
pkg_check_modules(WIREPLUMBER REQUIRED IMPORTED_TARGET wireplumber-0.4)

add_executable(${project_name} main.c)

target_link_libraries(${project_name} PUBLIC PkgConfig::GLIB
                                             PkgConfig::PIPEWIRE
                                             PkgConfig::WIREPLUMBER)
