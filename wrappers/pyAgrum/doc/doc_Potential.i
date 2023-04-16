%feature("docstring") gum::Potential
"
Class representing a potential.

Potential() -> Potential
    default constructor

Potential(src) -> Potential
    Parameters:
        - **src** (*pyAgrum.Potential*) -- the Potential to copy
"

%feature("docstring") gum::Potential::KL
"
Check the compatibility and compute the Kullback-Leibler divergence between the potential and.

Parameters
----------
p : pyAgrum.Potential
  the potential from which we want to calculate the divergence.

Returns
-------
float
  The value of the divergence

Raises
------
  pyAgrum.InvalidArgument
    If p is not compatible with the potential (dimension, variables)
  pyAgrum.FatalError
    If a zero is found in p or the potential and not in the other.
"

%feature("docstring") gum::Potential::abs
"
Apply abs on every element of the container

Returns
-------
pyAgrum.Potential
    a reference to the modified potential.
"

%feature("docstring") gum::Potential::add
"
Add a discrete variable to the potential.

Parameters
----------
v : pyAgrum.DiscreteVariable
  the var to be added

Raises
------
DuplicateElement
  If the variable is already in this Potential.
InvalidArgument
  If the variable is empty.

Returns
-------
pyAgrum.Potential
    a reference to the modified potential.
"

%feature("docstring") gum::Potential::contains
"
Parameters
----------
v : pyAgrum.Potential
    a DiscreteVariable.

Returns
-------
bool
    True if the var is in the potential
"

%feature("docstring") gum::Potential::empty
"
Returns
-------
bool
    Returns true if no variable is in the potential.
"

%feature("docstring") gum::Potential::entropy
"
Returns
-------
float
  the entropy of the potential
"

%feature("docstring") gum::Potential::extract
"
create a new Potential extracted from self given a partial instantiation.

Parameters
----------
inst : pyAgrum.instantiation
  a partial instantiation
dict : Dict[str,str|int]
  a dictionnary containing values for some discrete variables.

Warning
--------
    if the dictionnary contains a key that is not the name of a variable in the `pyAgrum.Potential`,
    this key is just not used without notification. Then `pyAgrum.Potential.extract` concerns
    only the variables that  both are in the Potential and in the dictionnary.

Returns
-------
pyAgrum.Potential
  the new Potential
"
%feature("docstring") gum::Potential::fill
"
Automatically fills the potential with v.

Parameters
----------
v : number or list or pyAgrum.Potential the number of parameters of the Potential
    a value or a list/pyAgrum.Potential containing the values to fill the Potential with.

Warning
-------
    if v is a list, the size of the list must be the
    if v is a pyAgrum.Potential. It must to contain variables with exactly the same names and labels but not necessarily the same variables.

Returns
-------
pyAgrum.Potential
      a reference to the modified potentia

Raises
------
pyAgrum.SizeError
  If v size's does not matches the domain size.
"

%feature("docstring") gum::Potential::fillWith
"
Automatically fills the potential with v.

Parameters
----------
v : number or list of values or pyAgrum.Potential
    a value or a list/pyAgrum.Potential containing the values to fill the Potential with.

mapping : list|tuple|dict

Warning
-------
    - if `v` is a list, the size of the list must be the size of the potential

    - if `v` is a ref:pyAgrum.Potential, it must contain variables with exactly the same names and labels but not necessarily the same variables. If

    - If the second argument `mapping` is given, `mapping` explains how to map the variables of the potential source to the variables of the potential destination.

    - If `mapping` is a sequence, the order follows the same order as `destination.names`. If `mapping` is a dict, the keys are the names in the destination and the values are the names in the source.

Returns
-------
pyAgrum.Potential
      a reference to the modified potentia

Raises
------
pyAgrum.SizeError
  If v size's does not matches the domain size.
pyAgrum.ArgumentError
  If anything wrong with the arguments.
"

%feature("docstring") gum::Potential::populate
"
Raises:
--------
pyAgrum.SizeError
  if v size's does not matches the domain size.
"

%feature("docstring") gum::Potential::get
"
Parameters
----------
i : pyAgrum.Instantiation
  an Instantiation

Returns
-------
float
  the value in the Potential at the position given by the instantiation
"

%feature("docstring") gum::Potential::isNonZeroMap
"
Returns
-------
pyAgrum.Potential
  a boolean-like potential using the predicate `isNonZero`.
"

%feature("docstring") gum::Potential::margMaxIn
"
Projection using `max` as operation.

Parameters
----------
varnames : set
  the set of vars to keep

Returns
-------
pyAgrum.Potential
  the projected Potential
"

%feature("docstring") gum::Potential::margMaxOut
"
Projection using `max` as operation.

Parameters
----------
varnames : set
  the set of vars to eliminate

Returns
-------
pyAgrum.Potential
  the projected Potential

Raises
------
pyAgrum.InvalidArgument
  If varnames contains only one variable that does not exist in the Potential
"

%feature("docstring") gum::Potential::margMinIn
"
Projection using `min` as operation.

Parameters
----------
varnames : set
  the set of vars to keep

Returns
-------
pyAgrum.Potential
  the projected Potential
"

%feature("docstring") gum::Potential::margMinOut
"
Projection using `min` as operation.

Parameters
----------
varnames : set
  the set of vars to eliminate

Returns
-------
pyAgrum.Potential
  the projected Potential

Warnings
--------
InvalidArgument raised if varnames contains only one variable that does not exist in the Potential
"

%feature("docstring") gum::Potential::margProdIn
"
Projection using multiplication as operation.

Parameters
----------
varnames : set
  the set of vars to keep

Returns
-------
pyAgrum.Potential
  the projected Potential
"

%feature("docstring") gum::Potential::margProdOut
"
Projection using multiplication as operation.

