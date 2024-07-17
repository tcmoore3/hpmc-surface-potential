// Copyright (c) 2009-2024 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.

#pragma once

#include "hoomd/HOOMDMath.h"
#include "hoomd/VectorMath.h"
#include "hoomd/hpmc/ExternalPotential.h"
#include <pybind11/pybind11.h>

namespace hoomd
    {
namespace hpmc
    {

/** Example external potential for use with HPMC simulations.

    This example external potential applies a harmonic trap at the center of the simulation box.

    TODO: Rename the "ExampleExternalPotential" to a class name that represents your potential.
    "ExampleExternal" appears many times in C++, CMakeLists, and Python files. Consider using a
    global search and replace tool.
*/
class ExampleExternalPotential : public ExternalPotential
    {
    public:
    ExampleExternalPotential(std::shared_ptr<SystemDefinition> sysdef);
    virtual ~ExampleExternalPotential() { }

    /// Set type-pair-dependent parameters to the potential.
    void setParamsPython(const std::string& particle_type, pybind11::dict params);

    /// Get type-pair-dependent parameters.
    pybind11::dict getParamsPython(const std::string& particle_type);

    protected:
    virtual LongReal particleEnergyImplementation(unsigned int type_i,
                                                  const vec3<LongReal>& r_i,
                                                  const quat<LongReal>& q_i,
                                                  LongReal charge_i,
                                                  bool trial);

    /// per-type parameters
    struct ParamType
        {
        ParamType() { }

        /// Construct a parameter set from a dictionary.
        ParamType(pybind11::dict v);

        /// Convert a parameter set to a dictionary.
        pybind11::dict asDict();

        // TODO: Rename m_epsilon and add per-type quantities as needed.
        /// Prefactor in harmonic potential.
        LongReal m_epsilon;
        };

    /// Parameters per type.
    std::vector<ParamType> m_params;
    };

namespace detail
    {
//! Export the ExampleUpdater class to python
void export_ExampleExternalPotential(pybind11::module& m);

    } // end namespace detail

    } // end namespace hpmc
    } // end namespace hoomd
