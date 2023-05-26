using System;
using Sakura.Tools;
using System.Reflection;
using System.Linq;
using System.Collections.Generic;

namespace Sakura.Tools {
    public class Program {
        public static Dictionary<String,Sakura.BuildTools.BuildTarget> targets;
        public static void Main(string[] args) {
            ArgumentParser parser = new ArgumentParser();
            Options options = parser.accept<Options>(args);
            Console.WriteLine(
"░██████╗░█████╗░██╗░░██╗██╗░░░██╗██████╗░░█████╗░\n" +
"██╔════╝██╔══██╗██║░██╔╝██║░░░██║██╔══██╗██╔══██╗\n" +
"╚█████╗░███████║█████═╝░██║░░░██║██████╔╝███████║\n" +
"░╚═══██╗██╔══██║██╔═██╗░██║░░░██║██╔══██╗██╔══██║\n" +
"██████╔╝██║░░██║██║░╚██╗╚██████╔╝██║░░██║██║░░██║\n" +
"╚═════╝░╚═╝░░╚═╝╚═╝░░╚═╝░╚═════╝░╚═╝░░╚═╝╚═╝░░╚═╝");    


            switch(options.type) {
                case "build":
                    targets = BuildTargetAssembler.generateAssembliesFromTargets();

                    if(options.name == null) {
                        Console.WriteLine("Building all...");
                    } else {
                        Sakura.BuildTools.BuildTarget finalTarget = null;
                        foreach(Sakura.BuildTools.BuildTarget target in targets.Values.ToList()) {
                            if(string.Equals(target.GetName(), options.name, StringComparison.OrdinalIgnoreCase)) {
                                finalTarget = target;
                            }
                        }

                        if(finalTarget == null) {
                            Console.WriteLine("ERROR: Unable to find target with this name");
                            return;
                        }

                        Console.WriteLine("Building " + finalTarget.GetName() + "");
                    }
                    break;
                case "projects":
                    targets = BuildTargetAssembler.generateAssembliesFromTargets();
                    PremakeScriptBuilder.createPremake5Scripts(targets);
                    Console.WriteLine("Generated projects");
                    break;
            }
        }
    }
}