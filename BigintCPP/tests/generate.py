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

# same size
for i in range(10):
    a = random.randint(2, 2**200)
    b = random.randint(2, 2**200)

    As.append(BigInt.from_value(a))
    Bs.append(BigInt.from_value(b))


# different size
diff_sizes = [32, 64, 128, 200, 300]
for i in range(10):
    a_size = random.choice(diff_sizes)
    b_size = random.choice(diff_sizes)

    a = random.randint(2, 2**a_size)
    b = random.randint(2, 2**b_size)

    As.append(BigInt.from_value(a))
    Bs.append(BigInt.from_value(b))

# corner case
As.append(BigInt.from_value(2**128-1))
Bs.append(BigInt.from_value(2**128-1))

As.append(BigInt.from_value(2**256-1))
Bs.append(BigInt.from_value(2**128-2))

As.append(BigInt.from_value(0))
Bs.append(BigInt.from_value(2**130))

As.append(BigInt.from_value(1))
Bs.append(BigInt.from_value(2**1000))


# construct tests
Results_Add = []
Results_Sub = []
Results_Mul = []
for i in range(len(As)):
    a = As[i]
    b = Bs[i]
    Results_Add.append(BigInt.from_value(a.value + b.value))
    Results_Sub.append(BigInt.from_value(a.value - b.value))
    # mul_sign = -1 ** ((a.sign ^ b.sign) & 1)
    Results_Mul.append(BigInt.from_value(a.value * b.value))


