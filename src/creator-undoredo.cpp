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


#include "creator-undoredo.hpp"

namespace srp_creator
{
  CreatorUndoRedo::CreatorUndoRedo(QWidget *parent)
    : QWidget(parent)
    , transaction_(false)
    , index_(0)
  {
  }

  CreatorUndoRedo::~CreatorUndoRedo()
  {
  }

  void CreatorUndoRedo::undo()
  {
    Transaction ts = transactions_.at(index_);
    if (ts.old_size !=  ts.new_size)
      emit sig_size_changed(ts.old_size);
    emit sig_items_removed(ts.add_items);
    emit sig_items_added(ts.remove_items);
    --index_;
  }

  void CreatorUndoRedo::redo()
  {
    if (index_ >= transactions_.count())
      return;
    ++index_;
    Transaction ts = transactions_.at(index_);
    if (ts.old_size !=  ts.new_size)
      emit sig_size_changed(ts.new_size);
    emit sig_items_removed(ts.remove_items);
    emit sig_items_added(ts.add_items);
  }


  void CreatorUndoRedo::do_begin()
  {
    transaction_ = true;
    new_transaction_.remove_items.clear();
    new_transaction_.add_items.clear();
  }

  void CreatorUndoRedo::do_add_item(const QVector3D& p, const QColor& c)
  {
    new_transaction_.add_items.insert(p, c);
  }

  void CreatorUndoRedo::do_remove_item(const QVector3D& p, const QColor& c)
  {
    new_transaction_.remove_items.insert(p, c);
  }

  void CreatorUndoRedo::do_resize_scene(const QRect& old_size, const QRect& new_size)
  {
    new_transaction_.old_size = old_size;
    new_transaction_.new_size = new_size;
  }

  void CreatorUndoRedo::do_end()
  {
    transaction_ = false;
    transactions_.append(new_transaction_);
    //FIXME
    index_ = transactions_.count() - 1;
  }

}
