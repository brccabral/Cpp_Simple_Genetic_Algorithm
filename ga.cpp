#include <iostream>
#include <math.h>
#include <random>
#include <vector>
#include <algorithm>

const int NUM = 60000;        // number of solutions per generation
const int SAMPLE_SIZE = 1000; // how many top solutions for mutation/cross
int max_generations = 100;    // max number of generations to calculate (while loop)

struct Solution
{
    double rank, x, y, z;

    Solution(double in_r, double in_x, double in_y, double in_z)
        : rank(in_r), x(in_x), y(in_y), z(in_z)
    {
        // run our fitness function right at constructor
        fitness();
    }

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

    for (int i = 0; i < NUM; i++)
    {
        solutions.push_back(Solution{0, unif(device), unif(device), unif(device)});
    }

    while (max_generations > 0)
    {
        max_generations--;

        // sort our solutions by rank
        std::sort(solutions.begin(), solutions.end(), [](const auto &left, const auto &right)
                  { return left.rank > right.rank; });

        // print top solutions
        std::cout << "Generation " << max_generations << " ================== " << std::endl;
        std::for_each(solutions.begin(), solutions.begin() + 2, [](const auto &s)
                      { std::cout << std::fixed
                                  << "Rank " << static_cast<int>(s.rank)
                                  << "\nx=" << s.x << "\ny=" << s.y << "\nz=" << s.z << "\n"
                                  << std::endl; });

        // take top solutions
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
}