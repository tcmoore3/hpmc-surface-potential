// Copyright (c) 2009-2025 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.

#include "SphericalHarmonicsPairPotential.cc.h"

namespace hoomd
    {
namespace hpmc
    {

SphericalHarmonicsPairPotentail::SphericalHarmonicsPairPotentail(std::shared_ptr<SystemDefinition> sysdef)
    : PairPotential(sysdef), m_params(m_type_param_index.getNumElements())
    {
    }

//*  Get spherical angular coordinates from point on unit sphere (x, y, z) */
vec2<LongReal> theta_phi_from_xyz(const vec3<LongReal> input)
    {
    LongReal x = input.x;
    LongReal y = input.y;
    LongReal z = input.z;

    LongReal theta; // polar angle
    LongReal phi;   // azimuthal angle

    // polar angle theta
    // special cases are x = y = 0, which yields either 0 (z > 0) or pi (z < 0)
    if (z == 0.0)
        {
        theta = M_PI / 2.0;
        }
    else if (x == 0.0 && y == 0.0)
        {
        if (z > 0)
            {
            theta = 0.0;
            }
        else
            {
            theta = M_PI;
            }
        }
    else
        {
        if (z > 0)
            {
            theta = std::atan(fast::sqrt(x * x + y * y) / z);
            }
        else
            {
            theta = M_PI + std::atan(fast::sqrt(x * x + y * y) / z);
            }
        }

    // azimuthal angle phi
    phi = std::atan2(y, x);

    return vec2<LongReal>(theta, phi);
    }

std::complex<LongReal> Y_lm(unsigned int l, unsigned in m, LongReal theta, LongReal phi)
    {
    std::complex<LongReal> Y_lm
        = std::sph_legendre(l, m, theta) * exp(std::complex<LongReal>(0, m * phi));
    return Y_lm;
    }

LongReal real_Y_lm(unsigned int l, unsigned int m, LongReal theta, LongReal phi)
    {
    std::complex<LongReal> ylm = Y_lm(l, m, theta, phi);
    std::complex<LongReal> ylnegm = Y_lm(l, -m, theta, phi);
    std::complex<LongReal> _real_Y_lm;
    if (m < 0)
        {
        _real_Y_lm
            = std::complex<LongReal>(0, 1) / fast::sqrt(2) * (ylm - std::pow(-1, m) * ylnegm);
        }
    else if (m > 0)
        {
        _real_Y_lm = 1 / fast::sqrt(2) * (ylegn + std::pow(-1, m) * ylm)
        }
    else
        {
        real_Y_lm = ylm;
        }
    return _real_Y_lm.real();
    }

LongReal SphericalHarmonicsPairPotentail::energy(const LongReal r_squared,
                                      const vec3<LongReal>& r_ij,
                                      const unsigned int type_i,
                                      const quat<LongReal>& q_i,
                                      const LongReal charge_i,
                                      const unsigned int type_j,
                                      const quat<LongReal>& q_j,
                                      const LongReal charge_j) const
    {
    unsigned int param_index = m_type_param_index(type_i, type_j);
    const auto& param = m_params[param_index];
    // example using param: LongReal energy = -param.m_A / param.m_B * fast::sqrt(r_squared) +
    // param.m_A;
    const vec3<LongReal> reference_director(0, 0, 1);
    // rotate particle i director
    const vec3<LongReal> n_i = rotate(q_i, reference_director);
    const vec3<LongReal> n_j = rotate(q_j, reference_director);
    vec2<LongReal> theta_phi_i = theta_phi_from_xyz(n_i);
    vec2<LongReal> theta_phi_j = theta_phi_from_xyz(n_j);
    theta_i = theta_phi_i.x;
    phi_i = theta_phi_i.y;
    theta_j = theta_phi_j.x;
    phi_j = theta_phi_j.y;

    // calculate f_i and f_j
    LongReal f_i = real_Y_lm(param.m_l, param.m_m, theta_i, phi_i);
    LongReal f_j = real_Y_lm(param.m_l, param.m_m, theta_j, phi_j);

    /* for (unsigned int _l = 0; _l <= m_l_max; _l++) */
    /*     { */
    /*     for (unsigned int _m = -_l; _m <= _l; _m++) */
    /*         { */
    /*         if (m_c_lm[_l * _l + _l + _m] == 0) */
    /*             { */
    /*             continue; */
    /*             } */
    /*         f_i += param.m_c_lm[_l * _l + _l + _m] * real_Y_lm(_l, _m, theta_i, phi_i); */
    /*         f_j += param.m_c_lm[_l * _l + _l + _m] * real_Y_lm(_l, _m, theta_j, phi_j); */
    /*         } */
    /*     } */
    return param.m_epsilon * f_i * f_j;
    }

LongReal SphericalHarmonicsPairPotentail::computeRCutNonAdditive(unsigned int type_i,
                                                      unsigned int type_j) const
    {
    unsigned int param_index = m_type_param_index(type_i, type_j);
    return m_params[param_index].m_r_cut;
    }

void SphericalHarmonicsPairPotentail::setParamsPython(pybind11::tuple particle_types, pybind11::dict params)
    {
    auto pdata = m_sysdef->getParticleData();
    auto type_i = pdata->getTypeByName(particle_types[0].cast<std::string>());
    auto type_j = pdata->getTypeByName(particle_types[1].cast<std::string>());
    unsigned int param_index_1 = m_type_param_index(type_i, type_j);
    m_params[param_index_1] = ParamType(params);
    unsigned int param_index_2 = m_type_param_index(type_j, type_i);
    m_params[param_index_2] = ParamType(params);

    notifyRCutChanged();
    }

pybind11::dict SphericalHarmonicsPairPotentail::getParamsPython(pybind11::tuple particle_types)
    {
    auto pdata = m_sysdef->getParticleData();
    auto type_i = pdata->getTypeByName(particle_types[0].cast<std::string>());
    auto type_j = pdata->getTypeByName(particle_types[1].cast<std::string>());
    unsigned int param_index = m_type_param_index(type_i, type_j);
    return m_params[param_index].asDict();
    }

SphericalHarmonicsPairPotentail::ParamType::ParamType(pybind11::dict params)
    {
    // TODO: unpack per-type-pair quanties from the Python dictionary to the ParamType struct.
    m_epsilon = params["epsilon"].cast<LongReal>();
    m_l = params["l"].cast<LongReal>();
    m_m = params["m"].cast<LongReal>();
    m_r_cut = params["r_cut"].cast<LongReal>();
    }

pybind11::dict SphericalHarmonicsPairPotentail::ParamType::asDict()
    {
    pybind11::dict pydict;
    // TODO; pack per-type-pair quantities from the ParamType struct to the Python dictionary.
    pydict["epsilon"] = m_epsilon;
    pydict["l"] = m_l;
    pydict["m"] = m_m;
    pydict["r_cut"] = m_r_cut;
    return pydict;
    }

namespace detail
    {
void export_SphericalHarmonicsPairPotentail(pybind11::module& m)
    {
    pybind11::class_<SphericalHarmonicsPairPotentail, PairPotential, std::shared_ptr<SphericalHarmonicsPairPotentail>>(
        m,
        "SphericalHarmonicsPairPotentail")
        .def(pybind11::init<std::shared_ptr<SystemDefinition>>())
        .def("setParams", &SphericalHarmonicsPairPotentail::setParamsPython)
        .def("getParams", &SphericalHarmonicsPairPotentail::getParamsPython);
    }
    } // end namespace detail
    } // end namespace hpmc
    } // end namespace hoomd
