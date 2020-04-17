from setuptools import Extension, setup
from Cython.Build import cythonize

extensions_debug = [
    Extension("words_finder", ["words_finder.pyx"],
        include_dirs=["../WordsLookup"], 
        libraries=["WordsLookup"],
        library_dirs=["../x64/Debug"], 
        extra_compile_args=["/Od", "/MTd", "/D_DEBUG"], 
        undef_macros=["NDEBUG"])
]
extensions_release = [
    Extension("words_finder", ["words_finder.pyx"],
        include_dirs=["../WordsLookup"], 
        libraries=["WordsLookup"],
        library_dirs=["../x64/Release"], 
        extra_compile_args=["/MT"])
]
setup(
    name="words_finder_lib",
    ext_modules=cythonize(extensions_release),
)
