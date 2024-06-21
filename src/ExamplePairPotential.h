// Copyright (c) 2009-2024 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.

#pragma once

#include <hoomd/VectorMath.h>
#include <hoomd/hpmc/PairPotential.h>
#include <pybind11/pybind11.h>

namespace hoomd
    {
namespace hpmc
    {

/*** Example pair potential for use with HPMC simulations.
 */
class ExamplePairPotential : public PairPotential
    {
    public:
    ExamplePairPotential(std::shared_ptr<SystemDefinition> sysdef);
    virtual ~ExamplePairPotential() { }

    virtual LongReal energy(const LongReal r_squared,
                            const vec3<LongReal>& r_ij,
                            const unsigned int type_i,
                            const quat<LongReal>& q_i,
                            const LongReal charge_i,
                            const unsigned int type_j,
                            const quat<LongReal>& q_j,
                            const LongReal charge_j) const;

    /// Compute the non-additive cuttoff radius
    LongReal computeRCutNonAdditive(unsigned int type_i, unsigned int type_j) const;

    /// Set type-pair-dependent parameters to the potential.
    void setParamsPython(pybind11::tuple particle_types, pybind11::dict params);

    /// Get type-pair-dependent parameters.
    pybind11::dict getParamsPython(pybind11 : tuple particle_types);

    protected:
    /// per-type-pair parameters
    struct ParamType
        {
        ParamType() { }

        /// Construct a parameter set from a dictionary.
        ParamType(pybind11::dict v);

        /// Convert a parameter set to a dictionary.
        pybind11::dict asDict();

        // TODO: rename/add parameters as needed
        LongReal m_A;
        LongReal m_B;
        LongReal m_r_cut;
        };

    /// Parameters per type pair.
    std::vector<ParamType> m_params;
    };

namespace detail
    {
//! Export the ExampleUpdater class to python
void export_ExamplePairPotential(pybind11::module& m);

    } // end namespace detail

    } // end namespace hpmc
    } // end namespace hoomd
