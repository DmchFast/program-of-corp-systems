using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace _2book8ex
{
    internal class Program
    {
        static void input_M(int n, int m, int[,] matrix)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    Console.Write($"Ввод значений на позицию ({i},{j}):");
                    matrix[i, j] = Convert.ToInt32(Console.ReadLine());
                }
            }
        }

        static void output_M(int n, int m, int[,] matrix)
        {
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    Console.Write(matrix[i, j] + "\t");
                }
                Console.WriteLine();
            }
        }

        static void input_MRND(int n, int m, int a, int b, int[,] matrix)
        {
            Random rnd = new Random();
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    matrix[i, j] = rnd.Next(a, b+1);
                }
            }
            output_M(n, m, matrix);
            //Console.WriteLine();
        }

        static void sum_M(int n, int m, int n1, int m1, int[,] matrix, int[,] matrix1)
        {
            if ((n == n1) && (m == m1))
            {
                int[,] res = new int[n, m];
                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < m; j++)
                    {
                        res[i, j] = matrix[i, j] + matrix1[i, j];
                    }
                }
                Console.WriteLine("Сложение матриц:");
                output_M(n1, m1, res);
                
            }
            else Console.WriteLine("Невозможность сложения матриц по причине несоответствия их размерностей.");
        }

        static void mult_M(int n, int m, int n1, int m1, int[,] matrix, int[,] matrix1)
        {
            if (m == n1)
            {
                int[,] res = new int[n, m1];
                for (int i = 0; i < n; i++)
                {
                    for (int j = 0; j < m1; j++)
                    {
                        for (int k = 0; k < m; k++)
                        {
                            res[i, j] += matrix[i, k] * matrix1[k, j];
                        }
                    }
                }
                Console.WriteLine("Умножение матриц:");
                output_M(n, m1, res);
                
            }
            else Console.WriteLine("Невозможность умножения матриц в связи с их несовместимостью.");
        }

        static int det_M(int n, int m, int[,] matrix)
        {
            if (n != m)
            {
                Console.WriteLine("Детерминант не льзя посчитать.");
                return 0;
            }

            if (n == 1)
                return matrix[0, 0];

            if (n == 2)
                return matrix[0, 0] * matrix[1, 1] - matrix[0, 1] * matrix[1, 0];

            int det = 0;
            for (int p = 0; p < n; p++)
            {
                int[,] subMatrix = new int[n - 1, n - 1];

                for (int i = 1; i < n; i++)
                {
                    int colIndex = 0;
                    for (int j = 0; j < n; j++)
                    {
                        if (j == p) continue;
                        subMatrix[i - 1, colIndex] = matrix[i, j];
                        colIndex++;
                    }
                }

                int sign = (p % 2 == 0) ? 1 : -1;
                det += sign * matrix[0, p] * det_M(n - 1, n - 1, subMatrix);
            }

            return det;
        }

        static double[,] inverse_M(int n, int[,] matrix)
        {
            int det = det_M(n, n, matrix);
            if (det == 0)
            {
                Console.WriteLine("Обратная матрица не существует (детерминант равен 0).");
                return null;
            }

            double[,] inv = new double[n, n];

            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    int[,] minor = new int[n - 1, n - 1];
                    int mi = 0;

                    for (int r = 0; r < n; r++)
                    {
                        if (r == i) continue;
                        int mj = 0;
                        for (int c = 0; c < n; c++)
                        {
                            if (c == j) continue;
                            minor[mi, mj] = matrix[r, c];
                            mj++;
                        }
                        mi++;
                    }

                    int sign = ((i + j) % 2 == 0) ? 1 : -1;
                    inv[j, i] = sign * det_M(n - 1, n - 1, minor) / (double)det; // Транспонирование сразу
                }
            }

            Console.WriteLine("Обратная матрица:");
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    Console.Write($"{inv[i, j]:F2}\t");
                }
                Console.WriteLine();
            }

            Console.WriteLine();
            return inv;
        }

        static void trans_M(int n, int m, int[,] matrix)
        {
            int[,] trans = new int[m, n];

            for (int i = 0; i < n; i++)
                for (int j = 0; j < m; j++)
                    trans[j, i] = matrix[i, j];

            Console.WriteLine("Транспонирование матрицы:");
            output_M(m, n, trans);
            
        }

        static void solve_SLAU(int n, int[,] A, double[] b)
        {
            double[,] invA = inverse_M(n, A);
            if (invA == null) return;

            double[] x = new double[n];

            for (int i = 0; i < n; i++)
            {
                x[i] = 0;
                for (int j = 0; j < n; j++)
                {
                    x[i] += invA[i, j] * b[j];
                }
            }

            Console.WriteLine("Решение системы (вектор x):");
            for (int i = 0; i < n; i++)
            {
                Console.WriteLine($"x{i + 1} = {x[i]:F2}");
            }
            Console.WriteLine();
        }

        static void Main(string[] args)
        {
            //1
            Console.WriteLine("Создание матрицы размерности n*m");
            Console.Write("n:");
            int n = Convert.ToInt32(Console.ReadLine());
            Console.Write("m:");
            int m = Convert.ToInt32(Console.ReadLine());

            int[,] matrix = new int[n, m];

            //2
            input_M(n, m, matrix);
            output_M(n, m, matrix);

            Console.WriteLine("**************");
            //3
            Console.WriteLine("Создание матрицы размерности n*m");
            Console.Write("n:");
            int n1 = Convert.ToInt32(Console.ReadLine());
            Console.Write("m:");
            int m1 = Convert.ToInt32(Console.ReadLine());

            int[,] matrix1 = new int[n1, m1];

            Console.WriteLine("Диапазон случайных чисел [a; b]");
            Console.Write("a:");
            int a = Convert.ToInt32(Console.ReadLine());
            Console.Write("b:");
            int b = Convert.ToInt32(Console.ReadLine());

            input_MRND(n1, m1, a, b, matrix1);
            
            Console.WriteLine("***************");
            //4
            sum_M(n, m, n1, m1, matrix, matrix1);

            //5
            mult_M(n, m, n1, m1, matrix, matrix1);
            Console.WriteLine("***************");
            //6
            Console.WriteLine($"{det_M(n, m, matrix)}\n");
            //7
            if (n == m)
            {
                Console.WriteLine("Нахождение обратной матрицы:");
                inverse_M(n, matrix);
            }
            else
            {
                Console.WriteLine("Невозможно найти обратную матрицу, так как она не квадратная.");
            }
            //8
            trans_M(n, m, matrix);

            //9
            if (n == m)
            {
                int det = det_M(n, n, matrix);
                if (det == 0)
                {
                    Console.WriteLine("Система не имеет единственного решения (детерминант равен 0).");
                }
                else
                {
                    double[] bVec = new double[n];
                    Console.WriteLine("Введите вектор правых частей b:");
                    for (int i = 0; i < n; i++)
                    {
                        Console.Write($"b[{i + 1}] = ");
                        bVec[i] = Convert.ToDouble(Console.ReadLine());
                    }

                    solve_SLAU(n, matrix, bVec);
                }
            }
            else
            {
                Console.WriteLine("Невозможно решить СЛАУ: матрица коэффициентов должна быть квадратной.");
            }


        }
    }
}
