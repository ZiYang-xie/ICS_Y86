//
// Created by 谢子飏 on 2020/12/01.
//

#include <gtest/gtest.h>

#include "../src/Controller.h"
#include "../src/Device.h"
#include "test_util.h"

enum {
    RAX = 0,
    RCX = 1,
    RDX = 2,
    RBX = 3,
    RSP = 4,
    RBP = 5,
    RSI = 6,
    RDI = 7,
    R8 = 8,
    R9 = 9,
    R10 = 0xa,
    R11 = 0xb,
    R12 = 0xc,
    R13 = 0xd,
    R14 = 0xe,
    R15 = 0xf,
    ZF = 0,
    SF = 1,
    OF = 2
};

class FlashCodeTest : public testing::Test {
   protected:
    void SetUp() override {
        c.FlashCode("112233eeff000ff0");
        c.Run();
    }
    Controller c;
};

TEST_F(FlashCodeTest, test_FlashCode) {
    c.Run();
    EXPECT_EQ(c.d.Mem[0], 17);
    EXPECT_EQ(c.d.Mem[0], 17);
    EXPECT_EQ(c.d.Mem[1], 34);
    EXPECT_EQ(c.d.Mem[2], 51);
    EXPECT_EQ(c.d.Mem[3], 0xe * 17);
    EXPECT_EQ(c.d.Mem[4], 0xf * 17);
    EXPECT_EQ(c.d.Mem[5], 0);
    EXPECT_EQ(c.d.Mem[6], 0xf);
    EXPECT_EQ(c.d.Mem[7], 0xf * 16);
}

class InstrTest : public testing::Test {
   protected:
    Controller c;
};

TEST_F(InstrTest, test_halt) {
    c.FlashCode("00");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.GetPC(), 1);
}

TEST_F(InstrTest, test_nop) {
    c.FlashCode("101000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.GetPC(), 3);
}

TEST_F(InstrTest, test_rrmov) {
    // rrmovq
    c.FlashCode("203000");
    c.d.Reg[RBX] = 5;
    c.Run();

    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.GetPC(), 3);
    EXPECT_EQ(c.d.Reg[RAX], 5);

    // c.FlashCode("20f000") 异常处理
}

TEST_F(InstrTest, test_CmovLe) {
    // (SF ^ OF) | ZF
    c.FlashCode("213000");
    c.d.CFLAG[ZF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[ZF] = false;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);
}

TEST_F(InstrTest, test_CmovL) {
    // SF ^ OF
    c.FlashCode("223000");
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);
}

TEST_F(InstrTest, test_CmovE) {
    // ZF
    c.FlashCode("233000");
    c.Reset();
    c.d.CFLAG[ZF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[ZF] = false;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);
}

TEST_F(InstrTest, test_CmovNe) {
    // ~ZF
    c.FlashCode("243000");
    c.Reset();
    c.d.CFLAG[ZF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[ZF] = false;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);
}

TEST_F(InstrTest, test_CmovGe) {
    // ~(SF ^ OF)
    c.FlashCode("253000");
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);
}

TEST_F(InstrTest, test_CmovG) {
    // ~(SF ^ OF) & ~ZF
    c.FlashCode("263000");
    c.d.CFLAG[ZF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[ZF] = 0;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 0);

    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.CFLAG[SF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);

    c.Reset();
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.Reg[RAX], 5);
}

// ---------- End rrmovq Test ---------- //

