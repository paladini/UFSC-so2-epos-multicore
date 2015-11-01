// EPOS Semaphore Abstraction Test Program

#include <utility/ostream.h>
#include <thread.h>
#include <mutex.h>
#include <semaphore.h>
#include <alarm.h>
#include <display.h>
#include <architecture/ia32/cpu.h>

using namespace EPOS;

const int iterations = 10;

Mutex table;

Thread * phil[5];
Semaphore * chopstick[5];

OStream cout;

void countDelay(int delay_ms){
    unsigned long iterations = delay_ms * (CPU::clock() / 1000);
	for(int i; i < iterations; i++) {
        asm("");
	}
}

int philosopher(int n, int l, int c)
{
    int first = (n < 4)? n : 0;
    int second = (n < 4)? n + 1 : 4;

    for(int i = iterations; i > 0; i--) {

        table.lock();
        cout << "Philosopher # "<< n << " is thinking on CPU# " << Machine::cpu_id() << endl;
        table.unlock();

        countDelay(500);

        chopstick[first]->p();   // get first chopstick
        chopstick[second]->p();   // get second chopstick

        table.lock();
        cout << "Philosopher # "<< n << " is eating on CPU# " << Machine::cpu_id() << endl;
        table.unlock();

        countDelay(500);

        chopstick[first]->v();   // release first chopstick
        chopstick[second]->v();   // release second chopstick
    }

    table.lock();
    cout << "Philosopher " << n << " done  on CPU#" << Machine::cpu_id() << endl;
    table.unlock();

    return iterations;
}

int main()
{
    table.lock();
    Display::clear();
    Display::position(0, 0);
    cout << "The Philosopher's Dinner: on #" << Machine::cpu_id() << endl;

    for(int i = 0; i < 5; i++)
        chopstick[i] = new Semaphore;

    phil[0] = new Thread(&philosopher, 0,  5, 32);
    phil[1] = new Thread(&philosopher, 1, 10, 44);
    phil[2] = new Thread(&philosopher, 2, 16, 39);
    phil[3] = new Thread(&philosopher, 3, 16, 24);
    phil[4] = new Thread(&philosopher, 4, 10, 20);

    cout << "Philosophers are alive and angry! (on CPU# " << Machine::cpu_id() << endl;

    cout << "The dinner is served ... on Table#" << Machine::cpu_id() << endl;
    table.unlock();

    for(int i = 0; i < 5; i++) {
        int ret = phil[i]->join();
        table.lock();
        cout << "Philosopher " << i << " ate " << ret << " times (on #" << Machine::cpu_id() << ")" << endl;
        table.unlock();
    }

    for(int i = 0; i < 5; i++)
        delete chopstick[i];
    for(int i = 0; i < 5; i++)
        delete phil[i];

    cout << "Dinna is Ova! on Table#" << Machine::cpu_id() << endl;

    return 0;
}
