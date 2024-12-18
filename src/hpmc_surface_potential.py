# Copyright (c) 2009-2024 The Regents of the University of Michigan.
# Part of HOOMD-blue, released under the BSD 3-Clause License.

"""Template HOOMD-blue component."""

import hoomd
import hoomd.conftest
from hoomd.hpmc_surface_potential import _hpmc_surface_potential


class SurfacePotential(hoomd.hpmc.external.External):
    """Apply a potential between particles and a wall.

    Args:
        epsilon (float): prefactor
        r_cut (float): self-explanatory
        plane_normal (array of length-3 arrays):
        plane_origin (array of length-3 arrays):

    """

    _cpp_class_name = 'SurfacePotential'
    _ext_module = _hpmc_surface_potential

    def __init__(self, plane_normal=(0, 1, 0), plane_origin=(0, 0, 0)):
        param_dict = hoomd.data.parameterdicts.ParameterDict(
            plane_origin=(float, float, float), plane_normal=(float, float, float)
        )
        param_dict['plane_origin'] = plane_origin
        param_dict['plane_normal'] = plane_normal
        self._param_dict = param_dict

        params = hoomd.data.typeparam.TypeParameter(
            'params',
            'particle_types',
            hoomd.data.parameterdicts.TypeParameterDict(
                epsilon=float, r_cut=hoomd.data.typeconverter.positive_real, len_keys=1
            ),
        )
        self._add_typeparam(params)
        self.plane_origin = plane_origin
        self.plane_normal = plane_normal