TEST_F(InstrTest, test_irmovq) {
    c.FlashCode("30f4040000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Reg[RSP], 4);
    c.FlashCode("30f5ffffffffffffffff00");
    c.Reset();
    c.Run();
    EXPECT_EQ(c.d.Reg[RBP], 0xffffffffffffffff);
}

TEST_F(InstrTest, test_rmmovq) {
    c.FlashCode("40120001000000000000");
    c.d.Reg[1] = 0xdeadbeef;
    c.Run();
    EXPECT_EQ(c.d.Read8Bytes(0x100), 0xdeadbeef);

    c.FlashCode("40120001000000000000");
    c.d.Reg[1] = 0xffffffffffffffff;
    c.Run();
    EXPECT_EQ(c.d.Read8Bytes(0x100), 0xffffffffffffffff);

    c.FlashCode("40120001000000000000");
    c.d.Reg[1] = 0xdeadbeef;
    c.d.Reg[2] = 0x10;
    c.Run();
    EXPECT_EQ(c.d.Read8Bytes(0x110), 0xdeadbeef);
}

TEST_F(InstrTest, test_mrmovq) {
    c.FlashCode("50120001000000000000");
    c.d.Write8Bytes(0x100, 0xdeadbeef);
    c.Run();
    EXPECT_EQ(c.d.Reg[1], 0xdeadbeef);
    c.FlashCode("50120001000000000000");
    c.Reset();
    c.d.Write8Bytes(0x100, 0xffffffffffffffff);
    c.Run();
    EXPECT_EQ(c.d.Reg[1], 0xffffffffffffffff);
    c.FlashCode("50120001000000000000");
    c.Reset();
    c.d.Write8Bytes(0x110, 0xdeadbeef);
    c.d.Reg[2] = 0x10;
    c.Run();
    EXPECT_EQ(c.d.Reg[1], 0xdeadbeef);
}

TEST_F(InstrTest, test_OPq) {
    c.FlashCode("6012");
    c.Reset();
    c.d.Reg[RCX] = 0x10;
    c.d.Reg[RDX] = 0x03;
    c.Run();
    EXPECT_EQ(c.d.Reg[RDX], 0x13);
    EXPECT_EQ(c.d.CFLAG[ZF], 0);
    EXPECT_EQ(c.d.CFLAG[SF], 0);
    EXPECT_EQ(c.d.CFLAG[OF], 0);
    c.FlashCode("6112");
    c.Reset();
    c.d.Reg[RCX] = 0x10;
    c.d.Reg[RDX] = 0x03;
    c.Run();
    EXPECT_EQ(c.d.Reg[RDX], (0x3 - 0x10));
    EXPECT_EQ(c.d.CFLAG[ZF], 0);
    EXPECT_EQ(c.d.CFLAG[SF], 1);
    EXPECT_EQ(c.d.CFLAG[OF], 0);
    c.FlashCode("6212");
    c.Reset();
    c.d.Reg[RCX] = 0xf;
    c.d.Reg[RDX] = 0x7;
    c.Run();
    EXPECT_EQ(c.d.Reg[RDX], (0x7));
    EXPECT_EQ(c.d.CFLAG[ZF], 0);
    EXPECT_EQ(c.d.CFLAG[SF], 0);
    EXPECT_EQ(c.d.CFLAG[OF], 0);
    c.FlashCode("6312");
    c.Reset();
    c.d.Reg[RCX] = 0xf;
    c.d.Reg[RDX] = 0x7;
    c.Run();
    EXPECT_EQ(c.d.Reg[2], (0x8));
    EXPECT_EQ(c.d.CFLAG[ZF], 0);
    EXPECT_EQ(c.d.CFLAG[SF], 0);
    EXPECT_EQ(c.d.CFLAG[OF], 0);
    c.FlashCode("6012");
    c.Reset();
    c.d.Reg[RCX] = 0x0;
    c.d.Reg[RDX] = 0x0;
    c.Run();
    EXPECT_EQ(c.d.CFLAG[ZF], 1);
    EXPECT_EQ(c.d.CFLAG[SF], 0);
    EXPECT_EQ(c.d.CFLAG[OF], 0);
    c.Reset();
    c.d.Reg[RCX] = -1;
    c.d.Reg[RDX] = 0;
    c.Run();
    EXPECT_EQ(c.d.CFLAG[ZF], 0);
    EXPECT_EQ(c.d.CFLAG[SF], 1);
    EXPECT_EQ(c.d.CFLAG[OF], 0);
    c.Reset();
    c.d.Reg[RCX] = 0x7fffffffffffffff;
    c.d.Reg[RDX] = 0x7fffffffffffffff;
    c.Run();
    EXPECT_EQ(c.d.CFLAG[ZF], 0);
    EXPECT_EQ(c.d.CFLAG[SF], 1);
    EXPECT_EQ(c.d.CFLAG[OF], 1);
    c.FlashCode("6112");
    c.Reset();
    c.d.Reg[RCX] = 0x8000000000000000;
    c.d.Reg[RDX] = 0x7fffffffffffffff;
    c.Run();
    EXPECT_EQ(c.d.CFLAG[ZF], 0);
    EXPECT_EQ(c.d.CFLAG[SF], 1);
    EXPECT_EQ(c.d.CFLAG[OF], 1);
}

TEST_F(InstrTest, test_JXX) {
    c.FlashCode("70" + int2str(0x100));
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    c.Reset();
    c.FlashCode("71" + int2str(0x100));
    c.d.CFLAG[ZF] = true;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    c.Reset();
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 10);
    c.Reset();
    c.d.CFLAG[SF] = true;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    c.Reset();
    c.d.CFLAG[OF] = true;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.CFLAG[SF] = true;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 10);
    c.FlashCode("72" + int2str(0x100));
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 10);
    c.Reset();
    c.d.CFLAG[ZF] = 0;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 10);
    c.Reset();
    c.d.CFLAG[SF] = true;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    c.Reset();
    c.d.CFLAG[OF] = true;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    c.Reset();
    c.d.CFLAG[OF] = true;
    c.d.CFLAG[SF] = true;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 10);
    c.FlashCode("73" + int2str(0x100));
    c.Reset();
    c.d.CFLAG[ZF] = true;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    c.Reset();
    c.d.CFLAG[ZF] = 0;
    c.d.Reg[RBX] = 5;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 10);
}

