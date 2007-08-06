//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Transport_DefaultLinkFactory__
#define __SpringCat_Transport_DefaultLinkFactory__

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
