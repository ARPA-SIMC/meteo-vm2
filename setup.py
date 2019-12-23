import os
import re

from setuptools import find_packages, setup



def get_version(package):
    # Thanks to Tom Christie
    init_py = open(os.path.join(package, '__init__.py')).read()
    return re.search("__version__ = ['\"]([^'\"]+)['\"]", init_py).group(1)


def read_md(path):
    try:
        import pypandoc
        return pypandoc.convert(path, 'rst')
    except ImportError:
        return open(path).read()


version = get_version('meteovm2')

setup(
    name="meteovm2",
    version=version,
    packages=find_packages(include=["meteovm2", "meteovm2.*"]),
    include_package_data=True,
    license='GPLv2+',
    description='Python library for VM2 data',
    long_description=read_md('README.md'),
    url='http://github.com/arpa-simc/meteo-vm2',
    author='Emanuele Di Giacomo',
    author_email="edigiacomo@arpae.it",
    install_requires=["requests"],
    test_suite="tests",
    classifiers=[
        'Development Status :: 4 - Beta',
        'License :: OSI Approved :: GNU General Public License v2 or later (GPLv2+)',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
    ],
    entry_points={
        'console_scripts': [
            'meteo-vm2-to-bufr = meteovm2.cli.meteovm2_to_bufr:main',
            'bufr-to-meteo-vm2 = meteovm2.cli.bufr_to_meteovm2:main',
            'meteo-vm2-table-dump = meteovm2.cli.meteovm2_table_dump:main',
        ],
    },
)
