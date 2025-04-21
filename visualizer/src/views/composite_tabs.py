"""Module containing the tabs for the Scoutbook application.

The code is structured by a main abstract CodeTab class that is inherited by the specific code tabs. Each tab can
override the private functions to customize the tab to the specific code.
"""

from src.views import config
from src.controllers import edac_controller as controller

import tkinter as tk
from tkinter import ttk
from abc import ABC


class CodeTab(ABC, ttk.Frame):
    """Abstract class for the different code tabs in the Scoutbook application.

    If you need a new tab must be customizable further, you can override the necessary functions."""

    def __init__(self, notebook: ttk.Notebook) -> None:
        """
        Constructor for the CodeTab class.

        It contains default values for all fields that should be customized by the specific code tabs.
        :param notebook: the tkinter notebook to add the tab to.
        """
        super().__init__(notebook)
        self.pack(fill='both', expand=True)
        self._init_grid_weights()
        # Default Specific code values
        self._controller = None
        self._title: str = 'Lorem Ipsum'
        self._code_description: str = 'Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam nec '
        self._default_dataword: str = '1000101'
        self._entry_width: int = 24

    @property
    def title(self) -> str:
        return self._title

    # ------------------------------------------------------------ #
    # ----------------------------- Info  ------------------------ #
    # ------------------------------------------------------------ #

    def _init_grid_weights(self) -> None:
        self.grid_rowconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)
        self.grid_rowconfigure(2, weight=1)
        self.grid_columnconfigure(0, weight=2)
        self.grid_columnconfigure(1, weight=1)
        self.grid_columnconfigure(2, weight=1)

    def _create_code_description(self) -> None:
        """
        Creates the general description of the code in the top left corner.
        """
        code_description_label = ttk.Label(self, text=self._code_description,
                                           wraplength=300,
                                           font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB))
        code_description_label.grid(row=0, column=0, rowspan=2, padx=10, pady=20, sticky='nsew')

    def _create_generator_matrix(self) -> ttk.Label:
        """
        Creates the generator matrix label on the left side.
        :return: the label containing the bits of the generator matrix.
        """
        g_frame = ttk.Frame(self)
        g_frame.grid(row=2, column=0, rowspan=2, padx=30, pady=20, sticky='nsew')
        g_label = ttk.Label(g_frame, text='G = ',
                            font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_MATRIX_SB, 'bold'))
        g_label.grid(row=0, column=0, sticky='nsew')
        separator_left = ttk.Separator(g_frame, orient='vertical')
        separator_left.grid(row=0, column=1, sticky='ns')

        g_matrix = ttk.Label(g_frame, text='010101\n010101\n110101',
                             font=(config.FONT_TYPEFACE_SB, config.MAT_FONT_SIZE_SB))
        g_matrix.grid(row=0, column=2, sticky='n'
                                              'sew')
        separator_right = ttk.Separator(g_frame, orient='vertical')
        separator_right.grid(row=0, column=3, sticky='ns')

        return g_matrix

    def _create_h_matrix(self) -> ttk.Label:
        """
        Creates the parity check matrix label in the bottom left corner.
        :return: the label containing the bits of the parity check matrix.
        """
        h_frame = ttk.Frame(self)
        h_frame.grid(row=4, column=0, padx=30, pady=20, sticky='nsew')
        h_label = ttk.Label(h_frame, text='H = ',
                            font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_MATRIX_SB, 'bold'))
        h_label.grid(row=0, column=0, sticky='nsew')
        separator_left = ttk.Separator(h_frame, orient='vertical')
        separator_left.grid(row=0, column=1, sticky='ns')

        h_matrix = ttk.Label(h_frame, text='011101\n000101\n110111',
                             font=(config.FONT_TYPEFACE_SB, config.MAT_FONT_SIZE_SB))
        h_matrix.grid(row=0, column=2, sticky='nsew')

        separator_right = ttk.Separator(h_frame, orient='vertical')
        separator_right.grid(row=0, column=3, sticky='ns')

        return h_matrix

    def _create_separator(self) -> None:
        """
        Creates a vertical separator between the info and the correction pipeline.
        """
        separator = ttk.Separator(self, orient='vertical')
        separator.grid(row=0, column=1, rowspan=5, sticky='nsw')

    # ------------------------------------------------------------ #
    # -------------------------- Entries  ------------------------ #
    # ------------------------------------------------------------ #

    def _create_data_word_entry(self) -> ttk.Entry:
        """
        Creates the data word entry field. Where the user can write the bits he wants to protect.
        :return: the entry field.
        """
        data_word_frame = ttk.Frame(self)
        data_word_frame.grid(row=1, column=2, padx=10, pady=20, sticky='sew')

        dw_label = ttk.Label(data_word_frame, text='Data Word',
                             font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB))
        dw_label.grid(row=0, column=0, sticky='ns')

        dw_entry = ttk.Entry(data_word_frame, font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB),
                             width=self._entry_width)
        dw_entry.grid(row=1, column=0, sticky='nsew')
        dw_entry.insert(0, self._default_dataword)

        dw_G_label = ttk.Label(data_word_frame, text='x G',
                               font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB, 'italic'))
        dw_G_label.grid(row=1, column=1, sticky='nsew')
        dw_arrow = ttk.Label(data_word_frame, text='||\nV',
                             font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB, 'bold'))
        dw_arrow.grid(row=2, column=0, sticky='nse')

        return dw_entry

    def _create_code_word_entry(self) -> ttk.Entry:
        """
        Creates the code word entry field. Where the user can see the encoded word and introduce errors.
        :return: the entry field.
        """
        code_word_frame = ttk.Frame(self)
        code_word_frame.grid(row=2, column=2, padx=10, pady=20, sticky='nsew')

        cw_label = ttk.Label(code_word_frame, text='Encoded Word',
                             font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB))
        cw_label.grid(row=0, column=0, sticky='ns')

        cw_entry = ttk.Entry(code_word_frame, font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB),
                             width=self._entry_width)
        cw_entry.grid(row=1, column=0, sticky='nsew')

        cw_G_label = ttk.Label(code_word_frame, text='x H transpose',
                               font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB, 'italic'))
        cw_G_label.grid(row=1, column=1, sticky='nsew')
        cw_arrow = ttk.Label(code_word_frame, text='||\nV',
                             font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB, 'bold'))
        cw_arrow.grid(row=2, column=0, sticky='nse')

        return cw_entry

    def _create_syndrome_entry(self) -> ttk.Entry:
        """
        Creates the syndrome entry field. Where the user can see the syndrome of the code word.
        :return: the entry field.
        """
        syndrome_frame = ttk.Frame(self)
        syndrome_frame.grid(row=3, column=2, padx=10, pady=20, sticky='nsew')

        s_label = ttk.Label(syndrome_frame, text='Syndrome',
                            font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB))
        s_label.grid(row=0, column=0, sticky='ns')

        s_entry = ttk.Entry(syndrome_frame, font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB),
                            width=self._entry_width)
        s_entry.grid(row=1, column=0, sticky='nsew')

        s_G_label = ttk.Label(syndrome_frame, text='we look for the syndrome in H',
                              font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB))
        s_G_label.grid(row=1, column=1, sticky='nsew')
        s_arrow = ttk.Label(syndrome_frame, text='||\nV',
                            font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB, 'bold'))
        s_arrow.grid(row=2, column=0, sticky='nse')

        return s_entry

    def _create_outcome_label(self) -> ttk.Label:
        """
        Creates the outcome label. Where the user can see the outcome of the error correction pipeline.
        :return: the label that displays the outcome.
        """
        o_frame = ttk.Frame(self)
        o_frame.grid(row=4, column=2, padx=10, pady=20, sticky='nsew')
        o_label = ttk.Label(o_frame, text='Execute the pipeline to see an outcome...\n\n',
                            font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB, 'bold'))
        o_label.grid(row=0, column=0, sticky='nsew')
        return o_label

    # ------------------------------------------------------------ #
    # -------------------------- Buttons  ------------------------ #
    # ------------------------------------------------------------ #

    def _create_encode_button(self) -> ttk.Button:
        """
        Creates the encode button. When clicked, it encodes the data word into the code word.
        :return: the button as a tkinter object that encodes the data word.
        """
        e_frame = ttk.Frame(self)
        e_frame.grid(row=1, column=3, padx=10, pady=20, sticky='sew')
        e_button = tk.Button(e_frame, text="Encode\nWord",
                             width=config.BUTTON_WIDTH_SB, height=config.BUTTON_HEIGHT_SB,
                             font=(config.BUTTON_TYPEFACE_SB, config.BUTTON_FONT_SIZE_SB))
        e_button.grid(row=0, column=0, sticky='nsew')

        return e_button

    def _create_syndrome_button(self) -> ttk.Button:
        """
        Creates the syndrome button. When clicked, it calculates the syndrome of the code word.
        :return: the button as a tkinter object that calculates the syndrome.
        """
        sb_frame = ttk.Frame(self)
        sb_frame.grid(row=2, column=3, padx=10, pady=20, sticky='sew')
        sb_button = tk.Button(sb_frame, text="Get\nSyndrome",
                              width=config.BUTTON_WIDTH_SB, height=config.BUTTON_HEIGHT_SB,
                              font=(config.BUTTON_TYPEFACE_SB, config.BUTTON_FONT_SIZE_SB))  # command=on_button_click
        sb_button.grid(row=0, column=0, sticky='nsew')

        return sb_button

    def _create_correct_button(self) -> ttk.Button:
        """
        Creates the correct button. When clicked, it corrects the errors in the code word.
        :return: the button as a tkinter object that corrects the errors.
        """
        cb_frame = ttk.Frame(self)
        cb_frame.grid(row=3, column=3, padx=10, pady=20, sticky='sew')
        cb_button = tk.Button(cb_frame, text="Correct\nErrors",
                              width=config.BUTTON_WIDTH_SB, height=config.BUTTON_HEIGHT_SB,
                              font=(config.BUTTON_TYPEFACE_SB, config.BUTTON_FONT_SIZE_SB))  # command=on_button_click
        cb_button.grid(row=0, column=0, sticky='nsew')

        return cb_button

    def _create_title(self) -> None:
        """
        Creates the title of the tab.
        """
        t_frame = ttk.Frame(self)
        t_frame.grid(row=0, column=2, columnspan=2, padx=50, pady=20, sticky='nsew')
        t_label = ttk.Label(t_frame, text=self._title + ' Code',
                            font=(config.TITLE_TYPEFACE_SB, config.TITLE_FONT_SIZE_SB, 'bold'))
        t_label.grid(row=0, column=0, sticky='nsew')

    def init_default_setup(self) -> None:
        """
        Initializes a default setup/layout of the tab.
        """
        # Info
        self._create_code_description()
        g_mat_label = self._create_generator_matrix()
        h_mat_label = self._create_h_matrix()
        self._create_separator()
        # Fields
        dw_entry = self._create_data_word_entry()
        cw_entry = self._create_code_word_entry()
        s_entry = self._create_syndrome_entry()
        o_label = self._create_outcome_label()
        # Buttons
        e_button = self._create_encode_button()
        s_button = self._create_syndrome_button()
        c_button = self._create_correct_button()
        # Title
        self._create_title()

        # Bindings
        e_button.bind('<Button-1>', lambda e: self._controller.encode_word(dw_entry, cw_entry))
        s_button.bind('<Button-1>', lambda e: self._controller.get_syndrome(cw_entry, s_entry))
        c_button.bind('<Button-1>', lambda e: self._controller.decode_word(dw_entry, cw_entry, o_label))
        self._init_mats(g_mat_label, h_mat_label)

    def _init_mats(self, g_mat: ttk.Label, h_mat: ttk.Label) -> None:
        """
        Initializes the values of the matrices that are displayed in the information part of the tab.
        :param g_mat: the generator matrix.
        :param h_mat: the parity check matrix.
        """
        self._controller.set_g_matrix(g_mat)
        self._controller.set_h_matrix(h_mat)


