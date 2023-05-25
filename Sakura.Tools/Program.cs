using System;
using Sakura.Tools;
using System.Reflection;
using System.Collections.Generic;

namespace Sakura.Tools {
    public class Program {
        public static List<Sakura.BuildTools.BuildInfo> targets;
        public static void Main(string[] args) {
            ArgumentParser parser = new ArgumentParser();
            Options options = parser.accept<Options>(args);
            
            switch(options.type) {
                case "build":
                    targets = BuildTargetAssembler.generateAssembliesFromTargets();

                    if(options.name == null) {
                        Console.WriteLine("Building all...");
                    } else {
                        Console.WriteLine("Building " + options.name + "...");
                    }
                    break;
            }
        }
    }
}