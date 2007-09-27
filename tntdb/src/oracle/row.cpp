/* 
 * Copyright (C) 2007 Tommi Maekitalo
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

#include <tntdb/oracle/row.h>
#include <tntdb/oracle/value.h>
#include <cxxtools/log.h>

log_define("tntdb.oracle.row")

namespace tntdb
{
  namespace oracle
  {
    Row::Row(Statement* stmt)
    {
      ub4 columncount;

      sword ret = OCIAttrGet(stmt->getHandle(), OCI_HTYPE_STMT, &columncount,
        0, OCI_ATTR_PARAM_COUNT, stmt->getErrorHandle());
      stmt->checkError(ret, "OCIAttrGet(OCI_ATTR_PARAM_COUNT)");

      log_debug("define " << columncount << " parameters");
      values.reserve(columncount);
      for (ub4 pos = 0; pos < columncount; ++pos)
        values.push_back(tntdb::Value(new tntdb::oracle::Value(stmt, pos)));
    }

    Row::Row(Statement* stmt, unsigned columncount)
    {
      log_debug("define " << columncount << " parameters");
      values.reserve(columncount);
      for (ub4 pos = 0; pos < columncount; ++pos)
        values.push_back(tntdb::Value(new tntdb::oracle::Value(stmt, pos)));
    }

    Row::size_type Row::size() const
    {
      return values.size();
    }

    tntdb::Value Row::getValue(size_type field_num) const
    {
      return values.at(field_num);
    }

  }
}