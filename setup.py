import pathlib
from setuptools import setup

# The directory containing this file
HERE = pathlib.Path(__file__).parent

# The text of the README file
with open("README.md", "rt") as f:
    README = f.read()

# This call to setup() does all the work
setup(
    name="gloomhavenhelper",
    version="0.1.1",
    description="A server and client implementation to talk to GloomhavenHelper Application",
    long_description=README,
    long_description_content_type="text/markdown",
    url="https://github.com/Gronis/gloomhaven-helper-rfid",
    author="Gronis",
    author_email="robingronberg@gmail.com",
    license="MIT",
    classifiers=[
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
    ],
    packages=['gloomhavenhelper'],
    package_dir={'gloomhavenhelper': 'out/ghh'},
    include_package_data=True,
    install_requires=[],

)
