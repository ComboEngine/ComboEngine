using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Combo
{
    public class Interop
    {
        public static void ExposeComponents()
        {
            List<string> components = new List<string>();
            
            foreach(Type type in Assembly.GetExecutingAssembly().GetTypes()) {
                if(type.IsSubclassOf(typeof(Component)))
                {
                    components.Add(type.Name);
                }
            }

            Bindings.Interop_ExposeComponents(components.ToArray(),components.Count);
        }
    }
}
