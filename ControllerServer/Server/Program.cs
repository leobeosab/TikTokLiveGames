
using System;
using System.Diagnostics;

namespace ControllerServer
{
    class Program
    {
        
    private static bool _keepRunning = true;

        static void Main(string[] args)
        {
            Console.CancelKeyPress += delegate(object sender, ConsoleCancelEventArgs e)
            {
                e.Cancel = true;
                Program._keepRunning = false;
            };
    
            Console.WriteLine("Welcome");
            HttpServer server = new HttpServer();
            server.Start();
            
            while (Program._keepRunning) { }
        }
    } // class Program
} // namespace FeederDemoCS
