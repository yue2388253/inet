Step 3. Showing the playground
==============================

Goals
-----

It is possible to express the position of network nodes, the
movement of mobile nodes, the geometry of physical objects using
geographic locations. However, internally, INET uses a Cartesian coordinate system
called the playground. Such coordinates may appear in the runtime GUI or
the simulation log or even in the debugger. For easier understanding of
the playground and its coordinate system, in this step we go further into details
with these.

The model
---------

As mentioned in ``Step 1.``, the size of the playground is 800x800 meters and is set via display
string. In 2D visualization, the playground is visualized as a grey
square. In 3D visualization the playground color is *light green*, but
it is opaque by default so it is not visible. By decreasing the
:par:`sceneOpacity`, the playground becomes visible. In addition, the
:par:`sceneColor` parameter is set to *black* because it fits better with the current
map.

The playground is placed on the map according to the coordinate system
module. The position of the origin of the playground is set in the previous step.
The axes are not visible because their length is 0m by default. We can
display them by setting their length to a positive number.

This step is configured as follows:

.. literalinclude:: ../omnetpp.ini
   :language: ini
   :start-at: [Config Visualization03]
   :end-before: #---

The length of the axes is set to 100m and their origin is set 0.5m above the playground's
level.

Results
-------

.. figure:: media/step3_result_playground.png
   :width: 100%

Sources: :download:`omnetpp.ini <../omnetpp.ini>`,
:download:`VisualizationB.ned <../VisualizationB.ned>`
