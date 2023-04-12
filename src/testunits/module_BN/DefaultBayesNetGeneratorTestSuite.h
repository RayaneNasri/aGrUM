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


#include <iostream>
#include <string>
#include <vector>

#include <gumtest/AgrumTestSuite.h>
#include <gumtest/testsuite_utils.h>

#include <agrum/BN/generator/simpleBayesNetGenerator.h>
#include <agrum/BN/generator/simpleCPTGenerator.h>
#include <agrum/BN/inference/ShaferShenoyInference.h>
#include <agrum/BN/inference/lazyPropagation.h>
#include <agrum/tools/core/exceptions.h>
#include <agrum/tools/core/set.h>

namespace gum_tests {

  class [[maybe_unused]] SimpleBayesNetGeneratorTestSuite: public CxxTest::TestSuite {
    public:
    void setUp() {}

    void tearDown() {}

    GUM_ACTIVE_TEST(CreationDeletion_1) {
      gum::SimpleBayesNetGenerator< double >* gen = nullptr;

      TS_GUM_ASSERT_THROWS_NOTHING(gen = new gum::SimpleBayesNetGenerator< double >(10, 20))
      TS_GUM_ASSERT_THROWS_NOTHING(delete gen)
    }

    GUM_ACTIVE_TEST(CreationDeletion_2) {
      gum::SimpleBayesNetGenerator< double >* gen = nullptr;

      TS_GUM_ASSERT_THROWS_NOTHING(gen = new gum::SimpleBayesNetGenerator< double >(10, 20))
      TS_GUM_ASSERT_THROWS_NOTHING(delete gen)
    }

    GUM_ACTIVE_TEST(GenerationFloat_1) {
      gum::SimpleBayesNetGenerator< double > gen(10, 20);
      gum::BayesNet< double >*               bn = new gum::BayesNet< double >();

      TS_GUM_ASSERT_THROWS_NOTHING(gen.generateBN(*bn))

      if (bn != 0) delete bn;
    }

    GUM_ACTIVE_TEST(GenerationFloat_2) {
      gum::SimpleBayesNetGenerator< double > gen(100, 500);
      ;

      gum::BayesNet< double >* bn = new gum::BayesNet< double >();
      gen.generateBN(*bn);
      // Test for cicuits
      std::vector< gum::NodeId > stack;
      gum::Set< gum::NodeId >    passed;
      const gum::DAG&            dag = bn->dag();

      for (const auto node: dag.nodes()) {
        TS_ASSERT_THROWS(dag.directedPath(node, node), const gum::NotFound&)
      }

      if (bn != 0) delete bn;
    }

    GUM_ACTIVE_TEST(GenerationFloat_3) {
      gum::SimpleBayesNetGenerator< double > gen(10, 45);

      gum::BayesNet< double >* bn = new gum::BayesNet< double >();
      gen.generateBN(*bn);
      // Test for inference
      gum::ShaferShenoyInference< double > ssInf(bn);
      TS_GUM_ASSERT_THROWS_NOTHING(ssInf.makeInference())

      if (bn != 0) delete bn;
    }

    GUM_ACTIVE_TEST(GenerationFloat_4) {
      gum::SimpleBayesNetGenerator< double > gen(10, 35);

      gum::BayesNet< double >* bn = new gum::BayesNet< double >();
      gen.generateBN(*bn);
      // Test for inference
      gum::LazyPropagation< double > lazyInf(bn);
      TS_GUM_ASSERT_THROWS_NOTHING(lazyInf.makeInference())

      if (bn != 0) delete bn;
    }

    GUM_ACTIVE_TEST(GenerationFloat_5) {
      gum::SimpleBayesNetGenerator< double > gen(10, 45);
      gum::BayesNet< double >*               bn = 0;

      TS_GUM_ASSERT_THROWS_NOTHING(bn = new gum::BayesNet< double >(); gen.generateBN(*bn);
                                   gum::LazyPropagation< double > lazyInf(bn);
                                   lazyInf.makeInference();
                                   delete bn)
      TS_GUM_ASSERT_THROWS_NOTHING(bn = new gum::BayesNet< double >(); gen.generateBN(*bn);
                                   gum::LazyPropagation< double > lazyInf(bn);
                                   lazyInf.makeInference();
                                   delete bn)
      TS_GUM_ASSERT_THROWS_NOTHING(bn = new gum::BayesNet< double >(); gen.generateBN(*bn);
                                   gum::LazyPropagation< double > lazyInf(bn);
                                   lazyInf.makeInference();
                                   delete bn)
    }

    GUM_ACTIVE_TEST(GenerationDouble_1) {
      gum::SimpleBayesNetGenerator< double > gen(10, 35);
      gum::BayesNet< double >*               bn = new gum::BayesNet< double >();

      TS_GUM_ASSERT_THROWS_NOTHING(gen.generateBN(*bn))

      if (bn != 0) delete bn;
    }

    GUM_ACTIVE_TEST(GenerationDouble_2) {
      // Test for cicuits
      gum::SimpleBayesNetGenerator< double > gen(10, 45);
      gum::BayesNet< double >*               bn = new gum::BayesNet< double >();
      gen.generateBN(*bn);

      std::vector< gum::NodeId > stack;
      gum::Set< gum::NodeId >    passed;
      const gum::DAG&            dag = bn->dag();

      for (const auto node: dag.nodes()) {
        TS_ASSERT_THROWS(dag.directedPath(node, node), const gum::NotFound&)
      }

      if (bn != 0) delete bn;
    }

    GUM_ACTIVE_TEST(GenerationDouble_3) {
      gum::SimpleBayesNetGenerator< double > gen(10, 45);
      gum::BayesNet< double >*               bn = new gum::BayesNet< double >();
      gen.generateBN(*bn);
      // Test for inference
      gum::ShaferShenoyInference< double > ssInf(bn);
      TS_GUM_ASSERT_THROWS_NOTHING(ssInf.makeInference())

      if (bn != 0) delete bn;
    }

    GUM_ACTIVE_TEST(GenerationDouble_4) {
      gum::SimpleBayesNetGenerator< double > gen(10, 45);
      gum::BayesNet< double >*               bn = new gum::BayesNet< double >();
      gen.generateBN(*bn);

      // Test for inference
      gum::LazyPropagation< double > lazyInf(bn);
      TS_GUM_ASSERT_THROWS_NOTHING(lazyInf.makeInference())

      if (bn != 0) delete bn;
    }
  };
}   // namespace gum_tests
