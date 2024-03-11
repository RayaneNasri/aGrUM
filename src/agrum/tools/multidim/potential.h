/**
 *
 *   Copyright (c) 2005-2023  by Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
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


/**
 * @file
 * @brief Header of the Potential class.
 * @author Pierre-Henri WUILLEMIN(_at_LIP6) & Christophe GONZALES(_at_AMU)
 */
#ifndef GUM_POTENTIAL_H
#define GUM_POTENTIAL_H

#include <functional>
#include <vector>

#include <agrum/agrum.h>

#include <agrum/tools/multidim/implementations/multiDimDecorator.h>

namespace gum {
  // ==========================================================================
  // ===                          POTENTIAL                                 ===
  // ==========================================================================

  /**
   * @class Potential
   * @headerfile potential.h <agrum/tools/multidim/potential.h>
   * @ingroup multidim_group
   *
   * @brief aGrUM's Potential is a multi-dimensional array with tensor operators.
   * It is used to represent probabilities and utilities in aGrUMs'
   * multidimensional (graphical) models.
   *
   * Using the decorator pattern, this representation is independent from the
   * implementation of the multidimensional matrix.
   *
   * @tparam GUM_SCALAR The type of the scalar stored in this multidimensional
   * matrix.
   */
  template < typename GUM_SCALAR >
  class Potential: public MultiDimDecorator< GUM_SCALAR > {
    public:
    // =========================================================================
    /// @name Constructors, Destructors and Copy
    // =========================================================================
    /// @{

    /**
     * @brief Default constructor.
     *
     * Creates an empty null dimensional matrix with a MultiDimArray as
     * its implementation.
     */
    Potential();

    /**
     * @brief Creates an potential around aContent.
     * @param aContent The implementation of this Potential.
     */
    explicit Potential(MultiDimImplementation< GUM_SCALAR >* aContent);

    /**
     * @brief Copy constructor.
     *
     * The newly created Potential share the variables and the values
     * from src, but no instantiation is associated to it. It allows to force
     * the chosen implementation and to copy the data from src.
     *
     * @param aContent The implementation to use in this Potential.
     * @param src The MultiDimContainer to copy.
     */
    Potential(MultiDimImplementation< GUM_SCALAR >*  aContent,
              const MultiDimContainer< GUM_SCALAR >& src);
    /**
     * @brief Copy constructor & assignment
     */
    Potential(const Potential< GUM_SCALAR >& src);
    Potential< GUM_SCALAR >& operator=(const Potential< GUM_SCALAR >& src);

    /**
     * move constructor & assignement
     **/
    Potential(Potential< GUM_SCALAR >&& from);
    Potential< GUM_SCALAR >& operator=(Potential< GUM_SCALAR >&& src);


    /**
     * @brief Destructor.
     */
    ~Potential() final;

    /// @}
    // ========================================================================
    /// @name MultiDimContainer implementation
    // ========================================================================
    /// @{

    Potential< GUM_SCALAR >* newFactory() const final;

    /// @}
    // ========================================================================
    /// @name Class operation for Potential instances
    // ========================================================================
    ///@{
    /** generate a random Potential with each parameter in [0,1]*/
    const Potential< GUM_SCALAR >& random() const;

    /** generate a random Distribution in the Potential */
    const Potential< GUM_SCALAR >& randomDistribution() const;

    /** generate a random CPT in the Potential */
    const Potential< GUM_SCALAR >& randomCPT() const;

    /** add a noise in a CPT by mixing (1-alpha)this+alpha.randomCPT()
     * @warning alpha in [0,1]
     */
    const Potential< GUM_SCALAR >& noising(GUM_SCALAR alpha) const;

    /**
     * Projection using sum as operation (and implementation-optimized
     * operations)
     * @param del_vars is the set of vars to eliminate
     */
    Potential< GUM_SCALAR > margSumOut(const gum::VariableSet& del_vars) const;

