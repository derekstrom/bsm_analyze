// Threads Base
//
// All threads should inherit from this base class
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_CORE_THREAD
#define BSM_CORE_THREAD

#include <queue>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace bsm
{
    class Reader;

    namespace core
    {
        class Condition;
        class KeyboardController;
        class AnalyzerThread;
        class Thread;

        typedef boost::shared_ptr<Condition> ConditionPtr;
        typedef std::vector<std::string> Files;

        class ThreadController
        {
            public:
                enum Command
                {
                    QUIT = 0,
                    HELP = 1,
                    STATUS = 2
                };

                ThreadController(const uint32_t &max_threads = 0) throw();
                ~ThreadController();

                ConditionPtr condition() const;

                void process(const Files &input_files);

                void notify(AnalyzerThread *thread);
                void notify(const Command &command);

            private:
                bool init(const Files &);
                void reset();

                void createThreads();
                void startThreads();
                void run();
                void wait();
                void onThreadComplete();
                void continueThread();
                void stopThread();

                uint32_t _max_threads;
                boost::shared_ptr<Condition> _condition;

                Files _input_files;
                Files::const_iterator _next_file;

                typedef boost::shared_ptr<Thread> ThreadPtr;
                typedef boost::shared_ptr<AnalyzerThread> AnalyzerThreadPtr;
                typedef std::vector<AnalyzerThreadPtr> Threads;
                typedef std::queue<AnalyzerThread *> ThreadsFIFO;

                Threads _threads;
                ThreadsFIFO _complete_threads;

                ThreadPtr _keyboard_thread;

                uint32_t _running_threads;
        };

        class Thread
        {
            public:
                Thread(ThreadController *controller) throw();
                virtual ~Thread();

                ConditionPtr condition() const;

                bool start();
                void join();

                virtual void stop() = 0;

            protected:
                virtual void run() = 0;

                 ThreadController *controller() const;

            private:
                boost::shared_ptr<Condition> _condition;
                boost::thread _thread;

                ThreadController *_controller;
        };

        class AnalyzerThread: public Thread
        {
            public:
                AnalyzerThread(ThreadController *controller);

                void init(const std::string &file_name);

                virtual void stop();

            protected:
                virtual void run();

            private:
                void wait();

                bool _continue;
                bool _wait_for_instructions;

                typedef boost::shared_ptr<Reader> ReaderPtr;
                ReaderPtr _reader;

                uint32_t _events_read; // read events in last file
                uint32_t _events_processed;
        };

        class KeyboardThread: public Thread
        {
            public:
                KeyboardThread(ThreadController *controller) throw();

                virtual void stop();

            protected:
                virtual void run();

            private:
                boost::shared_ptr<KeyboardController> _controller;

                bool _continue;

        };

        class Lock
        {
            public:
                typedef boost::unique_lock<boost::mutex> UniqueLock;

                Lock(const ConditionPtr &);

                UniqueLock &operator()();

            private:
                UniqueLock _lock;
        };

        class Condition
        {
            public:
                typedef boost::shared_ptr<boost::mutex> MutexPtr;
                typedef boost::shared_ptr<boost::condition_variable> VariablePtr;

                Condition() throw();

                MutexPtr mutex() const;
                VariablePtr variable() const;

            private:
                typedef boost::shared_ptr<boost::mutex> SharedMutexPtr;
                typedef boost::shared_ptr<boost::condition_variable> SharedVariablePtr;

                SharedMutexPtr _mutex;
                SharedVariablePtr _variable;
        };
    }
}

#endif
