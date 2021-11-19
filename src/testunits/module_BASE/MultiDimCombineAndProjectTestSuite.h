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


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gumtest/AgrumTestSuite.h>
#include <gumtest/testsuite_utils.h>

#include <agrum/tools/core/set.h>
#include <agrum/tools/multidim/potential.h>
#include <agrum/tools/multidim/utils/operators/multiDimCombinationDefault.h>
#include <agrum/tools/multidim/utils/operators/multiDimCombineAndProjectDefault.h>
#include <agrum/tools/multidim/utils/operators/multiDimProjection.h>
#include <agrum/tools/variables/labelizedVariable.h>

namespace gum_tests {

  class MultiDimCombineAndProjectTestSuite: public CxxTest::TestSuite {
    public:
    void testDouble() {
      gum::IScheduleMultiDim<>::resetIdGenerator();
      std::vector< gum::LabelizedVariable* > vars(11);

      for (gum::Idx i = 0; i < 11; ++i) {
        std::stringstream str;
        str << "x" << i;
        std::string s = str.str();
        vars[i]       = new gum::LabelizedVariable(s, s, 4);
      }

      gum::Potential< double > t1, t2, t3, t4, t5, t6;
      t1 << *(vars[0]) << *(vars[1]);
      t2 << *(vars[1]) << *(vars[2]);
      t3 << *(vars[3]) << *(vars[4]) << *(vars[5]);
      t4 << *(vars[4]) << *(vars[7]);
      t5 << *(vars[5]) << *(vars[6]);
      t6 << *(vars[8]) << *(vars[9]);

      randomInitP(t1);
      randomInitP(t2);
      randomInitP(t3);
      randomInitP(t4);
      randomInitP(t5);
      randomInitP(t6);

      gum::MultiDimCombineAndProjectDefault< double, gum::Potential > projcomb(multPot, mySum);

      gum::Set< const gum::Potential< double >* > to_comb;
      to_comb << &t1 << &t2 << &t3 << &t4 << &t5 << &t6;
      gum::Set< const gum::DiscreteVariable* > del_vars;
      del_vars << vars[1] << vars[4] << vars[5] << vars[6] << vars[9] << vars[10];

      try {
        gum::Set< const gum::Potential< double >* > res
           = projcomb.execute(to_comb, del_vars);

        double nb_ops = projcomb.nbOperations(to_comb, del_vars);

        auto yyy = projcomb.memoryUsage(to_comb, del_vars);

        TS_ASSERT_EQUALS(yyy.first, 116 * sizeof(double))
        TS_ASSERT_EQUALS(yyy.second, 36 * sizeof(double))

        TS_ASSERT_EQUALS(nb_ops, 421) // combinations + projections + deletions
        TS_ASSERT_EQUALS(res.size(), (gum::Size)3)

        gum::Set< const gum::Potential< double >* >::const_iterator iter = res.begin();
        const gum::Potential< double >*                             res1 = *iter;
        ++iter;
        const gum::Potential< double >* res2 = *iter;
        ++iter;
        const gum::Potential< double >* res3 = *iter;

        gum::MultiDimCombinationDefault< double, gum::Potential > comb(multPot);
        gum::MultiDimProjection< double, gum::Potential >         proj(mySum);

        gum::Set< const gum::Potential< double >* > comb_set;
        comb_set << &t1 << &t2;
        gum::Set< const gum::DiscreteVariable* > del_vars2;
        del_vars2 << vars[1];
        gum::Potential< double >* tt1 = comb.execute(comb_set);
        gum::Potential< double >* tt2 = proj.execute(*tt1, del_vars2);
        delete tt1;
        TS_ASSERT((*tt2 == *res1) || (*tt2 == *res2) || (*tt2 == *res3))

        delete tt2;
        comb_set.clear();
        del_vars2.clear();
        comb_set << &t3 << &t4 << &t5;
        del_vars2 << vars[4] << vars[5] << vars[6];
        tt1 = comb.execute(comb_set);
        tt2 = proj.execute(*tt1, del_vars2);
        delete tt1;
        TS_ASSERT((*tt2 == *res1) || (*tt2 == *res2) || (*tt2 == *res3))

        delete tt2;
        del_vars2.clear();
        del_vars2 << vars[9] << vars[10];
        tt2 = proj.execute(t6, del_vars2);
        TS_ASSERT((*tt2 == *res1) || (*tt2 == *res2) || (*tt2 == *res3))
        delete tt2;

        gum::MultiDimCombineAndProjectDefault< double, gum::Potential > projcomb2 = projcomb;
        projcomb2.setCombinationFunction(addPot);
        projcomb2.setProjectionFunction(myMax);

        gum::MultiDimCombineAndProjectDefault< double, gum::Potential >* projcomb3
           = projcomb.newFactory();

        delete projcomb3;

        for (const auto pot: res)
          delete pot;


        gum::IScheduleMultiDim<>::resetIdGenerator();
        gum::ScheduleMultiDim< gum::Potential< double > > xt1(t1, false);
        gum::ScheduleMultiDim< gum::Potential< double > > xt2(t2, false);
        gum::ScheduleMultiDim< gum::Potential< double > > xt3(t3, false);
        gum::ScheduleMultiDim< gum::Potential< double > > xt4(t4, false);
        gum::ScheduleMultiDim< gum::Potential< double > > xt5(t5, false);
        gum::ScheduleMultiDim< gum::Potential< double > > xt6(t6, false);
        gum::Set< const gum::IScheduleMultiDim<>* >
           sched_to_comb {&xt1, &xt2, &xt3, &xt4, &xt5, &xt6};

        auto ops_plus_res = projcomb.operations(sched_to_comb, del_vars);
        for(auto op: ops_plus_res.first) {
          const_cast< gum::ScheduleOperation<>* >(op)->execute();
        }

        gum::Potential< double > result1;
        if (ops_plus_res.second.size() > 1) {
          auto comb_ops_plus_res = comb.operations(ops_plus_res.second);
          for (auto op: comb_ops_plus_res.first) {
            const_cast< gum::ScheduleOperation<>* >(op)->execute();
          }
          result1 =
             dynamic_cast< const gum::ScheduleMultiDim< gum::Potential< double > >* >(
             comb_ops_plus_res.second)->multiDim();
          for (auto op: comb_ops_plus_res.first) delete op;
        }
        else {
          result1 = dynamic_cast< const gum::ScheduleMultiDim< gum::Potential< double > >* >(
             *(ops_plus_res.second.begin()))->multiDim();
        }

        gum::Potential< double >* result2a = comb.execute(to_comb);
        gum::Potential< double >* result2 = proj.execute(*result2a, del_vars);
        TS_ASSERT(result1 == *result2);

        delete result2;
        delete result2a;

        for (auto op: ops_plus_res.first)
          delete op;

        for (gum::Idx i = 0; i < vars.size(); ++i)
          delete vars[i];

      } catch (gum::NotFound& e) {
        GUM_SHOWERROR(e);
        TS_ASSERT(false)
      }
    }


