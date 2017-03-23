#pragma once

#include "ArmAsmUtils.h"

#define NOASSEMBLE  0
#define NOREVERSE   1
constexpr void constexprError(char* s) {
    *s = 0;
}


template<typename T>
static constexpr unsigned countInstances(T s, unsigned length, char c) {
    unsigned result = 0;
    for(unsigned i = 0; i < length; ++i){
        if(s[i] == c)
            ++result;
    }
    return result;
}
template<typename T>
static constexpr unsigned countInstancesFrom(T s, unsigned start, unsigned end, char c) {
    unsigned result = 0;
    for(unsigned i = start; i < end; ++i){
        if(s[i] == c)
            ++result;
    }
    return result;
}

static_assert(countInstances("mov r0, #0; bx lr;", sizeof("mov r0, #0; bx lr;"), ';') == 2);
template<typename T>
static constexpr unsigned getIndexOfNext(T s, unsigned start, unsigned length, char c) {
    for(unsigned i = start + 1; i < length; ++i){
        if(s[i] == c)
            return i;
    }
    //throw std::invalid_argument("Bad!");
    return -1;
}
template<typename T>
static constexpr unsigned getIndexOfReverse(T s, unsigned end, unsigned start, char c) {
    for(unsigned i = end - 1; i >= start; --i){
        if(s[i] == c)
            return i;
    }
    return -1;
}

template<typename T, unsigned ix>
static constexpr unsigned getNthIndex(T s, unsigned start, unsigned length, char c) {
    unsigned current = start;
    for(unsigned i = 0; i < ix; ++i) {
        current = getIndexOfNext(s, current, length, c);
        if(current == -1)
            break;
    }
    return current;
};

template<typename T>
static constexpr bool compareStrs(T s, const char* s2) {
    for(unsigned i = 0; s[i] == s2[i]; ++i) {
        if(!s[i] && !s2[i])
            return true;
    }
    return false;
}
template<typename T>
static constexpr unsigned lengthOfString(T s) {
    unsigned i = 0;
    for (; s[i]; ++i);
    return i;
}
template<typename masterTemplate>
class ArmOperand {
    using barrelType = ArmAsmUtils::barrelType;
    using uint8 =  cloture::util::common::uint8;
    char op[128];
    unsigned length;
    barrelType m_btype = barrelType::none;
    bool m_bval_is_register = false;
    uint8 m_bval = 0xFF;
public:
    template<typename T>
    constexpr ArmOperand(masterTemplate& asmState, T s, unsigned start, unsigned end) : op(), length(start - end) {
        unsigned j = 0;
        bool possibleShiftOrRotate = false;
        while(s[start] == ' ' || s[start] == ',')
            start++;
        if(start >= end)
            constexprError("skipping commas and spaces while parsing operand brought us to the end of the operand!");
        unsigned i = start;
        for(; i < end; ++i, ++j) {
            if(s[i] == ' ') {
                possibleShiftOrRotate = true;
                break;
            }
            op[j] = s[i];
        }

        if(possibleShiftOrRotate) {
            using namespace cloture::util::ctfe;
            length = cstrlen(&op[0]);

            int index = 0;
            #define iOfNext(s_) ((index = indexOfBounded(s, start, end, s_)) != -1)

            bool foundBarrel = true;
            if(iOfNext("asr")) {
                m_btype = barrelType ::asr;
            } else if(iOfNext("lsl")) {
                m_btype = barrelType ::lsl;
            } else if(iOfNext("ror")) {
                m_btype = barrelType ::ror;
            } else if(iOfNext("lsr")) {
                m_btype = barrelType ::lsr;
            } else if(iOfNext("rrx")) {
                m_btype = barrelType ::rrx;
            } else {
                foundBarrel = false;
            }
            if(foundBarrel) {
                unsigned constIndex = getIndexOfReverse(s, end, i, '#');
                if(constIndex == -1) {
                    m_bval_is_register = true;
                    unsigned tempstart = start;
                    start = i;
                    for(const auto currentRegister : ArmAsmUtils::regNames) {

                        if(iOfNext(currentRegister)) {
                            char regname[32] = {};
                            for(unsigned k = index, l = 0; s[k] != ' ' && s[k] != ';' && k < end; ++k, ++l) {
                                regname[l] = s[k];

                            }
                            //check if it was just a partial match (r1 matching to r10)
                            if(cstrlen(&regname[0]) != cstrlen(currentRegister))
                                continue;
                            //otherwise set m_bval to the gp reg index after resolving any possible aliases
                            auto aliasResolved = ArmAsmUtils::resolveRegisterAlias(&regname[0]);
                            if(aliasResolved[0] != 'r')
                                constexprError("resolveRegisterAlias is supposed to return a gp reg starting with the letter 'r'");
                            m_bval = cloture::util::ctfe::parser::parse<int>(&aliasResolved[1]);
                            if(m_bval == 0xFF)
                                constexprError("wattt??");

                        }
                    }
                    start = tempstart;
                }
                else { //constIndex != -1, so find and parse the constant
                    char const_txt[256] = {};

                    for(unsigned k = constIndex + 1, l = 0; k < end && isNumericChar(s[k]); ++k, ++l)
                        const_txt[l] = s[k];
                    m_bval = cloture::util::ctfe::parser::parse<int>(&const_txt[0]);
                }

                if(m_bval > 31)
                    constexprError("Parsing barrel shifter value gave a bad result!");
            }

        }
    }
    constexpr ArmOperand() : op(), length() {}

