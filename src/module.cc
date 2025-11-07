// Copyright (c) 2009-2025 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.

#include "SurfacePotential.h"
#include <pybind11/pybind11.h>

using namespace hoomd::hpmc::detail;

PYBIND11_MODULE(_hpmc_surface_potential, m)
    {
    export_SurfacePotential(m);

#ifdef ENABLE_HIP
#endif
    }
