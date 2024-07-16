# Copyright (c) 2009-2024 The Regents of the University of Michigan.
# Part of HOOMD-blue, released under the BSD 3-Clause License.

"""Test the version module."""

import hoomd.hpmc.pytest.conftest
import hoomd.hpmc_energy
import pytest

# TODO: rewrite the unit tests to verify that your potentials function correctly.


@pytest.mark.cpu()
def test_pair(simulation_factory, two_particle_snapshot_factory):
    """Test that ExamplePair computes the correct energies for 1 pair."""
    pair_potential = hoomd.hpmc_energy.ExamplePair()
    pair_potential.params.default = dict(A=1.0, B=2.0, r_cut=2.0)
    r_sep = 1.5
    expected_energy = 1.0 * (1 - r_sep / 2.0)

    simulation = simulation_factory(two_particle_snapshot_factory(d=r_sep))
    sphere = hoomd.hpmc.integrate.Sphere()
    sphere.shape['A'] = dict(diameter=0)
    simulation.operations.integrator = sphere
    sphere.pair_potentials = [pair_potential]
    simulation.run(0)
    assert sphere.pair_energy == pytest.approx(expected_energy)


@pytest.mark.cpu()
def test_external(simulation_factory, two_particle_snapshot_factory):
    """Test that ExampleExternal computes the correct energies for 2 particles."""
    external_potential = hoomd.hpmc_energy.ExampleExternal()
    external_potential.params.default = dict(epsilon=2)

    r_sep = 1.5
    expected_energy = 2 * 2 * ((r_sep / 2) ** 2 + 0.1**2)

    simulation = simulation_factory(two_particle_snapshot_factory(d=r_sep))
    sphere = hoomd.hpmc.integrate.Sphere()
    sphere.shape['A'] = dict(diameter=0)
    sphere.external_potentials = [external_potential]
    simulation.operations.integrator = sphere
    simulation.run(0)
    assert sphere.external_energy == pytest.approx(expected_energy)
