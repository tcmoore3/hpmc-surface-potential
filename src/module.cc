// Copyright (c) 2009-2024 The Regents of the University of Michigan.
// Part of HOOMD-blue, released under the BSD 3-Clause License.

#include <pybind11/pybind11.h>
#include "SurfacePotential.h"

using namespace hoomd::hpmc::detail;

// TODO: Set the name of the python module to match ${COMPONENT_NAME} (set in
// CMakeLists.txt), prefixed with an underscore.
PYBIND11_MODULE(_hpmc_surface_potential, m)
    {
    export_SurfacePotential(m);

#ifdef ENABLE_HIP
#endif
    }
