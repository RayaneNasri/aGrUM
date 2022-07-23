%feature("docstring") gum::InfluenceDiagram
"
InfluenceDiagram represents an Influence Diagram.

InfluenceDiagram() -> InfluenceDiagram
    default constructor

InfluenceDiagram(source) -> InfluenceDiagram
    Parameters:
        * **source** (*pyAgrum.InfluenceDiagram*) -- the InfluenceDiagram to copy
"

%feature("docstring") gum::InfluenceDiagram::add
"
Add a variable, it's associate node and it's CPT.

The id of the new variable is automatically generated.

Parameters
----------
variable : pyAgrum.DiscreteVariable
	The variable added by copy that will be a chance node.
descr: str
  the descr of the variable following :ref:`fast syntax<Quick specification of (randomly parameterized) graphical models>` extended for :func:`pyAgrum.fastID`.
nbr_mod_or_id : int
	if the first argument is `variable`, this set an optional fixed id for the node. If the first argument is `descr`, this gives the default number of modalities
	for the variable. Note that if a utility node is described in `descr`, this value is overriden by 1.

Returns
-------
int
    the id of the added variable.

Raises
------
  pyAgrum.DuplicateElement
	  If already used id or name.
"

%feature("docstring") gum::InfluenceDiagram::addArc
"
Add an arc in the ID, and update diagram's potential nodes cpt if necessary.

Parameters
----------
tail : Union[int,str]
	a variable's id (int) or name
head : Union[int,str]
	a variable's id (int) or name

Raises
------
  pyAgrum.InvalidEdge
	If arc.tail and/or arc.head are not in the ID.
  pyAgrum.InvalidEdge
	If tail is a utility node
"

%feature("docstring") gum::InfluenceDiagram::addChanceNode
"
Add a chance variable, it's associate node and it's CPT.

The id of the new variable is automatically generated.

Parameters
----------
variable : pyAgrum.DiscreteVariable
	the variable added by copy.
id : int
	the chosen id. If 0, the NodeGraphPart will choose.

Warnings
--------
give an id (not 0) should be reserved for rare and specific situations !!!

Returns
-------
int
    the id of the added variable.

Raises
------
pyAgrum.DuplicateElement
	If id(<>0) is already used
"

%feature("docstring") gum::InfluenceDiagram::addDecisionNode
"
Add a decision variable.

The id of the new variable is automatically generated.

Parameters
----------
variable : pyAgrum.DiscreteVariable
	the variable added by copy.
id : int
	the chosen id. If 0, the NodeGraphPart will choose.

Warnings
--------
give an id (not 0) should be reserved for rare and specific situations !!!

Returns
-------
int
    the id of the added variable.

Raises
------
pyAgrum.DuplicateElement
	If id(<>0) is already used
"

%feature("docstring") gum::InfluenceDiagram::addUtilityNode
"
Add a utility variable, it's associate node and it's UT.

The id of the new variable is automatically generated.

Parameters
----------
variable : pyAgrum.DiscreteVariable
	the variable added by copy
id : int
	the chosen id. If 0, the NodeGraphPart will choose

Warnings
--------
give an id (not 0) should be reserved for rare and specific situations !!!

Returns
-------
int
    the id of the added variable.

Raises
------
pyAgrum.InvalidArgument
	If variable has more than one label
pyAgrum.DuplicateElement
	If id(<>0) is already used
"

%feature("docstring") gum::InfluenceDiagram::arcs
"
Returns
-------
list:
	the list of all the arcs in the Influence Diagram.
"

%feature("docstring") gum::InfluenceDiagram::chanceNodeSize
"
Returns
-------
int
	the number of chance nodes.
"

%feature("docstring") gum::InfluenceDiagram::changeVariableName
"
Parameters
----------
var : Union[int,str]
	a variable's id (int) or name
new_name : str
	the name of the variable

Raises
------
pyAgrum.DuplicateLabel
	If this name already exists
pyAgrum.NotFound
	If no nodes matches id.
"

%feature("docstring") gum::InfluenceDiagram::children
"
Parameters
----------
var : Union[int,str]
	a variable's id (int) or name

Returns
-------
Set
	the set of all the children
"

%feature("docstring") gum::InfluenceDiagram::cpt
"
Returns the CPT of a variable.

Parameters
----------
var : Union[int,str]
	a variable's id (int) or name

Returns
-------
pyAgrum.Potential
	The variable's CPT.

Raises
------
pyAgrum.NotFound
	If no variable's id matches varId.
"

%feature("docstring") gum::InfluenceDiagram::decisionNodeSize
"
Returns
-------
int
	the number of decision nodes
"

%feature("docstring") gum::InfluenceDiagram::decisionOrderExists
"
Returns
-------
bool
	True if a directed path exist with all decision node
"

%feature("docstring") gum::InfluenceDiagram::erase
"
Erase a Variable from the network and remove the variable from all his childs.

If no variable matches the id, then nothing is done.

Parameters
----------
id : int
	The id of the variable to erase.
var :  Union[int,str,pyAgrum.DiscreteVariable]
	a variable's id (int) or name or th reference on the variable to remove.
"

%feature("docstring") gum::InfluenceDiagram::eraseArc
"
Removes an arc in the ID, and update diagram's potential nodes cpt if necessary.

If (tail, head) doesn't exist, the nothing happens.

Parameters
----------
arc : pyAgrum.Arc
	The arc to be removed whn calling eraseArc(arc)
tail : Union[int,str]
	a variable's id (int) or name when calling eraseArc(tail,head)
head : Union[int,str]
	a variable's id (int) or name when calling eraseArc(tail,head)
"

%feature("docstring") gum::InfluenceDiagram::existsPathBetween
"
Returns
-------
bool
	true if a path exists between two nodes.
