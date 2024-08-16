import random
from timeit import timeit
import sort


sorted_list = list(range(1, 1001))
reverse_sorted_list = list(range(1000, 0, -1))
equal_values_list = [4] * 1000
random_list = random.sample(range(1, 1001), 1000)


t1_sorted = timeit("sort.bubblesort(sorted_list)", number=1000, globals=globals())
t2_reverse_sorted = timeit("sort.bubblesort(reverse_sorted_list)", number=1000, globals=globals())
t3_equal_values = timeit("sort.bubblesort(equal_values_list)", number=1000, globals=globals())
t4_random = timeit("sort.bubblesort(random_list)", number=1000, globals=globals())

t1_quicksorted = timeit("sort.quicksort(sorted_list,0,len(sorted_list)-1)", number=1000, globals=globals())
t2_reverse_quicksorted = timeit("sort.quicksort(reverse_sorted_list,0,len(reverse_sorted_list)-1)", number=1000, globals=globals())
t3_equal_quickvalues = timeit("sort.quicksort(equal_values_list,0,len(equal_values_list)-1)", number=1000, globals=globals())
t4_quickrandom = timeit("sort.quicksort(random_list,0,len(random_list)-1)", number=1000, globals=globals())

# Wyświetlenie wyników
print("Czas sortowania bubblesort posortowanej listy:", t1_sorted / 1000, "sekundy")
print("Czas sortowania bubblesort listy posortowanej odwrotnie:", t2_reverse_sorted / 1000, "sekundy")
print("Czas sortowania bubblesort listy z równymi wartościami:", t3_equal_values / 1000, "sekundy")
print("Czas sortowania bubblesort listy z losowymi wartościami:", t4_random / 1000, "sekundy")


print("Czas sortowania quicksort posortowanej listy:", t1_quicksorted / 1000, "sekundy")
print("Czas sortowania quicksort listy posortowanej odwrotnie:", t2_reverse_quicksorted / 1000, "sekundy")
print("Czas sortowania quicksort listy z równymi wartościami:", t3_equal_quickvalues / 1000, "sekundy")
print("Czas sortowania quicksort listy z losowymi wartościami:", t4_quickrandom / 1000, "sekundy")
