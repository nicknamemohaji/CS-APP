void while_loop()
{
    int i = 0;
    while (i < 10)
    {
        printf("%d\n", i);
        i++;
    }
    printf("end\n");
}

// void while_loop()
// {
//     int i = 0;
    
//     // while loop 시작 >>>>
//     goto test;  // goto test-expression
// loop:
//     // loop body >>>>
//     printf("%d\n", i);
//     i++;
//     // <<<< loop body
// test:
//     if (i < 10) // test-expression
//         goto loop; 
//     // <<<< while loop 끝
    
//     printf("end\n");
// }