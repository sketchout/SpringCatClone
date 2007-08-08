#include <SpringCat/SpringCat.h>
#include <iostream>

using namespace SpringCat;

class EchoServer : private BaseCat::System::TL::NonCopyable
{
private:
    typedef Transport::Acceptor<Transport::Link, EchoServer> Acceptor;

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
        autoRecvFilter_(new Filter::AutoRecvFilter),
        executorFilter_(new Filter::ExecutorFilter(executorThreadPool_.get())),
        textLineFilter_(new Filter::TextLineFilter),
        acceptor_(new Acceptor(threadPool_.get(), chain_.get()))
    {
        chain_->Add(autoRecvFilter_.get());
        chain_->Add(executorFilter_.get());
        chain_->Add(textLineFilter_.get());

        acceptor_->SetDefaultPort(9999);
        acceptor_->SetHandler(this);
        acceptor_->Start();
    }
    ~EchoServer(void)
    {}

    void OnOpened(Transport::Link *)
    {
        std::cout << "EchoServer::OnOpened" << std::endl;
    }
    void OnClosed(Transport::Link *)
    {
        std::cout << "EchoServer::OnClosed" << std::endl;
    }
    void OnReceived(Transport::Link *link)
    {
        Transport::InputStream<Transport::Link> readStream(link);

        const char *text = reinterpret_cast<const char *>(readStream.GetRawPtr());
        std::cout << "EchoServer::OnReceived - Message: " <<  text;

        Transport::OutputStream<Transport::Link> writeStream(link);
        writeStream.Attach(readStream.Detach());
        writeStream.Flush();

        for (int i = 0; i != 20; ++i)
        {
            char buffer[100];
            sprintf_s(buffer, 100, "test msg: %d\r\n", i + 1);
            writeStream.Write(buffer, strlen(buffer) + 1);
            writeStream.Flush();
        }
    }
};

int main(void)
{
    EchoServer server;

    getchar();

    return 0;
}
