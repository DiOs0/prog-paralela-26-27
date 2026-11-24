#include <fmt/core.h>
#include <omp.h>
#include <vector>
#include <map>
#include <fmt/ranges.h>

int main(int argsc, char **argv)
{

    /*fmt::println("-------------------------- region paralela");
// Seccion paralela
#pragma omp parallel
    {
        int thread_count = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        fmt::println("Goodbye serial world, hello OpenMP");
        fmt::println("I have {} thread(s) and my thread_id is {}", thread_count, thread_id);
    }

    fmt::println("--------------------------------------- regio paralela num_threads=2");

    #pragma omp parallel num_threads(2)
    {
        int thread_count = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        fmt::println("I have {} thread(s) and my thread_id is {}", thread_count, thread_id);
    }*/

    /*fmt::println("--------------------------------------- regio paralela master thread");

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();

#pragma omp master
        {
            int threads_count = omp_get_num_threads();
            fmt::println("I have {} thread(s) and my thread_id is {}", threads_count);
        }

#pragma omp barrier
        fmt::println("MY thread_id is {}", thread_id);
    }*/

    fmt::println("----------------------------- region paralela for------------------------------------");

    std::vector<int> datos;
    for (int i = 0; i < 1000; i++)
    {
        datos.push_back(i);
    }

#pragma omp parallel for num_threads(2)
    for (int i = 0; i < datos.size(); i++)
    {
        int thread_id = omp_get_thread_num();

        fmt::println("My thread is {} and datos [{}]={}", thread_id, i, datos[i]);
    }

    fmt::println("    ");

    std::map<int, std::vector<int>> datos_map;

    datos_map[0] = {};
    datos_map[1] = {};

    #pragma omp parallel num_threads(2)
    {

        int thread_id = omp_get_thread_num();

    #pragma omp for
        for (int i = 0; i < datos.size(); i++)
        {
            // fmt::println("My thread is {} and datos [{}]={}", thread_id,i,datos[i]);
            datos_map[thread_id].push_back(datos[i]);
        }
    }

    for (auto it : datos_map)
    {
        fmt::println("datos_map[{}]={}", it.first, it.second);
    }


    // fmt::println("{}:{}",0,datos_map[0]);
    // fmt::println("{}:{}",1,datos_map[1]);

    return 0;
}