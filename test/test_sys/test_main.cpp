#include <gtest/gtest.h>
#include "can_control.h"
#include "mbed.h"

using namespace sonia_embed;
#define MSG_SIZE 8
#define MAX_IDS 10

TEST(CAN_CONTROL, CONNECT)
{
    EXPECT_ANY_THROW(CanControl<8U>(USER_BUTTON, LED1, false));
    EXPECT_NO_THROW(CanControl<8U>(USBTX, USBRX, true));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock(&argc, argv);

    RUN_ALL_TESTS();

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}