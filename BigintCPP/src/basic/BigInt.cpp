#include "BigInt.hpp"


BigInt::BigInt(Blocks blocks, size_t blocks_count, SIGN sign):
    blocks(std::move(blocks)),
    blocks_count(blocks_count),
    sign(sign)
{
    this->Normalize();
}

void BigInt::Normalize(){
    // IDEA: можно сделать оптимизацию, вставив в конец 0 или в начало 0
    while (blocks_count > 0 && !blocks[blocks_count-1]){
        this->blocks_count--;
    }
}


