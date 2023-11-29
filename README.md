# HOOMD-blue component template

`hoomd-component-template` provides a framework to develop a component that extends
[HOOMD-blue](https://glotzerlab.engin.umich.edu/hoomd-blue/).

It includes:

* CMake scripts to build the component.
* Template C++ and Python modules.
* An example unit test.
* Pre-commit configuration.
* GitHub Actions configuration.

## Building the component

To build this component:

1. Build and install HOOMD-blue from source.
2. Obtain the component's source.
    ```
    $ git clone https://github.com/glotzerlab/hoomd-component-template
    ```
3. Configure.
    ```
    $ cmake -B build/hoomd-component-template -S hoomd-component-template
    ```
4. Build the component.
    ```
    $ cmake --build build/hoomd-component-template
    ```
5. Install the component.
    ```
    $ cmake --install build/hoomd-component-template
    ```

Once installed, the template is available for import via:
```
import hoomd.template
```

## Creating a new component

To create a new component:

1. Fork the [hoomd-component-template](https://github.com/glotzerlab/hoomd-component-template/)
    repository.
2. Add C++ and Python files to `src/`.
3. Address all TODO comments. These explain how to set your package name, add C++ files, add unit
    tests, and other tasks. Use [ripgrep](https://github.com/BurntSushi/ripgrep) to find all TODO
    messages:
    ```
    $ rg TODO . .github
    ```
4. Add unit tests in `src/pytest` using [pytest](https://docs.pytest.org).
5. Format and check code style with [pre-commit](https://pre-commit.com/).
    ```
    $ pre-commit run --all-files
    ```
6. Push your changes to GitHub and the provided [GitHub Actions](https://docs.github.com/en/actions)
    workflow will run and test your that your code compiles on the CPU (with and without MPI), and
    on the GPU (with and without MPI). The workflow also runs the unit tests on the CPU. You should
    run GPU unit tests locally, GitHub does not provide free GPU runners for GitHub Actions.

## Maintaining your component

The HOOMD-blue developers will periodically update
[hoomd-component-template](https://github.com/glotzerlab/hoomd-component-template/), including
updates to the GitHub Actions workflow, pre-commit configuration, and CMake scripts. Merge these
changes into your fork to to test with the latest version of HOOMD-blue.

For example, run:
```
$ git remote add upstream https://github.com/glotzerlab/hoomd-component-template
$ git fetch upstream
$ git merge upstream/trunk
```
and resolve merge conflicts as appropriate.

## Documenting and releasing your component

TODO: Document your component in `README.md` (this file).

When appropriate:

* Add [Sphinx](https://www.sphinx-doc.org) documentation and publish it on
[readthedocs](https://www.readthedocs.org).
* Add a [conda-forge](https://conda-forge.org/) package.
* Announce your component on the [hoomd-users](https://groups.google.com/g/hoomd-users) mailing
  list.
