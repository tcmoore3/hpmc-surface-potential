# TODO: List all pytest files in files.
set(files __init__.py
          test_version.py
    )

# Copy tests to the install directory.
install(FILES ${files}
        DESTINATION ${PYTHON_SITE_INSTALL_DIR}/${COMPONENT_NAME}/pytest
       )

# Copy tests to the build directory for testing prior to installation.
copy_files_to_build("${files}" "hoomd-component-${COMPONENT_NAME}-pytest" "*.py")
