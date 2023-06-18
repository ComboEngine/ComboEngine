using Combo;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Runtime.Source
{
    public class Logger
    {
        public static void Info(string message)
        {
            Bindings.Log_Info(message);
        }
    }
}
