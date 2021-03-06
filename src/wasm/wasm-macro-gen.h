// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_WASM_MACRO_GEN_H_
#define V8_WASM_MACRO_GEN_H_

#include "src/wasm/wasm-opcodes.h"

// Convenience macros for building Wasm bytecode directly into a byte array.

//------------------------------------------------------------------------------
// Statements.
//------------------------------------------------------------------------------
#define WASM_IF(cond, tstmt) kStmtIf, cond, tstmt
#define WASM_IF_THEN(cond, tstmt, fstmt) kStmtIfThen, cond, tstmt, fstmt
#define WASM_NOP kStmtNop
#define WASM_BLOCK(count, ...) kStmtBlock, static_cast<byte>(count), __VA_ARGS__
#define WASM_INFINITE_LOOP kStmtLoop, 0
#define WASM_LOOP(count, ...) kStmtLoop, static_cast<byte>(count), __VA_ARGS__
#define WASM_SWITCH(count, key, ...) \
  kStmtSwitch, static_cast<byte>(count), key, __VA_ARGS__
#define WASM_SWITCH_NF(count, key, ...) \
  kStmtSwitchNf, static_cast<byte>(count), key, __VA_ARGS__
#define WASM_CONTINUE(depth) kStmtContinue, static_cast<byte>(depth)
#define WASM_BREAK(depth) kStmtBreak, static_cast<byte>(depth)
#define WASM_RETURN0 kStmtReturn
#define WASM_RETURN(...) kStmtReturn, __VA_ARGS__

//------------------------------------------------------------------------------
// Misc expressions.
//------------------------------------------------------------------------------
#define WASM_ID(...) __VA_ARGS__
#define WASM_ZERO kExprI8Const, 0
#define WASM_ONE kExprI8Const, 1
#define WASM_I8(val) kExprI8Const, static_cast<byte>(val)
#define WASM_I32(val)                                                 \
  kExprI32Const, static_cast<byte>(val), static_cast<byte>(val >> 8), \
      static_cast<byte>(val >> 16), static_cast<byte>(val >> 24)
#define WASM_I64(val)                                           \
  kExprI64Const, static_cast<byte>(static_cast<uint64_t>(val)), \
      static_cast<byte>(static_cast<uint64_t>(val) >> 8),       \
      static_cast<byte>(static_cast<uint64_t>(val) >> 16),      \
      static_cast<byte>(static_cast<uint64_t>(val) >> 24),      \
      static_cast<byte>(static_cast<uint64_t>(val) >> 32),      \
      static_cast<byte>(static_cast<uint64_t>(val) >> 40),      \
      static_cast<byte>(static_cast<uint64_t>(val) >> 48),      \
      static_cast<byte>(static_cast<uint64_t>(val) >> 56)
#define WASM_F32(val)                                                       \
  kExprF32Const,                                                            \
      static_cast<byte>(bit_cast<int32_t>(static_cast<float>(val))),        \
      static_cast<byte>(bit_cast<uint32_t>(static_cast<float>(val)) >> 8),  \
      static_cast<byte>(bit_cast<uint32_t>(static_cast<float>(val)) >> 16), \
      static_cast<byte>(bit_cast<uint32_t>(static_cast<float>(val)) >> 24)
#define WASM_F64(val)                                        \
  kExprF64Const, static_cast<byte>(bit_cast<uint64_t>(val)), \
      static_cast<byte>(bit_cast<uint64_t>(val) >> 8),       \
      static_cast<byte>(bit_cast<uint64_t>(val) >> 16),      \
      static_cast<byte>(bit_cast<uint64_t>(val) >> 24),      \
      static_cast<byte>(bit_cast<uint64_t>(val) >> 32),      \
      static_cast<byte>(bit_cast<uint64_t>(val) >> 40),      \
      static_cast<byte>(bit_cast<uint64_t>(val) >> 48),      \
      static_cast<byte>(bit_cast<uint64_t>(val) >> 56)
