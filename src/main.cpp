#include <boost/asio.hpp>
#include <thread>
#include <iostream>
#include <CLI/CLI.hpp>

int main(int argc, char *argv[])
{
    CLI::App app;

    int num_threads = 4;
    app.add_option("-n", num_threads, "Number of threads to run");

    int jobs = 10000;
    app.add_option("-j", jobs, "Number of jobs to run");

    int busy_wait = 100000;
    app.add_option("-b", busy_wait, "Busy wait cycle");

    CLI11_PARSE(app, argc, argv);

    boost::asio::io_context context;
    auto work = boost::asio::make_work_guard(context);
    std::mutex print_mutex;

    std::vector<std::thread> threads;

    for (auto i = 0; i < num_threads; i++)
    {
        threads.emplace_back([&context]{ context.run(); });
    }

    std::vector<std::future<void>> futures;
    for (auto i = 0; i < jobs; i++)
    {
        auto promise = std::make_shared<std::promise<void>>();
        futures.emplace_back(promise->get_future());
        context.post([i, promise = std::move(promise), &print_mutex, busy_wait](){
            {
                std::lock_guard<std::mutex> lock(print_mutex);
                std::cout << "Hello world\t" << i << "\t" << std::this_thread::get_id() << std::endl;
            }

            for (auto x = 0; x < busy_wait; x++) {
                __asm__ volatile("" : "+g" (x) : :);
            }            

            promise->set_value();
        });
    }

    for (auto &future : futures)
    {
        future.get();
    }

    context.stop();

    for (auto &t : threads)
    {
        t.join();
    }

    return 0;
}
