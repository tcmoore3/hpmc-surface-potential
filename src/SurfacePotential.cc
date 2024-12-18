// Copyright (c) 2009-2024 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.

#include "SurfacePotential.h"

namespace hoomd
    {
namespace hpmc
    {

SurfacePotential::SurfacePotential(std::shared_ptr<SystemDefinition> sysdef)
    : ExternalPotential(sysdef), m_params(sysdef->getParticleData()->getNTypes())
    {
    }

LongReal SurfacePotential::particleEnergyImplementation(uint64_t timestep,
                                                        unsigned int tag_i,
                                                        unsigned int type_i,
                                                        const vec3<LongReal>& r_i,
                                                        const quat<LongReal>& q_i,
                                                        LongReal charge_i,
                                                        Trial trial)
    {
    // TODO: implement the functional form of the external potential.
    const auto& param = m_params[type_i];

    LongReal distance_to_wall_squared = dot(m_plane_normal, r_i - m_plane_origin);
    if (distance_to_wall_squared > param.m_rcut * param.m_rcut)
        {
        return 0;
        }
    else
        {
        return param.m_epsilon / distance_to_wall_squared;
        }
    }

void SurfacePotential::setParamsPython(const std::string& particle_type, pybind11::dict params)
    {
    unsigned int particle_type_id = m_sysdef->getParticleData()->getTypeByName(particle_type);
    m_params[particle_type_id] = ParamType(params);
    }

pybind11::dict SurfacePotential::getParamsPython(const std::string& particle_type)
    {
    unsigned int particle_type_id = m_sysdef->getParticleData()->getTypeByName(particle_type);
    return m_params[particle_type_id].asDict();
    }

SurfacePotential::ParamType::ParamType(pybind11::dict params)
    {
    pybind11::dict v = params;

    // TODO: unpack per-type quanties from the Python dictionary to the ParamType struct.
    m_epsilon = v["epsilon"].cast<LongReal>();
    m_rcut = v["r_cut"].cast<LongReal>();
    }

pybind11::dict SurfacePotential::ParamType::asDict()
    {
    pybind11::dict pydict;
    pydict["epsilon"] = m_epsilon;
    pydict["r_cut"] = m_rcut;
    return pydict;
    }

namespace detail
    {
void export_SurfacePotential(pybind11::module& m)
    {
    pybind11::class_<SurfacePotential, ExternalPotential, std::shared_ptr<SurfacePotential>>(
        m,
        "SurfacePotential")
        .def(pybind11::init<std::shared_ptr<SystemDefinition>>())
        .def("setParams", &SurfacePotential::setParamsPython)
        .def("getParams", &SurfacePotential::getParamsPython)
        .def_property("plane_origin",
                      &SurfacePotential::getPlaneOrigin,
                      &SurfacePotential::setPlaneOrigin)
        .def_property("plane_normal",
                      &SurfacePotential::getPlaneNormal,
                      &SurfacePotential::setPlaneNormal);
    }
    } // end namespace detail
    } // end namespace hpmc
    } // end namespace hoomd
