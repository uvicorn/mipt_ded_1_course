import random
from dataclasses import dataclass
from enum import Enum

CODE_GEN_DIR = 'codegen_output/'

CODEGEN_WARNING = '''// CODE WAS GENERATED BY CHATGPT DONT TOUCH\n'''*10
bigint_classname = 'BigIntTester'

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

    def __str__(self):
        # BigInt a = BigInt({1ull,2,3}, PLUS);
        blocks = '{'+', '.join(str(i)+'ull' for i in self.blocks) + '}'
        return f"{bigint_classname}({blocks}, {self.sign.name})"
As = []
Bs = []

# same size
for i in range(10):
    a = random.randint(-(2**200), 2**200)
    b = random.randint(-(2**200), 2**200)

    As.append(BigInt.from_value(a))
    Bs.append(BigInt.from_value(b))


# different size
diff_sizes = [32, 64, 128, 200, 300]
for i in range(10):
    a_size = random.choice(diff_sizes)
    b_size = random.choice(diff_sizes)

    a = random.randint(-(2**a_size), 2**a_size)
    b = random.randint(-(2**b_size), 2**b_size)

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

As.append(BigInt.from_value(123231231))
Bs.append(BigInt.from_value(123231231+1))

As.append(BigInt.from_value(123231231+2))
Bs.append(BigInt.from_value(123231231))
##

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

for i in range(len(Results_Sub)):
    print(f"Eblo {i}:")
    print(str(As[i]))
    print(str(Bs[i]))
    print(str(Results_Sub[i])+'\n')

def format_array(name, size, content):
    str_content = '{\n'+',\n'.join(str(i) for i in content) + '\n}'
    return f"{bigint_classname} {name}[{size}] = {str_content};\n"

arr_size = len(As)
assert len(As) == len(Bs) == len(Results_Add) == len(Results_Mul) == len(Results_Sub)

fixture_members = [
    ("First_args", arr_size, As),
    ("Second_args", arr_size, Bs),
    ("Mul_res", arr_size, Results_Mul),
    ("Add_res", arr_size, Results_Add),
    ("Sub_res", arr_size, Results_Sub),
]
code_gen_output = '\n'.join(format_array(*i) for i in fixture_members)+f'\n\nsize_t tests_size = {arr_size};\n'

open(CODE_GEN_DIR+'basic_fixture.cpp', 'w+').write(CODEGEN_WARNING+code_gen_output+CODEGEN_WARNING)