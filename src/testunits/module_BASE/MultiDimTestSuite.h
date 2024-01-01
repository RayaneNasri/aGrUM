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


#include <sstream>

#include <gumtest/AgrumTestSuite.h>
#include <gumtest/utils.h>

#include <agrum/tools/multidim/implementations/multiDimArray.h>
#include <agrum/tools/multidim/instantiation.h>
#include <agrum/tools/variables/labelizedVariable.h>

namespace gum_tests {

  class [[maybe_unused]] MultidimTestSuite: public CxxTest::TestSuite {
    public:
    GUM_ACTIVE_TEST(Creation) {
      gum::MultiDimArray< double > m;
      TS_ASSERT_EQUALS(m.nbrDim(), (gum::Size)0)
      TS_ASSERT_EQUALS(m.domainSize(), (gum::Size)1)

      gum::LabelizedVariable a("a", "", 4), b("b", "", 5);
      m << a;
      TS_ASSERT_EQUALS(m.nbrDim(), (gum::Size)1)
      TS_ASSERT_EQUALS(m.domainSize(), (gum::Size)4)
      m << b;
      TS_ASSERT_EQUALS(m.nbrDim(), (gum::Size)2)
      TS_ASSERT_EQUALS(m.domainSize(), (gum::Size)20)

      gum::MultiDimArray< double > mm = m;
      TS_ASSERT_EQUALS(mm.nbrDim(), (gum::Size)2)
      TS_ASSERT_EQUALS(mm.domainSize(), (gum::Size)20)
    }

    GUM_ACTIVE_TEST(MemoryCrash) {
      gum::MultiDimArray< double > m;
      gum::LabelizedVariable*      v[100];

      for (int i = 0; i < 100; i++) {
        std::string name = "x";
        name.append(std::to_string(i));
        v[i] = new gum::LabelizedVariable(name, "x");
      }

      TS_ASSERT_THROWS(feedMultiDimUntilOverflow(v, m), const gum::OutOfBounds&)

      for (int i = 0; i < 100; i++)
        delete (v[i]);
    }

    private:
    void feedMultiDimUntilOverflow(gum::LabelizedVariable* v[], gum::MultiDimArray< double >& t) {
      t.beginMultipleChanges();

      for (int i = 0; i < 100; i++)
        t << *v[i];

      t.endMultipleChanges();
    }
  };
}   // namespace gum_tests
