import copy

class Cog:
    def __init__(self, start, end, level):
        self.start = start
        self.end = end
        self.level = level

    def length(self):
        return self.end - self.start

    def requires(self, b):
        return Cog(self.end - self.level, self.end + b - 1, self.level + 1)

    def canMove(self, ell):
        j = self.start + (self.level - 1)
        return Cog(j - ell, j, self.level - 1)

    def shift(self, b):
        self.start += b
        self.end += b

    def summands(self):
        if self.level == 0:
            return (self.length() + 2)/2
        elif self.level == 1:
            return self.length() + 1
        else:
            return (2 ** (self.level - 1)) * self.length()

    def numMoves(self, other):
        assert other.level == (self.level + 1), "level mismatch"
        assert other.start == (self.end - self.level), "mesh mismatch"
        b = other.end - self.end + 1
        factor = 2 ** (self.level - 1)
        return factor * b * (self.length() + 2 - self.level)

    def interact(self, other):
        assert other.level == (self.level + 1), "level mismatch"
        assert other.start == (self.end - self.level), "mesh mismatch"
        moves = self.numMoves(other)
        b = other.end - self.end + 1
        self.shift(b)
        other.shift(self.level - self.length() - 2)
        return moves

    def getTo(self, other):
        assert other.level == (self.level - 1), "level mismatch"
        return (other.end - other.level) - self.start

    def __str__(self):
        return "Cog({}, {}, {})".format(str(self.start), str(self.end), str(self.level))

    def decompose(self):
        assert self.level != 0, "level too low to decompose"
        left = Cog(self.start, self.end - 1, self.level - 1)
        right = Cog(self.start + 1, self.end, self.level - 1)
        return (left, right)

def makeTrain(k, i, j, b):
    if k == 0:
        return [Cog(i, j, 0)]
    if k == 1:
        cog0 = Cog(i, j, 0)
        cog1 = cog0.requires(b)
        return [cog0, cog1]
    oldTrain = makeTrain(k - 1, i, j, b)
    newTrain = copy.deepcopy(oldTrain)
    runTrain(oldTrain)
    b = oldTrain[-1].getTo(oldTrain[-2])
    newTrain.append(oldTrain[-1].requires(b))
    return newTrain

def runTrain(train):
    totalMoves = 0
    for i in range(0, len(train) -1):
        for j in range(0, i+1):
            totalMoves += train[i - j].interact(train[i - j + 1])
    return (train, totalMoves)

def summandsTrain(train):
    return sum(map(lambda x: x.summands(), train))

def strTr(train):
    return list(map(str, train))
