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


#ifndef CREATORWAVEFRONT_HPP
# define CREATORWAVEFRONT_HPP

# include <QMultiHash>
# include <QRgb>
# include <QVector>
# include <QVector3D>

namespace srp_creator
{
  class map;

  class CreatorWavefront
  {
    typedef QMultiHash<QRgb, QVector3D> MapColor;

    public:
      CreatorWavefront();

      bool generate(const map& m, QString& dir) const;

    private:
      void get_obj_colors(MapColor& map_color, const map& m) const;

      bool write_obj_file(const QString& filename, const QString& file_mtl, const MapColor& map_color) const;
      void write_obj_line(QByteArray& tampon, int& id_vertex, const MapColor& map_color) const;

      bool write_mtl_file(const QString& filename, const MapColor& map_color) const;
      void write_mtl_line(QByteArray& tampon, const MapColor& map_color) const;
  };
}

#endif // CREATORWAVEFRONT_HPP