    void testFloat() {
      std::vector< gum::LabelizedVariable* > vars(11);

      for (gum::Idx i = 0; i < 11; ++i) {
        std::stringstream str;
        str << "x" << i;
        std::string s = str.str();
        vars[i]       = new gum::LabelizedVariable(s, s, 4);
      }

      gum::Potential< float > t1, t2, t3, t4, t5, t6;
      t1 << *(vars[0]) << *(vars[1]);
      t2 << *(vars[1]) << *(vars[2]);
      t3 << *(vars[3]) << *(vars[4]) << *(vars[5]);
      t4 << *(vars[4]) << *(vars[7]);
      t5 << *(vars[5]) << *(vars[6]);
      t6 << *(vars[8]) << *(vars[9]);

      randomInitP(t1);
      randomInitP(t2);
      randomInitP(t3);
      randomInitP(t4);
      randomInitP(t5);
      randomInitP(t6);

      gum::MultiDimCombineAndProjectDefault< float, gum::Potential > projcomb(multPot, mySum);

      gum::Set< const gum::Potential< float >* > to_comb;
      to_comb << &t1 << &t2 << &t3 << &t4 << &t5 << &t6;
      gum::Set< const gum::DiscreteVariable* > del_vars;
      del_vars << vars[1] << vars[4] << vars[5] << vars[6] << vars[9] << vars[10];

      try {
        gum::Set< const gum::Potential< float >* > res
           = projcomb.execute(to_comb, del_vars);

        float nb_ops = projcomb.nbOperations(to_comb, del_vars);

        auto yyy = projcomb.memoryUsage(to_comb, del_vars);

        TS_ASSERT_EQUALS(yyy.first, 116 * sizeof(float))
        TS_ASSERT_EQUALS(yyy.second, 36 * sizeof(float))

        TS_ASSERT_EQUALS(nb_ops, 421)
        TS_ASSERT_EQUALS(res.size(), (gum::Size)3)

        gum::Set< const gum::Potential< float >* >::const_iterator iter = res.begin();
        const gum::Potential< float >*                             res1 = *iter;
        ++iter;
        const gum::Potential< float >* res2 = *iter;
        ++iter;
        const gum::Potential< float >* res3 = *iter;

        gum::MultiDimCombinationDefault< float, gum::Potential > comb(multPot);
        gum::MultiDimProjection< float, gum::Potential >         proj(mySum);

        gum::Set< const gum::Potential< float >* > comb_set;
        comb_set << &t1 << &t2;
        gum::Set< const gum::DiscreteVariable* > del_vars2;
        del_vars2 << vars[1];
        gum::Potential< float >* tt1 = comb.execute(comb_set);
        gum::Potential< float >* tt2 = proj.execute(*tt1, del_vars2);
        delete tt1;
        TS_ASSERT((*tt2 == *res1) || (*tt2 == *res2) || (*tt2 == *res3))

        delete tt2;
        comb_set.clear();
        del_vars2.clear();
        comb_set << &t3 << &t4 << &t5;
        del_vars2 << vars[4] << vars[5] << vars[6];
        tt1 = comb.execute(comb_set);
        tt2 = proj.execute(*tt1, del_vars2);
        delete tt1;

        TS_ASSERT((*tt2 == *res1) || (*tt2 == *res2) || (*tt2 == *res3))

        delete tt2;
        del_vars2.clear();
        del_vars2 << vars[9] << vars[10];
        tt2 = proj.execute(t6, del_vars2);
        TS_ASSERT((*tt2 == *res1) || (*tt2 == *res2) || (*tt2 == *res3))
        delete tt2;

        gum::MultiDimCombineAndProjectDefault< float, gum::Potential > projcomb2 = projcomb;
        projcomb2.setCombinationFunction(addPot);
        projcomb2.setProjectionFunction(myMax);

        gum::MultiDimCombineAndProjectDefault< float, gum::Potential >* projcomb3
           = projcomb.newFactory();

        delete projcomb3;

        for (const auto pot: res)
          delete pot;

        for (gum::Idx i = 0; i < vars.size(); ++i)
          delete vars[i];

      } catch (gum::NotFound& e) {
        GUM_SHOWERROR(e);
        TS_ASSERT(false)
      }
    }


