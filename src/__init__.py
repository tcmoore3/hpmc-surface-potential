# Copyright (c) 2009-2025 The Regents of the University of Michigan.
# Part of HOOMD-blue, released under the BSD 3-Clause License.

"""An anisotropic pair potential based on a spherical harmonic.

As implemented, only a single spherical harmonic is used, specifiec by the
arguments `l` and `m`.  The next implementation will allow for linear
combinations of arbitrary `l`s and `m`s.

"""

# TODO: Import all Python modules in your component.
from . import version
from .hpmc_energy import SphericalHarmonic