    constexpr bool isRegister() const {
        return op[0] == 'r' || compareStrs(op, "lr") || compareStrs(op, "sp") || compareStrs(op, "ip");
    }
    constexpr bool isConstant() const {
        return op[0] == '#';
    }
    constexpr bool isLabel() const {
        if(op[length]) {
            return op[length] == ':';
        } else {
            return op[length -1] == ':';
        }
    }
    constexpr int getConstant() const {
        return cloture::util::ctfe::parser::parse<int>(&op[1]);
    }
    constexpr bool hasBShift() const {
        return m_btype != barrelType::none;
    }
    constexpr bool shiftsUsingReg() const {
        return m_bval_is_register;
    }
    constexpr auto getBShift() const {
        return m_bval;
    }
    constexpr auto getBType() const {
        return m_btype;
    }
    constexpr unsigned getConstantEncoded() const {
        int constant = getConstant();
        unsigned result = ArmAsmUtils::encodeImm12(constant);
        if(result == -1)
            constexprError("Failed to encode constant!");
        return result;
    }
    constexpr int getRegister() const {
        auto resolved = ArmAsmUtils::resolveRegisterAlias(&op[0]);
        return cloture::util::ctfe::parser::parse<int>(&resolved[1]);
    }
    constexpr const char* getLabelName() const {
        return &op[0];
    }

