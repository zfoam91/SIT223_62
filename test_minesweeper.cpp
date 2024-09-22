#include "header.hpp"
#include <gtest/gtest.h>

namespace sf {
    struct Vector2i {
        int x, y;
        Vector2i(int x, int y) : x(x), y(y) {}
    };
}

class MinesweeperTest : public ::testing::Test {
protected:
    void SetUp() override {
        n = 20;
        m = 20;
        cnt = 60;
        arr = initarr(arr);
        showarr = initarr(showarr);
        generate(arr, n, m, cnt);
        count(arr, showarr, n, m);
    }

    void TearDown() override {
        for(int i = 0; i < 100; i++) {
            free(arr[i]);
            free(showarr[i]);
        }
        free(arr);
        free(showarr);
    }
};

TEST_F(MinesweeperTest, InitializationTest) {
    EXPECT_EQ(n, 20);
    EXPECT_EQ(m, 20);
    EXPECT_EQ(cnt, 60);
}

TEST_F(MinesweeperTest, MineCountTest) {
    int mineCount = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (arr[i][j] == 9) mineCount++;
        }
    }
    EXPECT_EQ(mineCount, cnt);
}

TEST_F(MinesweeperTest, BFSTraversalTest) {
    bfstraversal(showarr, arr, 1, 1);
    EXPECT_NE(showarr[1][1], 10); // 10 represents an unopened cell
}

TEST_F(MinesweeperTest, FlagCellTest) {
    showarr[1][1] = 11; // 11 represents a flagged cell
    EXPECT_EQ(showarr[1][1], 11);
}

TEST_F(MinesweeperTest, FirstMoveTest) {
    firstmove(arr, 1, 1, n, m);
    EXPECT_NE(arr[1][1], 9); // The first clicked cell should never be a mine
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
