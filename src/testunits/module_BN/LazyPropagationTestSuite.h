/**
 *
 *   Copyright (c) 2005-2021 by Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
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


#include <iostream>
#include <string>

#include <gumtest/AgrumTestSuite.h>
#include <gumtest/testsuite_utils.h>

#include <agrum/tools/core/math/math_utils.h>
#include <agrum/BN/BayesNet.h>
#include <agrum/BN/io/BIF/BIFReader.h>

#include <agrum/BN/inference/ShaferShenoyInference.h>
#include <agrum/BN/inference/lazyPropagation.h>
#include <agrum/BN/inference/tools/relevantPotentialsFinderType.h>
#include <agrum/BN/inference/variableElimination.h>
#include <agrum/tools/multidim/implementations/multiDimArray.h>
#include <agrum/tools/variables/labelizedVariable.h>

// The graph used for the tests:
//          1   2_          1 -> 3
//         / \ / /          1 -> 4
//        3   4 /           3 -> 5
//         \ / /            4 -> 5
//          5_/             2 -> 4
//                          2 -> 5

namespace gum_tests {

  class LazyPropagationTestSuite: public CxxTest::TestSuite {
    public:
    gum::BayesNet< double >* bn;
    gum::NodeId              i1, i2, i3, i4, i5;
    gum::Potential< double >*e_i1, *e_i4;

    float __epsilon{1e-6f};

    void setUp() {
      bn = new gum::BayesNet< double >();

      gum::LabelizedVariable n1("1", "", 2), n2("2", "", 2), n3("3", "", 2);
      gum::LabelizedVariable n4("4", "", 2), n5("5", "", 3);

      i1 = bn->add(n1);
      i2 = bn->add(n2);
      i3 = bn->add(n3);
      i4 = bn->add(n4);
      i5 = bn->add(n5);

      bn->addArc(i1, i3);
      bn->addArc(i1, i4);
      bn->addArc(i3, i5);
      bn->addArc(i4, i5);
      bn->addArc(i2, i4);
      bn->addArc(i2, i5);

      e_i1 = new gum::Potential< double >();
      (*e_i1) << bn->variable(i1);
      e_i1->fill(0.0f);
      gum::Instantiation inst_1(*e_i1);
      inst_1.chgVal(bn->variable(i1), 0);
      e_i1->set(inst_1, 1.0f);

      e_i4 = new gum::Potential< double >();
      (*e_i4) << bn->variable(i4);
      e_i4->fill(0.0f);
      gum::Instantiation inst_4(*e_i4);
      inst_4.chgVal(bn->variable(i4), 1);
      e_i4->set(inst_4, 1.0f);
    }

    void tearDown() {
      delete bn;
      delete e_i1;
      delete e_i4;
    }

    bool equalPotentials(const gum::Potential< double >& p1, const gum::Potential< double >& p2) {
      gum::Instantiation i1(p1);
      gum::Instantiation i2(p2);

      for (i1.setFirst(), i2.setFirst(); !i1.end(); i1.inc(), i2.inc()) {
        if ((p1[i1] == 0) && (std::fabs(p2[i2]) > __epsilon)) return false;
        if (p1[i1] > p2[i2]) {
          if (std::fabs((p1[i1] - p2[i2]) / p1[i1]) > __epsilon) return false;
        } else {
          if (std::fabs((p1[i1] - p2[i2]) / p1[i2]) > __epsilon) return false;
        }
      }

      return true;
    }

    // Testing when there is no evidence
    void testCreationAndInference() {
      fill(*bn);
      // Testing the inference
      gum::LazyPropagation< double >* inf = nullptr;
      TS_ASSERT_THROWS_NOTHING(inf = new gum::LazyPropagation< double >(bn))
      TS_ASSERT_THROWS_NOTHING(inf->makeInference())

      if (inf != nullptr) { TS_ASSERT_THROWS_NOTHING(delete inf); }
    }

    // Testing when there is no evidence
    void testSequentialInference() {
      fill(*bn);
      // Testing the inference
      gum::LazyPropagation< double > inf(bn);
      inf.setNumberOfThreads(1);
      TS_ASSERT(inf.getNumberOfThreads() == 1)
      TS_ASSERT_THROWS_NOTHING(inf.makeInference())
    }

    void testBigInference() {
      std::string              file = GET_RESSOURCES_PATH("bif/Munin1.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);
      gum::Size                nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)

      gum::LazyPropagation< double > inf(&bn);

      // const bool seq = true;
      const bool seq = false;
      if (seq) {
        inf.setNumberOfThreads(1);
      } else {
        inf.setNumberOfThreads(4);
      }

      inf.makeInference();
    }

    void testMarginal1() {
      fill(*bn);
      gum::LazyPropagation< double >       inf(bn);
      gum::ShaferShenoyInference< double > inf2(bn);

      inf.setNumberOfThreads(1);

      TS_ASSERT_THROWS_NOTHING(inf.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i1))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i2))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i3))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i4))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i5))

      inf.makeInference();


      inf2.makeInference();

      TS_ASSERT(equalPotentials(inf.posterior(i1), inf2.posterior(i1)))
      TS_ASSERT(equalPotentials(inf.posterior(i2), inf2.posterior(i2)))
      TS_ASSERT(equalPotentials(inf.posterior(i3), inf2.posterior(i3)))
      TS_ASSERT(equalPotentials(inf.posterior(i4), inf2.posterior(i4)))
      TS_ASSERT(equalPotentials(inf.posterior(i5), inf2.posterior(i5)))
    }

    void testMarginal() {
      fill(*bn);
      gum::LazyPropagation< double >       inf(bn);
      gum::ShaferShenoyInference< double > inf2(bn);

      TS_ASSERT_THROWS_NOTHING(inf.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i1))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i2))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i3))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i4))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i5))

      inf.makeInference();
      inf2.makeInference();

      TS_ASSERT(equalPotentials(inf.posterior(i1), inf2.posterior(i1)))
      TS_ASSERT(equalPotentials(inf.posterior(i2), inf2.posterior(i2)))
      TS_ASSERT(equalPotentials(inf.posterior(i3), inf2.posterior(i3)))
      TS_ASSERT(equalPotentials(inf.posterior(i4), inf2.posterior(i4)))
      TS_ASSERT(equalPotentials(inf.posterior(i5), inf2.posterior(i5)))
    }

    void testMarginalWithEvidence() {
      fill(*bn);
      gum::List< const gum::Potential< double >* > e_list;
      e_list.insert(e_i1);
      e_list.insert(e_i4);

      gum::LazyPropagation< double >       inf(bn);
      gum::ShaferShenoyInference< double > infX(bn);

      for (auto pot: e_list) {
        TS_ASSERT_THROWS_NOTHING(inf.addEvidence(*pot))
        TS_ASSERT_THROWS_NOTHING(infX.addEvidence(*pot))
      }

      TS_ASSERT_THROWS_NOTHING(inf.makeInference())
      TS_ASSERT_THROWS_NOTHING(infX.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i1))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i2))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i3))

      TS_ASSERT_THROWS_NOTHING(inf.posterior(i4))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(i5))

      TS_ASSERT(equalPotentials(inf.posterior(i1), infX.posterior(i1)))
      TS_ASSERT(equalPotentials(inf.posterior(i2), infX.posterior(i2)))
      TS_ASSERT(equalPotentials(inf.posterior(i3), infX.posterior(i3)))
      TS_ASSERT(equalPotentials(inf.posterior(i4), infX.posterior(i4)))
      TS_ASSERT(equalPotentials(inf.posterior(i5), infX.posterior(i5)))

      gum::LazyPropagation< double >       inf2(bn);
      gum::ShaferShenoyInference< double > inf2X(bn);

      TS_ASSERT_THROWS_NOTHING(inf2.addEvidence(i1, 0))
      TS_ASSERT_THROWS_NOTHING(inf2.addEvidence(i4, 1))
      TS_ASSERT_THROWS_NOTHING(inf2X.addEvidence(i1, 0))
      TS_ASSERT_THROWS_NOTHING(inf2X.addEvidence(i4, 1))

      TS_ASSERT_THROWS_NOTHING(inf2.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf2X.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf2.posterior(i1))
      TS_ASSERT_THROWS_NOTHING(inf2.posterior(i2))
      TS_ASSERT_THROWS_NOTHING(inf2.posterior(i3))
      TS_ASSERT_THROWS_NOTHING(inf2.posterior(i4))
      TS_ASSERT_THROWS_NOTHING(inf2.posterior(i5))

      TS_ASSERT_EQUALS(inf.posterior(i1), inf2.posterior(i1))
      TS_ASSERT_EQUALS(inf.posterior(i2), inf2.posterior(i2))
      TS_ASSERT_EQUALS(inf.posterior(i3), inf2.posterior(i3))
      TS_ASSERT_EQUALS(inf.posterior(i4), inf2.posterior(i4))
      TS_ASSERT_EQUALS(inf.posterior(i5), inf2.posterior(i5))

      TS_ASSERT(equalPotentials(inf2.posterior(i1), inf2X.posterior(i1)))
      TS_ASSERT(equalPotentials(inf2.posterior(i2), inf2X.posterior(i2)))
      TS_ASSERT(equalPotentials(inf2.posterior(i3), inf2X.posterior(i3)))
      TS_ASSERT(equalPotentials(inf2.posterior(i4), inf2X.posterior(i4)))
      TS_ASSERT(equalPotentials(inf2.posterior(i5), inf2X.posterior(i5)))
    }

    // Testing when there is no evidence
    void testJoint() {
      fill(*bn);
      // Testing the inference
      gum::LazyPropagation< double > inf(bn);
      gum::NodeSet                   nodeset;
      nodeset.insert(2);
      nodeset.insert(4);
      inf.addJointTarget(nodeset);

      TS_ASSERT_THROWS_NOTHING(inf.jointPosterior(nodeset))
    }

    // Testing when there is no evidence
    void testJoint2() {
      fill(*bn);
      // Testing the inference
      gum::LazyPropagation< double > inf(bn);
      gum::NodeSet                   nodeset;
      nodeset.insert(1);
      nodeset.insert(2);
      nodeset.insert(3);
      nodeset.insert(4);
      inf.eraseAllTargets();
      inf.addJointTarget(nodeset);

      gum::NodeSet nodeset2;
      nodeset2.insert(2);
      nodeset2.insert(4);

      TS_ASSERT_THROWS_NOTHING(inf.jointPosterior(nodeset2))
      TS_ASSERT_THROWS_NOTHING(inf.posterior(3))
    }

    // Testing information methods
    void testInformationMethods() {
      fill(*bn);

      gum::LazyPropagation< double > inf(bn);
      gum::NodeSet                   nodeset;
      nodeset.insert(0);
      nodeset.insert(4);
      inf.addJointTarget(nodeset);
      nodeset.clear();
      nodeset.insert(2);
      nodeset.insert(4);
      inf.addJointTarget(nodeset);
      inf.makeInference();

      TS_GUM_ASSERT_THROWS_NOTHING(inf.H((gum::NodeId)2));
      TS_GUM_ASSERT_THROWS_NOTHING(inf.I((gum::NodeId)2, (gum::NodeId)4));
      TS_ASSERT_THROWS(inf.I((gum::NodeId)2, (gum::NodeId)2), gum::OperationNotAllowed)
      TS_GUM_ASSERT_THROWS_NOTHING(inf.VI((gum::NodeId)2, (gum::NodeId)4));
      TS_GUM_ASSERT_THROWS_NOTHING(inf.I((gum::NodeId)0, (gum::NodeId)4));

      for (const auto node: bn->dag()) {
        for (const auto par: bn->dag().parents(node)) {
          TS_GUM_ASSERT_THROWS_NOTHING(inf.I(node, par));
          TS_GUM_ASSERT_THROWS_NOTHING(inf.I(par, node));
        }
      }

      //@TODO : test computations and not only good behaviour
    }
    void testInformationMethodsWithNames() {
      fill(*bn);

      gum::LazyPropagation< double > inf(bn);
      gum::NodeSet                   nodeset;
      nodeset.insert(0);
      nodeset.insert(4);
      inf.addJointTarget(nodeset);
      nodeset.clear();
      nodeset.insert(2);
      nodeset.insert(4);
      inf.addJointTarget(nodeset);
      inf.makeInference();

      TS_GUM_ASSERT_THROWS_NOTHING(inf.H(bn->variable(2).name()));
      TS_GUM_ASSERT_THROWS_NOTHING(inf.I(bn->variable(2).name(), bn->variable(4).name()));
      TS_ASSERT_THROWS(inf.I(bn->variable(2).name(), bn->variable(2).name()),
                       gum::OperationNotAllowed);
      TS_GUM_ASSERT_THROWS_NOTHING(inf.VI(bn->variable(2).name(), bn->variable(4).name()));
      TS_GUM_ASSERT_THROWS_NOTHING(inf.I(bn->variable(0).name(), bn->variable(4).name()));

      for (const auto node: bn->dag()) {
        for (const auto par: bn->dag().parents(node)) {
          TS_GUM_ASSERT_THROWS_NOTHING(inf.I(bn->variable(node).name(), bn->variable(par).name()));
          TS_GUM_ASSERT_THROWS_NOTHING(inf.I(bn->variable(par).name(), bn->variable(node).name()));
        }
      }

      //@TODO : test computations and not only good behaviour
    }

    void testSmartManagementOfJointTarget() {
      fill(*bn);

      gum::LazyPropagation< double > inf(bn);
      inf.addJointTarget(gum::NodeSet{0, 1, 2});
      inf.addJointTarget(gum::NodeSet{2, 3});
      TS_ASSERT_EQUALS(inf.nbrJointTargets(), (gum::Size)2)

      // should not be added since {0,1,2} already exists
      inf.addJointTarget(gum::NodeSet{0, 1});
      TS_ASSERT_EQUALS(inf.nbrJointTargets(), (gum::Size)2)

      // should remove {2,3} since {2,3,4} includes {2,3}
      inf.addJointTarget(gum::NodeSet{2, 3, 4});
      TS_ASSERT_EQUALS(inf.nbrJointTargets(), (gum::Size)2)
    }

    void testEvidenceProbability() {
      fill(*bn);
      gum::LazyPropagation< double > inf(bn);
      inf.setRelevantPotentialsFinderType(gum::RelevantPotentialsFinderType::FIND_ALL);
      inf.makeInference();
      auto p     = inf.posterior(0);
      auto I     = gum::Instantiation(p);
      auto proba = p.get(I);

      inf.addEvidence(0, 0);
      inf.makeInference();
      auto proba2 = inf.evidenceProbability();

      TS_ASSERT_DELTA(proba, proba2, TS_GUM_SMALL_ERROR)
    }

    void testEvidenceProbability2() {
      fill(*bn);
      gum::LazyPropagation< double > inf(bn);
      inf.makeInference();
      auto p     = inf.posterior(0);
      auto I     = gum::Instantiation(p);
      auto proba = p.get(I);

      inf.addEvidence(0, 0);
      inf.makeInference();
      auto proba2 = inf.evidenceProbability();

      TS_ASSERT_DELTA(proba, proba2, TS_GUM_SMALL_ERROR)
    }

    void testEvidenceProbabilityAsia() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);
      gum::Size                nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)

      auto id = bn.idFromName("lung_cancer");

      gum::LazyPropagation< double > inf(&bn);
      inf.setRelevantPotentialsFinderType(gum::RelevantPotentialsFinderType::FIND_ALL);
      inf.makeInference();
      auto p     = inf.posterior(id);
      auto I     = gum::Instantiation(p);
      auto proba = p.get(I);

      inf.addEvidence(id, 0);
      inf.makeInference();
      auto proba2 = inf.evidenceProbability();

      TS_ASSERT_DELTA(proba, proba2, TS_GUM_SMALL_ERROR)
    }


    void testAsia() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);
      gum::Size                nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)

      for (auto node: bn.dag()) {
        const auto&              variable = bn.variable(node);
        gum::Potential< double > ev_pot;
        ev_pot << variable;
        ev_pot.fill(0.0f);
        gum::List< const gum::Potential< double >* > evidences;
        evidences.insert(&ev_pot);

        gum::Instantiation inst(ev_pot);
        for (inst.setFirst(); !inst.end(); ++inst) {
          ev_pot.set(inst, 1.0f);
          gum::LazyPropagation< double >       inf1(&bn);
          gum::ShaferShenoyInference< double > inf2(&bn);
          for (auto pot: evidences) {
            TS_ASSERT_THROWS_NOTHING(inf1.addEvidence(*pot))
            TS_ASSERT_THROWS_NOTHING(inf2.addEvidence(*pot))
          }
          TS_ASSERT_THROWS_NOTHING(inf1.makeInference())
          TS_ASSERT_THROWS_NOTHING(inf2.makeInference())
          for (auto node: bn.dag()) {
            TS_ASSERT(equalPotentials(inf1.posterior(node), inf2.posterior(node)))
          }
          ev_pot.set(inst, (float)0);
        }
      }
    }

    void testAlarm() {
      std::string              file = GET_RESSOURCES_PATH("bif/alarm.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);
      gum::Size                nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)

      gum::LazyPropagation< double >     inf1(&bn);
      gum::VariableElimination< double > inf2(&bn);

      TS_ASSERT_THROWS_NOTHING(inf1.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf2.makeInference())

      for (auto node: bn.dag()) {
        TS_ASSERT_THROWS_NOTHING(inf1.posterior(node))
        TS_ASSERT_THROWS_NOTHING(inf2.posterior(node))
        TS_ASSERT(equalPotentials(inf1.posterior(node), inf2.posterior(node)))
      }

      std::vector< gum::NodeId >                   ev_nodes{2, 6, 7, 10, 12, 14, 16};
      gum::List< const gum::Potential< double >* > evidences;
      for (const auto node: ev_nodes) {
        gum::Potential< double >* ev_pot = new gum::Potential< double >;

        (*ev_pot) << bn.variable(node);
        ev_pot->fill(0.0f);
        gum::Instantiation inst(*ev_pot);
        if (node <= 10) {
          inst.chgVal(bn.variable(node), 0);
          ev_pot->set(inst, 1.0f);
        } else {
          inst.chgVal(bn.variable(node), 0);
          ev_pot->set(inst, 0.4f);
          inst.chgVal(bn.variable(node), 1);
          ev_pot->set(inst, 0.6f);
        }
        evidences.insert(ev_pot);
      }

      gum::LazyPropagation< double >       inf3(&bn);
      gum::ShaferShenoyInference< double > inf4(&bn);
      for (auto pot: evidences) {
        TS_ASSERT_THROWS_NOTHING(inf1.addEvidence(*pot))
        TS_ASSERT_THROWS_NOTHING(inf2.addEvidence(*pot))
        TS_ASSERT_THROWS_NOTHING(inf3.addEvidence(*pot))
        TS_ASSERT_THROWS_NOTHING(inf4.addEvidence(*pot))
      }

      TS_ASSERT_THROWS_NOTHING(inf1.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf2.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf3.makeInference())
      TS_ASSERT_THROWS_NOTHING(inf4.makeInference())

      for (auto node: bn.dag()) {
        TS_ASSERT_THROWS_NOTHING(inf1.posterior(node))
        TS_ASSERT_THROWS_NOTHING(inf2.posterior(node))
        TS_ASSERT_THROWS_NOTHING(inf3.posterior(node))
        TS_ASSERT_THROWS_NOTHING(inf4.posterior(node))
        TS_ASSERT(equalPotentials(inf1.posterior(node), inf2.posterior(node)))
        TS_ASSERT(equalPotentials(inf1.posterior(node), inf3.posterior(node)))
        TS_ASSERT(equalPotentials(inf1.posterior(node), inf4.posterior(node)))
      }

      gum::LazyPropagation< double > inf5(&bn);
      inf5.setRelevantPotentialsFinderType(gum::RelevantPotentialsFinderType::DSEP_BAYESBALL_NODES);
      for (auto pot: evidences) {
        TS_ASSERT_THROWS_NOTHING(inf5.addEvidence(*pot))
      }
      TS_ASSERT_THROWS_NOTHING(inf5.makeInference())
      for (auto node: bn.dag()) {
        TS_ASSERT_THROWS_NOTHING(inf5.posterior(node))
        TS_ASSERT(equalPotentials(inf1.posterior(node), inf5.posterior(node)))
      }

      for (auto pot: evidences)
        delete pot;
    }

    void testAsia2() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia3.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);
      gum::Size                nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)

      for (auto node: bn.dag()) {
        const auto&              variable = bn.variable(node);
        gum::Potential< double > ev_pot;
        ev_pot << variable;
        ev_pot.fill(0.0f);

        gum::Instantiation inst(ev_pot);
        for (inst.setFirst(); !inst.end(); ++inst) {
          ev_pot.set(inst, 1.0f);

          for (auto node2: bn.dag()) {
            if (node2 > node) {
              const auto&              variable2 = bn.variable(node2);
              gum::Potential< double > ev_pot2;
              ev_pot2 << variable2;
              ev_pot2.fill(0.0f);

              gum::List< const gum::Potential< double >* > evidences;
              evidences.insert(&ev_pot);
              evidences.insert(&ev_pot2);

              gum::Instantiation inst2(ev_pot2);
              for (inst2.setFirst(); !inst2.end(); ++inst2) {
                ev_pot2.set(inst2, 1.0f);

                gum::LazyPropagation< double >       inf1(&bn);
                gum::ShaferShenoyInference< double > inf2(&bn);
                for (auto pot: evidences) {
                  TS_ASSERT_THROWS_NOTHING(inf1.addEvidence(*pot))
                  TS_ASSERT_THROWS_NOTHING(inf2.addEvidence(*pot))
                }
                TS_ASSERT_THROWS_NOTHING(inf1.makeInference())
                TS_ASSERT_THROWS_NOTHING(inf2.makeInference())

                for (auto xnode: bn.dag()) {
                  TS_ASSERT(equalPotentials(inf1.posterior(xnode), inf2.posterior(xnode)))
                }
                ev_pot2.set(inst2, 0.0f);
              }
            }
          }

          ev_pot.set(inst, 0.0f);
        }
      }
    }

    void testAsia3() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia3.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);
      gum::Size                nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)

      for (auto node: bn.dag()) {
        const auto&              variable = bn.variable(node);
        gum::Potential< double > ev_pot;
        ev_pot << variable;
        ev_pot.fill(0.0f);

        gum::Instantiation inst(ev_pot);
        for (inst.setFirst(); !inst.end(); ++inst) {
          ev_pot.set(inst, 1.0f);

          for (auto node2: bn.dag()) {
            if (node2 > node) {
              const auto&              variable2 = bn.variable(node2);
              gum::Potential< double > ev_pot2;
              ev_pot2 << variable2;
              ev_pot2.fill(0.0f);

              gum::List< const gum::Potential< double >* > evidences;
              evidences.insert(&ev_pot);
              evidences.insert(&ev_pot2);

              gum::Instantiation inst2(ev_pot2);
              for (inst2.setFirst(); !inst2.end(); ++inst2) {
                ev_pot2.set(inst2, 1.0f);

                gum::LazyPropagation< double > inf1(&bn);
                inf1.setRelevantPotentialsFinderType(
                   gum::RelevantPotentialsFinderType::DSEP_BAYESBALL_NODES);
                gum::ShaferShenoyInference< double > inf2(&bn);
                for (auto pot: evidences) {
                  TS_ASSERT_THROWS_NOTHING(inf1.addEvidence(*pot))
                  TS_ASSERT_THROWS_NOTHING(inf2.addEvidence(*pot))
                }
                TS_ASSERT_THROWS_NOTHING(inf1.makeInference())
                TS_ASSERT_THROWS_NOTHING(inf2.makeInference())

                for (auto xnode: bn.dag()) {
                  TS_ASSERT(equalPotentials(inf1.posterior(xnode), inf2.posterior(xnode)))
                }
                ev_pot2.set(inst2, 0.0f);
              }
            }
          }

          ev_pot.set(inst, 0.0f);
        }
      }
    }

    void testAsia4() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);
      gum::Size                nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)

      for (auto node: bn.dag()) {
        const auto&              variable = bn.variable(node);
        gum::Potential< double > ev_pot;
        ev_pot << variable;
        ev_pot.fill(0.0f);

        gum::Instantiation inst(ev_pot);
        int                inst_index = 0;
        for (inst.setFirst(); !inst.end(); ++inst, ++inst_index) {
          ev_pot.set(inst, 1.0f);

          for (auto node2: bn.dag()) {
            if (node2 > node) {
              const auto&              variable2 = bn.variable(node2);
              gum::Potential< double > ev_pot2;
              ev_pot2 << variable2;
              ev_pot2.fill(0.0f);

              gum::List< const gum::Potential< double >* > evidences;
              evidences.insert(&ev_pot);
              evidences.insert(&ev_pot2);

              gum::Instantiation inst2(ev_pot2);
              int                inst2_index = 0;
              for (inst2.setFirst(); !inst2.end(); ++inst2, ++inst2_index) {
                ev_pot2.set(inst2, 1.0f);

                gum::LazyPropagation< double > inf1(&bn);
                inf1.setRelevantPotentialsFinderType(
                   gum::RelevantPotentialsFinderType::DSEP_BAYESBALL_POTENTIALS);
                gum::LazyPropagation< double > inf2(&bn);
                for (auto pot: evidences) {
                  TS_ASSERT_THROWS_NOTHING(inf1.addEvidence(*pot))
                  TS_ASSERT_THROWS_NOTHING(inf2.addEvidence(*pot))
                }
                TS_ASSERT_THROWS_NOTHING(inf1.makeInference())
                TS_ASSERT_THROWS_NOTHING(inf2.makeInference())

                for (auto xnode: bn.dag()) {
                  try {
                    equalPotentials(inf1.posterior(xnode), inf2.posterior(xnode));
                  } catch (gum::IncompatibleEvidence&) {
                    // check evidence incompatibility:
                    if (node2 == gum::NodeId(2)) {
                      // node2 = tuberculos_or_cancer, then node =
                      // tuberculosis
                      TS_ASSERT((inst2_index == 1) && (inst_index == 0))
                    } else {   // node2 = lung_cancer & node =
                      // tuberculos_or_cancer
                      TS_ASSERT((inst2_index == 0) && (inst_index == 1))
                    }
                  }
                }
                ev_pot2.set(inst2, 0.0f);
              }
            }
          }

          ev_pot.set(inst, 0.0f);
        }
      }
    }

    void testChgEvidence() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);

      gum::Size nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)


      gum::LazyPropagation< double > ie_0(&bn);
      ie_0.addTarget(0);        // visit_to_asia
      ie_0.addEvidence(1, 0);   // tuberculosis
      ie_0.makeInference();
      gum::Potential< double > p_0 = ie_0.posterior(0);

      gum::LazyPropagation< double > ie_1(&bn);
      ie_1.addTarget(0);
      ie_1.addEvidence(1, 1);
      ie_1.makeInference();
      gum::Potential< double > p_1 = ie_1.posterior(0);

      gum::LazyPropagation< double > ie(&bn);
      ie.addTarget(0);
      ie.addEvidence(1, 0);
      ie.makeInference();
      TS_ASSERT_EQUALS(p_0, ie.posterior(0))

      ie.chgEvidence(1, 1);
      ie.makeInference();
      TS_ASSERT_DIFFERS(p_0, ie.posterior(0))
      TS_ASSERT_EQUALS(p_1, ie.posterior(0))
    }

    void testChgEvidence2() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);

      gum::Size nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)


      gum::LazyPropagation< double > ie_0(&bn);
      ie_0.addTarget(0);        // visit_to_asia
      ie_0.addEvidence(1, 0);   // tuberculosis
      ie_0.makeInference();
      gum::Potential< double > p_0 = ie_0.posterior(0);

      gum::LazyPropagation< double > ie_1(&bn);
      ie_1.addTarget(0);
      ie_1.addEvidence(1, 1);
      ie_1.makeInference();
      gum::Potential< double > p_1 = ie_1.posterior(0);

      gum::LazyPropagation< double > ie(&bn);
      ie.eraseAllTargets();
      ie.addTarget(0);
      ie.addEvidence(1, 0);
      ie.makeInference();
      TS_ASSERT_EQUALS(p_0, ie.posterior(0))

      ie.chgEvidence(1, 1);
      ie.makeInference();
      TS_ASSERT_DIFFERS(p_0, ie.posterior(0))
      TS_ASSERT_EQUALS(p_1, ie.posterior(0))
    }

    void testStaticEvidenceImpact() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);

      gum::Size nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)

      gum::LazyPropagation< double > ie_all(&bn);
      TS_ASSERT_THROWS(ie_all.evidenceImpact(gum::NodeId(0), gum::NodeSet{0, 1, 2}),
                       gum::InvalidArgument);

      auto res = ie_all.evidenceImpact(gum::NodeId(0), gum::NodeSet{1, 2});

      TS_ASSERT_EQUALS(res.nbrDim(), (gum::Size)2);   // 2 indep 0 given 1

      gum::LazyPropagation< double > ie_0(&bn);
      ie_0.addTarget(0);        // visit_to_asia
      ie_0.addEvidence(1, 0);   // tuberculosis
      ie_0.makeInference();
      gum::Potential< double > p_0 = ie_0.posterior(0);

      gum::LazyPropagation< double > ie_1(&bn);
      ie_1.addTarget(0);
      ie_1.addEvidence(1, 1);
      ie_1.makeInference();
      gum::Potential< double > p_1 = ie_1.posterior(0);

      gum::Instantiation i;
      i.add(bn.variable(1));
      i.setFirst();
      TS_ASSERT_EQUALS(p_0, res.extract(i))
      i.inc();
      TS_ASSERT_EQUALS(p_1, res.extract(i))
    }

    void testEvidenceImpactWithNames() {
      std::string              file = GET_RESSOURCES_PATH("bif/asia.bif");
      gum::BayesNet< double >  bn;
      gum::BIFReader< double > reader(&bn, file);

      gum::Size nbrErr = (gum::Size)0;
      TS_GUM_ASSERT_THROWS_NOTHING(nbrErr = reader.proceed());
      TS_ASSERT_EQUALS(nbrErr, (gum::Size)0)
      TS_ASSERT_EQUALS(reader.warnings(), (gum::Size)0)


      gum::LazyPropagation< double > ie_all(&bn);
      TS_ASSERT_THROWS(ie_all.evidenceImpact(gum::NodeId(0), gum::NodeSet{0, 1, 2}),
                       gum::InvalidArgument);

      TS_ASSERT_THROWS(ie_all.evidenceImpact("visit_to_asia", {"tuberculosis", "toto"}),
                       gum::NotFound);

      auto res = ie_all.evidenceImpact("visit_to_Asia", {"tuberculosis", "tuberculos_or_cancer"});

      TS_ASSERT_EQUALS(res.nbrDim(), (gum::Size)2);   // 2 indep 0 given 1

      gum::LazyPropagation< double > ie_0(&bn);
      ie_0.addTarget(0);        // visit_to_asia
      ie_0.addEvidence(1, 0);   // tuberculosis
      ie_0.makeInference();
      gum::Potential< double > p_0 = ie_0.posterior(0);

      gum::LazyPropagation< double > ie_1(&bn);
      ie_1.addTarget(0);
      ie_1.addEvidence(1, 1);
      ie_1.makeInference();
      gum::Potential< double > p_1 = ie_1.posterior(0);

      gum::Instantiation i;
      i.add(bn.variable(1));
      i.setFirst();
      TS_ASSERT_EQUALS(p_0, res.extract(i))
      i.inc();
      TS_ASSERT_EQUALS(p_1, res.extract(i))
    }

    void testEvidenceImpact() {
      /*
      F  A
      \ / \
       B   |
       |   E
       C   |
      / \ /
      H  D
      */
      auto bn = gum::BayesNet< double >::fastPrototype("A->B->C->D;A->E->D;F->B;C->H;");

      gum::LazyPropagation< double > ie(&bn);
      gum::Potential< double >       res;
      TS_GUM_ASSERT_THROWS_NOTHING(res = ie.evidenceImpact("E", {"A", "B", "C", "D", "F"}));
      TS_ASSERT_EQUALS(res.nbrDim(), (gum::Size)4);   // MarkovBlanket(E)=(A,D,C)
      try {
        auto joint = bn.cpt("A") * bn.cpt("B") * bn.cpt("C") * bn.cpt("D") * bn.cpt("E")
                   * bn.cpt("F") * bn.cpt("H");
        auto pADCE = joint.margSumIn({&bn.variableFromName("A"),
                                      &bn.variableFromName("C"),
                                      &bn.variableFromName("D"),
                                      &bn.variableFromName("E")});
        auto pADC  = pADCE.margSumOut({&bn.variableFromName("E")});
        TS_ASSERT_EQUALS(res, pADCE / pADC)
      } catch (gum::Exception& e) { GUM_SHOWERROR(e); }
    }
    void testJointWithHardEvidence() {
      /*
      F  A
      \ / \
       B   |
       |   E
       C   |
      / \ /
      H  D
      */
      auto bn = gum::BayesNet< double >::fastPrototype("A->B->C->D;A->E->D;F->B;C->H;");

      gum::LazyPropagation< double > ie(&bn);
      ie.addEvidence("B", 0);
      gum::NodeSet joint{bn.idFromName("A"), bn.idFromName("B"), bn.idFromName("D")};

      ie.addJointTarget(joint);
      ie.makeInference();
      try {
        auto p = ie.jointPosterior(joint);
      } catch (gum::Exception& e) {
        GUM_SHOWERROR(e);
        TS_ASSERT(false)
      }
    }
    void testJointEvidenceImpact() {
      /*
      F  A
      \ / \
       B   |
       |   E
       C   |
      / \ /
      H  D
      */
      auto bn = gum::BayesNet< double >::fastPrototype("A->B->C->D;A->E->D;F->B;C->H;");

      gum::LazyPropagation< double > ie(&bn);
      gum::Potential< double >       res;
      TS_GUM_ASSERT_THROWS_NOTHING(res = ie.evidenceJointImpact({"D", "E"}, {"A", "B", "C", "F"}));
      TS_ASSERT_EQUALS(res.nbrDim(), (gum::Size)4);   // MarkovBlanket(E)=(A,D,C)
      try {
        auto joint = bn.cpt("A") * bn.cpt("B") * bn.cpt("C") * bn.cpt("D") * bn.cpt("E")
                   * bn.cpt("F") * bn.cpt("H");
        auto pADCE = joint.margSumIn({&bn.variableFromName("A"),
                                      &bn.variableFromName("C"),
                                      &bn.variableFromName("D"),
                                      &bn.variableFromName("E")});
        auto pAC   = pADCE.margSumOut({&bn.variableFromName("D"), &bn.variableFromName("E")});
        TS_ASSERT_EQUALS(res, pADCE / pAC)
      } catch (gum::Exception& e) { GUM_SHOWERROR(e); }
    }

    void testJointMutualInformation() {
      auto bn = gum::BayesNet< double >::fastPrototype("A->B->C->D;A->E->D;F->B;C->H;");

      gum::LazyPropagation< double > ie(&bn);
      ie.makeInference();

      TS_ASSERT_THROWS(ie.jointMutualInformation(gum::NodeSet{0}), gum::InvalidArgument)
      TS_ASSERT_DELTA(ie.I(0, 1), ie.jointMutualInformation(gum::NodeSet{0, 1}), 1e-7)

      ie.addJointTarget({1, 4, 3});
      ie.makeInference();
      double byHandJMI;
      double JMI;

      byHandJMI = -ie.jointPosterior({1, 3, 4}).entropy();
      byHandJMI += ie.jointPosterior({1, 4}).entropy() + ie.jointPosterior({1, 3}).entropy()
                 + ie.jointPosterior({4, 3}).entropy();
      byHandJMI
         -= ie.posterior(1).entropy() + ie.posterior(4).entropy() + ie.posterior(3).entropy();

      //@todo why do I need to create a new LazyPropagation
      gum::LazyPropagation< double > ie2(&bn);
      JMI = ie2.jointMutualInformation({1, 3, 4});

      TS_ASSERT_DELTA(JMI, byHandJMI, 1e-7)

      ie.clear();
      ie.addJointTarget({0, 1, 2, 3});
      ie.makeInference();
      byHandJMI = -ie.jointPosterior({0, 1, 2, 3}).entropy();
      byHandJMI += ie.jointPosterior({0, 1, 2}).entropy() + ie.jointPosterior({0, 1, 3}).entropy()
                 + ie.jointPosterior({0, 2, 3}).entropy() + ie.jointPosterior({1, 2, 3}).entropy();
      byHandJMI -= ie.jointPosterior({0, 1}).entropy() + ie.jointPosterior({0, 2}).entropy()
                 + ie.jointPosterior({0, 3}).entropy() + ie.jointPosterior({1, 2}).entropy()
                 + ie.jointPosterior({1, 3}).entropy() + ie.jointPosterior({2, 3}).entropy();
      byHandJMI += ie.posterior(0).entropy() + ie.posterior(1).entropy() + ie.posterior(2).entropy()
                 + ie.posterior(3).entropy();

      //@todo why do I need to create a new LazyPropagation
      gum::LazyPropagation< double > ie3(&bn);
      JMI = ie3.jointMutualInformation({0, 1, 2, 3});

      TS_ASSERT_DELTA(JMI, byHandJMI, 1e-7)
    }

    void testLogitInBN() {
      gum::BayesNet< double > bn;

      gum::LabelizedVariable cold("Cold", "", 2);
      gum::LabelizedVariable flu("Flu", "", 2);
      gum::LabelizedVariable malaria("Malaria", "", 2);
      gum::LabelizedVariable fever("Fever", "", 2);
      gum::LabelizedVariable oneMore("OneMore", "", 2);
      gum::LabelizedVariable oneMoreParent1("OneMoreParent1", "", 2);
      gum::LabelizedVariable oneMoreParent2("OneMoreParent2", "", 2);

      gum::NodeId idCold    = bn.add(cold);
      gum::NodeId idFlu     = bn.add(flu);
      gum::NodeId idMalaria = bn.add(malaria);
      gum::NodeId idFever   = 0;
      TS_GUM_ASSERT_THROWS_NOTHING(idFever = bn.addLogit(fever, 0.3f));
      gum::NodeId idOneMore        = bn.add(oneMore);
      gum::NodeId idOneMoreParent1 = bn.add(oneMoreParent1);
      gum::NodeId idOneMoreParent2 = bn.add(oneMoreParent2);

      bn.addWeightedArc(idMalaria, idFever, 0.9f);
      bn.addWeightedArc(idFlu, idFever, 0.8f);
      bn.addWeightedArc(idCold, idFever, 0.4f);

      TS_ASSERT_THROWS(bn.addWeightedArc(idMalaria, idCold, 0.8f), gum::InvalidArc)

      const gum::Potential< double >& pOneMoreParent1 = bn.cpt(idOneMoreParent1);
      // FILLING PARAMS
      pOneMoreParent1.fillWith({0.2f, 0.8f});

      const gum::Potential< double >& pOneMoreParent2 = bn.cpt(idOneMoreParent2);
      // FILLING PARAMS
      pOneMoreParent2.fillWith({0.3f, 0.7f});

      bn.addArc(idOneMoreParent1, idOneMore);
      bn.addArc(idFever, idOneMore);
      bn.addArc(idOneMoreParent2, idOneMore);
      const gum::Potential< double >& pOneMore = bn.cpt(idOneMore);
      const gum::Potential< double >& p        = bn.cpt(idFever);

      gum::Instantiation i(p);

      // FILLING PARAMS
      // clang-format off
      pOneMore.fillWith(
                        {0.1f,0.9f,
                         0.8f,0.2f,
                         0.1f,0.9f,
                         0.8f,0.2f,
                         0.1f,0.9f,
                         0.8f,0.2f,
                         0.1f,0.9f,
                         0.8f,0.2f});

      float witness[] =
                        {0.42555748318834097f,0.574442516811659f,
                         0.23147521650098246f,0.7685247834990175f,
                         0.2497398944048823f,0.7502601055951177f,
                         0.11920292202211769f,0.8807970779778823f,
                         0.33181222783183384f,0.6681877721681662f,
                         0.16798161486607555f,0.8320183851339245f,
                         0.18242552380635635f,0.8175744761936437f,
                         0.08317269649392234f,0.9168273035060777f};
      // clang-format on

      int j = 0;

      for (i.setFirst(); !i.end(); ++i, j++) {
        TS_ASSERT_DELTA(p[i], witness[j], 1e-6)
      }

      gum::LazyPropagation< double > inf_LazyProp(&bn);

      inf_LazyProp.makeInference();
    }

    void testMultipleJointMutualInformation() {
      auto                           bn = gum::BayesNet< double >::fastPrototype("A->B;A->C->D");
      gum::LazyPropagation< double > ie(&bn);

      try {
        for (const auto& a: bn.arcs()) {
          ie.eraseAllTargets();
          ie.eraseAllEvidence();
          // GUM_TRACE_VAR(a);
          ie.addJointTarget({a.first(), a.second()});
          // GUM_TRACE_VAR(ie.jointTargets());
          ie.makeInference();
          const auto po = ie.jointPosterior({a.first(), a.second()});
        }
      } catch (gum::Exception& e) { GUM_SHOWERROR(e); }
      /*
      try {
        for (const auto& a : bn.arcs()) {
          std::cout << a << " : " << std::flush
                    << ie.jointMutualInformation({a.first(), a.second()})
                    << std::endl;
        }
      } catch (gum::Exception& e) { GUM_SHOWERROR(e); }
       */
    }


    void testAdaptiveNbThreads() {
      fill(*bn);
      // Testing the inference
      gum::LazyPropagation< double > inf(bn);

      TS_ASSERT(inf.getNumberOfThreads() == gum::getNumberOfThreads());
      inf.setNumberOfThreads(10);
      TS_ASSERT(inf.getNumberOfThreads() == 10);
      gum::setNumberOfThreads(25);
      TS_ASSERT(inf.getNumberOfThreads() == 10);
      inf.setNumberOfThreads(0);
      TS_ASSERT(inf.getNumberOfThreads() == gum::getNumberOfThreads());
      gum::setNumberOfThreads(32);
      TS_ASSERT(inf.getNumberOfThreads() == gum::getNumberOfThreads());
    }

    void testProbaEvidence() {
      std::vector< gum::LabelizedVariable* > vars(10);

      for (gum::Idx i = 0; i < 10; ++i) {
        std::stringstream str;
        str << "x" << i;
        std::string s = str.str();
        vars[i]       = new gum::LabelizedVariable(s, s, 2);
      }

      gum::BayesNet< double > bn;
      gum::NodeId             i0 = bn.add(*vars[0]);
      auto&                   x0 = bn.variable(i0);
      auto&                   p0 = bn.cpt(i0);
      p0.fillWith({0.7, 0.3});

      gum::Potential< double > ev0;
      ev0 << x0;
      ev0.fillWith({0.3, 0.2});

      gum::Potential< double > ev0b;
      ev0b << x0;
      ev0b.fillWith({1.0, 0.0});

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        double xpe0 = inf.evidenceProbability();
        double pe0  = (p0 * ev0).sum();
        TS_ASSERT_DELTA(pe0, 0.27, 0.0001)
        TS_ASSERT_DELTA(xpe0, pe0, 0.0001)
      }

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0b);
        double xpe0b = inf.evidenceProbability();
        double pe0b  = (p0 * ev0b).sum();
        TS_ASSERT_DELTA(pe0b, 0.7, 0.0001)
        TS_ASSERT_DELTA(xpe0b, pe0b, 0.0001)
      }

      gum::NodeId i1 = bn.add(*vars[1]);
      auto&       x1 = bn.variable(i1);
      auto&       p1 = bn.cpt(i1);
      p1.fillWith({0.4, 0.6});

      gum::Potential< double > ev1;
      ev1 << x1;
      ev1.fillWith({0.4, 0.6});

      gum::Potential< double > ev1b;
      ev1b << x1;
      ev1b.fillWith({0.0, 1.0});

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        double xpe01 = inf.evidenceProbability();
        double pe01  = (p0 * ev0 * p1 * ev1).sum();
        TS_ASSERT_DELTA(pe01, 0.27 * 0.52, 0.0001)
        TS_ASSERT_DELTA(xpe01, pe01, 0.0001)
      }

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0b);
        inf.addEvidence(ev1);
        double xpe01 = inf.evidenceProbability();
        double pe01  = (p0 * ev0b * p1 * ev1).sum();
        TS_ASSERT_DELTA(pe01, 0.7 * 0.52, 0.0001)
        TS_ASSERT_DELTA(xpe01, pe01, 0.0001)
      }

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1b);
        double xpe01 = inf.evidenceProbability();
        double pe01  = (p0 * ev0 * p1 * ev1b).sum();
        TS_ASSERT_DELTA(pe01, 0.27 * 0.6, 0.0001)
        TS_ASSERT_DELTA(xpe01, pe01, 0.0001)
      }

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0b);
        inf.addEvidence(ev1b);
        double xpe01 = inf.evidenceProbability();
        double pe01  = (p0 * ev0b * p1 * ev1b).sum();
        TS_ASSERT_DELTA(pe01, 0.7 * 0.6, 0.0001)
        TS_ASSERT_DELTA(xpe01, pe01, 0.0001)
      }

      gum::NodeId i2 = bn.add(*vars[2]);
      bn.addArc(i0, i2);
      auto& x2 = bn.variable(i2);
      auto& p2 = bn.cpt(i2);
      p2.fillWith({0.4, 0.6, 0.8, 0.2});

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        double xpe02 = inf.evidenceProbability();
        double pe02  = (p0 * ev0 * p1 * ev1 * p2).sum();
        TS_ASSERT_DELTA(pe02, 0.27 * 0.52, 0.0001)
        TS_ASSERT_DELTA(xpe02, pe02, 0.0001)
      }

      gum::NodeId i3 = bn.add(*vars[3]);
      bn.addArc(i2, i3);
      auto& x3 = bn.variable(i3);
      auto& p3 = bn.cpt(i3);
      p3.fillWith({0.1, 0.9, 0.7, 0.3});

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        double xpe03 = inf.evidenceProbability();
        double pe03  = (p0 * ev0 * p1 * ev1 * p2 * p3).sum();
        TS_ASSERT_DELTA(pe03, 0.27 * 0.52, 0.0001)
        TS_ASSERT_DELTA(xpe03, pe03, 0.0001)
      }

      gum::Potential< double > ev3;
      ev3 << x3;
      ev3.fillWith({0.4, 0.3});

      gum::Potential< double > ev3b;
      ev3b << x3;
      ev3b.fillWith({1.0, 0.0});

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        inf.addEvidence(ev3);
        double xpe0_3 = inf.evidenceProbability();
        double pe0_3  = (p0 * ev0 * p1 * ev1 * p2 * p3 * ev3).sum();

        // P(x0,x3) = [X00: [0.3220, 0.3780], x01: [0.0660, 0.2340]]
        double p03 = 0.322 * 0.12 + 0.378 * 0.09 + 0.066 * 0.08 + 0.234 * .06;
        TS_ASSERT_DELTA(pe0_3, p03 * 0.52, 0.0001)
        TS_ASSERT_DELTA(xpe0_3, pe0_3, 0.0001)
      }

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        inf.addEvidence(ev3b);
        double xpe0_3 = inf.evidenceProbability();
        double pe0_3  = (p0 * ev0 * p1 * ev1 * p2 * p3 * ev3b).sum();

        // P(x0,x3) = [X00: [0.3220, 0.3780], x01: [0.0660, 0.2340]]
        double p03 = 0.322 * 0.3 + 0.066 * 0.2;
        TS_ASSERT_DELTA(pe0_3, p03 * 0.52, 0.0001)
        TS_ASSERT_DELTA(xpe0_3, pe0_3, 0.0001)
      }

      gum::NodeId i4 = bn.add(*vars[4]);
      bn.addArc(i1, i4);
      bn.addArc(i3, i4);
      auto& x4 = bn.variable(i4);
      auto& p4 = bn.cpt(i4);
      p4.fillWith({0.2, 0.8, 0.3, 0.7, 0.4, 0.6, 0.5, 0.5});

      gum::NodeId i5 = bn.add(*vars[5]);
      bn.addArc(i3, i5);
      auto& x5 = bn.variable(i5);
      auto& p5 = bn.cpt(i5);
      p5.fillWith({0.7, 0.3, 0.8, 0.2});

      gum::Potential< double > ev5;
      ev5 << x5;
      ev5.fillWith({0.7, 0.6});

      gum::Potential< double > pot0_5;
      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addJointTarget({i0, i1, i5});
        pot0_5 = inf.jointPosterior({i0, i1, i5});
      }

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        inf.addEvidence(ev5);

        double xpe0_5 = inf.evidenceProbability();
        double pe0_5  = (pot0_5 * ev0 * ev1 * ev5).sum();
        TS_ASSERT_DELTA(xpe0_5, pe0_5, 0.0001)
      }

      gum::NodeId i6 = bn.add(*vars[6]);
      auto&       x6 = bn.variable(i6);
      auto&       p6 = bn.cpt(i6);
      p6.fillWith({0.8, 0.2});

      gum::Potential< double > ev6;
      ev6 << x6;
      ev6.fillWith({0.7, 0.6});

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        inf.addEvidence(ev5);
        inf.addEvidence(ev6);

        double xpe0_6 = inf.evidenceProbability();
        double pe0_6  = (pot0_5 * ev0 * ev1 * ev5 * p6 * ev6).sum();
        TS_ASSERT_DELTA(xpe0_6, pe0_6, 0.0001)
      }

      gum::NodeId i7 = bn.add(*vars[7]);
      bn.addArc(i4, i7);
      auto& x7 = bn.variable(i7);
      auto& p7 = bn.cpt(i7);
      p7.fillWith({0.4, 0.6, 0.6, 0.4});

      gum::Potential< double > ev7;
      ev7 << x7;
      ev7.fillWith({0.2, 0.5});

      gum::Potential< double > pot0_7;
      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addJointTarget({i0, i1, i5, i7});
        pot0_7 = inf.jointPosterior({i0, i1, i5, i7});
      }

      {
        gum::LazyPropagation< double > inf(&bn);
        inf.addEvidence(ev0);
        inf.addEvidence(ev1);
        inf.addEvidence(ev5);
        inf.addEvidence(ev6);
        inf.addEvidence(ev7);

        double xpe0_7 = inf.evidenceProbability();
        double pe0_7  = (pot0_7 * ev0 * ev1 * ev5 * ev7 * p6 * ev6).sum();
        TS_ASSERT_DELTA(xpe0_7, pe0_7, 0.0001)
      }

      for (gum::Idx i = 0; i < vars.size(); ++i)
        delete vars[i];
    }


    private:
    void randomInitP(const gum::Potential< double >& tt) {
      auto&              t = const_cast< gum::Potential< double >& >(tt);
      gum::Instantiation i(t);

      for (i.setFirst(); !i.end(); ++i)
        t.set(i, 1.0f + rand() * 50.0f / float(RAND_MAX));
    }


    // Builds a BN to test the inference
    void fill(gum::BayesNet< double >& bn) {
      bn.cpt(i1).fillWith({0.2f, 0.8f});
      bn.cpt(i2).fillWith({0.3f, 0.7f});
      bn.cpt(i3).fillWith({0.1f, 0.9f, 0.9f, 0.1f});
      bn.cpt(i4).fillWith(   // clang-format off
              {0.4f, 0.6f,
               0.5f, 0.5f,
               0.5f, 0.5f,
               1.0f, 0.0f});                     // clang-format on
      bn.cpt(i5).fillWith(   // clang-format off
              {0.3f, 0.6f, 0.1f,
               0.5f, 0.5f, 0.0f,
               0.5f, 0.5f, 0.0f,
               1.0f, 0.0f, 0.0f,
               0.4f, 0.6f, 0.0f,
               0.5f, 0.5f, 0.0f,
               0.5f, 0.5f, 0.0f,
               0.0f, 0.0f, 1.0f});   // clang-format on
    }
  };
}   // namespace gum_tests
