if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # additional warnings
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Gui)
find_package(Qt6 REQUIRED COMPONENTS Core Gui)
