// Copyright (C) 2003-2011 Anders Logg
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
// Modified by Ola Skavhaug, 2007, 2009.
// Modified by Garth N. Wells, 2011.
//
// First added:  2003-03-13
// Last changed: 2011-11-15

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <dolfin/common/constants.h>
#include <dolfin/common/MPI.h>
#include <dolfin/common/types.h>
#include <dolfin/parameter/GlobalParameters.h>
#include "LogLevel.h"
#include "Table.h"
#include "Logger.h"

using namespace dolfin;

typedef std::map<std::string, std::pair<dolfin::uint, double> >::iterator map_iterator;
typedef std::map<std::string, std::pair<dolfin::uint, double> >::const_iterator const_map_iterator;

//-----------------------------------------------------------------------------
Logger::Logger()
  : active(true), log_level(INFO), indentation_level(0), logstream(&std::cout),
    num_processes(0), process_number(0)
{
  // Do nothing
}
//-----------------------------------------------------------------------------
Logger::~Logger()
{
  // Do nothing
}
//-----------------------------------------------------------------------------
void Logger::log(std::string msg, int log_level) const
{
  write(log_level, msg);
}
//-----------------------------------------------------------------------------
void Logger::log_underline(std::string msg, int log_level) const
{
  if (msg.size() == 0)
    log(msg, log_level);

  std::stringstream s;
  s << msg;
  s << "\n";
  for (int i = 0; i < indentation_level; i++)
    s << "  ";
  for (uint i = 0; i < msg.size(); i++)
    s << "-";

  log(s.str(), log_level);
}
//-----------------------------------------------------------------------------
void Logger::warning(std::string msg) const
{
  std::string s = std::string("*** Warning: ") + msg;
  write(WARNING, s);
}
//-----------------------------------------------------------------------------
void Logger::error(std::string msg) const
{
  std::string s = std::string("*** Error: ") + msg;
  throw std::runtime_error(s);
}
//-----------------------------------------------------------------------------
void Logger::dolfin_error(std::string location,
                          std::string task,
                          std::string reason) const
{
  std::stringstream s;
  s << std::endl << std::endl
    << "*** "
    << "-------------------------------------------------------------------------"
    << std::endl
    << "*** DOLFIN encountered an error. If you are not able to resolve this issue"
    << std::endl
    << "*** using the information listed below, you can ask for help at"
    << std::endl
    << "***" << std::endl
    << "***     https://answers.launchpad.net/dolfin"
    << std::endl
    << "***" << std::endl
    << "*** Remember to include the error message listed below and, if possible,"
    << std::endl
    << "*** include a *minimal* running example to reproduce the error."
    << std::endl
    << "***" << std::endl
    << "*** "
    << "-------------------------------------------------------------------------"
    << std::endl
    << "*** " << "Error:  Unable to " << task << "." << std::endl
    << "*** " << "Reason: " << reason << "." << std::endl
    << "*** " << "Where:  This error was encountered inside " << location << "."
    << std::endl
    << "*** "
    << "-------------------------------------------------------------------------"
    << std::endl;

  throw std::runtime_error(s.str());
}
//-----------------------------------------------------------------------------
void Logger::begin(std::string msg, int log_level)
{
  // Write a message
  log(msg, log_level);
  indentation_level++;
}
//-----------------------------------------------------------------------------
void Logger::end()
{
  indentation_level--;
}
//-----------------------------------------------------------------------------
void Logger::progress(std::string title, double p) const
{
  std::stringstream line;
  line << title << " [";

  const int N = DOLFIN_TERM_WIDTH - title.size() - 12 - 2*indentation_level;
  const int n = static_cast<int>(p*static_cast<double>(N));

  for (int i = 0; i < n; i++)
    line << '=';
  if (n < N)
    line << '>';
  for (int i = n+1; i < N; i++)
    line << ' ';

  line << std::setiosflags(std::ios::fixed);
  line << std::setprecision(1);
  line << "] " << 100.0*p << '%';

  write(PROGRESS, line.str());
}
//-----------------------------------------------------------------------------
void Logger::set_output_stream(std::ostream& ostream)
{
  logstream = &ostream;
}
//-----------------------------------------------------------------------------
void Logger::set_log_active(bool active)
{
  this->active = active;
}
//-----------------------------------------------------------------------------
void Logger::set_log_level(int log_level)
{
  this->log_level = log_level;
}
//-----------------------------------------------------------------------------
void Logger::register_timing(std::string task, double elapsed_time)
{
  // Remove small or negative numbers
  if (elapsed_time < DOLFIN_EPS)
    elapsed_time = 0.0;

  // Print a message
  std::stringstream line;
  line << "Elapsed time: " << elapsed_time << " (" << task << ")";
  log(line.str(), TRACE);

  // Store values for summary
  map_iterator it = timings.find(task);
  if (it == timings.end())
  {
    std::pair<uint, double> timing(1, elapsed_time);
    timings[task] = timing;
  }
  else
  {
    it->second.first += 1;
    it->second.second += elapsed_time;
  }
}
//-----------------------------------------------------------------------------
void Logger::summary(bool reset)
{
  if (timings.size() == 0)
  {
    log("Timings: no timings to report.");
    return;
  }

  log("");
  Table table("Summary of timings");
  for (const_map_iterator it = timings.begin(); it != timings.end(); ++it)
  {
    const std::string task    = it->first;
    const uint num_timings    = it->second.first;
    const double total_time   = it->second.second;
    const double average_time = total_time / static_cast<double>(num_timings);

    table(task, "Average time") = average_time;
    table(task, "Total time")   = total_time;
    table(task, "Reps")         = num_timings;
  }
  log(table.str(true));

  // Clear timings
  if (reset)
    timings.clear();
}
//-----------------------------------------------------------------------------
double Logger::timing(std::string task, bool reset)
{
  // Find timing
  map_iterator it = timings.find(task);
  if (it == timings.end())
  {
    std::stringstream line;
    line << "No timings registered for task \"" << task << "\".";
    dolfin_error("Logger.cpp",
                 "extract timing for task",
                 line.str());
  }

  // Compute average
  const uint num_timings  = it->second.first;
  const double total_time   = it->second.second;
  const double average_time = total_time / static_cast<double>(num_timings);

  // Clear timing
  timings.erase(it);

  return average_time;
}
//-----------------------------------------------------------------------------
void Logger::__debug(std::string msg) const
{
  std::string s = std::string("Debug: ") + msg;
  write(DBG, s);
}
//-----------------------------------------------------------------------------
void Logger::__dolfin_assert(std::string file, unsigned long line,
                      std::string function, std::string check) const
{
  std::stringstream location;
  location << file << " (line " << line << ")";
  std::stringstream task;
  task << "complete call to function " << function << "()";
  std::stringstream reason;
  reason << "Assertion " << check << " failed";
  dolfin_error(location.str(), task.str(), reason.str());
}
//-----------------------------------------------------------------------------
void Logger::write(int log_level, std::string msg) const
{
  // Check log level
  if (!active || log_level < this->log_level)
    return;

  // Get data from MPI (only first time)
  if (num_processes == 0)
  {
    num_processes = MPI::num_processes();
    process_number = MPI::process_number();
  }

  // Check if we want output on root process only
  const bool std_out_all_processes = parameters["std_out_all_processes"];
  if (process_number > 0 && !std_out_all_processes && log_level < WARNING)
    return;

  // Prefix with process number if running in parallel
  if (num_processes > 1)
  {
    std::stringstream prefix;
    prefix << "Process " << process_number << ": ";
    msg = prefix.str() + msg;
  }

  // Add indentation
  for (int i = 0; i < indentation_level; i++)
    msg = "  " + msg;

  // Write to stream
  *logstream << msg << std::endl;
}
//----------------------------------------------------------------------------
