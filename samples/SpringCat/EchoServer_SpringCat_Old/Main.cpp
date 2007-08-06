//
// ExecutorFilter 사용 시 Link의 동기화 문제는 없는지 확인해볼 것
//

#include <Common/ThreadPool.h>
#include <Common/FilterChain.h>
//#include <Common/IOHandler.h>
#include <Filter/AutoRecvFilter.h>
#include <Filter/ExecutorFilter.h>
#include <Filter/TextLineFilter.h>
#include <Transport/Acceptor.h>
#include <Transport/Link.h>
#include <Transport/InputStream.h>
#include <Transport/OutputStream.h>

#include <string>
#include <iostream>
#include <typeinfo>

using namespace SpringCat;

class EchoLink : public SpringCat::Transport::Link
{
private:
    BaseCat::System::TL::string name_;

public:
    EchoLink(BaseCat::Network::Link::Handle link)
        : SpringCat::Transport::Link(link)
    {}
    EchoLink(BaseCat::Network::Link::Handle link, const char *name)
        : SpringCat::Transport::Link(link), name_(name)
    {}

    void SetName(const BaseCat::System::TL::string &name)
    {
        name_ = name;
    }
    const BaseCat::System::TL::string &GetName(void) const
    {
        return name_;
    }
};

typedef BaseCat::System::TL::RefCountPtr<EchoLink> EchoLinkPtr;

class EchoServer : private BaseCat::System::TL::NonCopyable
{
private:
    typedef Transport::Acceptor<EchoLink, EchoServer> Acceptor;

private:
    std::auto_ptr<Common::ThreadPool> threadPool_;
    std::auto_ptr<Common::ThreadPool> executorThreadPool_;
    std::auto_ptr<Common::FilterChain> chain_;
    std::auto_ptr<Filter::AutoRecvFilter> autoRecvFilter_;
    std::auto_ptr<Filter::ExecutorFilter> executorFilter_;
    std::auto_ptr<Filter::TextLineFilter> textLineFilter_;
    std::auto_ptr<Acceptor> acceptor_;

public:
    EchoServer(void)
        : threadPool_(new Common::ThreadPool(0)),
        executorThreadPool_(new Common::ThreadPool(10)),
        chain_(new Common::FilterChain),
        autoRecvFilter_(new Filter::AutoRecvFilter(chain_.get())),
        executorFilter_(new Filter::ExecutorFilter(chain_.get(),
                                    executorThreadPool_.get())),
        textLineFilter_(new Filter::TextLineFilter(chain_.get())),
        acceptor_(new Acceptor(threadPool_.get(), chain_.get()))
    {
        chain_->Add(autoRecvFilter_->GetName().c_str(), autoRecvFilter_.get());

        acceptor_->SetDefaultPort(9999);
        acceptor_->SetHandler(this);
        acceptor_->Start();
    }
    virtual ~EchoServer(void)
    {}

    void OnOpened(EchoLink *link)
    {
        link->SetName("test link");

        std::cout << "EchoServer::OnOpened" << std::endl;
    }
    void OnClosed(EchoLink *link)
    {
        std::cout << "EchoServer::OnClosed" << std::endl;
    }
    void OnReceived(EchoLink *link)
    {
        Transport::InputStream<EchoLink> readStream(link);

        const char *text = reinterpret_cast<const char *>(readStream.GetRawPtr());
        std::cout << "EchoServer::OnReceived - Message: " <<  text;

        Transport::OutputStream<EchoLink> writeStream(link);
        writeStream.Attach(readStream.Detach());
    }
};

int main(void)
{
    EchoServer server;

    getchar();

    return 0;
}