#define WASM_GET_LOCAL(index) kExprGetLocal, static_cast<byte>(index)
#define WASM_SET_LOCAL(index, val) kExprSetLocal, static_cast<byte>(index), val
#define WASM_LOAD_GLOBAL(index) kExprLoadGlobal, static_cast<byte>(index)
#define WASM_STORE_GLOBAL(index, val) \
  kExprStoreGlobal, static_cast<byte>(index), val
#define WASM_LOAD_MEM(type, index)                                 \
  v8::internal::wasm::WasmOpcodes::LoadStoreOpcodeOf(type, false), \
      v8::internal::wasm::WasmOpcodes::LoadStoreAccessOf(type), index
#define WASM_STORE_MEM(type, index, val)                          \
  v8::internal::wasm::WasmOpcodes::LoadStoreOpcodeOf(type, true), \
      v8::internal::wasm::WasmOpcodes::LoadStoreAccessOf(type), index, val
#define WASM_CALL_FUNCTION(index, ...) \
  kExprCallFunction, static_cast<byte>(index), __VA_ARGS__
#define WASM_CALL_INDIRECT(index, func, ...) \
  kExprCallIndirect, static_cast<byte>(index), func, __VA_ARGS__
#define WASM_CALL_FUNCTION0(index) kExprCallFunction, static_cast<byte>(index)
#define WASM_CALL_INDIRECT0(index, func) \
  kExprCallIndirect, static_cast<byte>(index), func
#define WASM_EXPR_IF(cond, tval, fval) kExprIf, cond, tval, fval
#define WASM_COMMA(left, right) kExprComma, left, right
#define WASM_NOT(x) kExprBoolNot, x
#define WASM_EXPR_BLOCK(count, ...) \
  kExprBlock, static_cast<byte>(count), __VA_ARGS__
#define WASM_EXPR_LOOP(count, ...) \
  kExprLoop, static_cast<byte>(count), __VA_ARGS__
#define WASM_EXPR_BREAK(depth, x) kExprBreak, static_cast<byte>(depth), x

//------------------------------------------------------------------------------
// Statements and expressions that are composed of multiple bytecodes.
//------------------------------------------------------------------------------
#define WASM_WHILE(x, y) kStmtLoop, 1, kStmtIfThen, x, y, kStmtBreak, 0
#define WASM_INC_LOCAL(index)                                          \
  kExprSetLocal, static_cast<byte>(index), kExprI32Add, kExprGetLocal, \
      static_cast<byte>(index), kExprI8Const, 1
#define WASM_INC_LOCAL_BY(index, count)                                \
  kExprSetLocal, static_cast<byte>(index), kExprI32Add, kExprGetLocal, \
      static_cast<byte>(index), kExprI8Const, static_cast<int8_t>(count)


#define WASM_UNOP(opcode, x) static_cast<byte>(opcode), x
#define WASM_BINOP(opcode, x, y) static_cast<byte>(opcode), x, y

//------------------------------------------------------------------------------
// Int32 operations
//------------------------------------------------------------------------------
#define WASM_I32_ADD(x, y) kExprI32Add, x, y
#define WASM_I32_SUB(x, y) kExprI32Sub, x, y
#define WASM_I32_MUL(x, y) kExprI32Mul, x, y
#define WASM_I32_DIVS(x, y) kExprI32DivS, x, y
#define WASM_I32_DIVU(x, y) kExprI32DivU, x, y
#define WASM_I32_REMS(x, y) kExprI32RemS, x, y
#define WASM_I32_REMU(x, y) kExprI32RemU, x, y
#define WASM_I32_AND(x, y) kExprI32And, x, y
#define WASM_I32_IOR(x, y) kExprI32Ior, x, y
#define WASM_I32_XOR(x, y) kExprI32Xor, x, y
#define WASM_I32_SHL(x, y) kExprI32Shl, x, y
#define WASM_I32_SHR(x, y) kExprI32ShrU, x, y
#define WASM_I32_SAR(x, y) kExprI32ShrS, x, y
#define WASM_I32_EQ(x, y) kExprI32Eq, x, y
#define WASM_I32_NE(x, y) kExprI32Ne, x, y
#define WASM_I32_LTS(x, y) kExprI32LtS, x, y
#define WASM_I32_LES(x, y) kExprI32LeS, x, y
#define WASM_I32_LTU(x, y) kExprI32LtU, x, y
#define WASM_I32_LEU(x, y) kExprI32LeU, x, y
#define WASM_I32_GTS(x, y) kExprI32GtS, x, y
#define WASM_I32_GES(x, y) kExprI32GeS, x, y
#define WASM_I32_GTU(x, y) kExprI32GtU, x, y
#define WASM_I32_GEU(x, y) kExprI32GeU, x, y
#define WASM_I32_CLZ(x) kExprI32Clz, x
#define WASM_I32_CTZ(x) kExprI32Ctz, x
#define WASM_I32_POPCNT(x) kExprI32PopCnt, x

