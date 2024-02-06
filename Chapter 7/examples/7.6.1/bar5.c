// GCC 11버전에서는 링킹 에러가 발생하므로 extern storage class를 사용하여 해결
// 단, extern storage class를 사용하면 링킹 과정에서 경고도 나타나지 않음
extern double x;

// 원본 예시. GCC 4버전에서는 링킹 에러가 발생하지 않음
// double x;

void f(void)
{
    x = -0.0;
}