#!/usr/bin/python
# -*- coding : utf-8 -*-
# ***************************************************************************
# *   Copyright (c) 2015-2023 by Pierre-Henri WUILLEMIN                     *
# *   {prenom.nom}_at_lip6.fr                                               *
# *                                                                         *
# *   "act" is free software; you can redistribute it and/or modify         *
# *   it under the terms of the GNU General Public License as published by  *
# *   the Free Software Foundation; either version 2 of the License, or     *
# *   (at your option) any later version.                                   *
# *                                                                         *
# *   This program is distributed in the hope that it will be useful,       *
# *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
# *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
# *   GNU General Public License for more details.                          *
# *                                                                         *
# *   You should have received a copy of the GNU General Public License     *
# *   along with this program; if not, write to the                         *
# *   Free Software Foundation, Inc.,                                       *
# *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
# **************************************************************************
import os
from subprocess import call

from .utils import trace, notif, critic, warn, error, recglob, srcAgrum, notif_oneline
from .configuration import cfg

from .missingDocs import computeNbrError
from .checkDependencies import check_gum_dependencies


def _aff_errors(nb: int, typ: str):
  if nb > 0:
    error(f"{nb} {typ} error{'s' if nb > 1 else ''}{' '*40}") # spaces to remove others possible characters
  return nb


def guideline(current, modif=False):
  if modif:
    notif("[aGrUM guideline (with correction)]")
  else:
    notif("[aGrUM guideline]")

  nbrError = 0

  notif("  [(1) ]*.cpp[ file for every ]*.h[ file]")
  nbrError += _aff_errors(_checkCppFileExists(current, modif),"missing cppfile")
  notif("  [(2) check for ]LGPL[ license]")
  nbrError += _aff_errors(_checkForLGPLlicense(current, modif),"missing LGPL licence")
  notif("  [(3) check for missing documentation in pyAgrum]")
  nbrError += _aff_errors(_checkForMissingDocs(modif),"missing documentation")
  notif("  [(5) check for format]")
  nbrError += _aff_errors(_checkForFormat(current, modif),"format")
  notif("  [(4) check for deps]")
  nbrError += _aff_errors(check_gum_dependencies(graph=current['build_graph']),"redundant dependency")

  return nbrError


def _checkForFormat(current, modif):
  nbrError = 0
  if cfg.clangformat is None:
    error("No correct [clang-format] tool has been found.")
  else:
    with open(os.devnull, "w") as blackhole:
      for src in srcAgrum():
        exceptions = [f'{os.sep}external{os.sep}', 'Parser', 'Scanner']
        if any(subs in src for subs in exceptions):
          continue

        line = cfg.clangformat + " " + src + " | cmp " + src + " -"
        if call(line, shell=True, stderr=blackhole, stdout=blackhole) == 1:
          nbrError += 1
          if modif:
            line = cfg.clangformat + " -i " + src
            call(line, shell=True)
            notif("    [" + src + "] not correctly formatted : [changed]")
          else:
            notif("    [" + src + "] not correctly formatted")
        else:
          notif_oneline("    [" + src.split("/")[-1] + "] OK")
  return nbrError


def __addLGPLatTop(filename):
  with open(filename, "r") as origine:
    codes = origine.read().split("***********/")

  # removing old license if any
  if len(codes) == 1:
    code = codes[0]
  elif len(codes) == 2:
    code = codes[1]
  else:
    code = "***********/".join(codes[1:])

  with open(filename, "w") as dest:
    dest.write(_template_license)
    dest.write(code)


def _checkForLGPLlicense(current, modif):
  nbrError = 0

  exceptions = [f'{os.sep}mvsc{os.sep}', f'{os.sep}external{os.sep}', f'{os.sep}cxxtest{os.sep}', 'Parser', 'Scanner']
  for agrumfile in srcAgrum():
    if any(subs in agrumfile for subs in exceptions):
      continue

    fragment = ""
    nbr = 0
    with open(agrumfile, "r") as f:
      for line in f:
        if nbr == 40:
          continue
        fragment += line
        nbr += 1

    if "GNU Lesser General Public License" not in fragment:
      nbrError += 1
      if modif:
        __addLGPLatTop(agrumfile)
        notif(
          "    [" + agrumfile + "] has no LGPL copyright in its first lines : [changed]")
      else:
        notif("    [" + agrumfile + "] has no LGPL copyright in its first lines")

  return nbrError


def __addCppFileForHeader(header, cppfile):
  subinclude = header[4:]  # remove the /src
  cppfile = header[:-1] + "cpp"  # name

  with open(cppfile, 'w') as out:
    out.write(_template_cpp.replace("{include_file}", subinclude))


def _checkCppFileExists(current, modif):
  nbrError = 0

  exceptions = [f'{os.sep}mvsc{os.sep}', f'{os.sep}signal{os.sep}', f'{os.sep}external{os.sep}',
                f'multidim{os.sep}patterns{os.sep}', 'agrum.h', 'inline.h']
  for header in recglob(f"src{os.sep}agrum", "*.h"):
    if any(subs in header for subs in exceptions):
      continue

    subs = header[:-1]
    if subs.endswith("_tpl."):
      continue
    if subs.endswith("_inl."):
      continue
    cppfile = subs + "cpp"
    if not os.path.isfile(cppfile):
      nbrError += 1
      if modif:
        __addCppFileForHeader(header, cppfile)
        error("No cpp file for [" + header + "h] : [added]")
      else:
        error("No cpp file for [" + header + "h]")

  return nbrError


def _checkForMissingDocs(modif):
  return computeNbrError(modif)


_template_license = """
/**
 *
 *  Copyright 2005-2023 Pierre-Henri WUILLEMIN (@LIP6) and Christophe GONZALES (@AMU)
 *   {prenom.nom}_at_lip6.fr
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

"""
_template_cpp = _template_license + """

/**
 * @file
 * @brief Class to include at least once this header
 *
 * @author Pierre-Henri WUILLEMIN (@LIP6) and Christophe GONZALES (@AMU)
 */

#include <{include_file}>

"""
