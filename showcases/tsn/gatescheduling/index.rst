Gate Scheduling
===============

In time-aware shaping, gate schedules (i.e. when the gates corresponding to different traffic categories are open or closed) can be specified manually. This might be sufficient in some simple cases.
However, in complex cases, manually calculating gate schedules may be impossible, thus automation may be required. Gate schedule configurators can be used for this purpose.

One needs to specify constraints for the different traffic categories, such as maximum delay,
and the configurator automatically calculates and configures the gate schedules that satisfy
these constraints.

Currently, INET contains three gate scheduling configurator models:

- :ned:`EagerGateScheduleConfigurator`: This is a simple model, which iterates over traffic classes in order of priority, and sets gate schedules. Might not be able to find a solution that the other configurators are
- :ned:`TSNschedGateScheduleConfigurator` and :ned:`Z3GateScheduleConfigurator`: These use a SAT-solver-based method to find solutions that satisfy the requirements

The following showcases demonstrate gate scheduling for time-aware shaping:

.. toctree::
   :maxdepth: 1

   eager/doc/index
   sat/doc/index
   tsnsched/doc/index
