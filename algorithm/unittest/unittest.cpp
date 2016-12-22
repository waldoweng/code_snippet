#include <string>
#include "gtest/gtest.h"
#include "suffix_tree.h"

using namespace std;

namespace 
{

class SuffixTreeTest : public ::testing::Test 
{
    protected:
        virtual void SetUp()
        {
            srand(time(NULL));
            int str_len = rand() % 10 + 1;

            for(int i = 0; i < str_len; i++)
                str += char(rand() % 26 + 'a');

            tree.build(str);
        }

        ::testing::AssertionResult isSuffix(const string &tmp)
        {
            if(tree.isSuffix(tmp))
                return ::testing::AssertionSuccess();
            else
                return ::testing::AssertionFailure() << tmp 
                    << " is not a suffix of " << str;
        }
        
        string str;
        SuffixTree tree;
};


TEST_F(SuffixTreeTest, isSuffixTest)
{
    size_t n = str.size();
    for(int i = 1; i < n; i++)
    {
        string tmp = str.substr(i);
        ASSERT_TRUE(isSuffix(tmp));
    }
}

} // namespace 

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

