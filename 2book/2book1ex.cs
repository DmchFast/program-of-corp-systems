using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Schema;

namespace _2book1ex
{
    internal class Program
    {
        static void Main(string[] args)
        {

            double x;
            do
            {
                Console.WriteLine("Ввод x в пределах (-1,1):");
                x = Convert.ToDouble(Console.ReadLine());
                if (x <= -1 || x >= 1)
                {
                    Console.WriteLine("Некорректный ввод");
                }
            }
            while (x <= -1 || x >= 1);

            double e;
            do
            {
                Console.WriteLine("Ввод e < 0.01:");
                e = Convert.ToDouble(Console.ReadLine());
                if (e >= 0.01 || e <= 0)
                {
                    Console.WriteLine("Некорректный ввод");
                }
            }
            while (e >= 0.01 || e <= 0);


            double sum = 0;
            int n = 0;
            double formula;

            do
            {
                // знак (-1)^n
                int sign = (n % 2 == 0) ? 1 : -1;

                // x^(2n+1)
                double power = 1;
                int exponent = 2 * n + 1;
                for (int i = 0; i < exponent; i++)
                {
                    power *= x;
                }

                // (2n - 1)!!
                int numeratorFact = 1;
                for (int i = 2 * n - 1; i > 0; i -= 2)
                {
                    numeratorFact *= i;
                }

                // (2n)!!
                int denominatorFact = 1;
                for (int i = 2 * n; i > 0; i -= 2)
                {
                    denominatorFact *= i;
                }

                // n-й член ряда
                formula = sign * numeratorFact * power / (denominatorFact * (2 * n + 1));

                sum += formula;
                n++;

            } while (Math.Abs(formula) >= e);

            Console.WriteLine($"f(x) с точностью e ={e}: {sum}");
            Console.WriteLine($"n-ый ряд при x = {x}, n = {n}: {formula}");
        }
    }
}
