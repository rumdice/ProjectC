using Newtonsoft.Json;
using System;
using System.Text;
using System.IO;
using System.Net.WebSockets;
using System.Threading;
using System.Threading.Tasks;
using ChatPacketStruct;

namespace ChatTester
{
    public static class PacketHandler
    {
        public static Task Send(ClientWebSocket ws, string data, CancellationToken cancellation)
        {
            var encoded = Encoding.UTF8.GetBytes(data);
            var buffer = new ArraySegment<byte>(encoded, 0, encoded.Length);
            return ws.SendAsync(buffer, WebSocketMessageType.Text, true, cancellation);
        }

        public static async Task<string> Recv(ClientWebSocket ws)
        {
            var buffer = new ArraySegment<byte>(new byte[8192]);
            using (var ms = new MemoryStream())
            {
                WebSocketReceiveResult result;
                do
                {
                    result = await ws.ReceiveAsync(buffer, CancellationToken.None);
                    ms.Write(buffer.Array, buffer.Offset, result.Count);
                }
                while (!result.EndOfMessage);

                ms.Seek(0, SeekOrigin.Begin);

                using var reader = new StreamReader(ms, Encoding.UTF8);
                return reader.ReadToEnd();
            }
        }

        public static async Task PacketProcAsync(ClientWebSocket ws, string cmd)
        {
            string packetStr = "";

            switch (cmd)
            {
                case "login":
                    {
                        var reqbody = new RequestBodyLogin
                        {
                            userUid = "aabbccdd123",
                        };

                        var req = new RequestLogin
                        {
                            body = reqbody,
                        };

                        packetStr = JsonConvert.SerializeObject(req);
                        break;
                    }

                case "test":
                    {
                        break;
                    }

                case "chat":
                    {
                        Console.WriteLine("Input Chat msg:");
                        var msg = Console.ReadLine();

                        var reqbody = new RequestBodyChat
                        {

                        };

                        var req = new RequestChat
                        {
                            body = reqbody,
                        };

                        packetStr = JsonConvert.SerializeObject(req);
                        break;
                    }

                case "":
                    {
                        break;
                    }

                default:
                    break;
            }

            await Send(ws, packetStr, CancellationToken.None);
            Console.WriteLine("[Send] MSG:{0}", JsonConvert.ToString(packetStr));

            var recv = await Recv(ws);
            Console.WriteLine("[Recv] MSG:{0}", recv);
        }
    }
}
