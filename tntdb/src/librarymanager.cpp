/* 
 * Copyright (C) 2006 Tommi Maekitalo
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <tntdb/librarymanager.h>
#include <cxxtools/log.h>

#ifndef PKGLIBDIR
#define PKGLIBDIR "tntdb"
#endif

log_define("tntdb.librarymanager");

namespace tntdb
{
  static const std::string libraryPrefix = "tntdb-";

  LibraryManager::LibraryManager(const std::string& driverName)
  {
    const SearchPathType& path = getSearchPath();

    SearchPathType::const_iterator it;
    for (it = path.begin(); it != path.end(); ++it)
    {
      std::string d = *it + '/' + libraryPrefix + driverName;
      try
      {
        log_debug("loading library \"" << d << '"');
        lib = cxxtools::dl::Library(d.c_str());
        break;
      }
      catch (const cxxtools::dl::DlopenError&)
      {
        log_debug("library \"" << d << "\" not found");
      }
    }

    if (it == path.end())
    {
      std::string d = libraryPrefix + driverName;
      log_debug("loading library \"" << d << '"');
      lib = cxxtools::dl::Library(d.c_str());
    }

    connectionManager = static_cast<IConnectionManager*>(lib.sym(("connectionManager_" + driverName).c_str()).getSym());

    log_debug("driver " << driverName << " loaded (" << lib.getHandle() << ") connectionManager=" << connectionManager);
  }

  LibraryManager::SearchPathType& LibraryManager::getSearchPath()
  {
    static LibraryManager::SearchPathType searchPath;
    if (searchPath.empty())
      searchPath.push_back(PKGLIBDIR);
    return searchPath;
  }
}
