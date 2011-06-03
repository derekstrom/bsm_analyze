// Threads Base
//
// All threads should inherit from this base class
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#ifndef BSM_THREAD
#define BSM_THREAD

#include <queue>
#include <stack>
#include <string>

#include <boost/shared_ptr.hpp>

#include "interface/bsm_fwd.h"
#include "bsm_core/interface/bsm_core_fwd.h"
#include "bsm_core/interface/Thread.h"

namespace bsm
{
    class Reader;
    class ThreadController;

    typedef boost::shared_ptr<Analyzer> AnalyzerPtr;

    // Keyaboard Thread: watch for keyboard input and report to the controller
    //
    class KeyboardOperation : public core::Operation
    {
        public:
            enum Command
            {
                QUIT = 0,
                INFO = 1,
                HELP = 2
            };

            KeyboardOperation();

            void init(ThreadController *);

            // Operation interface
            //
            virtual void run();
            virtual void stop();

            virtual void onThreadInit(core::Thread *);

        private:
            bool isContinue() const;

            core::Thread *_thread;
            ThreadController *_thread_controller;

            bool _continue;

            boost::shared_ptr<core::Keyboard> _keyboard_controller;
    };

    // Analyzer Thread: perform the analysis
    //
    class AnalyzerOperation : public core::Operation,
                                public core::RunLoopDelegate
    {
        public:
            AnalyzerOperation();
            virtual ~AnalyzerOperation();

            // Controller and Analyzer can only be set when thread
            // is not running
            //
            void use(ThreadController *controller);
            void use(const AnalyzerPtr &analyzer);

            AnalyzerPtr analyzer() const;

            // Scheule file for processing. Method does nothing is file
            // is already set but processing didn't start
            bool init(const std::string &file_name);

            // Operation interface
            //
            virtual void run();
            virtual void stop();

            virtual void onThreadInit(core::Thread *);

            // RunLoop Delegate interface
            //
            virtual void onRunLoopCommand(const uint32_t &);

            uint32_t eventsProcessed() const;
            uint32_t totalEventsSize() const;

        private:
            typedef boost::shared_ptr<Reader> ReaderPtr;

            core::Thread *thread() const;

            bool isRunning() const;
            // isContinue is called only when thread is running and therefore
            // uses lock
            //
            bool isContinue() const;
            bool isFileEmpty() const;

            // hasAnalyzer/Controller are only called when thread is running.
            // Therefore lock is safe for use
            //
            bool hasAnalyzer() const;
            bool hasController() const;

            // Create input file reader and reset input_file
            //
            ReaderPtr createReader();

            // Create input file reader and apply analyzer to events
            //
            void processFile();

            // Wait for new instructions from Controller
            //
            void waitForInstructions();

            core::Thread *_thread;
            ThreadController *_controller;

            bool _continue;

            AnalyzerPtr _analyzer;
            std::string _file_name;

            uint32_t _events_processed;
            uint32_t _total_events_size;
    };

    class ThreadController
    {
        public:
            ThreadController();
            ~ThreadController();

            core::ConditionPtr condition() const;

            void use(const AnalyzerPtr &analyzer);

            // Schedule file for processing
            //
            void push(const std::string &file_name);

            // Start processing scheduled files
            //
            void start();

            void threadIsWaiting(core::Thread *);

            void quit();
            void info();

        private:
            // Test if any input files left for processing
            //
            bool hasInputFiles() const;
            bool hasAnalyzer() const;

            // Return maximum number of threads to be created:
            //  min(CORES, Input FILES)
            //
            uint32_t countMaxThreads();

            // Create new thread, instruct and start
            //
            void addThread();

            void instruct(AnalyzerOperation *operation);

            void run();
            void wait();

            bool isRunning() const;

            void onThreadWait();
            core::Thread *waitingThread();

            void startKeyboardThread();
            void stopKeyboardThread();

            // Typedefs
            //
            typedef std::queue<std::string> InputFiles; // FIFO

            typedef boost::shared_ptr<core::Thread> ThreadPtr;

            typedef std::map<core::Thread *, ThreadPtr> Threads;
            typedef std::queue<core::Thread *> ThreadsFIFO;

            typedef boost::shared_ptr<ThreadsFIFO> ThreadsFIFOPtr;

            // Properties
            //
            const uint32_t _max_threads;

            core::ConditionPtr _condition;
            boost::shared_ptr<InputFiles> _input_files;

            uint32_t _processed_files;

            Threads _threads;
            ThreadsFIFOPtr _threads_waiting;
            ThreadPtr _keyboard_thread;

            AnalyzerPtr _analyzer;

            class Summary;

            boost::shared_ptr<Summary> _summary;
    };
}

#endif
