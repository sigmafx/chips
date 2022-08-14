#include "acutest.h"

#include "../chips/7400.h"
#include "../chips/7404.h"
#include "../chips/7408.h"
#include "../chips/7410.h"
#include "../chips/7432.h"
#include "../chips/7474.h"
#include "../chips/7486.h"
#include "../chips/7493.h"
#include "../chips/74157.h"

uint64_t pins;
// 7400
void test_7400_Gate1(void)
{
    TTL7400_RESET_A1(pins);
    TTL7400_RESET_B1(pins);
    TEST_CHECK(TTL7400_GET_Y1(pins));

    TTL7400_RESET_A1(pins);
    TTL7400_SET_B1(pins);
    TEST_CHECK(TTL7400_GET_Y1(pins));

    TTL7400_SET_A1(pins);
    TTL7400_RESET_B1(pins);
    TEST_CHECK(TTL7400_GET_Y1(pins));

    TTL7400_SET_A1(pins);
    TTL7400_SET_B1(pins);
    TEST_CHECK(!TTL7400_GET_Y1(pins));
}

void test_7400_Gate2(void)
{
    TTL7400_RESET_A2(pins);
    TTL7400_RESET_B2(pins);
    TEST_CHECK(TTL7400_GET_Y2(pins));

    TTL7400_RESET_A2(pins);
    TTL7400_SET_B2(pins);
    TEST_CHECK(TTL7400_GET_Y2(pins));

    TTL7400_SET_A2(pins);
    TTL7400_RESET_B2(pins);
    TEST_CHECK(TTL7400_GET_Y2(pins));

    TTL7400_SET_A2(pins);
    TTL7400_SET_B2(pins);
    TEST_CHECK(!TTL7400_GET_Y2(pins));
}

void test_7400_Gate3(void)
{
    TTL7400_RESET_A3(pins);
    TTL7400_RESET_B3(pins);
    TEST_CHECK(TTL7400_GET_Y3(pins));

    TTL7400_RESET_A3(pins);
    TTL7400_SET_B3(pins);
    TEST_CHECK(TTL7400_GET_Y3(pins));

    TTL7400_SET_A3(pins);
    TTL7400_RESET_B3(pins);
    TEST_CHECK(TTL7400_GET_Y3(pins));

    TTL7400_SET_A3(pins);
    TTL7400_SET_B3(pins);
    TEST_CHECK(!TTL7400_GET_Y3(pins));
}

void test_7400_Gate4(void)
{
    TTL7400_RESET_A4(pins);
    TTL7400_RESET_B4(pins);
    TEST_CHECK(TTL7400_GET_Y4(pins));

    TTL7400_RESET_A4(pins);
    TTL7400_SET_B4(pins);
    TEST_CHECK(TTL7400_GET_Y4(pins));

    TTL7400_SET_A4(pins);
    TTL7400_RESET_B4(pins);
    TEST_CHECK(TTL7400_GET_Y4(pins));

    TTL7400_SET_A4(pins);
    TTL7400_SET_B4(pins);
    TEST_CHECK(!TTL7400_GET_Y4(pins));
}

// 7404
void test_7404_AllGates_True(void)
{
    TTL7404_SET_A1(pins);
    TTL7404_SET_A2(pins);
    TTL7404_SET_A3(pins);
    TTL7404_SET_A4(pins);
    TTL7404_SET_A5(pins);
    TTL7404_SET_A6(pins);

    TEST_CHECK(!TTL7404_GET_Y1(pins));
    TEST_CHECK(!TTL7404_GET_Y2(pins));
    TEST_CHECK(!TTL7404_GET_Y3(pins));
    TEST_CHECK(!TTL7404_GET_Y4(pins));
    TEST_CHECK(!TTL7404_GET_Y5(pins));
    TEST_CHECK(!TTL7404_GET_Y6(pins));
}

void test_7404_AllGates_False(void)
{
    TTL7404_RESET_A1(pins);
    TTL7404_RESET_A2(pins);
    TTL7404_RESET_A3(pins);
    TTL7404_RESET_A4(pins);
    TTL7404_RESET_A5(pins);
    TTL7404_RESET_A6(pins);

    TEST_CHECK(TTL7404_GET_Y1(pins));
    TEST_CHECK(TTL7404_GET_Y2(pins));
    TEST_CHECK(TTL7404_GET_Y3(pins));
    TEST_CHECK(TTL7404_GET_Y4(pins));
    TEST_CHECK(TTL7404_GET_Y5(pins));
    TEST_CHECK(TTL7404_GET_Y6(pins));
}