//------------------------------------------------------------------------------
// Int64 operations
//------------------------------------------------------------------------------
#define WASM_I64_ADD(x, y) kExprI64Add, x, y
#define WASM_I64_SUB(x, y) kExprI64Sub, x, y
#define WASM_I64_MUL(x, y) kExprI64Mul, x, y
#define WASM_I64_DIVS(x, y) kExprI64DivS, x, y
#define WASM_I64_DIVU(x, y) kExprI64DivU, x, y
#define WASM_I64_REMS(x, y) kExprI64RemS, x, y
#define WASM_I64_REMU(x, y) kExprI64RemU, x, y
#define WASM_I64_AND(x, y) kExprI64And, x, y
#define WASM_I64_IOR(x, y) kExprI64Ior, x, y
#define WASM_I64_XOR(x, y) kExprI64Xor, x, y
#define WASM_I64_SHL(x, y) kExprI64Shl, x, y
#define WASM_I64_SHR(x, y) kExprI64ShrU, x, y
#define WASM_I64_SAR(x, y) kExprI64ShrS, x, y
#define WASM_I64_EQ(x, y) kExprI64Eq, x, y
#define WASM_I64_NE(x, y) kExprI64Ne, x, y
#define WASM_I64_LTS(x, y) kExprI64LtS, x, y
#define WASM_I64_LES(x, y) kExprI64LeS, x, y
#define WASM_I64_LTU(x, y) kExprI64LtU, x, y
#define WASM_I64_LEU(x, y) kExprI64LeU, x, y
#define WASM_I64_GTS(x, y) kExprI64GtS, x, y
#define WASM_I64_GES(x, y) kExprI64GeS, x, y
#define WASM_I64_GTU(x, y) kExprI64GtU, x, y
#define WASM_I64_GEU(x, y) kExprI64GeU, x, y
#define WASM_I64_CLZ(x) kExprI64Clz, x
#define WASM_I64_CTZ(x) kExprI64Ctz, x
#define WASM_I64_POPCNT(x) kExprI64PopCnt, x

//------------------------------------------------------------------------------
// Float32 operations
//------------------------------------------------------------------------------
#define WASM_F32_ADD(x, y) kExprF32Add, x, y
#define WASM_F32_SUB(x, y) kExprF32Sub, x, y
#define WASM_F32_MUL(x, y) kExprF32Mul, x, y
#define WASM_F32_DIV(x, y) kExprF32Div, x, y
#define WASM_F32_MIN(x, y) kExprF32Min, x, y
#define WASM_F32_MAX(x, y) kExprF32Max, x, y
#define WASM_F32_ABS(x) kExprF32Abs, x
#define WASM_F32_NEG(x) kExprF32Neg, x
#define WASM_F32_COPYSIGN(x) kExprF32CopySign, x
#define WASM_F32_CEIL(x) kExprF32Ceil, x
#define WASM_F32_FLOOR(x) kExprF32Floor, x
#define WASM_F32_TRUNC(x) kExprF32Trunc, x
#define WASM_F32_NEARESTINT(x) kExprF32NearestInt, x
#define WASM_F32_SQRT(x) kExprF32Sqrt, x
#define WASM_F32_EQ(x, y) kExprF32Eq, x, y
#define WASM_F32_NE(x, y) kExprF32Ne, x, y
#define WASM_F32_LT(x, y) kExprF32Lt, x, y
#define WASM_F32_LE(x, y) kExprF32Le, x, y
#define WASM_F32_GT(x, y) kExprF32Gt, x, y
#define WASM_F32_GE(x, y) kExprF32Ge, x, y


