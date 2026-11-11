#include <stdio.h>
#include <stdint.h>
#include "bits.h"

static void print_bin(uint32_t v) {
    printf("0x%08X  ", v);
    for (int i = 31; i >= 0; i--) {
        putchar(((v >> i) & 1u) ? '1' : '0');
        if (i % 4 == 0 && i != 0) putchar('_');
    }
    putchar('\n');
}

int main(void) {
    print_bin(set_bit(0x00000000, 3));               // 0x00000008
    print_bin(set_bit(0x00000008, 3));               // 0x00000008
    print_bin(clear_bit(0x00000004, 2));             // 0x00000000
    print_bin(clear_bit(0x00000004, 3));             // 0x00000004
    print_bin(clear_bit(0xFFFFFFFF, 3));             // 0xFFFFFFF7
    print_bin(toggle_bit(0x00000000, 4));            // 0x00000010
    print_bin(toggle_bit(0x00000010, 4));            // 0x00000000
    printf("%u\n", test_bit(0x00010000, 16));        // 1
    printf("%u\n", test_bit(0x00010000, 17));        // 0
    print_bin(set_field(0x00000000, 0x5u, 6, 3));    // 0x00000140
    print_bin(set_field(0xFFFFFFFF, 0x5u, 6, 3));    // 0xFFFFFF7F
    print_bin(set_field(0, 0xFF, 6, 3));             // 0x000001C0
    return 0;
}

/* Example 1:
    reg = 0x00000000 (0000 0000 0000 0000 0000 0000 0000 0000)
    v = 0x5u (101)
    shift = 6
    width = 3

    expected output: 0x00000140 (0000 0000 0000 0000 0000 0001 0100 0000)

    0000 0000 0000 0000 0000 0000 0000 0111 - mask
    0000 0000 0000 0000 0000 0001 1100 0000 - shifted mask
    1111 1111 1111 1111 1111 1110 0011 1111 - ~mask
    0000 0000 0000 0000 0000 0000 0000 0000 - clear
    0000 0000 0000 0000 0000 0000 0000 0101 - value
    0000 0000 0000 0000 0000 0000 0000 0101 - value & mask
    0000 0000 0000 0000 0000 0001 0100 0000 - shifted value
    0000 0000 0000 0000 0000 0001 0100 0000 - reg | value
    */

    /* Example 2:
    reg = 0xFFFFFFFF (1111 1111 1111 1111 1111 1111 1111 1111)
    v = 0x5u (101)
    shift = 6
    width = 3

    expected output: 0xFFFFFF7F (1111 1111 1111 1111 1111 1111 0111 1111)

    0000 0000 0000 0000 0000 0000 0000 0111 - mask
    0000 0000 0000 0000 0000 0001 1100 0000 - shifted mask
    1111 1111 1111 1111 1111 1110 0011 1111 - ~mask
    1111 1111 1111 1111 1111 1110 0011 1111 - clear
    0000 0000 0000 0000 0000 0000 0000 0101 - value
    0000 0000 0000 0000 0000 0000 0000 0101 - value & mask
    0000 0000 0000 0000 0000 0001 0100 0000 - shifted value
    1111 1111 1111 1111 1111 1111 0111 1111 - reg | value
    */

    /* Example 3:
    reg = 0 (0000 0000 0000 0000 0000 0000 0000 0000)
    v = 0xFF (1111 1111)
    shift = 6
    width = 3

    expected output: 0x000001C0 (0000 0000 0000 0000 0000 0001 1100 0000)

    0000 0000 0000 0000 0000 0000 0000 0111 - mask
    0000 0000 0000 0000 0000 0001 1100 0000 - shifted mask
    1111 1111 1111 1111 1111 1110 0011 1111 - ~mask
    0000 0000 0000 0000 0000 0000 0000 0000 - clear
    0000 0000 0000 0000 0000 0000 1111 1111 - value
    0000 0000 0000 0000 0000 0000 0000 0111 - value & mask
    0000 0000 0000 0000 0000 0001 1100 0000 - shifted value
    0000 0000 0000 0000 0000 0001 1100 0000 - reg | value
*/