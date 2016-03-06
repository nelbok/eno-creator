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


#ifndef CREATORSETTINGS_HPP
# define CREATORSETTINGS_HPP

#include <QSettings>

namespace srp_creator
{
  class CreatorSettings
  {
    public:
      CreatorSettings();

      QStringList do_load_recent_files();
      void do_save_recent_files(const QStringList& list);

      QColor do_load_color();
      void do_save_color(const QColor& color);

      QString do_load_obj_directory();
      void do_save_obj_directory(const QString& dir);

      QString do_load_eno_directory();
      void do_save_eno_directory(const QString& dir);

      QString do_load_language();
      void do_save_language(const QString& language);

      QRect do_load_dimension();
      void do_save_dimension(const QRect& dimension);

    private:
      QSettings settings_;
  };
}

#endif // CREATORSETTINGS_HPP
