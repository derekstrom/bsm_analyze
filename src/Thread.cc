// Threads Base
//
// All threads should inherit from this base class
//
// Created by Samvel Khalatyan, Apr 30, 2011
// Copyright 2011, All rights reserved

#include <iostream>

#include <boost/pointer_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "bsm_input/interface/Event.pb.h"
#include "bsm_input/interface/Reader.h"
#include "bsm_core/interface/Keyboard.h"

#include "interface/Analyzer.h"
#include "interface/Thread.h"

using namespace std;

using boost::shared_ptr;

using bsm::AnalyzerPtr;
using bsm::KeyboardOperation;
using bsm::AnalyzerOperation;
using bsm::ThreadController;

using bsm::core::Lock;
using bsm::core::Thread;

// Keyboard Thread
//
KeyboardOperation::KeyboardOperation():
    _continue(true)
{
    _thread = 0;
    _keyboard_controller.reset(new core::Keyboard());
}

void KeyboardOperation::run()
{
    if (!_thread)
        return;

    for(boost::posix_time::milliseconds delay(100);
            isContinue();
            boost::this_thread::sleep(delay))
    {
    }
}

void KeyboardOperation::stop()
{
    Lock lock(_thread->condition());

    _continue = false;
}

void KeyboardOperation::onThreadInit(Thread *thread)
{
    _thread = thread;
}

// Private
//
bool KeyboardOperation::isContinue() const
{
    Lock lock(_thread->condition());

    return _continue;
}



// Analyzer Thread
//
AnalyzerOperation::AnalyzerOperation():
    _continue(true)
{
    _thread = 0;
    _controller = 0;
}

AnalyzerOperation::~AnalyzerOperation()
{
    // Check if thread is still working
}

void AnalyzerOperation::use(ThreadController *controller)
{
    if (isRunning())
        return;

    _controller = controller;
}

void AnalyzerOperation::use(const AnalyzerPtr &analyzer)
{
    if (isRunning())
        return;

    _analyzer = analyzer;
}

AnalyzerPtr AnalyzerOperation::analyzer() const
{
    return _analyzer;
}

bool AnalyzerOperation::init(const std::string &file_name)
{
    if (file_name.empty())
        return false;

    if (thread())
    {
        if (!isFileEmpty())
            return false;

        Lock lock(thread()->condition());
        _file_name = file_name;
    }
    else
    {
        if (!_file_name.empty())
            return false;

        _file_name = file_name;
    }

    return true;
}

void AnalyzerOperation::run()
{
    if (!thread()
            || !hasAnalyzer()
            || !hasController())
        return;

    for(; isContinue();)
    {
        // Process file
        //
        processFile();

        // Wait for instructions
        //
        waitForInstructions();
    }
}

void AnalyzerOperation::stop()
{
    Lock lock(thread()->condition());

    _continue = false;
}

void AnalyzerOperation::onThreadInit(Thread *thread)
{
    _thread = thread;
}

Thread *AnalyzerOperation::thread() const
{
    return _thread;
}

// Privates
//
bool AnalyzerOperation::isRunning() const
{
    return thread()
        && thread()->isRunning();
}

bool AnalyzerOperation::isContinue() const
{
    Lock lock(thread()->condition());
    
    return _continue;
}

bool AnalyzerOperation::isFileEmpty() const
{
    Lock lock(thread()->condition());

    return _file_name.empty();
}

bool AnalyzerOperation::hasAnalyzer() const
{
    Lock lock(thread()->condition());

    return _analyzer;
}

bool AnalyzerOperation::hasController() const
{
    Lock lock(thread()->condition());

    return _controller;
}

AnalyzerOperation::ReaderPtr AnalyzerOperation::createReader()
{
    Lock lock(thread()->condition());

    ReaderPtr reader(new Reader(_file_name));
    _file_name.clear();

    return reader;
}

void AnalyzerOperation::processFile()
{
    if (isFileEmpty())
        return;

    ReaderPtr reader = createReader();
    for(shared_ptr<Event> event(new Event());
            isContinue()
            && reader->read(*event);
            event->Clear())
    {
        Lock lock(thread()->condition());

        _analyzer->process(event.get());
    }
}

