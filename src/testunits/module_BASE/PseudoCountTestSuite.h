/**
 *
 *   Copyright (c) 2005-2023 by Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
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
#include <agrum/tools/database/DBRowGeneratorParser.h>
#include <agrum/tools/database/DBInitializerFromCSV.h>
#include <agrum/tools/database/databaseTable.h>
#include <agrum/tools/database/DBTranslatorSet.h>
#include <agrum/BN/learning/priors/noPrior.h>
#include <agrum/BN/learning/priors/smoothingPrior.h>
#include <agrum/tools/stattests/pseudoCount.h>

namespace gum_tests {

  class [[maybe_unused]] PseudoCountTestSuite: public CxxTest::TestSuite {
    public:
    GUM_ACTIVE_TEST(_simple_counts) {
      gum::learning::DBInitializerFromCSV initializer(GET_RESSOURCES_PATH("csv/minimal.csv"));
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
      {
        gum::learning::NoPrior     prior(database);
        gum::learning::PseudoCount counts(parser, prior);

        TS_ASSERT_EQUALS(counts.get({1}), std::vector< double >({4, 3}))
        TS_ASSERT_EQUALS(counts.get({2}), std::vector< double >({3, 2, 2}))
        TS_ASSERT_EQUALS(counts.get({0, 2}), std::vector< double >({2, 1, 1, 1, 0, 2}))
      }
      {
        gum::learning::SmoothingPrior prior(database);
        gum::learning::PseudoCount    counts(parser, prior);

        TS_ASSERT_EQUALS(counts.get({1}), std::vector< double >({5, 4}))
        TS_ASSERT_EQUALS(counts.get({2}), std::vector< double >({4, 3, 3}))
        TS_ASSERT_EQUALS(counts.get({0, 2}), std::vector< double >({3, 2, 2, 2, 1, 3}))
      }
      {
        gum::learning::SmoothingPrior prior(database);
        prior.setWeight(0.1);
        gum::learning::PseudoCount counts(parser, prior);

        TS_ASSERT_EQUALS(counts.get({1}), std::vector< double >({4.1, 3.1}))
        TS_ASSERT_EQUALS(counts.get({2}), std::vector< double >({3.1, 2.1, 2.1}))
        TS_ASSERT_EQUALS(counts.get({0, 2}), std::vector< double >({2.1, 1.1, 1.1, 1.1, 0.1, 2.1}))
      }
    }
  };
} /* namespace gum_tests */
