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


#ifndef MISC_HPP
# define MISC_HPP

# include <QColor>
# include <QVector3D>

typedef QMap<QVector3D, QColor> MapItem;
bool operator<(const QVector3D& p1, const QVector3D& p2);

#endif // MISC_HPP
