#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>

struct Solution
{
    double rank, x, y, z;

    // the closer f(x,y,z) is to zero, the higher the rank
    void fitness()
    {
        double ans = (6 * x + -y + std::pow(z, 200)) - 25;
        rank = (ans == 0) ? 9999 : std::abs(1 / ans);
    }
};

int main()
{
    //  create initial random solutions
    std::random_device device;
    std::uniform_real_distribution<double> unif(-100, 100);
    std::vector<Solution> solutions;

    const int NUM = 9000000;
    for (int i = 0; i < NUM; i++)
    {
        solutions.push_back(Solution{0, unif(device), unif(device), unif(device)});
    }

    // run our fitness function
    for (auto &s : solutions)
    {
        s.fitness();
    }

    // sort our solutions by rank
    std::sort(solutions.begin(), solutions.end(), [](const auto &left, const auto &right)
              { return left.rank > right.rank; });

    // print top solutions
    std::for_each(solutions.begin(), solutions.begin() + 10, [](const auto &s)
                  { std::cout << std::fixed
                              << "Rank " << static_cast<int>(s.rank)
                              << "\nx=" << s.x << "\ny=" << s.y << "\nz=" << s.z << "\n"
                              << std::endl; });
}