TEST_F(InstrTest, test_call) {
    c.FlashCode("80" + int2str(0x100));
    c.d.Reg[RSP] = 0x150;
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x101);
    EXPECT_EQ(c.d.Reg[RSP], 0x148);
    EXPECT_EQ(c.d.Read8Bytes(0x148), 0x9);
    c.Reset();
    c.FlashCode("803800000000000000");
    c.d.Reg[RSP] = 0x200;
    c.Run();
    EXPECT_EQ(c.d.Reg[RSP], 0x1f8);
    EXPECT_EQ(c.d.GetPC(), 0x39);
}

TEST_F(InstrTest, test_ret) {
    c.FlashCode("9000101010");
    c.d.Reg[RSP] = 0x148;
    c.d.Write8Bytes(0x148, 0x3);
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 6);
    EXPECT_EQ(c.d.Reg[RSP], 0x150);
    c.Reset();
    c.d.Reg[RSP] = 0x148;
    c.d.Write8Bytes(0x148, 0x110);
    c.Run();
    EXPECT_EQ(c.d.GetPC(), 0x111);
    EXPECT_EQ(c.d.Reg[RSP], 0x150);
}

TEST_F(InstrTest, test_pushq) {
    c.FlashCode("a010");
    c.d.Reg[1] = 0xdeadbeef;
    c.d.Reg[RSP] = 0x150;
    c.Run();
    EXPECT_EQ(c.d.Reg[RSP], 0x148);
    EXPECT_EQ(c.d.Read8Bytes(0x148), 0xdeadbeef);
}

TEST_F(InstrTest, test_popq) {
    c.FlashCode("b010");
    c.d.Reg[RSP] = 0x148;
    c.d.Write8Bytes(0x148, 0xdeadbeef);
    c.Run();
    EXPECT_EQ(c.d.Reg[RSP], 0x150);
    EXPECT_EQ(c.d.Reg[1], 0xdeadbeef);
}

