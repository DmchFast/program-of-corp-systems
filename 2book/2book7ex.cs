using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2book7ex
{
    internal class Program
    {


        static int mod_w(int w, int a, int d)
        {
            return w / (a + (2 * d));
        }

        static int mod_h(int h, int b, int d)
        {
            return h / (b + (2 * d));
        }

        static void Main(string[] args)
        {
            Console.Write("Введите n: ");
            int n = Convert.ToInt32(Console.ReadLine());
            Console.Write("Введите a: ");
            int a = Convert.ToInt32(Console.ReadLine());
            Console.Write("Введите b: ");
            int b = Convert.ToInt32(Console.ReadLine());
            Console.Write("Введите w: ");
            int w = Convert.ToInt32(Console.ReadLine());
            Console.Write("Введите h: ");
            int h = Convert.ToInt32(Console.ReadLine());

            int d_max = 0;



            for (int d = 0; d < 101; d++)
            {
                int total = mod_w(w, a, d) * mod_h(h, b, d);

                if (total >= n)
                {
                    d_max = d;
                }
                

            }

            Console.WriteLine($"Ответ d = {d_max}");
        }
    }
}
