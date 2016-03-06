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
#include "files/creator-eno.hpp"

#include <QFile>
#include <QFileDialog>
#include <QDebug>

namespace srp_creator
{
  CreatorEno::CreatorEno(map& m, QString& dir)
    : map_(m)
    , directory_(dir)
  {

  }


  bool CreatorEno::save() const
  {
    if (map_.file_name().isEmpty())
      return save_as();
    else
      return do_save(map_.file_name());
  }

  bool CreatorEno::save_as() const
  {
    QString fileName = QFileDialog::getSaveFileName(0, QObject::tr("Save as..."), directory_, QObject::tr("Encoded Obj (*.eno)"));

    if (fileName.isEmpty())
      return false;

    if (QFileInfo(fileName).suffix() != "eno")
      fileName.append(QString(".%1").arg("eno"));

    qDebug() << "Debug :: CreatorEno :: " << fileName;

    directory_ = QFileInfo(fileName).absolutePath();

    return do_save(fileName);
  }

  bool CreatorEno::open(Creator& creator)
  {
    QString fileName = QFileDialog::getOpenFileName(0, QObject::tr("Open..."), directory_, QObject::tr("Encoded Obj (*.eno)"));

    if (fileName.isEmpty())
      return false;

    qDebug() << "Debug :: CreatorEno :: " << fileName;

    directory_ = QFileInfo(fileName).absolutePath();

    return do_load(fileName, creator);
  }

  bool CreatorEno::open(const QString& fileName, Creator& creator)
  {
    qDebug() << "Debug :: CreatorEno :: " << fileName;

    return do_load(fileName, creator);
  }


  bool CreatorEno::do_save(const QString& fileName) const
  {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream tampon(&file);
    tampon.setFieldWidth(8);

    tampon << map_.x_min() << map_.x_max() << map_.y_min() << map_.y_max();

    MapItem::const_iterator items = map_.items().constBegin();
    while (items != map_.items().constEnd())
    {
        endl(tampon);
        tampon << items.key().x() << items.key().y() << items.key().z() << items.value().name();
        ++items;
    }

    file.close();

    map_.file_name(fileName);
    map_.is_modified(false);

    return true;
  }

  bool CreatorEno::do_load(const QString &fileName, Creator& creator)
  {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream tampon(&file);
    tampon.setFieldWidth(8);

    int x_min, x_max, y_min, y_max;

    tampon >> x_min >> x_max >> y_min >> y_max;

    map_.x_min(x_min);
    map_.x_max(x_max);
    map_.y_min(y_min);
    map_.y_max(y_max);

    qDebug() << "Debug :: CreatorEno :: do_load_encoded_obj : " << x_min << " | " << x_max << " | " << y_min << " | " << y_max;
    while(not tampon.atEnd())
    {
      qreal x;
      qreal y;
      qreal z;
      QString color;
      tampon >> x >> y >> z >> color;

      qDebug() << "Debug :: CreatorEno :: do_load_encoded_obj : " << x << " | " << y << " | " << z;

      map_.add_item(QVector3D(x, y, z), QColor(color));
    }

    file.close();

    creator.show_floor();
    creator.update_scene_rect();
    map_.file_name(fileName);

    return true;
  }
}