// 7408
void test_7408_Gate1(void)
{
    TTL7408_RESET_A1(pins);
    TTL7408_RESET_B1(pins);
    TEST_CHECK(!TTL7408_GET_Y1(pins));

    TTL7408_RESET_A1(pins);
    TTL7408_SET_B1(pins);
    TEST_CHECK(!TTL7408_GET_Y1(pins));

    TTL7408_SET_A1(pins);
    TTL7408_RESET_B1(pins);
    TEST_CHECK(!TTL7408_GET_Y1(pins));

    TTL7408_SET_A1(pins);
    TTL7408_SET_B1(pins);
    TEST_CHECK(TTL7408_GET_Y1(pins));
}

void test_7408_Gate2(void)
{
    TTL7408_RESET_A2(pins);
    TTL7408_RESET_B2(pins);
    TEST_CHECK(!TTL7408_GET_Y2(pins));

    TTL7408_RESET_A2(pins);
    TTL7408_SET_B2(pins);
    TEST_CHECK(!TTL7408_GET_Y2(pins));

    TTL7408_SET_A2(pins);
    TTL7408_RESET_B2(pins);
    TEST_CHECK(!TTL7408_GET_Y2(pins));

    TTL7408_SET_A2(pins);
    TTL7408_SET_B2(pins);
    TEST_CHECK(TTL7408_GET_Y2(pins));
}

void test_7408_Gate3(void)
{
    TTL7408_RESET_A3(pins);
    TTL7408_RESET_B3(pins);
    TEST_CHECK(!TTL7408_GET_Y3(pins));

    TTL7408_RESET_A3(pins);
    TTL7408_SET_B3(pins);
    TEST_CHECK(!TTL7408_GET_Y3(pins));

    TTL7408_SET_A3(pins);
    TTL7408_RESET_B3(pins);
    TEST_CHECK(!TTL7408_GET_Y3(pins));

    TTL7408_SET_A3(pins);
    TTL7408_SET_B3(pins);
    TEST_CHECK(TTL7408_GET_Y3(pins));
}

void test_7408_Gate4(void)
{
    TTL7408_RESET_A4(pins);
    TTL7408_RESET_B4(pins);
    TEST_CHECK(!TTL7408_GET_Y4(pins));

    TTL7408_RESET_A4(pins);
    TTL7408_SET_B4(pins);
    TEST_CHECK(!TTL7408_GET_Y4(pins));

    TTL7408_SET_A4(pins);
    TTL7408_RESET_B4(pins);
    TEST_CHECK(!TTL7408_GET_Y4(pins));

    TTL7408_SET_A4(pins);
    TTL7408_SET_B4(pins);
    TEST_CHECK(TTL7408_GET_Y4(pins));
}

// 7410
void test_7410_Gate1(void)
{
    TTL7410_RESET_A1(pins);
    TTL7410_RESET_B1(pins);
    TTL7410_RESET_C1(pins);
    TEST_CHECK(TTL7410_GET_Y1(pins));

    TTL7410_RESET_A1(pins);
    TTL7410_SET_B1(pins);
    TTL7410_SET_C1(pins);
    TEST_CHECK(!TTL7410_GET_Y1(pins));

    TTL7410_SET_A1(pins);
    TTL7410_RESET_B1(pins);
    TTL7410_SET_C1(pins);
    TEST_CHECK(!TTL7410_GET_Y1(pins));

    TTL7410_SET_A1(pins);
    TTL7410_SET_B1(pins);
    TTL7410_RESET_C1(pins);
    TEST_CHECK(!TTL7410_GET_Y1(pins));

    TTL7410_SET_A1(pins);
    TTL7410_RESET_B1(pins);
    TTL7410_RESET_C1(pins);
    TEST_CHECK(!TTL7410_GET_Y1(pins));

    TTL7410_RESET_A1(pins);
    TTL7410_SET_B1(pins);
    TTL7410_RESET_C1(pins);
    TEST_CHECK(!TTL7410_GET_Y1(pins));

    TTL7410_RESET_A1(pins);
    TTL7410_RESET_B1(pins);
    TTL7410_SET_C1(pins);
    TEST_CHECK(!TTL7410_GET_Y1(pins));

    TTL7410_SET_A1(pins);
    TTL7410_SET_B1(pins);
    TTL7410_SET_C1(pins);
    TEST_CHECK(!TTL7410_GET_Y1(pins));
}

