#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "deque.h"

template<typename data_type>
std::ostream& operator<<(std::ostream& output, const Deque<data_type> &to_print) {
    for (auto it = to_print.begin(); it != to_print.end(); ++it) {
        if (it != to_print.begin())
            output << ' ';
        output << *it;
    }
    return output;
}

class DequeTest : public ::testing::Test {

protected:

    static void popElements(Deque<int> &container, size_t amount) {
        while (amount--) {
            container.pop_back();
        }
    }
    
    static void pushElements(Deque<int> &container, size_t amount) {
        while (amount--) {
            container.push_back(1337);
        }
    }

    static bool checkIfFunctionIsLinear(unsigned int *xValues, unsigned int *yValues, size_t range,
            float tangentLimit) {
        float tangent;
        float maxTangent;
        maxTangent = static_cast<float>(yValues[0]) / static_cast<float>(xValues[0]);

        for (size_t index = 1; index < range; ++index) {
            tangent = static_cast<float>(yValues[index]) / static_cast<float>(xValues[index]);

            maxTangent = maxTangent < tangent ? tangent : maxTangent;
        }

        return maxTangent <= tangentLimit;
    }

    unsigned int measureOperationTime(void (*operation)(Deque<int>&, size_t), Deque<int> &firstArgument,
            size_t secondArgument) {
        clock_t startTime = clock();

        operation(bigDeque, secondArgument);

        clock_t result = clock() - startTime;

        return static_cast<unsigned int>(result);
    }

    DequeTest() {
        floatDeque.push_back(0.0f);
        floatDeque.push_back(1.0f);
        floatDeque.push_back(2.0f);

        stringDeque.push_front("world");
        stringDeque.push_front("hello");

        for (int count = 0; count < 1000; ++count) {
            bigDeque.push_front(999 - count);
        }
    }

    Deque<int> intDeque;
    Deque<float> floatDeque;
    Deque<std::string> stringDeque;

    Deque<int> bigDeque;

};

TEST_F(DequeTest, SizeTest) {
    EXPECT_TRUE(intDeque.empty());
    EXPECT_FALSE(floatDeque.empty());
    EXPECT_FALSE(stringDeque.empty());

    EXPECT_EQ(intDeque.size(), 0);
    EXPECT_EQ(floatDeque.size(), 3);
    EXPECT_EQ(stringDeque.size(), 2);
}

TEST_F(DequeTest, RandomAccessTest) {
    EXPECT_EQ(bigDeque.front(), 0);
    EXPECT_EQ(bigDeque.back(), 999);

    for (int count = 0; count < 1000; ++count) {
        EXPECT_EQ(bigDeque[count], count);
    }

    EXPECT_EQ(stringDeque[0], "hello");
    EXPECT_EQ(stringDeque[1], "world");

    EXPECT_EQ(floatDeque[1], 1.0f);
}

TEST_F(DequeTest, AssignmentTest) {
    for (int count = 0; count < 1000; ++count) {
        bigDeque[999 - count] = count;
    }

    for (int count = 0; count < 1000; ++count) {
        ASSERT_EQ(bigDeque[count], 999 - count);
    }
}

TEST_F(DequeTest, PushTest) {
    for (int count = 5; count <= 9; ++count) {
        intDeque.push_back(count);
    }

    for (int count = 4; count >= 0; --count) {
        intDeque.push_front(count);
    }

    for (int pointer = 0; pointer <= 9; ++pointer) {
        ASSERT_EQ(intDeque[pointer], pointer);
    }
}

TEST_F(DequeTest, PopTest) {
    for (size_t count = 1; count <= 2; ++count) {
        floatDeque.pop_back();
    }
    floatDeque.pop_front();

    EXPECT_TRUE(floatDeque.empty());

    stringDeque.pop_front();

    ASSERT_EQ(stringDeque.size(), 1);

    EXPECT_EQ(stringDeque[0], "world");
}

