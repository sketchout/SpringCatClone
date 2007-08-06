#ifndef __SpringCat_Common_DefaultLinkFactory__
#define __SpringCat_Common_DefaultLinkFactory__

#include <BaseCat/System.h>
#include <BaseCat/Network.h>

namespace SpringCat
{
    namespace Transport
    {
        template<typename LinkT>
        struct DefaultLinkFactory : private BaseCat::System::TL::NonCopyable
        {
            static LinkT *Alloc(BaseCat::Network::Link::Handle link)
            {
                return new LinkT(link);
            }
            static void Free(LinkT *linkPtr)
            {
                delete linkPtr;
                linkPtr = NULL;
            }
        };
    }
}

#endif
