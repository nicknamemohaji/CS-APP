# Bomb lab - CSAPP

> last update 24/3/23

## Basic infos

`file ./bomb` 명령을 이용해 바이너리 정보를 볼 수 있다. strip되지 않은 바이너리이고, 디버깅 심볼들도 살아 있으므로 쉽게 분석할 수 있다.

## Phase 1

`phase_1` 함수에 BP를 걸고 `disas` 명령을 통해 디스어셈블러의 결과를 볼 수 있다.

```assembly
Dump of assembler code for function phase_1:
=> 0x0000000000400ee0 <+0>:     sub    $0x8,%rsp
   0x0000000000400ee4 <+4>:     mov    $0x402400,%esi
   0x0000000000400ee9 <+9>:     call   0x401338 <strings_not_equal>
   0x0000000000400eee <+14>:    test   %eax,%eax
   0x0000000000400ef0 <+16>:    je     0x400ef7 <phase_1+23>
   0x0000000000400ef2 <+18>:    call   0x40143a <explode_bomb>
   0x0000000000400ef7 <+23>:    add    $0x8,%rsp
   0x0000000000400efb <+27>:    ret    
End of assembler dump.
```

- 이때, `rdi` 레지스터에는 `read_input`함수의 결과가 담겨 있다.
    - 리눅스에서 사용되는 sysv x86 컨벤션 상 인자는 [rdi -> rsi -> rdx -> rcx -> r8 -> r9 -> 스택] 순으로 전달하는데,
    - main 루틴에서 호출하기 전 준비하는 부분을 살펴보면 rdi에 입력 버퍼를 담는다.
    - 디버거에서 이 값을 보려면 `x/s $rdi`로 확인 가능
- `strings_not_equal`함수의 첫 인자로 입력값을, 두 번째 인자로 0x402400을 넣어 호출하고
    - 0x402400은 "Border relations with Canada have never been better."이라는 문자열이다.
- 이 값에 따라 explode_bomb으로 넘어가거나 함수가 정상 종료된다. 

즉, `phase_1`함수는 다음 C코드로 번역할 수 있다.
```c
void phase_1(char *input)
{
    if (strings_not_equal(input, "Border relations with Canada have never been better."))
        explode_bomb();
    return ;
}
```

`strings_not_equal`함수의 동작을 확인해 볼 필요가 있다.
```assembly
Dump of assembler code for function strings_not_equal:
   (...)
   0x000000000040133c <+4>:     mov    %rdi,%rbx
   0x000000000040133f <+7>:     mov    %rsi,%rbp
   0x0000000000401342 <+10>:    call   0x40131b <string_length>
   0x0000000000401347 <+15>:    mov    %eax,%r12d
   0x000000000040134a <+18>:    mov    %rbp,%rdi
   0x000000000040134d <+21>:    call   0x40131b <string_length>
   0x0000000000401352 <+26>:    mov    $0x1,%edx
   0x0000000000401357 <+31>:    cmp    %eax,%r12d
   0x000000000040135a <+34>:    jne    0x40139b <strings_not_equal+99>
   0x000000000040135c <+36>:    movzbl (%rbx),%eax
   0x000000000040135f <+39>:    test   %al,%al
   0x0000000000401361 <+41>:    je     0x401388 <strings_not_equal+80>
   0x0000000000401363 <+43>:    cmp    0x0(%rbp),%al
   0x0000000000401366 <+46>:    je     0x401372 <strings_not_equal+58>
   0x0000000000401368 <+48>:    jmp    0x40138f <strings_not_equal+87>
   0x000000000040136a <+50>:    cmp    0x0(%rbp),%al
   0x000000000040136d <+53>:    nopl   (%rax)
   0x0000000000401370 <+56>:    jne    0x401396 <strings_not_equal+94>
   0x0000000000401372 <+58>:    add    $0x1,%rbx
   0x0000000000401376 <+62>:    add    $0x1,%rbp
   0x000000000040137a <+66>:    movzbl (%rbx),%eax
   0x000000000040137d <+69>:    test   %al,%al
   0x000000000040137f <+71>:    jne    0x40136a <strings_not_equal+50>
   0x0000000000401381 <+73>:    mov    $0x0,%edx
   0x0000000000401386 <+78>:    jmp    0x40139b <strings_not_equal+99>
   0x0000000000401388 <+80>:    mov    $0x0,%edx
   0x000000000040138d <+85>:    jmp    0x40139b <strings_not_equal+99>
   0x000000000040138f <+87>:    mov    $0x1,%edx
   0x0000000000401394 <+92>:    jmp    0x40139b <strings_not_equal+99>
   0x0000000000401396 <+94>:    mov    $0x1,%edx
   0x000000000040139b <+99>:    mov    %edx,%eax
   (...)
   0x00000000004013a1 <+105>:   ret    
End of assembler dump.
```
인자로 전달된 rdi, rsi에는 문자열(char *)이 담겨 있다는 것을 기억하자. 
- 함수 프롤로그에서 r12, rbp, rbx를 푸시하고 에필로그에서 이를 다시 팝하는 패턴은 Callee-Saved register를 이용하기 위한 GCC의 최적화 기법이다. 
- `+10` ~ `+34`에서는 rdi, rsi로 전달된 인자 각각에 대해 `string_length`함수를 돌리고 값을 비교한다.
- `+36` ~ `+41`에서는 rdi로 전달된 인자가 빈 문자열인지 확인한다. 
- `+50` ~ `+71`은 반복문의 패턴이다. 포인터를 1씩 증가시키며, 주소를 역참조해 값을 비교한다. 

