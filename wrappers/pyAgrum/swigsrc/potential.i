/**
 *
 *  Copyright 2005-2023 Pierre-Henri WUILLEMIN et Christophe GONZALES (LIP6)
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
%ignore gum::MultiDimWithOffset;
%ignore gum::MultiDimImplementation;
%ignore gum::MultiDimInterface;
%ignore gum::MultiDimDecorator;
%ignore gum::MultiDimArray;

/* keep tracks of variables to trick with  garbage collector */
%pythonappend gum::Potential<double>::Potential %{
        self._list_vars=list()
%}
%pythonappend gum::Potential<double>::remove %{
        self._list_vars.remove(var)
%}

%pythonappend gum::Potential<double>::add %{
        self._list_vars.append(v)
        return self
%}


%define CHANGE_THEN_RETURN_SELF(methodname)
%pythonappend gum::Potential<double>::methodname %{
        return self
%}
%enddef

CHANGE_THEN_RETURN_SELF(abs)
CHANGE_THEN_RETURN_SELF(sq)
CHANGE_THEN_RETURN_SELF(log2)
CHANGE_THEN_RETURN_SELF(normalize)
CHANGE_THEN_RETURN_SELF(normalizeAsCPT)
CHANGE_THEN_RETURN_SELF(scale)
CHANGE_THEN_RETURN_SELF(inverse)
CHANGE_THEN_RETURN_SELF(translate)

CHANGE_THEN_RETURN_SELF(fillWith)
%pythonprepend gum::Potential<double>::fillWith %{
# test
if len(args)>1:
  d=args[1]
  if type(d)==dict:
    if set(d.keys())==set(self.names):
      return self.fillWith(args[0],[d[s] for s in self.names])
    else:
      raise pyAgrum.ArgumentError(f"[pyAgrum] keys in dict {tuple(d.keys())} does not match the Potential's variables {self.names}")
%}

