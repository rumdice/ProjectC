using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace CSharpStudy
{
    class Coffee
    {
        public Coffee() { }
    }

    class Egg
    {
    }

    class Bacon
    {
    }

    class Toast
    {
    }

    class Juice
    {

    }

    class Program
    {
        static async Task Main(string[] args)
        {
            var cup = PourCoffee();
            Console.WriteLine("coffee is ready");

            var eggsTask = FryEggs(2);
            var baconTask = FryBacon(3);
            var toastTask = MakeToastWithButterAndJam(2);

            var allTasks = new List<Task> { eggsTask, baconTask, toastTask};
            while (allTasks.Any())
            {
                var finished = await Task.WhenAny(allTasks);
                if (finished == eggsTask)
                {
                    Console.WriteLine("eggs is ready");
                }
                else if (finished == baconTask)
                {
                    Console.WriteLine("bacon is ready");
                }
                else if (finished == toastTask)
                {
                    Console.WriteLine("toast is ready");
                }
                allTasks.Remove(finished);
            }

            var oj = PourOrangeJuice();
            Console.WriteLine("oj is ready");

            Console.WriteLine("breakfast is ready");
        }

        private static Juice PourOrangeJuice()
        {
            var juice = new Juice();
            return juice;
        }

        private static async Task<Toast> MakeToastWithButterAndJam(int num)
        {
            var toast = await ToastBread(num);
            ApplyButter(toast);
            ApplyJam(toast);
            return toast;
        }

        private static bool ApplyJam(Toast toast)
        {
            // TODO: Apply Jam to Toast
            return true;
        }

        private static bool ApplyButter(Toast toast)
        {
            // TODO: Apply Butter to Toast
            return true;
        }

        private static Task<Toast> ToastBread(int num)
        {
            var taskToast = new Task<Toast>(() =>
            {
                var toasts = new Toast();
                return toasts;
            });

            taskToast.Start();

            return taskToast;
        }

        private static Task<Bacon> FryBacon(int num)
        {
            var taskBacon = new Task<Bacon>(() =>
            {
                var bacons = new Bacon();
                return bacons;
            });

            taskBacon.Start();

            return taskBacon;
        }

        private static Task<Egg> FryEggs(int num)
        {
            // TODO: num 만큼 만드는 작업 추가

            var taskEgg = new Task<Egg>(() =>
            {
                var eggs = new Egg();
                return eggs;
            });


            taskEgg.Start();

            return taskEgg;
        }

        private static Coffee PourCoffee()
        {
            return null;
        }
    }
}

// 혼자서 아침식사 준비하기 - 병렬로 수행 되지 않는 비동기 작업
// 요리에 익숙한 사람은 
// 커피를 올려놓고 준비되길 기다리면서
// 펜을 대우고 계란 후라이를 올리고
// 베이컨을 튀긴다
// 빵을 토스터기에 넣어놓고
// 토스트에 버터와 잼을 바르고
// 오렌지 주스 한잔 따름


// 병렬 알고리즘 은 여러 요리사 (스레드)가 필요하다
// 한사람은 계란만 후라이 하고
// 또 한사람은 베이컨만 굽는다
// 각 사람은 요리가 완성 되기전까지 독립적이다 (동기적으로 차단 됨)

// https://docs.microsoft.com/ko-kr/dotnet/csharp/programming-guide/concepts/async/