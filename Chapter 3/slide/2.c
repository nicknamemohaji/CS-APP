int min(int x, int y)
{
    if (x < y)
        goto x_is_smaller;
    return y;
x_is_smaller:
    return x;
}