    /**
     * Projection using sum as operation (and implementation-optimized
     * operations)
     * @param kept_vars is the set of vars to keep
     */
    Potential< GUM_SCALAR > margSumIn(const gum::VariableSet& kept_vars) const;

    /**
     * Projection using multiplication as operation (and
     * implementation-optimized operations)
     * @param del_vars is the set of vars to eliminate
     */
    Potential< GUM_SCALAR > margProdOut(const gum::VariableSet& del_vars) const;

    /**
     * Projection using multiplication as operation (and
     * implementation-optimized operations)
     * @param kept_vars is the set of vars to keep
     */
    Potential< GUM_SCALAR > margProdIn(const gum::VariableSet& kept_vars) const;

    /**
     * Projection using min as operation (and implementation-optimized
     * operations)
     * @param del_vars is the set of vars to eliminate
     */
    Potential< GUM_SCALAR > margMinOut(const gum::VariableSet& del_vars) const;

    /**
     * Projection using min as operation (and implementation-optimized
     * operations)
     * @param kept_vars is the set of vars to keep
     */
    Potential< GUM_SCALAR > margMinIn(const gum::VariableSet& kept_vars) const;

    /**
     * Projection using max as operation (and implementation-optimized
     * operations)
     * @param del_vars is the set of vars to eliminate
     */
    Potential< GUM_SCALAR > margMaxOut(const gum::VariableSet& del_vars) const;

    /**
     * Projection using max as operation (and implementation-optimized
     * operations)
     * @param kept_vars is the set of vars to keep
     */
    Potential< GUM_SCALAR > margMaxIn(const gum::VariableSet& kept_vars) const;

    /**
     * create a boolean-like potential using the predicate isNonZero
     */
    Potential< GUM_SCALAR > isNonZeroMap() const;

    /// sum of all elements in the Potential
    GUM_SCALAR sum() const;
    /// product of all elements in the Potential
    GUM_SCALAR product() const;
    /// max of all elements in the Potential
    GUM_SCALAR max() const;
    /// min of all elements in the Potential
    GUM_SCALAR min() const;
    /// max of all non one elements in the Potential
    /// @warning can return 1 if no other value than 1 ...
    GUM_SCALAR maxNonOne() const;
    /// min of all non zero elements in the Potential
    /// @warning can return 0 if no other value than 0 ...
    GUM_SCALAR minNonZero() const;

    /// set of instantiation corresponding to the parameter v in the Potential
    Set< Instantiation > findAll(GUM_SCALAR v) const;

    /// Pair of the set of instantiation corresponding to the max and this max in the Potential
    std::pair< Set< Instantiation >, GUM_SCALAR > argmax() const;

    /// Pair of the set of instantiation corresponding to the min and this min in the Potential
    std::pair< Set< Instantiation >, GUM_SCALAR > argmin() const;

    /** ExpectedValue computes the expectation of f over *this
     *
     * Warnings
     * --------
     * ExpectedValue assumes (and does not check) that the Potential contains a joint distribution.
     */
    GUM_SCALAR expectedValue(std::function< GUM_SCALAR(const gum::Instantiation&) >) const;

    /// entropy of the Potential
    GUM_SCALAR entropy() const;

    /** create a new Potential with another order
     * @throw InvalidArgument if not all and only the vars of the potential are
     * in vars
     */
    Potential< GUM_SCALAR > reorganize(const std::vector< const DiscreteVariable* >& vars) const;

    /** create a new Potential with another order
     * @throw InvalidArgument if not all and only the vars of the potential are
     * in vars
     */
    Potential< GUM_SCALAR > reorganize(const std::vector< std::string >& vars) const;

    /** create a new Potential extracted from *this given a partial
     * instantiation
     */
    Potential< GUM_SCALAR > extract(const Instantiation& inst) const;

    /** create a new Potential with a certain variable in first
     * @throw InvalidArgument if the var is not in the potential
     */
    Potential< GUM_SCALAR > putFirst(const DiscreteVariable* var) const;

