#include <BaseCat/System.h>
#include <iostream>

struct Test
{
public:
    size_t index_;
    char test_[100];

public:
    Test(size_t index)
        : index_(index)
    {
        std::cout << "constructor: " << index_ << std::endl;
    }
    Test(const Test &rhs)
        : index_(rhs.index_)
    {
        std::cout << "copy-constructor: " << index_ << std::endl;
    }
    ~Test(void)
    {
        std::cout << "destructor: " << index_ << std::endl;
    }
    Test &operator=(const Test &rhs)
    {
        index_ = rhs.index_;
        std::cout << "operator=(): " << index_ << std::endl;
        return *this;
    }
};

int main(void)
{
    BaseCat::System::TL::vector<size_t> v1;
    for (size_t i = 0; i < 100; ++i)
    {
        v1.push_back(i);
    }
    for (BaseCat::System::TL::vector<size_t>::iterator it = v1.begin(); it != v1.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << v1[2] << std::endl;

    BaseCat::System::TL::vector<Test> v2;
    v2.reserve(10);
    for (size_t i = 0; i < 10; ++i)
    {
        v2.push_back(Test(i));
    }
    for (BaseCat::System::TL::vector<Test>::iterator it = v2.begin(); it != v2.end(); ++it)
    {
        std::cout << (*it).index_ << std::endl;
    }

    BaseCat::System::TL::map<size_t, size_t> m1;
    m1.insert(std::make_pair(1, 1));

    return 0;
}
