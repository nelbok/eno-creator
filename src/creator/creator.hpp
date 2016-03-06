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


#ifndef CREATOR_HPP
# define CREATOR_HPP

# include "creator/creator-action.hpp"
# include "data/map.hpp"

# include <QGraphicsRectItem>
# include <QGraphicsView>

namespace srp_creator
{
  class Creator : public QGraphicsView
  {
    Q_OBJECT

    public:
      Creator(QWidget* parent = 0);
      ~Creator();

      void reset();
      void action(CreatorAction::type_action action);
      srp_creator::map& map();
      QGraphicsScene& scene();

      int pos_x() const;
      int pos_y() const;

      const QColor& color() const;
      void color(const QColor& val);

      int width_pen() const;
      void width_pen(int val);
      const int& depth() const;
      void depth(int val);
      void zoom(float val);
      void show_floor();
      void update_scene_rect();

    protected:
      virtual void mouseMoveEvent(QMouseEvent* event);
      virtual void mousePressEvent(QMouseEvent* event);
      virtual void mouseReleaseEvent(QMouseEvent* event);

    private:
      QGraphicsScene scene_;
      QGraphicsRectItem rect_;
      QGraphicsLineItem x_line_;
      QGraphicsLineItem y_line_;
      CreatorAction* creator_action_;

      QColor color_;
      int width_pen_;
      int depth_;
      float zoom_;

      srp_creator::map map_;

    signals:
      void sig_color_updated();
      void sig_width_pen_updated();
      void sig_depth_updated();
      void sig_pos_updated();
  };
}

#endif // CREATOR_HPP
