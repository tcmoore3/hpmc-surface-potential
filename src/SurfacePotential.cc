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
    // see if _particle_ is within interaction range
    // r_sq = distance from particle center to wall
    // return 0.0;
    const auto& param = m_params[type_i];
    auto r_sq_i = fabs(dot(m_plane_normal, r_i - m_plane_origin));
    if (r_sq_i > param.m_max_extent_sq)
        {
        return 0.0;
        }

    auto num_facets = static_cast<unsigned int>(param.m_position.size());
    LongReal energy = 0.0;
    for (unsigned int i = 0; i < num_facets; i++)
        {
        auto facet_pos = rotate(q_i, param.m_position[i]) + r_i;
        auto r_sq = fabs(dot(m_plane_normal, facet_pos - m_plane_origin));
        if (r_sq > param.m_rcut * param.m_rcut)
            {
            continue;
            }
        auto facet_normal = rotate(q_i, param.m_normal[i]);
        if (dot(-facet_normal, m_plane_normal) <= 0.0)
            {
            continue;
            }
        LongReal rmd_over_sigma_2 = r_sq / param.m_sigma_2;
        LongReal exp_val = fast::exp(-LongReal(1.0) / LongReal(2.0) * rmd_over_sigma_2);
        LongReal f_orientation = fast::sqrt(dot(-facet_normal, m_plane_normal));
        energy += param.m_epsilon * exp_val * f_orientation;
        }
    return energy;
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
    if (v.is_none())
        {
        m_position.clear();
        m_normal.clear();
        return;
        }

    m_epsilon = v["epsilon"].cast<LongReal>();
    m_rcut = v["r_cut"].cast<LongReal>();
    m_sigma_2 = v["sigma"].cast<LongReal>() * v["sigma"].cast<LongReal>();

    // unpack vector quantities
    pybind11::list positions = v["positions"];
    pybind11::list normals = v["normals"];

    auto N = pybind11::len(positions);
    m_normal.resize(N);
    m_position.resize(N);
    LongReal max_extent_sq = 0.0;
    for (unsigned int i = 0; i < N; i++)
        {
        pybind11::tuple pos_python = positions[i];
        pybind11::tuple normal_python = normals[i];

        m_position[i] = vec3<LongReal>(pos_python[0].cast<LongReal>(),
                                       pos_python[1].cast<LongReal>(),
                                       pos_python[2].cast<LongReal>());
        max_extent_sq = std::fmax(dot(m_position[i], m_position[i]), max_extent_sq);
        m_normal[i] = vec3<LongReal>(normal_python[0].cast<LongReal>(),
                                     normal_python[1].cast<LongReal>(),
                                     normal_python[2].cast<LongReal>());
        }
    m_max_extent_sq = (sqrt(max_extent_sq) + m_rcut) * (sqrt(max_extent_sq) + m_rcut);
    }

pybind11::dict SurfacePotential::ParamType::asDict()
    {
    size_t N = m_position.size();
    if (N == 0)
        {
        return pybind11::none();
        }
    pybind11::list positions;
    pybind11::list normals;
    for (unsigned int i = 0; i < N; i++)
        {
        positions.append(pybind11::make_tuple(m_position[i].x, m_position[i].y, m_position[i].z));
        normals.append(pybind11::make_tuple(m_normal[i].x, m_normal[i].y, m_normal[i].z));
        }

    pybind11::dict pydict;
    pydict["epsilon"] = m_epsilon;
    pydict["r_cut"] = m_rcut;
    pydict["sigma"] = sqrt(m_sigma_2);
    pydict["positions"] = positions;
    pydict["normals"] = normals;
    pydict["max_extent"] = sqrt(m_max_extent_sq);
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
