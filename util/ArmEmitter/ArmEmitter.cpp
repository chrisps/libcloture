//
// Created by chris on 2/9/17.
//
#include "util/stl.hpp"
#include "ArmEmitter.h"
#include "glitch-common.h"

#include <sys/mman.h>
#include <unistd.h>
using namespace cs;

ArmEmitter::ArmEmitter(const insn_t *insns, unsigned length) {
    buff_orig = mmap (NULL, length + buff_alignment + 1, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    buff_orig_length = length + buff_alignment + 1;
    buff_aligned = (void*)((reinterpret_cast<uintptr_t>(buff_orig) + (buff_alignment - 1)) & -buff_alignment);
    cs::copyMemory((insn_t*)buff_aligned, insns, length);
    //__builtin___clear_cache((char*)buff_aligned, (char*)buff_aligned + (length*sizeof(int)));
    cacheflush((uintptr_t)buff_aligned, (uintptr_t)buff_aligned + (length*(sizeof(unsigned))), 0);
}
ArmEmitter::~ArmEmitter() {
    munmap(buff_orig, buff_orig_length);
}

using loltype = mMetaString("movne r1, #1; mov r0, #0; add r1, #1; .marker cmpVal #1 cmp r1, #66; mov r0, r1 lsl #3; bx lr;", 1024);
static constexpr cs::ArmAssembler<loltype> aTest;

void testcall(int* x) {
    using namespace cloture::util::ctfe;
    static typeof(aTest.getArray()) arr = aTest.getArray();
   // static int* xxx = &arr[0];

    cs::ArmEmitter *emitter = new cs::ArmEmitter((const unsigned int*)&arr[0], arr.size());//aTest);
    int result = (*emitter)();
    delete emitter;
}