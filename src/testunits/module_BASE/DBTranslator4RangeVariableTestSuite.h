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


#include <gumtest/AgrumTestSuite.h>
#include <gumtest/testsuite_utils.h>
#include <ressources/include/countedAlloc.h>
#include <ressources/include/poolAlloc.h>
#include <iostream>

#include <agrum/tools/database/DBTranslator4RangeVariable.h>

namespace gum_tests {

  class DBTranslator4RangeVariableTestSuite: public CxxTest::TestSuite {
    public:
    void test_trans1() {
      gum::learning::DBTranslator4RangeVariable<> translator;
      TS_ASSERT(translator.isLossless())
      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("3"));
      TS_ASSERT_EQUALS(translator.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");

      TS_ASSERT_EQUALS(translator.missingValue().discr_val,
                       std::numeric_limits< std::size_t >::max());

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("5"));
      TS_ASSERT_EQUALS(translator.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("1"));
      TS_ASSERT_EQUALS(translator.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "2");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("-1"));
      TS_ASSERT_EQUALS(translator.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "2");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "-1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{6}}),
                       "0");

      std::vector< std::string > missing{"2", "N/A", "20", "4", "xxx"};

      gum::learning::DBTranslator4RangeVariable<> translator2(missing);
      TS_ASSERT_EQUALS(translator2.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator2.translate("2").discr_val,
                       std::numeric_limits< std::size_t >::max());
      TS_ASSERT(translator2.translate("N/A").discr_val
                == std::numeric_limits< std::size_t >::max());
      TS_ASSERT(translator2.translate("xxx").discr_val
                == std::numeric_limits< std::size_t >::max());
      TS_ASSERT_THROWS(translator2.translate("yyy"), gum::TypeError)
      TS_ASSERT_THROWS(translator2.translate("1"), gum::OperationNotAllowed)
      TS_ASSERT_EQUALS(translator2.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator2.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");

      gum::Set< std::string > possible_translated_miss{"N/A", "xxx"};
      TS_ASSERT(possible_translated_miss.exists(translator2.translateBack(
         gum::learning::DBTranslatedValue{std::numeric_limits< std::size_t >::max()})));
      TS_ASSERT_EQUALS(translator2.needsReordering(), false)
      const auto new_order = translator2.reorder();
      TS_ASSERT_EQUALS(new_order.size(), (gum::Size)0)

      gum::RangeVariable                          var("X2", "", 2, 3);
      gum::learning::DBTranslator4RangeVariable<> translator3(var, missing, true);
      TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "2");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "3");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "4");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "5");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "0");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "1");
      TS_ASSERT(translator3.needsReordering())
      const auto new_order3 = translator3.reorder();
      TS_ASSERT_EQUALS(new_order3[0], (std::size_t)2)
      TS_ASSERT_EQUALS(new_order3[1], (std::size_t)3)
      TS_ASSERT_EQUALS(new_order3[2], (std::size_t)4)
      TS_ASSERT_EQUALS(new_order3[3], (std::size_t)5)
      TS_ASSERT_EQUALS(new_order3[4], (std::size_t)0)
      TS_ASSERT_EQUALS(new_order3[5], (std::size_t)1)

      TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator3.translate("3").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)5)


      std::vector< std::string >                  missing2;
      gum::learning::DBTranslator4RangeVariable<> translator4(var, missing2, true, 3);
      TS_ASSERT_EQUALS(translator4.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_THROWS(translator4.translate("5"), gum::SizeError)
      TS_ASSERT_EQUALS(translator4.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_THROWS(translator4.translate("0"), gum::SizeError)
      TS_ASSERT_THROWS(translator4.translate("1"), gum::SizeError)
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "2");
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "3");
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "4");
      TS_ASSERT_THROWS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       gum::UnknownLabelInDatabase);
    }


    void test_trans2() {
      gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > translator;
      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("3"));
      TS_ASSERT_EQUALS(translator.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("5"));
      TS_ASSERT_EQUALS(translator.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("1"));
      TS_ASSERT_EQUALS(translator.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "2");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("-1"));
      TS_ASSERT_EQUALS(translator.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "2");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "-1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{6}}),
                       "0");

      std::vector< std::string > missing{"2", "N/A", "20", "4", "xxx"};

      gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > translator2(missing);
      TS_ASSERT_EQUALS(translator2.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator2.translate("2").discr_val,
                       std::numeric_limits< std::size_t >::max());
      TS_ASSERT(translator2.translate("N/A").discr_val
                == std::numeric_limits< std::size_t >::max());
      TS_ASSERT(translator2.translate("xxx").discr_val
                == std::numeric_limits< std::size_t >::max());
      TS_ASSERT_THROWS(translator2.translate("yyy"), gum::TypeError)
      TS_ASSERT_THROWS(translator2.translate("1"), gum::OperationNotAllowed)
      TS_ASSERT_EQUALS(translator2.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator2.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");

      gum::Set< std::string > possible_translated_miss{"N/A", "xxx"};
      TS_ASSERT(possible_translated_miss.exists(translator2.translateBack(
         gum::learning::DBTranslatedValue{std::numeric_limits< std::size_t >::max()})));
      TS_ASSERT_EQUALS(translator2.needsReordering(), false)
      const auto new_order = translator2.reorder();
      TS_ASSERT_EQUALS(new_order.size(), (gum::Size)0)

      gum::RangeVariable                                             var("X2", "", 2, 3);
      gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > translator3(var,
                                                                                 missing,
                                                                                 true);
      TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "2");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "3");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "4");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "5");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "0");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "1");
      TS_ASSERT(translator3.needsReordering())
      const auto new_order3 = translator3.reorder();
      TS_ASSERT_EQUALS(new_order3[0], (std::size_t)2)
      TS_ASSERT_EQUALS(new_order3[1], (std::size_t)3)
      TS_ASSERT_EQUALS(new_order3[2], (std::size_t)4)
      TS_ASSERT_EQUALS(new_order3[3], (std::size_t)5)
      TS_ASSERT_EQUALS(new_order3[4], (std::size_t)0)
      TS_ASSERT_EQUALS(new_order3[5], (std::size_t)1)

      TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "0");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "1");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "2");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "3");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "4");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "5");


      std::vector< std::string >                                     missing2;
      gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > translator4(var,
                                                                                 missing2,
                                                                                 true,
                                                                                 3);
      TS_ASSERT_EQUALS(translator4.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_THROWS(translator4.translate("5"), gum::SizeError)
      TS_ASSERT_EQUALS(translator4.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_THROWS(translator4.translate("0"), gum::SizeError)
      TS_ASSERT_THROWS(translator4.translate("1"), gum::SizeError)
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "2");
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "3");
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "4");
      TS_ASSERT_THROWS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       gum::UnknownLabelInDatabase);
    }


    void test_trans4() {
      {
        gum::RangeVariable var("X1", "", 2, 3);

        std::vector< std::string >                  missing;
        gum::learning::DBTranslator4RangeVariable<> translator(var, missing, true);
        TS_ASSERT_EQUALS(translator.translate("2").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator.translate("5").discr_val, (std::size_t)3)
        TS_ASSERT_EQUALS(translator.translate("4").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator.translate("0").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator.translate("1").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                         "2");
        TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                         "3");
        TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                         "4");
        TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                         "5");
        TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                         "0");
        TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                         "1");
        TS_ASSERT(translator.needsReordering())
        const auto new_order = translator.reorder();
        TS_ASSERT_EQUALS(new_order[0], (std::size_t)2)
        TS_ASSERT_EQUALS(new_order[1], (std::size_t)3)
        TS_ASSERT_EQUALS(new_order[2], (std::size_t)4)
        TS_ASSERT_EQUALS(new_order[3], (std::size_t)5)
        TS_ASSERT_EQUALS(new_order[4], (std::size_t)0)
        TS_ASSERT_EQUALS(new_order[5], (std::size_t)1)
        TS_ASSERT_EQUALS(translator.variable()->toString(), "X1:Range([0,5])")

        gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > translator2(var,
                                                                                   missing,
                                                                                   true);
        TS_ASSERT_EQUALS(translator2.translate("2").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator2.translate("5").discr_val, (std::size_t)3)
        TS_ASSERT_EQUALS(translator2.translate("4").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator2.translate("0").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator2.translate("1").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "2");
        TS_ASSERT_EQUALS(
           translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "3");
        TS_ASSERT_EQUALS(
           translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "4");
        TS_ASSERT_EQUALS(
           translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "5");
        TS_ASSERT_EQUALS(
           translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "0");
        TS_ASSERT_EQUALS(
           translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "1");
        TS_ASSERT(translator2.needsReordering())
        const auto new_order2 = translator2.reorder();
        TS_ASSERT_EQUALS(new_order2[0], (std::size_t)2)
        TS_ASSERT_EQUALS(new_order2[1], (std::size_t)3)
        TS_ASSERT_EQUALS(new_order2[2], (std::size_t)4)
        TS_ASSERT_EQUALS(new_order2[3], (std::size_t)5)
        TS_ASSERT_EQUALS(new_order2[4], (std::size_t)0)
        TS_ASSERT_EQUALS(new_order2[5], (std::size_t)1)
        TS_ASSERT_EQUALS(translator2.variable()->toString(), "X1:Range([0,5])")

        gum::learning::DBTranslator4RangeVariable<> translator3(
           translator,
           std::allocator< gum::learning::DBTranslatedValue >());
        TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");

        gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > translator4(
           translator2,
           DebugCountedAlloc< gum::learning::DBTranslatedValue >());
        TS_ASSERT_EQUALS(translator4.translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator4.translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator4.translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator4.translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator4.translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");

        gum::learning::DBTranslator4RangeVariable<> translator5(std::move(translator3));
        TS_ASSERT_EQUALS(translator5.translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator5.translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator5.translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator5.translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator5.translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");

        gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > translator6(
           std::move(translator4));
        TS_ASSERT_EQUALS(translator6.translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator6.translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator6.translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator6.translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator6.translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator6.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator6.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator6.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator6.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator6.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator6.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");

        gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc >* translator7
           = translator6.clone();
        TS_ASSERT_EQUALS(translator7->translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator7->translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator7->translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator7->translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator7->translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator7->translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator7->translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator7->translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator7->translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator7->translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator7->translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");

        DebugCountedAlloc< gum::learning::DBTranslator4RangeVariable< DebugCountedAlloc > >
           allocator(translator7->getAllocator());
        allocator.destroy(translator7);
        allocator.deallocate(translator7, 1);

        translator4 = translator6;
        TS_ASSERT_EQUALS(translator4.translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator4.translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator4.translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator4.translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator4.translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");

        translator3 = translator5;
        TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");

        translator5 = std::move(translator3);
        TS_ASSERT_EQUALS(translator5.translate("0").discr_val, (std::size_t)0)
        TS_ASSERT_EQUALS(translator5.translate("1").discr_val, (std::size_t)1)
        TS_ASSERT_EQUALS(translator5.translate("2").discr_val, (std::size_t)2)
        TS_ASSERT_EQUALS(translator5.translate("4").discr_val, (std::size_t)4)
        TS_ASSERT_EQUALS(translator5.translate("5").discr_val, (std::size_t)5)
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
           "0");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
           "1");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
           "2");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
           "3");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
           "4");
        TS_ASSERT_EQUALS(
           translator5.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
           "5");
      }

      TS_ASSERT_EQUALS(CountedAlloc::hasMemoryLeak(), false)
    }


    void xtest_trans5() {
      LearningAlloc< double >                                    alloc(10000);
      gum::learning::DBTranslator4RangeVariable< LearningAlloc > translator(1000, alloc);
      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("3"));
      TS_ASSERT_EQUALS(translator.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("5"));
      TS_ASSERT_EQUALS(translator.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("1"));
      TS_ASSERT_EQUALS(translator.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "2");

      TS_GUM_ASSERT_THROWS_NOTHING(translator.translate("-1"));
      TS_ASSERT_EQUALS(translator.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "2");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "-1");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{6}}),
                       "0");

      const auto& allocator = translator.getAllocator();
      TS_ASSERT_EQUALS(allocator, alloc)

      std::vector< std::string > missing{"2", "N/A", "20", "4", "xxx"};

      gum::learning::DBTranslator4RangeVariable< LearningAlloc > translator2(missing);
      TS_ASSERT_EQUALS(translator2.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator2.translate("2").discr_val,
                       std::numeric_limits< std::size_t >::max());
      TS_ASSERT(translator2.translate("N/A").discr_val
                == std::numeric_limits< std::size_t >::max());
      TS_ASSERT(translator2.translate("xxx").discr_val
                == std::numeric_limits< std::size_t >::max());
      TS_ASSERT_THROWS(translator2.translate("yyy"), gum::TypeError)
      TS_ASSERT_THROWS(translator2.translate("1"), gum::OperationNotAllowed)
      TS_ASSERT_EQUALS(translator2.translate("5").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator2.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "3");
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "4");
      TS_ASSERT_EQUALS(translator2.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "5");

      gum::Set< std::string > possible_translated_miss{"N/A", "xxx"};
      TS_ASSERT(possible_translated_miss.exists(translator2.translateBack(
         gum::learning::DBTranslatedValue{std::numeric_limits< std::size_t >::max()})));
      TS_ASSERT_EQUALS(translator2.needsReordering(), false)
      const auto new_order = translator2.reorder();
      TS_ASSERT_EQUALS(new_order[0], (std::size_t)0)
      TS_ASSERT_EQUALS(new_order[1], (std::size_t)1)
      TS_ASSERT_EQUALS(new_order[2], (std::size_t)2)

      gum::RangeVariable                                         var("X2", "", 2, 3);
      gum::learning::DBTranslator4RangeVariable< LearningAlloc > translator3(var, missing, true);
      TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "2");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "3");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "4");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "5");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "0");
      TS_ASSERT_EQUALS(translator3.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "1");
      TS_ASSERT(translator3.needsReordering())
      const auto new_order3 = translator3.reorder();
      TS_ASSERT_EQUALS(new_order3[0], (std::size_t)2)
      TS_ASSERT_EQUALS(new_order3[1], (std::size_t)3)
      TS_ASSERT_EQUALS(new_order3[2], (std::size_t)4)
      TS_ASSERT_EQUALS(new_order3[3], (std::size_t)5)
      TS_ASSERT_EQUALS(new_order3[4], (std::size_t)0)
      TS_ASSERT_EQUALS(new_order3[5], (std::size_t)1)

      std::vector< std::string >                                 missing2;
      gum::learning::DBTranslator4RangeVariable< LearningAlloc > translator4(var,
                                                                             missing2,
                                                                             true,
                                                                             3);
      TS_ASSERT_EQUALS(translator4.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_THROWS(translator4.translate("5"), gum::SizeError)
      TS_ASSERT_EQUALS(translator4.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_THROWS(translator4.translate("0"), gum::SizeError)
      TS_ASSERT_THROWS(translator4.translate("1"), gum::SizeError)
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "2");
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "3");
      TS_ASSERT_EQUALS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "4");
      TS_ASSERT_THROWS(translator4.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       gum::UnknownLabelInDatabase);

      translator4 = translator3;
      TS_ASSERT_EQUALS(translator4.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator4.translate("5").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator4.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator4.translate("0").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator4.translate("1").discr_val, (std::size_t)5)

      translator4 = std::move(translator3);
      TS_ASSERT_EQUALS(translator4.translate("2").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator4.translate("5").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator4.translate("4").discr_val, (std::size_t)2)
      TS_ASSERT_EQUALS(translator4.translate("0").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator4.translate("1").discr_val, (std::size_t)5)

      gum::learning::DBTranslator4RangeVariable< LearningAlloc > translator5(translator);
      TS_ASSERT_EQUALS(translator5.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator5.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator5.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator5.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator5.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator5.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator5.translate("5").discr_val, (std::size_t)2)

      gum::learning::DBTranslator4RangeVariable< LearningAlloc > translator6(std::move(translator));
      TS_ASSERT_EQUALS(translator6.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator6.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator6.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator6.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator6.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator6.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator6.translate("5").discr_val, (std::size_t)2)

      gum::learning::DBTranslator4RangeVariable< LearningAlloc >* translator7 = translator6.clone();
      TS_ASSERT_EQUALS(translator7->translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator7->translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator7->translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator7->translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator7->translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator7->translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator7->translate("5").discr_val, (std::size_t)2)

      LearningAlloc< gum::learning::DBTranslator4RangeVariable< LearningAlloc > > allocator2(
         translator7->getAllocator());
      allocator2.destroy(translator7);
      allocator2.deallocate(translator7, 1);

      translator5 = translator6;
      TS_ASSERT_EQUALS(translator5.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator5.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator5.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator5.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator5.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator5.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator5.translate("5").discr_val, (std::size_t)2)

      translator3 = translator5;
      TS_ASSERT_EQUALS(translator3.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator3.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator3.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator3.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator3.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator3.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator3.translate("5").discr_val, (std::size_t)2)

      translator5 = std::move(translator3);
      TS_ASSERT_EQUALS(translator5.translate("-1").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS(translator5.translate("0").discr_val, (std::size_t)6)
      TS_ASSERT_EQUALS(translator5.translate("1").discr_val, (std::size_t)3)
      TS_ASSERT_EQUALS(translator5.translate("2").discr_val, (std::size_t)4)
      TS_ASSERT_EQUALS(translator5.translate("3").discr_val, (std::size_t)0)
      TS_ASSERT_EQUALS(translator5.translate("4").discr_val, (std::size_t)1)
      TS_ASSERT_EQUALS(translator5.translate("5").discr_val, (std::size_t)2)
    }


    void test_trans6() {
      gum::RangeVariable         var("X1", "", 2, 4);
      std::vector< std::string > missing{"N/A", "3", "toto"};

      gum::learning::DBTranslator4RangeVariable<> translator(var, missing, true);
      TS_ASSERT_EQUALS(translator.needsReordering(), false)
      TS_ASSERT_EQUALS(translator.domainSize(), (gum::Size)3)

      TS_ASSERT_EQUALS((translator << "7").discr_val, (std::size_t)5)
      TS_ASSERT_EQUALS((translator >> gum::learning::DBTranslatedValue{std::size_t{5}}), "7")
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{0}}),
                       "2");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{1}}),
                       "3");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{2}}),
                       "4");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{3}}),
                       "5");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{4}}),
                       "6");
      TS_ASSERT_EQUALS(translator.translateBack(gum::learning::DBTranslatedValue{std::size_t{5}}),
                       "7");
      TS_ASSERT((translator.translateBack(
                    gum::learning::DBTranslatedValue{std::numeric_limits< std::size_t >::max()})
                 == "N/A")
                || (translator.translateBack(
                       gum::learning::DBTranslatedValue{std::numeric_limits< std::size_t >::max()})
                    == "toto"));

      gum::Set< std::string > missing_kept{"N/A", "toto"};
      TS_ASSERT_EQUALS(translator.missingSymbols(), missing_kept)
    }
  };

} /* namespace gum_tests */
