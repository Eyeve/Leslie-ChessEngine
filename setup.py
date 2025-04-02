from setuptools import setup, find_packages

setup(
    name="chess-nnue",
    version="0.1.0",
    packages=find_packages(),
    install_requires=[
        "numpy",
        "logging",
    ]
)