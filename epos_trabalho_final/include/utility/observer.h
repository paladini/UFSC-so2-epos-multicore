// EPOS Observer Utility Declarations

// Observation about the lack of virtual destructors in the following classes:
// Observed x Observer is used in mediators, so they appear very early in the system.
// To be more precise, they are used in SETUP, where we cannot yet handle a heap.
// Since the purpose of the destructors is only to trace the classes, we accepted to
// declare them as non-virtual. But it must be clear that this is one of the few uses
// for them.

#ifndef __observer_h
#define	__observer_h

#include <utility/list.h>

__BEGIN_UTIL

// Observer x Observed
class Observer;

class Observed
{ 
    friend class Observer;

private:
    typedef Simple_List<Observer>::Element Element;

protected:
    Observed() {
        db<Observed>(TRC) << "Observed() => " << this << endl;
    }

public: 
    ~Observed() {
        db<Observed>(TRC) << "~Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Observer * o);
    virtual void detach(Observer * o);
    virtual bool notify();

private: 
    Simple_List<Observer> _observers;
}; 

class Observer
{ 
    friend class Observed;

protected: 
    Observer(): _link(this) {
        db<Observer>(TRC) << "Observer() => " << this << endl;
    }

public: 
    ~Observer() {
        db<Observer>(TRC) << "~Observer(this=" << this << ")" << endl;
    }

    virtual void update(Observed * o) = 0;

private:
    Observed::Element _link;
};

inline void Observed::attach(Observer * o)
{
    db<Observed>(TRC) << "Observed::attach(obs=" << o << ")" << endl;

    _observers.insert(&o->_link);
}

inline void Observed::detach(Observer * o)
{
    db<Observed>(TRC) << "Observed::detach(obs=" << o << ")" << endl;

    _observers.remove(&o->_link);
}

inline bool Observed::notify()
{
    bool notified = false;

    db<Observed>(TRC) << "Observed::notify()" << endl;

    for(Element * e = _observers.head(); e; e = e->next()) {
        db<Observed>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;

        e->object()->update(this);
        notified = true;
    }

    return notified;
}


// Observer x Conditionally Observed
template<typename T = int>
class Conditional_Observer;

template<typename T = int>
class Conditionally_Observed
{
    friend class Conditional_Observer<T>;

private:
    typedef typename Simple_Ordered_List<Conditional_Observer<T>, T>::Element Element;

public:
    typedef T Observing_Condition;

protected:
    Conditionally_Observed() {
        db<Observed>(TRC) << "Observed() => " << this << endl;
    }

public:
    ~Conditionally_Observed() {
        db<Observed>(TRC) << "~Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Conditional_Observer<T> * o, T c);
    virtual void detach(Conditional_Observer<T> * o, T c);
    virtual bool notify(T c);

private: 
    Simple_Ordered_List<Conditional_Observer<T>, T> _observers;
}; 

template<typename T>
class Conditional_Observer
{
    friend class Conditionally_Observed<T>;

public:
    typedef T Observing_Condition;

public:
    Conditional_Observer(): _link(this) {
        db<Observer>(TRC) << "Observer() => " << this << endl;
    } 

    ~Conditional_Observer() {
        db<Observer>(TRC) << "~Observer(this=" << this << ")" << endl;
    }
    
    virtual void update(Conditionally_Observed<T> * o, T c) = 0;

private:
    typename Conditionally_Observed<T>::Element _link;
};

template<typename T>
inline void Conditionally_Observed<T>::attach(Conditional_Observer<T> * o, T c)
{
    db<Observed>(TRC) << "Observed::attach(o=" << o << ",c=" << c << ")" << endl;

    o->_link = Element(o, c);
    _observers.insert(&o->_link);
}

template<typename T>
inline void Conditionally_Observed<T>::detach(Conditional_Observer<T> * o, T c)
{
    db<Observed>(TRC) << "Observed::detach(obs=" << o << ",c=" << c << ")" << endl;

    _observers.remove(&o->_link);
}

template<typename T>
inline bool Conditionally_Observed<T>::notify(T c)
{
    bool notified = false;

    db<Observed>(TRC) << "Observed::notify(cond=" << hex << c << ")" << endl;

    for(Element * e = _observers.head(); e; e = e->next()) {
        if(e->rank() == c) {
            db<Observed>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;
            e->object()->update(this, c);
            notified = true;
        }
    }

    return notified;
}


// Observer x Conditionally Observed with Data
template<typename T1, typename T2 = int>
class Data_Observer;

template<typename T1, typename T2 = int>
class Data_Observed
{
    friend class Data_Observer<T1, T2>;

private:
    typedef Data_Observer<T1, T2> Observer;
    typedef typename Simple_Ordered_List<Data_Observer<T1, T2>, T2>::Element Element;

public:
    typedef T2 Observing_Condition;

public:
    Data_Observed() {
        db<Observed>(TRC) << "Observed() => " << this << endl;
    }

    ~Data_Observed() {
        db<Observed>(TRC) << "~Observed(this=" << this << ")" << endl;
    }

    virtual void attach(Data_Observer<T1, T2> * o, T2 c) {
        db<Observed>(TRC) << "Observed::attach(obs=" << o << ",cond=" << c << ")" << endl;

        o->_link = Element(o, c);
        _observers.insert(&o->_link);
    }

    virtual void detach(Data_Observer<T1, T2> * o, T2 c) {
        db<Observed>(TRC) << "Observed::detach(obs=" << o << ",cond=" << c << ")" << endl;

        _observers.remove(&o->_link);
    }

    virtual bool notify(T2 c, T1 * d) {
        bool notified = false;

        db<Observed>(TRC) << "Observed::notify(this=" << this << ",cond=" << c << ")" << endl;

        for(Element * e = _observers.head(); e; e = e->next()) {
            if(e->rank() == c) {
                db<Observed>(INF) << "Observed::notify(this=" << this << ",obs=" << e->object() << ")" << endl;
                e->object()->update(this, c, d);
                notified = true;
            }
        }

        return notified;
    }

private:
    Simple_Ordered_List<Data_Observer<T1, T2>, T2> _observers;
};

template<typename T1, typename T2>
class Data_Observer
{
    friend class Data_Observed<T1, T2>;

public:
    typedef T2 Observing_Condition;

public:
    Data_Observer(): _link(this) {
        db<Observer>(TRC) << "Observer() => " << this << endl;
    }

    ~Data_Observer() {
        db<Observer>(TRC) << "~Observer(this=" << this << ")" << endl;
    }

    virtual void update(Data_Observed<T1, T2> * o, T2 c, T1 * d) = 0;

private:
    typename Data_Observed<T1, T2>::Element _link;
};

__END_UTIL
 
#endif
