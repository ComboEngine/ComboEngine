using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Combo
{
    public class Bindings
    {
        public delegate void ReadyCallback();
        [DllImport("Combo.Engine.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int Core_Init();

        [DllImport("Combo.Engine.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int Platform_CSPreInit();

        [DllImport("Combo.Engine.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static int Interop_AddBeginPlayCallback(IntPtr callback);
        [DllImport("Combo.Engine.dll", CallingConvention = CallingConvention.Cdecl)]
        public extern static void Log_Info(string message);

    }
}