    /** create a new Potential with a certain variable in first
     * @throw InvalidArgument if the var is not in the potential
     */
    Potential< GUM_SCALAR > putFirst(const std::string& varname) const;

    /**
     * @brief copy a Potential data using name of variables and labels (not
     * necessarily the same variables in the same orders)
     *
     * @warning a strict control on names of variables and labels are made
     *
     * @throw InvalidArgument if the Potential is not compatible with this
     */
    const Potential< GUM_SCALAR >& fillWith(const Potential< GUM_SCALAR >& src) const;

    /**
     * @brief copy a Potential data using the sequence of names in mapSrc to find
     * the corresponding variables.
     *
     * For instance, to copy the potential P(A,B,C) in Q(D,E,A) with the mapping
     * P.A<->Q.E, P.B<->Q.A, P.C<->Q.D (assuming that the corresponding variables
     * have the same domain size and the order of labels):
     *
     * @code
     *   Q.fillWith(P,{"C","A","B"});
     * @endcode
     *
     * @warning a strict control on names of variables and labels are made
     *
     * @throw InvalidArgument if the Potential is not compatible with this
     * */
    const Potential< GUM_SCALAR >& fillWith(const Potential< GUM_SCALAR >&    src,
                                            const std::vector< std::string >& mapSrc) const;

    /**
     * @brief Automatically fills the potential with the values in
     * v.
     *
     * @param v Vector of values.
     * @throw SizeError Raised if v size's does not matches this
     * MultiDimContainer domain size.
     */
    const Potential< GUM_SCALAR >& fillWith(const std::vector< GUM_SCALAR >& data) const;

    /**
     * @brief Automatically fills this MultiDimContainer with the value v
     *
     * @param v contains the data.
     */
    const Potential< GUM_SCALAR >& fillWith(const GUM_SCALAR& val) const;


    /**
     * @brief Apply abs on every element of the container
     */
    const Potential< GUM_SCALAR >& abs() const;

    /**
     * @brief apply $x^2$ on every element of the container
     */
    const Potential< GUM_SCALAR >& sq() const;

    /**
     * @brief apply $log_2(x)$ on every element of the container
     */
    const Potential< GUM_SCALAR >& log2() const;

    /**
     * @brief apply sgn(x)$ on every element of the container
     *
     * fill the potential with -1 for a GUM_SCALAR<0,1 if >0 and 0 if ==0
     */
    const Potential< GUM_SCALAR >& sgn() const;

    /**
     * @brief Create a new potential and apply abs on every element of the
     * container
     */
    Potential< GUM_SCALAR > new_abs() const;

    /**
     * @brief Create a new potential and apply $x^2$ on every element of the
     * container
     */
    Potential< GUM_SCALAR > new_sq() const;

    /**
     * @brief Create a new potential and apply $log_2(x)$ on every element of the
     * container
     */
    Potential< GUM_SCALAR > new_log2() const;

    /**
     * @brief Create a new potential and apply sgn(x)$ on every element of the container
     *
     * fill the potential with -1 for a GUM_SCALAR<0,1 if >0 and 0 if ==0
     */
    Potential< GUM_SCALAR > new_sgn() const;


    /**
     * @brief normalisation of this do nothing if sum is 0
     */
    const Potential< GUM_SCALAR >& normalize() const;

    /**
     * @brief compute KL divergence between this and p
     * Checks the compatibility and then compute KL divergence
     * @throws gum::InvalidArgument if p is not compatible with $this (dimension,
     * variables)
     * @throws gum::FatalError if a zero is found in p or this and not in the
     * other.
     */
    GUM_SCALAR KL(const Potential< GUM_SCALAR >& p) const;

    /**
     * @brief normalisation of this as a CPT for the variable varId
     *
     * If the Potential is empty, the argument is not used.
     *
     * @throw FatalError it some distribution sums to 0, or if varId>=nbrDim()
     */
    const Potential< GUM_SCALAR >& normalizeAsCPT(const Idx& varId = 0) const;