이를 C코드로 다시 번역하면, 다음과 같다.
```c
bool string_not_equal(char *s1, char *s2)
{
    if (string_length(s1) != string_length(s2))
        return true;
    while (*s1 != '\0')
    {
        if (*s1 != *s2)
            return true;
        s1++;
        s2++;
    }
    return false;
}
```

완전한 분석을 위해서는 `string_length`가 어떤 동작을 하는지 알아야 한다. 

```assembly
Dump of assembler code for function string_length:
   0x000000000040131b <+0>:     cmpb   $0x0,(%rdi)
   0x000000000040131e <+3>:     je     0x401332 <string_length+23>
   0x0000000000401320 <+5>:     mov    %rdi,%rdx
   0x0000000000401323 <+8>:     add    $0x1,%rdx
   0x0000000000401327 <+12>:    mov    %edx,%eax
   0x0000000000401329 <+14>:    sub    %edi,%eax
   0x000000000040132b <+16>:    cmpb   $0x0,(%rdx)
   0x000000000040132e <+19>:    jne    0x401323 <string_length+8>
   0x0000000000401330 <+21>:    repz ret 
   0x0000000000401332 <+23>:    mov    $0x0,%eax
   0x0000000000401337 <+28>:    ret    
End of assembler dump.
```
반복문의 구조를 확인할 수 있다. C코드로 다시 번역하면 다음과 같다.
```c
int string_length(char *str)
{
    char *ptr = str;
    while (*ptr != '\0')
        ptr++;
    return (int) (ptr - str);
}
```

`string_length + 14`에서 eax에서 edi를 지속적으로 빼 주는 것에 주목하면, 포인터를 담고 싶은 것이 아니라 별도의 변수를 담고 있다고 생각해볼 수 있다. 보기 쉬운 형태로 바꿔보자.
```c
int string_length(char *str)
{
    int len = 0;
    char *ptr = str;
    while (*ptr != '\0')
    {
        ptr++;
        len++;
    }
    return len;
}
```

괜한 짓을 한 것 같다... Dr.Evil이 함수 이름과 동작을 일치시켰다. ~~닉값좀~~

> Phase 1 Key: "Border relations with Canada have never been better."

# Phase 2