"

%feature("docstring") gum::InfluenceDiagram::getDecisionGraph
"
Returns
-------
pyAgrum.DAG
	the temporal Graph.
"

%feature("docstring") gum::InfluenceDiagram::getDecisionOrder
"
Returns
-------
list
	the sequence of decision nodes in the directed path.

Raises
------
NotFound
	If such a path does not exist
"

%feature("docstring") gum::InfluenceDiagram::idFromName
"
Returns a variable's id given its name.

Parameters
----------
name : str
	the variable's name from which the id is returned.

Returns
-------
int
	the variable's node id.

Raises
------
pyAgrum.NotFound
	If no such name exists in the graph.
"

%feature("docstring") gum::InfluenceDiagram::ids
"
.. note:: Deprecated in pyAgrum>0.13.0
    Please use nodes() instead
"

%feature("docstring") gum::InfluenceDiagram::nodes
"
Returns
-------
set
    the set of ids
"

%feature("docstring") gum::InfluenceDiagram::isChanceNode
"
Parameters
----------
varId : int
	the tested node id.

Returns
-------
bool
	true if node is a chance node
"

%feature("docstring") gum::InfluenceDiagram::isDecisionNode
"
Parameters
----------
varId : int
	the tested node id.

Returns
-------
bool
	true if node is a decision node
"

%feature("docstring") gum::InfluenceDiagram::isUtilityNode
"
Parameters
----------
varId : int
	the tested node id.

Returns
-------
bool
	true if node is an utility node
"

%feature("docstring") gum::InfluenceDiagram::loadBIFXML
"
Load a BIFXML file.

Parameters
----------
name : str
	the name's file

Raises
------
pyAgrum.IOError
	If file not found
pyAgrum.FatalError
	If file is not valid
"

%feature("docstring") gum::InfluenceDiagram::names
"
Returns
-------
List[str]
	The names of the InfluenceDiagram variables
"


%feature("docstring") gum::InfluenceDiagram::nodeId
"
Parameters
----------
var : pyAgrum.DiscreteVariable
	a variable

Returns
-------
int
	the id of the variable

Raises
------
pyAgrum.IndexError
	If the InfluenceDiagram does not contain the variable
"

%feature("docstring") gum::InfluenceDiagram::parents
"
Parameters
----------
var : Union[int,str]
	a variable's id (int) or name

Returns
-------
set
    the set of the parents ids.
"

%feature("docstring") gum::InfluenceDiagram::saveBIFXML
"
Save the BayesNet in a BIFXML file.

Parameters
----------
name : str
	the file's name
"

%feature("docstring") gum::InfluenceDiagram::toDot
"
Returns
-------
str
    a friendly display of the graph in DOT format
"

%feature("docstring") gum::InfluenceDiagram::utility
"
Parameters
----------
var : Union[int,str]
	a variable's id (int) or name

Returns
-------
pyAgrum.Potential
	the utility table of the node

Raises
------
pyAgrum.IndexError
	If the InfluenceDiagram does not contain the variable
"

%feature("docstring") gum::InfluenceDiagram::utilityNodeSize
"
Returns
-------
int
	the number of utility nodes
"

%feature("docstring") gum::InfluenceDiagram::variable
"
Parameters
----------
id : int
 	the node id

Returns
------
pyAgrum.DiscreteVariable
	a constant reference over a variabe given it's node id

Raises
------
pyAgrum.NotFound
	If no variable's id matches the parameter
"

%feature("docstring") gum::InfluenceDiagram::variableFromName
"
Parameters
----------
name : str
	a variable's name

Returns
-------
pyAgrum.DiscreteVariable
	the variable

Raises
------
pyAgrum.IndexError
	If the InfluenceDiagram does not contain the variable
"

%feature("docstring") gum::InfluenceDiagram::variableNodeMap
"
TBD
"

%feature("docstring") gum::InfluenceDiagram::fastPrototype
"
Create an Influence Diagram with a dot-like syntax which specifies:
    - the structure 'a->b<-c;b->d;c<-e;'.
    - a prefix for the type of node (chance/decision/utiliy nodes):

      - `a` : a chance node named 'a' (by default)
      - `$a` : a utility node named 'a'
      - `*a` : a decision node named 'a'

    - the type of the variables with different syntax as postfix:

      - by default, a variable is a pyAgrum.RangeVariable using the default domain size (second argument)
      - with `'a[10]'`, the variable is a pyAgrum.RangeVariable using 10 as domain size (from 0 to 9)
      - with `'a[3,7]'`, the variable is a pyAgrum.RangeVariable using a domainSize from 3 to 7
      - with `'a[1,3.14,5,6.2]'`, the variable is a pyAgrum.DiscretizedVariable using the given ticks (at least 3 values)
      - with `'a{top|middle|bottom}'`, the variable is a pyAgrum.LabelizedVariable using the given labels.
      - with 'a{-1|5|0|3}', the variable is a pyAgrum.IntegerVariable using the sorted given values.
      - with 'a{-0.5|5.01|0|3.1415}', the variable is a pyAgrum.NumericalDiscreteVariable using the sorted given values.

Note
----
  - If the dot-like string contains such a specification more than once for a variable, the first specification will be used.
  - the potentials (probabilities, utilities) are randomly generated.
  - see also pyAgrum.fastID.

Examples
--------
>>> import pyAgrum as gum
>>> bn=pyAgrum.fastID('A->B[1,3]<-*C{yes|No}->$D<-E[1,2.5,3.9]',6)

Parameters
----------
dotlike : str
        the string containing the specification
domainSize : int
        the default domain size for variables

Returns
-------
pyAgrum.InfluenceDiagram
        the resulting Influence Diagram
"