    void testConstants() {
      std::vector< gum::LabelizedVariable* > vars(3);

      for (gum::Idx i = 0; i < 3; ++i) {
        std::stringstream str;
        str << "x" << i;
        std::string s = str.str();
        vars[i]       = new gum::LabelizedVariable(s, s, 4);
      }

      gum::Potential< float > t1, t2, t3;
      t1 << *(vars[0]) << *(vars[1]) << *(vars[2]);
      randomInitP(t1);

      gum::Instantiation I2(t2), I3(t3);
      t2.set(I2, 3.0);
      t3.set(I3, 5.0);

      gum::MultiDimCombineAndProjectDefault< float, gum::Potential > projcomb(multPot, mySum);

      gum::Set< const gum::Potential< float >* > to_comb;
      to_comb << &t1 << &t2 << &t3;
      gum::Set< const gum::DiscreteVariable* > del_vars;
      del_vars << vars[0] << vars[2];

      try {
        gum::Set< const gum::Potential< float >* > res
           = projcomb.execute(to_comb, del_vars);
        TS_ASSERT_EQUALS(res.size(), (gum::Size)3)

        int   nb_empty = 0;
        float prod     = 1;
        for (const auto ptrPot: res) {
          if (ptrPot->nbrDim() == 0) {
            gum::Instantiation I(*ptrPot);
            prod *= (*ptrPot)[I];
            nb_empty++;
          }
        }

        TS_ASSERT_EQUALS(nb_empty, 2)
        TS_ASSERT(prod = 15.0)
      } catch (gum::Exception& e) { GUM_SHOWERROR(e) }

      return;

      del_vars << vars[1];
      {
        gum::Set< const gum::Potential< float >* > res
           = projcomb.execute(to_comb, del_vars);
        TS_ASSERT_EQUALS(res.size(), (gum::Size)3)

        int   nb_empty = 0;
        float prod     = 1;
        for (const auto ptrPot: res) {
          if (ptrPot->nbrDim() == 0) {
            gum::Instantiation I(*ptrPot);
            prod *= (*ptrPot)[I];
            nb_empty++;
          }
        }

        TS_ASSERT_EQUALS(nb_empty, 3)
        TS_ASSERT_DELTA(prod, 15.0, 1e-6)
      }


      to_comb.clear();
      to_comb << &t2 << &t3;
      {
        gum::Set< const gum::Potential< float >* > res
           = projcomb.execute(to_comb, del_vars);
        TS_ASSERT_EQUALS(res.size(), (gum::Size)2)

        int   nb_empty = 0;
        float prod     = 1;
        for (const auto ptrPot: res) {
          if (ptrPot->nbrDim() == 0) {
            gum::Instantiation I(*ptrPot);
            prod *= (*ptrPot)[I];
            nb_empty++;
          }
        }

        TS_ASSERT_EQUALS(nb_empty, 2)
        TS_ASSERT_DELTA(prod, 15.0, 1e-6)
      }
    }


