#ifndef __SpringCat_Common_IOHandler__
#define __SpringCat_Common_IOHandler__

#include <BaseCat/System.h>

namespace SpringCat
{
    namespace Common
    {
        template<typename LinkT = Transport::Link>
        class IIOHandler : private BaseCat::System::TL::NonCopyable
        {
        public:
            virtual ~IIOHandler(void) {}

        public:
            virtual void OnOpened(LinkT *link) = 0;
            virtual void OnOpened(LinkT *link, void *context) = 0;
            virtual void OnClosed(LinkT *link) = 0;
            virtual void OnReceived(LinkT *link) = 0;

        public:
            static void *operator new(size_t size)
            {
                if (size == 0)
                {
                    size = 1;
                }

                void *result = NULL;

#pragma warning(disable:4127)
                while (true)
#pragma warning(default:4127)
                {
                    result = BaseCat::System::MPHeap::Alloc(NULL, size);
                    if (result != NULL)
                    {
                        break;
                    }

                    std::new_handler newHandler = std::set_new_handler(0);
                    std::set_new_handler(newHandler);

                    if (newHandler != NULL)
                    {
                        (*newHandler)();
                    }
                    else
                    {
                        throw std::bad_alloc();
                    }
                }

                return result;
            }
            static void operator delete(void *p) throw()
            {
                if (p == NULL)
                {
                    return;
                }

                BaseCat::System::MPHeap::Free(NULL, p);
                p = NULL;
            }
        };
    }
}

#endif
