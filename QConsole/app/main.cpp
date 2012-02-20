#include <QtGui/QApplication>
#include <FooInjector.h>
#include <Foo.h>
#include <Console.h>
#include "qconsole.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QConsole w;

    // Inject sample Foo object
    FooInjector *injector = new FooInjector;
    w.introduce(injector);
    w.inject((void*) new Foo(), "foo",
             injector);
    w.show();
    return a.exec();
}
