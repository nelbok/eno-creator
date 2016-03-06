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


#ifndef CREATORITEM_HPP
# define CREATORITEM_HPP

# include <QGraphicsItem>

class QVector3D;

namespace srp_creator
{
  class CreatorItem : public QGraphicsItem
  {
    public:
      CreatorItem(const QColor& color, const int& depth, QGraphicsItem* parent = 0);

      const QColor& color() const;

      void setPos(const QVector3D& pos);
      void setPos(qreal x, qreal y, qreal z);

      virtual QRectF boundingRect() const;
      virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0 );

    private:
      QColor color_;
      const int& depth_;

  };
}

#endif // CREATORITEM_HPP
