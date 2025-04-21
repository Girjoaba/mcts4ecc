"""Module containing the root and main view for the Scoutbook application.

These are the components that are required to have a first window for our application.
"""

from src.views import config
from src.views.composite_tabs import CodeTab

import tkinter as tk
from tkinter import ttk


class RootSB(tk.Tk):
    """The root of the user interface for the Scoutbook. It sets the title and the starting resolution."""
    def __init__(self, title: str = 'Placeholder Title', resolution: str = '800x600'):
        """
        Initializes the root object with the title and resolution.
        :param title: the title of the window.
        :param resolution: the resolution of the window.
        """
        super().__init__()
        self.title(title)
        self.geometry(resolution)


class MainViewSB(ttk.Notebook):
    """The main view for the Scoutbook. It needs the root object to be passed in order to be attached.

    It manages the tabs by determining their style and adding them to the view.
    """
    def __init__(self, root: tk.Tk):
        """
        Initializes the main view with the root object.
        :param root: a tkinter root object.
        """
        self.__create_style()
        super().__init__(root)
        self.pack(expand=True, fill='both')

    @staticmethod
    def __create_style():
        style = ttk.Style()
        style.configure('TNotebook', tabposition='s')
        style.configure('TNotebook.Tab', font=(config.FONT_TYPEFACE_SB, '11'), padding=[5, 5, 5, 5])

    def add_tab(self, tab: CodeTab) -> None:
        """
        Adds a tab to the main view.
        :param tab: the tab to be added.
        """
        self.add(tab, text=tab.title)
