/** -*- C++ -*-
 * Copyright (C) 2008  Luke Lu (Zvents, Inc.)
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hypertable. If not, see <http://www.gnu.org/licenses/>
 */

#include "Common/Compat.h"
#include "Common/TimeInline.h"

#include <time.h>
#include <iostream>

#include "ThriftHelper.h"

namespace Hypertable { namespace ThriftGen {

std::ostream &operator<<(std::ostream &out, const RowInterval &ri) {
  out <<"{RowInterval:";

  if (ri.__isset.start_row)
    out <<" start='"<< ri.start_row <<"'";

  if (ri.__isset.start_inclusive)
    out <<" start_inclusive="<< ri.start_inclusive;

  if (ri.__isset.end_row)
    out <<" end='"<< ri.end_row <<"'";

  if (ri.__isset.end_inclusive)
    out <<" end_inclusive="<< ri.end_inclusive;

  return out <<"}";
}

std::ostream &operator<<(std::ostream &out, const Cell &cell) {
  out <<"{Cell:";

  if (cell.__isset.row_key)
    out <<" row='"<< cell.row_key <<"'";

  if (cell.__isset.column_family)
    out <<" cf='"<< cell.column_family <<"'";

  if (cell.__isset.column_qualifier)
    out <<" cq='"<< cell.column_qualifier <<"'";

  if (cell.__isset.value)
    out <<" value='"<< cell.value <<"'";

  if (cell.__isset.timestamp)
    out << " ts="<< cell.timestamp;

  if (cell.__isset.revision)
    out <<" rev="<< cell.revision;

  if (cell.__isset.flag)
    out << " flag="<< cell.flag;

  return out <<"}";
}

std::ostream &operator<<(std::ostream &out, const CellInterval &ci) {
  out <<"{CellInterval:";

  if (ci.__isset.start_row)
    out <<" start_row='"<< ci.start_row <<"'";

  if (ci.__isset.start_column)
    out <<" start_column='"<< ci.start_column <<"'";

  if (ci.__isset.start_inclusive)
    out <<" start_inclusive="<< ci.start_inclusive;

  if (ci.__isset.end_row)
    out <<" end_row='"<< ci.end_row <<"'";

  if (ci.__isset.end_column)
    out <<" end_column='"<< ci.end_column <<"'";

  if (ci.__isset.end_inclusive)
    out <<" end_inclusive="<< ci.end_inclusive;

  return out <<"}";
}

std::ostream &operator<<(std::ostream &out, const ScanSpec &ss) {
  out <<"{ScanSpec:";

  if (ss.__isset.row_intervals) {
    out <<" rows=[\n";
    foreach(const RowInterval &ri, ss.row_intervals)
      out <<"  "<< ri <<"\n";
    out <<"  ]\n";
  }
  if (ss.__isset.cell_intervals) {
    out <<" cells=[\n";
    foreach(const CellInterval &ci, ss.cell_intervals)
      out <<"  "<< ci <<"\n";
    out <<"  ]\n";
  }
  if (ss.__isset.row_limit)
    out <<" row_limit="<< ss.row_limit;

  if (ss.__isset.revs)
    out <<" revs="<< ss.revs;

  if (ss.__isset.return_deletes)
    out <<" return_deletes="<< ss.return_deletes;

  if (ss.__isset.start_time)
    out <<" start_time="<< ss.start_time;

  if (ss.__isset.end_time)
    out <<" end_time="<< ss.end_time;

  return out <<'}';
}

std::ostream &operator<<(std::ostream &out, const HqlResult &hr) {
  out <<"{HqlResult:";

  if (hr.__isset.results) {
    out <<" results=[";
    foreach(const std::string &s, hr.results)
      out <<"  '"<< s <<"'\n";
    out <<"  ]\n";
  }
  if (hr.__isset.cells) {
    out <<" cells=[\n";
    foreach(const Cell &cell, hr.cells)
      out <<"  "<< cell <<"\n";
    out <<"  ]\n";
  }
  if (hr.__isset.scanner)
    out <<" scanner="<< hr.scanner;

  if (hr.__isset.mutator)
    out <<" mutator="<< hr.mutator;

  return out <<'}';
}

std::ostream &operator<<(std::ostream &out, const ClientException &e) {
  return out <<"{ClientException: code="<< e.code <<" what='"<< e.what <<"'}";
}

// must be synced with AUTO_ASSIGN in Hypertable/Lib/KeySpec.h
const int64_t AUTO_ASSIGN = INT64_MIN + 2;

Cell
make_cell(const char *row, const char *cf, const char *cq,
          const std::string &value, int64_t ts, int64_t rev, CellFlag flag) {
  Cell cell;

  cell.row_key = row;
  cell.column_family = cf;
  cell.timestamp = ts;
  cell.revision = rev;
  cell.flag = flag;
  cell.__isset.row_key = cell.__isset.column_family = cell.__isset.timestamp
      = cell.__isset.revision = cell.__isset.flag = true;

  if (cq) {
    cell.column_qualifier = cq;
    cell.__isset.column_qualifier = true;
  }
  if (value.size()) {
    cell.value = value;
    cell.__isset.value = true;
  }
  return cell;
}

Cell
make_cell(const char *row, const char *cf, const char *cq,
    const std::string &value, const char *ts, const char *rev, CellFlag flag) {
  int64_t revn = rev ? atoll(rev) : AUTO_ASSIGN;

  if (ts)
    return make_cell(row, cf, cq, value, parse_ts(ts), revn, flag);
  else
    return make_cell(row, cf, cq, value, AUTO_ASSIGN, revn, flag);
}

}} // namespace Hypertable::Thrift
