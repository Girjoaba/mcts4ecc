"""Module containing the entry point of the Scoutbook application.

It is built from the scoutbook view, the ecc controller and the ecc module that performs the error detection and
correction operations.
"""

import sys
import os

# Add the parent directory of 'src' to the system path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../..')))

from src.views import composite_root as root
from src.views import composite_tabs as tabs
from src.views import config

# Create the root window and the main view
root_obj = root.RootSB(title=config.TITLE_SB, resolution=config.STARTING_RESOLUTION_SB)
scoutbook = root.MainViewSB(root_obj)

# Add the tabs
scoutbook.add_tab(tabs.TabN22K16(scoutbook))
scoutbook.add_tab(tabs.TabN39K32(scoutbook))
scoutbook.add_tab(tabs.TabHsiao(scoutbook))
scoutbook.add_tab(tabs.TabNewLi(scoutbook))

# Start the main event loop
root_obj.mainloop()
