"""Module containing the implementation of various block codes used in error detection and correction.

All of them are derived from an abstract class `BlockCode` that provides the basic methods to encode, decode and
calculate the syndrome.
"""

from src.ecc import gf2
import galois
from src.ecc.flags import UE_FLAG

from abc import ABC
from numpy.typing import NDArray
from typing import Union

import numpy as np


class BlockCode(ABC):
    """An abstract class that represents an error detection and correction code (EDAC).

    The operations to correct, encode and get the syndrome are implemented in this class since most codes share the
    same techniques.
    """

    def __init__(self) -> None:
        """
        Initializes the code with the matrices that define it. The generator matrix (G) and parity check matrix (H) are
        defined to perform the encoding and decoding operations. The adjacent syndrome matrix is used to correct
        adjacent errors.
        """
        self._p_systematic_part: NDArray[np.int8] = None
        self._g_matrix: NDArray[np.int8] = None
        self._h_matrix: NDArray[np.int8] = None
        self._adj_syndromes: NDArray[np.int8] = None

    @property
    def g_matrix(self) -> NDArray[np.int8]:
        return self._g_matrix

    @property
    def h_matrix(self) -> NDArray[np.int8]:
        return self._h_matrix

    @property
    def adj_syndromes(self) -> NDArray[np.int8]:
        return self._adj_syndromes

    def encode(self, data_word: NDArray[np.int8]) -> NDArray[np.int8]:
        """
        Encodes a word of K bits to a word of N bits.
        :param data_word: the data word as a bit array of length K that will be encoded to N bits.
        :return: the encoded N bits word.
        """
        return gf2.product_mod2(data_word, self._g_matrix)

    def decode(self, code_word: NDArray[np.int8]) -> Union[NDArray[np.int8], int]:
        """
        Corrects any random bit flips in the code word.
        :param code_word: the word that is checked for random bit flips.
        :return: the corrected code word or the flag that indicates an uncorrectable error. It can return an incorrectly
        corrected code word.
        """
        syndrome = self.syndrome(code_word)
        _h_T: np.ndarray = self._h_matrix.T
        corrected_code_word = code_word.copy()
        if np.all(syndrome == 0):  # No error
            return corrected_code_word

        # Correct 1 bit error
        for i in range(len(_h_T)):
            if np.array_equal(syndrome, _h_T[i]):
                corrected_code_word[i] = 1 - corrected_code_word[i]
                return corrected_code_word

        # Correct 2 adjacent bit error
        _adj_syndrome_T = self._adj_syndromes.T
        for i in range(len(_adj_syndrome_T)):
            if np.array_equal(syndrome, _adj_syndrome_T[i]):
                corrected_code_word[i] = 1 - corrected_code_word[i]
                corrected_code_word[i + 1] = 1 - corrected_code_word[i + 1]
                return corrected_code_word

        return UE_FLAG

    def syndrome(self, code_word: NDArray[np.int8]) -> NDArray[np.int8]:
        """
        Calculates the syndrome of a code word.
        :param code_word: the provided code word.
        :return: the syndrome as a numpy array.
        """
        result = gf2.product_mod2(code_word, self._h_matrix.T)
        return result


class CodeN22K16(BlockCode):
    """A (22, 16) cyclic avoidant code that can correct 1 bit errors, detect 2 bit errors
     and correct 2 adjacent bits errors.

     It was introduced by Dutta and Touba in 2007.
     """

    def __init__(self) -> None:
        """
        Initializes the code with the generator matrix, parity check matrix and the adjacent syndrome matrix.
        """
        super().__init__()
        self._p_systematic_part = np.array([[1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0],
                                            [0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1],
                                            [1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1],
                                            [1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0],
                                            [0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1],
                                            [0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0]]
                                           , dtype=np.int8)
        self._h_matrix = np.concatenate((self._p_systematic_part, np.identity(6, dtype=np.int8))
                                        , axis=1
                                        , dtype=np.int8)
        self._g_matrix = np.concatenate((np.identity(16, dtype=np.int8), self._p_systematic_part.transpose())
                                        , axis=1
                                        , dtype=np.int8)
        self._adj_syndromes = gf2.sum_adj_cols(self._h_matrix)

    def encode(self, data_word: NDArray[np.int8]) -> NDArray[np.int8]:
        """
        Encodes a 16 bits data word to a 22 bits codeword using the generator matrix.
        :param data_word: the data word as a bit array of length 16 that will be encoded to 22 bits.
        :return: the encoded 22 bits word.
        """
        if data_word.shape != (16,):
            raise ValueError('Failed to encode! data_word must be a 16x1 array.')
        return gf2.product_mod2(data_word, self._g_matrix)


