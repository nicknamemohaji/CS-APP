void for_loop()
{
    int i;
    for (i = 0; i < 10; i++)
    {
        printf("%d\n", i);
    }
    printf("end\n");
}

// void for_loop()
// {
//     int i;
    
//     // for loop 시작 >>>>
//     i = 0;      // init-expression
//     goto test;  // goto test-expression
// loop:
//     // loop body >>>>
//     printf("%d\n", i);
//     // <<<< loop body
//     i++;        // loop-expression
// test:
//     if (i < 10) // test-expression
//         goto loop; 
//     // <<<< for loop 끝
    
//     printf("end\n");
    
// }