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


#include "help/help.hpp"

#include <QApplication>
#include <QHBoxLayout>

namespace srp_creator
{
  help::help(QWidget* parent)
    : QWidget(parent)
    , tree_(this)
    , browser_(this)
  {
    setWindowIcon(QIcon(":/logo/logo.png"));
    setWindowTitle(tr("Shadow Revival Project - Creator"));
    resize(800,600);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(&tree_);
    layout->addWidget(&browser_);

    init();
  }


  void help::init()
  {
    tree_.setMaximumWidth(240);
    tree_.setHeaderHidden(true);
    connect(&tree_, SIGNAL(itemSelectionChanged()), SLOT(do_update_text_browser()));

    QTreeWidgetItem* i0 = new QTreeWidgetItem(&tree_);
    i0->setText(0, QApplication::translate("help", "SRP-Creator"));
    i0->setData(0, Qt::UserRole, QString("i0"));

    QTreeWidgetItem* i1 = new QTreeWidgetItem(i0);
    i1->setText(0, QApplication::translate("help", "Barre d'outils"));
    i1->setData(0, Qt::UserRole, QString("i1"));

    QTreeWidgetItem* i2 = new QTreeWidgetItem(i0);
    i2->setText(0, QApplication::translate("help", "Fichier eno"));
    i2->setData(0, Qt::UserRole, QString("i2"));

    QTreeWidgetItem* i3 = new QTreeWidgetItem(i0);
    i3->setText(0, QApplication::translate("help", "Fichier Wavefront"));
    i3->setData(0, Qt::UserRole, QString("i3"));

    tree_.setCurrentItem(i0);
    tree_.expandAll();
  }


  void help::do_update_text_browser_i0()
  {
    browser_.setText(QApplication::translate("help", \
      "<h2><center>SRP-Creator</center></h2>" \
      "<h3>Description :</h3>" \
      "<p>SRP-Creator est un editeur de map 3D avec export en fichier Wavefront.<br/>" \
      "Il a pour but de simplifier la vie entre le programmeur et le modelisateur 3D.</p>" \
      "<h3>Presentation :</h3>" \
      "<p>La creation d'une carte se fait comme un dessin sur un editeur.<br/>" \
      "Ayant plusieurs couches de profondeur, il est possible de creer plusieurs etages d'un chateau par exemple.<br/>" \
      "Lors de la generation, l'application cree des cubes d'une unite de cote aux emplacements definis par l'utilisateur.</p>" \
      ));
  }

  void help::do_update_text_browser_i1()
  {
    browser_.setText(QApplication::translate("help", \
      "<h2><center>Barre d'outils</center></h2>" \
      "<h3>Suppression :</h3>" \
      "<p>Pour supprimer les cubes avec la souris, soit en cliquant dessus, soit en glissant.<br/>" \
      "<em>Note : Cela ne supprime que les cubes de la profondeur selectionnee.</em></p>" \
      "<h3>Ajout :</h3>" \
      "<p>Pour ajouter des cubes avec la souris, soit en cliquant, soit en glissant.<br/>" \
      "<em>Note : Cela n'ajoute que des cubes sur la profondeur selectionnee.</em></p>" \
      "<h3>Pipette de couleur :</h3>" \
      "<p>Pour recuperer une couleur sur un cube precis.</p>" \
      "<h3>Redimensionnement :</h3>" \
      "<p>Pour redimensionner la zone de dessin des cubes.<br/>" \
      "Vous pouvez redimensionner la zone toutes les directions.</p>" \
      "<h3>Couleur :</h3>" \
      "<p>Pour selectionner la couleur pour les cubes.</p>" \
      "<h3>Profondeur :</h3>" \
      "<p>Pour selectionner la profondeur.<br/>" \
      "<em>Note : On pourrait aussi dire les couches sur l'axe Z d'un objet 3D.</em></p>" \
      "<h3>Taille du crayon :</h3>" \
      "<p>Pour selectionner la taille du crayon.</p>" \
      "<h3>Generation :</h3>" \
      "<p>Pour generer au format Wavefront.<br/>" \
      "<em>Note : Voir la section \"Fichier Wavefront\" pour plus d'informations.</em></p>" \
      "<h3>Apercu :</h3>" \
      "<p>Pour avoir un apercu en 3D de la carte.<br/>" \
      "Le moteur utilise OpenGL pour afficher la carte en 3D.<br/>" \
      "Utilisez les touches directionnels pour vous deplacer.</p>" \
      ));
  }

  void help::do_update_text_browser_i2()
  {
    browser_.setText(QApplication::translate("help", \
      "<h2><center>Fichier eno</center></h2>" \
      "<p>Format de fichier texte pour le projet SRP-Creator.<br/>" \
      "Ce fichier comporte deux parties.</p>" \
      "<h3>Taille :</h3>" \
      "<p>C'est la premiere ligne du fichier, elle comporte quatre valeurs :<br/>" \
      "- Coordonnees X minimal<br/>" \
      "- Coordonnees X maximal<br/>" \
      "- Coordonnees Y minimal<br/>" \
      "- Coordonnees Y maximal</p>" \
      "<h3>Cubes :</h3>" \
      "<p>Chaque ligne comporte quatre valeurs :<br/>" \
      "- Coordonnees X<br/>" \
      "- Coordonnees Y<br/>" \
      "- Coordonnees Z<br/>" \
      "- Couleur RGB</p>" \
      ));
  }

  void help::do_update_text_browser_i3()
  {
    browser_.setText(QApplication::translate("help", \
      "<h2><center>Fichier Wavefront</center></h2>" \
      "<p>Format de fichier texte pour la 3D.</p>" \
      "<h3>Interet :</h3>" \
      "<p>C'est avec ce format que vous pourrez echanger avec le modelisateur 3D.<br/>" \
      "Il peut etre facilement important dans Blender et donc modifiable.<br/>" \
      "Ce format genere deux fichiers :<br/>" \
      "- Un fichier obj, qui comporte tout les vertex et faces de l'objet 3D<br/>" \
      "- Un fichier mtl, qui comporte tout ce qui est couleurs ou texture, donc les materiels de l'objet 3D</p>" \
      "<h3>Choix de ce format :</h3>" \
      "<p>Ce format a ete choisi pour sa simplicite et les differentes options.<br/>" \
      "Etant un format texte, il est facilement integrable dans un moteur 3D a base d'OpenGL par exemple.<br/>" \
      "Il peut aussi etre importer ou exporter par Blender.<br/>" \
      "Avec ces nombreuses options, on peut creer des decors affines.</p>" \
      "<h3>Liens :</h3>" \
      "<p><a href=\"http://www.martinreddy.net/gfx/3d/OBJ.spec\">SPEC OBJ</a><br/>" \
      "<a href=\"http://www.fileformat.info/format/material/\">SPEC MTL</a></p>" \
      ));
  }


  void help::do_update_text_browser()
  {
    QTreeWidgetItem* i = tree_.currentItem();
    if (not i)
      return;

    QString id = i->data(0, Qt::UserRole).toString();
    if (id == "i0")
      do_update_text_browser_i0();
    if (id == "i1")
      do_update_text_browser_i1();
    if (id == "i2")
      do_update_text_browser_i2();
    if (id == "i3")
      do_update_text_browser_i3();
  }
}
