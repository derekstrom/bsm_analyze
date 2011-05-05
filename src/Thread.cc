// Threads Base
//
// All threads should inherit from this base class
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#include <iostream>
#include <iterator>
#include <stdexcept>

#include <boost/date_time/posix_time/posix_time.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Input.pb.h"
#include "bsm_input/interface/Reader.h"
#include "bsm_keyboard/interface/KeyboardController.h"

#include "interface/Thread.h"

using std::bad_alloc;
using std::cerr;
using std::cout;
using std::distance;
using std::endl;

using bsm::core::AnalyzerThread;
using bsm::core::Condition;
using bsm::core::ConditionPtr;
using bsm::core::KeyboardThread;
using bsm::core::Lock;
using bsm::core::Thread;
using bsm::core::ThreadController;

// ThreadController class
//
ThreadController::ThreadController(const uint32_t &max_threads) throw()
try:
    _max_threads(boost::thread::hardware_concurrency()),
    _running_threads(0)
{
    // Max threads are limited by hardware Core(s)
    //
    if (max_threads
            && max_threads < _max_threads)

        _max_threads = max_threads;

    _condition.reset(new Condition());

    _next_file = _input_files.begin();
}
catch(const bad_alloc &error)
{
    cerr << "ThreadController: failed to allocate memory - " << error.what()
        << endl;
}

ThreadController::~ThreadController()
{
}

ConditionPtr ThreadController::condition() const
{
    return _condition;
}

void ThreadController::process(const Files &input_files)
{
    if (!condition())
        return;

    if (!init(input_files))
        return;

    run();
    reset();
}

void ThreadController::notify(AnalyzerThread *thread)
{
    Lock lock(condition());

    _complete_threads.push(thread);
}

void ThreadController::notify(const Command &command)
{
    Lock lock(condition());

    switch(command)
    {
        case QUIT:  // quit application
                    //
                    for(Threads::iterator thread = _threads.begin();
                            _threads.end() != thread;
                            ++thread)
                    {
                        (*thread)->stop();
                    }
                    _next_file = _input_files.end();

                    //_keyboard_thread->stop();

                    break;

        case HELP:  // Print help
                    //
                    break;

        case STATUS:    // Print status
                        //
                        cout << "Finished " << distance(Files::const_iterator(_input_files.begin()),
                                                        _next_file)
                            << "/" << _input_files.size() << " files" << endl;

                        break;

        default:    // Unknown command
                    //
                    break;
    }
}

// Privates
//
bool ThreadController::init(const Files &input_files)
{
    Lock lock(condition());

    if (_running_threads
            || input_files.empty())

        return false;

    _input_files = input_files;
    _next_file = _input_files.begin();

    createThreads();
    startThreads();

    return true;
}

void ThreadController::reset()
{
    Lock lock(condition());

    _keyboard_thread->stop();
    _keyboard_thread->condition()->variable()->notify_all();
    _keyboard_thread->join();

    _keyboard_thread.reset();

    // Reset object
    //
    _running_threads = 0;
    _complete_threads = ThreadsFIFO();
    _threads.clear();

    _input_files.clear();
    _next_file = _input_files.begin();
}

void ThreadController::createThreads()
{
    uint32_t max_threads = _max_threads;

    if (_input_files.size() < _max_threads)
        max_threads = _input_files.size();

    for(uint32_t i = 0; max_threads > i; ++i)
    {
        AnalyzerThreadPtr thread(new AnalyzerThread(this));

        _threads.push_back(thread);
    }

    _keyboard_thread.reset(new KeyboardThread(this));
}

void ThreadController::startThreads()
{
    for(Threads::iterator thread = _threads.begin();
            _threads.end() != thread;
            ++thread)
    {
        AnalyzerThreadPtr thread_ptr = *thread;

        // Pass input file to thread
        //
        thread_ptr->init(*_next_file);

        if (!thread_ptr->start())
            continue;

        ++_next_file;
        ++_running_threads;
    }

    _keyboard_thread->start();
}

void ThreadController::run()
{
    while(_running_threads)
    {
        wait();

        onThreadComplete();
    }
} 

void ThreadController::wait()
{
    Lock lock(condition());
    while(_complete_threads.empty())
        condition()->variable()->wait(lock());
}

