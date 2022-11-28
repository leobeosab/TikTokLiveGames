using System;
using Nefarius.ViGEm.Client;
using Nefarius.ViGEm.Client.Targets;
using Nefarius.ViGEm.Client.Targets.DualShock4;
using Nefarius.ViGEm.Client.Targets.Xbox360;

namespace ControllerServer;

public class Controller
{
    private ViGEmClient client;
    private IDualShock4Controller controller;
    private uint id = 1;

    public Controller()
    {
        client = new ViGEmClient();
        controller = client.CreateDualShock4Controller();
        controller.Connect();
    }

    public void PressButton(string buttonId)
    {
        buttonId = buttonId.ToUpper();
        switch (buttonId)
        {
            case "A":
                Button(DualShock4Button.Cross);
                break;
            case "B":
                Button(DualShock4Button.Circle);
                break;
            case "START":
                Button(DualShock4Button.Share);
                break;
            case "SELECT":
                Button(DualShock4Button.Options);
                break;
        }
    }

    public void PressDPad(string direction)
    {
        direction = direction.ToUpper();
        switch (direction)
        {
            case "NORTH":
                Dpad(DualShock4DPadDirection.North);
                break;
            case "NORTHWEST":
                Dpad(DualShock4DPadDirection.Northwest);
                break;
            case "NORTHEAST":
                Dpad(DualShock4DPadDirection.Northwest);
                break;
            case "EAST":
                Dpad(DualShock4DPadDirection.East);
                break;
            case "WEST":
                Dpad(DualShock4DPadDirection.West);
                break;
            case "SOUTH":
                Dpad(DualShock4DPadDirection.South);
                break;
            case "SOUTHWEST":
                Dpad(DualShock4DPadDirection.Southwest);
                break;
            case "SOUTHEAST":
                Dpad(DualShock4DPadDirection.Southeast);
                break;
            
        }
    }

    private void Sleep()
    {
        System.Threading.Thread.Sleep(50);
    }

    private void Dpad(DualShock4DPadDirection direction)
    {
        controller.SetDPadDirection(direction);
        Sleep();
        controller.SetDPadDirection(DualShock4DPadDirection.None);
    }

    private void Button(DualShock4Button button)
    {
        controller.SetButtonState(button, true);
        Sleep();
        controller.SetButtonState(button, false);
    }
}
 