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


#include "data/map.hpp"

#include <QRect>

namespace srp_creator
{
  map::map()
  {
  }


  void map::reset()
  {
    file_name("");
    is_modified(false);

    x_min(-50);
    x_max(50);
    y_min(-50);
    y_max(50);
    z_min(0);
    z_max(0);

    floor_.clear();
  }

  const QString& map::file_name() const
  {
    return file_name_;
  }

  void map::file_name(const QString& str)
  {
    file_name_ = str;
    emit sig_file_name_updated(file_name_);
  }


  bool map::is_modified() const
  {
    return is_modified_;
  }

  void map::is_modified(bool val)
  {
    is_modified_ = val;
    emit sig_is_modified_updated(is_modified_);
  }


  QRect map::rect() const
  {
    return QRect(x_min_, y_min_, x_max_ - x_min_, y_max_ - y_min_);
  }

  int map::x_min() const
  {
    return x_min_;
  }

  int map::x_max() const
  {
    return x_max_;
  }

  int map::y_min() const
  {
    return y_min_;
  }

  int map::y_max() const
  {
    return y_max_;
  }

  int map::z_min() const
  {
    return z_min_;
  }

  int map::z_max() const
  {
    return z_max_;
  }

  int map::z_delta() const
  {
    return z_max_ - z_min_ + 1;
  }


  void map::x_min(int val)
  {
    x_min_ = val;
    if (x_min_ > -50)
      x_min_ = -50;
    update_map();
    emit sig_boundingbox_updated();
  }

  void map::x_max(int val)
  {
    x_max_ = val;
    if (x_max_ < 50)
      x_max_ = 50;
    update_map();
    emit sig_boundingbox_updated();
  }

  void map::y_min(int val)
  {
    y_min_ = val;
    if (y_min_ > -50)
      y_min_ = -50;
    update_map();
    emit sig_boundingbox_updated();
  }

  void map::y_max(int val)
  {
    y_max_ = val;
    if (y_max_ < 50)
      y_max_ = 50;
    update_map();
    emit sig_boundingbox_updated();
  }

  void map::z_min(int val)
  {
    z_min_ = val;
  }

  void map::z_max(int val)
  {
    z_max_ = val;
  }


  const MapItem& map::items() const
  {
    return floor_;
  }

  MapItem map::floor(int depth) const
  {
    MapItem floor;

    MapItem::const_iterator i = floor_.constBegin();
    while (i != floor_.constEnd())
    {
      if (i.key().z() == depth or i.key().z() == depth - 10)
        floor.insert(i.key(), i.value());
      ++i;
    }

    return floor;
  }

  void map::add_item(const QVector3D& p, const QColor& c)
  {
    floor_.insert(p, c);
    if (p.z() > z_max_)
      z_max_ = p.z();
    if (p.z() < z_min_)
      z_min_ = p.z();
  }

  void map::remove_item(const QVector3D& p)
  {
    floor_.remove(p);
  }


  void map::update_map()
  {
    MapItem local_map = floor_;
    MapItem::const_iterator items = local_map.constBegin();
    while (items != local_map.constEnd())
    {
      int x = items.key().x();
      int y = items.key().y();
      if (x_min_ > x or x >= x_max_ or y_min_ > y or y >= y_max_)
        floor_.remove(items.key());
      ++items;
    }
  }
}