void ThreadController::onThreadComplete()
{
    Lock lock(condition());
    while(!_complete_threads.empty())
    {
        if (_input_files.end() != _next_file)
            continueThread();
        else
            stopThread();
    }
}

void ThreadController::continueThread()
{
    AnalyzerThread *thread = _complete_threads.front();
    _complete_threads.pop();

    // Pass input file to thread
    //
    thread->init(*_next_file);
    ++_next_file;

    thread->condition()->variable()->notify_all();
}

void ThreadController::stopThread()
{
    Thread *thread = _complete_threads.front();
    _complete_threads.pop();

    thread->stop();
    thread->condition()->variable()->notify_all();
    thread->join();

    --_running_threads;
}



// Thread class
//
Thread::Thread(ThreadController *controller) throw()
try
    : _controller(controller)
{
    _condition.reset(new Condition());
}
catch(const bad_alloc &error)
{
    cerr << "Thread: failed to allocate memory - " << error.what() << endl;
}

Thread::~Thread()
{
}

ConditionPtr Thread::condition() const
{
    return _condition;
}

bool Thread::start()
{
    if (boost::thread() != _thread)
        return false;

    _thread = boost::thread(&Thread::run, this);

    return true;
}

void Thread::join()
{
    if (boost::thread() != _thread)
        _thread.join();
}

// Protected
//
ThreadController *Thread::controller() const
{
    return _controller;
}



// Analyzer Thread
//
AnalyzerThread::AnalyzerThread(ThreadController *controller):
    Thread(controller),
    _events_read(0),
    _events_processed(0)
{
}

void AnalyzerThread::init(const std::string &file_name)
{
    Lock lock(condition());

    _reader.reset(new Reader(file_name));

    _events_read = 0;

    _continue = true;
    _wait_for_instructions = false;
}

void AnalyzerThread::stop()
{
    Lock lock(condition());

    _continue = false;
    _wait_for_instructions = false;
}

// Protected
//
void AnalyzerThread::run()
{
    for(_wait_for_instructions = true;
            _continue;
            _wait_for_instructions = true)
    {
        // Process file
        //
        if (_reader)
        {
            for(boost::shared_ptr<Event> event(new Event());
                    _continue && _reader->read(*event);
                    ++_events_read)
            {
                event->Clear();
            }
        }

        // Notify Controller that Thread has finished the analysis
        //
        controller()->notify(this);

        // Wait for new instructions
        //
        wait();
    }
}

// Private
//
void AnalyzerThread::wait()
{
    Lock lock(condition());

    controller()->condition()->variable()->notify_all();
    while(_wait_for_instructions)
    {
        controller()->condition()->variable()->notify_all();
        condition()->variable()->wait(lock());
    }
}



// Keyboard Thread
//
KeyboardThread::KeyboardThread(ThreadController *controller) throw():
    Thread(controller),
    _continue(true)
{
    _controller.reset(new KeyboardController());
}

void KeyboardThread::stop()
{
    Lock lock(condition());

    _continue = false;
}

void KeyboardThread::run()
{
    // Update frequency: every 100 ms
    //
    boost::posix_time::milliseconds delay(100);
    for(char key = 0; _continue; boost::this_thread::sleep(delay))
    {
        key = _controller->get();

        switch(key)
        {
            case 'h': // help
                      //
                      controller()->notify(ThreadController::HELP);

                      continue;

            case 's': // status
                      //
                      controller()->notify(ThreadController::STATUS);

                      continue;

            case 'q': // quit
                      //
                      controller()->notify(ThreadController::QUIT);

                      break;

            default: continue;
        }

        break;
    }
}



// Lock
//
Lock::Lock(const ConditionPtr &condition):
    _lock(*condition->mutex())
{
}

Lock::UniqueLock &Lock::operator()()
{
    return _lock;
}




// Condition
//
Condition::Condition() throw()
try
{
    _mutex.reset(new boost::mutex());
    _variable.reset(new boost::condition_variable());
}
catch(const std::bad_alloc &error)
{
    cerr << "Condition: failed to allocate memory - " << error.what() << endl;

    _mutex.reset();
    _variable.reset();
}

Condition::MutexPtr Condition::mutex() const
{
    return _mutex;
}

Condition::VariablePtr Condition::variable() const
{
    return _variable;
}
