Comparison of Bayesian networks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. figure:: _static/BNcomparison.png
    :align: center
    :alt: tools for comparing Bayesian networks

To compare Bayesian network, one can compare the structure of the BNs
(see pyagrum.lib.bn_vs_vb.GraphicalBNComparator).
However BNs can also be compared as probability distributions.

.. autoclass:: pyagrum.ExactBNdistance


.. autoclass:: pyagrum.GibbsBNdistance