```assembly
Dump of assembler code for function phase_2:
=> 0x0000000000400efc <+0>:     push   %rbp
   (...)
   0x0000000000400f02 <+6>:     mov    %rsp,%rsi
   0x0000000000400f05 <+9>:     call   0x40145c <read_six_numbers>
   0x0000000000400f0a <+14>:    cmpl   $0x1,(%rsp)
   0x0000000000400f0e <+18>:    je     0x400f30 <phase_2+52>
   0x0000000000400f10 <+20>:    call   0x40143a <explode_bomb>
   0x0000000000400f15 <+25>:    jmp    0x400f30 <phase_2+52>
   0x0000000000400f17 <+27>:    mov    -0x4(%rbx),%eax
   0x0000000000400f1a <+30>:    add    %eax,%eax
   0x0000000000400f1c <+32>:    cmp    %eax,(%rbx)
   0x0000000000400f1e <+34>:    je     0x400f25 <phase_2+41>
   0x0000000000400f20 <+36>:    call   0x40143a <explode_bomb>
   0x0000000000400f25 <+41>:    add    $0x4,%rbx
   0x0000000000400f29 <+45>:    cmp    %rbp,%rbx
   0x0000000000400f2c <+48>:    jne    0x400f17 <phase_2+27>
   0x0000000000400f2e <+50>:    jmp    0x400f3c <phase_2+64>
   0x0000000000400f30 <+52>:    lea    0x4(%rsp),%rbx
   0x0000000000400f35 <+57>:    lea    0x18(%rsp),%rbp
   0x0000000000400f3a <+62>:    jmp    0x400f17 <phase_2+27>
   (...)
   0x0000000000400f42 <+70>:    ret    
End of assembler dump.
```

- 마찬가지로 rdi에는 read_input의 결과가 담겨 있다.
- `read_six_numbers`함수에서 sscanf를 이용해 문자열 입력을 숫자로 파싱한다. 이 결과는 phase_2의 스택에 저장된다.
- `+52`, `+57`: lea 인스트럭션으로 반복문의 조건을 세팅한다.
- `+27` ~ `+48`: 반복문 패턴이다

phase 2는 다음과 같이 번역할 수 있다.
```c
void phase_2(char *str)
{
    int n[6];     // rsp, rsp + 0x4 , rsp + 0x8, rsp + 0xc, rsp + 0x10, rsp + 0x14
    
    // read_six_numbers >>>>
    if(sscanf("%d %d %d %d %d %d", str, &n[0], &n[1], &n[2], &n[3], &n[4], &n[5]) < 5)
        explode_bomb();
    // <<< read_six_numbers

    if (n[0] != 0x1)
        explode_bomb();
    int idx = 1;
    while(idx < 6)
    {
        if (n[idx - 1] * 2 != n[idx])
            explode_bomb();
        idx++;        
    }
}
```

> Phase 2 Key: "1 2 4 8 16 32"

## Phase 3

```assembly
Dump of assembler code for function phase_3:
=> (...)
   0x0000000000400f51 <+14>:    mov    $0x4025cf,%esi
   0x0000000000400f56 <+19>:    mov    $0x0,%eax
   0x0000000000400f5b <+24>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000400f60 <+29>:    cmp    $0x1,%eax
   0x0000000000400f63 <+32>:    jg     0x400f6a <phase_3+39>
   0x0000000000400f65 <+34>:    call   0x40143a <explode_bomb>
   0x0000000000400f6a <+39>:    cmpl   $0x7,0x8(%rsp)
   0x0000000000400f6f <+44>:    ja     0x400fad <phase_3+106>
   0x0000000000400f71 <+46>:    mov    0x8(%rsp),%eax
   0x0000000000400f75 <+50>:    jmp    *0x402470(,%rax,8)
   0x0000000000400f7c <+57>:    mov    $0xcf,%eax
   0x0000000000400f81 <+62>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f83 <+64>:    mov    $0x2c3,%eax
   0x0000000000400f88 <+69>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f8a <+71>:    mov    $0x100,%eax
   0x0000000000400f8f <+76>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f91 <+78>:    mov    $0x185,%eax
   0x0000000000400f96 <+83>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f98 <+85>:    mov    $0xce,%eax
   0x0000000000400f9d <+90>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400f9f <+92>:    mov    $0x2aa,%eax
   0x0000000000400fa4 <+97>:    jmp    0x400fbe <phase_3+123>
   0x0000000000400fa6 <+99>:    mov    $0x147,%eax
   0x0000000000400fab <+104>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fad <+106>:   call   0x40143a <explode_bomb>
   0x0000000000400fb2 <+111>:   mov    $0x0,%eax
   0x0000000000400fb7 <+116>:   jmp    0x400fbe <phase_3+123>
   0x0000000000400fb9 <+118>:   mov    $0x137,%eax
   0x0000000000400fbe <+123>:   cmp    0xc(%rsp),%eax
   0x0000000000400fc2 <+127>:   je     0x400fc9 <phase_3+134>
   0x0000000000400fc4 <+129>:   call   0x40143a <explode_bomb>
   (...)
   0x0000000000400fcd <+138>:   ret    
End of assembler dump.
```

