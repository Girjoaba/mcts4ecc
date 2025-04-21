""" Module to bridge the GUI view to the Error Correction and Detection (EDAC) module.

This module provides controllers that bridge the widgets to the functions of the EDAC.
"""

from src.ecc import cyclic_avoidance_codes as cac
from src.ecc import flags as flg

import tkinter as tk
from tkinter import ttk
from abc import ABC
import numpy as np


def str2arr(string: str) -> np.ndarray:
    """
    Converts a string to a numpy array.
    :param string: must contain only number characters.
    :return: all the number characters place in a numpy array.
    """
    return np.array([int(char) for char in string])


def arr2str(arr: np.ndarray) -> str:
    """
    Converts a numpy array to a string.
    :param arr: the converted array.
    :return: all the elements in the array concatenated.
    """
    return ''.join([str(bit) for bit in arr])


def mat2str(mat: np.ndarray) -> str:
    """
    Converts a matrix to a string.
    :param mat: the matrix to be converted.
    :return: all the elements in the array concatenated. Each row is separated by a newline.
    """
    return '\n'.join([' '.join([str(bit) for bit in row]) for row in mat])


class PipelineController(ABC):
    """Abstract class to control the elements in the GUI."""

    def __init__(self, code: cac.BlockCode) -> None:
        """Initializes the controller with the given EDAC code."""
        self.code = code

    def encode_word(self, data_entry: ttk.Entry, encoded_entry: ttk.Entry) -> None:
        """
        Encodes the data in the data entry and sets the result in the encoded entry.
        :param data_entry: the entry in which the user is writing the bits of his data.
        :param encoded_entry: the entry in which teh bits are encoded s.t. they can be protected against errors.
        """
        data_word_arr = str2arr(data_entry.get())
        encoded_word_arr = self.code.encode(data_word_arr)
        encoded_word = arr2str(encoded_word_arr)
        encoded_entry.delete(0, tk.END)
        encoded_entry.insert(0, encoded_word)

    def get_syndrome(self, code_entry: ttk.Entry, syndrome_entry: ttk.Entry) -> None:
        """
        Gets the syndrome of the code word in the code entry and sets it in the syndrome entry.
        :param code_entry: the entry in the coded word is placed. The user can modify the bits to introduce errors.
        :param syndrome_entry: the entry in which the syndrome will be displayed.
        """
        code_word_arr = str2arr(code_entry.get())
        syndrome_arr = self.code.syndrome(code_word_arr)
        syndrome = arr2str(syndrome_arr)
        syndrome_entry.delete(0, tk.END)
        syndrome_entry.insert(0, syndrome)

    def decode_word(self, data_entry: ttk.Entry, code_entry: ttk.Entry, decoded_label: ttk.Label) -> None:
        """
        Decodes the code word in the code entry and sets the result in the decoded label.
        :param data_entry: the original entry of the data word.
        :param code_entry: the entry which contains the encoded word.
        :param decoded_label: the label which displays the result of the correction.
        :return:
        """
        # Get original data word
        data_word_arr = str2arr(data_entry.get())
        original_word_arr = self.code.encode(data_word_arr)
        original_word = arr2str(original_word_arr)

        # Correct the encoded one
        code_word = str2arr(code_entry.get())
        corrected_word_arr = self.code.decode(code_word)

        # Check if UE occurred
        if isinstance(corrected_word_arr, int) and corrected_word_arr == flg.UE_FLAG:
            output = 'An Uncorrectable Error was detected.\n' \
                     'Better detected than incorrectly corrected.\n'
        else:
            decoded_word = arr2str(corrected_word_arr)
            if decoded_word == original_word:
                output = f'Successfully corrected word: {decoded_word}\n\n'
            else:
                output = f'Incorrect correction:\n' \
                         f'Original word:    {original_word}\n' \
                         f'Corrected word: {decoded_word}'
        decoded_label.config(text=output)

    def set_g_matrix(self, g_mat_label: ttk.Label) -> None:
        """
        Sets the G matrix in the given label.
        :param g_mat_label: the label in which the G matrix will be displayed.
        """
        g_str = mat2str(self.code.g_matrix)
        g_mat_label.config(text=g_str)

    def set_h_matrix(self, h_mat_label: ttk.Label) -> None:
        """
        Sets the H matrix in the given label.
        :param h_mat_label: the label in which the H matrix will be displayed.
        """
        h_str = mat2str(self.code.h_matrix)
        h_mat_label.config(text=h_str)


class N22K16Controller(PipelineController):
    """Controller for the N22K16 code."""

    def __init__(self):
        super().__init__(cac.CodeN22K16())


class N39K32Controller(PipelineController):
    """Controller for the N39K32 code."""

    def __init__(self):
        super().__init__(cac.CodeN39K32())


class HsiaoController(PipelineController):
    """Controller for the Hsiao code."""

    def __init__(self):
        super().__init__(cac.HsiaoCodeN22K16())


class NewDuttaController(PipelineController):
    """Controller for our improved Dutta N22K16 code."""

    def __init__(self):
        super().__init__(cac.NewDuttaN22K16())

class NewLiControllerN39K32(PipelineController):
    """Controller for our imporved Li N39K32 code."""

    def __init__(self) -> None:
        super().__init__(cac.NewLiN39K32())
