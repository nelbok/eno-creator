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


#ifndef CREATORPREFERENCE_HPP
# define CREATORPREFERENCE_HPP

# include <QWidget>

namespace Ui
{
  class CreatorPreference;
}

namespace srp_creator
{
  class CreatorPreference : public QWidget
  {
    Q_OBJECT

    public:
      CreatorPreference(QWidget *parent = 0);
      ~CreatorPreference();

    protected:
      virtual void changeEvent(QEvent *event);

    private:
      Ui::CreatorPreference* ui_;

    private slots:
      void do_update_window_title();
      void do_init_settings();

      //General
      void do_eno_clicked();
      void do_obj_clicked();

      //New file
      void do_color_clicked();

      //Settings
      void do_apply_clicked();
      void do_cancel_clicked();
      void do_ok_clicked();

    signals:
      void sig_settings_updated();
  };
}
#endif // CREATORPREFERENCE_HPP