class RealCodeTest : public testing::Test {
   protected:
    Controller c;
};

TEST_F(RealCodeTest, test_prog2) {
    c.FlashCode("30f20a0000000000000030f003000000000000001010602000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x000000000000000d);
    EXPECT_EQ(c.d.Reg[RDX], 0x000000000000000a);
}

TEST_F(RealCodeTest, test_pushquestion) {
    c.FlashCode("30f40001000000000000a04fb00f00");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000100);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000100);
}

TEST_F(RealCodeTest, test_asumi) {
    c.FlashCode(
        "30f4000100000000000080380000000000000000000000000d000d000d000000c000c0"
        "00c0000000000b000b000b000000a000a000a0000030f7180000000000000030f60400"
        "000000000000805600000000000000906300626670830000000000000050a700000000"
        "0000000060a0c0f70800000000000000c0f6ffffffffffffffff746300000000000000"
        "9000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SINS);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000d000d000d);
    EXPECT_EQ(c.d.Reg[RSP], 0x00000000000000f0);
    EXPECT_EQ(c.d.Reg[RSI], 0x0000000000000004);
    EXPECT_EQ(c.d.Reg[R10], 0x0000000d000d000d);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000d000d000d);
    EXPECT_EQ(c.d.Reg[RSP], 0x00000000000000f0);
    EXPECT_EQ(c.d.Reg[RSI], 0x0000000000000004);
    EXPECT_EQ(c.d.Reg[R10], 0x0000000d000d000d);
}

