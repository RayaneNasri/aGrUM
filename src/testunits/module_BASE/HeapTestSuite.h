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

#include <agrum/tools/core/heap.h>
#include <agrum/tools/core/set.h>
namespace gum_tests {

  class HeapTestSuite: public CxxTest::TestSuite {
    public:
    void testConstructor() {
      gum::Heap< int > heap1;

      heap1.insert(8);
      heap1.emplace(10);
      heap1.insert(2);
      heap1.insert(23);
      heap1.insert(24);
      heap1.insert(10);
      heap1.insert(10);

      TS_ASSERT_EQUALS(heap1.top(), 2)
      heap1.pop();
      TS_ASSERT_EQUALS(heap1.top(), 8)
      heap1.eraseTop();
      TS_ASSERT_EQUALS(heap1.top(), 10)
      heap1.eraseTop();
      TS_ASSERT_EQUALS(heap1.top(), 10)

      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)4)
      TS_ASSERT_EQUALS(heap1.empty(), false)
      TS_ASSERT_EQUALS(heap1.contains(8), false)
      TS_ASSERT_EQUALS(heap1.contains(23), true)
      TS_ASSERT_EQUALS(heap1.contains(10), true)

      heap1.erase(10);
      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)3)
      TS_GUM_ASSERT_THROWS_NOTHING(heap1.erase(150));

      heap1.eraseByPos(0);
      TS_ASSERT_EQUALS(heap1.top(), 23)
    }

    void testConstructor2() {
      gum::Heap< int, std::greater< int > > heap1;

      heap1.insert(8);
      heap1.insert(10);
      heap1.insert(2);
      heap1.insert(23);
      heap1.insert(24);
      heap1.insert(10);
      heap1.insert(10);

      TS_ASSERT_EQUALS(heap1.top(), 24)
      heap1.pop();
      TS_ASSERT_EQUALS(heap1.top(), 23)
      heap1.eraseTop();
      TS_ASSERT_EQUALS(heap1.top(), 10)
      heap1.eraseTop();
      TS_ASSERT_EQUALS(heap1.top(), 10)

      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)4)
      TS_ASSERT_EQUALS(heap1.empty(), false)
      TS_ASSERT_EQUALS(heap1.contains(23), false)
      TS_ASSERT_EQUALS(heap1.contains(2), true)
      TS_ASSERT_EQUALS(heap1.contains(8), true)

      heap1.erase(10);
      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)3)
      TS_GUM_ASSERT_THROWS_NOTHING(heap1.erase(150));

      heap1.eraseByPos(0);
      TS_ASSERT_EQUALS(heap1.top(), 8)
    }

    void testMoves() {
      gum::Heap< int, std::greater< int > > heap1{1, 5, 2, 4};
      gum::Heap< int, std::greater< int > > heap2{3, 9};
      gum::Heap< int, std::greater< int > > heap3{1, 10, 20};

      gum::Heap< int, std::greater< int > > heap4 = std::move(heap3);
      heap3                                       = std::move(heap2);
      heap2                                       = std::move(heap1);
      TS_ASSERT_EQUALS(heap2.size(), (gum::Size)4)
      TS_ASSERT_EQUALS(heap2.top(), 5)
    }

    void testCopy() {
      gum::Heap< int > heap1;
      heap1.insert(8);
      heap1.insert(10);
      heap1.insert(2);
      heap1.insert(23);
      heap1.insert(24);
      heap1.insert(10);
      heap1.insert(10);

      gum::Heap< int > heap2;
      heap2.insert(8);
      heap2.insert(50);
      heap2.insert(20);

      heap2 = heap1;
      TS_ASSERT_EQUALS(heap2.size(), (gum::Size)heap1.size())

      gum::Heap< int > heap3(heap1);
      TS_ASSERT_EQUALS(heap3.size(), (gum::Size)heap1.size())
    }

    void testErase() {
      gum::Heap< int > heap1;

      heap1.insert(8);
      heap1.insert(10);
      heap1.insert(2);
      heap1.insert(23);
      heap1.insert(24);
      heap1.insert(10);
      heap1.insert(10);

      heap1.eraseByPos(0);
      TS_ASSERT_EQUALS(heap1.top(), 8)
      TS_GUM_ASSERT_THROWS_NOTHING(heap1.eraseByPos(150));
      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)6)
      heap1.eraseByPos(4);
      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)5)
      heap1.eraseByPos(4);
      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)4)
      TS_ASSERT_EQUALS(heap1.contains(24), false)
    }

    class SetCmp {
      public:
      bool operator()(const gum::Set< int >& x, const gum::Set< int >& y) const {
        return x.size() < y.size();
      }
    };

    void testObj() {
      gum::Heap< gum::Set< int >, SetCmp > heap1;

      gum::Set< int > s1, s2, s3, s4, s5;
      s1.insert(1);
      s2.insert(1);
      s2.insert(2);
      s3.insert(1);
      s3.insert(2);
      s3.insert(3);
      s4.insert(1);
      s4.insert(2);
      s4.insert(3);
      s4.insert(4);
      s5.insert(1);
      s5.insert(2);
      s5.insert(3);
      s5.insert(4);
      s5.insert(5);

      heap1.insert(s4);
      heap1.insert(s2);
      heap1.insert(s1);
      heap1.insert(s3);
      heap1.insert(s2);
      heap1.insert(s2);
      heap1.insert(s5);

      TS_ASSERT_EQUALS(heap1.top(), s1)
      heap1.pop();
      TS_ASSERT_EQUALS(heap1.top(), s2)
      heap1.eraseTop();
      TS_ASSERT_EQUALS(heap1.top(), s2)
      heap1.eraseTop();
      TS_ASSERT_EQUALS(heap1.top(), s2)

      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)4)
      TS_ASSERT_EQUALS(heap1.empty(), false)
      TS_ASSERT_EQUALS(heap1.contains(s1), false)
      TS_ASSERT_EQUALS(heap1.contains(s4), true)
      TS_ASSERT_EQUALS(heap1.contains(s5), true)

      gum::Set< int > s6;
      heap1.erase(s2);
      TS_ASSERT_EQUALS(heap1.size(), (gum::Size)3)
      TS_GUM_ASSERT_THROWS_NOTHING(heap1.erase(s6));

      heap1.eraseByPos(0);
      TS_ASSERT_EQUALS(heap1.top(), s4)
    }
  };

} /* namespace gum_tests */
