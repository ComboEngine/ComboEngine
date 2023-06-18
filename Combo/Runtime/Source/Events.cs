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
            Bindings.BeginPlayCallback callback = new Bindings.BeginPlayCallback(action);
            Bindings.Interop_AddBeginPlayCallback(Marshal.GetFunctionPointerForDelegate(callback));
        }

        public static void AddUpdateEvent(Action action)
        {
            Bindings.UpdateCallback callback = new Bindings.UpdateCallback(action);
            Bindings.Interop_AddUpdateCallback(Marshal.GetFunctionPointerForDelegate(callback));
        }

        public static void AddDrawEvent(Action action)
        {
            Bindings.DrawCallback callback = new Bindings.DrawCallback(action);
            Bindings.Interop_AddDrawCallback(Marshal.GetFunctionPointerForDelegate(callback));
        }
        public static void AddExitEvent(Action action)
        {
            Bindings.ExitCallback callback = new Bindings.ExitCallback(action);
            Bindings.Interop_AddExitCallback(Marshal.GetFunctionPointerForDelegate(callback));
        }
    }
}
