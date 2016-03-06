/**
  SRP-Creator, map editor
  Copyright (C) 2011  Shadow Revival

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
                                                                      **/


#include "creator-settings.hpp"

#include <QColor>
#include <QDir>
#include <QRect>
#include <QStringList>

namespace srp_creator
{
  CreatorSettings::CreatorSettings()
    : settings_(QSettings::IniFormat, QSettings::UserScope, "Shadow Revival", "SRP-Creator")
  {
  }


  QStringList CreatorSettings::do_load_recent_files()
  {
    QStringList list;

    int size = settings_.beginReadArray("RecentFiles");
    for (int i = 0; i < size; ++i)
    {
      settings_.setArrayIndex(i);
      list.append(settings_.value("file").toString());
    }
    settings_.endArray();

    return list;
  }

  void CreatorSettings::do_save_recent_files(const QStringList& list)
  {
    settings_.beginWriteArray("RecentFiles");
    for (int i = 0; i < list.size() and i < 10; ++i)
    {
        settings_.setArrayIndex(i);
        settings_.setValue("file", list.at(i));
    }
    settings_.endArray();
  }


  QColor CreatorSettings::do_load_color()
  {
    QString color = settings_.value("color").toString();
    if (color.isEmpty())
      color = "#ffaa00";
    return QColor(color);
  }

  void CreatorSettings::do_save_color(const QColor& color)
  {
    settings_.setValue("color", color.name());
  }


  QString CreatorSettings::do_load_eno_directory()
  {
    QString eno_directory = settings_.value("eno_directory").toString();
    if (eno_directory.isEmpty())
      eno_directory = QDir::homePath();
    return eno_directory;
  }

  void CreatorSettings::do_save_eno_directory(const QString& dir)
  {
    settings_.setValue("eno_directory", dir);
  }


  QString CreatorSettings::do_load_obj_directory()
  {
    QString obj_directory = settings_.value("obj_directory").toString();
    if (obj_directory.isEmpty())
      obj_directory = QDir::homePath();
    return obj_directory;
  }

  void CreatorSettings::do_save_obj_directory(const QString& dir)
  {
    settings_.setValue("obj_directory", dir);
  }


  QString CreatorSettings::do_load_language()
  {
    QString language = settings_.value("language").toString();
    if (language.isEmpty())
      language = "en_US";
    return language;
  }

  void CreatorSettings::do_save_language(const QString& language)
  {
    settings_.setValue("language", language);
  }


  QRect CreatorSettings::do_load_dimension()
  {
    QRect dimension = settings_.value("dimension").toRect();
    if (dimension.isNull())
      dimension = QRect(-5, -5, 10, 10);
    return dimension;
  }

  void CreatorSettings::do_save_dimension(const QRect& dimension)
  {
    settings_.setValue("dimension", dimension);
  }
}
