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


#ifndef CREATORENO_HPP
# define CREATORENO_HPP

class QString;

namespace srp_creator
{
  class Creator;
  class map;

  class CreatorEno
  {
    public:
      CreatorEno(map& m, QString& dir);

      bool save() const;
      bool save_as() const;
      bool open(Creator& creator);
      bool open(const QString& fileName, Creator& creator);

    private:
      bool do_save(const QString& fileName) const;
      bool do_load(const QString& fileName, Creator& creator);

      map& map_;
      QString& directory_;
  };
}

#endif // CREATORENO_HPP
