/*
 * Copyright (C) 2009, 2010, 2012 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef LinkBuffer_h
#define LinkBuffer_h

#include <Platform.h>

#if ENABLE(ASSEMBLER)

#define DUMP_LINK_STATISTICS 0
#define DUMP_CODE 0

#define GLOBAL_THUNK_ID reinterpret_cast<void*>(static_cast<intptr_t>(-1))
#define REGEXP_CODE_ID reinterpret_cast<void*>(static_cast<intptr_t>(-2))

#include "JITCompilationEffort.h"
#include "MacroAssembler.h"
#include "Options.h"
#include <wtf/DataLog.h>
#include <wtf/Noncopyable.h>

namespace JSC {

class JSGlobalData;

template <typename T>
struct DefaultExecutableOffsetCalculator {
    template <typename Assembler>
    static T applyOffset(Assembler *, T src) { return src; }
};

// LinkBuffer:
//
// This class assists in linking code generated by the macro assembler, once code generation
// has been completed, and the code has been copied to is final location in memory.  At this
// time pointers to labels within the code may be resolved, and relative offsets to external
// addresses may be fixed.
//
// Specifically:
//   * Jump objects may be linked to external targets,
//   * The address of Jump objects may taken, such that it can later be relinked.
//   * The return address of a Call may be acquired.
//   * The address of a Label pointing into the code may be resolved.
//   * The value referenced by a DataLabel may be set.
//
template <typename MacroAssembler, template <typename T> class ExecutableOffsetCalculator>
class LinkBufferBase {
    WTF_MAKE_NONCOPYABLE(LinkBufferBase)
    typedef MacroAssemblerCodeRef CodeRef;
    typedef MacroAssemblerCodePtr CodePtr;
    typedef typename MacroAssembler::Label Label;
    typedef typename MacroAssembler::Jump Jump;
    typedef typename MacroAssembler::PatchableJump PatchableJump;
    typedef typename MacroAssembler::JumpList JumpList;
    typedef typename MacroAssembler::Call Call;
    typedef typename MacroAssembler::DataLabelCompact DataLabelCompact;
    typedef typename MacroAssembler::DataLabel32 DataLabel32;
    typedef typename MacroAssembler::DataLabelPtr DataLabelPtr;
    typedef typename MacroAssembler::ConvertibleLoadLabel ConvertibleLoadLabel;

public:
    LinkBufferBase(JSGlobalData& globalData, MacroAssembler* masm, JITCompilationEffort effort = JITCompilationMustSucceed)
        : m_size(0)
        , m_code(0)
        , m_assembler(masm)
        , m_globalData(&globalData)
#ifndef NDEBUG
        , m_completed(false)
        , m_effort(effort)
#endif
    {
#ifdef NDEBUG
        UNUSED_PARAM(effort);
#endif
        // Simon: Moved this to the sub-classes linkCode(ownerUID, effort);
    }

    ~LinkBufferBase()
    {
        ASSERT(m_completed || (!m_executableMemory && m_effort == JITCompilationCanFail));
    }
    
    bool didFailToAllocate() const
    {
        return !m_executableMemory;
    }

    bool isValid() const
    {
        return !didFailToAllocate();
    }
    
    // These methods are used to link or set values at code generation time.

    void link(Call call, FunctionPtr function)
    {
        ASSERT(call.isFlagSet(Call::Linkable));
        call.m_label = applyOffset(call.m_label);
        MacroAssembler::linkCall(code(), call, function);
    }
    
    void link(Jump jump, CodeLocationLabel label)
    {
        jump.m_label = applyOffset(jump.m_label);
        MacroAssembler::linkJump(code(), jump, label);
    }

    void link(JumpList list, CodeLocationLabel label)
    {
        for (unsigned i = 0; i < list.m_jumps.size(); ++i)
            link(list.m_jumps[i], label);
    }

    void patch(DataLabelPtr label, void* value)
    {
        AssemblerLabel target = applyOffset(label.m_label);
        MacroAssembler::linkPointer(code(), target, value);
    }

    void patch(DataLabelPtr label, CodeLocationLabel value)
    {
        AssemblerLabel target = applyOffset(label.m_label);
        MacroAssembler::linkPointer(code(), target, value.executableAddress());
    }

    // These methods are used to obtain handles to allow the code to be relinked / repatched later.

    CodeLocationCall locationOf(Call call)
    {
        ASSERT(call.isFlagSet(Call::Linkable));
        ASSERT(!call.isFlagSet(Call::Near));
        return CodeLocationCall(MacroAssembler::getLinkerAddress(code(), applyOffset(call.m_label)));
    }

    CodeLocationNearCall locationOfNearCall(Call call)
    {
        ASSERT(call.isFlagSet(Call::Linkable));
        ASSERT(call.isFlagSet(Call::Near));
        return CodeLocationNearCall(MacroAssembler::getLinkerAddress(code(), applyOffset(call.m_label)));
    }

    CodeLocationLabel locationOf(PatchableJump jump)
    {
        return CodeLocationLabel(MacroAssembler::getLinkerAddress(code(), applyOffset(jump.m_jump.m_label)));
    }

    CodeLocationLabel locationOf(Label label)
    {
        return CodeLocationLabel(MacroAssembler::getLinkerAddress(code(), applyOffset(label.m_label)));
    }

    CodeLocationDataLabelPtr locationOf(DataLabelPtr label)
    {
        return CodeLocationDataLabelPtr(MacroAssembler::getLinkerAddress(code(), applyOffset(label.m_label)));
    }

    CodeLocationDataLabel32 locationOf(DataLabel32 label)
    {
        return CodeLocationDataLabel32(MacroAssembler::getLinkerAddress(code(), applyOffset(label.m_label)));
    }
    
    CodeLocationDataLabelCompact locationOf(DataLabelCompact label)
    {
        return CodeLocationDataLabelCompact(MacroAssembler::getLinkerAddress(code(), applyOffset(label.m_label)));
    }

    CodeLocationConvertibleLoad locationOf(ConvertibleLoadLabel label)
    {
        return CodeLocationConvertibleLoad(MacroAssembler::getLinkerAddress(code(), applyOffset(label.m_label)));
    }

    // This method obtains the return address of the call, given as an offset from
    // the start of the code.
    unsigned returnAddressOffset(Call call)
    {
        call.m_label = applyOffset(call.m_label);
        return MacroAssembler::getLinkerCallReturnOffset(call);
    }

    uint32_t offsetOf(Label label)
    {
        return applyOffset(label.m_label).m_offset;
    }

    // Upon completion of all patching 'FINALIZE_CODE()' should be called once to
    // complete generation of the code. Alternatively, call
    // finalizeCodeWithoutDisassembly() directly if you have your own way of
    // displaying disassembly.

    inline CodeRef finalizeCodeWithoutDisassembly();
    inline CodeRef finalizeCodeWithDisassembly(const char *jitKind, const char* func);

    CodePtr trampolineAt(Label label)
    {
        return CodePtr(MacroAssembler::AssemblerType_T::getRelocatedAddress(code(), applyOffset(label.m_label)));
    }

    void* debugAddress()
    {
        return m_code;
    }
    
    size_t debugSize()
    {
        return m_size;
    }

    inline bool makeExecutable();

private:
    template <typename T> T applyOffset(T src)
    {
        return ExecutableOffsetCalculator<T>::applyOffset(m_assembler, src);
    }
    
protected:
    // Keep this private! - the underlying code should only be obtained externally via finalizeCode().
    void* code()
    {
        return m_code;
    }

    inline void linkCode(void* ownerUID, JITCompilationEffort);

    virtual void performFinalization();

#if DUMP_LINK_STATISTICS
    static void dumpLinkStatistics(void* code, size_t initialSize, size_t finalSize);
#endif
    
#if DUMP_CODE
    static void dumpCode(void* code, size_t);
#endif
    
    RefPtr<ExecutableMemoryHandle> m_executableMemory;
    size_t m_size;
    void* m_code;
    MacroAssembler* m_assembler;
    JSGlobalData* m_globalData;
protected:
#ifndef NDEBUG
    bool m_completed;
    JITCompilationEffort m_effort;
#endif
};

#define FINALIZE_CODE_IF(condition, linkBufferReference, jitKind, func)  \
    (UNLIKELY((condition))                                              \
     ? ((linkBufferReference).finalizeCodeWithDisassembly (jitKind, func)) \
     : (linkBufferReference).finalizeCodeWithoutDisassembly())

// Use this to finalize code, like so:
//
// CodeRef code = FINALIZE_CODE(linkBuffer, ("my super thingy number %d", number));
//
// Which, in disassembly mode, will print:
//
// Generated JIT code for my super thingy number 42:
//     Code at [0x123456, 0x234567]:
//         0x123456: mov $0, 0
//         0x12345a: ret
//
// ... and so on.
//
// Note that the dataLogFArgumentsForHeading are only evaluated when showDisassembly
// is true, so you can hide expensive disassembly-only computations inside there.

#define FINALIZE_CODE(linkBufferReference, jitKind, func)  \
    FINALIZE_CODE_IF(Options::showDisassembly(), linkBufferReference, jitKind, func)

#define FINALIZE_DFG_CODE(linkBufferReference, jitKind, func)  \
    FINALIZE_CODE_IF((Options::showDisassembly() || Options::showDFGDisassembly()), linkBufferReference, jitKind, func)


template <typename MacroAssembler, template <typename T> class ExecutableOffsetCalculator>
inline typename LinkBufferBase<MacroAssembler, ExecutableOffsetCalculator>::CodeRef LinkBufferBase<MacroAssembler, ExecutableOffsetCalculator>::finalizeCodeWithoutDisassembly()
{
    performFinalization();

    return CodeRef(m_executableMemory);
}

template <typename MacroAssembler, template <typename T> class ExecutableOffsetCalculator>
inline typename LinkBufferBase<MacroAssembler, ExecutableOffsetCalculator>::CodeRef LinkBufferBase<MacroAssembler, ExecutableOffsetCalculator>::finalizeCodeWithDisassembly(const char *jitKind, const char* func)
{
    ASSERT(Options::showDisassembly() || Options::showDFGDisassembly());

    CodeRef result = finalizeCodeWithoutDisassembly();

    dataLogF("Generated %s code for function %s:", jitKind, func);

    dataLogF(
#if OS(WINDOWS)
                "    Code at [0x%p, 0x%p):",
#else
                "    Code at [%p, %p):",
#endif
                result.code().executableAddress(), static_cast<char*>(result.code().executableAddress()) + result.size());
    disassemble(result.code(), m_size, "    ", WTF::dataFile());

    return result;
}

template <typename MacroAssembler, template <typename T> class ExecutableOffsetCalculator>
inline void LinkBufferBase<MacroAssembler, ExecutableOffsetCalculator>::linkCode(void* ownerUID, JITCompilationEffort effort)
{
    UNUSED_PARAM(ownerUID);
    UNUSED_PARAM(effort);
    ASSERT(!m_code);
    m_executableMemory = m_assembler->m_assembler.executableCopy(*m_globalData, ownerUID, effort);
    if (!m_executableMemory)
        return;
    m_code = m_executableMemory->codeStart();
    m_size = m_assembler->m_assembler.codeSize();
    ASSERT(m_code);
}

template <typename MacroAssembler, template <typename T> class ExecutableOffsetCalculator>
void LinkBufferBase<MacroAssembler, ExecutableOffsetCalculator>::performFinalization()
{
    // NOTE: This function is specialized in LinkBuffer<MacroAssemblerARMv7>
#ifndef NDEBUG
    ASSERT(!m_completed);
    ASSERT(isValid());
    m_completed = true;
#endif

    ASSERT(m_size <= INT_MAX);
    MacroAssembler::cacheFlush(code(), m_size);
}

template <typename MacroAssembler, template <typename T> class ExecutableOffsetCalculator>
inline bool LinkBufferBase<MacroAssembler, ExecutableOffsetCalculator>::makeExecutable()
{
    return ExecutableAllocator::makeExecutable(m_executableMemory->memoryStart(),
                                               m_executableMemory->memorySize());
}

template <typename MacroAssembler>
class LinkBuffer : public LinkBufferBase<MacroAssembler, DefaultExecutableOffsetCalculator>
{
public:
    LinkBuffer(JSGlobalData& globalData, MacroAssembler* masm, void* ownerUID, JITCompilationEffort effort = JITCompilationMustSucceed)
        : LinkBufferBase<MacroAssembler, DefaultExecutableOffsetCalculator>(globalData, masm, effort)
    {
        this->linkCode(ownerUID, effort);
    }
};

#if CPU(ARM_THUMB2) || CPU(ARM64)

template <typename T>
struct BranchCompactingExecutableOffsetCalculator {
    template <typename Assembler>
    static T applyOffset(Assembler *as, T src) {
        src.m_offset -= as->executableOffsetFor(src.m_offset);
        return src;
    }
};

template <typename MacroAssembler>
class BranchCompactingLinkBuffer : public LinkBufferBase<MacroAssembler, BranchCompactingExecutableOffsetCalculator>
{
public:
    BranchCompactingLinkBuffer(JSGlobalData& globalData, MacroAssembler* masm, void* ownerUID, JITCompilationEffort effort = JITCompilationMustSucceed)
        : LinkBufferBase<MacroAssembler, BranchCompactingExecutableOffsetCalculator>(globalData, masm, effort)
    {
        linkCode(ownerUID, effort);
    }

    virtual void performFinalization() override final;
    inline bool makeExecutable();

    inline void linkCode(void* ownerUID, JITCompilationEffort);

private:
    using Base = LinkBufferBase<MacroAssembler, BranchCompactingExecutableOffsetCalculator>;
#ifndef NDEBUG
    using Base::m_completed;
#endif
    using Base::isValid;
    using Base::code;
    using Base::m_code;
    using Base::m_size;
    using Base::m_assembler;
    using Base::m_executableMemory;
    using Base::m_globalData;

    using LinkRecord = typename MacroAssembler::LinkRecord;
    using JumpLinkType = typename MacroAssembler::JumpLinkType;

    size_t m_initialSize = 0;
};

template <typename MacroAssembler>
void BranchCompactingLinkBuffer<MacroAssembler>::performFinalization()
{
#ifndef NDEBUG
    ASSERT(!m_completed);
    ASSERT(isValid());
    this->m_completed = true;
#endif

    MacroAssembler::cacheFlush(code(), m_size);
}

template <typename MacroAssembler>
inline bool BranchCompactingLinkBuffer<MacroAssembler>::makeExecutable()
{
    return ExecutableAllocator::makeExecutable(code(), m_initialSize);
}

template <typename MacroAssembler>
inline void BranchCompactingLinkBuffer<MacroAssembler>::linkCode(void* ownerUID, JITCompilationEffort effort)
{
    UNUSED_PARAM(ownerUID);
    UNUSED_PARAM(effort);
    ASSERT(!m_code);
    m_initialSize = m_assembler->m_assembler.codeSize();
    m_executableMemory = m_globalData->executableAllocator.allocate(*m_globalData, m_initialSize, ownerUID, effort);
    if (!m_executableMemory)
        return;
    if (Q_UNLIKELY(!ExecutableAllocator::makeWritable(m_executableMemory->memoryStart(), m_executableMemory->memorySize()))) {
        m_executableMemory = {};
        return;
    }
    m_code = (uint8_t*)m_executableMemory->codeStart();
    ASSERT(m_code);
    uint8_t* inData = (uint8_t*)m_assembler->unlinkedCode();
    uint8_t* outData = reinterpret_cast<uint8_t*>(m_code);
    int readPtr = 0;
    int writePtr = 0;
    Vector<LinkRecord, 0, UnsafeVectorOverflow>& jumpsToLink = m_assembler->jumpsToLink();
    unsigned jumpCount = unsigned(jumpsToLink.size());
    for (unsigned i = 0; i < jumpCount; ++i) {
        int offset = readPtr - writePtr;
        ASSERT(!(offset & 1));

        // Copy the instructions from the last jump to the current one.
        unsigned regionSize = unsigned(jumpsToLink[i].from() - readPtr);
        uint16_t* copySource = reinterpret_cast_ptr<uint16_t*>(inData + readPtr);
        uint16_t* copyEnd = reinterpret_cast_ptr<uint16_t*>(inData + readPtr + regionSize);
        uint16_t* copyDst = reinterpret_cast_ptr<uint16_t*>(outData + writePtr);
        ASSERT(!(regionSize % 2));
        ASSERT(!(readPtr % 2));
        ASSERT(!(writePtr % 2));
        while (copySource != copyEnd)
            *copyDst++ = *copySource++;
        m_assembler->recordLinkOffsets(readPtr, jumpsToLink[i].from(), offset);
        readPtr += regionSize;
        writePtr += regionSize;

        // Calculate absolute address of the jump target, in the case of backwards
        // branches we need to be precise, forward branches we are pessimistic
        const uint8_t* target;
        if (jumpsToLink[i].to() >= jumpsToLink[i].from())
            target = outData + jumpsToLink[i].to() - offset; // Compensate for what we have collapsed so far
        else
            target = outData + jumpsToLink[i].to() - m_assembler->executableOffsetFor(jumpsToLink[i].to());

        JumpLinkType jumpLinkType = m_assembler->computeJumpType(jumpsToLink[i], outData + writePtr, target);
        // Compact branch if we can...
        if (m_assembler->canCompact(jumpsToLink[i].type())) {
            // Step back in the write stream
            int32_t delta = m_assembler->jumpSizeDelta(jumpsToLink[i].type(), jumpLinkType);
            if (delta) {
                writePtr -= delta;
                m_assembler->recordLinkOffsets(jumpsToLink[i].from() - delta, readPtr, readPtr - writePtr);
            }
        }
        jumpsToLink[i].setFrom(writePtr);
    }
    // Copy everything after the last jump
    memcpy(outData + writePtr, inData + readPtr, m_initialSize - readPtr);
    m_assembler->recordLinkOffsets(readPtr, unsigned(m_initialSize), readPtr - writePtr);

    for (unsigned i = 0; i < jumpCount; ++i) {
        uint8_t* location = outData + jumpsToLink[i].from();
        uint8_t* target = outData + jumpsToLink[i].to() - m_assembler->executableOffsetFor(jumpsToLink[i].to());
        m_assembler->link(jumpsToLink[i], location, target);
    }

    jumpsToLink.clear();
    m_size = writePtr + m_initialSize - readPtr;
    m_executableMemory->shrink(m_size);
}

#if CPU(ARM_THUMB2)
template <>
class LinkBuffer<JSC::MacroAssembler<MacroAssemblerARMv7>> : public BranchCompactingLinkBuffer<JSC::MacroAssembler<MacroAssemblerARMv7>>
{
public:
    LinkBuffer(JSGlobalData& globalData, JSC::MacroAssembler<MacroAssemblerARMv7>* masm, void* ownerUID, JITCompilationEffort effort = JITCompilationMustSucceed)
        : BranchCompactingLinkBuffer<JSC::MacroAssembler<MacroAssemblerARMv7>>(globalData, masm, ownerUID, effort)
    {}
};
#endif

#if CPU(ARM64)
template <>
class LinkBuffer<JSC::MacroAssembler<MacroAssemblerARM64>> : public BranchCompactingLinkBuffer<JSC::MacroAssembler<MacroAssemblerARM64>>
{
public:
    LinkBuffer(JSGlobalData& globalData, JSC::MacroAssembler<MacroAssemblerARM64>* masm, void* ownerUID, JITCompilationEffort effort = JITCompilationMustSucceed)
        : BranchCompactingLinkBuffer<JSC::MacroAssembler<JSC::MacroAssemblerARM64>>(globalData, masm, ownerUID, effort)
    {}
};
#endif

#endif

#if CPU(ARM_THUMB2)
typedef LinkBuffer<MacroAssembler<MacroAssemblerARMv7>> DefaultLinkBuffer;
#elif CPU(ARM64)
typedef LinkBuffer<MacroAssembler<MacroAssemblerARM64>> DefaultLinkBuffer;
#elif CPU(MIPS)
typedef LinkBuffer<MacroAssembler<MacroAssemblerMIPS>> DefaultLinkBuffer;
#elif CPU(X86)
typedef LinkBuffer<MacroAssembler<MacroAssemblerX86>> DefaultLinkBuffer;
#elif CPU(X86_64)
typedef LinkBuffer<MacroAssembler<MacroAssemblerX86_64>> DefaultLinkBuffer;
#endif

} // namespace JSC

#endif // ENABLE(ASSEMBLER)

#endif // LinkBuffer_h
