#pragma once
/*
template<unsigned index, unsigned start>
class ArmBlockList {
    static constexpr unsigned endIndex =
            getNthIndex<decltype(s), index>(s, start, len, ':');
    ArmBlockList<index + 1, endIndex> nextBlock;
    static constexpr unsigned blockLen = countInstancesFrom(s, start, (endIndex == -1) ? len : endIndex, ';');
    ArmBlock<blockLen> block;
public:
    constexpr ArmBlockList() : block(index), nextBlock() {}

    bool assemble(unsigned (&out)[insnCount]) {

    }
};


template<unsigned index>
class ArmBlockList<index, (unsigned int)-1> {
public:
    constexpr ArmBlockList() {}
};

*/