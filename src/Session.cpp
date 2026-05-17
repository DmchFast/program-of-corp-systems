#include "Session.h"
#include <sstream>
#include <iomanip>

using namespace std;

Session::Session() : isActive(true)
{
   startTime = chrono::system_clock::now(); //todo фиксация текущего времени начала
}

void Session::stop()
{
   if (isActive)
   {
      endTime = chrono::system_clock::now();
      isActive = false;
   }
}

long long Session::getDuration() const
{
   if (isActive)
   {
      auto now = chrono::system_clock::now();
      return chrono::duration_cast<chrono::seconds>(now - startTime).count();
   }
   return chrono::duration_cast<chrono::seconds>(endTime - startTime).count();
}

bool Session::isFinished() const { return !isActive; }

string Session::getStartTimeStr() const
{
   //todo формат даты/времени: ДД-ММ-ГГГГ ЧЧ:ММ:СС
   auto tt = chrono::system_clock::to_time_t(startTime);
   stringstream ss;
   ss << put_time(localtime(&tt), "%d-%m-%Y %H:%M:%S");
   return ss.str();
}

string Session::getEndTimeStr() const
{
   if (isActive)
      return "";
   auto tt = chrono::system_clock::to_time_t(endTime);
   stringstream ss;
   ss << put_time(localtime(&tt), "%d-%m-%Y %H:%M:%S");
   return ss.str();
}

void Session::setStartTime(const string &str)
{
   tm tm = {};
   stringstream ss(str);
   ss >> get_time(&tm, "%d-%m-%Y %H:%M:%S");
   if (!ss.fail())
   {
      startTime = chrono::system_clock::from_time_t(mktime(&tm));
      isActive = true;
   }
}

void Session::setEndTime(const string &str)
{
   if (str.empty())
      return;
   tm tm = {};
   stringstream ss(str);
   ss >> get_time(&tm, "%d-%m-%Y %H:%M:%S");
   if (!ss.fail())
   {
      endTime = chrono::system_clock::from_time_t(mktime(&tm));
      isActive = false;
   }
}