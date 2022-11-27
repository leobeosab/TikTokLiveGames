using System;
using vJoyInterfaceWrap;

namespace ControllerServer;

public class Controller
{
    private vJoy joystick;
    private uint id = 1;
    
    public Controller()
    {
            joystick = new vJoy();

            // Get the driver attributes (Vendor ID, Product ID, Version Number)
            if (!joystick.vJoyEnabled())
            {
                Console.WriteLine("vJoy driver not enabled: Failed Getting vJoy attributes.\n");
                return;
            }
            
            Console.WriteLine("Vendor: {0}\nProduct :{1}\nVersion Number:{2}\n", joystick.GetvJoyManufacturerString(), joystick.GetvJoyProductString(), joystick.GetvJoySerialNumberString());

            // Get the state of the requested device
            VjdStat status = joystick.GetVJDStatus(id);
            switch (status)
            {
                case VjdStat.VJD_STAT_OWN:
                    Console.WriteLine("vJoy Device {0} is already owned by this feeder\n", id);
                    break;
                case VjdStat.VJD_STAT_FREE:
                    Console.WriteLine("vJoy Device {0} is free\n", id);
                    break;
                case VjdStat.VJD_STAT_BUSY:
                    Console.WriteLine("vJoy Device {0} is already owned by another feeder\nCannot continue\n", id);
                    return;
                case VjdStat.VJD_STAT_MISS:
                    Console.WriteLine("vJoy Device {0} is not installed or disabled\nCannot continue\n", id);
                    return;
                default:
                    Console.WriteLine("vJoy Device {0} general error\nCannot continue\n", id);
                    return;
            };

            // Test if DLL matches the driver
            UInt32 DllVer = 0, DrvVer = 0;
            bool match = joystick.DriverMatch(ref DllVer, ref DrvVer);
            if (match)
            {
                
                Console.WriteLine("Version of Driver Matches DLL Version ({0:X})\n", DllVer);
            }
            else
            {
                Console.WriteLine("Version of Driver ({0:X}) does NOT match DLL Version ({1:X})\n", DrvVer, DllVer);
            }


            // Acquire the target
            if ((status == VjdStat.VJD_STAT_OWN) || ((status == VjdStat.VJD_STAT_FREE) && (!joystick.AcquireVJD(id))))
            {
                Console.WriteLine("Failed to acquire vJoy device number {0}.\n", id);
                return ;
            }
            
            Console.WriteLine("Acquired: vJoy device number {0}.\n", id);

            long maxval = 0;

            joystick.GetVJDAxisMax(id, HID_USAGES.HID_USAGE_X, ref maxval);
            joystick.ResetVJD(id);
    }

    public void PressButton(uint buttonId)
    {
        joystick.SetBtn(true, id, buttonId);
        System.Threading.Thread.Sleep(50);
        joystick.SetBtn(false, id, buttonId);
    }
}