class TabN22K16(CodeTab):
    """Tab for the N22K16 code."""

    def __init__(self, notebook: ttk.Notebook):
        super().__init__(notebook)
        self._controller = controller.N22K16Controller()
        self._title = 'N22K16'
        self._code_description = 'The N22K16 code is a cyclic avoidance code used for error detection and error' \
                                 ' correction of randomly corrupted bits. It can detect up to 2 errors and correctly' \
                                 ' correct any individual error. What makes this code special is that it can also' \
                                 ' correct double adjacent errors.'
        self._default_dataword = '1111000011110000'
        self._entry_width = 24
        self.init_default_setup()


class TabN39K32(CodeTab):
    """Tab for the N39K32 code."""

    def __init__(self, notebook: ttk.Notebook):
        super().__init__(notebook)
        self._controller = controller.N39K32Controller()
        self._title = 'N39K32'
        self._code_description = 'The N39K32 code is an extended version of N22K16. It is able to take datawords' \
                                 'of 32 bits in length and encode them into 39 bits. It has the same correction' \
                                 'and detection capabilities as N22K16, single error correction, double error ' \
                                 'detection, and double adjacent error correction.'
        self._default_dataword = '11110000111100001111000011110000'
        self._entry_width = 41
        self.init_default_setup()

    def _create_generator_matrix(self) -> ttk.Label:
        """
        Creates a specific default value for the N39K32 generator matrix. The full matrix is too large to display.
        :return: the label containing the bits of the generator matrix.
        """
        g_frame = ttk.Frame(self)
        g_frame.grid(row=2, column=0, padx=30, pady=20, sticky='nsew')
        g_label = ttk.Label(g_frame, text='G = ',
                            font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_MATRIX_SB, 'bold'))
        g_label.grid(row=0, column=0, sticky='nsew')
        separator_left = ttk.Separator(g_frame, orient='vertical')
        separator_left.grid(row=0, column=1, sticky='ns')

        g_matrix = ttk.Label(g_frame, text='Identity\t (+)     Systematic Part\n'
                                           ' 32x32\t             32x7',
                             font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB))
        g_matrix.grid(row=0, column=2, sticky='n'
                                              'sew')
        separator_right = ttk.Separator(g_frame, orient='vertical')
        separator_right.grid(row=0, column=3, sticky='ns')

        return g_matrix

    def _init_mats(self, g_mat: ttk.Label, h_mat: ttk.Label) -> None:
        """
        For the N39K32 code, the generator matrix is too large to display normally.
        :param g_mat: the generator matrix label as a tkinter object.
        :param h_mat: the parity check matrix label as a tkinter object.
        """
        self._controller.set_h_matrix(h_mat)
        h_mat.config(font=(config.FONT_TYPEFACE_SB, config.MAT_FONT_SIZE_SB - 1))


