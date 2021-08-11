import math

LambdaM = {0: None}
L = [2, 1]
Ll = 2


def compute_Lucas(n):
    global L
    global Ll
    while Ll <= n:
        L.append(L[-1] + L[-2])
        Ll += 1
    return L[n]


def struct_thm(n, i=0):
    # TODO: make this loop more efficient
    # it loops up to log n ^2 times
    # get it down to log n by storing in between attempts
    if n <= 2:
        return n
    if i:
        assert L[i] >= n
        while L[i] > n:
            i -= 1
    else:
        while L[i + 1] <= n:
            i += 1
    if i % 2 == 0:
        return 2 + struct_thm(n - L[i], i)
    elif n == L[i]:
        return i
    elif n < L[i] + L[i - 3]:  #
        return 1 + struct_thm(n - L[i - 1], i)
    elif n <= L[i] + L[i - 2]:
        return 2 + struct_thm(n - L[i], i)
    else:
        return 1 + struct_thm(n - L[i], i)


# Interval [L2k, L2k + 1] if even
# Or interval [L2k+1 + 1, L2k+2 - 1] if odd
# Ensure M >= 2n for the encoding
def Lambda(n, M):
    v = vector([0] * M)
    for i in range(L[n] + (n % 2), L[n + 1] + ((n + 1) % 2)):
        v[struct_thm(i)] += 1
    return v


def shift(v, k):
    n = len(v)
    return vector([v[(i - k) % n] for i in range(n)])


# Ensure M >= 2n for the encoding
def STHM_Lambda(n, M):
    global LambdaM
    if not LambdaM.get(M, None):
        LambdaM[M] = {i: Lambda(i, M) for i in range(1, 5)}
    if LambdaM[M].get(n, None):
        return LambdaM[M][n]
    A = STHM_Lambda(n - 2, M)
    B = STHM_Lambda(n - 3, M)
    if n % 2 == 0:
        K = shift(shift(A, -2) + A + B, 2)
    else:
        K = 2 * shift(A, 1) + shift(B, 2)
    LambdaM[M][n] = K
    return K

def average(dic):
    tmp = 0
    leng = 0
    for key, val in dic.items():
        if val != None:
            tmp += key * val
            leng += val
    return (tmp/leng)

def variance(dic, avg):
    tmp = 0
    leng = 0
    for key, val in dic.items():
        if val != None:
            tmp += ((key - avg) ** 2) * val
            leng += val
    return (tmp/leng)

def normalize(dic):
    avg = average(dic)
    std = math.sqrt(variance(dic), avg)
    return {((key - avg)/std):val for (key, val) in dic}

def moments(dic, moms=10):
    leng = 0
    new_dic = normalize(dic)
    tmp_pows = {}
    tmp = []

    for i in range(0, moments + 1):
        for (key, val) in new_dic.items():
            if (i != 0):
                tmp_pows[key] *= key
            tmp[i] += tmp_pows[key] * val
            leng += val
    return [x/leng for x in tmp]

def STHM_moments(n, moments=10):
    M = 2 * n + 2
    dic = STHM_Lambda(n, M)
    return moments(dic)

