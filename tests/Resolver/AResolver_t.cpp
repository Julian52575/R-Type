#include <gtest/gtest.h>
#include <memory>
#include <string>
#include "../../src/Resolver/AResolver.hpp"

class intResolver : public Rengine::Resolver::AResolver<long int, std::string> {
    public:
        intResolver(void) = default;
        ~intResolver(void) = default;

    private:
        std::string createNewElement(const long int& key)
        {
            return std::to_string(key);
        };
};
TEST(AResolver, createNewElement)
{
    intResolver r;

    ASSERT_EQ("1", r.get(1));
    ASSERT_EQ("2", r.get(2));
    ASSERT_EQ("123456789", r.get(123456789));
}
TEST(AResolver, get)
{
    intResolver r;
    std::string& one = r.get(1);

    ASSERT_EQ(std::addressof(one), std::addressof(r.get(1)));
    std::string& aLot = r.get(123456789);

    ASSERT_EQ(std::addressof(aLot), std::addressof(r.get(123456789)));
}
TEST(AResolver, remove)
{
    intResolver r;

    ASSERT_THROW(r.remove(1), Rengine::Resolver::AResolverExceptionUnknowKey);
    std::string modValue = "this modified value is stored in the resolver map.";
    std::string& ogStr = r.get(1);  // very bad reference

    ogStr = modValue;
    ASSERT_EQ(r.get(1), modValue);
    r.remove(1);
    ASSERT_NE(r.get(1), modValue);
    ASSERT_EQ(r.get(1), "1");
}
