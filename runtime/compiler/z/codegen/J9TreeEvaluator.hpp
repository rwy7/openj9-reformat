/*******************************************************************************
 * Copyright (c) 2000, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#ifndef J9_Z_TREE_EVALUATOR_INCL
#define J9_Z_TREE_EVALUATOR_INCL

/*
 * The following #define and typedef must appear before any #includes in this file
 */
#ifndef J9_TREE_EVALUATOR_CONNECTOR
#define J9_TREE_EVALUATOR_CONNECTOR
namespace J9 {
namespace Z {
class TreeEvaluator;
}
} // namespace J9
namespace J9 {
typedef J9::Z::TreeEvaluator TreeEvaluatorConnector;
}
#else
#error J9::Z::TreeEvaluator expected to be a primary connector, but a J9 connector is already defined
#endif

#include "compiler/codegen/J9TreeEvaluator.hpp" // include parent

//#define TRACE_EVAL
#if defined(TRACE_EVAL)
//
// this is a handy thing to turn on if you want to figure out what the common
// code generator is mapping nodes to and what is really being driven through
// evaluation.
// It is not on by default (too noisy and expensive) but can be enabled
// by just defining TRACE_EVAL for this file
//
// If we wanted to enable this by default, it would make sense to create a
// new 'phase' for debugging that could be queried and to have the macro
// be a test of the tracing before making a function call out, e.g.
// instead of PRINT_ME(...) we would have:
// if (compilation->getOutFile() != NULL && compilation->getTraceEval())
//   {
//     print("iadd", compilation->getOutFile());
//   }
// and then traceEval would be a forced no-inline method
//
// Add another version of PRINT_ME, undef EVAL_BLOCK to go back to the old one
#define EVAL_BLOCK
#if defined(EVAL_BLOCK)
#define PRINT_ME(string, node, cg) \
    TR::Delimiter evalDelimiter(cg->comp(), cg->comp()->getOption(TR_TraceCG), "EVAL", string)
#else
void PRINT_ME(char* string, TR::Node* node, TR::CodeGenerator* cg)
{
    TR::Compilation* compilation = cg->comp();
    TR::FILE* outFile = compilation->getOutFile();
    if (outFile != NULL) {
        diagnostic("EVAL: %s\n", string);
    }
}
#endif
#else
#define PRINT_ME(string, node, cg)
#endif

#define INSN_HEAP cg->trHeapMemory()

namespace J9 {

namespace Z {

class OMR_EXTENSIBLE TreeEvaluator : public J9::TreeEvaluator {
public:
    static TR::Register* awrtbarEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* awrtbariEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* monentEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* monexitEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* monexitfenceEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* asynccheckEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* instanceofEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* checkcastEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* checkcastAndNULLCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* generateHelperCallForVMNewEvaluators(
        TR::Node* node, TR::CodeGenerator* cg, bool doInlineAllocation = false, TR::Register* resReg = NULL);
    static TR::Register* newObjectEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* newArrayEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* anewArrayEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* multianewArrayEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* arraylengthEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* resolveCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* DIVCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* BNDCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ArrayCopyBNDCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* BNDCHKwithSpineCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ArrayStoreCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ArrayCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* conditionalHelperEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static float interpreterProfilingInstanceOfOrCheckCastTopProb(TR::CodeGenerator* cg, TR::Node* node);

    /*           START BCD Evaluators          */
    // Used for VPSOP in vector setSign operations
    enum SignOperationType {
        maintain, // maintain sign
        complement, // complement sign
        setSign // force positive or negative
    };

    static TR::Register* df2zdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* zd2ddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* zdsls2zdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* zd2zdslsEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2zdslsEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* zdsls2pdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static void zonedToZonedSeparateSignHelper(TR::Node* node, TR_PseudoRegister* srcReg, TR_PseudoRegister* targetReg,
        TR::MemoryReference* sourceMR, TR::MemoryReference* destMR, TR::CodeGenerator* cg);
    static TR::MemoryReference* packedToZonedHelper(TR::Node* node, TR_PseudoRegister* targetReg,
        TR::MemoryReference* sourceMR, TR_PseudoRegister* childReg, TR::CodeGenerator* cg);
    static void pd2zdSignFixup(TR::Node* node, TR::MemoryReference* destMR, TR::CodeGenerator* cg);

