//
// SpringCat
// Copyright (C) 2007 Bluecat Studio. All rights reserved.
//
// Designed and implemented by Jae-Pil Jeong.
//

#ifndef __SpringCat_Common_IOHandler__
#define __SpringCat_Common_IOHandler__

#include <SpringCat/Common/SmallObject.h>

namespace SpringCat
{
    namespace Common
    {
        template<typename LinkT = Transport::Link>
        class IIOHandler : public SmallObject<IIOHandler<LinkT> >
        {
        public:
            virtual ~IIOHandler(void) {}

        public:
            virtual void OnOpened(LinkT *link) = 0;
            virtual void OnOpened(LinkT *link, void *context) = 0;
            virtual void OnClosed(LinkT *link) = 0;
            virtual void OnReceived(LinkT *link) = 0;
        };
    }
}

#endif
