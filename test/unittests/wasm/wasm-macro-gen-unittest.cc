// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "test/unittests/test-utils.h"

#include "src/wasm/wasm-macro-gen.h"

namespace v8 {
namespace internal {
namespace wasm {

class WasmMacroGenTest : public TestWithZone {};

#define EXPECT_SIZE(size, ...)     \
  do {                             \
    byte code[] = {__VA_ARGS__};   \
    EXPECT_EQ(size, sizeof(code)); \
  } while (false)


TEST_F(WasmMacroGenTest, Constants) {
  EXPECT_SIZE(2, WASM_ONE);
  EXPECT_SIZE(2, WASM_ZERO);

  EXPECT_SIZE(2, WASM_I8(122));
  EXPECT_SIZE(2, WASM_I8(254));

  EXPECT_SIZE(5, WASM_I32(1));
  EXPECT_SIZE(5, WASM_I32(10000));
  EXPECT_SIZE(5, WASM_I32(-9828934));

  EXPECT_SIZE(9, WASM_I64(1));
  EXPECT_SIZE(9, WASM_I64(10000));
  EXPECT_SIZE(9, WASM_I64(-9828934));
  EXPECT_SIZE(9, WASM_I64(0x123456789abcdef0ULL));

  EXPECT_SIZE(5, WASM_F32(1.0f));
  EXPECT_SIZE(5, WASM_F32(10000.0f));
  EXPECT_SIZE(5, WASM_F32(-9828934.0f));

  EXPECT_SIZE(9, WASM_F64(1.5));
  EXPECT_SIZE(9, WASM_F64(10200.0));
  EXPECT_SIZE(9, WASM_F64(-9818934.0));
}


TEST_F(WasmMacroGenTest, Statements) {
  EXPECT_SIZE(4, WASM_SET_LOCAL(0, WASM_ZERO));

  EXPECT_SIZE(4, WASM_STORE_GLOBAL(0, WASM_ZERO));

  EXPECT_SIZE(6, WASM_STORE_MEM(kMemI32, WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(4, WASM_IF(WASM_ZERO, WASM_NOP));

  EXPECT_SIZE(5, WASM_IF_THEN(WASM_ZERO, WASM_NOP, WASM_NOP));

  EXPECT_SIZE(1, WASM_NOP);

  EXPECT_SIZE(3, WASM_BLOCK(1, WASM_NOP));
  EXPECT_SIZE(4, WASM_BLOCK(2, WASM_NOP, WASM_NOP));
  EXPECT_SIZE(5, WASM_BLOCK(3, WASM_NOP, WASM_NOP, WASM_NOP));

  EXPECT_SIZE(2, WASM_INFINITE_LOOP);

  EXPECT_SIZE(3, WASM_LOOP(1, WASM_NOP));
  EXPECT_SIZE(4, WASM_LOOP(2, WASM_NOP, WASM_NOP));
  EXPECT_SIZE(5, WASM_LOOP(3, WASM_NOP, WASM_NOP, WASM_NOP));
  EXPECT_SIZE(4, WASM_LOOP(1, WASM_BREAK(0)));
  EXPECT_SIZE(4, WASM_LOOP(1, WASM_CONTINUE(0)));

  EXPECT_SIZE(1, WASM_RETURN0);

  EXPECT_SIZE(3, WASM_RETURN(WASM_ZERO));
  EXPECT_SIZE(5, WASM_RETURN(WASM_ZERO, WASM_ONE));
  EXPECT_SIZE(7, WASM_RETURN(WASM_ZERO, WASM_ONE, WASM_ZERO));

  EXPECT_SIZE(4, WASM_SWITCH(0, WASM_ZERO));
  EXPECT_SIZE(5, WASM_SWITCH(1, WASM_ZERO, WASM_NOP));
  EXPECT_SIZE(6, WASM_SWITCH(2, WASM_ZERO, WASM_NOP, WASM_NOP));

  EXPECT_SIZE(4, WASM_SWITCH_NF(0, WASM_ZERO));
  EXPECT_SIZE(5, WASM_SWITCH_NF(1, WASM_ZERO, WASM_NOP));
  EXPECT_SIZE(6, WASM_SWITCH_NF(2, WASM_ZERO, WASM_NOP, WASM_NOP));
}


TEST_F(WasmMacroGenTest, MacroStatements) {
  EXPECT_SIZE(8, WASM_WHILE(WASM_I8(0), WASM_NOP));
  EXPECT_SIZE(7, WASM_INC_LOCAL(0));
  EXPECT_SIZE(7, WASM_INC_LOCAL_BY(0, 3));
}


TEST_F(WasmMacroGenTest, Expressions) {
  EXPECT_SIZE(2, WASM_GET_LOCAL(0));
  EXPECT_SIZE(2, WASM_GET_LOCAL(1));
  EXPECT_SIZE(2, WASM_GET_LOCAL(12));
  EXPECT_SIZE(2, WASM_LOAD_GLOBAL(0));
  EXPECT_SIZE(2, WASM_LOAD_GLOBAL(1));
  EXPECT_SIZE(2, WASM_LOAD_GLOBAL(12));
  EXPECT_SIZE(4, WASM_LOAD_MEM(kMemI32, WASM_ZERO));
  EXPECT_SIZE(4, WASM_LOAD_MEM(kMemF64, WASM_ZERO));
  EXPECT_SIZE(4, WASM_LOAD_MEM(kMemF32, WASM_ZERO));

  EXPECT_SIZE(7, WASM_EXPR_IF(WASM_ZERO, WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_COMMA(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(3, WASM_NOT(WASM_ZERO));

  EXPECT_SIZE(7, WASM_EXPR_IF(WASM_ZERO, WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(4, WASM_EXPR_BREAK(1, WASM_ZERO));

  EXPECT_SIZE(4, WASM_EXPR_BLOCK(1, WASM_ZERO));
  EXPECT_SIZE(5, WASM_EXPR_BLOCK(2, WASM_NOP, WASM_ZERO));
  EXPECT_SIZE(6, WASM_EXPR_BLOCK(3, WASM_NOP, WASM_NOP, WASM_ZERO));

  EXPECT_SIZE(4, WASM_EXPR_LOOP(1, WASM_ZERO));
  EXPECT_SIZE(5, WASM_EXPR_LOOP(2, WASM_NOP, WASM_ZERO));
  EXPECT_SIZE(6, WASM_EXPR_LOOP(3, WASM_NOP, WASM_NOP, WASM_ZERO));
}


TEST_F(WasmMacroGenTest, FunctionCalls) {
  EXPECT_SIZE(2, WASM_CALL_FUNCTION0(0));
  EXPECT_SIZE(2, WASM_CALL_FUNCTION0(1));
  EXPECT_SIZE(2, WASM_CALL_FUNCTION0(11));

  EXPECT_SIZE(4, WASM_CALL_FUNCTION(0, WASM_ZERO));
  EXPECT_SIZE(6, WASM_CALL_FUNCTION(1, WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(4, WASM_CALL_INDIRECT0(0, WASM_ZERO));
  EXPECT_SIZE(4, WASM_CALL_INDIRECT0(1, WASM_ZERO));
  EXPECT_SIZE(4, WASM_CALL_INDIRECT0(11, WASM_ZERO));

  EXPECT_SIZE(6, WASM_CALL_INDIRECT(0, WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(8, WASM_CALL_INDIRECT(1, WASM_ZERO, WASM_ZERO, WASM_ZERO));
}


TEST_F(WasmMacroGenTest, Int32Ops) {
  EXPECT_SIZE(5, WASM_I32_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_DIVS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_DIVU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_REMS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_REMU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_AND(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_IOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_XOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_SHL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_SHR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_SAR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_EQ(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_I32_LTS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_LES(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_LTU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_LEU(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_I32_GTS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_GES(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_GTU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I32_GEU(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_I32_CLZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I32_CTZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I32_POPCNT(WASM_ZERO));
}


TEST_F(WasmMacroGenTest, Int64Ops) {
  EXPECT_SIZE(5, WASM_I64_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_DIVS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_DIVU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_REMS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_REMU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_AND(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_IOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_XOR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_SHL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_SHR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_SAR(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_EQ(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_I64_LTS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_LES(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_LTU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_LEU(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(5, WASM_I64_GTS(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_GES(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_GTU(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_I64_GEU(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_I64_CLZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_CTZ(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_POPCNT(WASM_ZERO));
}


TEST_F(WasmMacroGenTest, Float32Ops) {
  EXPECT_SIZE(5, WASM_F32_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_DIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_MIN(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_MAX(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_F32_ABS(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_NEG(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_COPYSIGN(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_CEIL(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_FLOOR(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_TRUNC(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_NEARESTINT(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_SQRT(WASM_ZERO));

  EXPECT_SIZE(5, WASM_F32_EQ(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_LT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_LE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_GT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F32_GE(WASM_ZERO, WASM_ZERO));
}


TEST_F(WasmMacroGenTest, Float64Ops) {
  EXPECT_SIZE(5, WASM_F64_ADD(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_SUB(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_MUL(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_DIV(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_MIN(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_MAX(WASM_ZERO, WASM_ZERO));

  EXPECT_SIZE(3, WASM_F64_ABS(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_NEG(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_COPYSIGN(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_CEIL(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_FLOOR(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_TRUNC(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_NEARESTINT(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_SQRT(WASM_ZERO));

  EXPECT_SIZE(5, WASM_F64_EQ(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_LT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_LE(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_GT(WASM_ZERO, WASM_ZERO));
  EXPECT_SIZE(5, WASM_F64_GE(WASM_ZERO, WASM_ZERO));
}


TEST_F(WasmMacroGenTest, Conversions) {
  EXPECT_SIZE(3, WASM_I32_SCONVERT_F32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I32_SCONVERT_F64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I32_UCONVERT_F32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I32_UCONVERT_F64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I32_CONVERT_I64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_SCONVERT_F32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_SCONVERT_F64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_UCONVERT_F32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_UCONVERT_F64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_SCONVERT_I32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_I64_UCONVERT_I32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_SCONVERT_I32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_UCONVERT_I32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_SCONVERT_I64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_UCONVERT_I64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_CONVERT_F64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F32_REINTERPRET_I32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_SCONVERT_I32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_UCONVERT_I32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_SCONVERT_I64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_UCONVERT_I64(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_CONVERT_F32(WASM_ZERO));
  EXPECT_SIZE(3, WASM_F64_REINTERPRET_I64(WASM_ZERO));
}

static const MemType kMemTypes[] = {
    kMemI8,   kMemU8, kMemI16,  kMemU16,  kMemI32,
    kMemU32, kMemI64, kMemU64, kMemF32, kMemF64};

TEST_F(WasmMacroGenTest, LoadsAndStores) {
  for (size_t i = 0; i < arraysize(kMemTypes); i++) {
    EXPECT_SIZE(4, WASM_LOAD_MEM(kMemTypes[i], WASM_ZERO));
  }
  for (size_t i = 0; i < arraysize(kMemTypes); i++) {
    EXPECT_SIZE(6, WASM_STORE_MEM(kMemTypes[i], WASM_ZERO, WASM_GET_LOCAL(0)));
  }
}
}
}
}