void test_7410_Gate2(void)
{
    TTL7410_RESET_A2(pins);
    TTL7410_RESET_B2(pins);
    TTL7410_RESET_C2(pins);
    TEST_CHECK(TTL7410_GET_Y2(pins));

    TTL7410_RESET_A2(pins);
    TTL7410_SET_B2(pins);
    TTL7410_SET_C2(pins);
    TEST_CHECK(!TTL7410_GET_Y2(pins));

    TTL7410_SET_A2(pins);
    TTL7410_RESET_B2(pins);
    TTL7410_SET_C2(pins);
    TEST_CHECK(!TTL7410_GET_Y2(pins));

    TTL7410_SET_A2(pins);
    TTL7410_SET_B2(pins);
    TTL7410_RESET_C2(pins);
    TEST_CHECK(!TTL7410_GET_Y2(pins));

    TTL7410_SET_A2(pins);
    TTL7410_RESET_B2(pins);
    TTL7410_RESET_C2(pins);
    TEST_CHECK(!TTL7410_GET_Y2(pins));

    TTL7410_RESET_A2(pins);
    TTL7410_SET_B2(pins);
    TTL7410_RESET_C2(pins);
    TEST_CHECK(!TTL7410_GET_Y2(pins));

    TTL7410_RESET_A2(pins);
    TTL7410_RESET_B2(pins);
    TTL7410_SET_C2(pins);
    TEST_CHECK(!TTL7410_GET_Y2(pins));

    TTL7410_SET_A2(pins);
    TTL7410_SET_B2(pins);
    TTL7410_SET_C2(pins);
    TEST_CHECK(!TTL7410_GET_Y2(pins));
}

void test_7410_Gate3(void)
{
    TTL7410_RESET_A3(pins);
    TTL7410_RESET_B3(pins);
    TTL7410_RESET_C3(pins);
    TEST_CHECK(TTL7410_GET_Y3(pins));

    TTL7410_RESET_A3(pins);
    TTL7410_SET_B3(pins);
    TTL7410_SET_C3(pins);
    TEST_CHECK(!TTL7410_GET_Y3(pins));

    TTL7410_SET_A3(pins);
    TTL7410_RESET_B3(pins);
    TTL7410_SET_C3(pins);
    TEST_CHECK(!TTL7410_GET_Y3(pins));

    TTL7410_SET_A3(pins);
    TTL7410_SET_B3(pins);
    TTL7410_RESET_C3(pins);
    TEST_CHECK(!TTL7410_GET_Y3(pins));

    TTL7410_SET_A3(pins);
    TTL7410_RESET_B3(pins);
    TTL7410_RESET_C3(pins);
    TEST_CHECK(!TTL7410_GET_Y3(pins));

    TTL7410_RESET_A3(pins);
    TTL7410_SET_B3(pins);
    TTL7410_RESET_C3(pins);
    TEST_CHECK(!TTL7410_GET_Y3(pins));

    TTL7410_RESET_A3(pins);
    TTL7410_RESET_B3(pins);
    TTL7410_SET_C3(pins);
    TEST_CHECK(!TTL7410_GET_Y3(pins));

    TTL7410_SET_A3(pins);
    TTL7410_SET_B3(pins);
    TTL7410_SET_C3(pins);
    TEST_CHECK(!TTL7410_GET_Y3(pins));
}

// 7432
void test_7432_Gate1(void)
{
    TTL7432_RESET_A1(pins);
    TTL7432_RESET_B1(pins);
    TEST_CHECK(!TTL7432_GET_Y1(pins));

    TTL7432_RESET_A1(pins);
    TTL7432_SET_B1(pins);
    TEST_CHECK(TTL7432_GET_Y1(pins));

    TTL7432_SET_A1(pins);
    TTL7432_RESET_B1(pins);
    TEST_CHECK(TTL7432_GET_Y1(pins));

    TTL7432_SET_A1(pins);
    TTL7432_SET_B1(pins);
    TEST_CHECK(TTL7432_GET_Y1(pins));
}

