


def fibonaccigenerator(n):
# generate fibonacci numbers up to one bigger than n. This is NOT the nth fibonacci.
    F=[1,2]
    i=1
    x=2
    while x < n:
        x=x+F[i-1]
        F.append(x)
        i = i+1
    return F


def zechdecomp(z):
    F=fibonaccigenerator(z)
    n=len(F)
    ZD=[0]*(n)
    switch=True
    while z > 0:
        for i in range(0,n,1):
            if z == F[i]:
                ZD[i] = 1
                z = z - F[i]
                break
            elif z < F[i] and z > F[i-1]:
                ZD[i-1]=1
                z=z-F[i-1]
                break
    return ZD


def zerobuffer(L1,L2):
    n1=len(L1)
    n2=len(L2)
    if n1 > n2:
        ndif=n1-n2
        buffer= [0] * ndif
        L2=L2+buffer
    elif n1 < n2:
        ndif=n2-n1
        buffer= [0] * ndif
        L1=L1+buffer
    return(L1,L2)


def hammingdistance(L1,L2):
    Ls=zerobuffer(L1,L2)
    L1=Ls[0]
    L2=Ls[1]
    n=len(L1)
    ham=0
    for i in range(0,n):
        if L1[i] != L2[i]:
            ham=ham+(abs(L1[i]-L2[i]))
    return ham

def successivehammingdistance(n):
    L1=zechdecomp(n)
    L2=zechdecomp(n+1)
    ham=hammingdistance(L1, L2)
    return ham


def rangehamdistance(n):
    Hams=[]
    for i in range(0,n+1):
        h=successivehammingdistance(i)
        print(str(h)+" at n=" + str(i))
    return


n=59
output=rangehamdistance(n)

output=successivehammingdistance(n)
print(output)

output=zechdecomp(n)
print(output)
output=zechdecomp(n+1)
print(output)
