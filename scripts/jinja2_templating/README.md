Generate Fortran and C template code.

Templates use the Python jinja2 framework: https://jinja.palletsprojects.com/en/2.10.x/

The Python unittest framework wraps the individual file generation. To generate
files, use the unittest CLI: https://docs.python.org/3/library/unittest.html#command-line-interface

For example, to generate the `ESMF_Attribute.F90` code:
```sh
python -m unittest generate_templates.Runner.test_ESMF_Attribute
```