void test_7432_Gate2(void)
{
    TTL7432_RESET_A2(pins);
    TTL7432_RESET_B2(pins);
    TEST_CHECK(!TTL7432_GET_Y2(pins));

    TTL7432_RESET_A2(pins);
    TTL7432_SET_B2(pins);
    TEST_CHECK(TTL7432_GET_Y2(pins));

    TTL7432_SET_A2(pins);
    TTL7432_RESET_B2(pins);
    TEST_CHECK(TTL7432_GET_Y2(pins));

    TTL7432_SET_A2(pins);
    TTL7432_SET_B2(pins);
    TEST_CHECK(TTL7432_GET_Y2(pins));
}

void test_7432_Gate3(void)
{
    TTL7432_RESET_A3(pins);
    TTL7432_RESET_B3(pins);
    TEST_CHECK(!TTL7432_GET_Y3(pins));

    TTL7432_RESET_A3(pins);
    TTL7432_SET_B3(pins);
    TEST_CHECK(TTL7432_GET_Y3(pins));

    TTL7432_SET_A3(pins);
    TTL7432_RESET_B3(pins);
    TEST_CHECK(TTL7432_GET_Y3(pins));

    TTL7432_SET_A3(pins);
    TTL7432_SET_B3(pins);
    TEST_CHECK(TTL7432_GET_Y3(pins));
}

void test_7432_Gate4(void)
{
    TTL7432_RESET_A4(pins);
    TTL7432_RESET_B4(pins);
    TEST_CHECK(!TTL7432_GET_Y4(pins));

    TTL7432_RESET_A4(pins);
    TTL7432_SET_B4(pins);
    TEST_CHECK(TTL7432_GET_Y4(pins));

    TTL7432_SET_A4(pins);
    TTL7432_RESET_B4(pins);
    TEST_CHECK(TTL7432_GET_Y4(pins));

    TTL7432_SET_A4(pins);
    TTL7432_SET_B4(pins);
    TEST_CHECK(TTL7432_GET_Y4(pins));
}

void test_7474_FF1(void)
{
    TTL7474_t ttl7474;

    pins = TTL7474_Init(&ttl7474);
    TTL7474_RESET_1PRE(&ttl7474, pins);
    TTL7474_SET_1CLR(&ttl7474, pins);
    TTL7474_RESET_1CLK(&ttl7474, pins);
    TTL7474_RESET_1D(&ttl7474, pins);
}

// 7486 XOR
void test_7486_Gate1(void)
{
    TTL7486_RESET_A1(pins);
    TTL7486_RESET_B1(pins);
    TEST_CHECK(!TTL7486_GET_Y1(pins));

    TTL7486_RESET_A1(pins);
    TTL7486_SET_B1(pins);
    TEST_CHECK(TTL7486_GET_Y1(pins));

    TTL7486_SET_A1(pins);
    TTL7486_RESET_B1(pins);
    TEST_CHECK(TTL7486_GET_Y1(pins));

    TTL7486_SET_A1(pins);
    TTL7486_SET_B1(pins);
    TEST_CHECK(!TTL7486_GET_Y1(pins));
}


void test_7486_Gate2(void)
{
    TTL7486_RESET_A2(pins);
    TTL7486_RESET_B2(pins);
    TEST_CHECK(!TTL7486_GET_Y2(pins));

    TTL7486_RESET_A2(pins);
    TTL7486_SET_B2(pins);
    TEST_CHECK(TTL7486_GET_Y2(pins));

    TTL7486_SET_A2(pins);
    TTL7486_RESET_B2(pins);
    TEST_CHECK(TTL7486_GET_Y2(pins));

    TTL7486_SET_A2(pins);
    TTL7486_SET_B2(pins);
    TEST_CHECK(!TTL7486_GET_Y2(pins));
}