    constexpr unsigned encodeAsDestination() const {
        if(isRegister()) {
            return getRegister() << 12;
        }
        else {
            constexprError("Destination must be a register!");
        }
        return -1;
    }
    constexpr unsigned encodeAsOperand(unsigned int operandIndex) const {
        if(operandIndex == 0) {
            if(isRegister()) {
                return getRegister() << 16;
            } else {
                constexprError("Operand at index 0 must be a register.");
            }
        } else if(operandIndex == 1) {
            if(isRegister()) {
                auto regVal = getRegister();
                if(!hasBShift())
                    return regVal;
                if(shiftsUsingReg()) {
                    auto shiftReg = getBShift();
                    if(!ArmAsmUtils::isValidGP(shiftReg))
                        constexprError("Invalid GP reg in encodeAsOperand!");
                    if(getBType() == barrelType::rrx)
                        constexprError("RRX is not a valid shift type for register operands!");
                    return shiftReg << 8 |1 << 4 | static_cast<unsigned>(getBType()) << 5 | regVal;
                } else {
                    auto shiftVal = getBShift();
                    return shiftVal << 7 | static_cast<unsigned>(getBType()) << 5 | regVal;
                }
            }
            else if(isConstant()) {
                return getConstantEncoded();
            }
            else {
                constexprError("Operand at index 1 must be a register or constant!");
            }
        } else {
            constexprError("Invalid operand index!");
        }
        return -1;
    }

};
template<typename masterTemplate>
class ArmInsn {
    char mnem[128];
    unsigned operandCount;
    unsigned m_cc = -1;
    cloture::util::ctfe::strhash_t m_mnem_hash = 0;
    using ArmOperand = ArmOperand<masterTemplate>;
    ArmOperand ops[3];
    template<typename T>
    static constexpr ArmOperand makeOperand(masterTemplate& asmState, T s, unsigned start, unsigned length) {
        if(start == -1 || start >= length)
            return ArmOperand();
        unsigned insnEnd = getIndexOfNext(s, start, length, ';');
        unsigned endOfOperand = getIndexOfNext(s, start, insnEnd, ',');
        if(endOfOperand == start +1) {
            start = start+1;
            endOfOperand = getIndexOfNext(s, start, insnEnd, ',');
        }
        int i = 0;
        for(; i < insnEnd && s[i+start] == ' ' || s[i+start] == ','; ++i)
            ;
        if(endOfOperand == -1) {
            return ArmOperand(asmState, s, start+i, insnEnd);
        };
        if(endOfOperand == -1 || (endOfOperand > insnEnd && insnEnd != -1)) {
            return ArmOperand(asmState, s, start, insnEnd);
        }
        return ArmOperand(asmState, s, start, endOfOperand);
    }
public:
#define GETNEXTOPER(index)      makeOperand(asmState, s, getNthIndex<T, index>(s, mnemEnd + 1, length, ','), length)
    template<typename T>
    constexpr ArmInsn(masterTemplate& asmState, T s, unsigned start, unsigned length)
            : ops(), mnem(), operandCount(0)
    {
        unsigned mnemEnd = getIndexOfNext(s, start, length, ' ');
        for(; start < mnemEnd && s[start] == ' '; ++start)
            ;
        unsigned end = getIndexOfNext(s, start, length, ';');
        operandCount = countInstancesFrom(s, start, end, ',') + 1;
        if(operandCount >= 1)
            ops[0] = GETNEXTOPER(0);
        if(operandCount >= 2)
            ops[1] = GETNEXTOPER(1);
        if(operandCount >= 3)
            ops[2] = GETNEXTOPER(2);
        unsigned j = 0;
        for(unsigned i = start; i < mnemEnd; ++i, j++)
            mnem[j] = s[i];
        using namespace ArmAsmUtils;
        const armCC_t& cc = mapcc(&mnem[0]);
        if(!cc.isNullCC()) {
            m_cc = indexOfCC(cc);
            mnem[j - 1] = 0;
            mnem[j - 2] = 0;
        }
        m_mnem_hash = cstrhash(&mnem[0]);
    }

#undef GETNEXTOPER
    constexpr ArmInsn() : ops(), mnem(), operandCount(0) {}

    constexpr bool hasCC() const {
        return m_cc != -1;
    }
    constexpr const ArmOperand& operator [](unsigned index) const {
        return ops[index];
    }
    constexpr unsigned assemble() const {
        /*
        if(compareStrs(mnem, "mov")) {
            return 0x0000A0E3;
        }
        else {
            return 0x1EFF2FE1;
        }*/
        bool didStuffConst = false;
        using namespace ArmAsmUtils;
        constexpr strhash_t bxHash = cstrhash("bx");
        const armOpcode_t& op = getIntegerOp(&mnem[0]);
        unsigned opcode_ = op.encode();
        bool isBranch = false;
        unsigned result = 0;//op.encode();
        if( hasCC() ) {
            const armCC_t& cc = getArmCC(m_cc);
            if(cc.isNullCC())
                constexprError("ArmInsn::hasCC returned true, but getArmCC returned a null cc!");
            result |= cc.value();
        }
        else {
            result |= 0b1110 << 28;
        }
        if(operandCount >= 1) {
            if(op == bxHash) {
                result |= ops[0].encodeAsOperand(1);
                isBranch = true;
            }
            else
                result |= ops[0].encodeAsDestination();
        }
        if(!isBranch) {
            if (op1BelongsInRN(m_mnem_hash)) {
                if (ops[1].isConstant())
                    result |= 0x2000000;
                result |= ops[1].encodeAsOperand(1);
            }
            else {
                if (operandCount < 3) {
                    /*
                     * using add r0, #0 as opposed to add r0, r0, #0
                     */
                    result |= ops[0].encodeAsOperand(0);
                    if (ops[1].isConstant()) {

                        //unsigned stuffed = stuff_const(opcode_, ops[1].getConstant());
                        //if(stuffed == 0)
                        //     constexprError("bah!");
                        result |= ops[1].encodeAsOperand(1);
                        result |= 0x2000000;
                        //didStuffConst = true;
                    }
                    else
                        result |= ops[1].encodeAsOperand(1);
                }
                else {
                    if (operandCount >= 2) {
                        result |= ops[1].encodeAsOperand(0);
                    }
                    if (operandCount >= 3) {
                        if (ops[2].isConstant())
                            result |= 0x2000000;
                        result |= ops[2].encodeAsOperand(1);
                    }
                }
            }
        }
        //if(!didStuffConst)
        result |= (isBranch ? op.opcode() : opcode_) ;
        return result;
    }
};
/*
template<unsigned block_length>
class ArmBlock {
    ArmInsn insns[(block_length == 0) ? 1 : block_length];
public:
    //template<typename T>
    constexpr ArmBlock(unsigned index) : insns() {}
};*/