TEST_F(RealCodeTest, test_prog6) {
    c.FlashCode(
        "30f4300000000000000080200000000000000030f20a00000000000000000000902023"
        "00000000000000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RDX], 0x000000000000000a);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000030);
}

TEST_F(RealCodeTest, test_prog7) {
    c.FlashCode(
        "630074160000000000000030f001000000000000000030f2020000000000000030f303"
        "0000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000001);
}

TEST_F(RealCodeTest, test_prog3) {
    c.FlashCode("30f20a0000000000000030f0030000000000000010602000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x000000000000000d);
    EXPECT_EQ(c.d.Reg[RDX], 0x000000000000000a);
}

TEST_F(RealCodeTest, test_prog8) {
    c.FlashCode("30f20a0000000000000030f20300000000000000202000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000003);
    EXPECT_EQ(c.d.Reg[RDX], 0x0000000000000003);
}

TEST_F(RealCodeTest, test_pushtest) {
    c.FlashCode("30f400010000000000002040a04fb02f612000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RDX], 0x0000000000000100);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000100);
}

TEST_F(RealCodeTest, test_prog10) {
    c.FlashCode(
        "30f001000000000000006344a00f600030f0020000000000000030f003000000000000"
        "00");
    c.Run();
    EXPECT_EQ(c.d.Stat, SADR);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000001);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000000);
}

TEST_F(RealCodeTest, test_prog9) {
    c.FlashCode("630074160000000000000030f0010000000000000000ff");
    c.Run(true);
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000001);
}

TEST_F(RealCodeTest, test_abs_asum_cmov) {
    c.FlashCode(
        "30f4000200000000000080380000000000000000000000000d000d000d00000040ff3f"
        "ff3fffffff000b000b000b00000060ff5fff5fffff30f7180000000000000030f60400"
        "0000000000008056000000000000009030f8080000000000000030f901000000000000"
        "0063006266708d0000000000000050a7000000000000000063bb61ab26ba60a0608761"
        "9674770000000000000090000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000abcdabcdabcd);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000200);
    EXPECT_EQ(c.d.Reg[R8], 0x0000000000000008);
    EXPECT_EQ(c.d.Reg[R9], 0x0000000000000001);
    EXPECT_EQ(c.d.Reg[R10], 0x0000a000a000a000);
    EXPECT_EQ(c.d.Reg[R11], 0x0000a000a000a000);
}

TEST_F(RealCodeTest, test_ret_hazard) {
    c.FlashCode(
        "30f3400000000000000050430000000000000000900030f60500000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000500000000000"
        "000000000000000000001600000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RBX], 0x0000000000000040);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000058);
    EXPECT_EQ(c.d.Reg[RSI], 0x0000000000000005);
}

TEST_F(RealCodeTest, test_abs_asum_jmp) {
    c.FlashCode(
        "30f4000200000000000080380000000000000000000000000d000d000d00000040ff3f"
        "ff3fffffff000b000b000b00000060ff5fff5fffff30f7180000000000000030f60400"
        "0000000000008056000000000000009030f8080000000000000030f901000000000000"
        "006300626670960000000000000050a7000000000000000063bb61ab71900000000000"
        "000020ba60a06087619674770000000000000090000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000abcdabcdabcd);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000200);
    EXPECT_EQ(c.d.Reg[R8], 0x0000000000000008);
    EXPECT_EQ(c.d.Reg[R9], 0x0000000000000001);
    EXPECT_EQ(c.d.Reg[R10], 0x0000a000a000a000);
    EXPECT_EQ(c.d.Reg[R11], 0x0000a000a000a000);
}

TEST_F(RealCodeTest, test_j_cc) {
    c.FlashCode(
        "30f6010000000000000030f7020000000000000030f5040000000000000030f0e0ffff"
        "ffffffffff30f240000000000000006120733f000000000000001000606210101000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 1);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0xffffffffffffffa0);
    EXPECT_EQ(c.d.Reg[RDX], 0x0000000000000040);
    EXPECT_EQ(c.d.Reg[RBP], 0x0000000000000004);
    EXPECT_EQ(c.d.Reg[RSI], 0x0000000000000001);
}

TEST_F(RealCodeTest, test_poptest) {
    c.FlashCode("30f4000100000000000030f0cdab000000000000a00fb04f00");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x000000000000abcd);
    EXPECT_EQ(c.d.Reg[RSP], 0x000000000000abcd);
}

TEST_F(RealCodeTest, test_prog4) {
    c.FlashCode("30f20a0000000000000030f00300000000000000602000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x000000000000000d);
    EXPECT_EQ(c.d.Reg[RDX], 0x000000000000000a);
}

TEST_F(RealCodeTest, test_cjr) {
    c.FlashCode(
        "30f4400000000000000030f03800000000000000a00f6300742c0000000000000030f0"
        "0100000000000000009030f302000000000000000030f2030000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000001);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000038);
}

TEST_F(RealCodeTest, test_prog1) {
    c.FlashCode("30f20a0000000000000030f00300000000000000101010602000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x000000000000000d);
    EXPECT_EQ(c.d.Reg[RDX], 0x000000000000000a);
}

TEST_F(RealCodeTest, test_asum) {
    c.FlashCode(
        "30f4000200000000000080380000000000000000000000000d000d000d000000c000c0"
        "00c0000000000b000b000b000000a000a000a0000030f7180000000000000030f60400"
        "0000000000008056000000000000009030f8080000000000000030f901000000000000"
        "006300626670870000000000000050a7000000000000000060a0608761967477000000"
        "0000000090000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000abcdabcdabcd);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000200);
    EXPECT_EQ(c.d.Reg[R8], 0x0000000000000008);
    EXPECT_EQ(c.d.Reg[R9], 0x0000000000000001);
    EXPECT_EQ(c.d.Reg[R10], 0x0000a000a000a000);
}

TEST_F(RealCodeTest, test_prog5) {
    c.FlashCode(
        "30f2800000000000000030f103000000000000004012000000000000000030f30a0000"
        "000000000050020000000000000000603000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x000000000000000d);
    EXPECT_EQ(c.d.Reg[RCX], 0x0000000000000003);
    EXPECT_EQ(c.d.Reg[RDX], 0x0000000000000080);
    EXPECT_EQ(c.d.Reg[RBX], 0x000000000000000a);
}

TEST_F(RealCodeTest, test_asumr) {
    c.FlashCode(
        "30f4000200000000000080380000000000000000000000000d000d000d000000c000c0"
        "00c0000000000b000b000b000000a000a000a0000030f7180000000000000030f60400"
        "0000000000008056000000000000009063006266719400000000000000a03f50370000"
        "00000000000030faffffffffffffffff60a630fa080000000000000060a78056000000"
        "000000006030b03f900000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000abcdabcdabcd);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000200);
    EXPECT_EQ(c.d.Reg[R10], 0x0000000000000008);
}

class HonorCodeTest : public testing::Test {
   protected:
    Controller c;
};

TEST_F(HonorCodeTest, test_INS2) {
    c.FlashCode("2732");
    c.Run();
    EXPECT_EQ(c.d.Stat, SINS);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
}

TEST_F(HonorCodeTest, test_ex32) {
    c.FlashCode(
        "30f2050000000000000030f4000100000000000040240000000000000000b04f2040");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000005);
    EXPECT_EQ(c.d.Reg[RDX], 0x0000000000000005);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000005);
}

TEST_F(HonorCodeTest, test_INS3) {
    c.FlashCode("a002");
    c.Run();
    EXPECT_EQ(c.d.Stat, SINS);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000000);
}

TEST_F(HonorCodeTest, test_ex33) {
    c.FlashCode("30f0230100000000000030f2210300000000000063112402602200");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000000123);
    EXPECT_EQ(c.d.Reg[RDX], 0x0000000000000642);
}

TEST_F(HonorCodeTest, test_load_forward) {
    c.FlashCode(
        "30f4000200000000000080380000000000000000000000000001000000000000000200"
        "00000000000003000000000000000400000000000030f1180000000000000030f03412"
        "00000000000050210000000000000000a02f10b02f4002000000000000000090000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RAX], 0x0000000000001234);
    EXPECT_EQ(c.d.Reg[RCX], 0x0000000000000018);
    EXPECT_EQ(c.d.Reg[RDX], 0x0000000000000100);
    EXPECT_EQ(c.d.Reg[RSP], 0x0000000000000200);
}

TEST_F(HonorCodeTest, test_overflow1) {
    c.FlashCode(
        "30f7000000000000008030f6ffffffffffffffff606775000000000000000000");
    c.Run(true);
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 1);
    EXPECT_EQ(c.d.Reg[RSI], 0xffffffffffffffff);
}

TEST_F(HonorCodeTest, test_INS4) {
    c.FlashCode("200f");
    c.Run();
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
}

TEST_F(HonorCodeTest, test_overflow) {
    c.FlashCode(
        "30f7ffffffffffffff7f30f6ffffffffffffffff616771000000000000000000");
    c.Run(true);
    EXPECT_EQ(c.d.Stat, SHLT);
    EXPECT_EQ(c.d.CFLAG[0], 0);
    EXPECT_EQ(c.d.CFLAG[1], 1);
    EXPECT_EQ(c.d.CFLAG[2], 1);
    EXPECT_EQ(c.d.Reg[RSI], 0xffffffffffffffff);
}

TEST_F(HonorCodeTest, test_INS1) {
    c.FlashCode("30f2");
    c.Run();
    EXPECT_EQ(c.d.Stat, SINS);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
}

TEST_F(HonorCodeTest, test_memtest) {
    c.FlashCode(
        "30f6010000000000000030f7000000000000000040670000000000000000101010");
    c.Run();
    EXPECT_EQ(c.d.Stat, SADR);
    EXPECT_EQ(c.d.CFLAG[0], 1);
    EXPECT_EQ(c.d.CFLAG[1], 0);
    EXPECT_EQ(c.d.CFLAG[2], 0);
    EXPECT_EQ(c.d.Reg[RSI], 0x0000000000000001);
}