void test_7486_Gate3(void)
{
    TTL7486_RESET_A3(pins);
    TTL7486_RESET_B3(pins);
    TEST_CHECK(!TTL7486_GET_Y3(pins));

    TTL7486_RESET_A3(pins);
    TTL7486_SET_B3(pins);
    TEST_CHECK(TTL7486_GET_Y3(pins));

    TTL7486_SET_A3(pins);
    TTL7486_RESET_B3(pins);
    TEST_CHECK(TTL7486_GET_Y3(pins));

    TTL7486_SET_A3(pins);
    TTL7486_SET_B3(pins);
    TEST_CHECK(!TTL7486_GET_Y3(pins));
}

void test_7486_Gate4(void)
{
    TTL7486_RESET_A4(pins);
    TTL7486_RESET_B4(pins);
    TEST_CHECK(!TTL7486_GET_Y4(pins));

    TTL7486_RESET_A4(pins);
    TTL7486_SET_B4(pins);
    TEST_CHECK(TTL7486_GET_Y4(pins));

    TTL7486_SET_A4(pins);
    TTL7486_RESET_B4(pins);
    TEST_CHECK(TTL7486_GET_Y4(pins));

    TTL7486_SET_A4(pins);
    TTL7486_SET_B4(pins);
    TEST_CHECK(!TTL7486_GET_Y4(pins));
}

void test_7493_Div2(void)
{
    TTL7493_t t;

    pins = TTL7493_Init(&t);

    TTL7493_RESET_CP0(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_RESET_CP0(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_RESET_CP0(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));
}

void test_7493_Div8(void)
{
    TTL7493_t t;

    pins = TTL7493_Init(&t);

    //0
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //1
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //2
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //3
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //4
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //5
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //6
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //7
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //0
    TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));
}

void test_7493_Div16(void)
{
    TTL7493_t t;

    pins = TTL7493_Init(&t);

    //0
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //1
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //2
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //3
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //4
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //5
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //6
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //7
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    //8
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //9
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //10
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //11
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //12
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //13
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //14
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //15
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(TTL7493_GET_Q0(pins));
    TEST_CHECK(TTL7493_GET_Q1(pins));
    TEST_CHECK(TTL7493_GET_Q2(pins));
    TEST_CHECK(TTL7493_GET_Q3(pins));

    //0
    TTL7493_RESET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));

    TTL7493_SET_CP0(&t, pins);
    TTL7493_GET_Q0(pins) ? TTL7493_SET_CP1(&t, pins) : TTL7493_RESET_CP1(&t, pins);
    TEST_CHECK(!TTL7493_GET_Q0(pins));
    TEST_CHECK(!TTL7493_GET_Q1(pins));
    TEST_CHECK(!TTL7493_GET_Q2(pins));
    TEST_CHECK(!TTL7493_GET_Q3(pins));
}

void test_74157_MX1(void)
{
}

TEST_LIST = {
    { "test_7400_Gate1", test_7400_Gate1 },
    { "test_7400_Gate2", test_7400_Gate2 },
    { "test_7400_Gate3", test_7400_Gate3 },
    { "test_7400_Gate4", test_7400_Gate4 },
    { "test_7404_AllGates_True", test_7404_AllGates_True },
    { "test_7404_AllGates_False", test_7404_AllGates_False },
    { "test_7408_Gate1", test_7408_Gate1},
    { "test_7408_Gate2", test_7408_Gate2},
    { "test_7408_Gate3", test_7408_Gate3},
    { "test_7408_Gate4", test_7408_Gate4},
    { "test_7410_Gate1", test_7410_Gate1 },
    { "test_7410_Gate2", test_7410_Gate2 },
    { "test_7410_Gate3", test_7410_Gate3 },
    { "test_7432_Gate1", test_7432_Gate1 },
    { "test_7432_Gate2", test_7432_Gate2 },
    { "test_7432_Gate3", test_7432_Gate3 },
    { "test_7432_Gate4", test_7432_Gate4 },
    { "test_7486_Gate1", test_7486_Gate1 },
    { "test_7486_Gate2", test_7486_Gate2 },
    { "test_7486_Gate3", test_7486_Gate3 },
    { "test_7486_Gate4", test_7486_Gate4 },
    { "test_7474_FF1", test_7474_FF1 },
    { "test_7493_Div2", test_7493_Div2 },
    { "test_7493_Div8", test_7493_Div8 },
    { "test_7493_Div16", test_7493_Div16 },
    { "test_74157_MX1", test_74157_MX1 },
    { NULL, NULL }     // zeroed record marking the end of the list
};

