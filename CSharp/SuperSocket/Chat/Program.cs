using SuperSocket.SocketEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Chat
{
    class Program
    {
        static void Main()
        {
            var bootstrap = BootstrapFactory.CreateBootstrap();

            if (bootstrap == null)
            {
                return;
            }

            if (!bootstrap.Initialize())
            {
                Console.WriteLine("LobbyServer Failed to Init");
                Console.ReadKey();
                return;
            }

        }
    }
}