- `+14` ~ `+24`: sccanf를 이용해 입력을 파싱
    - `rdi`에는 read_input의 결과가 담겨 있고
    - 0x4025cf에는 "%d %d"라는 포맷스트링이 담겨 있다.
    - sscanf의 결과는 스택에(int이므로 4바이트 - `rsp + 0x8`, `rsp + 0xc`) 푸시되고
    - 몇 개의 인자를 읽었는지 확인한다(`+29` ~ `+34`)
- `+46` ~ `+50`: 첫 숫자에 대해 점프 테이블을 사용 
    - jg가 아니라 ja이므로 음수나, 7 초과의 값들은 모두 default case -> `explode_bomb`
    - 두 번째 수와 비교할 값을 세팅
    0. `+57`: 0xcf
    1. `+118`: 0x137
    2. `+64`: 0x2c3
    3. `+71`: 0x100
    4. `+78`: 0x185
    5. `+85`: 0xce
    6. `+92`: 0x2aa
    7. `+99`: 0x147
phase_3는 다음과 같이 번역할 수 있다.

```c
void phase_3(char *input)
{
    int n1, n2, cmp;
    if (sscanf(input, "%d %d", &n1, &n2) <= 1) 
        explode_bomb();
    switch (n1)
    {
        case 0:
            cmp = 0xcf;
            break;
        case 1:
            cmp = 0x137;
            break;
        case 2:
            cmp = 0x2c3;
            break;
        case 3:
            cmp = 0x100;
            break;
        case 4:
            cmp = 0x185;
            break;
        case 5:
            cmp = 0xce;
            break;
        case 6:
            cmp = 0x2aa;
            break;
        case 7:
            cmp = 0x147;
            break;
        default:
            explode_bomb();
            return ;
    }
    if (n2 != cmp)
        explode_bomb();
}
```

즉, phase_3에 대해서는 8종류의 키가 존재한다. 그 중 하나는 0, 207(0xcf)이다.


> Phase 3 Key: "0 207"

## phase 4

```assembly
Dump of assembler code for function phase_4:
=> 0x000000000040100c <+0>:     sub    $0x18,%rsp
   0x0000000000401010 <+4>:     lea    0xc(%rsp),%rcx
   0x0000000000401015 <+9>:     lea    0x8(%rsp),%rdx
   0x000000000040101a <+14>:    mov    $0x4025cf,%esi
   0x000000000040101f <+19>:    mov    $0x0,%eax
   0x0000000000401024 <+24>:    call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000401029 <+29>:    cmp    $0x2,%eax
   0x000000000040102c <+32>:    jne    0x401035 <phase_4+41>
   0x000000000040102e <+34>:    cmpl   $0xe,0x8(%rsp)
   0x0000000000401033 <+39>:    jbe    0x40103a <phase_4+46>
   0x0000000000401035 <+41>:    call   0x40143a <explode_bomb>
   0x000000000040103a <+46>:    mov    $0xe,%edx
   0x000000000040103f <+51>:    mov    $0x0,%esi
   0x0000000000401044 <+56>:    mov    0x8(%rsp),%edi
   0x0000000000401048 <+60>:    call   0x400fce <func4>
   0x000000000040104d <+65>:    test   %eax,%eax
   0x000000000040104f <+67>:    jne    0x401058 <phase_4+76>
   0x0000000000401051 <+69>:    cmpl   $0x0,0xc(%rsp)
   0x0000000000401056 <+74>:    je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:    call   0x40143a <explode_bomb>
   0x000000000040105d <+81>:    add    $0x18,%rsp
   0x0000000000401061 <+85>:    ret    
End of assembler dump.
```

phase_4를 번역하면 다음과 같다. 
```c
void phase_4(char *input)
{
    int n1, n2;
    if (sscanf(input, "%d %d", &n1, &n2) != 2)
        explode_bomb();
    if (n1 > 0xe)
        explode_bomb();
    if (!func4(n1, 0, 0xe))
        explode_bomb();
    if (n2 != 0)
        explode_bomb();
}
```
n2는 0으로 고정이지만, n1의 값을 알아내기 위해서는 func4를 분석해야 한다. 

