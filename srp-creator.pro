#  SRP-Creator, map editor
#  Copyright (C) 2011  Shadow Revival
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# -------------------------------------------------
# Project SRP Creator 2011 - 2012
# -------------------------------------------------

TEMPLATE = app
QT += widgets
TARGET = 3DCreator

# Source

HEADERS += \
    src/creator/creator-action.hpp \
    src/creator/creator.hpp \
    src/creator/creator-item.hpp \
    src/data/map.hpp \
    src/data/misc.hpp \
    src/files/creator-eno.hpp \
    src/files/creator-wavefront.hpp \
    src/help/help.hpp \
    src/opengl/opengl.hpp \
    src/creator-preference.hpp \
    src/creator-settings.hpp \
    src/creator-undoredo.hpp \
    src/creator-window.hpp \

SOURCES += \
    src/creator/creator-action.cpp \
    src/creator/creator-item.cpp \
    src/creator/creator.cpp \
    src/data/map.cpp \
    src/data/misc.cpp \
    src/files/creator-eno.cpp \
    src/files/creator-wavefront.cpp \
    src/help/help.cpp \
    src/opengl/opengl.cpp \
    src/creator-preference.cpp \
    src/creator-settings.cpp \
    src/creator-undoredo.cpp \
    src/creator-window.cpp \
    src/main.cpp \

TRANSLATIONS += \
    lang/fr_FR.ts \
    lang/en_US.ts

RESOURCES += \
    src/icons/icons.qrc \

FORMS += \
    src/creator-preference.ui \
    src/creator-window.ui \

INCLUDEPATH += \
    src

# Other files

DISTFILES += lang/en_US.ts
DISTFILES += lang/fr_FR.ts
