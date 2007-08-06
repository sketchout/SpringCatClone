//
// SmartHeap ����
//      SmartHeap: ��Ƽ������ ȯ�濡 ����ȭ�� �޸� ������ (ref. count ��� ����)
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

    // SmartHeap::Block�� void*�� �����ϰ� ��� ����
    const char * const text = "test string\n";
    memcpy_s(p1, 1024, text, strlen(text));

    // smart-copy: ���� ���簡 �Ͼ�� �� �ƴ϶�, ref. count�� �����Ѵ�.
    SmartHeap::Block p2 = SmartHeap::Copy(p1);
    printf_s("address of p2: 0x%p\n", p2);
    printf_s("size of p2: %d\n\n", SmartHeap::GetSize(p2));

    // realloc: p1�� ���� ref. count�� �����������Ƿ�
    // ���� ũ��� ���Ҵ��� �õ��ص� ���ο� �޸𸮸� �Ҵ��ϰ� ���� ������ �����Ѵ�.
    // �� copy-on-write�� �����ϰ� �����Ѵ�.
    // p1�� �� �̻� ��ȿ���� �ʴ�.
    SmartHeap::Block p3 = SmartHeap::Realloc(p1, SmartHeap::GetSize(p1));
    printf_s("address of p3: 0x%p\n", p3);
    printf_s("size of p3: %d\n\n", SmartHeap::GetSize(p3));

    // deep-copy: �޸� ���� �Ҵ� �� ���� ���� ����
    SmartHeap::Block p4 = SmartHeap::Clone(p2);
    printf_s("address of p4: 0x%p\n", p4);
    printf_s("size of p4: %d\n\n", SmartHeap::GetSize(p4));

    // free
    SmartHeap::Free(p4);
    SmartHeap::Free(p3);
    SmartHeap::Free(p2);

    return 0;
}
