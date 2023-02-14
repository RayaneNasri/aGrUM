# (c) Copyright by Pierre-Henri Wuillemin, 2022
# (pierre-henri.wuillemin@lip6.fr)

# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and
# without fee or royalty is hereby granted, provided
# that the above copyright notice appear in all copies
# and that both that copyright notice and this permission
# notice appear in supporting documentation or portions
# thereof, including modifications, that you make.

# THE AUTHOR P.H. WUILLEMIN  DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT
# OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
# RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
# IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
# ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
# OR PERFORMANCE OF THIS SOFTWARE!
import unittest

import numpy
import pyAgrum as gum
from .pyAgrumTestSuite import pyAgrumTestCase, addTests


class MarkovNetTestCase(pyAgrumTestCase):
  def _fill(self, mn):
    for i in [11, 21, 31, 41]:
      mn.add(str(i), 3)
    mn.add(str(51), 7)
    mn.addFactor({"11", "31"}).fillWith(
        [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8])
    mn.addFactor({"11", "41"}).fillWith(
        [0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0])
    mn.addFactor({"31", "51"}).fillWith(0.97)
    mn.addFactor({"21", "41", "51"}).fillWith(0.03)

  def testConstructor(self):
    mn = gum.MarkovNet()
    self._fill(mn)

    self.assertEqual(mn.size(), 5)
    self.assertEqual(mn.sizeEdges(), 6)
    self.assertEqual(mn.dim(), (3 * 3 + 3 * 3 + 3 * 7 + 3 * 3 * 7))
    self.assertEqual(mn.__str__(),
                      "MRF{nodes: 5, edges: 6, domainSize: 567, dim: 102}")
    self.assertEqual(mn.neighbours("41"), {0, 1, 4})
    self.assertEqual(mn.neighbours(3), {0, 1, 4})

    self.assertEqual(mn.variable(1).name(), "21")
    self.assertEqual(mn.nodeId(mn.variable(2)), 2)
    self.assertEqual(mn.idFromName("31"), 2)
    self.assertEqual(mn.variableFromName("41").name(), "41")

    self.assertEqual(mn.maxVarDomainSize(), 7)
    self.assertEqual(mn.minParam(), 0.0)
    self.assertEqual(mn.minNonZeroParam(), 0.03)
    self.assertEqual(mn.maxParam(), 1.0)
    self.assertEqual(mn.maxNonOneParam(), 0.97)

  def testCopyConstructor(self):
    mn = gum.MarkovNet()
    self._fill(mn)
    mn2 = gum.MarkovNet(mn)
    self.assertEqual(
        mn2.__str__(), "MRF{nodes: 5, edges: 6, domainSize: 567, dim: 102}")

    for n in mn.nodes():
      self.assertEqual(mn.variable(n).name(), mn2.variable(n).name())
      self.assertEqual(mn.neighbours(n), mn2.neighbours(n))

  def testCopyOperator(self):
    mn = gum.MarkovNet()
    mn2 = gum.MarkovNet()
    self._fill(mn)
    mn2 = mn
    self.assertEqual(mn2.__str__(),
                      "MRF{nodes: 5, edges: 6, domainSize: 567, dim: 102}")
    for n in mn.nodes():
      self.assertEqual(mn.variable(n).name(), mn2.variable(n).name())
      self.assertEqual(mn.neighbours(n), mn2.neighbours(n))

  def testEqualityOperators(self):
    mn1 = gum.MarkovNet()
    self._fill(mn1)
    mn2 = gum.MarkovNet()
    self.assertNotEqual(mn1, mn2)
    self._fill(mn2)
    self.assertEqual(mn1, mn2)
    mn2.generateFactors()
    self.assertNotEqual(mn1, mn2)

  def testInsertion(self):
    mn = gum.MarkovNet()
    self._fill(mn)

    with self.assertRaises(gum.InvalidArgument):
      mn.addFactor(gum.Potential())  # no empty factor

    with self.assertRaises(gum.InvalidArgument):
      mn.addFactor({"11", "31"})  # already exist

    mn1 = gum.MarkovNet()
    self._fill(mn1)
    pot = gum.Potential().add(mn1.variable("11")).add(mn1.variable("21"))
    pot.randomDistribution()
    mn1.addFactor(pot)
    self.assertEqual(pot.__str__(), mn1.factor({"11", "21"}).__str__())

    mn1 = gum.MarkovNet()
    self._fill(mn1)
    pot = gum.Potential().add(mn1.variable("21")).add(mn1.variable("11"))
    pot.randomDistribution()
    mn1.addFactor(pot)

    # should be equal : does not depend of the order of vars in the MarkonNet
    self.assertEqual(pot.__str__(), mn1.factor({"11", "21"}).__str__())

    # but the data should be the same
    I = gum.Instantiation(pot)
    factor = mn1.factor({"21", "11"})
    I.setFirst()
    while not I.end():
      self.assertAlmostEqual(pot.get(I), factor.get(I), places=7)
      I.inc()

  def testIterations(self):
    mn = gum.MarkovNet()
    self._fill(mn)

    cpt = 0
    for node in mn.nodes():
      cpt += 1
    self.assertEqual(cpt, mn.size())

    cpt = 0
    for edg in mn.edges():
      cpt += 1
    self.assertEqual(cpt, mn.sizeEdges())

  def testEraseFactor(self):
    mn = gum.MarkovNet()
    self._fill(mn)
    with self.assertRaises(gum.InvalidArgument):
      mn.eraseFactor({12, 14})
    mn.eraseFactor({2, 4})

    self.assertEqual(mn.size(), 5)
    self.assertEqual(mn.sizeEdges(), 5)
    self.assertEqual(mn.dim(), (3 * 3 + 3 * 3 + 3 * 3 * 7))
    self.assertEqual(mn.__str__(),
                      "MRF{nodes: 5, edges: 5, domainSize: 567, dim: 81}")

  def testEraseFactorWithNames(self):
    mn = gum.MarkovNet()
    self._fill(mn)
    with self.assertRaises(gum.InvalidArgument):
      mn.eraseFactor({"31", "21"})
    mn.eraseFactor({"31", "51"})

    self.assertEqual(mn.size(), 5)
    self.assertEqual(mn.sizeEdges(), 5)
    self.assertEqual(mn.dim(), (3 * 3 + 3 * 3 + 3 * 3 * 7))
    self.assertEqual(mn.__str__(),
                      "MRF{nodes: 5, edges: 5, domainSize: 567, dim: 81}")

  def testErase(self):
    mn = gum.MarkovNet()
    self._fill(mn)
    with self.assertRaises(gum.InvalidArgument):
      mn.erase(36)
    mn.erase(3)
    self.assertEqual(mn.size(), 4)
    self.assertEqual(mn.sizeEdges(), 3)
    self.assertEqual(mn.dim(), (3 * 3 + 3 * 7 + 3 * 7))
    self.assertEqual(mn.__str__(),
                      "MRF{nodes: 4, edges: 3, domainSize: 189, dim: 51}")

  def testEraseWithNames(self):
    mn = gum.MarkovNet()
    self._fill(mn)
    with self.assertRaises(gum.NotFound):
      mn.erase("36")
    mn.erase("41")
    self.assertEqual(mn.size(), 4)
    self.assertEqual(mn.sizeEdges(), 3)
    self.assertEqual(mn.dim(), (3 * 3 + 3 * 7 + 3 * 7))
    self.assertEqual(mn.__str__(),
                      "MRF{nodes: 4, edges: 3, domainSize: 189, dim: 51}")

  def testToDot(self):
    mn = gum.fastMN("A--B--C;B--D;C--E;D--E--F")
    s1 = mn.toDot()
    s2 = mn.toDotAsFactorGraph()

  def testFromBN(self):
    bn = gum.fastBN("A->B->C<-D;C<-E->F<-G;F<-A")
    mn = gum.MarkovNet.fromBN(bn)

    pbn = gum.Potential()
    pbn.fillWith(1)
    for nod in bn.nodes():
      self.assertEqual(bn.variable(nod).__str__(), mn.variable(nod).__str__())
      pbn *= bn.cpt(nod)

    pmn = gum.Potential()
    pmn.fillWith(1)
    for f in mn.factors():
      pmn *= mn.factor(f)
    pmn.normalize()

    ppmn = gum.Potential(pbn)
    ppmn.fillWith(pmn)  # copy of pmn using pbn's variables
    diff = (pbn - ppmn).abs()
    self.assertEqual(pbn.domainSize(), diff.domainSize())
    self.assertLessEqual(diff.max(), 1e-10)
    self.assertEqual(mn.graph(), bn.moralGraph())

  def testExistsEdge(self):
    mn = gum.fastMN("A--B--C;C--D;E--F--G")

    self.assertTrue(mn.existsEdge(0, 1))
    self.assertTrue(mn.existsEdge("A", "B"))
    self.assertTrue(mn.existsEdge(1, 0))
    self.assertTrue(mn.existsEdge("B", "A"))
    self.assertTrue(mn.existsEdge(0, 2))
    self.assertTrue(mn.existsEdge("A", "C"))
    self.assertFalse(mn.existsEdge(3, 7))
    self.assertFalse(mn.existsEdge("C", "G"))

  def testMinimalCondSet(self):
    mn = gum.fastMN("A--B--C;C--D;E--F--G;B--E")

    self.assertEqual(mn.minimalCondSet(0, {1, 2, 3, 4, 5, 6}), {1, 2})
    self.assertEqual(mn.minimalCondSet({0, 6}, {1, 2, 3, 4, 5}), {1, 2, 4, 5})
    self.assertEqual(mn.minimalCondSet(3, {0, 4, 5, 6}), {0, 4})

    self.assertEqual(mn.minimalCondSet(
        "A", {"B", "C", "D", "E", "F", "G"}), {1, 2})
    self.assertEqual(mn.minimalCondSet(
        {"A", "G"}, {"B", "C", "D", "E", "F"}), {1, 2, 4, 5})
    self.assertEqual(mn.minimalCondSet("D", {"A", "E", "F", "G"}), {0, 4})



  def testIndependence(self) :
    mn = gum.fastMN("A--B--C;C--D;E--F--G;B--E;D--G;X")

    self.assertTrue(mn.isIndependent("D", "X", {}))
    self.assertFalse(mn.isIndependent("D", "A", {"C"}))
    self.assertTrue(mn.isIndependent("D", "A", {"C","G"}))
    self.assertFalse(mn.isIndependent("G", "A", {"C","F"}))
    self.assertTrue(mn.isIndependent("G", "A", {"D","E"}))



  def testOrderInsertion(self):
    mn=gum.fastMN("V0;V1;V2;V3")

    # V0 should be the first
    mn.addFactor(["V0", "V1"])
    self.assertEquals(mn.factor({"V0", "V1"}).variable(0).name(), "V0")
    self.assertEquals(mn.factor({"V1", "V0"}).variable(0).name(), "V0")

    # V2 should be the first
    mn.addFactor(["V2", "V1"])
    self.assertEquals(mn.factor({"V2", "V1"}).variable(0).name(), "V2")
    self.assertEquals(mn.factor({"V1", "V2"}).variable(0).name(), "V2")

    # 2 should be the first
    mn.addFactor({"V2", "V3"})
    self.assertEquals(mn.factor({"V3", "V2"}).variable(0).name(), "V2")

    # 1 should be the first
    mn.addFactor({"V2","V0"})
    self.assertEquals(mn.factor({"V2","V0"}).variable(0).name(), "V0")

  def testOrderInsertionWithId(self):
    mn=gum.fastMN("V0;V1;V2;V3")

    # V0 should be the first
    mn.addFactor([0,1])
    self.assertEquals(mn.factor({0,1}).variable(0).name(), "V0")
    self.assertEquals(mn.factor({1,0}).variable(0).name(), "V0")

    # V2 should be the first
    mn.addFactor([2,1])
    self.assertEquals(mn.factor({2,1}).variable(0).name(), "V2")
    self.assertEquals(mn.factor({1,2}).variable(0).name(), "V2")

    # 2 should be the first
    mn.addFactor({2,3})
    self.assertEquals(mn.factor({2,3}).variable(0).name(), mn.variable(2).name())

    # 1 should be the first
    mn.addFactor({2,0})
    self.assertEquals(mn.factor({2,0}).variable(0).name(), mn.variable(0).name())

ts = unittest.TestSuite()
addTests(ts, MarkovNetTestCase)