```assembly
Dump of assembler code for function func4:
   0x0000000000400fce <+0>:     sub    $0x8,%rsp
   0x0000000000400fd2 <+4>:     mov    %edx,%eax
   0x0000000000400fd4 <+6>:     sub    %esi,%eax
   0x0000000000400fd6 <+8>:     mov    %eax,%ecx
   0x0000000000400fd8 <+10>:    shr    $0x1f,%ecx
   0x0000000000400fdb <+13>:    add    %ecx,%eax
   0x0000000000400fdd <+15>:    sar    %eax
   0x0000000000400fdf <+17>:    lea    (%rax,%rsi,1),%ecx
   0x0000000000400fe2 <+20>:    cmp    %edi,%ecx
   0x0000000000400fe4 <+22>:    jle    0x400ff2 <func4+36>
   0x0000000000400fe6 <+24>:    lea    -0x1(%rcx),%edx
   0x0000000000400fe9 <+27>:    call   0x400fce <func4>
   0x0000000000400fee <+32>:    add    %eax,%eax
   0x0000000000400ff0 <+34>:    jmp    0x401007 <func4+57>
   0x0000000000400ff2 <+36>:    mov    $0x0,%eax
   0x0000000000400ff7 <+41>:    cmp    %edi,%ecx
   0x0000000000400ff9 <+43>:    jge    0x401007 <func4+57>
   0x0000000000400ffb <+45>:    lea    0x1(%rcx),%esi
   0x0000000000400ffe <+48>:    call   0x400fce <func4>
   0x0000000000401003 <+53>:    lea    0x1(%rax,%rax,1),%eax
   0x0000000000401007 <+57>:    add    $0x8,%rsp
   0x000000000040100b <+61>:    ret    
End of assembler dump.
```

```c
// initial run: const1 = 0, const2 = 0xe
int func4(int n1, int const1, int const2)
{
    /*
    다음 주석처리된 코드들은 실제 인스트럭션들이지만,
    실질적으로 필요 없는 연산 과정이다.

    int temp1 = const2 - const1;
    unsigned int temp2 = ((unsigned int) temp1) >> 0x1f;
    temp1 += (signed int) temp2;
    temp1 = temp1 >> 1;
    temp2 = temp1 + const1;
    
    실질적으로는 이 연산만이 반영된다:
    */
    int temp2 = (const2 - const1) >> 1;
    if (temp2 <= n1)
    {
        if (temp2 >= const1)
            return 0;
        return func4(n1, temp2 + 1, const2) * 2 + 1;
    }
    else
        return func4(n1, const1, temp2 - 1) * 2;
}
```
func4는 재귀적으로 작동한다. 최종적으로 리턴값이 0이 되게 하기 위해서, 다음 과정을 거쳐야 한다.
1. 첫 재귀 func4(n1, 0, 0xe): 7 --> return func4(n1, 0, 7) * 2
2. func4(n1, 0, 7): 3 --> return func4(n1, 0, 3) * 2
3. func4(n1, 0, 3): 1 --> return 0

> Phase 4 Key: "1 0"

## Phase 5

