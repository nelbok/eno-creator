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


#include "creator-preference.hpp"
#include "creator-settings.hpp"
#include "ui_creator-preference.h"

#include <QColorDialog>
#include <QCompleter>
#include <QFileDialog>
#include <QFileSystemModel>

namespace srp_creator
{
  CreatorPreference::CreatorPreference(QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::CreatorPreference)
  {
    ui_->setupUi(this);
    setWindowIcon(QIcon(":/logo/logo.png"));

    do_update_window_title();
    do_init_settings();

    //General
    connect(ui_->enoButton,     SIGNAL(clicked()), SLOT(do_eno_clicked()));
    connect(ui_->objButton,     SIGNAL(clicked()), SLOT(do_obj_clicked()));

    //New file
    connect(ui_->colorButton,   SIGNAL(clicked()), SLOT(do_color_clicked()));

    //Settings
    connect(ui_->applyButton,   SIGNAL(clicked()), SLOT(do_apply_clicked()));
    connect(ui_->cancelButton,  SIGNAL(clicked()), SLOT(do_cancel_clicked()));
    connect(ui_->okButton,      SIGNAL(clicked()), SLOT(do_ok_clicked()));
  }

  CreatorPreference::~CreatorPreference()
  {
    delete ui_;
  }


  void CreatorPreference::changeEvent(QEvent *event)
  {
    if (event->type() == QEvent::LanguageChange)
    {
      ui_->retranslateUi(this);
      do_update_window_title();
    }
    else
      QWidget::changeEvent(event);
  }


  void CreatorPreference::do_update_window_title()
  {
    setWindowTitle(QApplication::translate("CreatorPreference",  "Preference - Shadow Revival Project - Creator"));
  }

  void CreatorPreference::do_init_settings()
  {
    //Default color
    QColor c = CreatorSettings().do_load_color();
    if (c.isValid())
    {
      ui_->colorButton->setStyleSheet(QString::fromUtf8("background-color: %1;").arg(c.name()));
      ui_->colorButton->setText(c.name());
    }

    //Default directory
    QFileSystemModel* model = new QFileSystemModel;
    model->setRootPath("");
    QCompleter* completer = new QCompleter(this);
    completer->setModel(model);
    ui_->enoLineEdit->setText(CreatorSettings().do_load_eno_directory());
    ui_->enoLineEdit->setCompleter(completer);
    ui_->objLineEdit->setText(CreatorSettings().do_load_obj_directory());
    ui_->objLineEdit->setCompleter(completer);

    //Default language
    ui_->languageComboBox->addItem(QString::fromUtf8("Français"), QVariant("fr_FR"));
    ui_->languageComboBox->addItem(QString::fromUtf8("English"), QVariant("en_US"));
    if (CreatorSettings().do_load_language() == "en_US")
      ui_->languageComboBox->setCurrentIndex(1);

    //Default dimension
    QRect dimension = CreatorSettings().do_load_dimension();
    ui_->minxSpinBox->setValue(dimension.left());
    ui_->maxxSpinBox->setValue(dimension.right() + 1);
    ui_->minySpinBox->setValue(dimension.top());
    ui_->maxySpinBox->setValue(dimension.bottom() + 1);

    emit sig_settings_updated();
  }


  void CreatorPreference::do_eno_clicked()
  {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a directory"), CreatorSettings().do_load_eno_directory());
    if (!dir.isEmpty())
      ui_->enoLineEdit->setText(dir);
  }

  void CreatorPreference::do_obj_clicked()
  {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select a directory"), CreatorSettings().do_load_obj_directory());
    if (!dir.isEmpty())
      ui_->objLineEdit->setText(dir);
  }


  void CreatorPreference::do_color_clicked()
  {
    QColor c = QColorDialog::getColor(CreatorSettings().do_load_color(), this);
    if (c.isValid())
    {
      ui_->colorButton->setStyleSheet(QString::fromUtf8("background-color: %1;").arg(c.name()));
      ui_->colorButton->setText(c.name());
    }
  }


  void CreatorPreference::do_apply_clicked()
  {
    //Default color
    CreatorSettings().do_save_color(QColor(ui_->colorButton->text()));

    //Default directory
    CreatorSettings().do_save_eno_directory(ui_->enoLineEdit->text());
    CreatorSettings().do_save_obj_directory(ui_->objLineEdit->text());

    //Default language
    CreatorSettings().do_save_language(ui_->languageComboBox->itemData(ui_->languageComboBox->currentIndex()).toString());


    //Default dimension
    CreatorSettings().do_save_dimension(QRect(
                ui_->minxSpinBox->value(),
                ui_->minySpinBox->value(),
                ui_->maxxSpinBox->value() - ui_->minxSpinBox->value(),
                ui_->maxySpinBox->value() - ui_->minySpinBox->value()));

    emit sig_settings_updated();
  }

  void CreatorPreference::do_cancel_clicked()
  {
    close();
  }

  void CreatorPreference::do_ok_clicked()
  {
    do_apply_clicked();
    close();
  }

}
