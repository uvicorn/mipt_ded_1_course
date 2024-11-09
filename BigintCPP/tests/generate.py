import random
from dataclasses import dataclass
from enum import Enum

CODE_GEN_DIR = 'codegen_output/'


Add_tests = ''
BLOCK_SIZE = 64

class SIGN(Enum):
    PLUS = 0b11
    MINUS = 0b10

@dataclass
class BigInt:
    blocks: list[int]
    blocks_count: int
    sign: SIGN
    value: int

    @staticmethod
    def from_value(x):
        value = x
        if x >= 0:
            sign = SIGN.PLUS
        else:
            sign = SIGN.MINUS
            x = -x
        blocks = []
        blocks_count = 0
        while x:
            blocks.append(x % (2**BLOCK_SIZE))
            blocks_count += 1
            x >>= BLOCK_SIZE
        return BigInt(blocks=blocks, blocks_count=blocks_count, sign=sign, value=value)

As = []
Bs = []
Results = []

# same size
for i in range(5):
    a = random.randint(2, 2**200)
    b = random.randint(2, 2**200)
    c = a + b
    As.append(BigInt.from_value(a))
    Bs.append(BigInt.from_value(b))
    Results.append(BigInt.from_value(c))

# different size
for i in range(5):
    a = random.randint(2, 2**100)
    b = random.randint(2, 2**300)
    c = a + b
    As.append(BigInt.from_value(a))
    Bs.append(BigInt.from_value(b))
    Results.append(BigInt.from_value(c))


