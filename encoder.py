    import math
import numpy as np

def get_dct_matrix():
    """Precomputes the 8x8 DCT transform matrix."""
    dct_m = np.zeros((8, 8))
    for i in range(8):
        c = math.sqrt(1/8) if i == 0 else math.sqrt(2/8)
        for j in range(8):
            dct_m[i, j] = c * math.cos((2 * j + 1) * i * math.pi / 16)
    return dct_m

DCT_MATRIX = get_dct_matrix()

def apply_dct(block):
    # DCT 2D = Matrix * Block * Matrix_T
    return np.dot(np.dot(DCT_MATRIX, block), DCT_MATRIX.T)

def zig_zag_scan(block):
    """Converts 8x8 matrix to 1D array in zig-zag order."""
    patterns = [
        (0,0), (0,1), (1,0), (2,0), (1,1), (0,2), (0,3), (1,2),
        (2,1), (3,0), (4,0), (3,1), (2,2), (1,3), (0,4), (0,5),
        # ... (full 64-point sequence omitted for brevity)
    ]
    # Standard zig-zag indices
    index_order = [
        0,  1,  5,  6, 14, 15, 27, 28,
        2,  4,  7, 13, 16, 26, 29, 42,
        3,  8, 12, 17, 25, 30, 41, 43,
        9, 11, 18, 24, 31, 40, 44, 53,
        10, 19, 23, 32, 39, 45, 52, 54,
        20, 22, 33, 38, 46, 51, 55, 60,
        21, 34, 37, 47, 50, 56, 59, 61,
        35, 36, 48, 49, 57, 58, 62, 63
    ]
    return block.flatten()[index_order]