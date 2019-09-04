Step 5. Using 3D models for network nodes
=========================================

Goals
-----

Although the environment is similar to the real world, customizing
network nodes may further ease the understanding of the simulation. In
this step, we replace default icon of the nodes with an external 3D osg
model.

The model
---------

So far, we have only dealt with the environment of the simulation. In this
step, we add network nodes to the simulation model. We add two
:ned:`WirelessHost`, ``pedestrian0`` and ``pedestrian1`` and one
:ned:`AccessPoint`, ``accessPoint0``. The pedestrians will communicate with
each other via ``accessPoint0`` using a VoIP application. The communication
will be configured in later steps.

The following picture shows what the network looks like and what the
default appearance of the :ned:`WirelessHost` nodes looks like.

.. figure:: media/step5_model_default_appearance_2d.png
   :width: 100%

To use a 3D model instead of a 2D icon, set the :par:`osgModel` parameter
of the network node. Examine the following code snippet.

.. literalinclude:: ../omnetpp.ini
   :language: ini
   :start-at: [Config Visualization05]
   :end-before: #---

The name *boxman.osgb* is the name of the external 3D model
and the *(0.05).scale* means that size of the external 3D model is decreased to 5%.

By default, appearance of the nodes is the same in 2D and 3D
visualization. The default icon of :ned:`WirelessHost` is a laptop.
We can change the 3D representation of the nodes as explained abow. The
icon representing the nodes in a 2D (and in 3D if :par:`osgModel` is not set)
can be replaced by modifying the display string of a network node. In 2D
visualization, the laptop icon is replaced by callphone icon, because
it represents pedestrians better.

The following code snippet shows :ned:`VisualizationD` network which is
used for this step.

.. literalinclude:: ../VisualizationD.ned
   :language: ned
   :start-at: network VisualizationD

An :ned:`Ipv4NetworkConfigurator` module instance and an
:ned:`Ieee80211ScalarRadioMedium` module instance are also added to the
model. We will use them as ``configurator`` and ``radioMedium``. The
``configurator`` module assigns IP addresses and sets up static routing
for an IPv4 network. The ``radioMedium`` module is a radio medium model which uses
scalar transmission power in the analog representation.

Results
-------
.. 3D visualization
The following videos show the resulted 3D representation of the playground
and the network:

.. video:: media/step5_result_3d_enc.mp4
   :width: 100%

.. video:: media/step5_result_3d_v2_enc.mp4
   :width: 100%

.. 2D visualization
The 2D representation looks like the following:

.. figure:: media/step5_result_2d.png
   :width: 100%

Sources: :download:`omnetpp.ini <../omnetpp.ini>`,
:download:`VisualizationD.ned <../VisualizationD.ned>`

