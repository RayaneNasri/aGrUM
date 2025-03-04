/**
 *
 *  Copyright 2005-2024 Pierre-Henri WUILLEMIN et Christophe GONZALES (LIP6)
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

/* INCLUDES */
%{
#include <agrum/mrf.h>
%}


%include "typemaps.i"
%include "std_vector.i"
%include "std_string.i"

%include "forUsing.i"


%include <agrum/MRF/IMarkovRandomField.h>
%include <agrum/MRF/MarkovRandomField.h>
%include <agrum/MRF/inference/ShaferShenoyMRFInference.h>

%template (IMarkovRandomField ) gum::IMarkovRandomField<double>;
%template ( MarkovRandomField ) gum::MarkovRandomField<double>;
%template ( ShaferShenoyMRFInference ) gum::ShaferShenoyMRFInference<double>;
