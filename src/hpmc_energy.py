# Copyright (c) 2009-2024 The Regents of the University of Michigan.
# Part of HOOMD-blue, released under the BSD 3-Clause License.

"""Template HOOMD-blue component."""


import hoomd
from hoomd.hpmc_energy import _hpmc_energy


class ExampleExternal(hoomd.hpmc.external.External):
    """An example external potential for HPMC."""

    _cpp_class_name = 'ExampleExternalPotential'
    _ext_module = _hpmc_energy

    def __init__(self):
        pass


class ExampleExternal(hoomd.hpmc.pair.Pair):
    """An example pair potential for HPMC."""

    _cpp_class_name = 'ExamplePairPotential'
    _ext_module = _hpmc_energy

    def __init__(self):
        pass
