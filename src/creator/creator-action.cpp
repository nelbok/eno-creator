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


#include "creator/creator-action.hpp"
#include "creator/creator.hpp"
#include "creator-item.hpp"

#include <QDebug>
#include <QMouseEvent>

namespace srp_creator
{

  CreatorAction::CreatorAction(srp_creator::Creator* creator)
    : value_(CreatorAction::Add)
    , line_(0)
    , pressed_(false)
    , pos_x_(0)
    , pos_y_(0)
    , creator_(creator)
  {
  }

  CreatorAction::~CreatorAction()
  {
  }


  int CreatorAction::pos_x() const
  {
    return pos_x_;
  }

  int CreatorAction::pos_y() const
  {
    return pos_y_;
  }


  void CreatorAction::value(CreatorAction::type_action val)
  {
    value_ = val;
  }

  CreatorAction::type_action CreatorAction::value() const
  {
    return value_;
  }


  void CreatorAction::mousePressEvent(QMouseEvent* event)
  {
    pressed_ = true;
    if (value_ == CreatorAction::Size)
    {
      const QPointF& position = creator_->mapToScene(event->pos());
      int pos_x = position.x();
      int pos_y = position.y();

      if (pos_x - 5 < creator_->map().x_min() and pos_x + 5 > creator_->map().x_min())
        line_ |= 0x1000;
      if (pos_x - 5 < creator_->map().x_max() and pos_x + 5 > creator_->map().x_max())
        line_ |= 0x0100;
      if (pos_y - 5 < creator_->map().y_min() and pos_y + 5 > creator_->map().y_min())
        line_ |= 0x0010;
      if (pos_y - 5 < creator_->map().y_max() and pos_y + 5 > creator_->map().y_max())
        line_ |= 0x0001;
    }
    calculate_pos(event);
  }

  void CreatorAction::mouseMoveEvent(QMouseEvent* event)
  {
    if (value_ == CreatorAction::Size and not pressed_)
    {
      const QPointF& position = creator_->mapToScene(event->pos());
      int pos_x = position.x();
      int pos_y = position.y();

      int line = 0;
      if (pos_x - 5 < creator_->map().x_min() and pos_x + 5 > creator_->map().x_min())
        line |= 0x1000;
      if (pos_x - 5 < creator_->map().x_max() and pos_x + 5 > creator_->map().x_max())
        line |= 0x0100;
      if (pos_y - 5 < creator_->map().y_min() and pos_y + 5 > creator_->map().y_min())
        line |= 0x0010;
      if (pos_y - 5 < creator_->map().y_max() and pos_y + 5 > creator_->map().y_max())
        line |= 0x0001;

      QCursor cursor = creator_->cursor();
      cursor.setShape(Qt::ArrowCursor);
      if ((line & 0x1000) == 0x1000 || (line & 0x0100) == 0x0100)
        cursor.setShape(Qt::SizeHorCursor);
      if ((line & 0x0010) == 0x0010 || (line & 0x0001) == 0x0001)
        cursor.setShape(Qt::SizeVerCursor);
      if ((line & 0x1010) == 0x1010 || (line & 0x0101) == 0x0101)
        cursor.setShape(Qt::SizeFDiagCursor);
      if ((line & 0x0110) == 0x0110 || (line & 0x1001) == 0x1001)
        cursor.setShape(Qt::SizeBDiagCursor);
      creator_->setCursor(cursor);
    }

    calculate_pos(event);
    if (pressed_)
      execute();
  }

  void CreatorAction::mouseReleaseEvent(QMouseEvent* event)
  {
    calculate_pos(event);
    if (pressed_)
      execute();
    pressed_ = false;
    line_ = 0;
  }


  void CreatorAction::calculate_pos(QMouseEvent *event)
  {
    const QPointF& position = creator_->mapToScene(event->pos());
    int pos_x = position.x();
    int pos_y = position.y();

    if (pos_x < 0)
      pos_x-=10;
    if (pos_y < 0)
      pos_y-=10;

    pos_x_ = pos_x /10 *10;
    pos_y_ = pos_y /10 *10;

    emit sig_pos_updated();
  }

  void CreatorAction::execute()
  {
    qDebug() << "Debug :: CreatorAction :: execute : " << pos_x_ << " | " << pos_y_;

    switch (value_)
    {
      case CreatorAction::Suppress:
        execute_suppress();
        break;
      case CreatorAction::Add:
        execute_add();
        break;
      case CreatorAction::Color:
        execute_color();
        break;
      case CreatorAction::Size:
        execute_size();
        break;
    }
  }

  void CreatorAction::execute_suppress()
  {
    int mid_w = creator_->width_pen() / 2;
    int mod_w = creator_->width_pen() % 2;

    for (int i = -mid_w; i < mid_w + mod_w; ++i)
      for (int j = -mid_w; j < mid_w + mod_w; ++j)
      {
        CreatorItem* item = dynamic_cast<CreatorItem*> (creator_->scene().itemAt(pos_x_ + 5 + i * 10, pos_y_ + 5 + j * 10, QTransform()));
        if (item && item->zValue() == creator_->depth())
        {
          creator_->map().remove_item(QVector3D(pos_x_ + i * 10, pos_y_ + j * 10, creator_->depth()));
          creator_->map().is_modified(true);
          delete item;
        }
      }
  }

  void CreatorAction::execute_add()
  {
    int mid_w = creator_->width_pen() / 2;
    int mod_w = creator_->width_pen() % 2;

    for (int i = -mid_w; i < mid_w + mod_w; ++i)
      for (int j = -mid_w; j < mid_w + mod_w; ++j)
      {
        CreatorItem* item = dynamic_cast<CreatorItem*> (creator_->scene().itemAt(pos_x_ + 5 + i * 10, pos_y_ + 5 + j * 10, QTransform()));
        if (item && item->zValue() == creator_->depth())
        {
          creator_->map().remove_item(QVector3D(pos_x_ + i * 10, pos_y_ + j * 10, creator_->depth()));
          creator_->map().is_modified(true);
          delete item;
        }

        if (creator_->map().rect().contains(pos_x_ + i * 10, pos_y_ + j * 10))
        {
          item = new CreatorItem(creator_->color(), creator_->depth());
          item->setPos(QVector3D(pos_x_ + i * 10, pos_y_ + j * 10, creator_->depth()));
          creator_->scene().addItem(item);

          creator_->map().add_item(QVector3D(pos_x_ + i * 10, pos_y_ + j * 10, creator_->depth()), creator_->color());
          creator_->map().is_modified(true);

        }
      }
  }

  void CreatorAction::execute_color()
  {
    CreatorItem* item = dynamic_cast<CreatorItem*> (creator_->scene().itemAt(pos_x_ + 5, pos_y_ + 5, QTransform()));

    if (item)
      creator_->color(item->color());
  }

  void CreatorAction::execute_size()
  {
    if (line_ & 0x1000)
      creator_->map().x_min(pos_x_);
    if (line_ & 0x0100)
      creator_->map().x_max(pos_x_);
    if (line_ & 0x0010)
      creator_->map().y_min(pos_y_);
    if (line_ & 0x0001)
      creator_->map().y_max(pos_y_);

    if (line_ > 0)
    {
      creator_->map().is_modified(true);
      creator_->show_floor();
      creator_->update_scene_rect();
    }
  }
}