%rename ("$ignore", fullname=1) gum::Potential<double>::margSumOut(const Set<const DiscreteVariable*>& del_vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::margProdOut(const Set<const DiscreteVariable*>& del_vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::margMaxOut(const Set<const DiscreteVariable*>& del_vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::margMinOut(const Set<const DiscreteVariable*>& del_vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::margSumIn(const Set<const DiscreteVariable*>& kept_vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::margProdIn(const Set<const DiscreteVariable*>& kept_vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::margMaxIn(const Set<const DiscreteVariable*>& kept_vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::margMinIn(const Set<const DiscreteVariable*>& kept_vars) const;

%rename ("$ignore", fullname=1) gum::Potential<double>::reorganize(const Set<const DiscreteVariable*>& vars) const;
%rename ("$ignore", fullname=1) gum::Potential<double>::putFirst(const DiscreteVariable* var) const;

%rename ("$ignore", fullname=1) gum::Potential<double>::argmin() const;
%rename ("$ignore", fullname=1) gum::Potential<double>::argmax() const;


%extend gum::Potential<double> {
  Potential<double> extract(PyObject* arg) {
    if (PyDict_Check(arg)) {
      gum::Instantiation inst;
      PyAgrumHelper::fillInstantiationFromPyObject(self, inst, arg);
      return self->extract(inst);
    } else {
      //auto arg1 = reinterpret_cast< gum::Instantiation * >(arg);
      //if (arg1==nullptr) {
        GUM_ERROR(gum::InvalidArgument,"arg is neither a dict or an pyAgrum.Instantiation.");
      //}
      //return self->extract(*arg1);
    }
  }

    Potential<double>
    margSumOut( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margSumOut(s);
    }

    Potential<double>
    margProdOut( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margProdOut(s);
    }

    Potential<double>
    margMaxOut( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margMaxOut(s);
    }

    Potential<double>
    margMinOut( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margMinOut(s);
    }

    Potential<double>
    margSumIn( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margSumIn(s);
    }

    Potential<double>
    margProdIn( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margProdIn(s);
    }

    Potential<double>
    margMaxIn( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margMaxIn(s);
    }

    Potential<double>
    margMinIn( PyObject* varnames ) const {
      gum::Set<const gum::DiscreteVariable*> s;
      PyAgrumHelper::fillDVSetFromPyObject(self,s,varnames); //from helpers.h
      return self->margMinIn(s);
    }

    PyObject* argmin() {
      const auto [argmi,mi] = self->argmin();
      return PyTuple_Pack(2,PyAgrumHelper::PySeqFromSetOfInstantiation(argmi),PyFloat_FromDouble(mi));
    }

    PyObject* argmax() {
      const auto [argma,ma] = self->argmax();
      return PyTuple_Pack(2,PyAgrumHelper::PySeqFromSetOfInstantiation(argma),PyFloat_FromDouble(ma));
    }

    // equality
    bool __eq__(const gum::Potential<double>& b) {
      return *self==b;
    }

    // non equality
    bool __ne__(const gum::Potential<double>& b) {
      return *self!=b;
    }


  %pythoncode {
    def __radd__(self,other):
      return self.__add__(other)

    def __rmul__(self,other):
      return self.__mul__(other)

    def __rsub__(self,other):
      return (self*-1)+other

    def __rfloordiv__(self,other):
      return Potential(self).inverse().scale(other)

    def __rtruediv__(self,other):
      return Potential(self).inverse().scale(other)

    def __rdiv__(self,other):
      return Potential(self).inverse().scale(other)

    def __neg__(self):
      return -1*self

    def __abs__(self):
      return Potential(self).abs()

    def loopIn(self):
      """
      Generator to iterate inside a Potential.

      Yield an pyAgrum.Instantiation that iterates over all the possible values for the pyAgrum.Potential

      Examples
      --------
      >>> import pyAgrum as gum
      >>> bn=gum.fastBN("A[3]->B[3]<-C[3]")
      >>> for i in bn.cpt("B").loopIn():
            print(i)
            print(bn.cpt("B").get(i))
            bn.cpt("B").set(i,0.3)
      """
      i=Instantiation(self)
      i.setFirst()
      while not i.end():
        yield i
        i.inc()
      return

    def fillWithFunction(self,s,noise=None):
      """
      Automatically fills the potential as a (quasi) deterministic CPT with the evaluation of the expression s.

      The expression s gives a value for the first variable using the names of the last variables.
      The computed CPT is deterministic unless noise is used to add a 'probabilistic' noise around the exact value given by the expression.


      Examples
      --------
      >>> import pyAgrum as gum
      >>> bn=gum.fastBN("A[3]->B[3]<-C[3]")
      >>> bn.cpt("B").fillWithFunction("(A+C)/2")

      Parameters
      ----------
      s : str
          an expression using the name of the last variables of the Potential and giving a value to the first variable of the Potential
      noise : list
          an (odd) list of numerics giving a pattern of 'probabilistic noise' around the value.

      Warning
      -------
          The expression may have any numerical values, but will be then transformed to the closest correct value for the range of the variable.

      Returns
      -------
      pyAgrum.Potential
            a reference to the modified potential

      Raises
      ------
        pyAgrum.InvalidArgument
        If the first variable is Labelized or Integer, or if the len of the noise is not odd.
      """
      if self.variable(0).varType()==VarType_Labelized:
        raise InvalidArgument("[pyAgrum] The variable "+self.variable(0).name()+" is a LabelizedVariable")
      if self.variable(0).varType()==VarType_Integer:
        raise InvalidArgument("[pyAgrum] The variable "+self.variable(0).name()+" is neither Range nor Discretized variable.")

      if noise==None:
        mid=0
      else:
        if len(noise)%2==0:
          raise InvalidArgument("[pyAgrum] len(noise) must not be even")
        mid=int((len(noise)-1)/2)

      self.fillWith(0)
      mi=self.variable(0).numerical(0)
      ma=self.variable(0).numerical(self.variable(0).domainSize()-1)

      I=Instantiation(self)

      I.setFirst()
      while not I.end():
        vars={self.variable(i).name():self.variable(i).numerical(I.val(i)) for i in range(1,self.nbrDim())}
        res=eval(s,None,vars)
        if res<mi:
          res=mi
        if res>ma:
          res=ma
        pos=self.variable(0).index(str(res))
        if mid==0:
          I.chgVal(0,pos)
          self.set(I,1)
        else:
          for i,v in enumerate(noise):
            if 0<=pos+i-mid<self.variable(0).domainSize():
              I.chgVal(0,pos+i-mid)
              self.set(I,v)
        I.incNotVar(self.variable(0))
      self.normalizeAsCPT()
      return self

    def variablesSequence(self):
        """
        Returns
        -------
        list
            a list containing the sequence of variables
        """
        varlist = []
        for i in range(0, self.nbrDim()):
            varlist.append(self.variable(i))
        return varlist

    def __prepareIndices__(self,ind):
      """
      From an indice (dict or tuple), returns a pair of pyAgrum.Instantiation to loop in a part of the Potential.
      """
      loopvars=Instantiation(self)
      loopvars.setMutable()

      inst=Instantiation(self)
      inst.setFirst()

      if isinstance(ind, (Number,slice)):
        i = tuple([ind])
      else:
        i = ind

      if isinstance(i,dict):
          for nam in self.names:
              if nam in i:
                  inst.chgVal(nam,i[nam])
                  loopvars.erase(nam)
      elif isinstance(i,tuple):
          vn=[n for n in reversed(self.names)]
          if len(i)>self.nbrDim():
              raise KeyError("Too many values in '"+str(i)+"' for '"+str(self)+"'")
          for k,v in enumerate(i):
              if not isinstance(v,slice):
                  nam=vn[k]
                  inst.chgVal(nam,v)
                  loopvars.erase(nam)
      else:
          raise ValueError("No subscript using '"+str(i)+"'")
      return inst,loopvars

    def __getitem__(self, id):
      if isinstance(id,Instantiation):
          return self.get(id)

      inst,loopvars=self.__prepareIndices__(id)

      if loopvars.nbrDim()==0:
          return self.get(inst)

      if loopvars.nbrDim()==self.nbrDim():
        content=[]

        inst=Instantiation(self)
        while not inst.end():
            content.append(self.get(inst))
            inst.inc()
        tab=numpy.array(content,dtype=numpy.float64)
        tab.shape=tuple(reversed(self.shape))
        return tab

      names=[loopvars.variable(i-1).name() for i in range(loopvars.nbrDim(),0,-1)]
      tab=numpy.zeros(tuple([loopvars.variable(i-1).domainSize() for i in range(loopvars.nbrDim(),0,-1)]))
      while not inst.end():
          indice=[inst.val(name) for name in names]
          tab[tuple(indice)]=self.get(inst)
          inst.incIn(loopvars)
      return tab

    def __setitem__(self, id, value):
      if isinstance(id,Instantiation):
          self.set(id,value)
          return

      inst,loopvars=self.__prepareIndices__(id)

      if loopvars.nbrDim()==0:
          self.set(inst,value)
          return

      if isinstance(value,Number):
        while not inst.end():
            self.set(inst,value)
            inst.incIn(loopvars)
      else:
        if isinstance(value,list):
            value=numpy.array(value)

        shape=tuple([loopvars.variable(i-1).domainSize() for i in range(loopvars.nbrDim(),0,-1)])
        if value.shape!=shape:
          raise IndexError("Shape of '"+str(value)+"' is not '"+str(shape)+"'")

        names = [loopvars.variable(i - 1).name() for i in range(loopvars.nbrDim(), 0, -1)]
        while not inst.end():
            indice = tuple([inst.val(name) for name in names])
            self.set(inst,float(value[indice]))
            inst.incIn(loopvars)

    def tolist(self):
        """
        Returns
        -------
        list
            the potential as a list
        """
        return self.__getitem__({}).tolist()

    def toarray(self):
        """
        Returns
        -------
        array
            the potential as an array
        """
        return self.__getitem__({})

    def topandas(self):
        """
        Returns
        -------
        pandas.DataFrame
           the potential as an pandas.DataFrame
        """
        import pandas as pd
        varnames = list(reversed(self.names))
        data = []
        pname = ""
        for inst in self.loopIn():
            d = {k:v for k,v in reversed(inst.todict(True).items())}
            d[pname] = self.get(inst)
            d[pname], d[varnames[-1]] = d[varnames[-1]], d[pname]
            data.append(d)
        cols = varnames[:-1] + [pname]
        return pd.DataFrame(data).set_index(cols).unstack(pname)

    def tolatex(self):
        """
        Render object to a LaTeX tabular.

        Requires to include `booktabs` package in the LaTeX document.

        Returns
        -------
        str
         the potential as LaTeX string
        """
        return self.topandas().to_latex()

    def toclipboard(self,**kwargs):
        """
        Write a text representation of object to the system clipboard. This can be pasted into spreadsheet, for instance.
        """
        return self.topandas().to_clipboard()

    @property
    def var_names(self):
        """
        Returns
        -------
        list
            a list containing the name of each variables in the potential

        Warnings
        --------
            This methods is deprecated. Please use gum.Potential.names and note the change in the order !

            var_names return a list in the reverse order of the enumeration order of the variables.
        """
        warnings.warn("\n** pyAgrum.Potential.var_names is obsolete in pyAgrum>0.22.9. Please use pyAgrum.Potential.names.\n")
        return [n for n in reversed(self.names)]

    @property
    def var_dims(self):
        """
        Returns
        -------
        list
            a list containing the dimensions of each variables in the potential

        Warnings
        --------
            This methods is deprecated. Please use gum.Potential.shape and note the change in the order !

            var_dims return a list in the reverse order of the enumeration order of the variables.
        """
        warnings.warn("\n** pyAgrum.Potential.var_dims is obsolete in pyAgrum>0.22.9. Please use pyAgrum.Potential.shape.\n")
        return [n for n in reversed(self.shape)]

    @property
    def names(self):
        """
        Returns
        -------
        list
            a list containing the name of each variables in the potential

        Warnings
        --------
            listed in the reverse order of the enumeration order of the variables.
        """
        return tuple([self.variable(i).name() for i in range(self.nbrDim())])

    @property
    def shape(self):
        """
        Returns
        -------
        list
            a list containing the dimensions of each variables in the potential

        Warnings
        --------
            `p.shape` and `p[:].shape` list the dimensions in different order
        """
        return tuple([self.variable(i).domainSize() for i in range(self.nbrDim())])
  }
}
