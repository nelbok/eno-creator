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


#ifndef HELP_HPP
# define HELP_HPP

# include <QTextBrowser>
# include <QTreeWidget>
# include <QWidget>

namespace srp_creator
{
  class help : public QWidget
  {
    Q_OBJECT

    public:
      help(QWidget* parent = 0);

    private:
      QTreeWidget tree_;
      QTextBrowser browser_;

      void init();

      void do_update_text_browser_i0();
      void do_update_text_browser_i1();
      void do_update_text_browser_i2();
      void do_update_text_browser_i3();

    private slots:
      void do_update_text_browser();
  };
}

#endif // HELP_HPP