//------------------------------------------------------------------------------
// Float64 operations
//------------------------------------------------------------------------------
#define WASM_F64_ADD(x, y) kExprF64Add, x, y
#define WASM_F64_SUB(x, y) kExprF64Sub, x, y
#define WASM_F64_MUL(x, y) kExprF64Mul, x, y
#define WASM_F64_DIV(x, y) kExprF64Div, x, y
#define WASM_F64_MIN(x, y) kExprF64Min, x, y
#define WASM_F64_MAX(x, y) kExprF64Max, x, y
#define WASM_F64_ABS(x) kExprF64Abs, x
#define WASM_F64_NEG(x) kExprF64Neg, x
#define WASM_F64_COPYSIGN(x) kExprF64CopySign, x
#define WASM_F64_CEIL(x) kExprF64Ceil, x
#define WASM_F64_FLOOR(x) kExprF64Floor, x
#define WASM_F64_TRUNC(x) kExprF64Trunc, x
#define WASM_F64_NEARESTINT(x) kExprF64NearestInt, x
#define WASM_F64_SQRT(x) kExprF64Sqrt, x
#define WASM_F64_EQ(x, y) kExprF64Eq, x, y
#define WASM_F64_NE(x, y) kExprF64Ne, x, y
#define WASM_F64_LT(x, y) kExprF64Lt, x, y
#define WASM_F64_LE(x, y) kExprF64Le, x, y
#define WASM_F64_GT(x, y) kExprF64Gt, x, y
#define WASM_F64_GE(x, y) kExprF64Ge, x, y


//------------------------------------------------------------------------------
// Type conversions.
//------------------------------------------------------------------------------
#define WASM_I32_SCONVERT_F32(x) kExprI32SConvertF32, x
#define WASM_I32_SCONVERT_F64(x) kExprI32SConvertF64, x
#define WASM_I32_UCONVERT_F32(x) kExprI32UConvertF32, x
#define WASM_I32_UCONVERT_F64(x) kExprI32UConvertF64, x
#define WASM_I32_CONVERT_I64(x) kExprI32ConvertI64, x
#define WASM_I64_SCONVERT_F32(x) kExprI64SConvertF32, x
#define WASM_I64_SCONVERT_F64(x) kExprI64SConvertF64, x
#define WASM_I64_UCONVERT_F32(x) kExprI64UConvertF32, x
#define WASM_I64_UCONVERT_F64(x) kExprI64UConvertF64, x
#define WASM_I64_SCONVERT_I32(x) kExprI64SConvertI32, x
#define WASM_I64_UCONVERT_I32(x) kExprI64UConvertI32, x
#define WASM_F32_SCONVERT_I32(x) kExprF32SConvertI32, x
#define WASM_F32_UCONVERT_I32(x) kExprF32UConvertI32, x
#define WASM_F32_SCONVERT_I64(x) kExprF32SConvertI64, x
#define WASM_F32_UCONVERT_I64(x) kExprF32UConvertI64, x
#define WASM_F32_CONVERT_F64(x) kExprF32ConvertF64, x
#define WASM_F32_REINTERPRET_I32(x) kExprF32ReinterpretI32, x
#define WASM_F64_SCONVERT_I32(x) kExprF64SConvertI32, x
#define WASM_F64_UCONVERT_I32(x) kExprF64UConvertI32, x
#define WASM_F64_SCONVERT_I64(x) kExprF64SConvertI64, x
#define WASM_F64_UCONVERT_I64(x) kExprF64UConvertI64, x
#define WASM_F64_CONVERT_F32(x) kExprF64ConvertF32, x
#define WASM_F64_REINTERPRET_I64(x) kExprF64ReinterpretI64, x
#define WASM_I32_REINTERPRET_F32(x) kExprI32ReinterpretF32, x
#define WASM_I64_REINTERPRET_F64(x) kExprI64ReinterpretF64, x

#endif  // V8_WASM_MACRO_GEN_H_
