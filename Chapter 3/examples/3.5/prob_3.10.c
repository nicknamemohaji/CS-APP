short arith3(short x, short y, short z) {
    short p1 = z | y;
    short p2 = p1 >> 9;
    short p3 = ~p2;
    short p4 = p3 - y;
    return p4;
}