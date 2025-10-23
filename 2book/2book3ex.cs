using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2book3ex
{
    internal class Program
    {
        static int NOD(int m, int n)
        {
            while (n != 0)
            {
                int r = m % n;
                m = n;
                n = r;
            }
            return m;
        }

        static void Main(string[] args)
        {
            int m;
            Console.Write("Введите числитель:");
            m = Convert.ToInt32(Console.ReadLine());

            int n;
            Console.Write("Введите знаменатель:");
            n = Convert.ToInt32(Console.ReadLine());

            int nod = NOD(m, n);

         
            m = m / nod;
            n = n / nod;


            if (n < 0)
            {
                n = -n;
                m = -m;
            }

            Console.WriteLine($"Результат: {m} / {n}");
        }
    }
}
