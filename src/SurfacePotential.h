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

*/
class SurfacePotential : public ExternalPotential
    {
    public:
    SurfacePotential(std::shared_ptr<SystemDefinition> sysdef);
    virtual ~SurfacePotential() { }

    /// Set type-pair-dependent parameters to the potential.
    void setParamsPython(const std::string& particle_type, pybind11::dict params);

    /// Get type-pair-dependent parameters.
    pybind11::dict getParamsPython(const std::string& particle_type);

    vec3<LongReal> getPlaneOrigin() const
        {
        return m_plane_origin;
        }

    void setPlaneOrigin(pybind11::tuple origin)
        {
        vec3<LongReal> new_origin(origin[0].cast<LongReal>(),
                                  origin[1].cast<LongReal>(),
                                  origin[2].cast<LongReal>());
        m_plane_origin = new_origin;
        }

    vec3<LongReal> getPlaneNormal() const
        {
        return m_plane_normal;
        }

    void setPlaneNormal(pybind11::tuple normal)
        {
        vec3<LongReal> new_normal(normal[0].cast<LongReal>(),
                                  normal[1].cast<LongReal>(),
                                  normal[2].cast<LongReal>());
        m_plane_normal = normalize(new_normal);
        }

    protected:
    LongReal particleEnergyImplementation(uint64_t timestep,
                                          unsigned int tag_i,
                                          unsigned int type_i,
                                          const vec3<LongReal>& r_i,
                                          const quat<LongReal>& q_i,
                                          LongReal charge_i,
                                          Trial trial = Trial::None) override;

    /// per-type parameters
    struct ParamType
        {
        ParamType() { }

        /// Construct a parameter set from a dictionary.
        ParamType(pybind11::dict v);

        /// Convert a parameter set to a dictionary.
        pybind11::dict asDict();

        LongReal m_epsilon;
        LongReal m_rcut;
        };

    /// Parameters per type.
    std::vector<ParamType> m_params;
    vec3<LongReal> m_plane_origin;
    vec3<LongReal> m_plane_normal;
    };

namespace detail
    {
//! Export the ExampleUpdater class to python
void export_SurfacePotential(pybind11::module& m);

    } // end namespace detail

    } // end namespace hpmc
    } // end namespace hoomd
