#pragma once

namespace ArmAsmUtils {
    using namespace cloture::util::common;
    using namespace cloture::util::ctfe;
    enum class barrelType : uint8 {
        lsl,
        lsr,
        asr,
        ror,
        rrx,
        none
    };
    static constexpr void ArmAsmError(const char* s, int extra = 2) {
        auto x = s[cstrlen(s) + extra];
    }
    static constexpr void ArmAsmErrorStr(const char* s, const char* str, int extra = 2) {
        auto x = s[cstrlen(s) + extra];
    }
    /*
 * rotation for the constant is at bits 8-11, the encoded constant is kept at
 * 0-7
 * returns -1 if the value couldnt be encoded as a constant
 */
    constexpr const char* regNames[] = {
            "r0",
            "r1",
            "r2",
            "r3",
            "r4",
            "r5",
            "r6",
            "r7",
            "r8",
            "r9",
            "r10",
            "r11",
            "r12",
            "r13",
            "r14",
            "r15",
            "sp",
            "lr",
            "pc",
            "ip",
    };

    constexpr bool isValidGP(int regIndex) {
        return regIndex >= 0 && regIndex < 16;
    }

    constexpr const char* resolveRegisterAlias(const char* rname) {
        strhash_t hash = cstrhash(rname);
        switch(hash) {
            case cstrhash("sp"):
                return "r13";
            case cstrhash("lr"):
                return "r14";
            case cstrhash("ip"):
                return "r12";
            case cstrhash("pc"):
                return "r15";
            default:
                return rname;
        }
    }



    static constexpr int encodeImm12(unsigned int n)
    {
        for (int i = 0; i < 16; i++, n = (n << 2) | (n >> 30)) {
            if (n <= 255)
                return (int) (n + (i << 8));
        }
        return -1;
    }
    static constexpr uint32 stuff_const(uint32 op, uint32 c)
    {
        int try_neg=0;
        uint32 nc = 0, negop = 0;

        switch(op&0x1F00000)
        {
            case 0x800000: //add
            case 0x400000: //sub
                try_neg=1;
                negop=op^0xC00000;
                nc=-c;
                break;
            case 0x1A00000: //mov
            case 0x1E00000: //mvn
                try_neg=1;
                negop=op^0x400000;
                nc=~c;
                break;
            case 0x200000: //xor
                if(c==~0)
                    return (op&0xF010F000)|((op>>16)&0xF)|0x1E00000;
                break;
            case 0x0: //and
                if(c==~0)
                    return (op&0xF010F000)|((op>>16)&0xF)|0x1A00000;
            case 0x1C00000: //bic
                try_neg=1;
                negop=op^0x1C00000;
                nc=~c;
                break;
            case 0x1800000: //orr
                if(c==~0)
                    return (op&0xFFF0FFFF)|0x1E00000;
                break;
        }
        do {
            uint32 m = 0;
            int i = 0;
            if(c<256) /* catch undefined <<32 */
                return op|c;
            for(i=2;i<32;i+=2) {
                m=(0xff>>i)|(0xff<<(32-i));
                if(!(c&~m))
                    return op|(i<<7)|(c<<i)|(c>>(32-i));
            }
            op=negop;
            c=nc;
        } while(try_neg--);
        return op | c;
    }
    constexpr uint32 encbranch(int pos, int addr, int fail)
    {
        addr-=pos+8;
        addr/=4;
        if(addr>=0x1000000 || addr<-0x1000000) {
            if(fail)
                ArmAsmError("FIXME: function bigger than 32MB");
            return 0;
        }
        return 0x0A000000|(addr&0xffffff);
    }

