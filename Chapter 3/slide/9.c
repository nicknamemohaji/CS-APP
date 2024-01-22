void do_while_loop()
{
    int i = 0;
    do {
        printf("%d\n", i);
        i++;
    } while (i < 10);
    printf("end\n");
}

// void do_while_loop()
// {
//     int i = 0;

//     // do-while loop 시작 >>>>
// loop:
//     // loop body >>>>
//     printf("%d\n", i);
//     i++;
//     // <<<< loop body
    
//     if (i < 10) // test-expression
//         goto loop; 
//     // <<<< do-while loop 끝

//     printf("end\n");
// }