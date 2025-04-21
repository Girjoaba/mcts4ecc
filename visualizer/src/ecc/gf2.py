"""The Galois Field 2 (GF2) or the Binary Field has the {0, 1} commutative group under
mod-2 addition and mod-2 multiplication operations.

In the following library we define the basic operations for the GF2 field and any helpful
functions that can be used to easier convert between decimal numbers and elements in the
field.
"""

from numpy.typing import NDArray
import numpy as np


# -----------------------------------------------------------------------------#
# ---------------------------- Operations in GF2 ------------------------------#
# -----------------------------------------------------------------------------#

def product_mod2(arr1: NDArray[np.int8], arr2: NDArray[np.int8]) -> NDArray[np.int8]:
    """
    The mod-2 product is equivalent to performing a dot product on elements in the field.

    In edac is equivalent to performing a bitwise AND operation on the two arrays.
    """
    return np.dot(arr1, arr2) % 2


def sum_mod2(arr1: NDArray[np.int8], arr2: NDArray[np.int8]) -> NDArray[np.int8]:
    """
    The mod-2 sum is equivalent to performing vector sum on the elements in the field and
    takin modulo 2.

    In edac is equivalent to performing a bitwise XOR operation on the two arrays.
    """
    return np.mod(arr1 + arr2, 2)


def sum_adj_cols(m: np.ndarray) -> np.ndarray:
    """
    Sum all adjacent columns in the matrix s.t. C_i + C_{i+1} for all i in columns of m.
    """
    if m.ndim != 2:
        raise ValueError('Providing array is not a matrix')

    m_col: np.ndarray = m.T

    return (np.array([sum_mod2(m_col[i], m_col[i + 1]) for i in range(len(m_col) - 1)], dtype=np.int8)).T


# -----------------------------------------------------------------------------#
# ---------------------------- Generate Spaces --------------------------------#
# -----------------------------------------------------------------------------#

def generate_vector_space(k: int) -> [np.ndarray]:  # Number of combinations
    """
    Generate the vector space containing k bits.

    To achieve this we perform bitwise operations to generate all 2^k combinations.
    """
    space_size = 2 ** k

    # Hack: bitwise operators in order to gain efficiency
    binary_combinations = (np.arange(space_size)[:, None] & (1 << np.arange(k)[::-1])) > 0
    binary_combinations = binary_combinations.astype(np.int8)
    vector_space = np.stack(binary_combinations)

    return vector_space


def generate_weight_cols(w: int, n: int) -> {np.ndarray}:
    """
    Generate all columns of length n which have weight w.
    :param w: the weight of the columns.
    :param n: the length/dimension of the vector.
    :return: a set containing all the vectors of weight w.
    """
    if w > n:
        raise ValueError('The weight of the column cannot be greater than the dimension of the space')

    vector_space: np.ndarray = generate_vector_space(n)
    return [vector for vector in vector_space if np.sum(vector) == w]


def rref(M):
    """Compute the Reduced Row-Echelon Form of a matrix over GF(2)."""
    M = M.copy() % 2
    rows, cols = M.shape
    pivot_row = 0
    for pivot_col in range(cols):
        if pivot_row >= rows:
            break
        # Find pivot
        max_row = None
        for r in range(pivot_row, rows):
            if M[r, pivot_col]:
                max_row = r
                break
        if max_row is None:
            continue  # No pivot in this column
        # Swap rows
        if max_row != pivot_row:
            M[[pivot_row, max_row]] = M[[max_row, pivot_row]]
        # Eliminate below
        for r in range(pivot_row + 1, rows):
            if M[r, pivot_col]:
                M[r] = (M[r] + M[pivot_row]) % 2
        pivot_row += 1
    # Backward phase to eliminate above pivots
    for pivot_col in reversed(range(cols)):
        # Find pivot row
        pivot_rows = [r for r in range(rows) if M[r, pivot_col]]
        if len(pivot_rows) != 1:
            continue
        pivot_row = pivot_rows[0]
        # Eliminate above
        for r in range(pivot_row):
            if M[r, pivot_col]:
                M[r] = (M[r] + M[pivot_row]) % 2
    return M

def nullspace(M):
    """Compute the nullspace of a matrix over GF(2)."""
    M_rref = rref(M)
    rows, cols = M_rref.shape
    pivot_cols = []
    free_cols = []
    pivot_rows = {}

    # Identify pivot columns and rows
    for r in range(rows):
        for c in range(cols):
            if M_rref[r, c]:
                pivot_cols.append(c)
                pivot_rows[c] = r
                break

    pivot_cols_set = set(pivot_cols)
    free_cols = [c for c in range(cols) if c not in pivot_cols_set]

    # Number of free variables
    num_free_vars = len(free_cols)
    nullspace_basis = []

    # For each free variable, construct a nullspace vector
    for free_var_index, free_col in enumerate(free_cols):
        vec = np.zeros(cols, dtype=np.int8)
        vec[free_col] = 1
        for pivot_col in reversed(pivot_cols):
            r = pivot_rows[pivot_col]
            s = M_rref[r, free_cols].dot(vec[free_cols]) % 2
            vec[pivot_col] = s
        nullspace_basis.append(vec % 2)
    return np.array(nullspace_basis, dtype=np.int8)