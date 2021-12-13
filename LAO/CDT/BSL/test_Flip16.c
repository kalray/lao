#include "Flip32.c"
#include "Flip16.c"

#define TEST_NUM 2

typedef struct {
    uint32_t input, result;
} test_t;

int main(void) {
    test_t test_input[TEST_NUM] = {
        {.input = 0x439896db, .result = 0x5cc5},
        {.input = 0xcf094f4a, .result = 0xfc00},

    };
    int i;
    for (i = 0; i < TEST_NUM; i++) {
        test_t test = test_input[i];
        uint32_t result = Flip16_fromFlip32(test.input);

        uint32_t expected = test.result;
        if (expected != result) {
            printf("ERROR test %d: result is 0x%x when 0x%x was expected\n", i, result, expected);
            return 1;
        }
    }

    return 0;

}
