using System;
using System.Net.WebSockets;
using System.Threading;
using System.Threading.Tasks;

namespace ChatTester
{
    class Program
    {
        static void Main(string[] args)
        {
            var t = TestClient();
            t.Wait();
        }

        static async Task TestClient()
        {
            var ws = new ClientWebSocket();
            var uri = new Uri("ws://localhost:8080");

            // connect to server
            await ws.ConnectAsync(uri, CancellationToken.None);

            var conn = (Convert.ToString(ws.State) != "Open") ? "Fail!" : "Success";
            Console.WriteLine($"Url:{uri}, Connect:{conn}");

            // 패킷 실행
            while (true)
            {
                Console.WriteLine("Input Packet Header (exit is end) :"); // 커맨드 입력 -> 해당 컨텐츠 테스트

                var cmd = Console.ReadLine();
                await PacketHandler.PacketProcAsync(ws, cmd);

                if (cmd == "exit")
                {
                    // disconnect to server
                    await ws.CloseAsync(WebSocketCloseStatus.NormalClosure, "Disconnect Normal", CancellationToken.None);
                    Console.WriteLine("Client Disconnect");

                    Console.WriteLine("Match Test End");
                    break;
                }
            }
        }
    }
}
