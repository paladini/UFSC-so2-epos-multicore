// EPOS Alarm Abstraction Test Program

#include <utility/ostream.h>
#include <alarm.h>
#include <system.h>

using namespace EPOS;

const int iterations = 1;

void func_a(void);
void func_b(void);
void func_c(void);

OStream cout;

int main()
{
    cout << "Alarm test" << endl;

    cout << "I'm the first thread of the first task created in the system." << endl;
    cout << "I'll now create two alarms and put myself in a delay ..." << endl;

    Function_Handler handler_a(&func_a);
    Alarm* alarm_a = new (UNCACHED) Alarm(2000000, &handler_a, iterations);

    Function_Handler handler_b(&func_b);
    Alarm* alarm_b = new Alarm(1000000, &handler_b, iterations);

    Function_Handler handler_c(&func_c);
    Alarm* alarm_c = new (SYSTEM) Alarm(1000000, &handler_c, iterations);

    // Note that in case of idle-waiting, this thread will go into suspend
    // and the alarm handlers above will trigger the functions in the context
    // of the idle thread!
    Alarm::delay(2000000 * (iterations + 2));

    delete alarm_a;
    delete alarm_b;
    delete alarm_c;
    cout << "I'm done, bye!" << endl;

    return 0;
}

void func_a()
{
    for(int i = 0; i < 79; i++)
        cout << "a";
    cout << endl;
}

void func_b(void)
{
    for(int i = 0; i < 79; i++)
        cout << "b";
    cout << endl;
}

void func_c(void)
{
    for(int i = 0; i < 79; i++)
        cout << "c";
    cout << endl;
}
