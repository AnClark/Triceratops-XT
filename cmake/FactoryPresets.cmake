#  FactoryPresets.cmake - Factory bank (built-in presets) support
#
#  Copyright (C) 2023 AnClark Liu. All rights reserved.
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

add_subdirectory(utils/factory_bank_to_c)

# Handle cross-build stuff
if (CMAKE_CROSSCOMPILING)
    # Move factory_bank_to_c executable to CMAKE_BINARY_DIR for convenience
    set_target_properties (factory_bank_to_c PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set (factory_bank_to_c_PATH ./factory_bank_to_c${CMAKE_EXECUTABLE_SUFFIX})

    set_target_properties (binary_to_compressed_c PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set (binary_to_compressed_c_PATH ./binary_to_compressed_c${CMAKE_EXECUTABLE_SUFFIX})
else ()
    set (factory_bank_to_c_PATH factory_bank_to_c)
    set (binary_to_compressed_c_PATH binary_to_compressed_c)
endif ()

set(GENERATED_FACTORY_BANK_DIR "${CMAKE_BINARY_DIR}/factory_bank")
file(MAKE_DIRECTORY ${GENERATED_FACTORY_BANK_DIR})

# Add include directories
include_directories(
    ${GENERATED_FACTORY_BANK_DIR}
)

# Convert category file
if(NOT TR_USE_EXTERNAL_CATEGORY_FILE)
    set(SOURCES_CATEGORY_FILE
        ${GENERATED_FACTORY_BANK_DIR}/triceratops_categories.h
    )

    add_custom_command(
        OUTPUT ${SOURCES_CATEGORY_FILE}
        COMMAND ${binary_to_compressed_c_PATH} ${PROJECT_SOURCE_DIR}/src/triceratops_categories.txt Triceratops_Category_File > ${SOURCES_CATEGORY_FILE}
        DEPENDS binary_to_compressed_c
        VERBATIM
    )

    target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_CATEGORY_FILE})
endif()

# Convert factory banks
if(TR_BUILTIN_FACTORY_PRESETS)
    set(SOURCES_FACTORY_BANK
        ${GENERATED_FACTORY_BANK_DIR}/triceratops_factory_banks.cpp
        ${GENERATED_FACTORY_BANK_DIR}/triceratops_factory_banks.h
    )

    add_custom_command(
        OUTPUT ${SOURCES_FACTORY_BANK}
        COMMAND ${factory_bank_to_c_PATH} ${PROJECT_SOURCE_DIR}/src/presets.lv2 ${GENERATED_FACTORY_BANK_DIR}
        DEPENDS factory_bank_to_c
        VERBATIM
    )

    target_sources(${PROJECT_NAME} PRIVATE ${SOURCES_FACTORY_BANK})
endif()
