/**
 *
 *   Copyright (c) 2005-2023  by Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
 *   info_at_agrum_dot_org
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


/**
 * @file
 * @brief gum::ApproximationSchemeListener header file.
 *
 * @author Christophe GONZALES(_at_AMU) and Pierre-Henri WUILLEMIN(_at_LIP6)
 */

#include <agrum/tools/core/approximations/approximationSchemeListener.h>

namespace gum {

  ApproximationSchemeListener::ApproximationSchemeListener(IApproximationSchemeConfiguration& sch) :
      _sch_(sch) {
    GUM_CONSTRUCTOR(ApproximationSchemeListener);

    GUM_CONNECT((_sch_), onProgress, (*this), ApproximationSchemeListener::whenProgress);
    GUM_CONNECT((_sch_), onStop, (*this), ApproximationSchemeListener::whenStop);
  }

  ApproximationSchemeListener::ApproximationSchemeListener(
     const ApproximationSchemeListener& other) :
      _sch_(other._sch_) {
    GUM_CONS_CPY(ApproximationSchemeListener);
    GUM_ERROR(OperationNotAllowed, "No copy constructor for ApproximationSchemeListener")
  }

  ApproximationSchemeListener::~ApproximationSchemeListener() {
    GUM_DESTRUCTOR(ApproximationSchemeListener);
  }

  ApproximationSchemeListener&
     ApproximationSchemeListener::operator=(const ApproximationSchemeListener& other) {
    GUM_CONS_CPY(ApproximationSchemeListener);
    GUM_ERROR(OperationNotAllowed, "No copy constructor for ApproximationSchemeListener")
  }

}   // namespace gum
