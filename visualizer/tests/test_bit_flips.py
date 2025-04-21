import unittest
import numpy as np
from tqdm import tqdm
import sys, os

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from src.ecc.cyclic_avoidance_codes import NewLiN39K32
import src.ecc.flags as flg

class TestAdjacentDoubleErrorCorrection(unittest.TestCase):
    def setUp(self):
        # Initialize your code object
        self.code = NewLiN39K32()
        # Uncorrectable Error Flag used by your decode method
        self.UE_FLAG = flg.UE_FLAG 
        self.k = 32  # Data word length (adjust if different)
        self.n = 39  # Codeword length (adjust if different)

        # Use a fixed data word for testing (all zeros)
        self.data_word = np.zeros(self.k, dtype=int)
        self.codeword = self.code.encode(self.data_word)

    def test_single_bit_errors(self):
        total_errors = self.n
        correct_corrections = 0
        detected_errors = 0
        incorrect_corrections = 0

        pbar = tqdm(total=total_errors, desc="Testing Single-Bit Errors")

        for i in range(self.n):
            with self.subTest(i=i):
                error_pattern = np.zeros(self.n, dtype=int)
                error_pattern[i] = 1  # Flip bit i
                corrupted_codeword = (self.codeword + error_pattern) % 2

                corrected_word = self.code.decode(corrupted_codeword)
                if isinstance(corrected_word, int) and corrected_word == self.UE_FLAG:
                    detected_errors += 1
                    # Output error details
                    print(f"\nUncorrectable error detected for single-bit error at position {i}")
                    print(f"Error pattern: {error_pattern}")
                    print(f"Data word: {self.data_word}")
                    print(f"Corrupted codeword: {corrupted_codeword}")
                elif np.array_equal(corrected_word, self.codeword):
                    correct_corrections += 1
                else:
                    incorrect_corrections += 1
                    # Output error details
                    print(f"\nIncorrect correction for single-bit error at position {i}")
                    print(f"Error pattern: {error_pattern}")
                    print(f"Data word: {self.data_word}")
                    print(f"Corrupted codeword: {corrupted_codeword}")
                    print(f"Corrected word: {corrected_word}")
                    print(f"Original codeword: {self.codeword}")
                pbar.update(1)

        pbar.close()

        # Assertions
        error_messages = []
        if incorrect_corrections > 0:
            error_messages.append(f"Incorrect corrections occurred in single-bit error correction: {incorrect_corrections}")
        if detected_errors > 0:
            error_messages.append(f"Uncorrectable errors detected in single-bit error correction: {detected_errors}")
        if correct_corrections < total_errors:
            error_messages.append(f"Not all single-bit errors were corrected. Corrected: {correct_corrections}/{total_errors}")

        if error_messages:
            self.fail("\n".join(error_messages))

        print(f"\nSingle-Bit Error Correction:")
        print(f"Correct Corrections: {correct_corrections}")
        print(f"Detected Uncorrectable Errors: {detected_errors}")
        print(f"Incorrect Corrections: {incorrect_corrections}")

    def test_adjacent_double_bit_errors(self):
        total_errors = self.n - 1  # Since wrap-around is excluded
        correct_corrections = 0
        detected_errors = 0
        incorrect_corrections = 0
        error_details = []  # List to collect error details

        pbar = tqdm(total=total_errors, desc="Testing Adjacent Double-Bit Errors")

        for i in range(self.n - 1):  # Loop from 0 to n - 2
            with self.subTest(i=i):
                error_pattern = np.zeros(self.n, dtype=int)
                error_pattern[i] = 1
                error_pattern[i + 1] = 1  # Flip adjacent bit without wrap-around
                corrupted_codeword = (self.codeword + error_pattern) % 2

                corrected_word = self.code.decode(corrupted_codeword)
                if isinstance(corrected_word, int) and corrected_word == self.UE_FLAG:
                    detected_errors += 1
                    # Collect error details
                    error_details.append(
                        f"Uncorrectable error detected for error pattern at positions {i} and {i + 1}"
                    )
                    error_details.append(f"Error pattern: {error_pattern.tolist()}")
                    error_details.append(f"Data word: {self.data_word.tolist()}")
                    error_details.append(f"Corrupted codeword: {corrupted_codeword.tolist()}")
                elif np.array_equal(corrected_word, self.codeword):
                    correct_corrections += 1
                else:
                    incorrect_corrections += 1
                    # Collect error details
                    error_details.append(
                        f"Incorrect correction for error pattern at positions {i} and {i + 1}"
                    )
                    error_details.append(f"Error pattern: {error_pattern.tolist()}")
                    error_details.append(f"Data word: {self.data_word.tolist()}")
                    error_details.append(f"Corrupted codeword: {corrupted_codeword.tolist()}")
                    error_details.append(f"Corrected word: {corrected_word.tolist()}")
                    error_details.append(f"Original codeword: {self.codeword.tolist()}")
                pbar.update(1)

        pbar.close()

        # Assertions
        error_messages = []
        if incorrect_corrections > 0:
            error_messages.append(f"Incorrect corrections occurred in adjacent double-bit error correction: {incorrect_corrections}")
        if detected_errors > 0:
            error_messages.append(f"Uncorrectable errors detected in adjacent double-bit error correction: {detected_errors}")
        if correct_corrections < total_errors:
            error_messages.append(f"Not all adjacent double-bit errors were corrected. Corrected: {correct_corrections}/{total_errors}")

        if error_messages:
            # Include error details in the assertion message
            error_message = "\n".join(error_messages + ["\nError details:"] + error_details)
            self.fail(error_message)

        print(f"\nAdjacent Double-Bit Error Correction:")
        print(f"Correct Corrections: {correct_corrections}")
        print(f"Detected Uncorrectable Errors: {detected_errors}")
        print(f"Incorrect Corrections: {incorrect_corrections}")

if __name__ == '__main__':
    unittest.main()