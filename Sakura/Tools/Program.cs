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

                        foreach(KeyValuePair<string,Sakura.BuildTools.BuildTarget> entry in targets) {
                            Console.WriteLine("Building " + entry.Value.GetName());
                            Compiler.CompileElement(entry.Key,entry.Value);
                        }
                    } else {
                        Sakura.BuildTools.BuildTarget finalTarget = null;
                        string path = null;
                        foreach(KeyValuePair<string,Sakura.BuildTools.BuildTarget> entry in targets) {
                            if(string.Equals(entry.Value.GetName(), options.name, StringComparison.OrdinalIgnoreCase)) {
                                finalTarget = entry.Value;
                                path = entry.Key;
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
                case "setup":
                    if(options.name != null) {
                        SakuraProjectGenerator.Setup(options.name);
                        Console.WriteLine("Generated project " + options.name);
                    } else {
                        Console.WriteLine("Name is required argument!");
                    }
                    break;

            }
        }
    }
}