    /**
     * @brief multiply (each value of) *this by v
     */
    const Potential< GUM_SCALAR >& scale(GUM_SCALAR v) const;

    /**
     * @brief add v to (each value of) *this
     */
    const Potential< GUM_SCALAR >& translate(GUM_SCALAR v) const;

    /**
     * @brief the function to inverse (each value of) *this
     */
    const Potential< GUM_SCALAR >& inverse() const;

    /**
     * @brief get a value at random from a 1-D distribution
     */
    [[nodiscard]] Idx draw() const;

    ///@}

    // ========================================================================
    /// @name Potential algebra operators
    // ========================================================================
    ///@{
    Potential< GUM_SCALAR >  operator+(const Potential< GUM_SCALAR >& p2) const;
    Potential< GUM_SCALAR >  operator+(const GUM_SCALAR& v) const;
    Potential< GUM_SCALAR >  operator-(const Potential< GUM_SCALAR >& p2) const;
    Potential< GUM_SCALAR >  operator-(const GUM_SCALAR& v) const;
    Potential< GUM_SCALAR >  operator*(const Potential< GUM_SCALAR >& p2) const;
    Potential< GUM_SCALAR >  operator*(const GUM_SCALAR& v) const;
    Potential< GUM_SCALAR >  operator/(const Potential< GUM_SCALAR >& p2) const;
    Potential< GUM_SCALAR >  operator/(const GUM_SCALAR& v) const;
    Potential< GUM_SCALAR >& operator+=(const Potential< GUM_SCALAR >& r);
    Potential< GUM_SCALAR >& operator+=(const GUM_SCALAR& v);
    Potential< GUM_SCALAR >& operator*=(const Potential< GUM_SCALAR >& r);
    Potential< GUM_SCALAR >& operator*=(const GUM_SCALAR& v);
    Potential< GUM_SCALAR >& operator-=(const Potential< GUM_SCALAR >& r);
    Potential< GUM_SCALAR >& operator-=(const GUM_SCALAR& v);
    Potential< GUM_SCALAR >& operator/=(const Potential< GUM_SCALAR >& r);
    Potential< GUM_SCALAR >& operator/=(const GUM_SCALAR& v);

    // these operations are only defined for boolean-like Potential (evidence/likelihood)
    [[nodiscard]] bool isEvidence() const;
    Potential< GUM_SCALAR >  operator|(const Potential< GUM_SCALAR >& p2) const;
    Potential< GUM_SCALAR >  operator&(const Potential< GUM_SCALAR >& p2) const;
    Potential< GUM_SCALAR >  operator~() const;

    bool operator==(const Potential< GUM_SCALAR >& r) const;

    Potential< GUM_SCALAR >& operator<<(const DiscreteVariable& v);

    [[nodiscard]] std::string toString() const final;

    ///@}
    ///@{
    /// numerical evidence generator
    static Potential< GUM_SCALAR >  evEq(const DiscreteVariable& v, double val);
    static Potential< GUM_SCALAR >  evIn(const DiscreteVariable& v, double val1, double val2);
    ///@}
    protected:
    [[nodiscard]] gum::VariableSet complementVars_(const gum::VariableSet& del_vars) const;
  };

#ifndef GUM_NO_EXTERN_TEMPLATE_CLASS
  extern template class Potential< double >;
#endif

  template < typename GUM_SCALAR >
  inline Potential< GUM_SCALAR > log2(const Potential< GUM_SCALAR >& arg) {
    return arg.new_log2();
  }

  template < typename GUM_SCALAR >
  inline Potential< GUM_SCALAR > abs(const Potential< GUM_SCALAR >& arg) {
    return arg.new_abs();
  }

  template < typename GUM_SCALAR >
  inline Potential< GUM_SCALAR > sq(const Potential< GUM_SCALAR >& arg) {
    return arg.new_sq();
  }

} /* namespace gum */

#include <agrum/tools/multidim/potential_tpl.h>
#endif /* GUM_POTENTIAL_H */