    private:
    // =========================================================================
    /// initialize randomly a table
    // =========================================================================
    void randomInitP(gum::Potential< double >& t) {
      gum::Instantiation i(t);

      for (i.setFirst(); !i.end(); ++i)
        t.set(i, 1.0f + rand() * 50.0f / RAND_MAX);
    }

    // the function used to combine two tables
    static gum::Potential< double > addPot(const gum::Potential< double >& t1,
                                           const gum::Potential< double >& t2) {
      return t1 + t2;
    }

    // the function used to combine two tables
    static gum::Potential< double > multPot(const gum::Potential< double >& t1,
                                            const gum::Potential< double >& t2) {
      return t1 * t2;
    }

    static gum::Potential< double >
       mySum(const gum::Potential< double >&                 table,
             const gum::Set< const gum::DiscreteVariable* >& del_vars) {
      return table.margSumOut(del_vars);
    }

    static gum::Potential< double >
       myMax(const gum::Potential< double >&                 table,
             const gum::Set< const gum::DiscreteVariable* >& del_vars) {
      return table.margMaxOut(del_vars);
    }

    void randomInitP(gum::Potential< float >& t) {
      gum::Instantiation i(t);

      for (i.setFirst(); !i.end(); ++i)
        t.set(i, float(1.0f + rand() * 50.0 / RAND_MAX));
    }

    // the function used to combine two tables
    static gum::Potential< float > addPot(const gum::Potential< float >& t1,
                                          const gum::Potential< float >& t2) {
      return t1 + t2;
    }

    // the function used to combine two tables
    static gum::Potential< float > multPot(const gum::Potential< float >& t1,
                                           const gum::Potential< float >& t2) {
      return t1 * t2;
    }

    static gum::Potential< float >
       mySum(const gum::Potential< float >&                  table,
             const gum::Set< const gum::DiscreteVariable* >& del_vars) {
      return table.margSumOut(del_vars);
    }

    static gum::Potential< float >
       myMax(const gum::Potential< float >&                  table,
             const gum::Set< const gum::DiscreteVariable* >& del_vars) {
      return table.margMaxOut(del_vars);
    }
  };

} /* namespace gum_tests */
