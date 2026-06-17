#include <clocale>
#include "TimeTracker.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main()
{
#ifdef _WIN32
   SetConsoleCP(65001);
   SetConsoleOutputCP(65001);
#endif
   setlocale(LC_ALL, ".UTF-8");
   TimeTracker app;
   app.run();
   return 0;
}