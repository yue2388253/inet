from omnetpp.simulation.project import *
from omnetpp.common.util import *

from inet.common.util import *

inet_project = define_simulation_project("inet",
                                         directory=get_inet_relative_path("."),
                                         executable=get_omnetpp_relative_path("bin/opp_run"),
                                         libraries=["src/INET"],
                                         ned_folders=["src", "examples", "showcases", "tutorials", "tests/networks", "tests/validation"],
                                         ned_exclusions=[s.strip() for s in open(get_inet_relative_path(".nedexclusions")).readlines()],
                                         ini_file_folders=["examples", "showcases", "tutorials", "tests/fingerprint", "tests/validation"],
                                         image_folders=["images"])

inet_baseline_project = define_simulation_project("inet-baseline",
                                                  directory=get_inet_relative_path("../inet-baseline"),
                                                  executable=inet_project.executable,
                                                  libraries=inet_project.libraries,
                                                  ned_folders=inet_project.ned_folders,
                                                  ini_file_folders=inet_project.ini_file_folders,
                                                  image_folders=inet_project.image_folders)

set_default_simulation_project(inet_project)
