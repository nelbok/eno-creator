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


#ifndef OPENGL_HPP
# define OPENGL_HPP

// FIXME: THIS CLASS NEED TO BE REWRITTEN !!!
// QGLWIDGET IS OBSOLETE
// REMOVE opengl option of QT in PRO FILE IF NEEDED
# include <QGLWidget>

namespace srp_creator
{
  class map;

  class opengl : public QGLWidget
  {
    public:
      opengl(const map& m, QWidget* parent = 0);

    protected:
      virtual void keyPressEvent(QKeyEvent* event);
      virtual void keyReleaseEvent(QKeyEvent* event);
      virtual void timerEvent(QTimerEvent* event);

      virtual void initializeGL();
      virtual void paintGL();
      virtual void resizeGL(int width, int height);

    private:
      void do_change_key(int key, bool actif);

      const map& map_;
      bool left_;
      bool right_;
      bool up_;
      bool down_;
      bool top_;
      bool bottom_;

      double x_;
      double y_;
      double z_;
      int angle_;
  };
}

#endif // OPENGL_HPP