    class armCC_t {
        const uint8 m_value;
        const uint16 m_txt;
    public:
        constexpr armCC_t(const char* txt, unsigned value) : m_value(static_cast<uint8>(value)),
                                                             m_txt(cstrlen(txt) >= 2 ? txt[0] << 8 | txt[1] : 0) {
        }
        constexpr uint32 value() const {
            return m_value << 24;
        }
        constexpr bool compare(const char* s) const {
            if(isNullCC())
                return false;
            unsigned len = cstrlen(s);
            return s[len - 1] == static_cast<char>(m_txt) && s[len - 2] == static_cast<char>(m_txt >> 8);
        }
        constexpr bool isNullCC() const {
            return m_value == 0xFF;
        }
        constexpr bool operator ==(const armCC_t& other) const {
            if(other.m_txt == m_txt && other.m_value == m_value)
                return true;
            return false;
        }
    };
    constexpr armCC_t armConditions[] = {
        armCC_t("lo", 0x30),
        armCC_t("hs", 0x20),
        armCC_t("eq", 0),
        armCC_t("ne", 0x10),
        armCC_t("ls", 0x90),
        armCC_t("hi", 0x80),
        armCC_t("mi", 0x40),
        armCC_t("pl", 0x50),
        armCC_t("lt", 0xB0),
        armCC_t("ge", 0xA0),
        armCC_t("le", 0xD0),
        armCC_t("gt", 0xC0),
        armCC_t("", 0xFF) //null cc
    };
    constexpr const armCC_t& getArmCC(unsigned index) {
        return armConditions[index];
    }

    constexpr const armCC_t& mapcc(const char* cc)
    {
        for(auto&& i : armConditions)
            if(i.compare(cc))
                return i;

        //find the null cc and return it
        for(auto&& i : armConditions)
            if(i.isNullCC())
                return i;
        //return armConditions[12];
    }
    constexpr const unsigned indexOfCC(const armCC_t& cc) {
        unsigned index = 0;
        for(auto&& i: armConditions) {
            if(i == cc)
                return index;
            index++;
        }
        ArmAsmError("Failed to find CC!", cc.value());
        return -1;
    }
    class armOpcode_t {
        const uint32 m_opc;
        const strhash_t m_hash;
    public:
        constexpr armOpcode_t(const char* mnem, uint32 opc) : m_hash(cstrhash(mnem)), m_opc(opc){

        }
        constexpr bool operator ==(strhash_t hash) const {
            return m_hash == hash;
        }
        constexpr uint32 opcode() const {
            return m_opc >> 1;
        }
        constexpr bool setsFlags() const {
            return m_opc & 1;
        }
        constexpr uint32 encode() const {
            return m_opc << 20;
        }
        constexpr unsigned expectedOperandCount() const {
            //just in "case" ;)
#pragma push_macro("_case");

#define _case(op)   case cstrhash(#op)
            switch(m_hash) {
                _case(bx):
                _case(b):
                    return 1;
#define doublecase(op)      _case(#op): _case(#op "s")
                doublecase(add):
                doublecase(sub):
                doublecase(addc):
                doublecase(subc):
                doublecase(eor):
                doublecase(orr):
                doublecase(and):
                doublecase(rsb):
                    return 3;
                doublecase(mov):
                doublecase(mvn):
                _case(cmp):
                _case(cmn):
                _case(tst):
                _case(teq):
                    return 2;
                default:
                    return 0;
            }
#undef _case
#undef doublecase
#pragma pop_macro("_case");
        }
    };

#define o(op, val)      armOpcode_t(#op , val), armOpcode_t(#op "s" , val | 1)
    constexpr armOpcode_t integerOps[] = {
            o(add, 0x8),
            o(sub, 0x4),
            o(addc, 0xA),
            o(subc, 0xC),
            o(eor, 0x2),
            o(orr, 0x18),
            o(and, 0),
            o(mov, 0x1A),
            o(mvn, 0x1E),
            o(rsb, 0x6),
            o(cmp, 0x14 | 1),
            o(cmn, 0x16 | 1),
            o(bx, 0x12FFF10 << 1)
    };

#undef o

    constexpr const armOpcode_t& getIntegerOp(const char* op)
    {
        int c = 0, func = 0;
        uint32 opc = 0, r = 0, fr = 0;
        const strhash_t ophash = cstrhash(op);
        for(auto&& i : integerOps) {
            if( i == ophash)
                return i;
        }
        ArmAsmErrorStr(op, "No match for integer op.");

        return integerOps[0];
    }

    constexpr strhash_t constOp2s[] = {
            cstrhash("mov"),
            cstrhash("mvn"),
            cstrhash("cmp"),
            cstrhash("tst"),
            cstrhash("teq"),
            cstrhash("movt"),
            cstrhash("cmn")
    };

    constexpr bool op1BelongsInRN(strhash_t hash) {
        for(auto&& i: constOp2s)
            if(i == hash)
                return true;
        return false;
    }
    static_assert(op1BelongsInRN(cstrhash("mov")) && !op1BelongsInRN(cstrhash("add")));
}