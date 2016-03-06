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


#include "creator/creator.hpp"
#include "creator/creator-item.hpp"
#include "creator-settings.hpp"

namespace srp_creator
{

  Creator::Creator(QWidget* parent)
    : QGraphicsView(parent)
    , creator_action_(new CreatorAction(this))
    , color_(QColor("#ffaa00"))
    , width_pen_(1)
    , depth_(0)
    , zoom_(1)
  {
    connect(creator_action_, SIGNAL(sig_pos_updated()), SIGNAL(sig_pos_updated()));

    QPen pen;
    pen.setStyle(Qt::DashLine);
    x_line_.setPen(pen);
    y_line_.setPen(pen);

    setScene(&scene_);
    scene_.addItem(&rect_);
    scene_.addItem(&x_line_);
    scene_.addItem(&y_line_);
    rect_.setZValue(-1000);
    setMouseTracking(true);
    reset();
  }

  Creator::~Creator()
  {
    delete creator_action_;
  }


  void Creator::reset()
  {
    map_.reset();

    //Default dimension
    QRect dimension = CreatorSettings().do_load_dimension();
    map_.x_min(dimension.left() * 10);
    map_.x_max((dimension.right() + 1) * 10);
    map_.y_min(dimension.top() * 10);
    map_.y_max((dimension.bottom() + 1) * 10);

    QList<QGraphicsItem*> list = items();
    for(int i=0; i < list.count(); ++i)
    {
      CreatorItem* item = dynamic_cast<CreatorItem*> (list.at(i));
      if (item)
        delete item;
    }

    update_scene_rect();
  }

  void Creator::action(CreatorAction::type_action action)
  {
    creator_action_->value(action);
  }

  srp_creator::map& Creator::map()
  {
    return map_;
  }

  QGraphicsScene& Creator::scene()
  {
    return scene_;
  }


  int Creator::pos_x() const
  {
    return creator_action_->pos_x();
  }

  int Creator::pos_y() const
  {
    return creator_action_->pos_y();
  }


  const QColor& Creator::color() const
  {
    return color_;
  }

  void Creator::color(const QColor& val)
  {
    color_ = val;
    emit sig_color_updated();
  }


  int Creator::width_pen() const
  {
    return width_pen_;
  }

  void Creator::width_pen(int val)
  {
    width_pen_ = val;
    emit sig_width_pen_updated();
  }

  const int& Creator::depth() const
  {
    return depth_;
  }

  void Creator::depth(int val)
  {
    depth_ = val;
    show_floor();
    emit sig_depth_updated();
  }

  void Creator::zoom(float val)
  {
    scale(1 / zoom_, 1 / zoom_);
    scale(val, val);
    zoom_ = val;
  }

  void Creator::show_floor()
  {
    QList<QGraphicsItem*> list = items();
    for(int i=0; i < list.count(); ++i)
    {
      CreatorItem* item = dynamic_cast<CreatorItem*> (list.at(i));
      if (item)
        delete item;
    }

    MapItem floor = map_.floor(depth_);

    MapItem::const_iterator i = floor.constBegin();
    while (i != floor.constEnd())
    {
      CreatorItem* item = new CreatorItem(i.value(), depth());
      item->setPos(i.key());
      scene_.addItem(item);
      ++i;
    }
  }

  void Creator::update_scene_rect()
  {
    scene_.setSceneRect(map_.rect());
    rect_.setRect(map_.rect());
    x_line_.setLine(map_.x_min() - 30, 0, map_.x_max() + 30, 0);
    y_line_.setLine(0, map_.y_min() - 30, 0, map_.y_max() + 30);
  }

  void Creator::mouseMoveEvent(QMouseEvent* event)
  {
    QWidget::mouseMoveEvent(event);
    creator_action_->mouseMoveEvent(event);
  }

  void Creator::mousePressEvent(QMouseEvent* event)
  {
    QWidget::mousePressEvent(event);
    creator_action_->mousePressEvent(event);
  }

  void Creator::mouseReleaseEvent(QMouseEvent* event)
  {
    QWidget::mouseReleaseEvent(event);
    creator_action_->mouseReleaseEvent(event);
  }
}