Parameters
----------
varnames : set
  the set of vars to eliminate

Returns
-------
pyAgrum.Potential
  the projected Potential

Raises
------
pyAgrum.InvalidArgument
  If varnames contains only one variable that does not exist in the Potential
"

%feature("docstring") gum::Potential::margSumIn
"
Projection using sum as operation.

Parameters
----------
varnames : set
  the set of vars to keep

Returns
-------
pyAgrum.Potential
  the projected Potential
"

%feature("docstring") gum::Potential::margSumOut
"
Projection using sum as operation.

Parameters
----------
varnames : set
  the set of vars to eliminate

Returns
-------
pyAgrum.Potential
  the projected Potential

Raises
------
pyAgrum.InvalidArgument
  If varnames contains only one variable that does not exist in the Potential
"

%feature("docstring") gum::Potential::argmax
"
Returns
-------
Tuple[Dict[str,int],float]
  the list of positions of the max and the max of all elements in the Potential
"

%feature("docstring") gum::Potential::max
"
Returns
-------
float
  the maximum of all elements in the Potential
"

%feature("docstring") gum::Potential::maxNonOne
"
Returns
-------
float
  the maximum of non one elements in the Potential

Raises
------
pyAgrum.NotFound
  If all value == 1.0
"

%feature("docstring") gum::Potential::argmin
"
Returns
-------
Tuple[Dict[str,int],float]
  the list of positions of the min and the min of all elements in the Potential
"

%feature("docstring") gum::Potential::min
"
Returns
-------
float
  the min of all elements in the Potential
"

%feature("docstring") gum::Potential::minNonZero
"
Returns
-------
float
  the min of non zero elements in the Potential

Raises
------
pyAgrum.NotFound
  If all value == 0.0
"

%feature("docstring") gum::Potential::nbrDim
"
Returns
-------
int
  the number of vars in the multidimensional container.
"

%feature("docstring") gum::Potential::newFactory
"
Erase the Potential content and create a new empty one.

Returns
-------
pyAgrum.Potential
 a reference to the new Potential
"

%feature("docstring") gum::Potential::normalize
"
Normalize the Potential (do nothing if sum is 0)

Returns
-------
pyAgrum.Potential
  a reference to the normalized Potential
"

%feature("docstring") gum::Potential::normalizeAsCPT
"
Normalize the Potential as a CPT

Returns
-------
pyAgrum.Potential
  a reference to the normalized Potential

Raises
------
pyAgrum.FatalError
  If some distribution sums to 0
"

%feature("docstring") gum::Potential::pos
"
Parameters
----------
v : pyAgrum.DiscreteVariable
    The variable for which the index is returned.

Returns
-------
    Returns the index of a variable.

Raises
------
pyAgrum.NotFound
  If v is not in this multidimensional matrix.
"

%feature("docstring") gum::Potential::product
"
Returns
-------
float
  the product of all elements in the Potential
"

%feature("docstring") gum::Potential::putFirst
"
Parameters
----------
v : pyAgrum.DiscreteVariable
    The variable for which the index should be 0.

Returns
-------
pyAgrum.Potential
  a reference to the modified potential

Raises
------
pyAgrum.InvalidArgument
  If the var is not in the potential
"

%feature("docstring") gum::Potential::remove
"
Parameters
----------
v : pyAgrum.DiscreteVariable
    The variable to be removed

Returns
-------
pyAgrum.Potential
  a reference to the modified potential

Warnings
--------
IndexError raised if the var is not in the potential
"

%feature("docstring") gum::Potential::reorganize
"
Create a new Potential with another order.

Returns
-------
varnames : list
  a list of the var names in the new order

Returns
-------
pyAgrum.Potential
  a reference to the modified potential
"

%feature("docstring") gum::Potential::scale
"
Create a new potential multiplied by v.

Parameters
----------
v : float
  a multiplier

Returns
-------
  a reference to the modified potential
"

%feature("docstring") gum::Potential::set
"
Change the value pointed by i

Parameters
----------
i : pyAgrum.Instantiation
  The Instantiation to be changed
value : float
  The new value of the Instantiation
"

%feature("docstring") gum::Potential::sq
"
Square all the values in the Potential
"

%feature("docstring") gum::Potential::log2
"
log2 all the values in the Potential

Warning
-------
When the Potential contains 0 or negative values, no exception are raised but `-inf` or `nan` values are assigned.
"

%feature("docstring") gum::Potential::sum
"
Returns
-------
float :
  the sum of all elements in the Potential
"

%feature("docstring") gum::Potential::translate
"
Create a new potential added with v.

Parameters
----------
v : float
  The value to be added

Returns
-------
  a reference to the modified potential
"

%feature("docstring") gum::Potential::variable
"
Parameters
----------
i : int
  An index of this multidimensional matrix.

Returns
-------
  the varible at the ith index

Raises
------
pyAgrum.NotFound
  If i does not reference a variable in this multidimensional matrix.
"


%feature("docstring") gum::Potential::draw
"
draw a value using the potential as a probability table.

Returns
-------
int
  the index of the drawn value
"

%feature("docstring") gum::Potential::expectedValue
"
Calculate the mathematical expected value of a (joint) random variable using the given function as an argument.

Parameters
----------
func : function(Dict[str,int])->float
    A function that takes a single argument, representing the value of a python representation of a `gum.Instantiation` (as a dictionary), and returns a float.

Warnings
--------
The `gum.Potential` is assumed to contain a joint distribution.

Example
-------
def log2cptA(x):
 return -math.log2(bn.cpt('A')[x])
entropy_of_A=bn.cpt('A').expectedValue(log2cptA) # OK it A has no parents.

Returns
-------
float
    The mathematical expected value of the random variable calculated using the given function as an argument.
"
