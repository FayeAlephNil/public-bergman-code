import numpy
from math import sqrt
from itertools import zip_longest

phi = (1 + sqrt(5))/2

def greedy_for_pos(x):
    result_pos = []
    if x < 1:
        assert False

    m = 0
    while (x > phi**(m + 1)):
        result_pos.append((m, False))
        m += 1
    result_pos.append((m, True))

    temp = x - phi**m
    while temp >= 1:
        n = 0
        while (temp > phi**(n + 1)):
            result_pos[n] = (n, False)
            n += 1
        result_pos[n] = (n, True)
        temp = temp - phi**n
    return (temp, result_pos)

def greedy_for_neg(x):
    result_neg = []
    if x >= 1:
        assert False
    temp = x

    m = -1
    while (temp > 1e-15):
        while (phi**m - temp > 1e-15):
            result_neg.append((m, False))
            m -= 1
        result_neg.append((m, True))
        temp = temp - phi ** m
    return result_neg

def greedy_raw(x):
    if x < 1:
        return(reverse(greedy_for_neg(x)), [(0, False)])
    else:
        (y, result_pos) = greedy_for_pos(x)
        result_neg = greedy_for_neg(y)
        return (result_neg, result_pos)

def greedy_append(x):
    (result_neg, result_pos) = greedy_raw(x)
    result_neg.reverse()
    return result_neg + result_pos

def greedy(x):
    (result_neg, result_pos) = greedy_raw(x)
    result_neg.reverse()
    result = []
    for (_, p) in result_neg:
        result.append(1 if p else 0)
    result.append(("start", 1 if result_pos[0] else 0))
    for (_, p) in result_pos[1:]:
        result.append(1 if p else 0)
    return result

def greedy_depth(x):
    (result_neg, result_pos) = greedy_raw(x)
    return (-len(result_neg), len(result_pos) - 1)



