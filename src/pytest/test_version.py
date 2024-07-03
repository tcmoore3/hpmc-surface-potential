# Copyright (c) 2009-2024 The Regents of the University of Michigan.
# Part of HOOMD-blue, released under the BSD 3-Clause License.

"""Test the version module."""

import hoomd.hpmc_energy


def test_version():
    """Test the version attribute."""
    assert hoomd.hpmc_energy.version.version == '0.0.0'
