/***************************************************************************
 *   Copyright (c) 2005-2024  by Pierre-Henri WUILLEMIN et Christophe GONZALES   *
 *   {prenom.nom}_at_lip6.fr                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/**
 * @file
 * @brief This file contains definition for a "loading action" listener in python
 *
 * @author Pierre-Henri WUILLEMIN
 */

#include <Python.h>

#include <agrum/base/core/signal/listener.h>


class PythonLoadListener: public gum::Listener {
  private:
  PyObject* _whenLoading_;

  public:
  void whenLoading(const void* buffer, int percent) {
    if (_whenLoading_) {
      PyObject* arglist = Py_BuildValue("(i)", percent);
      PyObject_Call(_whenLoading_, arglist, NULL);
      Py_DECREF(arglist);
    }
  }

  bool setPythonListener(PyObject* l) {
    if (!PyCallable_Check(l)) {
      return false;
    } else {
      _whenLoading_ = l;
      Py_INCREF(l);
      return true;
    }
  }

  PythonLoadListener() { _whenLoading_ = (PyObject*)0; }
  ~PythonLoadListener() {
    if (_whenLoading_) Py_DECREF(_whenLoading_);
  }
};


int _fillLoadListeners_(std::vector< PythonLoadListener >& py_listener, PyObject* l) {
  if (!l) return 0;

  if (l == Py_None) return 0;

  int       l_size = 0;
  PyObject* item;

  if (PySequence_Check(l)) {
    l_size = (int)PySequence_Size(l);
    py_listener.resize(l_size);

    for (int i = 0; i < l_size; i++) {
      item = PySequence_GetItem(l, i);

      if (!py_listener[i].setPythonListener(item)) return 0;
    }
  } else {
    l_size = 1;
    py_listener.resize(l_size);

    if (!py_listener[0].setPythonListener(l)) return 0;
  }

  return l_size;
}