    static void zonedSeparateSignToPackedOrZonedHelper(TR::Node* node, TR_PseudoRegister* targetReg,
        TR::MemoryReference* sourceMR, TR::MemoryReference* destMR, TR::CodeGenerator* cg);
    static TR::Register* zdsle2zdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::MemoryReference* zonedToPackedHelper(TR::Node* node, TR_PseudoRegister* targetReg,
        TR::MemoryReference* sourceMR, TR_PseudoRegister* childReg, TR::CodeGenerator* cg);
    static TR::MemoryReference* packedToUnicodeHelper(TR::Node* node, TR_PseudoRegister* targetReg,
        TR::MemoryReference* sourceMR, TR_PseudoRegister* childReg, bool isSeparateSign, TR::CodeGenerator* cg,
        TR_StorageReference* srcStorageReference);
    static TR::MemoryReference* asciiAndUnicodeToPackedHelper(TR::Node* node, TR_PseudoRegister* targetReg,
        TR::MemoryReference* sourceMR, TR_PseudoRegister* childReg, TR::CodeGenerator* cg);

    /*   PD -> UD/ZD   */
    static TR::Register* pd2udslEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2udslEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2udslVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2udEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2udEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2udVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2zdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2zdVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);

    /*   PD <- UD/ZD   */
    static TR::Register* ud2pdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ud2pdVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* udsl2pdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* zd2pdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* zd2pdVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);

    static bool isZonedOperationAnEffectiveNop(TR::Node* node, int32_t shiftAmount, bool isTruncation,
        TR_PseudoRegister* srcReg, bool isSetSign, int32_t sign, TR::CodeGenerator* cg);

    static TR::Register* BCDCHKEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* BCDCHKEvaluatorImpl(TR::Node* node, TR::CodeGenerator* cg, uint32_t numCallParam,
        uint32_t callChildStartIndex, bool isResultPD, bool isUseVector, bool isVariableParam);

    static TR::Register* df2pdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2ddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2iEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2lEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* i2pdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* l2pdEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pd2lVariableEvaluator(TR::Node* node, TR::CodeGenerator* cg, bool isUseVectorBCD);
    static TR::Register* generatePackedToBinaryConversion(
        TR::Node* node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator* cg);
    static TR::Register* generateVectorPackedToBinaryConversion(
        TR::Node* node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator* cg);
    static TR::Register* generateBinaryToPackedConversion(
        TR::Node* node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator* cg);
    static TR::Register* generateVectorBinaryToPackedConversion(
        TR::Node* node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator* cg);

    static void correctPackedArithmeticPrecision(TR::Node* node, int32_t op1EncodingSize, TR_PseudoRegister* targetReg,
        int32_t computedResultPrecision, TR::CodeGenerator* cg);

    static TR::Register* pdaddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdsubEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdaddsubEvaluatorHelper(TR::Node* node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator* cg);
    static TR::Register* pdArithmeticVectorEvaluatorHelper(
        TR::Node* node, TR::InstOpCode::Mnemonic op, TR::CodeGenerator* cg);

