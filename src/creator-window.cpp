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


#include "files/creator-eno.hpp"
#include "files/creator-wavefront.hpp"
#include "help/help.hpp"
#include "opengl/opengl.hpp"
#include "creator-preference.hpp"
#include "creator-settings.hpp"
#include "creator-window.hpp"
#include "ui_creator-window.h"

#include <QCloseEvent>
#include <QColorDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTranslator>

#define TIME_STATUSBAR 1500

namespace srp_creator
{
  CreatorWindow::CreatorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::CreatorWindow)
    , creator_(this)
  {
    ui_->setupUi(this);
    ui_->centralLayout->addWidget(&creator_);
    connect(&creator_,       SIGNAL(sig_color_updated()), SLOT(do_update_color()));
    connect(&creator_,       SIGNAL(sig_pos_updated()), SLOT(do_update_info()));
    connect(&creator_.map(), SIGNAL(sig_file_name_updated(QString)), SLOT(do_update_window_title()));
    connect(&creator_.map(), SIGNAL(sig_is_modified_updated(bool)), SLOT(do_update_window_title()));
    connect(&creator_.map(), SIGNAL(sig_boundingbox_updated()), SLOT(do_update_info()));
    setWindowIcon(QIcon(":/logo/logo.png"));
    do_update_window_title();
    do_init_settings();
    do_update_info();

    // File menu
    connect(ui_->newAction, SIGNAL(triggered()), SLOT(do_new_triggered()));
    connect(ui_->openAction, SIGNAL(triggered()), SLOT(do_open_triggered()));
    connect(ui_->saveAction, SIGNAL(triggered()), SLOT(do_save_triggered()));
    connect(ui_->saveAsAction, SIGNAL(triggered()), SLOT(do_save_as_triggered()));
    connect(ui_->generateAction, SIGNAL(triggered()), SLOT(do_generate_triggered()));
    connect(ui_->preferenceAction, SIGNAL(triggered()), SLOT(do_preference_triggered()));
    connect(ui_->quitAction, SIGNAL(triggered()), SLOT(do_quit_triggered()));

    // Help menu
    connect(ui_->helpAction, SIGNAL(triggered()), SLOT(do_help_triggered()));
    connect(ui_->aboutCreatorAction, SIGNAL(triggered()), SLOT(do_about_creator_triggered()));
    connect(ui_->aboutQtAction, SIGNAL(triggered()), SLOT(do_about_qt_triggered()));

    // Tool bar
    connect(ui_->toolsAction, SIGNAL(toggled(bool)),            ui_->mainToolBar, SLOT(setVisible(bool)));
    connect(ui_->mainToolBar, SIGNAL(visibilityChanged(bool)),  ui_->toolsAction, SLOT(setChecked(bool)));
    suppress_action_ = ui_->mainToolBar->addAction(QIcon(":items/suppress.png"), "", this, SLOT(do_suppress_clicked()));
    suppress_action_->setCheckable(true);

    add_action_  = ui_->mainToolBar->addAction(QIcon(":items/add.png"), "", this, SLOT(do_add_clicked()));
    add_action_->setCheckable(true);
    add_action_->setChecked(true);
    creator_.action(CreatorAction::Add);

    color_picker_action_ = ui_->mainToolBar->addAction(QIcon(":items/picker.png"), "", this, SLOT(do_color_picker_clicked()));
    color_picker_action_->setCheckable(true);

    resize_action_ = ui_->mainToolBar->addAction(QIcon(":items/size.png"), "", this, SLOT(do_resize_clicked()));
    resize_action_->setCheckable(true);

    ui_->mainToolBar->addSeparator();
    ui_->mainToolBar->addWidget(&color_button_);
    connect(&color_button_, SIGNAL(clicked()), SLOT(do_color_clicked()));
    do_update_color();

    ui_->mainToolBar->addSeparator();
    ui_->mainToolBar->addWidget(&depth_box_);
    depth_box_.setMinimum(-10);
    depth_box_.setMaximum(10);
    depth_box_.setValue(creator_.depth());
    ui_->mainToolBar->addWidget(&depth_label_);
    depth_label_.setPixmap(QPixmap(":items/depth.png"));
    connect(&depth_box_, SIGNAL(valueChanged(int)), SLOT(do_depth_clicked()));

    ui_->mainToolBar->addSeparator();
    ui_->mainToolBar->addWidget(&width_box_);
    width_box_.setMinimum(1);
    width_box_.setMaximum(10);
    width_box_.setValue(creator_.width_pen());
    ui_->mainToolBar->addWidget(&width_label_);
    width_label_.setPixmap(QPixmap(":items/width.png"));
    connect(&width_box_, SIGNAL(valueChanged(int)), SLOT(do_width_clicked()));

    ui_->mainToolBar->addSeparator();
    ui_->mainToolBar->addWidget(&zoom_box_);
    zoom_box_.addItem("400 %", QVariant(400));
    zoom_box_.addItem("200 %", QVariant(200));
    zoom_box_.addItem("100 %", QVariant(100));
    zoom_box_.addItem("50 %", QVariant(50));
    zoom_box_.addItem("25 %", QVariant(25));
    zoom_box_.addItem("10 %", QVariant(10));
    zoom_box_.setCurrentIndex(2);
    ui_->mainToolBar->addWidget(&zoom_label_);
    zoom_label_.setPixmap(QPixmap(":items/zoom.png"));
    connect(&zoom_box_, SIGNAL(currentIndexChanged(int)), SLOT(do_zoom_clicked()));

    ui_->mainToolBar->addSeparator();
    generate_action_ = ui_->mainToolBar->addAction(QIcon(":export/generate.png"), "", this, SLOT(do_generate_clicked()));
    opengl_action_ = ui_->mainToolBar->addAction(QIcon(":export/opengl.png"), "", this, SLOT(do_preview_clicked()));

    ui_->mainToolBar->addSeparator();
    ui_->mainToolBar->addWidget(&info_label_);

    retranslateUi();
  }

  CreatorWindow::~CreatorWindow()
  {
    delete ui_;
  }


  void CreatorWindow::changeEvent(QEvent *event)
  {
    if (event->type() == QEvent::LanguageChange)
    {
      ui_->retranslateUi(this);
      do_update_window_title();
      retranslateUi();
    }
    else
      QMainWindow::changeEvent(event);
  }

  void CreatorWindow::closeEvent(QCloseEvent* event)
  {
    event->ignore();
    do_quit_triggered();
  }


  void CreatorWindow::retranslateUi()
  {
    suppress_action_->setToolTip(QApplication::translate("CreatorWindow",  "Suppress"));
    add_action_->setToolTip(QApplication::translate("CreatorWindow",  "Add"));
    color_picker_action_->setToolTip(QApplication::translate("CreatorWindow",  "Color picker"));
    resize_action_->setToolTip(QApplication::translate("CreatorWindow",  "Resize"));
    color_button_.setToolTip(QApplication::translate("CreatorWindow",  "Change color"));
    depth_box_.setToolTip(QApplication::translate("CreatorWindow",  "Change depth of the map"));
    width_box_.setToolTip(QApplication::translate("CreatorWindow",  "Change width of the pen"));
    generate_action_->setToolTip(QApplication::translate("CreatorWindow",  "Generate Wavefront file"));
    opengl_action_->setToolTip(QApplication::translate("CreatorWindow",  "Preview with OpenGL"));
  }


  void CreatorWindow::do_update_window_title()
  {
    QString title = QApplication::translate("CreatorWindow",  "Shadow Revival Project - Creator");

    if (!creator_.map().file_name().isEmpty())
    {
      title.prepend(" - ");
      if (creator_.map().is_modified())
        title.prepend("*");
      title.prepend(QFileInfo(creator_.map().file_name()).fileName());
    }

    setWindowTitle(title);
  }

  void CreatorWindow::do_init_settings()
  {
    //Recent files
    QMenu* m = ui_->recentFilesMenu;

    m->clear();
    QStringList list = CreatorSettings().do_load_recent_files();
    for(int i = 0; i < list.size(); ++i)
    {
      if (QFile::exists(list.at(i)))
        m->addAction(list.at(i), this, SLOT(do_recent_files_triggered()));
      recent_files_.append(list.at(i));
    }

    //Default color
    QColor c = CreatorSettings().do_load_color();
    if (c.isValid())
      creator_.color(c);

    //Default directory
    eno_directory_ = CreatorSettings().do_load_eno_directory();
    obj_directory_ = CreatorSettings().do_load_obj_directory();

    //Default language
    QTranslator* trans = new QTranslator();
    trans->load(":/lang/" + CreatorSettings().do_load_language());
    QApplication::installTranslator(trans);
  }

  void CreatorWindow::do_add_recent_file()
  {
    QMenu* m = ui_->recentFilesMenu;

    recent_files_.removeAll(creator_.map().file_name());
    recent_files_.prepend(creator_.map().file_name());

    m->clear();
    int max = 5;
    for(int i = 0; i < recent_files_.size() && i < max; ++i)
      if (QFile::exists(recent_files_.at(i)))
        m->addAction(recent_files_.at(i), this, SLOT(do_recent_files_triggered()));
      else
        ++max;
  }


  //File menu

  void CreatorWindow::do_new_triggered()
  {
    creator_.reset();
    do_update_window_title();
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "New map completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_open_triggered()
  {
    creator_.reset();
    CreatorEno ce(creator_.map(), eno_directory_);
    if (ce.open(creator_))
    {
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Opening the map completed."), TIME_STATUSBAR);
      do_add_recent_file();
    }
    else
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Error while opening the map."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_recent_files_triggered()
  {
    QAction* act = qobject_cast<QAction*> (sender());

    creator_.reset();
    CreatorEno ce(creator_.map(), eno_directory_);
    if (ce.open(act->text(), creator_))
    {
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Opening the map completed."), TIME_STATUSBAR);
      do_add_recent_file();
    }
    else
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Error while opening the map."), TIME_STATUSBAR);
  }


  bool CreatorWindow::do_save_triggered()
  {
    CreatorEno ce(creator_.map(), eno_directory_);
    if (ce.save())
    {
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Saving the map completed."), TIME_STATUSBAR);
      do_add_recent_file();
      return true;
    }
    else
    {
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Error while saving the map."), TIME_STATUSBAR);
      return false;
    }
  }

  void CreatorWindow::do_save_as_triggered()
  {
    CreatorEno ce(creator_.map(), eno_directory_);
    if (ce.save_as())
    {
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Saving the map completed."), TIME_STATUSBAR);
      do_add_recent_file();
    }
    else
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Error while saving the map."), TIME_STATUSBAR);
  }


  void CreatorWindow::do_generate_triggered()
  {
    CreatorWavefront cw;
    if (cw.generate(creator_.map(), obj_directory_))
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Generating of the Wavefront file completed."), TIME_STATUSBAR);
    else
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Error while generating of the Wavefront file."), TIME_STATUSBAR);
  }


  void CreatorWindow::do_preference_triggered()
  {
    CreatorPreference* obj = new CreatorPreference();
    connect(obj, SIGNAL(sig_settings_updated()), SLOT(do_init_settings()));
    obj->show();
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Open preference completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_quit_triggered()
  {
    CreatorSettings().do_save_recent_files(recent_files_);
    CreatorSettings().do_save_eno_directory(eno_directory_);
    CreatorSettings().do_save_obj_directory(obj_directory_);

    if (creator_.map().is_modified())
      switch (QMessageBox::warning(this,
                QApplication::translate("CreatorWindow",  "Shadow Revival Project - Creator"),
                QApplication::translate("CreatorWindow",  "The map has been modified.\nDo you want to save your changes?"),
                QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                QMessageBox::Save))
      {
        case QMessageBox::Save:
          if (!do_save_triggered())
        default:
        case QMessageBox::Discard:
          qApp->exit();
          break;
        case QMessageBox::Cancel:
          break;
      }
    else
      qApp->exit();
  }


  //Help menu
  void CreatorWindow::do_help_triggered()
  {
    (new help())->show();
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Open help completed."), TIME_STATUSBAR);
  }


  void CreatorWindow::do_about_creator_triggered()
  {
    QString msg ("<strong>SRP-Creator</strong><br/><br/>");
//    msg.append(QString("Version : %1<br/><br/>").arg(SRPCREATOR_VERSION));
//    msg.append(QString("2011-%1 Shadow Revival<br/><br/>").arg(SRPCREATOR_YEAR));
//    msg.append("<a href=\"http://www.shadowrevival.com\">www.shadowrevival.com</a>");
    msg.append("Version : 1.0 Alpha<br/><br/>");
    msg.append("2011-2016 Shadow Revival<br/>");
    QMessageBox::about(this, QApplication::translate("CreatorWindow",  "About Creator"), msg);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Open about creator completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_about_qt_triggered()
  {
    QMessageBox::aboutQt(this);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Open about Qt completed."), TIME_STATUSBAR);
  }


  // Tool bar

  void CreatorWindow::do_suppress_clicked()
  {
    suppress_action_->setChecked(true);
    add_action_->setChecked(false);
    color_picker_action_->setChecked(false);
    resize_action_->setChecked(false);
    creator_.action(CreatorAction::Suppress);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set suppress mode completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_add_clicked()
  {
    suppress_action_->setChecked(false);
    add_action_->setChecked(true);
    color_picker_action_->setChecked(false);
    resize_action_->setChecked(false);
    creator_.action(CreatorAction::Add);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set add mode completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_color_picker_clicked()
  {
    suppress_action_->setChecked(false);
    add_action_->setChecked(false);
    color_picker_action_->setChecked(true);
    resize_action_->setChecked(false);
    creator_.action(CreatorAction::Color);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set color picker mode completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_resize_clicked()
  {
    suppress_action_->setChecked(false);
    add_action_->setChecked(false);
    color_picker_action_->setChecked(false);
    resize_action_->setChecked(true);
    creator_.action(CreatorAction::Size);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set resize mode completed."), TIME_STATUSBAR);
  }


  void CreatorWindow::do_color_clicked()
  {
    QColor color = QColorDialog::getColor(creator_.color(), this);
    if (color.isValid())
    {
      creator_.color(color);
      ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set color completed."), TIME_STATUSBAR);
    }
  }

  void CreatorWindow::do_update_color()
  {
    color_button_.setStyleSheet(QString::fromUtf8("background-color: %1;").arg(creator_.color().name()));
    color_button_.setText(creator_.color().name());
  }


  void CreatorWindow::do_depth_clicked()
  {
    creator_.depth(depth_box_.value() * 10);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set depth completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_width_clicked()
  {
    creator_.width_pen(width_box_.value());
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set width pen completed."), TIME_STATUSBAR);
  }

  void CreatorWindow::do_zoom_clicked()
  {
    creator_.zoom(zoom_box_.itemData(zoom_box_.currentIndex()).toFloat() / 100.);
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Set zoom completed."), TIME_STATUSBAR);
  }


  void CreatorWindow::do_generate_clicked()
  {
    do_generate_triggered();
  }

  void CreatorWindow::do_preview_clicked()
  {
    (new opengl(creator_.map()))->show();
    ui_->statusBar->showMessage(QApplication::translate("CreatorWindow",  "Show preview completed."), TIME_STATUSBAR);
  }


  void CreatorWindow::do_update_info()
  {
    info_label_.setText(QString("%1, %2 (%3, %4) (%5, %6)")
                        .arg(  (creator_.pos_x() / 10)    )
                        .arg(- (creator_.pos_y() / 10) - 1)
                        .arg(creator_.map().x_min() / 10)
                        .arg(creator_.map().y_min() / 10)
                        .arg(creator_.map().x_max() / 10)
                        .arg(creator_.map().y_max() / 10)
                        );
  }
}