static constexpr unsigned reverseByteOrderForArm(unsigned val) {
    using cloture::util::common::uint8;
#if NOREVERSE
    return val;
#else
    unsigned value = 0;
    uint8 lolo = (val >> 0) & 0xFF;
    uint8 lohi = (val >> 8) & 0xFF;
    uint8 hilo = (val >> 16) & 0xFF;
    uint8 hihi = (val >> 24) & 0xFF;
    value |= lolo << 24;
    value |= lohi << 16;
    value |= hilo << 8;
    value |= hihi;
    return value;
#endif
}

template<typename masterTemplate>
class AsmLabel {
    unsigned m_instructionIndex = -1;
};

template<typename masterTemplate>
class AsmMarker {
    unsigned m_markerIndex = -1;
    unsigned m_markedInstructionIndex = -1;
    unsigned m_markedOperand = -1;
    cloture::util::ctfe::strhash_t m_namehash = 0;
    using ArmOperand = ArmOperand<masterTemplate>;
    ArmOperand oper;
    bool isSet = false;
public:
    constexpr AsmMarker(){}
    constexpr void set(unsigned index, unsigned insnIndex, unsigned operand, typeof(m_namehash) namehash) {
        if(isSet)
            constexprError("Attempted to set already initialized AsmMarker!");
        m_markerIndex = index, m_markedInstructionIndex = insnIndex, m_markedOperand = operand, m_namehash = namehash;
        isSet = true;
    }

    constexpr auto getIndex() const {
        if(!isSet)
            constexprError("Attempted to get index of uninitialized AsmMarker!");
        return m_markerIndex;
    }
    constexpr auto getInsnIndex() const {
        if(!isSet)
            constexprError("Attempted to get insn index of unitialized AsmMarker!");
        return m_markedInstructionIndex;
    }
    constexpr auto getMarkedOperand() const {
        if(!isSet)
            constexprError("Attempted to get marked operand of unitialized AsmMarker!");
        return m_markedOperand;
    }
    constexpr auto getNameHash() const {
        if(!isSet)
            constexprError("Attempted to get name hash of unitialized AsmMarker!");
        return m_namehash;
    }
    constexpr bool isInitialized() const {
        return isSet;
    }
};

