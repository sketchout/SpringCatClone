//
// MPHeap ����
//      MPHeap: ��Ƽ������ ȯ�濡 ����ȭ�� �޸� ������
//

#include <BaseCat/System.h>
#include <cstdio>

using namespace BaseCat::System;

int main(void)
{
    void *p = MPHeap::Alloc(NULL, 1024);
    printf_s("address: 0x%p\n", p);
    printf_s("size: %d\n\n", MPHeap::GetSize(NULL, p));

    p = MPHeap::Realloc(NULL, p, 2048);
    printf_s("address: 0x%p\n", p);
    printf_s("size: %d\n\n", MPHeap::GetSize(NULL, p));

    p = MPHeap::Realloc(NULL, p, 1024);
    printf_s("address: 0x%p\n", p);
    printf_s("size: %d\n\n", MPHeap::GetSize(NULL, p));

    MPHeap::Free(NULL, p);

    return 0;
}
