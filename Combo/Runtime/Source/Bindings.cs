using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Combo
{

    public enum TaskType
    {
        GetCSClasses
    };
    public class Bindings
    {
        public const string DllPath = @"D:\ComboEngine\Combo\Engine\Binary\Combo.Engine.dll";

        public delegate void BeginPlayCallback();
        public delegate void UpdateCallback();
        public delegate void DrawCallback();
        public delegate void ExitCallback();
        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public extern static int Core_Init();

        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Platform_CSPreInit();

        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Interop_AddBeginPlayCallback(IntPtr callback);
        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Interop_AddUpdateCallback(IntPtr callback);
        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Interop_AddDrawCallback(IntPtr callback);
        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Interop_AddExitCallback(IntPtr callback);
        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public extern static void Log_Info(string message);

        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Interop_ExposeComponents(string[] components,int arraySize);

        [DllImport(DllPath, CallingConvention = CallingConvention.Cdecl)]
        public static extern string[] Interop_GetUpdateComponentsList();

    }
}
