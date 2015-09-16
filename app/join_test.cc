/* This class will test Thread::join() method. 

There's two basic tests for thread join. Please, compare the output of the
application with the lines below.
	
Test nº 1 should print:
	Joining 0.
	Finished 0.
	Joining 1.
	Finished 1.
	Joining 2.
	Finished 2.

Test nº 2 should print:
	Joining 2.
	Finished 2.
	Joining 1.
	Finished 1.
	Joining 0.
	Finished 0.

Written by Fernando Paladini on 09/07/2015.
*/

#include <utility/ostream.h>
#include <thread.h>
#include <mutex.h>
#include <alarm.h>
#include <display.h>

using namespace EPOS;

Thread * anything[3];
Mutex mutex_display;
OStream cout;

int do_anything(int n) {
	Delay thinking(n * 1000000); 
	return 0;
}

int main() {

	// #### Test nº 1 ####
	cout << endl << "Test #1: " << endl;
	anything[0] = new Thread(&do_anything, 1);
	anything[1] = new Thread(&do_anything, 5);
	anything[2] = new Thread(&do_anything, 3);

	for (int i = 0; i < 3; i++) {
		mutex_display.lock();
		cout << "Joining " << i << "." << endl;
		mutex_display.unlock();
		
		anything[i]->join();

		mutex_display.lock();
		cout << "Finished " << i << "." << endl;
		mutex_display.unlock();

        delete anything[i];
	}


	// #### Test nº 2 ####
	cout << endl << "Test #2: " << endl;
	anything[0] = new Thread(&do_anything, 1);
	anything[1] = new Thread(&do_anything, 5);
	anything[2] = new Thread(&do_anything, 3);

	for (int i = 2; i >= 0; i--) {
		mutex_display.lock();
		cout << "Joining " << i << "." << endl;
		mutex_display.unlock();
		
		anything[i]->join();

		mutex_display.lock();
		cout << "Finished " << i << "." << endl;
		mutex_display.unlock();

        delete anything[i];
	}

	cout << "I'm done!" << endl;

	return 0;
}
