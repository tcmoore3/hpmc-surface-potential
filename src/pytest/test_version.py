"""Test the version module."""

import hoomd.template


def test_version():
    """Test the version attribute."""
    assert hoomd.template.version.version == '0.0.0'
