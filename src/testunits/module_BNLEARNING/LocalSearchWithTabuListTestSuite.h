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

#include <agrum/BN/BayesNet.h>
#include <agrum/tools/graphs/DAG.h>
#include <agrum/tools/variables/labelizedVariable.h>

#include <agrum/tools/database/DBTranslator4LabelizedVariable.h>
#include <agrum/tools/database/DBRowGeneratorParser.h>
#include <agrum/tools/database/DBInitializerFromCSV.h>
#include <agrum/tools/database/databaseTable.h>
#include <agrum/tools/database/DBTranslatorSet.h>

#include <agrum/BN/learning/scores_and_tests/scoreBDeu.h>
#include <agrum/BN/learning/scores_and_tests/scoreK2.h>

#include <agrum/BN/learning/aprioris/aprioriSmoothing.h>

#include <agrum/BN/learning/constraints/structuralConstraintDAG.h>
#include <agrum/BN/learning/constraints/structuralConstraintDiGraph.h>
#include <agrum/BN/learning/constraints/structuralConstraintIndegree.h>
#include <agrum/BN/learning/constraints/structuralConstraintSetStatic.h>
#include <agrum/BN/learning/constraints/structuralConstraintSliceOrder.h>
#include <agrum/BN/learning/constraints/structuralConstraintTabuList.h>

#include <agrum/BN/learning/structureUtils/graphChangesGenerator4DiGraph.h>
#include <agrum/BN/learning/structureUtils/graphChangesSelector4DiGraph.h>

#include <agrum/BN/learning/localSearchWithTabuList.h>
#include <agrum/BN/learning/paramUtils/paramEstimatorML.h>

namespace gum_tests {

  class LocalSearchWithTabuListTestSuite: public CxxTest::TestSuite {
    public:
    void test_asia() {
      gum::learning::DBInitializerFromCSV initializer(GET_RESSOURCES_PATH("csv/asia.csv"));
      const auto&                         var_names = initializer.variableNames();
      const std::size_t                   nb_vars   = var_names.size();

      gum::learning::DBTranslatorSet                translator_set;
      gum::learning::DBTranslator4LabelizedVariable translator;
      for (std::size_t i = 0; i < nb_vars; ++i) {
        translator_set.insertTranslator(translator, i);
      }

      gum::learning::DatabaseTable database(translator_set);
      database.setVariableNames(initializer.variableNames());
      initializer.fillDatabase(database);

      gum::learning::DBRowGeneratorSet    genset;
      gum::learning::DBRowGeneratorParser parser(database.handler(), genset);
      gum::learning::AprioriSmoothing     apriori(database);
      gum::learning::ScoreK2              score(parser, apriori);

      gum::learning::StructuralConstraintSetStatic< gum::learning::StructuralConstraintDAG,
                                                    gum::learning::StructuralConstraintIndegree,
                                                    gum::learning::StructuralConstraintSliceOrder,
                                                    gum::learning::StructuralConstraintTabuList >
         struct_constraint;

      struct_constraint.setMaxIndegree(2);
      struct_constraint.setTabuListSize(100);

      gum::NodeProperty< gum::NodeId > slices{std::make_pair(gum::NodeId(0), 0),
                                              std::make_pair(gum::NodeId(1), 0)};
      struct_constraint.setSliceOrder(slices);
      struct_constraint.setDefaultSlice(1);

      gum::learning::ParamEstimatorML estimator(parser, apriori, score.internalApriori());

      gum::learning::GraphChangesGenerator4DiGraph< decltype(struct_constraint) > op_set(
         struct_constraint);

      gum::learning::GraphChangesSelector4DiGraph< decltype(struct_constraint), decltype(op_set) >
         selector(score, struct_constraint, op_set);

      gum::learning::LocalSearchWithTabuList search;
      search.setMaxNbDecreasingChanges(2);

      try {
        gum::BayesNet< double > bn  = search.learnBN< double >(selector, estimator);
        gum::BayesNet< double > bn2 = search.learnBN< double >(selector, estimator);
        TS_ASSERT_EQUALS(bn.dag().arcs().size(), (gum::Size)10)
      } catch (gum::Exception& e) { GUM_SHOWERROR(e); }
    }


