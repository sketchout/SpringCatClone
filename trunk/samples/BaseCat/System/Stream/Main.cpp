//
// Stream 예제
//      Stream: 간단한 메모리 스트림
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

    // stream1과 같은 메모리 블럭을 가리키는 stream2를 생성한다.
    // SmartHeap::Copy()를 이용한 것에 주의!
    Stream::Handle stream2 = Stream::CreateHandle();
    Stream::Attach(stream2, SmartHeap::Copy(Stream::GetRawPtr(stream1)));

    char buffer[1024];

    // stream2의 내용을 buffer로 복사한다.
    Stream::Peek(stream2, buffer, Stream::GetLength(stream2));
    printf_s("length of stream1: %d, length of stream2: %d\n",
        Stream::GetLength(stream1), Stream::GetLength(stream2));
    printf_s("address of stream1: 0x%p, address of stream2: 0x%p\n\n",
        Stream::GetRawPtr(stream1), Stream::GetRawPtr(stream2));

    // stream2의 내용을 buffer로 읽어들인다. (stream2에서 데이터 지워짐)
    // 이 시점에서 copy-on-write가 일어나 stream1의 복사본이 생성된다.
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