class CodeN39K32(BlockCode):
    """A (39, 32) cyclic avoidant code that can correct 1 bit errors, detect 2 bit errors
     and correct 2 adjacent bits errors.

     It was introduced by Dutta and Touba in 2007.
     """

    def __init__(self) -> None:
        super().__init__()
        self._p_systematic_part = np.array([[1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
                                             1, 1, 1, 0, 0, 0, 0],
                                            [0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0,
                                             1, 1, 0, 0, 0, 0, 1],
                                            [1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1,
                                             1, 0, 0, 0, 0, 1, 1],
                                            [0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1,
                                             0, 0, 0, 0, 1, 1, 1],
                                            [0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0,
                                             0, 0, 0, 1, 1, 1, 0],
                                            [1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1,
                                             0, 0, 1, 1, 1, 0, 0],
                                            [0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0,
                                             0, 1, 1, 1, 0, 0, 0]]
                                           , dtype=np.int8)
        self._h_matrix = np.concatenate((self._p_systematic_part, np.identity(7, dtype=np.int8))
                                        , axis=1
                                        , dtype=np.int8)
        self._g_matrix = np.concatenate((np.identity(32, dtype=np.int8), self._p_systematic_part.transpose())
                                        , axis=1
                                        , dtype=np.int8)
        self._adj_syndromes = gf2.sum_adj_cols(self._h_matrix)

        self.n = 72
        self.k = 64

    def encode(self, data_word: NDArray[np.int8]) -> NDArray[np.int8]:
        """
        Encodes a 32 bits data word to a 39 bits codeword using the generator matrix.
        :param data_word: the data word as a bit array of length 32 that will be encoded to 22 bits.
        :return: the encoded 39 bits word.
        """
        if data_word.shape != (32,):
            raise ValueError('Failed to encode! data_word must be a 32x1 array.')
        return gf2.product_mod2(data_word, self._g_matrix)


class HsiaoCodeN22K16(BlockCode):
    """A (22, 16) hamming code that can correct 1 bit errors, detect 2 bit errors.

    It was introduced by Hsiao in 1970.
    """

    def __init__(self) -> None:
        super().__init__()
        self._p_systematic_part = np.array([[1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0],
                                            [1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0],
                                            [1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1],
                                            [0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1],
                                            [0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0],
                                            [0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1]]
                                           , dtype=np.int8)
        self._h_matrix = np.concatenate((self._p_systematic_part, np.identity(6, dtype=np.int8))
                                        , axis=1
                                        , dtype=np.int8)
        self._g_matrix = np.concatenate((np.identity(16, dtype=np.int8), self._p_systematic_part.transpose())
                                        , axis=1
                                        , dtype=np.int8)
        self._adj_syndromes = gf2.sum_adj_cols(self._h_matrix)


class NewLiN39K32(BlockCode):
    """Our improved Li N39K32 code, using MCTS."""

    def __init__(self) -> None:
        super().__init__()
        self._p_systematic_part = np.array([
            [0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0],
            [0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,1,0,1,0,0,1,1,0],
            [0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,1,0,1],
            [0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,1,0,1,0,0,1],
            [0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0,0,1,0],
            [1,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1,1,0,0,1,0,1,0,0,1,0],
            [1,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1]
        ], dtype=np.int8)
        self._h_matrix = self._p_systematic_part
    
        # Define GF(2)
        GF2 = galois.GF(2)
        H = GF2(self._h_matrix)

        # Compute the nullspace over GF(2)
        nullspace = H.null_space()
        if nullspace.shape[0] == 0:
            raise ValueError("Null space is empty. Cannot compute generator matrix.")

        # Assign the generator matrix
        self._g_matrix = np.array(nullspace, dtype=np.int8)

        # Verify that H * G^T = 0 over GF(2)
        G = GF2(self._g_matrix)
        result = H @ G.T
        if not np.all(result == 0):
            raise ValueError("H * G^T does not equal zero matrix over GF(2)")

        self._adj_syndromes = gf2.sum_adj_cols(self._h_matrix)
