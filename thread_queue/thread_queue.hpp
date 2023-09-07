/**
 * @file thread_queue.hpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @brief Defines the `thread::thread_queue` class, which is used to manage a queue of jobs to run.
 * @details This queue is thread-safe and automatically executes jobs in parallel.
 *
 * @license MIT license (https://opensource.org/licenses/MIT).
 * @copyright (c) 2023 by Cayden Lund.
 *
 */

#ifndef THREAD_QUEUE_HPP
#define THREAD_QUEUE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <utility>

namespace util {

    class thread_queue {
    private:
        /**
         * @brief A thread-safe FIFO queue.
         *
         * @tparam item_t The type of item in the queue.
         */
        template<typename item_t>
        class fifo_queue {
        private:
            /**
             * @brief The type of a single node in the queue.
             *
             */
            struct node_t {
                /**
                 * @brief The item that this node holds.
                 *
                 */
                item_t item;

                /**
                 * @brief A pointer to the next node.
                 * @details Maintains ownership of the next node.
                 *
                 */
                std::unique_ptr<node_t> next;

                /**
                 * @brief Class constructor.
                 *
                 * @param item The item that this node holds.
                 */
                node_t(item_t item) : item(std::move(item)) {}
            };

            /**
             * @brief The size of the queue.
             *
             */
            size_t _queue_size {0};

            /**
             * @brief A pointer to the first node.
             * @details Maintains ownership.
             *
             */
            std::unique_ptr<node_t> _first;

            /**
             * @brief A pointer to the last node.
             * @details Does not maintain ownership; just a reference.
             *
             */
            node_t* _last {nullptr};

            /**
             * @brief The mutex that protects critical sections.
             *
             */
            std::mutex _lock;

        public:
            /**
             * @brief Pushes the given item to the back of the queue.
             *
             * @param item The item to push to the queue.
             */
            void push(item_t item) {
                _lock.lock();
                if (_queue_size++ == 0) {
                    _first = std::make_unique<node_t>(std::move(item));
                    _last = _first.get();
                } else {
                    _last->next = std::move(std::make_unique<node_t>(std::move(item)));
                    _last = _last->next.get();
                }
                _lock.unlock();
            }

            /**
             * @brief Pops the first item from the queue.
             *
             * @return The first item from the queue.
             */
            item_t pop() {
                _lock.lock();
                if (_queue_size-- == 0) throw std::runtime_error("`pop()` called on empty queue");
                item_t item = std::move(_first->item);
                _first = std::move(_first->next);
                _lock.unlock();
                return item;
            }

            /**
             * @brief Returns a reference to the first item in the queue.
             *
             * @return A reference to the first item in the queue.
             */
            item_t& peek() { return _first->item; }

            /**
             * @brief Returns the size of the queue.
             *
             * @return The size of the queue.
             */
            size_t size() { return _queue_size; }

            /**
             * @brief Reports whether the queue is empty.
             *
             * @return True if the queue is empty; false otherwise.
             */
            bool empty() { return _queue_size == 0; }
        };

        /**
         * @brief Defines a job type (zero-argument no-return-type lambda).
         *
         */
        using job_t = std::function<void()>;

        /**
         * @brief The number of threads to use to run jobs.
         *
         */
        size_t _num_threads;

        /**
         * @brief The number of currently-running jobs.
         *
         */
        size_t _current_num_threads {0};

        /**
         * @brief Whether to auto-start jobs when added to the queue.
         *
         */
        bool _auto_start;

        /**
         * @brief Whether new jobs can be added to the queue.
         * @details Useful for destruction.
         *
         */
        bool _allow_new_jobs {true};

        /**
         * @brief The FIFO queue of jobs to run.
         *
         */
        fifo_queue<job_t> _jobs;

        /**
         * @brief The mutex that guards the critical sections.
         *
         */
        std::mutex _lock;

        /**
         * @brief Given a job, constructs a thread that includes the auto-running functionality.
         *
         * @param job The job to run.
         * @return A full job that can be used in a new thread.
         */
        job_t _get_job_runner(const job_t& job) {
            return [this, job]() {
                job();
                --_current_num_threads;
                _start_next_job();
            };
        }

        /**
         * @brief Starts the next job in the queue.
         *
         */
        void _start_next_job() {
            _lock.lock();
            if (_jobs.empty()) {
                _lock.unlock();
                return;
            }
            std::thread new_thread(_get_job_runner(_jobs.pop()));
            new_thread.detach();
            ++_current_num_threads;
            _lock.unlock();
        }

        /**
         * @brief Starts all jobs in the queue, up to the specified number of threads.
         *
         */
        void _start_all_jobs() {
            _lock.lock();
            while (_current_num_threads < _num_threads && _jobs.size() > 0) {
                std::thread new_thread(_get_job_runner(_jobs.pop()));
                new_thread.detach();
                ++_current_num_threads;
            }
            _lock.unlock();
        }

    public:
        /**
         * @brief Class constructor.
         *
         * @param num_threads The number of threads to use to run jobs. Defaults to 4.
         * @param start Whether to automatically start running jobs when added to the queue. Defaults to true.
         */
        thread_queue(size_t num_threads = 4, bool auto_start = true)
            : _num_threads(num_threads), _auto_start(auto_start) {}

        /**
         * @brief Class destructor.
         *
         */
        ~thread_queue() {
            this->start();

            _lock.lock();
            while (!_jobs.empty() || _current_num_threads > 0) {
                _lock.unlock();
                _lock.lock();
            }
            _lock.unlock();
        }

        /**
         * @brief Adds the given job to the queue.
         *
         * @param item The item to add to the job queue.
         */
        void add_job(const job_t& job) {
            if (!_allow_new_jobs) return;
            _jobs.push(job);
            if (_auto_start) _start_all_jobs();
        }

        /**
         * @brief Starts running the jobs in the queue.
         *
         */
        void start() {
            _lock.lock();
            //  If already running, do nothing.
            if (_auto_start) {
                _lock.unlock();
                return;
            }
            _auto_start = true;
            _lock.unlock();

            _start_all_jobs();
        }

        /**
         * @brief Returns the size of the queue.
         *
         * @return The size of the queue.
         */
        size_t size() { return _jobs.size(); }

        /**
         * @brief Reports whether the queue is empty.
         *
         * @return True if the queue is empty; false otherwise.
         */
        bool empty() { return _jobs.empty(); }
    };

}  //  namespace util

#endif