void AnalyzerOperation::waitForInstructions()
{
    Lock lock(thread()->condition());

    // Inform the Controller that thread finished analyzing the  input file 
    //
    _controller->threadIsWaiting(thread());
    _controller->condition()->variable()->notify_all();

    // isContinue() and isFileEmpty() methods can not be used. Otherwise
    // interlock will occure. Controller will try to accquire mutex and then
    // notify this thread. Notification should come only after wait on
    // lock (lines below) is issued
    //
    while(_file_name.empty()
            && _continue)
    {
        thread()->condition()->variable()->wait(lock());
    }
}



// Thread controller
//
ThreadController::ThreadController():
    _max_threads(boost::thread::hardware_concurrency())
{
    _condition.reset(new core::Condition());
    _input_files.reset(new InputFiles());

    _threads_waiting.reset(new ThreadsFIFO());
}

ThreadController::~ThreadController()
{
    // Check if any threads are still running and let them finish
    // before quit application
}

bsm::core::ConditionPtr ThreadController::condition() const
{
    return _condition;
}

void ThreadController::use(const AnalyzerPtr &analyzer)
{
    _analyzer = analyzer;
}

void ThreadController::push(const std::string &file_name)
{
    Lock lock(condition());

    _input_files->push(file_name);
}

void ThreadController::start()
{
    if (!hasInputFiles()
            || !hasAnalyzer())
        return;

    for(uint32_t threads_to_create = countMaxThreads();
            threads_to_create;
            --threads_to_create)
    {
        addThread();
    }

    run();
}

void 
ThreadController::threadIsWaiting(Thread *thread)
{
    _threads_waiting->push(thread);
}

// Private
//
bool ThreadController::hasInputFiles() const
{
    Lock lock(condition());

    return !_input_files->empty();
}

bool ThreadController::hasAnalyzer() const
{
    Lock lock(condition());

    return _analyzer;
}

uint32_t ThreadController::countMaxThreads()
{
    Lock lock(condition());

    if (_max_threads > _input_files->size())
        return _input_files->size();

    return _max_threads;
}

void ThreadController::addThread()
{
    ThreadPtr thread(new Thread());
    AnalyzerOperationPtr operation(new AnalyzerOperation());
    thread->init(operation);

    operation->use(this);

    instruct(operation.get());

    {
        Lock lock(condition());
        operation->use(_analyzer->clone());
        _threads[thread.get()] = thread;
    }

    thread->start();
}

void ThreadController::instruct(AnalyzerOperation *operation)
{
    Lock lock(condition());

    const std::string input_file(_input_files->front());
    _input_files->pop();

    operation->init(input_file);
}

void ThreadController::run()
{
    for(; isRunning();)
    {
        // Wait for any thread to finish
        //
        wait();

        // Process waiting threads
        //
        onThreadWait();
    }
}

void ThreadController::wait()
{
    Lock lock(condition());

    while(_threads_waiting->empty())
    {
        condition()->variable()->wait(lock());
    }
}

bool ThreadController::isRunning() const
{
    Lock lock(condition());

    return !_threads.empty();
}

void ThreadController::onThreadWait()
{
    using boost::dynamic_pointer_cast;

    if (hasInputFiles())
    {
        // More input files left
        //
        Thread *thread = waitingThread();
        AnalyzerOperationPtr operation = 
            dynamic_pointer_cast<AnalyzerOperation>(thread->operation());
        
        if (operation)
            instruct(operation.get());

        thread->condition()->variable()->notify_all();
    }
    else
    {
        // Stop thread
        //
        Thread *thread = waitingThread();

        thread->stop();
        thread->condition()->variable()->notify_all();

        // Let thread finish
        //
        thread->join();

        AnalyzerOperationPtr operation =
            dynamic_pointer_cast<AnalyzerOperation>(thread->operation());

        if (operation)
            _analyzer->merge(operation->analyzer());

        // Remove thread form the list of running threads
        //
        _threads.erase(thread);
    }
}

Thread *ThreadController::waitingThread()
{
    Lock lock(condition());
    Thread *thread = _threads_waiting->front();
    _threads_waiting->pop();
    
    return thread;
}
