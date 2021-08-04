using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SuperSocket.SocketBase;
using SuperSocket.SocketEngine;

namespace Lobby
{
    class Program
    {
        public static string ServerName { get; private set; }

        public static void SetServerName(string serverName)
        {
            ServerName = serverName;
        }
        static void Main(string[] args)
        {
            if (args == null)
            {
                Console.WriteLine("Error ServerName Args");
                Console.ReadKey();
                return;
            }

           
            SetServerName(args[0]);
            Console.WriteLine("Start Init LobbyServer.({0})", ServerName);

            var bootstrap = BootstrapFactory.CreateBootstrap(); // supersocket 서버 준비

            if (!bootstrap.Initialize())
            {
                Console.WriteLine("LobbyServer Failed to Init");
                Console.ReadKey();
                return;
            }

            var result = bootstrap.Start();
            Console.WriteLine("LobbyServer Start result: {0}!", result);

            if (result == StartResult.Failed)
            {
                Console.WriteLine("LobbyServer Failed to start!");
                Console.ReadKey();
                return;
            }

            Console.WriteLine("Press key 'q' to stop it!");
            while (true)
            {
                switch (Console.ReadKey().KeyChar)
                {
                    case 'q':
                        break;
                    case 'r':
                        continue;
                    case 'd':
                        continue;
                    default:
                        continue;
                }
                break;
            }

            Console.WriteLine();

            bootstrap.Stop();
            Console.WriteLine("The LobbyServer was stopped!");
        }
    }
}
