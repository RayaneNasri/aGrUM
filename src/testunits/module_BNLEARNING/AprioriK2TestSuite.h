/**
 *
 *   Copyright (c) 2005-2022 by Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
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


#include <gumtest/AgrumTestSuite.h>
#include <gumtest/testsuite_utils.h>
#include <iostream>

#include <agrum/tools/database/DBTranslator4LabelizedVariable.h>
#include <agrum/tools/database/DBTranslatorSet.h>
#include <agrum/BN/learning/priors/K2Prior.h>

namespace gum_tests {

  class AprioriK2TestSuite: public CxxTest::TestSuite {
    public:
    void test1() {
      // create the translator set
      gum::LabelizedVariable var1("X1", "", 0);
      var1.addLabel("0");
      var1.addLabel("1");
      var1.addLabel("2");

      gum::LabelizedVariable var2("X2", "", 0);
      var2.addLabel("0");
      var2.addLabel("1");
      var2.addLabel("2");
      var2.addLabel("3");

      gum::learning::DBTranslatorSet trans_set;
      {
        const std::vector< std::string >              miss;
        gum::learning::DBTranslator4LabelizedVariable translator1(var1, miss);
        gum::learning::DBTranslator4LabelizedVariable translator2(var2, miss);
        std::vector< std::string >                    names{"A", "B", "C", "D", "E", "F"};

        for (auto i = std::size_t(0); i < names.size(); ++i)
          trans_set.insertTranslator(i % 2 == 0 ? translator1 : translator2, i);
      }

      gum::learning::DatabaseTable database(trans_set);


      gum::learning::K2Prior apriori(database);
      TS_ASSERT_EQUALS(apriori.weight(), 1.0)
      apriori.setWeight(4.0);
      TS_ASSERT_EQUALS(apriori.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori.getType(), gum::learning::PriorType::SmoothingPriorType)

      gum::NodeId                node0 = 0;
      gum::NodeId                node1 = 1;
      gum::NodeId                node3 = 3;
      gum::NodeId                node4 = 4;
      gum::NodeId                node5 = 5;
      std::vector< gum::NodeId > cond_empty;
      std::vector< gum::NodeId > cond1{node3, node5, node4};

      gum::learning::IdCondSet idset1(node0, cond_empty);   // #3,#0
      gum::learning::IdCondSet idset2(node0, node1, cond_empty,
                                      true);   // #12,#0
      gum::learning::IdCondSet idset3(node1, node0, cond1, true,
                                      true);   // #576,#48

      std::vector< double > vect(3, 1.0);
      apriori.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(12, 1.0);
      apriori.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }

      vect.clear();
      vect.resize(48, 1.0);
      apriori.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 13.0)
      }


      gum::learning::K2Prior apriori2(apriori);
      TS_ASSERT_EQUALS(apriori2.weight(), 1.0)
      apriori2.setWeight(2.0);
      TS_ASSERT_EQUALS(apriori2.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori2.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(3, 1.0);
      apriori2.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori2.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(12, 1.0);
      apriori2.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori2.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori2.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(48, 1.0);
      apriori2.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 13.0)
      }


      gum::learning::K2Prior apriori3(std::move(apriori2));
      TS_ASSERT_EQUALS(apriori3.weight(), 1.0)
      apriori3.setWeight(4.0);
      TS_ASSERT_EQUALS(apriori3.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori3.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(3, 1.0);
      apriori3.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori3.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(12, 1.0);
      apriori3.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori3.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori3.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(48, 1.0);
      apriori3.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 13.0)
      }


      gum::learning::K2Prior* apriori4 = apriori.clone();
      TS_ASSERT_EQUALS(apriori4->weight(), 1.0)
      apriori4->setWeight(2.0);
      TS_ASSERT_EQUALS(apriori4->weight(), 1.0)

      TS_ASSERT_EQUALS(apriori4->getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(3, 1.0);
      apriori4->addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori4->addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(12, 1.0);
      apriori4->addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori4->addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori4->addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(48, 1.0);
      apriori4->addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 13.0)
      }

      delete apriori4;


      gum::learning::DatabaseTable database2;
      gum::learning::K2Prior       apriori5(database2);
      apriori5 = apriori;
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)
      apriori5.setWeight(2.0);
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori5.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(3, 1.0);
      apriori5.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(12, 1.0);
      apriori5.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori5.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(48, 1.0);
      apriori5.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 13.0)
      }

      apriori5 = std::move(apriori);
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)
      apriori5.setWeight(1.0);
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori5.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(3, 1.0);
      apriori5.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(12, 1.0);
      apriori5.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori5.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(48, 1.0);
      apriori5.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 13.0)
      }
    }


    void test2() {
      // create the translator set
      gum::LabelizedVariable var1("X1", "", 0);
      var1.addLabel("0");
      var1.addLabel("1");
      var1.addLabel("2");

      gum::LabelizedVariable var2("X2", "", 0);
      var2.addLabel("0");
      var2.addLabel("1");
      var2.addLabel("2");
      var2.addLabel("3");

      gum::learning::DBTranslatorSet trans_set;
      {
        const std::vector< std::string >              miss;
        gum::learning::DBTranslator4LabelizedVariable translator1(var1, miss);
        gum::learning::DBTranslator4LabelizedVariable translator2(var2, miss);
        std::vector< std::string >                    names{"A", "B", "C", "D", "E", "F"};

        for (auto i = std::size_t(0); i < names.size(); ++i)
          if (i % 2 == 0) {
            trans_set.insertTranslator(translator1, i);
          } else {
            trans_set.insertTranslator(translator2, i);
          }
      }

      gum::learning::DatabaseTable database(trans_set);

      gum::NodeId                                node0 = 0;
      gum::NodeId                                node1 = 1;
      gum::NodeId                                node2 = 2;
      gum::NodeId                                node3 = 3;
      gum::NodeId                                node4 = 4;
      gum::NodeId                                node5 = 5;
      gum::Bijection< gum::NodeId, std::size_t > nodeId2columns;
      nodeId2columns.insert(node0, std::size_t(1));
      nodeId2columns.insert(node1, std::size_t(3));
      nodeId2columns.insert(node2, std::size_t(0));
      nodeId2columns.insert(node3, std::size_t(2));
      nodeId2columns.insert(node4, std::size_t(4));
      nodeId2columns.insert(node5, std::size_t(5));


      gum::learning::K2Prior apriori(database, nodeId2columns);
      TS_ASSERT_EQUALS(apriori.weight(), 1.0)
      apriori.setWeight(4.0);
      TS_ASSERT_EQUALS(apriori.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori.getType(), gum::learning::PriorType::SmoothingPriorType)

      std::vector< gum::NodeId > cond_empty;
      std::vector< gum::NodeId > cond1{node3, node5, node4};

      gum::learning::IdCondSet idset1(node0, cond_empty);   // #4,#0
      gum::learning::IdCondSet idset2(node0, node1, cond_empty,
                                      true);   // #16,#0
      gum::learning::IdCondSet idset3(node1, node0, cond1, true,
                                      true);   // #576,#36

      std::vector< double > vect(4, 1.0);
      apriori.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(16, 1.0);
      apriori.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(36, 1.0);
      apriori.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 17.0)
      }


      gum::learning::K2Prior apriori2(apriori);
      TS_ASSERT_EQUALS(apriori2.weight(), 1.0)
      apriori2.setWeight(2.0);
      TS_ASSERT_EQUALS(apriori2.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori2.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(4, 1.0);
      apriori2.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori2.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(16, 1.0);
      apriori2.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori2.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori2.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(36, 1.0);
      apriori2.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 17.0)
      }

      gum::learning::K2Prior apriori3(std::move(apriori2));
      TS_ASSERT_EQUALS(apriori3.weight(), 1.0)
      apriori3.setWeight(4.0);
      TS_ASSERT_EQUALS(apriori3.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori3.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(4, 1.0);
      apriori3.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori3.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(16, 1.0);
      apriori3.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori3.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori3.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(36, 1.0);
      apriori3.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 17.0)
      }


      gum::learning::K2Prior* apriori4 = apriori.clone();
      TS_ASSERT_EQUALS(apriori4->weight(), 1.0)
      apriori4->setWeight(2.0);
      TS_ASSERT_EQUALS(apriori4->weight(), 1.0)

      TS_ASSERT_EQUALS(apriori4->getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(4, 1.0);
      apriori4->addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori4->addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(16, 1.0);
      apriori4->addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori4->addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori4->addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(36, 1.0);
      apriori4->addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 17.0)
      }

      delete apriori4;

      gum::learning::DatabaseTable database2;
      gum::learning::K2Prior       apriori5(database2);
      apriori5 = apriori;
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)
      apriori5.setWeight(2.0);
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori5.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(4, 1.0);
      apriori5.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(16, 1.0);
      apriori5.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori5.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(36, 1.0);
      apriori5.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 17.0)
      }


      apriori5 = std::move(apriori);
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)
      apriori5.setWeight(1.0);
      TS_ASSERT_EQUALS(apriori5.weight(), 1.0)

      TS_ASSERT_EQUALS(apriori5.getType(), gum::learning::PriorType::SmoothingPriorType)

      vect.clear();
      vect.resize(4, 1.0);
      apriori5.addAllApriori(idset1, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset1, vect);

      vect.clear();
      vect.resize(16, 1.0);
      apriori5.addAllApriori(idset2, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      apriori5.addConditioningApriori(idset2, vect);

      vect.clear();
      vect.resize(576, 1.0);
      apriori5.addAllApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 2.0)
      }
      vect.clear();
      vect.resize(36, 1.0);
      apriori5.addConditioningApriori(idset3, vect);
      for (const auto val: vect) {
        TS_ASSERT_EQUALS(val, 17.0)
      }
    }
  };

} /* namespace gum_tests */
