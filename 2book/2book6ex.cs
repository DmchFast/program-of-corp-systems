using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2book6ex
{
    internal class Program
    {
        static ushort n_ost(ushort n, ushort kill_n)
        {
            if (kill_n > n)
            {
                n = 0;
            }
            else n -= kill_n;
            return n;
        }
        static void Main(string[] args)
        {
            Console.Write("Введите количество бактерий:");
            ushort n = Convert.ToUInt16(Console.ReadLine());

            Console.Write("Введите количество антибиотика:");
            ushort x = Convert.ToUInt16(Console.ReadLine());

            ushort cnt_hour = 0;

            do
            {
                cnt_hour++;

                ushort new_n = (ushort)(n * 2);

                ushort power = (ushort)(10 - (cnt_hour - 1));

                if (power == 0) break;
                ushort kill_n = (ushort)(x * power);

                n = n_ost(new_n, kill_n);

                Console.WriteLine($"После {cnt_hour} часа бактерий осталось {n}");

            } while (n > 0);

            Console.WriteLine("Эксперимент завершён.");
        }
    }
}
