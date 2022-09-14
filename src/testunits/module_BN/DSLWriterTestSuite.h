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


#include <iostream>
#include <string>

#include <gumtest/AgrumTestSuite.h>
#include <gumtest/testsuite_utils.h>

#include <agrum/BN/BayesNet.h>
#include <agrum/BN/io/DSL/DSLWriter.h>
#include <agrum/tools/variables/labelizedVariable.h>

// The graph used for the tests:
//          1   2_          1 -> 3
//         / \ / /          1 -> 4
//        3   4 /           3 -> 5
//         \ / /            4 -> 5
//          5_/             2 -> 4
//                          2 -> 5

namespace gum_tests {

  class DSLWriterTestSuite: public CxxTest::TestSuite {
    public:
    gum::BayesNet< double >* bn;
    gum::NodeId              i1, i2, i3, i4, i5;

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

      fill(*bn);
    }

    void tearDown() { delete bn; }

    void testConstuctor() {
      gum::DSLWriter< double >* writer = nullptr;
      TS_GUM_ASSERT_THROWS_NOTHING(writer = new gum::DSLWriter< double >())
      delete writer;
    }

    void testWriter_ostream() {
      gum::DSLWriter< double > writer;
      // Uncomment this to check the output
      // TS_GUM_ASSERT_THROWS_NOTHING(writer.write(std::cerr, *bn))
    }

    void testWriter_string() {
      gum::DSLWriter< double > writer;
      std::string              file = GET_RESSOURCES_PATH("outputs/DSLWriter_TestFile.txt");
      TS_GUM_ASSERT_THROWS_NOTHING(writer.write(file, *bn))
    }

    void testSyntaxicError() {
      gum::DSLWriter< double > writer;
      {
        TS_ASSERT(!writer.isModificationAllowed())
        std::string file = GET_RESSOURCES_PATH("outputs/shouldNotBeWrittenDSL.txt");
        {
          auto bn = gum::BayesNet< double >::fastPrototype("A->Hello World !->c");
          TS_ASSERT_THROWS(writer.write(file, bn), gum::FatalError&)
        }
        {
          auto bn = gum::BayesNet< double >::fastPrototype("A->Hello World->c");
          TS_ASSERT_THROWS(writer.write(file, bn), gum::FatalError&)
        }
        {
          auto bn = gum::BayesNet< double >::fastPrototype("A->HelloWorld!->c");
          TS_ASSERT_THROWS(writer.write(file, bn), gum::FatalError&)
        }
      }
      {
        writer.setAllowModification(true);
        TS_ASSERT(writer.isModificationAllowed())
        std::string file = GET_RESSOURCES_PATH("outputs/shouldBeWrittenDSL.txt");
        {
          auto bn = gum::BayesNet< double >::fastPrototype("A->Hello World !->c");
          TS_GUM_ASSERT_THROWS_NOTHING(writer.write(file, bn))
        }
        {
          auto bn = gum::BayesNet< double >::fastPrototype("A->Hello World->c");
          TS_GUM_ASSERT_THROWS_NOTHING(writer.write(file, bn))
        }
        {
          auto bn = gum::BayesNet< double >::fastPrototype("A->HelloWorld!->c");
          TS_GUM_ASSERT_THROWS_NOTHING(writer.write(file, bn))
        }
      }
    }

    private:
    // Builds a BN to test the inference
    void fill(gum::BayesNet< double >& bn) {
      bn.cpt(i1).fillWith({0.2, 0.8});
      bn.cpt(i2).fillWith({0.3, 0.7});
      bn.cpt(i3).fillWith({0.1, 0.9, 0.9, 0.1});
      bn.cpt(i4).fillWith(   // clang-format off
              {0.4, 0.6,
               0.5, 0.5,
               0.5, 0.5,
               1.0, 0.0} );   // clang-format on
      bn.cpt(i5).fillWith(                             // clang-format off
              {0.3, 0.6, 0.1,
               0.5, 0.5, 0.0,
               0.5, 0.5, 0.0,
               1.0, 0.0, 0.0,
               0.4, 0.6, 0.0,
               0.5, 0.5, 0.0,
               0.5, 0.5, 0.0,
               0.0, 0.0, 1.0} );  // clang-format
                                                                      // on
    }
  };
}
