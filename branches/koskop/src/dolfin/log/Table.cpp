// Copyright (C) 2008-2011 Anders Logg
//
// This file is part of DOLFIN.
//
// DOLFIN is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// DOLFIN is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with DOLFIN. If not, see <http://www.gnu.org/licenses/>.
//
// First added:  2008-07-19
// Last changed: 2011-11-15

#include <iomanip>
#include <iostream>
#include <sstream>

#include <dolfin/common/constants.h>
#include <dolfin/log/LogStream.h>
#include <dolfin/log/log.h>
#include "Table.h"

using namespace dolfin;

typedef std::vector<std::string>::const_iterator iterator;

//-----------------------------------------------------------------------------
Table::Table(std::string title, bool right_justify)
  : _title(title), right_justify(right_justify)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
Table::~Table()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
TableEntry Table::operator() (std::string row, std::string col)
{
  TableEntry entry(row, col, *this);
  return entry;
}
//-----------------------------------------------------------------------------
void Table::set(std::string row, std::string col, int value)
{
  std::stringstream s;
  s << value;
  set(row, col, s.str());
}
//-----------------------------------------------------------------------------
void Table::set(std::string row, std::string col, uint value)
{
  std::stringstream s;
  s << value;
  set(row, col, s.str());
}
//-----------------------------------------------------------------------------
void Table::set(std::string row, std::string col, double value)
{
  if (std::abs(value) < DOLFIN_EPS)
    value = 0.0;
  std::stringstream s;
  s << std::setprecision(5) << value;
  set(row, col, s.str());
  dvalues[std::make_pair(row, col)] = value;
}
//-----------------------------------------------------------------------------
void Table::set(std::string row, std::string col, std::string value)
{
  // Add row
  if (row_set.find(row) == row_set.end())
  {
    rows.push_back(row);
    row_set.insert(row);
  }

  // Add column
  if (col_set.find(col) == col_set.end())
  {
    cols.push_back(col);
    col_set.insert(col);
  }

  // Store value
  std::pair<std::string, std::string> key(row, col);
  values[key] = value;
}
//-----------------------------------------------------------------------------
std::string Table::get(std::string row, std::string col) const
{
  std::pair<std::string, std::string> key(row, col);
  std::map<std::pair<std::string, std::string>, std::string>::const_iterator
    it = values.find(key);

  if (it == values.end())
  {
    dolfin_error("Table.cpp",
                 "access table value",
                 "Missing table value for entry (\"%s\", \"%s\")",
                 row.c_str(), col.c_str());
  }

  return it->second;
}
//-----------------------------------------------------------------------------
double Table::get_value(std::string row, std::string col) const
{
  std::pair<std::string, std::string> key(row, col);
  std::map<std::pair<std::string, std::string>, double>::const_iterator
    it = dvalues.find(key);

  if (it == dvalues.end())
  {
    dolfin_error("Table.cpp",
                 "access table value",
                 "Missing double value for entry (\"%s\", \"%s\")",
                 row.c_str(), col.c_str());
  }

  return it->second;
}
//-----------------------------------------------------------------------------
std::string Table::title() const
{
  return _title;
}
//-----------------------------------------------------------------------------
/* Removed after storing values as strings instead of double
Table Table::operator+ (const Table& table) const
{
  // Check table sizes
  if (rows.size() != table.rows.size() || cols.size() != table.cols.size())
  {
    dolfin_error("Table.cpp",
                 "add tables",
                 "Dimension mismatch for addition of tables");
  }

  // Add tables
  Table t;
  for (iterator i = rows.begin(); i !=rows.end(); i++)
    for (iterator j = cols.begin(); j != cols.end(); j++)
      t.set(*i, *j, get(*i, *j) + table.get(*i, *j));

  return t;
}
//-----------------------------------------------------------------------------
Table Table::operator- (const Table& table) const
{
  // Check table sizes
  if (rows.size() != table.rows.size() || cols.size() != table.cols.size())
  {
    dolfin_error("Table.cpp",
                 "subtract tables",
                 "Dimension mismatch for addition of tables");
  }

  // Add tables
  Table t;
  for (iterator i = rows.begin(); i !=rows.end(); i++)
    for (iterator j = cols.begin(); j != cols.end(); j++)
      t.set(*i, *j, get(*i, *j) - table.get(*i, *j));

  return t;
}
*/
//-----------------------------------------------------------------------------
const Table& Table::operator= (const Table& table)
{
  // Assign everything but the title

  rows = table.rows;
  row_set = table.row_set;

  cols = table.cols;
  col_set = table.col_set;

  values = table.values;

  return *this;
}
//-----------------------------------------------------------------------------
std::string Table::str(bool verbose) const
{
  std::stringstream s;

  if (verbose)
  {
    //s << str(false) << std::endl << std::endl;

    std::vector<std::vector<std::string> > tvalues;
    std::vector<uint> col_sizes;

    // Format values and compute column sizes
    col_sizes.push_back(_title.size());
    for (uint j = 0; j < cols.size(); j++)
      col_sizes.push_back(cols[j].size());
    for (uint i = 0; i < rows.size(); i++)
    {
      tvalues.push_back(std::vector<std::string>());
      col_sizes[0] = std::max(col_sizes[0], (dolfin::uint)(rows[i].size()));
      for (uint j = 0; j < cols.size(); j++)
      {
        std::string value = get(rows[i], cols[j]);
        tvalues[i].push_back(value);
        col_sizes[j + 1] = std::max(col_sizes[j + 1], (dolfin::uint)(value.size()));
      }
    }
    uint row_size = 2*col_sizes.size() + 1;
    for (uint j = 0; j < col_sizes.size(); j++)
      row_size += col_sizes[j];

    // Write table
    s << _title;
    for (uint k = 0; k < col_sizes[0] - _title.size(); k++)
      s << " ";
    s << "  |";
    for (uint j = 0; j < cols.size(); j++)
    {
      if (right_justify)
      {
        for (uint k = 0; k < col_sizes[j + 1] - cols[j].size(); k++)
          s << " ";
        s << "  " << cols[j];
      }
      else
      {
        s << "  " << cols[j];
        for (uint k = 0; k < col_sizes[j + 1] - cols[j].size(); k++)
          s << " ";
      }
    }
    s << "\n";
    for (uint k = 0; k < row_size; k++)
      s << "-";
    for (uint i = 0; i < rows.size(); i++)
    {
      s << "\n";
      s << rows[i];
      for (uint k = 0; k < col_sizes[0] - rows[i].size(); k++)
        s << " ";
      s << "  |";
      for (uint j = 0; j < cols.size(); j++)
      {
        if (right_justify)
        {
          for (uint k = 0; k < col_sizes[j + 1] - tvalues[i][j].size(); k++)
            s << " ";
          s << "  " << tvalues[i][j];
        }
        else
        {
          s << "  " << tvalues[i][j];
          for (uint k = 0; k < col_sizes[j + 1] - tvalues[i][j].size(); k++)
            s << " ";
        }
      }
    }
  }
  else
  {
    s << "<Table of size " << rows.size() << " x " << cols.size() << ">";
  }

  return s.str();
}
//-----------------------------------------------------------------------------
std::string Table::str_latex() const
{
  if (rows.size() == 0 || cols.size() == 0)
    return "Empty table";

  std::stringstream s;

  s << _title << "\n";
  s << "\\begin{center}\n";
  s << "\\begin{tabular}{|l|";
  for (uint j = 0; j < cols.size(); j++)
    s << "|c";
  s << "|}\n";
  s << "\\hline\n";
  s << "& ";
  for (uint j = 0; j < cols.size(); j++)
  {
    if (j < cols.size() - 1)
      s << cols[j] << " & ";
    else
      s << cols[j] << " \\\\\n";
  }
  s << "\\hline\\hline\n";
  for (uint i = 0; i < rows.size(); i++)
  {
    s << rows[i] << " & ";
    for (uint j = 0; j < cols.size(); j++)
    {
      if (j < cols.size() - 1)
        s << get(rows[i], cols[j]) << " & ";
      else
        s << get(rows[i], cols[j]) << " \\\\\n";
    }
    s << "\\hline\n";
  }
  s << "\\end{tabular}\n";
  s << "\\end{center}\n";

  return s.str();
}
//-----------------------------------------------------------------------------
TableEntry::TableEntry(std::string row, std::string col, Table& table)
  : row(row), col(col), table(table)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
TableEntry::~TableEntry()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
const TableEntry& TableEntry::operator= (uint value)
{
  table.set(row, col, value);
  return *this;
}
//-----------------------------------------------------------------------------
const TableEntry& TableEntry::operator= (int value)
{
  table.set(row, col, value);
  return *this;
}
//-----------------------------------------------------------------------------
const TableEntry& TableEntry::operator= (double value)
{
  table.set(row, col, value);
  return *this;
}
//-----------------------------------------------------------------------------
const TableEntry& TableEntry::operator= (std::string value)
{
  table.set(row, col, value);
  return *this;
}
//-----------------------------------------------------------------------------
TableEntry::operator std::string() const
{
  return table.get(row, col);
}
//-----------------------------------------------------------------------------
