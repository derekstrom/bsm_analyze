// Cut's framework
//
// Created by Samvel Khalatyan, Jun 02, 2011
// Copyright 2011, All rights reserved

#include <boost/pointer_cast.hpp>

#include "interface/Cut.h"

using std::string;

using bsm::Counter;
using bsm::CounterPtr;
using bsm::Cut;
using bsm::LockCounterOnUpdate;

// Counter
//
Counter::Counter():
    _count(0),
    _is_locked(false),
    _is_lock_on_update(false)
{
}

Counter::operator uint32_t() const
{
    return _count;
}

bool Counter::isLocked() const
{
    return _is_locked;
}

bool Counter::isLockOnUpdate() const
{
    return _is_lock_on_update;
}

void Counter::lock()
{
    _is_locked = true;
}

void Counter::lockOnUpdate()
{
    _is_lock_on_update = true;
}

void Counter::unlock()
{
    _is_locked = false;
    _is_lock_on_update = false;
}

void Counter::add()
{
    if (isLocked())
        return;

    ++_count;

    update();
}

uint32_t Counter::id() const
{
    return core::ID<Counter>::get();
}

Counter::ObjectPtr Counter::clone() const
{
    return ObjectPtr(new Counter(*this));
}

void Counter::merge(const ObjectPtr &object_pointer)
{
    if (isLocked()
            || id() != object_pointer->id())
        return;

    boost::shared_ptr<Counter> object =
        boost::dynamic_pointer_cast<Counter>(object_pointer);

    if (!object)
        return;

    _count += object->_count;

    update();
}

void Counter::print(std::ostream &out) const
{
    out << _count;
}

// Private
//
void Counter::update()
{
    if (isLockOnUpdate())
    {
        lock();

        _is_lock_on_update = false;
    }
}



// Cut
//
Cut::Cut(const double &value, const string &name):
    _value(value),
    _name(name),
    _is_disabled(false)
{
    _objects.reset(new Counter());
    _events.reset(new Counter());

    monitor(_objects);
    monitor(_events);
}

Cut::Cut(const Cut &object):
    _value(object._value),
    _name(object._name),
    _is_disabled(object._is_disabled)
{
    _objects.reset(new Counter(*object._objects));
    _events.reset(new Counter(*object._events));

    monitor(_objects);
    monitor(_events);
}

const CounterPtr Cut::objects() const
{
    return _objects;
}

const CounterPtr Cut::events() const
{
    return _events;
}

string Cut::name() const
{
    return _name;
}

void Cut::setName(const string &name)
{
    _name = name;
}

double Cut::value() const
{
    return _value;
}

void Cut::setValue(const double &value)
{
    _value = value;
}

bool Cut::apply(const double &value)
{
    if (isDisabled())
        return true;

    if (!isPass(value))
        return false;

    _objects->add();
    _events->add();

    return true;
}

bool Cut::isDisabled() const
{
    return _is_disabled;
}

void Cut::disable()
{
    _is_disabled = true;
}

void Cut::enable()
{
    _is_disabled = false;
}

uint32_t Cut::id() const
{
    return core::ID<Cut>::get();
}

void Cut::merge(const ObjectPtr &object_pointer)
{
    if (id() != object_pointer->id())
        return;

    boost::shared_ptr<Cut> object =
        boost::dynamic_pointer_cast<Cut>(object_pointer);

    if (!object
            || _value != object->_value
            || _name != object->_name)
        return;

    _is_disabled = object->_is_disabled;

    Object::merge(object_pointer);
}

void Cut::print(std::ostream &out) const
{
    using std::setw;
    using std::left;

    out << setw(5) << left << value() << " ";
   
    if (!isDisabled())
        out << setw(7) << left << *objects()
            << " " << *events();
}



// Lock counter on update
//
LockCounterOnUpdate::LockCounterOnUpdate(const CounterPtr &counter):
    _counter(counter)
{
    _counter->lockOnUpdate();
}

LockCounterOnUpdate::~LockCounterOnUpdate()
{
    _counter->unlock();
}