```assembly
Dump of assembler code for function phase_5:
=> (...)
   0x0000000000401078 <+22>:    xor    %eax,%eax
   0x000000000040107a <+24>:    call   0x40131b <string_length>
   0x000000000040107f <+29>:    cmp    $0x6,%eax
   0x0000000000401082 <+32>:    je     0x4010d2 <phase_5+112>
   0x0000000000401084 <+34>:    call   0x40143a <explode_bomb>
   0x0000000000401089 <+39>:    jmp    0x4010d2 <phase_5+112>
   0x000000000040108b <+41>:    movzbl (%rbx,%rax,1),%ecx           // rbx = rdi -> *input, rax <-- loop counter (0 ~ 5)
   0x000000000040108f <+45>:    mov    %cl,(%rsp)                   // *rsp = input[i]
   0x0000000000401092 <+48>:    mov    (%rsp),%rdx
   0x0000000000401096 <+52>:    and    $0xf,%edx
   0x0000000000401099 <+55>:    movzbl 0x4024b0(%rdx),%edx          // edx = __stream[input[i] & 0xf]
   0x00000000004010a0 <+62>:    mov    %dl,0x10(%rsp,%rax,1)        // (rsp + 0x10)[i] = __stream[input[i] & 0xf]
   0x00000000004010a4 <+66>:    add    $0x1,%rax
   0x00000000004010a8 <+70>:    cmp    $0x6,%rax
   0x00000000004010ac <+74>:    jne    0x40108b <phase_5+41>
   0x00000000004010ae <+76>:    movb   $0x0,0x16(%rsp)
   0x00000000004010b3 <+81>:    mov    $0x40245e,%esi
   0x00000000004010b8 <+86>:    lea    0x10(%rsp),%rdi
   0x00000000004010bd <+91>:    call   0x401338 <strings_not_equal> // strings_not_equal(rsp + 0x10, __stream)
   0x00000000004010c2 <+96>:    test   %eax,%eax
   0x00000000004010c4 <+98>:    je     0x4010d9 <phase_5+119>
   0x00000000004010c6 <+100>:   call   0x40143a <explode_bomb>
   0x00000000004010cb <+105>:   nopl   0x0(%rax,%rax,1)
   0x00000000004010d0 <+110>:   jmp    0x4010d9 <phase_5+119>
   0x00000000004010d2 <+112>:   mov    $0x0,%eax
   0x00000000004010d7 <+117>:   jmp    0x40108b <phase_5+41>
   (...)
   0x00000000004010ee <+140>:   add    $0x20,%rsp
   0x00000000004010f2 <+144>:   pop    %rbx
   0x00000000004010f3 <+145>:   ret    
End of assembler dump.
```

stack canary를 확인하는 코드가 있어서 좀 헷갈렸다...
번역하면 다음과 같다.
```c
void phase_5(char *input)
{
    if (string_length(input) != 6)
        explode_bomb();
    
    char stream[] = "maduiersnfotvbyl";
    char compare[6] = {0};
    for (int i = 0; i < 6; i++)
        compare[i] = stream[input[i] & 0xf];
    
    if (strings_not_eqeual(compare, "flyers"))
        explode_bomb();
}
```

즉, 입력값의 각 문자의 아스키 코드 하위 16비트가 순서대로 [9, 15, 14, 5, 6, 7]여야 한다.
여러 종류가 가능하지만, 입력하기 쉽게 영문 아스키에서 찾자.

> Phase 5 Key: "ionefg"

## Phase 6