    /*
    void xtest_alarm1() {
      gum::learning::DatabaseFromCSV
    database(GET_RESSOURCES_PATH("csv/alarm.csv"));

      gum::learning::DBRowTranslatorSet<
        gum::learning::CellTranslatorCompactIntId >
        translators;
      translators.insertTranslator(0, 37);

      gum::learning::FilteredRowGeneratorSet< gum::learning::RowGeneratorIdentity >
        generators;
      generators.insertGenerator();

      auto filter =
        gum::learning::make_DB_row_filter(database, translators, generators);

      std::vector< gum::Size > modalities = filter.modalities();

      gum::learning::AprioriSmoothing apriori;
      apriori.setWeight(0);
      gum::learning::ScoreK2 score(filter, modalities, apriori);

      gum::learning::StructuralConstraintSetStatic<
        gum::learning::StructuralConstraintDAG,
        gum::learning::StructuralConstraintIndegree,
        gum::learning::StructuralConstraintSliceOrder,
        gum::learning::StructuralConstraintTabuList >
        struct_constraint;

      struct_constraint.setMaxIndegree(4);
      struct_constraint.setTabuListSize(100);

      gum::NodeProperty< gum::NodeId > slices{std::make_pair(gum::NodeId(0), 0),
                                              std::make_pair(gum::NodeId(1), 0)};
      struct_constraint.setSliceOrder(slices);
      struct_constraint.setDefaultSlice(1);

      gum::learning::ParamEstimatorML estimator(filter, modalities, apriori);

      gum::learning::GraphChangesGenerator4DiGraph< decltype(struct_constraint) >
        op_set(struct_constraint);

      gum::learning::GraphChangesSelector4DiGraph< decltype(score),
                                                   decltype(struct_constraint),
                                                   decltype(op_set) >
        selector(score, struct_constraint, op_set);

      gum::learning::LocalSearchWithTabuList search;

      try {
        gum::Timer timer;
        gum::DAG   bn = search.learnStructure(selector, modalities);
        std::cout << timer.step() << " : " << std::endl;
        std::cout << bn << std::endl;
      } catch (gum::Exception& e) {
        GUM_SHOWERROR(e);
      }
    }

    void xtest_alarm2() {
      gum::learning::DatabaseFromCSV
    database(GET_RESSOURCES_PATH("csv/alarm.csv"));

      gum::learning::DBRowTranslatorSet<
        gum::learning::CellTranslatorCompactIntId >
        translators;
      translators.insertTranslator(
        gum::learning::CellTranslatorCompactIntId(), 0, 37);

      gum::learning::FilteredRowGeneratorSet< gum::learning::RowGeneratorIdentity >
        generators;
      generators.insertGenerator();

      auto filter =
        gum::learning::make_DB_row_filter(database, translators, generators);

      std::vector< gum::Size > modalities = filter.modalities();

      gum::learning::AprioriSmoothing apriori;
      apriori.setWeight(0);
      gum::learning::ScoreK2 score(filter, modalities, apriori);

      gum::learning::StructuralConstraintSetStatic<
        gum::learning::StructuralConstraintDAG,
        gum::learning::StructuralConstraintIndegree,
        gum::learning::StructuralConstraintSliceOrder,
        gum::learning::StructuralConstraintTabuList >
        struct_constraint;

      struct_constraint.setMaxIndegree(4);
      struct_constraint.setTabuListSize(100);

      gum::NodeProperty< gum::NodeId > slices{std::make_pair(gum::NodeId(0), 0),
                                              std::make_pair(gum::NodeId(1), 0)};
      struct_constraint.setSliceOrder(slices);
      struct_constraint.setDefaultSlice(1);

      gum::learning::ParamEstimatorML estimator(filter, modalities, apriori);

      gum::learning::GraphChangesGenerator4DiGraph< decltype(struct_constraint) >
        op_set(struct_constraint);

      gum::learning::GraphChangesSelector4DiGraph< decltype(score),
                                                   decltype(struct_constraint),
                                                   decltype(op_set) >
        selector(score, struct_constraint, op_set);

      gum::learning::LocalSearchWithTabuList search;

      try {
        gum::Timer timer;
        gum::DAG   bn = search.learnStructure(selector, modalities);
        std::cout << timer.step() << " : " << std::endl;
        std::cout << bn << std::endl;
      } catch (gum::Exception& e) {
        GUM_SHOWERROR(e);
      }
    }

    void xtest_alarm3() {
      gum::learning::DatabaseFromCSV
    database(GET_RESSOURCES_PATH("csv/alarm.csv"));

      gum::learning::DBRowTranslatorSet< gum::learning::DBCellTranslator< 1, 1 > >
        translators;
      translators.insertTranslator(
        gum::learning::CellTranslatorCompactIntId(), 0, 37);

      gum::learning::FilteredRowGeneratorSet< gum::learning::RowGeneratorIdentity >
        generators;
      generators.insertGenerator();
      auto filter =
        gum::learning::make_DB_row_filter(database, translators, generators);

      std::vector< gum::Size > modalities = filter.modalities();

      gum::learning::AprioriSmoothing apriori;
      apriori.setWeight(0);
      gum::learning::ScoreK2 score(filter, modalities, apriori);

      gum::learning::StructuralConstraintSetStatic<
        gum::learning::StructuralConstraintDAG,
        gum::learning::StructuralConstraintIndegree,
        gum::learning::StructuralConstraintSliceOrder,
        gum::learning::StructuralConstraintTabuList >
        struct_constraint;

      struct_constraint.setMaxIndegree(4);
      struct_constraint.setTabuListSize(100);

      gum::NodeProperty< gum::NodeId > slices{std::make_pair(gum::NodeId(0), 0),
                                              std::make_pair(gum::NodeId(1), 0)};
      struct_constraint.setSliceOrder(slices);
      struct_constraint.setDefaultSlice(1);

      apriori.setWeight(0);
      gum::learning::ParamEstimatorML estimator(filter, modalities, apriori);

      gum::learning::GraphChangesGenerator4DiGraph< decltype(struct_constraint) >
        op_set(struct_constraint);

      gum::learning::GraphChangesSelector4DiGraph< decltype(score),
                                                   decltype(struct_constraint),
                                                   decltype(op_set) >
        selector(score, struct_constraint, op_set);

      gum::learning::LocalSearchWithTabuList search;

      try {
        gum::Timer timer;
        gum::DAG   bn = search.learnStructure(selector, modalities);
        std::cout << timer.step() << " : " << std::endl;
        std::cout << bn << std::endl;
      } catch (gum::Exception& e) {
        GUM_SHOWERROR(e);
      }
    }
    */
  };

} /* namespace gum_tests */
