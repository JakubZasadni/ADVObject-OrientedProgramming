# <Jakub Zasadni>, <415741>
from typing import List, Tuple

def quicksort(I: List[int], start: int, stop: int) -> None:
    B = I
    i = start
    j = stop

    pivot = B[(start + stop) // 2]

    while i < j:
        while B[i] < pivot:
            i += 1
        while B[j] > pivot:
            j -= 1
        if i <= j:
            B[i], B[j] = B[j], B[i]
            i += 1
            j -= 1

    if start < j:
        quicksort(B, start, j)
    if i < stop:
        quicksort(B, i, stop)
    return B

def bubblesort(I: List[int]) -> Tuple[List,int]:
    C = I[:]
    n = len(C)
    d = 0

    while n > 1:
        new_n = 0
        for i in range(1, n):
            d = d + 1
            if C[i - 1] > C[i]:
                C[i - 1], C[i] = C[i], C[i - 1]
                new_n = i
        n = new_n

    return C, d

