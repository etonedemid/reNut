set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(TRIPLE "aarch64-linux-gnu")

set(CMAKE_C_COMPILER   "${TRIPLE}-gcc")
set(CMAKE_CXX_COMPILER "${TRIPLE}-g++")
set(CMAKE_AR           "${TRIPLE}-ar")
set(CMAKE_RANLIB       "${TRIPLE}-ranlib")
set(CMAKE_STRIP        "${TRIPLE}-strip")

# Sysroot for the target
set(CMAKE_SYSROOT "/usr/${TRIPLE}")
set(CMAKE_FIND_ROOT_PATH "/usr/${TRIPLE}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# pkg-config must query aarch64 packages only
set(ENV{PKG_CONFIG_SYSROOT_DIR} "/usr/${TRIPLE}")
set(ENV{PKG_CONFIG_PATH}        "/usr/${TRIPLE}/lib/pkgconfig:/usr/lib/${TRIPLE}/pkgconfig:/usr/share/${TRIPLE}/pkgconfig")
