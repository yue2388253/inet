import os

from omnetpp.common.util import *

from inet.common.util import *

omnetpp_project_path = get_omnetpp_relative_path(".")
inet_project_path = get_inet_relative_path(".")

if omnetpp_project_path is not None and inet_project_path is not None and \
   os.path.exists(omnetpp_project_path) and os.path.exists(inet_project_path) and \
   os.path.exists(os.path.join(omnetpp_project_path, "include")) and os.path.exists(os.path.join(inet_project_path, "src")):
    from inet.simulation.cffi.event import *
    from inet.simulation.cffi.inet import *
    from inet.simulation.cffi.inprocess import *
    from inet.simulation.cffi.omnetpp import *

__all__ = [k for k,v in locals().items() if k[0] != "_" and v.__class__.__name__ != "module"]
