using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Combo
{
    public class Events
    {
        public static void AddBeginPlayEvent(Action action)
        {
            Bindings.ReadyCallback callback = new Bindings.ReadyCallback(action);
            Bindings.Interop_AddBeginPlayCallback(Marshal.GetFunctionPointerForDelegate(callback));
        }
    }
}
