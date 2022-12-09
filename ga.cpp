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

    // take top solutions
    const int SAMPLE_SIZE = 1000;
    std::vector<Solution> sample;
    std::copy(solutions.begin(), solutions.begin() + SAMPLE_SIZE, std::back_inserter(sample));
    solutions.clear();

    // mutate the top solutions by 1%
    std::uniform_real_distribution<double> m(0.99, 1.01);
    std::for_each(sample.begin(), sample.end(), [&](auto &s)
                  {
        s.x *= m(device);
        s.y *= m(device);
        s.z *= m(device); });

    // cross over
    std::uniform_int_distribution<int> cross(0, SAMPLE_SIZE - 1);
    // mix SAMPLE_SIZE objects to generate NUM amount of new objects
    for (int i = 0; i < NUM; i++)
    {
        solutions.push_back(Solution{
            0,
            sample[cross(device)].x,
            sample[cross(device)].y,
            sample[cross(device)].z});
    }
}