template<typename masterTemplate>
class AsmMarkers {
    using AsmMarker = AsmMarker<masterTemplate>;
    static constexpr unsigned maxMarkers = 64;
    AsmMarker m_markers[maxMarkers] = {};
    unsigned currentIndex = 0;
public:
    constexpr AsmMarkers(){};
    template<typename T>
    constexpr auto addMarker(T s, unsigned start, unsigned end, unsigned markerIndex) {
        using namespace cloture::util::ctfe;
        if(currentIndex >= maxMarkers)
            constexprError("Exceeded max AsmMarkers!");

        if(indexOfBounded(s, start, end, ".marker") - start != 0)
            constexprError("addMarker: start index should point to the start of .marker.");

        char nameOfMarker[256] = {};

        unsigned i = start + cstrlen(".marker");

        while(s[i] == ' ')
            ++i;
        unsigned j = 0;
        while(isIdentifierChar(s[i]))
            nameOfMarker[j++] = s[i++];

        auto hashOfName = cstrhash(&nameOfMarker[0]);

        while(s[i++] != '#') //find the index of the marked operand
            ;

        unsigned operandIndex = s[++i] - '0';
        m_markers[currentIndex].set(currentIndex, markerIndex, operandIndex, hashOfName);
        currentIndex++;
        return i + 1;
    }
    constexpr auto findByInsnIndex(unsigned index) {
        for( auto& marker : m_markers) {
            if(!marker.isInitialized())
                constexprError("No marker exists with the given instruction index.");
            if(marker.getInsnIndex() == index)
                return marker;
        }
        return m_markers[0];
    }
};

template</*int len, const cloture::util::ctfe::CString<len + 1> s*/typename T>
class ArmAssembler__ {
    static constexpr auto s = T::str;

    static constexpr int len = s.length();
    static constexpr unsigned insnCount = countInstances(s, len, ';');
    static constexpr unsigned labelCount = countInstances(s, len, ':');

    using ArmInsn = ArmInsn< ArmAssembler__<T> >;
    using AsmMarkers = AsmMarkers <ArmAssembler__<T>>;
    unsigned int insns[insnCount];
    ArmInsn armInsns[insnCount];
    AsmMarkers m_markers;
    #include "ArmBlockList_impl.h"
    template<typename T>
    static constexpr ArmInsn assembleInstruction(T s, unsigned start, unsigned length) {
        /*char mnem[128] = {};
        unsigned end = getIndexOfNext(s, start, length, ';');
        unsigned mnemEnd = getIndexOfNext(s, start, length, ' ');
        //unsigned paramCount = countInstances(s, end, ',');
        copyToBuffer(mnem, s, start, mnemEnd);*/
        return ArmInsn(s, start, length);
    }
    //ArmBlockList<0, 0> blockList;
public:
    constexpr ArmAssembler__() /* blockList()*/{
        unsigned j = 0;
        using namespace cloture::util::ctfe;
        for(unsigned i = 0; i < len; ++j) {

            //insns[j] = reverseByteOrderForArm(assembleInstruction(s, i, len));
            unsigned markerIndex = -1;
            if((markerIndex = indexOfBounded(s, i, getIndexOfNext(s, i, len, ';'), ".marker")) != -1) {
                i = m_markers.addMarker(s, markerIndex, getIndexOfNext(s, i, len, ';'), j);
            }
            //else
                armInsns[j] = //assembleInstruction(s, i, len);
                    ArmInsn(*this, s, i, len);
#if !NOASSEMBLE
            insns[j] = reverseByteOrderForArm(armInsns[j].assemble());
#endif
            i = getIndexOfNext(s, i, len, ';');
            if( i == -1)
                return;
            i += 1;
        }

    }
    using arrayreturnType = const unsigned (&)[insnCount];
    __forceinline constexpr arrayreturnType
    getInsns() const {
        return insns;
    }
    __forceinline constexpr unsigned getInsnCount() const {
        return insnCount;
    }

    constexpr cloture::util::ctfe::Array<unsigned, insnCount>
    getArray() const {
        return cloture::util::ctfe::Array<unsigned, insnCount>(insns);
    };
    constexpr const ArmInsn& getInsnUnassembled(unsigned index) const {
        return armInsns[index];
    }
};

#if 1
template<typename T>
class ArmAssembler : public ArmAssembler__<T> {
    using baseType = ArmAssembler__<T>;
public:
    constexpr ArmAssembler() : baseType::ArmAssembler__() {}
};
#else
template<typename T>
class ArmAssembler : public ArmAssembler__<typeof(T::str), T::str> {
    using baseType = ArmAssembler__<typeof(T::str), T::str>;
public:
    constexpr ArmAssembler() : baseType::ArmAssembler__() {}
};
#endif