using System;
using System.Threading;

namespace CSharpStudy1
{
    class Program
    {
        static void PrintNums()
        {
            Console.WriteLine("Start..");
            for (int i = 1; i < 10; i++) 
            {
                Console.WriteLine(i);
            }
        }


        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            var t = new Thread(PrintNums);
            t.Start();
            PrintNums();
        }
    }
}
