using System;
using System.Net;
using System.Text;

namespace ControllerServer;

public class HttpServer
{

    public int Port = 80;
    private HttpListener _listener;
    private static Controller controller;

    public void Start()
    {
        controller = new Controller();
        _listener = new HttpListener();
        _listener.Prefixes.Add("http://*:" + Port + "/");
        _listener.Start();
        Receive();
    }

    private void Receive()
    {
        _listener.BeginGetContext(new AsyncCallback(ListenerCallBack), _listener);
    }

    private void ListenerCallBack(IAsyncResult result)
    {
        if (!_listener.IsListening)
            return;
        
        
        var ctx = _listener.EndGetContext(result);
        var req = ctx.Request;
        
        if (req.HttpMethod == "POST" && req.Url.AbsolutePath.Contains("/button/"))
        {
            var arr = req.Url.AbsolutePath.Split('/');
            string buttonSt = arr[arr.Length - 1];
            controller.PressButton(buttonSt);
        }
        
        if (req.HttpMethod == "POST" && req.Url.AbsolutePath.Contains("/dpad/"))
        {
            var arr = req.Url.AbsolutePath.Split('/');
            string padSt = arr[arr.Length - 1];
            controller.PressDPad(padSt);
        }
        
        Respond(ctx.Response, "okay");
        Receive();
    }

    private void Respond(HttpListenerResponse resp, string response)
    {
        string message = "{ \"message\": \"" + response + "\"}";

        resp.ContentType = "text/html";
        resp.ContentEncoding = Encoding.UTF8;
        
        resp.Headers.Add("Access-Control-Allow-Origin", "*");
        resp.Headers.Add("Access-Control-Allow-Methods", "POST, GET");

        resp.OutputStream.Write(Encoding.UTF8.GetBytes(message), 0, message.Length);
        resp.Close();
    }
    
}