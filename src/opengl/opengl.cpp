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

#include <cmath>
//#include <GL/glu.h>

#include <QFileInfo>
#include <QIcon>
#include <QKeyEvent>
#include <QTimerEvent>
#include <QDebug>

#include "data/map.hpp"
#include "opengl/opengl.hpp"

#define PI 3.14159265
#define TO_R PI/180

namespace srp_creator
{
  opengl::opengl(const map& m, QWidget* parent)
    : QOpenGLWidget(parent)
    , map_(m)
    , left_(false), right_(false), up_(false), down_(false)
    , top_(false), bottom_(false), shift_(false)
    , x_(0), y_(0), z_(0), angle_(0)
  {
    setWindowIcon(QIcon(":/logo/logo.png"));

    QString title = tr(" - Shadow Revival Project - Creator");
    if (!map_.file_name().isEmpty())
      title.prepend(QFileInfo(map_.file_name()).fileName());
    else
      title.prepend(tr("New map"));
    title.prepend(tr("Preview - "));

    setWindowTitle(title);

    startTimer(50);
  }


  void opengl::timerEvent(QTimerEvent *e)
  {
    e->accept();

    if (left_ && !right_)
      angle_ += 10;
    if (!left_ && right_)
      angle_ -= 10;
    if (!up_ && down_)
    {
      x_-=cos(angle_ * TO_R) *.2 * ((shift_) ? 10 : 1);
      y_-=sin(angle_ * TO_R) *.2 * ((shift_) ? 10 : 1);
    }
    if (up_ && !down_)
    {
      x_+=cos(angle_ * TO_R) *.2 * ((shift_) ? 10 : 1);
      y_+=sin(angle_ * TO_R) *.2 * ((shift_) ? 10 : 1);
    }

    if (!top_ && bottom_)
      --z_;
    if (top_ && !bottom_)
      ++z_;
    if (z_ > 10)
      z_ = 10;
    if (z_ < -10)
      z_ = -10;

    angle_  = angle_ % 360;

    update();
  }

  void opengl::keyPressEvent(QKeyEvent *e)
  {
    do_change_key(e->key(), true);
    e->accept();
  }

  void opengl::keyReleaseEvent(QKeyEvent *e)
  {
    do_change_key(e->key(), false);
    e->accept();
  }

  void opengl::initializeGL()
  {
    qDebug() << "Debug :: initializeOpenGLFunctions :: " << initializeOpenGLFunctions();
  }

  void opengl::resizeGL(int width, int height)
  {
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    projection_.setToIdentity();
    projection_.perspective(70, (double) width / height, .1, 100);
    glMultMatrixf(projection_.data());
    glEnable(GL_DEPTH_TEST);
  }

  void opengl::paintGL()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    projection_.setToIdentity();
    projection_.lookAt(
                QVector3D(0, 0, 1.5),
                QVector3D(1, 0, 1.5),
                QVector3D(0, 0, 1));
    glMultMatrixf(projection_.data());

    //Movement of camera
    glRotated(-angle_, 0, 0, 1);
    glTranslated(-x_ - .5, -y_ - .5, -z_);

    //Draw
    glDisable(GL_TEXTURE_2D);

    MapItem::const_iterator items = map_.items().constBegin();
    while (items != map_.items().constEnd())
    {
        int x =  items.key().x() / 10;
        int y = -items.key().y() / 10 - 1;
        int z =  items.key().z() / 10;
        QColor color = items.value().rgb();

        glColor3ub(color.red(), color.green(), color.blue());
        glBegin(GL_QUADS);
          glVertex3d(x,   y,   z);
          glVertex3d(x,   y+1, z);
          glVertex3d(x+1, y+1, z);
          glVertex3d(x+1, y,   z);

          glVertex3d(x,   y,   z+1);
          glVertex3d(x,   y+1, z+1);
          glVertex3d(x+1, y+1, z+1);
          glVertex3d(x+1, y,   z+1);

          glVertex3d(x,   y,   z);
          glVertex3d(x,   y,   z+1);
          glVertex3d(x+1, y,   z+1);
          glVertex3d(x+1, y,   z);

          glVertex3d(x,   y,   z);
          glVertex3d(x,   y,   z+1);
          glVertex3d(x,   y+1, z+1);
          glVertex3d(x,   y+1, z);

          glVertex3d(x,   y+1, z);
          glVertex3d(x,   y+1, z+1);
          glVertex3d(x+1, y+1, z+1);
          glVertex3d(x+1, y+1, z);

          glVertex3d(x+1, y,   z);
          glVertex3d(x+1, y,   z+1);
          glVertex3d(x+1, y+1, z+1);
          glVertex3d(x+1, y+1, z);
        glEnd();

        glColor3ub(255, 255, 255);
        glBegin(GL_LINES);
          glVertex3d(x,   y,   z);
          glVertex3d(x,   y+1, z);
          glVertex3d(x+1, y,   z);
          glVertex3d(x+1, y+1, z);
          glVertex3d(x,   y,   z+1);
          glVertex3d(x,   y+1, z+1);
          glVertex3d(x+1, y,   z+1);
          glVertex3d(x+1, y+1, z+1);

          glVertex3d(x,   y,   z);
          glVertex3d(x+1, y,   z);
          glVertex3d(x,   y+1, z);
          glVertex3d(x+1, y+1, z);
          glVertex3d(x,   y,   z+1);
          glVertex3d(x+1, y,   z+1);
          glVertex3d(x,   y+1, z+1);
          glVertex3d(x+1, y+1, z+1);

          glVertex3d(x,   y,   z);
          glVertex3d(x,   y,   z+1);
          glVertex3d(x+1, y,   z);
          glVertex3d(x+1, y,   z+1);
          glVertex3d(x,   y+1, z);
          glVertex3d(x,   y+1, z+1);
          glVertex3d(x+1, y+1, z);
          glVertex3d(x+1, y+1, z+1);
        glEnd();

        ++items;
    }
  }


  void opengl::do_change_key(int key, bool actif)
  {
    switch (key)
    {
      case Qt::Key_Left:
        left_ = actif;
        break;
      case Qt::Key_Right:
        right_ = actif;
        break;
      case Qt::Key_Up:
        up_ = actif;
        break;
      case Qt::Key_Down:
        down_ = actif;
        break;
      case Qt::Key_PageUp:
        top_ = actif;
        break;
      case Qt::Key_PageDown:
        bottom_ = actif;
        break;
      case Qt::Key_Shift:
        shift_ = actif;
        break;
    };
  }
}
