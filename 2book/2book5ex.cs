using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2book5ex
{
    internal class Program
    {
        static void Main(string[] args)
        {
            ushort tea;
            ushort cnt_A = 0;
            ushort cnt_L = 0;
            ushort sum = 0;

            Console.Write("Введите количество воды в мл:");
            ushort w = Convert.ToUInt16(Console.ReadLine());

            Console.Write("Введите количество молока в мл:");
            ushort m = Convert.ToUInt16(Console.ReadLine());

            do
            {
                Console.Write("Выберите напиток (1 — американо, 2 — латте): ");
                tea = Convert.ToUInt16(Console.ReadLine());

                if (tea == 1)
                {
                    if (w >= 300)
                    {
                        w -= 300;
                        cnt_A++;
                        sum += 150;
                        Console.WriteLine("Ваш напиток готов");
                    }
                    else
                    {
                        Console.WriteLine("Не хватает воды");
                        Console.WriteLine("Доступен только 2 — латте");
                    }
                }
                else if (tea == 2)
                {
                    if (w >= 30 && m >= 270)
                    {
                        w -= 30;
                        m -= 270;
                        cnt_L++;
                        sum += 170;
                        Console.WriteLine("Ваш напиток готов");
                    }
                    else if (w < 30)
                    {
                        Console.WriteLine("Не хватает воды");
                        Console.WriteLine("Доступен только 1 — американо");
                    }
                    else
                    {
                        Console.WriteLine("Не хватает молока");
                        Console.WriteLine("Доступен только 1 — американо");
                    }
                }
                else
                {
                    Console.WriteLine("Некорректный ввод.");
                }

            } while (w >= 300 || (w >= 30 && m >= 270));

            Console.WriteLine("*Отчёт*");
            Console.WriteLine("Ингредиентов осталось:");
            Console.WriteLine($"    Вода: {w} мл\n    Молоко: {m} мл");
            Console.WriteLine($"Кружек американо приготовлено: {cnt_A}\nКружек латте приготовлено: {cnt_L}");
            Console.WriteLine($"Итого: {sum} рублей.");

        }
    }
}
