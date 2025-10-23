using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2book4ex
{
    internal class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Загадайте число от 0 до 63");
            short my_num = Convert.ToInt16(Console.ReadLine());

            int L = 0;
            int H = 63;
            int cnt = 0;

            while (L <= H)
            {
                int mid = (L + H) / 2;
                Console.WriteLine($"Ваше число больше {mid}? (1 - да, 0 - нет, 2 - равно)");
                string input = Console.ReadLine();

                if (input == "1")
                {
                    L = mid + 1;
                }
                else if (input == "0")
                {
                    H = mid - 1;
                }
                else if (input == "2")
                {
                    cnt++;
                    Console.WriteLine($"Ваше число {mid}. Угадано за {cnt} попыток!");
                    return;
                }
                else
                {
                    Console.WriteLine("Введите 1 (да), 0 (нет) или 2 (равно).");
                    continue;
                }

                cnt++;
            }

            Console.WriteLine("Не удалось определить число.");

        }
    }
}
