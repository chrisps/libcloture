//
// Created by chris on 2/9/17.
//

#ifndef IMAGEBENDER_ARMEMITTER_H
#define IMAGEBENDER_ARMEMITTER_H
#include <stdexcept>
namespace cs {
    using insn_t = unsigned;
    #include "ArmAsmUtils.h"
    #include "ArmAssembler_impl.h"
    class ArmEmitter {
        static constexpr unsigned buff_alignment = 16;

        void* buff_orig;
        unsigned buff_orig_length;
        void* buff_aligned;

public:
        ArmEmitter(const insn_t* insns, unsigned length);
        template<typename T>
        ArmEmitter(const T assembler) : ArmEmitter(assembler.getInsns(), assembler.getInsnCount()) {

        }
        template<unsigned length>
        ArmEmitter(const insn_t (&insns)[length]) : ArmEmitter(&insns[0], length) {}
        ~ArmEmitter();
        template<typename... ts>
        unsigned operator ()(ts... args) {
            using funcptr_type = unsigned (*)(ts...);
            funcptr_type func = (funcptr_type)buff_aligned;
            /*asm volatile ("pli %[funcptr]"
            :
            : [funcptr] "r" (func)
            );*/
            return func(args...);
        }
        void* getFunc() {
            return buff_aligned;
        }
    };



}
#endif //IMAGEBENDER_ARMEMITTER_H
