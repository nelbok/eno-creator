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


#include "creator-item.hpp"

#include <QPainter>
#include <QVector3D>

namespace srp_creator
{
  CreatorItem::CreatorItem(const QColor& color, const int& depth, QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , color_(color)
    , depth_(depth)
  {
  }


  const QColor& CreatorItem::color() const
  {
    return color_;
  }


  void CreatorItem::setPos(const QVector3D& pos)
  {
    QGraphicsItem::setPos(pos.toPointF());
    QGraphicsItem::setZValue(pos.z());
  }

  void CreatorItem::setPos(qreal x, qreal y, qreal z)
  {
    QGraphicsItem::setPos(x, y);
    QGraphicsItem::setZValue(z);
  }


  QRectF CreatorItem::boundingRect() const
  {
    return QRectF(0, 0, 10, 10);
  }

  void CreatorItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget* )
  {
    QPen pen;
    if (zValue() < depth_)
      pen.setStyle(Qt::DotLine);
    painter->setPen(pen);

    QBrush brush(color_);
    if (zValue() < depth_)
      brush.setStyle(Qt::Dense4Pattern);
    painter->setBrush(brush);

    painter->drawRect(0, 0, 9, 9);
  }
}