```assembly
=> (...)
   0x0000000000401103 <+15>:    mov    %rsp,%rsi
   0x0000000000401106 <+18>:    call   0x40145c <read_six_numbers>
   0x000000000040110b <+23>:    mov    %rsp,%r14
   0x000000000040110e <+26>:    mov    $0x0,%r12d
   0x0000000000401114 <+32>:    mov    %r13,%rbp
   0x0000000000401117 <+35>:    mov    0x0(%r13),%eax
   0x000000000040111b <+39>:    sub    $0x1,%eax
   0x000000000040111e <+42>:    cmp    $0x5,%eax
   0x0000000000401121 <+45>:    jbe    0x401128 <phase_6+52>
   0x0000000000401123 <+47>:    call   0x40143a <explode_bomb>
   0x0000000000401128 <+52>:    add    $0x1,%r12d
   0x000000000040112c <+56>:    cmp    $0x6,%r12d
   0x0000000000401130 <+60>:    je     0x401153 <phase_6+95>
   0x0000000000401132 <+62>:    mov    %r12d,%ebx
   0x0000000000401135 <+65>:    movslq %ebx,%rax
   0x0000000000401138 <+68>:    mov    (%rsp,%rax,4),%eax
   0x000000000040113b <+71>:    cmp    %eax,0x0(%rbp)
   0x000000000040113e <+74>:    jne    0x401145 <phase_6+81>
   0x0000000000401140 <+76>:    call   0x40143a <explode_bomb>
   0x0000000000401145 <+81>:    add    $0x1,%ebx
   0x0000000000401148 <+84>:    cmp    $0x5,%ebx
   0x000000000040114b <+87>:    jle    0x401135 <phase_6+65>
   0x000000000040114d <+89>:    add    $0x4,%r13
   0x0000000000401151 <+93>:    jmp    0x401114 <phase_6+32>
   0x0000000000401153 <+95>:    lea    0x18(%rsp),%rsi
   0x0000000000401158 <+100>:   mov    %r14,%rax
   0x000000000040115b <+103>:   mov    $0x7,%ecx
   0x0000000000401160 <+108>:   mov    %ecx,%edx
   0x0000000000401162 <+110>:   sub    (%rax),%edx
   0x0000000000401164 <+112>:   mov    %edx,(%rax)
   0x0000000000401166 <+114>:   add    $0x4,%rax
   0x000000000040116a <+118>:   cmp    %rsi,%rax
   0x000000000040116d <+121>:   jne    0x401160 <phase_6+108>
   0x000000000040116f <+123>:   mov    $0x0,%esi
   0x0000000000401174 <+128>:   jmp    0x401197 <phase_6+163>
   0x0000000000401176 <+130>:   mov    0x8(%rdx),%rdx
   0x000000000040117a <+134>:   add    $0x1,%eax
   0x000000000040117d <+137>:   cmp    %ecx,%eax
   0x000000000040117f <+139>:   jne    0x401176 <phase_6+130>
   0x0000000000401181 <+141>:   jmp    0x401188 <phase_6+148>
   0x0000000000401183 <+143>:   mov    $0x6032d0,%edx
   0x0000000000401188 <+148>:   mov    %rdx,0x20(%rsp,%rsi,2)
   0x000000000040118d <+153>:   add    $0x4,%rsi
   0x0000000000401191 <+157>:   cmp    $0x18,%rsi
   0x0000000000401195 <+161>:   je     0x4011ab <phase_6+183>
   0x0000000000401197 <+163>:   mov    (%rsp,%rsi,1),%ecx
   0x000000000040119a <+166>:   cmp    $0x1,%ecx
   0x000000000040119d <+169>:   jle    0x401183 <phase_6+143>
   0x000000000040119f <+171>:   mov    $0x1,%eax
   0x00000000004011a4 <+176>:   mov    $0x6032d0,%edx
   0x00000000004011a9 <+181>:   jmp    0x401176 <phase_6+130>
   0x00000000004011ab <+183>:   mov    0x20(%rsp),%rbx
   0x00000000004011b0 <+188>:   lea    0x28(%rsp),%rax
   0x00000000004011b5 <+193>:   lea    0x50(%rsp),%rsi
   0x00000000004011ba <+198>:   mov    %rbx,%rcx
   0x00000000004011bd <+201>:   mov    (%rax),%rdx
   0x00000000004011c0 <+204>:   mov    %rdx,0x8(%rcx)
   0x00000000004011c4 <+208>:   add    $0x8,%rax
   0x00000000004011c8 <+212>:   cmp    %rsi,%rax
   0x00000000004011cb <+215>:   je     0x4011d2 <phase_6+222>
   0x00000000004011cd <+217>:   mov    %rdx,%rcx
   0x00000000004011d0 <+220>:   jmp    0x4011bd <phase_6+201>
   0x00000000004011d2 <+222>:   movq   $0x0,0x8(%rdx)
   0x00000000004011da <+230>:   mov    $0x5,%ebp
   0x00000000004011df <+235>:   mov    0x8(%rbx),%rax
   0x00000000004011e3 <+239>:   mov    (%rax),%eax
   0x00000000004011e5 <+241>:   cmp    %eax,(%rbx)
   0x00000000004011e7 <+243>:   jge    0x4011ee <phase_6+250>
   0x00000000004011e9 <+245>:   call   0x40143a <explode_bomb>
   0x00000000004011ee <+250>:   mov    0x8(%rbx),%rbx
   0x00000000004011f2 <+254>:   sub    $0x1,%ebp
   0x00000000004011f5 <+257>:   jne    0x4011df <phase_6+235>
   (...)
   0x0000000000401203 <+271>:   ret    
End of assembler dump.
```

번역하면 다음과 같다.

```c
void phase_5(char *input)
{
    int n[6];     // rsp, rsp + 0x4 , rsp + 0x8, rsp + 0xc, rsp + 0x10, rsp + 0x14
    if(sscanf("%d %d %d %d %d %d", str, &n[0], &n[1], &n[2], &n[3], &n[4], &n[5]) < 5)
}
```
