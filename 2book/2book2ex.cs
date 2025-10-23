using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2book2ex
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int str;
            do
            {
                Console.Write("Введите номер билета:");
                str  = Convert.ToInt32(Console.ReadLine());
                if (str < 100000 || str > 999999)
                {
                    Console.WriteLine("Некорректный ввод:");
                }
            } while (str < 100000 || str > 999999);

            int x1 = (str / 100000) + (str / 10000 % 10) + (str / 1000 % 10);
            int x2 = (str / 100 % 10) + (str / 10 % 10) + (str % 10);

            if (x1 == x2)
            {
                Console.WriteLine("True");
            }
            else Console.WriteLine("False");
        }
    }
}
