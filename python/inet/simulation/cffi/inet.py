import cppyy

from inet.common.util import *

# setup inet include path
cppyy.add_include_path(get_inet_relative_path("src"))

# setup inet library path
cppyy.add_library_path(get_inet_relative_path("src"))

# load inet library
cppyy.load_library("libINET")

# setup inet namespace
# from cppyy.gbl import inet
