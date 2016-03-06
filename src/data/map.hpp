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


#ifndef MAP_HPP
# define MAP_HPP

# include "data/misc.hpp"

# include <QColor>
# include <QMap>
# include <QObject>
# include <QVector3D>

class QRect;

namespace srp_creator
{
  class map : public QObject
  {
    Q_OBJECT

    public:
      map();

      void reset();

      const QString& file_name() const;
      void file_name(const QString& str);

      bool is_modified() const;
      void is_modified(bool val);

      QRect rect() const;
      int x_min() const;
      int x_max() const;
      int y_min() const;
      int y_max() const;
      int z_min() const;
      int z_max() const;
      int z_delta() const;

      void x_min(int val);
      void x_max(int val);
      void y_min(int val);
      void y_max(int val);
      void z_min(int val);
      void z_max(int val);

      const MapItem& items() const;
      MapItem floor(int depth) const;
      void add_item(const QVector3D& p, const QColor& c);
      void remove_item(const QVector3D& p);

    private:
      QString file_name_;
      bool is_modified_;

      int x_min_;
      int x_max_;
      int y_min_;
      int y_max_;
      int z_min_;
      int z_max_;

      MapItem floor_;

      void update_map();

    signals:
      void sig_boundingbox_updated();
      void sig_file_name_updated(const QString&);
      void sig_is_modified_updated(bool);
  };
}

#endif // MAP_HPP