    static TR::Register* pdmulEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdmulEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);

    static TR::Register* pddivremEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pddivremEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pddivremVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);

    static TR::Register* pdnegEvaluator(TR::Node* node, TR::CodeGenerator* cg);

    static void clearAndSetSign(TR::Node* node, TR_PseudoRegister* targetReg, int32_t leftMostByteForClear,
        int32_t digitsToClear, TR::MemoryReference* destMR, TR_PseudoRegister* srcReg, TR::MemoryReference* sourceMR,
        bool isSetSign, int32_t sign, bool signCodeIsInitialized, TR::CodeGenerator* cg);

    static TR_PseudoRegister* simpleWideningOrTruncation(
        TR::Node* node, TR_PseudoRegister* srcReg, bool isSetSign, int32_t sign, TR::CodeGenerator* cg);

    static TR::Register* pdshrEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdshrEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdshrVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);

    static TR::Register* pdshlEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdshiftEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg, bool isRightShift);

    static TR::Register* pdModifyPrecisionEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdshlVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* vectorPerformSignOperationHelper(TR::Node* node, TR::CodeGenerator* cg, bool setPrecision,
        uint32_t precision, bool signedStatus, SignOperationType soType, bool signValidityCheck = false,
        int32_t sign = 0, bool setConditionCode = true);

    static TR::Register* pdchkEvaluator(TR::Node* node, TR::CodeGenerator* cg);

    static TR::Register* pdcmpeqEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdcmpneEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdcmpltEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdcmpgeEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdcmpgtEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdcmpleEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdcmpVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);

    static TR::Register* zdabsEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdloadEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdloadEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdloadVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdstoreEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdstoreEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdstoreVectorEvaluatorHelper(TR::Node* node, TR::CodeGenerator* cg);
    static TR_OpaquePseudoRegister* evaluateValueModifyingOperand(TR::Node* node, bool initTarget,
        TR::MemoryReference* sourceMR, TR::CodeGenerator* cg, bool trackSignState = false, int32_t srcSize = 0,
        bool alwaysLegalToCleanSign = false);
    static TR_PseudoRegister* evaluateBCDValueModifyingOperand(TR::Node* node, bool initTarget,
        TR::MemoryReference* sourceMR, TR::CodeGenerator* cg, bool trackSignState = false, int32_t srcSize = 0,
        bool alwaysLegalToCleanSign = false);
    static TR_OpaquePseudoRegister* evaluateSignModifyingOperand(TR::Node* node, bool isEffectiveNop,
        bool isNondestructiveNop, bool initTarget, TR::MemoryReference* sourceMR, TR::CodeGenerator* cg);
    static TR_PseudoRegister* evaluateBCDSignModifyingOperand(TR::Node* node, bool isEffectiveNop,
        bool isNondestructiveNop, bool initTarget, TR::MemoryReference* sourceMR, TR::CodeGenerator* cg);

    static TR::Register* pdclearEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* pdSetSignHelper(TR::Node* node, int32_t sign, TR::CodeGenerator* cg);
    static TR::Register* pdSetSignEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    /*           END BCD Evaluators          */

    /*           START DFP evaluators          */
    static TR::Register* deconstEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* deloadEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* destoreEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* deRegLoadEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* deRegStoreEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* df2fEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* f2dfEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* i2ddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* l2ddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* lu2ddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dd2lEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dd2luEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* df2ddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* df2deEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dd2dfEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dd2deEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* de2ddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* de2dxHelperAndSetRegister(
        TR::Register* targetReg, TR::Register* srcReg, TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dfaddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddaddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* deaddEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dfsubEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddsubEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* desubEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dfmulEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddmulEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* demulEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dfdivEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpeqEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpneEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpltEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpgeEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpgtEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpleEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpequEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpneuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpltuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpgeuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpgtuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ifddcmpleuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpeqEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpneEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpltEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpgeEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpgtEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpleEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpequEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpneuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpltuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpgeuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpgtuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcmpleuEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddnegEvaluator(TR::Node* node, TR::CodeGenerator* cg);

    static TR::Register* ddInsExpEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dffloorEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddfloorEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* defloorEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddshlEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddshrEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddshrRoundedEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddSetNegativeEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddModifyPrecisionEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* ddcleanEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* decleanEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* dd2iEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* de2iEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    /*           END DFP evaluators          */

    static TR::Register* countDigitsEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static void countDigitsHelper(TR::Node* node, TR::CodeGenerator* cg, int32_t memRefIndex,
        TR::MemoryReference* memRef, TR::Register* inputReg, TR::Register* countReg, TR::LabelSymbol* doneLabel,
        bool isLong);

    static TR::Register* tstartEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* tfinishEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* tabortEvaluator(TR::Node* node, TR::CodeGenerator* cg);

    static TR::Instruction* generateRuntimeInstrumentationOnOffSequence(TR::CodeGenerator* cg,
        TR::InstOpCode::Mnemonic op, TR::Node* node, TR::Instruction* preced = NULL, bool postRA = false);
    static TR::Instruction* genLoadForObjectHeaders(TR::CodeGenerator* cg, TR::Node* node, TR::Register* reg,
        TR::MemoryReference* tempMR, TR::Instruction* iCursor);
    static TR::Instruction* genLoadForObjectHeadersMasked(TR::CodeGenerator* cg, TR::Node* node, TR::Register* reg,
        TR::MemoryReference* tempMR, TR::Instruction* iCursor);
    static TR::Instruction* generateVFTMaskInstruction(
        TR::Node* node, TR::Register* reg, TR::CodeGenerator* cg, TR::Instruction* preced = NULL);
    static TR::Register* VMifInstanceOfEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static bool VMinlineCallEvaluator(TR::Node* node, bool, TR::CodeGenerator* cg);

    /** \brief
     *  Generates Sequence to check and use Guarded Load for ArrayCopy
     *
     * \param node
     *     The arraycopy node.
     *
     *  \param cg
     *     The code generator used to generate the instructions.
     *
     *  \param byteSrcReg
     *     Register holding starting address of source
     *
     *  \param byteDstReg
     *     Register holding starting address of destination
     *
     *  \param byteLenReg
     *     Register holding number of bytes to copy
     *
     *  \param mergeLabel
     *     Label Symbol to merge from generated OOL sequence
     *
     *  \param srm
     *     Scratch Register Manager providing pool of scratch registers to use
     *
     *  \param isForward
     *     Boolean specifying if we need to copy elements in forward direction
     *
     */
    static void genGuardedLoadOOL(TR::Node* node, TR::CodeGenerator* cg, TR::Register* byteSrcReg,
        TR::Register* byteDstReg, TR::Register* byteLenReg, TR::LabelSymbol* mergeLabel,
        TR_S390ScratchRegisterManager* srm, bool isForward);
    static void genArrayCopyWithArrayStoreCHK(TR::Node* node, TR::Register* srcObjReg, TR::Register* dstObjReg,
        TR::Register* srcAddrReg, TR::Register* dstAddrReg, TR::Register* lengthReg, TR::CodeGenerator* cg);
    static void genWrtbarForArrayCopy(
        TR::Node* node, TR::Register* srcObjReg, TR::Register* dstObjReg, bool srcNonNull, TR::CodeGenerator* cg);
    static TR::Register* VMgenCoreInstanceofEvaluator(TR::Node* node, TR::CodeGenerator* cg,
        TR::LabelSymbol* falseLabel, TR::LabelSymbol* trueLabel, bool needsResult, bool trueFallThrough,
        TR::RegisterDependencyConditions* conditions, bool isIfInstanceOf = false);
    static TR::Register* VMgenCoreInstanceofEvaluator2(TR::Node* node, TR::CodeGenerator* cg,
        TR::LabelSymbol* trueLabel, TR::LabelSymbol* falseLabel, bool initialResult, bool needResult,
        TR::RegisterDependencyConditions* conditions, bool ifInstanceOf = false);
    static TR::Register* VMinstanceOfEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMinstanceOfEvaluator2(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMifInstanceOfEvaluator2(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMcheckcastEvaluator2(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMcheckcastEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMmonentEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMmonexitEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMnewEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static TR::Register* VMarrayCheckEvaluator(TR::Node* node, TR::CodeGenerator* cg);
    static void restoreGPR7(TR::Node* node, TR::CodeGenerator* cg);
};
} // namespace Z

} // namespace J9

#endif
