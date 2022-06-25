TSNsched-based Gate Scheduling
==============================

Goals
-----

This showcase demonstrates the TSNsched gate schedule configurator that solves the autoconfiguration problem with an external SAT solver tool.

| INET version: ``4.4``
| Source files location: `inet/showcases/tsn/gatescheduling/tsnsched <https://github.com/inet-framework/tree/master/showcases/tsn/gatescheduling/tsnsched>`__

The Model
---------

The :ned:`TsnSchedGateScheduleConfigurator` module uses the external Z3 SAT solver tool (check the NED documentation the configurator for installation
instructions). The module has the same parameters as the other gate schedule configurators.

The simulation uses the same network as the showcases for the Eager and SAT configurators:

.. figure:: media/Network.png
    :align: center

Here is the configuration:

.. literalinclude:: ../omnetpp.ini
    :language: ini

Results
-------

Here is a gate

.. figure:: media/seqchart.png
    :align: center

.. figure:: media/seqchart2.png
    :align: center

Sources: :download:`omnetpp.ini <../omnetpp.ini>`

Discussion
----------

Use `this <https://github.com/inet-framework/inet/discussions/TODO>`__ page in the GitHub issue tracker for commenting on this showcase.

