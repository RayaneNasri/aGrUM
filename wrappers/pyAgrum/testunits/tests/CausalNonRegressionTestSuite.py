# (c) Copyright by Pierre-Henri Wuillemin, UPMC, 2017
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

import pyAgrum as gum
from pyAgrumTestSuite import pyAgrumTestCase, addTests
import pyAgrum.causal as csl


class TestSimpson(pyAgrumTestCase):
  def setUp(self):
    m1 = gum.fastBN("Gender->Drug->Patient;Gender->Patient")

    m1.cpt("Gender")[:] = [0.5, 0.5]
    m1.cpt("Drug")[:] = [[0.25, 0.75],  # Gender=0
                         [0.75, 0.25]]  # Gender=1

    m1.cpt("Patient")[{'Drug': 0, 'Gender': 0}] = [
        0.2, 0.8]  # No Drug, Male -> healed in 0.8 of cases
    # No Drug, Female -> healed in 0.4 of cases
    m1.cpt("Patient")[{'Drug': 0, 'Gender': 1}] = [0.6, 0.4]
    m1.cpt("Patient")[{'Drug': 1, 'Gender': 0}] = [
        0.3, 0.7]  # Drug, Male -> healed 0.7 of cases
    m1.cpt("Patient")[{'Drug': 1, 'Gender': 1}] = [
        0.8, 0.2]  # Drug, Female -> healed in 0.2 of cases
    self.model = csl.CausalModel(m1)

  def test_CausalImpactWithoutObservation(self):
    evs = {}
    latex, impact, explain = csl.causalImpact(
        self.model, "Patient", doing="Drug", knowing=evs)

  def test_CausalImpactWithObservation(self):
    evs = {'Gender'}
    latex, impact, explain = csl.causalImpact(
        self.model, "Patient", doing="Drug", knowing=evs)


class TestFromUsers(pyAgrumTestCase):
  def test_AccentsInVariables(self):
    bn = gum.fastBN("héhé->hoho")
    cm = csl.CausalModel(bn)
    formula,impact, explanation = csl.causalImpact(cm,"héhé","hoho")

class TestFromR(pyAgrumTestCase):
  def setUp(self):
    m = gum.fastBN("z2->x->z1->y;z2->z1;z2->z3->y")

    m.cpt("z2")[:] = [0.5, 0.5]
    m.cpt("x")[:] = [[0.4, 0.6],  # z2=0
                     [0.4, 0.6]]  # z2=1
    m.cpt("z3")[:] = [[0.3, 0.7],  # z2=0
                      [0.3, 0.7]]  # z2=1
    m.cpt("z1")[{"z2": 0, "x": 0}] = [0.2, 0.8]
    m.cpt("z1")[{"z2": 0, "x": 1}] = [0.25, 0.75]
    m.cpt("z1")[{"z2": 1, "x": 0}] = [0.1, 0.9]
    m.cpt("z1")[{"z2": 1, "x": 1}] = [0.15, 0.85]

    m.cpt("y")[{"z1": 0, "z3": 0}] = [0.5, 0.5]
    m.cpt("y")[{"z1": 0, "z3": 1}] = [0.45, 0.55]
    m.cpt("y")[{"z1": 1, "z3": 0}] = [0.4, 0.6]
    m.cpt("y")[{"z1": 1, "z3": 1}] = [0.35, 0.65]

    self.d = csl.CausalModel(m, [("X-Z2", ["x", "z2"]),
                                 ("X-Z3", ["x", "z3"]),
                                 ("X-Y", ["x", "y"]),
                                 ("Y-Z2", ["y", "z2"])],
                             True)
    try:
      formula, result, msg = csl.causalImpact(
          self.d, on={"y", "z2", "z1", "z3"}, doing={"x"})
    except csl.HedgeException as h:
      self.fail("Should not raise")


class TestsFromGumWhy(pyAgrumTestCase):
  def test_Carouselp115(self):
    ab = gum.fastBN('Elapsed time[11]->Bag on Carousel<-Bag on Plane')
    ab.cpt("Bag on Plane").fillWith(1).normalize()
    ab.cpt("Elapsed time").fillWith(1).normalize()
    ab.cpt("Bag on Carousel").fillWith(
        [1.0, 0.0] * 11 + [1 - i / 20 if i % 2 == 0 else (i - 1) / 20 for i in range(22)])
    abModele = csl.CausalModel(ab)
    formula, impact, explanation = csl.causalImpact(abModele, on={"Bag on Plane"}, doing={
                                                    "Elapsed time"}, knowing={"Bag on Carousel"}, values={"Elapsed time": 7, "Bag on Carousel": 0})
    self.assertAlmostEqual(impact[0], 0.7692, 4)

  def test_DoCalculusp213(self):
    fd = gum.fastBN("w->z->x->y")
    fdModele = csl.CausalModel(
        fd, [("u1", ["w", "x"]), ("u2", ["w", "y"])], True)
    formula, impact, explanation = csl.causalImpact(
        fdModele, on="y", doing="x")


ts = unittest.TestSuite()
addTests(ts, TestSimpson)
addTests(ts, TestFromUsers)
addTests(ts, TestFromR)
addTests(ts, TestsFromGumWhy)
