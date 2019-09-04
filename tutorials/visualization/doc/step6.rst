Step 6. Displaying movement of nodes
====================================

Goals
-----

In wireless networks with mobile nodes, mobility often affects network
operation. For this reason, visualizing mobility of nodes is essential.
INET visualizer modules not only support the visualization of nodes
at specific locations, but allows to display other properties of the movement
like direction, speed, velocity, orientation and track. In this step
we introduce the visualization of the movement of mobile network nodes.

The model
---------

Movement of mobile nodes is displayed by default but often we need more
details about the recent and the upcoming movements.

Playground
~~~~~~~~~~

This step also uses the VisualizationD network
(:download:`VisualizationD.ned <../VisualizationD.ned>`).

.. figure:: media/step5_model_default_appearance_2d.png
   :width: 100%

Mobility settings
~~~~~~~~~~~~~~~~~

At first, we have to set mobility of network nodes:

.. literalinclude:: ../omnetpp.ini
   :language: ini
   :start-at: # mobility settings
   :end-before: # mobility settings end

The :par:`initFromDisplayString` parameter is set to false so that
the values regarding the initial position of a node specified in its display string
do not affect the mobility model. Initial position can be set to
all nodes by using - :par:`initialX`, :par:`initialY` and :par:`initialZ` - or
:par:`initialLatitude`, :par:`initialLongitude` and :par:`initialAltitude`
parameters. The ``accessPoint0`` module is set to have :ned:`StationaryMobility`,
meaning that its position remains stationary during the simulation. The
pedestrians mobility module is set to :ned:`MassMobility`.
This is a random mobility model for a mobile host with a mass. Besides the speed
of the mobility, the change interval and the angle, by which the direction of
the movement is changing, are specified. For more information about
mobility models, please read the 
`showcases <https://inet.omnetpp.org/docs/showcases/mobility/index.html>`__
regarding this topic.


Pedestrians movement area is restricted by :par:`constraintAreaMinX`,
:par:`constraintAreaMaxX`, :par:`constraintAreaMinY` and
:par:`constraintAreaMaxY` (also :par:`constraintAreaMinZ`, :par:`constraintAreaMaxZ`
in case of 3D mobility) to prevent pedestrians roaming out of the
communication range of ``accessPoint0``.

The following video displays nodes' mobility using default
visualization.

.. video:: media/step6_model_2d.mp4
   :width: 100%

Visualization settings
~~~~~~~~~~~~~~~~~~~~~~
.. a velocity-re meg az orientation-re szukseg van?

Movement of nodes is visualized by default, but other useful
informations can also be visualized such as velocity, orientation and
movement trail. We enable these features by setting
:par:`displayVelocities`, :par:`displayOrientations` and
:par:`displayMovementTrails` to true. The pedestrians are moving slowly so
the length of velocity vector is too small. For this reason, we multiply
the length of velocity vector by the value of :par:`velocityArrowScale`.

The following code snippet shows how these features are configured.

.. literalinclude:: ../omnetpp.ini
   :language: ini
   :start-at: # displaying movements
   :end-before: #---

Results
-------

The following video shows the resulted 2D visualization of the simulation:

.. video:: media/step6_result_2d.mp4
   :width: 100%

.. todo::

   ![](step07_moving_2d.gif)
   ![](step5_result3.gif)
   It is advisable to run the simulation in Fast mode, because the nodes move very slowly if viewed in Normal mode.
   It can be seen in the animation below <i>pedestrian0</i> and <i>pedestrian1</i> roam in the park between invisible borders that we adjust to them.
   Here's that in Module view mode:
   And here's that in 3D Scene view mode:


Sources: :download:`omnetpp.ini <../omnetpp.ini>`, :download:`VisualizationD.ned <../VisualizationD.ned>`
