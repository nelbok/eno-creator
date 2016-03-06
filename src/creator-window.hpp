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


#ifndef CREATORWINDOW_HPP
# define CREATORWINDOW_HPP

# include "creator/creator.hpp"

# include <QMainWindow>
# include <QComboBox>
# include <QLabel>
# include <QPushButton>
# include <QSpinBox>

namespace Ui
{
  class CreatorWindow;
}

namespace srp_creator
{
  class CreatorWindow : public QMainWindow
  {
    Q_OBJECT

    public:
      CreatorWindow(QWidget *parent = 0);
      ~CreatorWindow();

    protected:
      virtual void changeEvent(QEvent *event);
      virtual void closeEvent(QCloseEvent* event);

    private:
      Ui::CreatorWindow *ui_;
      void retranslateUi();

      Creator creator_;
      QStringList recent_files_;
      QString eno_directory_;
      QString obj_directory_;

      // Tool bar
      QAction* suppress_action_;
      QAction* add_action_;
      QAction* color_picker_action_;
      QAction* resize_action_;
      QPushButton color_button_;
      QSpinBox depth_box_;
      QLabel depth_label_;
      QSpinBox width_box_;
      QLabel width_label_;
      QComboBox zoom_box_;
      QLabel zoom_label_;
      QAction* generate_action_;
      QAction* opengl_action_;
      QLabel info_label_;

    private slots:
      void do_update_window_title();
      void do_init_settings();
      void do_add_recent_file();

      // File menu
      void do_new_triggered();
      void do_open_triggered();
      void do_recent_files_triggered();

      bool do_save_triggered();
      void do_save_as_triggered();

      void do_generate_triggered();

      void do_preference_triggered();

      void do_quit_triggered();


      //Help menu
      void do_help_triggered();

      void do_about_creator_triggered();
      void do_about_qt_triggered();


      // Tool bar
      void do_suppress_clicked();
      void do_add_clicked();
      void do_color_picker_clicked();
      void do_resize_clicked();

      void do_color_clicked();
      void do_update_color();

      void do_depth_clicked();
      void do_width_clicked();
      void do_zoom_clicked();

      void do_generate_clicked();
      void do_preview_clicked();

      void do_update_info();
  };
}
#endif // CREATORWINDOW_HPP
