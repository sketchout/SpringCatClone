//
// SmartHeap 예제
//      SmartHeap: 멀티스레드 환경에 최적화된 메모리 관리자 (ref. count 기능 포함)
//

#include <BaseCat/System.h>
#include <cstdio>
#include <cstring>

using namespace BaseCat::System;

int main(void)
{
    // alloc
    SmartHeap::Block p1 = SmartHeap::Alloc(1024);
    printf_s("address of p1: 0x%p\n", p1);
    printf_s("size of p1: %d\n\n", SmartHeap::GetSize(p1));

    // SmartHeap::Block은 void*와 동일하게 사용 가능
    const char * const text = "test string\n";
    memcpy_s(p1, 1024, text, strlen(text));

    // smart-copy: 실제 복사가 일어나는 게 아니라, ref. count만 증가한다.
    SmartHeap::Block p2 = SmartHeap::Copy(p1);
    printf_s("address of p2: 0x%p\n", p2);
    printf_s("size of p2: %d\n\n", SmartHeap::GetSize(p2));

    // realloc: p1에 대해 ref. count를 증가시켰으므로
    // 같은 크기로 재할당을 시도해도 새로운 메모리를 할당하고 원본 내용을 복사한다.
    // 즉 copy-on-write와 유사하게 동작한다.
    // p1은 더 이상 유효하지 않다.
    SmartHeap::Block p3 = SmartHeap::Realloc(p1, SmartHeap::GetSize(p1));
    printf_s("address of p3: 0x%p\n", p3);
    printf_s("size of p3: %d\n\n", SmartHeap::GetSize(p3));

    // deep-copy: 메모리 새로 할당 후 원본 내용 복사
    SmartHeap::Block p4 = SmartHeap::Clone(p2);
    printf_s("address of p4: 0x%p\n", p4);
    printf_s("size of p4: %d\n\n", SmartHeap::GetSize(p4));

    // free
    SmartHeap::Free(p4);
    SmartHeap::Free(p3);
    SmartHeap::Free(p2);

    return 0;
}
