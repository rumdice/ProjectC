// See https://aka.ms/new-console-template for more information
// Console.WriteLine("Hello, World!");
// https://grpc.io/docs/languages/csharp/quickstart/ 참조

using Grpc.Core;
using Grpc.Net.Client;
using System;


Channel channel = new Channel("127.0.0.1:7293", ChannelCredentials.Insecure);

//var client = new Greeter.GreeterClient(channel);
//String user = "you";

//var reply = client.SayHello(new HelloRequest { Name = user });
//Console.WriteLine("Greeting: " + reply.Message);

//var secondReply = client.SayHelloAgain(new HelloRequest { Name = user });
//Console.WriteLine("Greeting: " + secondReply.Message);

channel.ShutdownAsync().Wait();
Console.WriteLine("Press any key to exit...");
Console.ReadKey();
