SAT Solver-based Gate Scheduling
================================

Goals
-----

This showcase demonstrates the SAT Solver-based gate scheduling configurator TODO

| INET version: ``4.4``
| Source files location: `inet/showcases/tsn/gatescheduling/sat <https://github.com/inet-framework/tree/master/showcases/tsn/gatescheduling/sat>`__

The Model
---------

The simulation uses the following network:

.. figure:: media/Network.png
    :align: center

Here is the configuration:

.. literalinclude:: ../omnetpp.ini
    :language: ini

Results
-------

A gate cycle of 1ms is displayed on the following sequence chart. Note the time efficiency of the gate schedules:

.. figure:: media/seqchart.png
    :align: center

The application end-to-end delay for the different traffic classes is displayed on the following chart:

.. figure:: media/delay.png
    :align: center

The delay is constant for every packet, and within the specified constraint of 500us.

The next sequence chart excerpt displays one packet as it travels from the packet source to the packet sink, with the delay indicated:

.. figure:: media/seqchart_delay.png
    :align: center

All packets have the exact same delay, which can be calculated analytically: ``(propagation time + transmission time) * 3`` (queueing time is zero).
Inserting the values of 86.64us transmission time and 0.05us propagation time per link, the delay is 260us for the best effort traffic category. **TODO** its apparently not 

The following charts compare the SAT-based and Eager gate scheduling configurators in terms of application end-to-end delay:

.. figure:: media/delay_comparison_besteffort.png
    :align: center

.. figure:: media/delay_comparison_video.png
    :align: center

The difference is that in case of the SAT-based gate scheduling configurator, all packets have the same constant delay; the eager configurator produces
outliers.

Sources: :download:`omnetpp.ini <../omnetpp.ini>`

Discussion
----------

Use `this <https://github.com/inet-framework/inet/discussions/TODO>`__ page in the GitHub issue tracker for commenting on this showcase.

