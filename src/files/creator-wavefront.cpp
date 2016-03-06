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


#include "creator/creator-item.hpp"
#include "creator/creator.hpp"
#include "files/creator-wavefront.hpp"

#include <QFile>
#include <QFileDialog>
#include <QDebug>

namespace srp_creator
{
  CreatorWavefront::CreatorWavefront()
  {

  }


  bool CreatorWavefront::generate(const map& m, QString& dir) const
  {
    QString fileName = QFileDialog::getSaveFileName(0, QObject::tr("Choose file for generate..."), dir, QObject::tr("Obj (*.obj)"));

    if (fileName.isEmpty())
      return false;

    QFileInfo fileinfo(fileName);

    if (fileinfo.suffix() != "obj")
      fileName.append(QString(".%1").arg("obj"));

    qDebug() << "Debug :: CreatorWavefront :: " << fileName;

    dir = fileinfo.absolutePath();

    //Initialization of map_color
    MapColor map_color;

    get_obj_colors(map_color, m);

    bool success;
    success = write_obj_file(fileName, QString("%1.mtl").arg(fileinfo.completeBaseName()), map_color);
    if (success)
      success = write_mtl_file(QString("%1/%2.mtl").arg(fileinfo.absolutePath()).arg(fileinfo.completeBaseName()), map_color);
    return success;
  }


  void CreatorWavefront::get_obj_colors(MapColor& map_color, const map& m) const
  {
    MapItem::const_iterator items = m.items().constBegin();
    while (items != m.items().constEnd())
    {
        int x =  items.key().x();
        int y = -items.key().y() - 10;
        int z =  items.key().z();
        QRgb color = items.value().rgb();
        map_color.insert(color, QVector3D(x, y, z));
        ++items;
    }
  }


  bool CreatorWavefront::write_obj_file(const QString& filename, const QString& file_mtl, const MapColor& map_color) const
  {
    /**
      # comment
      mtllib name mtl
      v vertex (x, y z)
      g group
      usemtl mtl
      s smooth
      f face (v1, v2, v3[,v4])
      **/


    QByteArray tampon;
    int id_vertex = 0;

    QFile file_obj(filename);

    if (!file_obj.open(QIODevice::WriteOnly))
        return false;
    tampon.append("# Generate with srpa\n");
    tampon.append(QString("mtllib %1\n").arg(file_mtl));

    write_obj_line(tampon, id_vertex, map_color);

    file_obj.write(tampon);
    file_obj.close();

    return true;
  }

  void CreatorWavefront::write_obj_line(QByteArray& tampon, int& id_vertex, const MapColor& map_color) const
  {
    QList<QRgb> keys = map_color.uniqueKeys();
    QString vertex;
    QString faces;
    for (int i=0; i < keys.size(); ++i)
    {
      vertex.clear();
      faces.clear();
      QList<QVector3D> values = map_color.values(keys.at(i));
      for (int j=0; j < values.size(); ++j)
      {
        int x = values.at(j).x();
        int y = values.at(j).y();
        int z = values.at(j).z();

        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10)    .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10+1)  .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10)    .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10+1)  .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10)    .arg(z/10+1));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10+1)  .arg(z/10+1));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10)    .arg(z/10+1));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10+1)  .arg(z/10+1));
        faces.append(QString("f %1 %2 %3 %4\n").arg(1+id_vertex).arg(2+id_vertex).arg(4+id_vertex).arg(3+id_vertex));
        faces.append(QString("f %1 %2 %3 %4\n").arg(1+id_vertex).arg(2+id_vertex).arg(6+id_vertex).arg(5+id_vertex));
        faces.append(QString("f %1 %2 %3 %4\n").arg(1+id_vertex).arg(3+id_vertex).arg(7+id_vertex).arg(5+id_vertex));
        faces.append(QString("f %1 %2 %3 %4\n").arg(5+id_vertex).arg(6+id_vertex).arg(8+id_vertex).arg(7+id_vertex));
        faces.append(QString("f %1 %2 %3 %4\n").arg(4+id_vertex).arg(3+id_vertex).arg(7+id_vertex).arg(8+id_vertex));
        faces.append(QString("f %1 %2 %3 %4\n").arg(4+id_vertex).arg(2+id_vertex).arg(6+id_vertex).arg(8+id_vertex));
        id_vertex += 8;

        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10)    .arg(z/10+.5));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10+1)  .arg(z/10+.5));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10)    .arg(z/10+.5));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10+1)  .arg(z/10+.5));
        faces.append(QString("f %1 %2 %3 %4\n").arg(1+id_vertex).arg(2+id_vertex).arg(4+id_vertex).arg(3+id_vertex));
        id_vertex += 4;

        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10+.5) .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10+.5) .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10)     .arg(y/10+.5) .arg(z/10+1));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+1)   .arg(y/10+.5) .arg(z/10+1));
        faces.append(QString("f %1 %2 %3 %4\n").arg(1+id_vertex).arg(2+id_vertex).arg(4+id_vertex).arg(3+id_vertex));
        id_vertex += 4;

        vertex.append(QString("v %1 %2 %3\n").arg(x/10+.5)  .arg(y/10)    .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+.5)  .arg(y/10+1)  .arg(z/10));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+.5)  .arg(y/10)    .arg(z/10+1));
        vertex.append(QString("v %1 %2 %3\n").arg(x/10+.5)  .arg(y/10+1)  .arg(z/10+1));
        faces.append(QString("f %1 %2 %3 %4\n").arg(1+id_vertex).arg(2+id_vertex).arg(4+id_vertex).arg(3+id_vertex));
        id_vertex += 4;
      }

      tampon.append(vertex);
      tampon.append(QString("g %1\n").arg(keys.at(i)));
      tampon.append(QString("usemtl %1\n").arg(keys.at(i)));
      tampon.append("s 1\n");
      tampon.append(faces);
    }
  }


  bool CreatorWavefront::write_mtl_file(const QString& filename, const MapColor& map_color) const
  {
    /**
      For me : (because the equation is too complex //FIX ME)
      mtl's color goes from 0 to 0.8
      QColor's color goes from 0 to 255
      So mtl's color = QColor's color * 0.8 / 255
      # comment
      newmtl name mtl
      Ns don't known
      Ka don't known
      Kd red green blue
      Ni don't known
      d don't known
      illum don't known
      **/


    QByteArray tampon;
    QFile file_mtl(filename);

    if (!file_mtl.open(QIODevice::WriteOnly))
        return false;
    tampon.append("# Generate with srpa\n");

    write_mtl_line(tampon, map_color);

    file_mtl.write(tampon);
    file_mtl.close();

    return true;
  }

  void CreatorWavefront::write_mtl_line(QByteArray& tampon, const MapColor& map_color) const
  {
    QList<QRgb> keys = map_color.uniqueKeys();
    for (int i=0; i < keys.size(); ++i)
    {
      QRgb key = keys.at(i);
      QColor color(key);

      tampon.append(QString("newmtl %1\n").arg(key));
      tampon.append("Ns 96.078431\n");
      tampon.append("Ka 0.000000 0.000000 0.000000\n");
      tampon.append(QString("Kd %1 %2 %3\n")
              .arg(color.red() * 0.003)
              .arg(color.green() * 0.003)
              .arg(color.blue() * 0.003));
      tampon.append("Ks 0.000000 0.000000 0.000000\n");
      tampon.append("Ni 1.000000\n");
      tampon.append("d 1.000000\n");
      tampon.append("illum 1\n\n");

    }
  }
}