TEST_F(DequeTest, ComplexityTest) {
    size_t testsCount = 16;
    unsigned int popTimes[testsCount];
    unsigned int pushTimes[testsCount];
    unsigned int testSizes[testsCount] = {
        900000, 500000, 200000, 100000,
        90000, 50000, 20000, 10000,
        9000, 5000, 2000, 1000,
        900, 500, 200, 100
    };

    for (size_t count = 0; count < testsCount; ++count) {
        pushTimes[count] = static_cast<int>(measureOperationTime(pushElements, intDeque, 
                    static_cast<unsigned int>(testSizes[count])));
        popTimes[count] = static_cast<int>(measureOperationTime(popElements, intDeque, 
                    static_cast<unsigned int>(testSizes[count])));
    }


    float tangentLimit = 4.0f;
    ASSERT_TRUE(checkIfFunctionIsLinear(testSizes, pushTimes, testsCount, tangentLimit));
    ASSERT_TRUE(checkIfFunctionIsLinear(testSizes, popTimes, testsCount, tangentLimit));
}

TEST_F(DequeTest, IteratorTest) {
    for (auto iterator = bigDeque.begin(); iterator < bigDeque.end(); ++iterator) {
        ASSERT_EQ(*iterator, iterator - bigDeque.begin());
    }

    for (auto iterator = bigDeque.end() - 1; iterator > bigDeque.begin(); --iterator) {
        ASSERT_EQ(*iterator, iterator - bigDeque.begin());
    }

    for (auto iterator = bigDeque.rbegin(); iterator < bigDeque.rend(); ++iterator) {
        ASSERT_EQ(*iterator, bigDeque.rend() - iterator - 1);
    }

    for (auto iterator = bigDeque.rend() - 1; iterator > bigDeque.rbegin(); --iterator) {
        ASSERT_EQ(*iterator, bigDeque.rend() - iterator - 1);
    }

    Deque<int>::iterator iterator = bigDeque.begin();
    Deque<int>::iterator secondIterator;

    secondIterator = iterator;

    secondIterator += 10;
    iterator += 10;

    ASSERT_EQ(iterator, secondIterator);
    ASSERT_EQ(*iterator, 10);

    secondIterator += 10;
    *secondIterator = *iterator;

    ASSERT_NE(iterator, secondIterator);
    ASSERT_EQ(*iterator, *secondIterator);

    secondIterator -= 10;

    ASSERT_EQ(iterator, secondIterator);
    ASSERT_EQ(*iterator, *secondIterator);

    Deque<int>::reverse_iterator reverseIterator = bigDeque.rbegin();
    Deque<int>::reverse_iterator secondReverseIterator = bigDeque.rbegin();

    reverseIterator += bigDeque.size() - 11;
    secondReverseIterator += bigDeque.size() - 11;

    ASSERT_EQ(*reverseIterator, *iterator);
    ASSERT_EQ(reverseIterator, secondReverseIterator);

    ASSERT_LT(bigDeque.begin(), bigDeque.end());
    ASSERT_GT(bigDeque.rend(), bigDeque.rbegin());

    ASSERT_LT(bigDeque.begin() + 10, bigDeque.end() - 20);
    ASSERT_GT(bigDeque.rend() - 10, bigDeque.rbegin() + 20);

    ASSERT_EQ(bigDeque.begin() - bigDeque.end(), -static_cast<int>(bigDeque.size()));
    ASSERT_EQ(bigDeque.end() - bigDeque.begin(), static_cast<int>(bigDeque.size()));
    ASSERT_EQ(bigDeque.rbegin() - bigDeque.rend(), -static_cast<int>(bigDeque.size()));
    ASSERT_EQ(bigDeque.rend() - bigDeque.rbegin(), static_cast<int>(bigDeque.size()));
    ASSERT_EQ(intDeque.rbegin() - intDeque.rend(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
