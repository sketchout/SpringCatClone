//
// Stream ����
//      Stream: ������ �޸� ��Ʈ��
//

#include <BaseCat/System.h>
#include <cstdio>
#include <cstring>

using namespace BaseCat::System;

int main(void)
{
    const char * const text = "test string\n";

    Stream::Handle stream1 = Stream::CreateHandle();
    Stream::Write(stream1, text, strlen(text));

    // stream1�� ���� �޸� ���� ����Ű�� stream2�� �����Ѵ�.
    // SmartHeap::Copy()�� �̿��� �Ϳ� ����!
    Stream::Handle stream2 = Stream::CreateHandle();
    Stream::Attach(stream2, SmartHeap::Copy(Stream::GetRawPtr(stream1)));

    char buffer[1024];

    // stream2�� ������ buffer�� �����Ѵ�.
    Stream::Peek(stream2, buffer, Stream::GetLength(stream2));
    printf_s("length of stream1: %d, length of stream2: %d\n",
        Stream::GetLength(stream1), Stream::GetLength(stream2));
    printf_s("address of stream1: 0x%p, address of stream2: 0x%p\n\n",
        Stream::GetRawPtr(stream1), Stream::GetRawPtr(stream2));

    // stream2�� ������ buffer�� �о���δ�. (stream2���� ������ ������)
    // �� �������� copy-on-write�� �Ͼ stream1�� ���纻�� �����ȴ�.
    Stream::Read(stream2, buffer, Stream::GetLength(stream2));
    printf_s("length of stream1: %d, length of stream2: %d\n",
        Stream::GetLength(stream1), Stream::GetLength(stream2));
    printf_s("address of stream1: 0x%p, address of stream2: 0x%p\n\n",
        Stream::GetRawPtr(stream1), Stream::GetRawPtr(stream2));

    Stream::DestroyHandle(stream2);
    stream2 = NULL;
    Stream::DestroyHandle(stream1);
    stream1 = NULL;

    return 0;
}
