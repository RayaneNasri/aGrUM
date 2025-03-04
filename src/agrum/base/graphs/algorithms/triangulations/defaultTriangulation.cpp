/**
 *
 *   Copyright (c) 2005-2024  by Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
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


/** @file
 * @brief source implementations for computing default triangulations of graphs
 *
 * @author Christophe GONZALES(_at_AMU) and Pierre-Henri WUILLEMIN(_at_LIP6)
 */

#include <agrum/agrum.h>

#include <agrum/base/graphs/algorithms/triangulations/defaultTriangulation.h>
#include <agrum/base/graphs/algorithms/triangulations/eliminationStrategies/defaultEliminationSequenceStrategy.h>
#include <agrum/base/graphs/algorithms/triangulations/junctionTreeStrategies/defaultJunctionTreeStrategy.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace gum {

  /// basic constructor. initialize the triangulation
  DefaultTriangulation::DefaultTriangulation(const UndiGraph*            theGraph,
                                             const NodeProperty< Size >* domsizes,
                                             bool                        minimality,
                                             double                      theRatio,
                                             double                      theThreshold) :
      UnconstrainedTriangulation(theGraph,
                                 domsizes,
                                 DefaultEliminationSequenceStrategy(),
                                 DefaultJunctionTreeStrategy(),
                                 minimality),
      _quasi_ratio_(theRatio), _threshold_(theThreshold) {   // for debugging purposes
    GUM_CONSTRUCTOR(DefaultTriangulation);
  }

  /// default constructor: initialize the triangulation for an empty graph
  DefaultTriangulation::DefaultTriangulation(bool   minimality,
                                             double theRatio,
                                             double theThreshold) :
      UnconstrainedTriangulation(DefaultEliminationSequenceStrategy(),
                                 DefaultJunctionTreeStrategy(),
                                 minimality),
      _quasi_ratio_(theRatio), _threshold_(theThreshold) {   // for debugging purposes
    GUM_CONSTRUCTOR(DefaultTriangulation);
  }

  /// copy constructor
  DefaultTriangulation::DefaultTriangulation(const DefaultTriangulation& from) :
      UnconstrainedTriangulation(from), _quasi_ratio_(from._quasi_ratio_),
      _threshold_(from._threshold_) {   // for debugging purposes
    GUM_CONS_CPY(DefaultTriangulation);
  }

  /// move constructor
  DefaultTriangulation::DefaultTriangulation(DefaultTriangulation&& from) :
      UnconstrainedTriangulation(std::move(from)), _quasi_ratio_(from._quasi_ratio_),
      _threshold_(from._threshold_) {
    // for debugging purposes
    GUM_CONS_MOV(DefaultTriangulation);
  }

  /// destructor
  DefaultTriangulation::~DefaultTriangulation() {
    // for debugging purposes
    GUM_DESTRUCTOR(DefaultTriangulation);
  }

  /// virtual clone constructor
  DefaultTriangulation* DefaultTriangulation::newFactory() const {
    return new DefaultTriangulation(isMinimalityRequired(), _quasi_ratio_, _threshold_);
  }

  /// virtual copy constructor
  DefaultTriangulation* DefaultTriangulation::copyFactory() const {
    return new DefaultTriangulation(*this);
  }

} /* namespace gum */

#endif /* DOXYGEN_SHOULD_SKIP_THIS */
