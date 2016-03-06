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


#ifndef CREATOR_ACTION_HPP
# define CREATOR_ACTION_HPP

# include <QObject>

class QMouseEvent;

namespace srp_creator
{
  class Creator;

  class CreatorAction : public QObject
  {
    Q_OBJECT

    public:
      enum type_action {Suppress, Add, Color, Size};

      CreatorAction(srp_creator::Creator* creator);
      ~CreatorAction();

      int pos_x() const;
      int pos_y() const;

      void value(CreatorAction::type_action val);
      CreatorAction::type_action value() const;

      void mousePressEvent(QMouseEvent* event);
      void mouseMoveEvent(QMouseEvent* event);
      void mouseReleaseEvent(QMouseEvent* event);

    private:
      CreatorAction::type_action value_;
      /** 0xLRTB : Left Right Top Bottom **/
      int line_;
      bool pressed_;
      int pos_x_;
      int pos_y_;


      srp_creator::Creator* creator_;

      void calculate_pos(QMouseEvent* event);
      void execute();
      void execute_suppress();
      void execute_add();
      void execute_color();
      void execute_size();

    signals:
      void sig_pos_updated();
  };
}

#endif // CREATOR_ACTION_HPP
