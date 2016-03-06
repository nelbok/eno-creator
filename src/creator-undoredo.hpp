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


#ifndef CREATORUNDOREDO_HPP
# define CREATORUNDOREDO_HPP

# include "data/misc.hpp"
# include <QColor>
# include <QMap>
# include <QVector3D>
# include <QWidget>

namespace srp_creator
{
  class CreatorUndoRedo : public QWidget
  {
    Q_OBJECT

    public:
      CreatorUndoRedo(QWidget *parent = 0);
      ~CreatorUndoRedo();

      void undo();
      void redo();

    public slots:
      void do_begin();
      void do_add_item(const QVector3D& p, const QColor& c);
      void do_remove_item(const QVector3D& p, const QColor& c);
      void do_resize_scene(const QRect& old_size, const QRect& new_size);
      void do_end();

    private:
      struct Transaction
      {
        QRect old_size;
        QRect new_size;
        MapItem remove_items;
        MapItem add_items;
      };

      bool transaction_;
      Transaction new_transaction_;
      QList<Transaction> transactions_;

      int index_;

    signals:
      void sig_items_removed(const MapItem& items);
      void sig_items_added(const MapItem& items);
      void sig_size_changed(const QRect& new_size);

  };
}
#endif // CREATORUNDOREDO_HPP
