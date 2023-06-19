using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Combo;
using System.Threading.Tasks;
using Runtime.Source;

namespace Game
{
    public class Counter : Component
    {
        public int Test = 0;
        public override void Update()
        {
            Logger.Info("Test " + Test.ToString());
        }
    }
}