class TabHsiao(CodeTab):
    """Tab for the Hsiao code."""

    def __init__(self, notebook: ttk.Notebook):
        super().__init__(notebook)
        self._controller = controller.HsiaoController()
        self._title = 'Hsiao'
        self._code_description = 'The Hsiao code is a SEC-DED code. The paper describing it was published in 1970 and ' \
                                 'is provided as a an example of a non double adjacent error correction code.'
        self._default_dataword = '1111000011110000'
        self._entry_width = 24
        self.init_default_setup()


class TabNewLi(CodeTab):
    """Tab for new Li"""

    def __init__(self, notebook: ttk.Notebook):
        super().__init__(notebook)
        self._controller = controller.NewLiControllerN39K32()
        self._title = 'Our Li N39K32'
        self._code_description = 'N39K32 paper visualizer.'
        self._default_dataword = '11110000111100001111000011110000'
        self._entry_width = 42
        self.init_default_setup()

    def _create_generator_matrix(self) -> ttk.Label:
        g_frame = ttk.Frame(self)
        g_frame.grid(row=2, column=0, padx=30, pady=20, sticky='nsew')
        g_label = ttk.Label(g_frame, text='G = ',
                            font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_MATRIX_SB, 'bold'))
        g_label.grid(row=0, column=0, sticky='nsew')
        separator_left = ttk.Separator(g_frame, orient='vertical')
        separator_left.grid(row=0, column=1, sticky='ns')

        g_matrix = ttk.Label(g_frame, text='Identity (+) Systematic Part\n'
                                           ' 32x32          32x7',
                             font=(config.FONT_TYPEFACE_SB, config.FONT_SIZE_SB))
        g_matrix.grid(row=0, column=2, sticky='n'
                                              'sew')
        separator_right = ttk.Separator(g_frame, orient='vertical')
        separator_right.grid(row=0, column=3, sticky='ns')

        return g_matrix

    def _init_mats(self, g_mat: ttk.Label, h_mat: ttk.Label) -> None:
        """
        For the N39K32 code, the generator matrix is too large to display normally.
        :param g_mat: the generator matrix label as a tkinter object.
        :param h_mat: the parity check matrix label as a tkinter object.
        """
        self._controller.set_h_matrix(h_mat)
        h_mat.config(font=(config.FONT_TYPEFACE_SB, config.MAT_FONT_SIZE_SB - 1))

